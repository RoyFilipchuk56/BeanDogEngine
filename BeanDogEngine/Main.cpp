#include "GLCommon.h"

//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>       // "smart array"
#include <fstream>      // C++ file I-O library (look like any other stream)
#include <map>

#include "GlobalItems.h"
#include "SceneManager.h"
#include "MapManager.h"
#include "random_helpers.h"

#include "cParticleContactGenerators.h"

//check if the string is a number
bool IsANumber(std::string number);
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

    SceneManager scene;
    scene.LoadSceneFromXML("SceneOne.xml");

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

    //Set all the globals 
    StartUp(window);

    //Set camera eye (AKA Location)
    g_pFlyCamera->setEye(scene.currentLevel.camera.tranform);

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

    //load in each model in the scene
    for (int i = 0; i < scene.currentLevel.models.size(); i++)
    {
        //make a temp model info
        sModelDrawInfo tempInfo;
        if (!gVAOManager->LoadModelIntoVAO(MODEL_DIR + scene.currentLevel.models[i].fileName, tempInfo, program))
        {
            std::cout << "Error: " << scene.currentLevel.models[i].fileName << " Didn't load OK" << std::endl;
        }
        else
        {
            std::cout << "Good: " << scene.currentLevel.models[i].fileName << " loaded OK" << std::endl;
            std::cout << tempInfo.numberOfVertices << " vertices loaded" << std::endl;
            std::cout << tempInfo.numberOfTriangles << " triangles loaded" << std::endl;
        }
        //make a temp mesh and load in all the attributes
        cMesh* tempMesh = new cMesh;
        tempMesh->meshName = MODEL_DIR + scene.currentLevel.models[i].fileName;
        tempMesh->transformXYZ = scene.currentLevel.models[i].transform;
        tempMesh->rotationXYZ = scene.currentLevel.models[i].rotation;
        tempMesh->scale = scene.currentLevel.models[i].scale;
        g_vecMeshes.push_back(tempMesh);
    }

    //Change to base projectile
    sModelDrawInfo cannonInfo;
    if (!gVAOManager->LoadModelIntoVAO(MODEL_DIR + std::string("Cannon.ply"), cannonInfo, program))
    {
        std::cout << "Error: " << "Cannon.ply" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "Cannon.ply" << " loaded OK" << std::endl;
        std::cout << cannonInfo.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << cannonInfo.numberOfTriangles << " triangles loaded" << std::endl;
    }

    //Create a mesh for the cannon
    cMesh* cannonMesh = new cMesh;
    cannonMesh->meshName = MODEL_DIR + std::string("Cannon.ply");
    cannonMesh->transformXYZ = glm::vec3(0, 1, 0);
    g_vecMeshes.push_back(cannonMesh);

    //Change to base projectile
    sModelDrawInfo bulletInfo;
    if (!gVAOManager->LoadModelIntoVAO(MODEL_DIR + std::string("WhiteBall.ply"), bulletInfo, program))
    {
        std::cout << "Error: " << "WhiteBall.ply" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "WhiteBall.ply" << " loaded OK" << std::endl;
        std::cout << bulletInfo.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << bulletInfo.numberOfTriangles << " triangles loaded" << std::endl;
    }

    //TODO: Remove
    //Testing textures
    sModelDrawInfo chunkyInfo;
    if (!gVAOManager->LoadModelIntoVAO(MODEL_DIR + std::string("Chunky.ply"), chunkyInfo, program))
    {
        std::cout << "Error: " << "Cannon.ply" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "Cannon.ply" << " loaded OK" << std::endl;
        std::cout << chunkyInfo.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << chunkyInfo.numberOfTriangles << " triangles loaded" << std::endl;
    }

    //Testing textures pt2
    cMesh* chunkyMesh = new cMesh;
    chunkyMesh->meshName = MODEL_DIR + std::string("Chunky.ply");
    chunkyMesh->transformXYZ = glm::vec3(0, 1, 0);
    chunkyMesh->rotationXYZ = glm::vec3(0, 3.1, 0);
    chunkyMesh->textureNames[0] = "chunky.bmp";
    chunkyMesh->textureRatios[0] = 1.0f;
    g_vecMeshes.push_back(chunkyMesh);

    //Create a mesh for the debug
    cMesh* debugMesh = new cMesh;
    debugMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
    debugMesh->transformXYZ = glm::vec3(0, 2, 0);
    debugMesh->bDontLight = true;

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

    //TODO: Load in the textures
    gTextureManager->SetBasePath("assets/textures");

    gTextureManager->Create2DTextureFromBMPFile("BrightColouredUVMap.bmp", true);
    gTextureManager->Create2DTextureFromBMPFile("chunky.bmp", true);
    std::cout << "Textures Loaded:" << std::endl;

    std::vector<std::string> texturesLoaded;
    ::gTextureManager->GetLoadedTextureList(texturesLoaded);

    for (std::vector<std::string>::iterator itTexName = texturesLoaded.begin(); itTexName != texturesLoaded.end(); itTexName++)
    {
        std::cout << "\t" << *itTexName << std::endl;
    }


    //Load in the lights
    if (scene.currentLevel.lights.size() > GL_MAX_LIGHTS)
    {
        std::cout << "Max number of lights has been reached, please stop" << std::endl;
    }

    for (int i = 0; i < scene.currentLevel.lights.size(); i++)
    {
        LightInfo currLight = scene.currentLevel.lights[i];
        gTheLights->theLights[i].position = glm::vec4(currLight.transform.x, currLight.transform.y, currLight.transform.z, 1.0f);
        gTheLights->theLights[i].direction = glm::vec4(currLight.direction.x, currLight.direction.y, currLight.direction.z, 1.0f);
        gTheLights->theLights[i].param1.x = currLight.param1.x;    // directional light
        gTheLights->theLights[i].param1.y = currLight.param1.y;   // Inner
        gTheLights->theLights[i].param1.z = currLight.param1.z;   // Outer
        gTheLights->theLights[i].atten.x = currLight.atten.x;
        gTheLights->theLights[i].atten.y = currLight.atten.y;
        gTheLights->theLights[i].atten.z = currLight.atten.z;
        gTheLights->theLights[i].diffuse = glm::vec4(currLight.diffuse.x, currLight.diffuse.y, currLight.diffuse.z, 1.0f);
        gTheLights->TurnOnLight(i);
    }

    //Create a cannon and set it in globals
    cannon = new TheCannon(cannonMesh);

    //Add the plane Generator
    nPhysics::cPlaneParticleContactGenerator* groundPlane = new nPhysics::cPlaneParticleContactGenerator(glm::vec3(0, 0.5f, 0), glm::vec3(0, 1, 0));
    gParticleWorld->AddContactGenerator(groundPlane);

    nPhysics::cPlaneParticleContactGenerator* zPositivePlane = new nPhysics::cPlaneParticleContactGenerator(glm::vec3(0, 0, 14.5f), glm::vec3(0, 0, -1));
    gParticleWorld->AddContactGenerator(zPositivePlane);

    nPhysics::cPlaneParticleContactGenerator* zNegativePlane = new nPhysics::cPlaneParticleContactGenerator(glm::vec3(0, 0, -14.5f), glm::vec3(0, 0, 1));
    gParticleWorld->AddContactGenerator(zNegativePlane);

    nPhysics::cPlaneParticleContactGenerator* xNegativePlane = new nPhysics::cPlaneParticleContactGenerator(glm::vec3(-14.5f, 0, 0), glm::vec3(1, 0, 0));
    gParticleWorld->AddContactGenerator(xNegativePlane);

    nPhysics::cPlaneParticleContactGenerator* xPositivePlane = new nPhysics::cPlaneParticleContactGenerator(glm::vec3(14.5, 0, 0), glm::vec3(-1, 0, 0));
    gParticleWorld->AddContactGenerator(xPositivePlane);
    
    nPhysics::cSphereParticleContactGenerator* collideThoseSpheres = new nPhysics::cSphereParticleContactGenerator();
    gParticleWorld->AddContactGenerator(collideThoseSpheres);
    
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
        
        //Update the cannon
        cannon->Update(deltaTime);

        //Update the particle world
        gParticleWorld->TimeStep(deltaTime);

        // Screen is cleared and we are ready to draw the scene...
        for (unsigned int index = 0; index != g_vecMeshes.size(); index++)
        {
            // So the code is a little easier...
            cMesh* curMesh = g_vecMeshes[index];
            //curMesh->bDontLight = true;

            matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
            DrawObject(curMesh, matModel, matModel_Location, matModelInverseTranspose_Location, program, gVAOManager);
        }

        // Screen is cleared and we are ready to draw the projectiles...
        for (unsigned int index = 0; index != projectiles.size(); index++)
        {
            // So the code is a little easier...
            cMesh* curMesh = projectiles[index]->myMesh;
            //curMesh->bDontLight = true;

            matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
            DrawObject(curMesh, matModel, matModel_Location, matModelInverseTranspose_Location, program, gVAOManager);
        }

        //Render the debug sphere
        if (isDebug)
        {
            matModel = glm::mat4(1.0f);
            debugMesh->transformXYZ = gTheLights->theLights[curLight].position;
            DrawObject(debugMesh, matModel, matModel_Location, matModelInverseTranspose_Location, program, gVAOManager);
        }

        // "Present" what we've drawn.
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Handle OUR keyboard, mouse stuff
        handleAsyncKeyboard(window, deltaTime);
        handleAsyncMouse(window, deltaTime);
    }

    //Gotta clean up
    CleanUp(window);

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

//TODO: Add to random helpers
//check if the string is a number
bool IsANumber(std::string number)
{
    bool isNegative = false;
    if (number[0] == '-')
    {
        number.erase(0);
        isNegative = true;
    }
    
    //check if each char in the string is a number
    for (char const& c : number)
    {
        //If theres a decimal
        if (c == '.')
        {
            continue;
        }

        if (std::isdigit(c) == 0) return false;
    }
    return true;
}