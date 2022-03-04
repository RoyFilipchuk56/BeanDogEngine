#include "GLCommon.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <sstream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "GlobalItems.h"
#include "EntitySystem.h"
#include "random_helpers.h"
#include "QuaternionHelpers.h"

#include "ObjectReader.h"

#include "cParticleContactGenerators.h"
#include "particle_force_generators.h"
#include "cParticleConstraints.h"


//Draw send in cMesh
void DrawObject(cMesh* pCurrentMesh, glm::mat4 matModel, GLint matModel_Location, GLint matModelInverseTranspose_Location, GLuint program, cVAOManager* pVAOManager);
//Load The meshes
void LoadMeshes(GLuint program);

// Global Variables
float deltaTime;

//ObjectReader
ObjectLoader objectLoader;

//calls the latest error
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main()
{
    GLFWwindow* window;
    //Set program to 0 since we dont start with a shader
    GLuint program = 0;
    //Default to -1 since thats the error
    GLint mvp_location = -1;
    //set current time
    float previousTime = static_cast<float>(glfwGetTime());

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        return -1;
        //exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1280, 720, "BeanDog Engine v1.0", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, GLFW_key_callback);
    // NEW: 
    glfwSetCursorEnterCallback(window, GLFW_cursor_enter_callback);
    glfwSetCursorPosCallback(window, GLFW_cursor_position_callback);
    glfwSetScrollCallback(window, GLFW_scroll_callback);
    glfwSetMouseButtonCallback(window, GLFW_mouse_button_callback);
    //This crashes and i have no clue to why 
    //glfwSetWindowSizeCallback(window, GLFW_window_size_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& imIo = ImGui::GetIO();
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer bindings
    if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
    {
        std::cout << "GUI FAILED" << std::endl;
    }

    if (!ImGui_ImplOpenGL3_Init())
    {
        std::cout << "GUI FAILED PT 2" << std::endl;
    }

    //Set all the globals 
    StartUp(window);

    //Load the scene
    if (gScene->LoadSceneFromXML("SceneOne.xml"))
    {
        std::cout << "Scene loaded in good" << std::endl;
    }
    else
    {
        std::cout << "Scene loaded in bad" << std::endl;
    }

    //Set camera eye (AKA Location)
    g_pFlyCamera->setEye(gScene->currentLevel.camera.tranform);

    //Load the shaders
    cShaderManager::cShader vertShader;
    vertShader.fileName = SHADER_DIR;
    vertShader.fileName.append("vertShader_01.glsl");

    cShaderManager::cShader fragShader;
    fragShader.fileName = SHADER_DIR;
    fragShader.fileName.append("fragShader_01.glsl");

    if (gShaderManager->createProgramFromFile("Shader#1", vertShader, fragShader))
    {
        std::cout << "Shader compiled OK" << std::endl;
        // 
        // Set the "program" variable to the one the Shader Manager used...
        program = gShaderManager->getIDFromFriendlyName("Shader#1");
    }
    else
    {
        std::cout << "Error making shader program: " << std::endl;
        std::cout << gShaderManager->getLastError() << std::endl;
    }

    //We only use one shader so just set it
    glUseProgram(program);

    //TODO: Remove, Trying to get the fbo to work
    cShaderManager::cShaderProgram* pShaderProc = gShaderManager->pGetShaderProgramFromFriendlyName("Shader#1");

    int theUniformIDLoc = -1;
    theUniformIDLoc = glGetUniformLocation(program, "matModel");
    pShaderProc->mapUniformName_to_UniformLocation["matModel"] = theUniformIDLoc;

    pShaderProc->mapUniformName_to_UniformLocation["matModel"] = glGetUniformLocation(program, "matModel");
    pShaderProc->mapUniformName_to_UniformLocation["matView"] = glGetUniformLocation(program, "matView");
    pShaderProc->mapUniformName_to_UniformLocation["matProjection"] = glGetUniformLocation(program, "matProjection");
    pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"] = glGetUniformLocation(program, "matModelInverseTranspose");

    pShaderProc->mapUniformName_to_UniformLocation["wholeObjectSpecularColour"] = glGetUniformLocation(program, "wholeObjectSpecularColour");

    //Get the mvp location
    mvp_location = glGetUniformLocation(program, "MVP");
    // Get "uniform locations" (aka the registers these are in)
    GLint matModel_Location = glGetUniformLocation(program, "matModel");
    GLint matView_Location = glGetUniformLocation(program, "matView");
    GLint matProjection_Location = glGetUniformLocation(program, "matProjection");
    GLint matModelInverseTranspose_Location = glGetUniformLocation(program, "matModelInverseTranspose");
    
    // Get the uniform locations of the light shader values
    gTheLights->SetUpUniformLocations(program);

    //Start of Meshes
    // 
    //load each model into the vaoManager
    for (int i = 0; i < gScene->currentLevel.meshsToLoad.size(); i++)
    {
        //make a temp model info
        sModelDrawInfo tempInfo;
        if (!gVAOManager->LoadModelIntoVAO(MODEL_DIR + gScene->currentLevel.meshsToLoad[i], tempInfo, program))
        {
            std::cout << "Error: " << gScene->currentLevel.meshsToLoad[i] << " Didn't load OK" << std::endl;
        }
        else
        {
            std::cout << "Good: " << gScene->currentLevel.meshsToLoad[i] << " loaded OK" << std::endl;
            std::cout << tempInfo.numberOfVertices << " vertices loaded" << std::endl;
            std::cout << tempInfo.numberOfTriangles << " triangles loaded" << std::endl;
        }
    }

    //load in each model in the scene
    for (int i = 0; i < gScene->currentLevel.models.size(); i++)
    {
        bool isTransparent = false;
        //make a temp mesh and load in all the attributes
        cMesh* tempMesh = new cMesh;
        tempMesh->meshName = MODEL_DIR + gScene->currentLevel.models[i].fileName;
        tempMesh->meshFriendlyName = gScene->currentLevel.models[i].fileName;
        tempMesh->transformXYZ = gScene->currentLevel.models[i].transform;
        tempMesh->rotationXYZ = gScene->currentLevel.models[i].rotation;
        tempMesh->rotationXYZQuat = quaternion::QuatFromAngles(gScene->currentLevel.models[i].rotation);
        tempMesh->scale = gScene->currentLevel.models[i].scale;
        tempMesh->scaleXYZ = glm::vec3(gScene->currentLevel.models[i].scale, gScene->currentLevel.models[i].scale, gScene->currentLevel.models[i].scale);
        tempMesh->alphaTransparency = gScene->currentLevel.models[i].transparency;
        if (tempMesh->alphaTransparency < 1.0f)
        {
            isTransparent = true;
        }
        
        if (gScene->currentLevel.models[i].textures.size() > 0)
        {
            for (int j = 0; j < gScene->currentLevel.models[i].textures.size(); j++)
            {
                //Get the unit number to simplify things
                int unitNumber = gScene->currentLevel.models[i].textures[j].textureUnit;
                
                if (unitNumber == 4)
                {
                    tempMesh->bHasDecal = true;
                }
                //check if an object is transparent
                else if (unitNumber == 7)
                {
                    isTransparent = true;
                }
                //load the texture into the proper texture unit
                tempMesh->textureNames[unitNumber] = gScene->currentLevel.models[i].textures[j].texName;
                tempMesh->textureRatios[unitNumber] = gScene->currentLevel.models[i].textures[j].ratio;
            }
        }


        //Add to lists to be drawn
        if (isTransparent)
        {
            g_transMeshes.push_back(tempMesh);
        }
        else
        {
            g_vecMeshes.push_back(tempMesh);
        }
        //Add the entity top the actual list
        GameObject* obj = new GameObject(tempMesh);
        nGameObject::AddGameObject(obj);
    }

    LoadMeshes(program);

    //Create a mesh debug sphere
    g_pDebugSphere = new cMesh;
    g_pDebugSphere->meshName = MODEL_DIR + std::string("WhiteBall.ply");
    g_pDebugSphere->transformXYZ = glm::vec3(0, 0, 0);
    g_pDebugSphere->scale = 10;
    g_pDebugSphere->scaleXYZ = glm::vec3(g_pDebugSphere->scale, g_pDebugSphere->scale, g_pDebugSphere->scale);
    g_pDebugSphere->bDontLight = true;

    //Create a mesh Skybox
    cMesh* skyBoxMesh = new cMesh;
    skyBoxMesh->meshName = MODEL_DIR + std::string("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
    skyBoxMesh->scale = 10000.0f;
    skyBoxMesh->scaleXYZ = glm::vec3(skyBoxMesh->scale, skyBoxMesh->scale, skyBoxMesh->scale);
    skyBoxMesh->transformXYZ = g_pFlyCamera->getEye();

    //Start of textures
    //
    //Get max texture information from OpenGL
    GLint glMaxCombinedTextureImageUnits = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &glMaxCombinedTextureImageUnits);
    std::cout << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = " << glMaxCombinedTextureImageUnits << std::endl;

    GLint glMaxVertexTextureImageUnits = 0;
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &glMaxVertexTextureImageUnits);
    std::cout << "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS = " << glMaxVertexTextureImageUnits << std::endl;

    GLint glMaxTextureSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTextureSize);
    std::cout << "GL_MAX_TEXTURE_SIZE = " << glMaxTextureSize << std::endl;

    //set the path for textures
    gTextureManager->SetBasePath("assets/textures");

    //Always load in the default texture
    gTextureManager->Create2DTextureFromBMPFile("MissingTexture.bmp", true);

    //Load in model textures from scene
    for (std::string texture : gScene->currentLevel.texturesToLoad)
    {
        gTextureManager->Create2DTextureFromBMPFile(texture, true);
    }

    gTextureManager->SetBasePath("assets/textures/cubemaps");

    std::string errorTextString;
    if (!gTextureManager->CreateCubeTextureFromBMPFiles("Jungle",
        "forestSkyboxNightRight.bmp",
        "forestSkyboxNightLeft.bmp",
        "forestSkyboxNightDown.bmp",
        "forestSkyboxNightUp.bmp",
        "forestSkyboxNightFront.bmp",
        "forestSkyboxNightBack.bmp",
        true, errorTextString))
    {
        std::cout << "Didn't load because: " << errorTextString << std::endl;
    }
    
    //Inform user of loaded textures
    std::cout << "Textures Loaded:" << std::endl;

    std::vector<std::string> texturesLoaded;
    ::gTextureManager->GetLoadedTextureList(texturesLoaded);

    for (std::vector<std::string>::iterator itTexName = texturesLoaded.begin(); itTexName != texturesLoaded.end(); itTexName++)
    {
        std::cout << "\t" << *itTexName << std::endl;
    }


    //Start of the Lights
    // 
    //Load in the lights
    if (gScene->currentLevel.lights.size() > GL_MAX_LIGHTS)
    {
        std::cout << "Max number of lights has been reached, please stop" << std::endl;
    }

    for (int i = 0; i < gScene->currentLevel.lights.size(); i++)
    {
        LightInfo currLight = gScene->currentLevel.lights[i];
        gTheLights->theLights[i].position = glm::vec4(currLight.transform.x, currLight.transform.y, currLight.transform.z, 1.0f);
        gTheLights->theLights[i].direction = glm::vec4(currLight.direction.x, currLight.direction.y, currLight.direction.z, 1.0f);
        gTheLights->theLights[i].param1.x = currLight.param1.x;    // directional light
        gTheLights->theLights[i].param1.y = currLight.param1.y;   // Inner
        gTheLights->theLights[i].param1.z = currLight.param1.z;   // Outer
        gTheLights->theLights[i].atten.x = currLight.atten.x;
        gTheLights->theLights[i].atten.y = currLight.atten.y;
        gTheLights->theLights[i].atten.z = currLight.atten.z;
        gTheLights->theLights[i].diffuse = glm::vec4(currLight.diffuse.x, currLight.diffuse.y, currLight.diffuse.z, 1.0f);
        gTheLights->theLights[i].specular = glm::vec4(currLight.specular.x, currLight.specular.y, currLight.specular.z, 1.0f);
        gTheLights->TurnOnLight(i);
    }

    // Create the FBO (Frame Buffer Object)
    // The texture we can render to
    ::g_pFBO = new cFBO();
    // Set this off screen texture buffer to some random size
    std::string FBOerrorString;
    //    if (::g_pFBO->init(1024, 1024, FBOerrorString))
    //    if (::g_pFBO->init( 8 * 1024, 8 * 1024, FBOerrorString))
    if (::g_pFBO->init(1920, 1080, FBOerrorString))
    {
        std::cout << "FBO is all set!" << std::endl;
    }
    else
    {
        std::cout << "FBO Error: " << FBOerrorString << std::endl;
    }

    // Clear the OG back buffer once, BEFORE we render anything
    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float)height;
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 164.0f / 255.0f, 239.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    enum eRenderPasses
    {
        PASS_1_G_BUFFER_PASS = 1,	// Renders only geometry to G-Buffer
        PASS_2_LIGHT_PASS = 2,		// Apply lighting to G-Buffer
        PASS_3_2D_EFFECTS_PASS = 3		// Optional effects (blur, whatever...)
    };

    GLint renderPassNumber_LocID = glGetUniformLocation(program, "renderPassNumber");

    //Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Set pass to #0
        glUniform1ui(renderPassNumber_LocID, PASS_1_G_BUFFER_PASS);

        glm::mat4 matModel;    // used to be "m"; Sometimes it's called "world"
        glm::mat4 p;
        glm::mat4 v;
        //Set Times for physics
        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // render your GUI
        ImGui::Begin("Cam position");
        ImGui::SetNextWindowSize(ImVec2{ 200, 1000 });

        std::stringstream ss;
        ss << "Position: " << g_pFlyCamera->eye.x << ", " << g_pFlyCamera->eye.y << ", " << g_pFlyCamera->eye.z;

        ImGui::Text(ss.str().c_str());

        ImGui::End();

        //Call the render funtion
        ImGui::Render();

        // Set the output of the renderer to the screen (default FBO)
        GLuint FBO_ID = ::g_pFBO->ID;
        glBindFramebuffer(GL_FRAMEBUFFER, ::g_pFBO->ID);

        // Set the viewport to the size of my offscreen texture (FBO)
        glViewport(0, 0, ::g_pFBO->width, ::g_pFBO->height);
        ratio = ::g_pFBO->width / (float)::g_pFBO->height;

        //turn on depth buffer
        glEnable(GL_DEPTH);     //turns on depth buffer
        glEnable(GL_DEPTH_TEST); // check if the pixel is already closer


        //We now clear buffers using the fbo
        ::g_pFBO->clearBuffers(true, true);

        /*glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/


        gTheLights->CopyLightInfoToShader();

        p = glm::perspective(g_pFlyCamera->FOV,
            ratio,
            g_pFlyCamera->nearPlane,
            g_pFlyCamera->farPlane);

        g_pFlyCamera->Update(deltaTime);

        glm::vec3 cameraEye = ::g_pFlyCamera->getEye();
        glm::vec3 cameraAt = ::g_pFlyCamera->getAtInWorldSpace();
        glm::vec3 cameraUp = ::g_pFlyCamera->getUpVector();


        v = glm::mat4(1.0f);
        v = glm::lookAt(cameraEye,   // "eye"
            cameraAt,    // "at"
            cameraUp);

        cShaderManager::cShaderProgram* pShaderProc = gShaderManager->pGetShaderProgramFromFriendlyName("Shader#1");

        glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"), 1, GL_FALSE, glm::value_ptr(v));
        glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matProjection"), 1, GL_FALSE, glm::value_ptr(p));
        //Update the particle world
        gParticleWorld->TimeStep(deltaTime);

        //Update the gameObjects
        nGameObject::UpdateGameObjects(deltaTime);

        //Rendering the Skybox
        //Let shader know its a skybox
        GLint bIsSkyBox_LocID = glGetUniformLocation(program, "isSkyBox");
        glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_TRUE);

        // Move the "skybox object" with the camera
        skyBoxMesh->transformXYZ = ::g_pFlyCamera->getEye();
        //Draw the skybox
        DrawObject(skyBoxMesh, glm::mat4(1.0f), matModel_Location, matModelInverseTranspose_Location, program, gVAOManager);

        //Let shader know its no longer a skybox
        glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_FALSE);

        //Draw all non transparent objects
        // 
        // Dissable transparency
        glDisable(GL_BLEND);

        // Screen is cleared and its time for the 1st pass
        for (unsigned int index = 0; index != g_vecMeshes.size(); index++)
        {
            // So the code is a little easier...
            cMesh* curMesh = g_vecMeshes[index];
            if (index == 20)
            {
                //curMesh->bDontLight = true;
                curMesh->bUseObjectDebugColour = true;
                curMesh->objectDebugColourRGBA = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
                curMesh->transformXYZ = glm::vec3(0, 0, 500);
                continue;
            }
            curMesh->bDontLight = true;

            //Check if mesh is near the camera
            //The radius is half of the skybox scale
            /*
            float radius = skyBoxMesh->scale;
            glm::vec3 midline = cameraEye - curMesh->transformXYZ;
            float mag = glm::length(midline);
            //we we aint colliding
            if (mag <= 0.0f || mag >= radius)
            {
                continue;
            }
            */

            //TODO: Call this thing literally anywhere else
            //Get location of dicard bool in shader
            GLint mainTextureDiscard_LodID = glGetUniformLocation(program, "mainTextureDiscard");
            // Turn discard transparency off
            glUniform1f(mainTextureDiscard_LodID, (GLfloat)GL_FALSE);

            GLint decalTexture_LodID = glGetUniformLocation(program, "isDecal");
            glUniform1f(decalTexture_LodID, (GLfloat)GL_FALSE);

            //TODO: Change to a bitmask and check that way
            if (curMesh->meshFriendlyName == "Shack.ply")
            {
                //Turn on discard transparency
                glUniform1f(mainTextureDiscard_LodID, (GLfloat)GL_TRUE);
            }

            if (curMesh->bHasDecal)
            {
                glUniform1f(decalTexture_LodID, (GLfloat)GL_TRUE);
            }

            matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
            DrawObject(curMesh, matModel, matModel_Location, matModelInverseTranspose_Location, program, gVAOManager);
        }

        //Render the debug sphere
        if (isDebug)
        {
            GLint mainTextureDiscard_LodID = glGetUniformLocation(program, "mainTextureDiscard");
            // Turn discard transparency off
            glUniform1f(mainTextureDiscard_LodID, (GLfloat)GL_FALSE);

            GLint decalTexture_LodID = glGetUniformLocation(program, "isDecal");
            glUniform1f(decalTexture_LodID, (GLfloat)GL_FALSE);

            matModel = glm::mat4(1.0f);
            if (debugObjType == 0)
            {
                g_pDebugSphere->transformXYZ = g_vecMeshes[curMesh]->transformXYZ;
            }
            else if (debugObjType == 1)
            {
                g_pDebugSphere->transformXYZ = gTheLights->theLights[curLight].position;
            }
            DrawObject(g_pDebugSphere, matModel, matModel_Location, matModelInverseTranspose_Location, program, gVAOManager);
        }

        //Draw all transparent objects
        // 
        // Alpha transparency
        glEnable(GL_BLEND);
        // Basic "alpha transparency"
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        int transparentMeshSize = g_transMeshes.size();
        //Sort objects draw order by distance from camera
        for (int index = 0; index < transparentMeshSize - 1; index++)
        {
            //Get the distances of both objects
            float obj1Dis = glm::distance(cameraEye, g_transMeshes[index]->transformXYZ);
            float obj2Dis = glm::distance(cameraEye, g_transMeshes[index + 1]->transformXYZ);
            //If the first object is closer than the second 
            if (obj1Dis < obj2Dis)
            {
                cMesh* obj1 = g_transMeshes[index];
                cMesh* obj2 = g_transMeshes[index + 1];
                g_transMeshes[index] = obj2;
                g_transMeshes[index + 1] = obj1;
            }
        }

        //Draw the transparent meshes
        for (unsigned int index = 0; index != g_transMeshes.size(); index++)
        {
            // So the code is a little easier...
            cMesh* curMesh = g_transMeshes[index];
            //curMesh->bDontLight = true;


            //TODO: Call this thing literally anywhere else
            //Get location of dicard bool in shader
            GLint mainTextureDiscard_LodID = glGetUniformLocation(program, "mainTextureDiscard");
            // Turn discard transparency off
            glUniform1f(mainTextureDiscard_LodID, (GLfloat)GL_FALSE);

            GLint decalTexture_LodID = glGetUniformLocation(program, "isDecal");
            glUniform1f(decalTexture_LodID, (GLfloat)GL_FALSE);

            if (curMesh->meshFriendlyName == "Shack.ply")
            {
                //Turn on discard transparency
                glUniform1f(mainTextureDiscard_LodID, (GLfloat)GL_TRUE);
            }

            matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
            DrawObject(curMesh, matModel, matModel_Location, matModelInverseTranspose_Location, program, gVAOManager);
        }
        //End of the 1st pass

        //Start of the 2nd pass
        {
            // 2nd pass of the render, where we do something bizzare
            if (::g_pFullScreenQuad == NULL)
            {
                ::g_pFullScreenQuad = new cMesh;
                //            ::g_pFullScreenQuad->meshName = "Imposter_Shapes/Quad_2_sided_aligned_on_XY_plane.ply";
                ::g_pFullScreenQuad->meshName = "Quad_2_sided_aligned_on_XY_plane.ply";
                ::g_pFullScreenQuad->transformXYZ = glm::vec3(0.0f, 0.0f, 500.0f);
                ::g_pFullScreenQuad->scale = 100.0f;
                ::g_pFullScreenQuad->scaleXYZ = glm::vec3(g_pFullScreenQuad->scale, g_pFullScreenQuad->scale, g_pFullScreenQuad->scale);
                ::g_pFullScreenQuad->bDontLight = true;
                //::g_pFullScreenQuad->bUseObjectDebugColour = true;
                //::g_pFullScreenQuad->objectDebugColourRGBA = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
            }

            //Set the viewport size to the fbo size
            glViewport(0, 0, ::g_pFBO->width, ::g_pFBO->height);
            ratio = ::g_pFBO->width / (float)::g_pFBO->height;

            v = glm::mat4(1.0f);


            glm::vec3 eyeForFullScreenQuad = glm::vec3(0.0f, 0.0f, 450.0f);   // "eye" is 100 units away from the quad
            glm::vec3 atForFullScreenQuad = glm::vec3(0.0f, 0.0f, 500.0f);    // "at" the quad
            glm::vec3 upForFullScreenQuad = glm::vec3(0.0f, 1.0f, 0.0f);      // "at" the quad
            v = glm::lookAt(eyeForFullScreenQuad, atForFullScreenQuad, upForFullScreenQuad);

            //glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
            //    1, GL_FALSE, glm::value_ptr(v));

            p = glm::ortho(
                0.0f,   // Left
                1.0f / (float)width,  // Right
                0.0f,   // Top
                1.0f / (float)height, // Bottom
                30.0f, // zNear  Eye is at 450, quad is at 500, so 50 units away
                5000.0f); // zFar

            //glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matProjection"),
            //    1, GL_FALSE, glm::value_ptr(p));

            //Get and set the screen width and height in the shader
            GLint screenWidthHeight_locID = glGetUniformLocation(program, "screenWidthHeight");
            glUniform2f(screenWidthHeight_locID, (float)width, (float)height);

            //Set shader to render pass 2
            glUniform1ui(renderPassNumber_LocID, PASS_2_LIGHT_PASS);

            //Setting all the 4 FBO G-Buffer textures in shader
            GLuint FSO_texVertexMaterialColour_TextureUnit = 5;	    // I picked 5 just because
            glActiveTexture(FSO_texVertexMaterialColour_TextureUnit + GL_TEXTURE0);
            GLuint texVertexMaterialColourTextureNumber = ::g_pFBO->vertexMaterialColour_1_ID;
            glBindTexture(GL_TEXTURE_2D, texVertexMaterialColourTextureNumber);
            // uniform sampler2D texVertexMaterialColour;
            GLint FSQ_textureSamplerSamplerID = glGetUniformLocation(program, "texVertexMaterialColour");
            glUniform1i(FSQ_textureSamplerSamplerID, FSO_texVertexMaterialColour_TextureUnit);

            GLuint FSO_texVertexNormal_TextureUnit = 6;	    // I picked 6 just because
            glActiveTexture(FSO_texVertexNormal_TextureUnit + GL_TEXTURE0);
            GLuint texVertexNormalTextureNumber = ::g_pFBO->vertexNormal_2_ID;
            glBindTexture(GL_TEXTURE_2D, texVertexNormalTextureNumber);
            // uniform sampler2D texVertexNormal;
            GLint FSQ_VertexNormalSamplerID = glGetUniformLocation(program, "texVertexNormal");
            glUniform1i(FSQ_VertexNormalSamplerID, FSO_texVertexNormal_TextureUnit);

            GLuint FSO_texVertexWorldPos_TextureUnit = 7;	    // I picked 7 just because
            glActiveTexture(FSO_texVertexWorldPos_TextureUnit + GL_TEXTURE0);
            GLuint texVertexWorldPosTextureNumber = ::g_pFBO->vertexWorldPos_3_ID;
            glBindTexture(GL_TEXTURE_2D, texVertexWorldPosTextureNumber);
            // uniform sampler2D texVertexWorldPos;
            GLint FSQ_VertexWorldPosSamplerID = glGetUniformLocation(program, "texVertexWorldPos");
            glUniform1i(FSQ_VertexWorldPosSamplerID, FSO_texVertexWorldPos_TextureUnit);

            GLuint FSO_texVertexSpecular_TextureUnit = 8;	    // I picked 8 just because
            glActiveTexture(FSO_texVertexSpecular_TextureUnit + GL_TEXTURE0);
            GLuint texVertexSpecularTextureNumber = ::g_pFBO->vertexSpecular_4_ID;
            glBindTexture(GL_TEXTURE_2D, texVertexSpecularTextureNumber);
            // uniform sampler2D texVertexSpecular;
            GLint FSQ_VertexSpecularSamplerID = glGetUniformLocation(program, "texVertexSpecular");
            glUniform1i(FSQ_VertexSpecularSamplerID, FSO_texVertexSpecular_TextureUnit);

            glm::mat4x4 matModelFullScreenQuad = glm::mat4(1.0f);   // identity matrix

            //glCullFace(GL_FRONT);

            DrawObject(g_vecMeshes[20],
                matModelFullScreenQuad,
                matModel_Location,
                matModelInverseTranspose_Location,
                program,
                gVAOManager);
        }//End of 2nd pass

        //Start of 3rd pass
        
        // Point the output of the renderer to the real framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClearColor(0.0f, 164.0f / 255.0f, 239.0f / 255.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        p = glm::perspective<float>(
            ::g_pFlyCamera->FOV,
            ratio,
            ::g_pFlyCamera->nearPlane,      // Near plane (as large as possible)
            ::g_pFlyCamera->farPlane);      // Far plane (as small as possible)

        glViewport(0, 0, width, height);

        GLint screenWidthHeight_locID = glGetUniformLocation(program, "screenWidthHeight");
        glUniform2f(screenWidthHeight_locID, (float)width, (float)height);

        glUniform1ui(renderPassNumber_LocID, PASS_3_2D_EFFECTS_PASS);

        GLuint FSQ_textureUnit = 9;	    // We picked 9 just because yolo (i.e. it doesn't matter, we just had to pick one)
        glActiveTexture(FSQ_textureUnit + GL_TEXTURE0);
        GLuint TextureNumber = ::g_pFBO->colourTexture_0_ID;
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // uniform sampler2D texLightPassColourBuffer;
        GLint FSQ_textureSamplerID = glGetUniformLocation(program, "texLightPassColourBuffer");
        glUniform1i(FSQ_textureSamplerID, FSQ_textureUnit);

        glm::mat4x4 matModelFullScreenQuad = glm::mat4(1.0f);   // identity matrix

        //glCullFace(GL_FRONT);

        // Place the camera in front of the quad(the "full screen" quad)
        //v = glm::mat4(1.0f);
        //
        //glm::vec3 eyeForFullScreenQuad = glm::vec3(0.0f, 0.0f, 0.0f);   // "eye" is 100 units away from the quad
        //glm::vec3 atForFullScreenQuad = glm::vec3(0.0f, 0.0f, 500.0f);    // "at" the quad
        //glm::vec3 upForFullScreenQuad = glm::vec3(0.0f, 1.0f, 0.0f);      // "at" the quad
        //v = glm::lookAt(eyeForFullScreenQuad,
        //    atForFullScreenQuad,
        //    upForFullScreenQuad);
        //
        //p = glm::ortho(
        //    0.0f,   // Left
        //    1.0f / (float)width,  // Right
        //    0.0f,   // Top
        //    1.0f / (float)height, // Bottom
        //    30.0f, // zNear  Eye is at 450, quad is at 500, so 50 units away
        //    5000.0f); // zFar
        //
        //glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matProjection"),
        //    1, GL_FALSE, glm::value_ptr(p));

 
        
        DrawObject(g_vecMeshes[20],
            matModelFullScreenQuad,
            matModel_Location,
            matModelInverseTranspose_Location,
            program,
            gVAOManager);
            
        //End of 3rd pass


        //TODO: remove because we disable this
        //glDisable(GL_BLEND);

        //Render the GUI data
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // "Present" what we've drawn.
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Handle OUR keyboard, mouse stuff
        handleAsyncKeyboard(window, deltaTime);
        handleAsyncMouse(window, deltaTime);
    }

    // Cleanup the gui 
    //TODO: Mode to clean up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    //Gotta clean up
    CleanUp(window);

    nGameObject::DeleteGameObjects();

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

//These are meshes that always need to be here and arent apart of the scene
void LoadMeshes(GLuint program)
{
    //Add the debug sphere
    sModelDrawInfo sphereInfo;
    if (!gVAOManager->LoadModelIntoVAO(MODEL_DIR + std::string("WhiteBall.ply"), sphereInfo, program))
    {
        std::cout << "Error: " << "WhiteBall.ply" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "WhiteBall.ply" << " loaded OK" << std::endl;
        std::cout << sphereInfo.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << sphereInfo.numberOfTriangles << " triangles loaded" << std::endl;
    }

    //Add the Skybox Mesh to the vao manager
    sModelDrawInfo skyBoxInfo;
    if (!gVAOManager->LoadModelIntoVAO(MODEL_DIR + std::string("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply"), skyBoxInfo, program))
    {
        std::cout << "Error: " << "Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply" << " loaded OK" << std::endl;
        std::cout << skyBoxInfo.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << skyBoxInfo.numberOfTriangles << " triangles loaded" << std::endl;
    }

    //Add the Skybox Mesh to the vao manager
    sModelDrawInfo drawingQuadInfo;
    if (!gVAOManager->LoadModelIntoVAO(MODEL_DIR + std::string("Quad_1_sided_aligned_on_XY_plane.ply"), skyBoxInfo, program))
    {
        std::cout << "Error: " << "Quad_1_sided_aligned_on_XY_plane.ply" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "Quad_1_sided_aligned_on_XY_plane.ply" << " loaded OK" << std::endl;
        std::cout << skyBoxInfo.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << skyBoxInfo.numberOfTriangles << " triangles loaded" << std::endl;
    }
}