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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "GlobalItems.h"
#include "EntitySystem.h"
#include "random_helpers.h"
#include "QuaternionHelpers.h"

#include "cParticleContactGenerators.h"
#include "cParticleConstraints.h"
#include "particle_force_generators.h"
#include "cProjectile.h"

//Draw send in cMesh
void DrawObject(cMesh* pCurrentMesh, glm::mat4 matModel, GLint matModel_Location, GLint matModelInverseTranspose_Location, GLuint program, cVAOManager* pVAOManager);

// Global Variables
float deltaTime;
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

    //Create a mesh debug sphere
    g_pDebugSphere = new cMesh;
    g_pDebugSphere->meshName = MODEL_DIR + std::string("WhiteBall.ply");
    g_pDebugSphere->transformXYZ = glm::vec3(0, 0, 0);
    g_pDebugSphere->scale = 1;
    g_pDebugSphere->scaleXYZ = glm::vec3(1, 1, 1);
    g_pDebugSphere->bDontLight = true;

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

    //Init values for imgui
    bool showDemo = true;

    //Main Loop
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glm::mat4 matModel;    // used to be "m"; Sometimes it's called "world"
        glm::mat4 p;
        glm::mat4 v;
        glm::mat4 mvp;
        //Set Times for physics
        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // render your GUI
        ImGui::Begin("Demo window");
        ImGui::Button("Hello!");
        ImGui::End();

        //Call the render funtion
        ImGui::Render();

        //turn on depth buffer
        glEnable(GL_DEPTH);     //turns on depth buffer
        glEnable(GL_DEPTH_TEST); // check if the pixel is already closer

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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

        glUniformMatrix4fv(matView_Location, 1, GL_FALSE, glm::value_ptr(v));
        glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(p));

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
        
        // Screen is cleared and we are ready to draw the scene...
        for (unsigned int index = 0; index != g_vecMeshes.size(); index++)
        {
            // So the code is a little easier...
            cMesh* curMesh = g_vecMeshes[index];
            //curMesh->bDontLight = true;


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

        //Draw the meshes
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

        //TODO: remove because we disable this
        glDisable(GL_BLEND);


        //TODO: Why doesnt this work
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