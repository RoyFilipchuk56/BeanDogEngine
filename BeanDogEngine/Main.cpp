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
#include "cShaderManager.h"
#include "cVAOManager.h"
#include "cMesh.h"
#include "SceneManager.h"
#include "cLightManager.h"
#include "cParticleWorld.h"
#include "cProjectile.h"
#include "ProjectileManager.h"
#include "particle_force_generators.h"
#include "random_helpers.h"


//check if the string is a number
bool IsANumber(std::string number);
//Draw send in cMesh
void DrawObject(cMesh* pCurrentMesh, glm::mat4 matModel, GLint matModel_Location, GLint matModelInverseTranspose_Location, GLuint program, cVAOManager* pVAOManager);

//Static locations
const std::string PROJECTILE_XML_LOCATION = CONFIG_DIR + std::string("Projectile.xml");
// Global Variables
// 
//The delta Time
float deltaTime;

//cannon stuff
//The mesh for the cannon
cMesh* cannonMesh;
glm::vec4 cannonInitialVec(0, 0, 1, 0);
//Cannon Rotation
glm::mat4 canRotationX;
glm::mat4 canRotationY;
//Cannon min and max's
CannonLoader cannonValues;

//calls the latest error
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
/*
//Handles key input
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    float cameraSpeed = 50.0f;

    /*
    // Basic camera controls
    if (key == GLFW_KEY_A)
    {
        cameraEye.x -= cameraSpeed * deltaTime;     // Go left
    }
    if (key == GLFW_KEY_D)
    {
        cameraEye.x += cameraSpeed * deltaTime;     // Go right
    }

    if (key == GLFW_KEY_W)
    {
        cameraEye.z += cameraSpeed * deltaTime;     // Go forward
    }
    if (key == GLFW_KEY_S)
    {
        cameraEye.z -= cameraSpeed * deltaTime;     // Go backwards
    }

    if (key == GLFW_KEY_Q)
    {
        cameraEye.y -= cameraSpeed * deltaTime;     // Go "Down"
    }
    if (key == GLFW_KEY_E)
    {
        cameraEye.y += cameraSpeed * deltaTime;     // Go "Up"
    }
    
    //Toggle Debug
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        isDebug = !isDebug;
    }

    //Control Object Placement in Debug
    if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {
        
        if (debugObjType == 0)
        {
            g_vecMeshes[curMesh]->transformXYZ += glm::vec3(0, objectMoveValue, 0);
        }

        if (debugObjType == 1)
        {
            gTheLights->theLights[curLight].position += glm::vec4(0, objectMoveValue, 0, 1);
        }
    }
    if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        if (debugObjType == 0)
        {
            g_vecMeshes[curMesh]->transformXYZ += glm::vec3(0, -objectMoveValue, 0);
        }

        if (debugObjType == 1)
        {
            gTheLights->theLights[curLight].position += glm::vec4(0, -objectMoveValue, 0, 1);
        }
    }
    if (key == GLFW_KEY_J && action == GLFW_PRESS)
    {
        if (debugObjType == 0)
        {
            g_vecMeshes[curMesh]->transformXYZ += glm::vec3(objectMoveValue, 0, 0);
        }

        if (debugObjType == 1)
        {
            gTheLights->theLights[curLight].position += glm::vec4(objectMoveValue, 0, 0, 1);
        }
    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (debugObjType == 0)
        {
            g_vecMeshes[curMesh]->transformXYZ += glm::vec3(-objectMoveValue, 0, 0);
        }

        if (debugObjType == 1)
        {
            gTheLights->theLights[curLight].position += glm::vec4(-objectMoveValue, 0, 0, 1);
        }
    }
    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        if (debugObjType == 0)
        {
            g_vecMeshes[curMesh]->transformXYZ += glm::vec3(0, 0, objectMoveValue);
        }

        if (debugObjType == 1)
        {
            gTheLights->theLights[curLight].position += glm::vec4(0, 0, objectMoveValue, 1);
        }
    }
    if (key == GLFW_KEY_U && action == GLFW_PRESS)
    {
        if (debugObjType == 0)
        {
            g_vecMeshes[curMesh]->transformXYZ += glm::vec3(0, 0, -objectMoveValue);
        }

        if (debugObjType == 1)
        {
            gTheLights->theLights[curLight].position += glm::vec4(0, 0, -objectMoveValue, 1);
        }
    }
    //change the object you control in debug
    if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS)
    {
        curLight++;
        if (curLight >= gTheLights->NUMBER_OF_LIGHTS)
        {
            curLight = cLightManager::NUMBER_OF_LIGHTS - 1;
        }
    }
    if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS)
    {
        curLight--;
        if (curLight >= cLightManager::NUMBER_OF_LIGHTS)
        {
            curLight = 0;
        }
    }

    //Change what type of object you control in debug (lights or meshes)
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        debugObjType++;
        if (debugObjType > 1)
        {
            debugObjType = 0;
        }
    }

    //Entry Tunnel
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        
    }

    //Main hangar
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        
    }
}
*/

int main()
{
    GLFWwindow* window;
    //Set program to 0 since we dont start with a shader
    GLuint program = 0;
    //Default to -1 since thats the error
    GLint mvp_location = -1;
    //set current time
    float previousTime = static_cast<float>(glfwGetTime());
    //Create a new vao manager
    gVAOManager = new cVAOManager();
    //Create a new shader manager
    gShaderManager = new cShaderManager();
    //Create a new light manager
    gTheLights = new cLightManager();
    //new fly camera 
    g_pFlyCamera = new cFlyCamera();

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
    //glfwSetWindowSizeCallback(window, GLFW_window_size_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    g_pFlyCamera->setEye(glm::vec3(0.0f, 0.0f, 20.0f));

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

    gTheLights->theLights[0].position = glm::vec4(0.0f, 33.5f, 26.0f, 1.0f);
    gTheLights->theLights[0].param1.x = 1.0f;    // directional
    gTheLights->theLights[0].param1.y = 15.0f;   // Inner
    gTheLights->theLights[0].param1.z = 30.0f;   // Outer
    gTheLights->theLights[0].atten.x = 1.09474325f;
    gTheLights->theLights[0].atten.y = 4.85198834e-05;
    gTheLights->theLights[0].atten.z = 2.18895515e-08;
    gTheLights->theLights[0].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    gTheLights->theLights[0].diffuse = glm::vec4(0.9922f, 0.9922f, 0.7f, 1.0f);
    gTheLights->TurnOnLight(0);
    curLight = 0;
    
    
    gTheLights->theLights[1].position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    gTheLights->theLights[1].param1.x = 2.0f;    // directional
    gTheLights->theLights[1].param1.y = 15.0f;   // Inner
    gTheLights->theLights[1].param1.z = 30.0f;   // Outer
    gTheLights->theLights[1].atten.y = 0.000001f;
    gTheLights->theLights[1].atten.z = 0.00000001f;
    gTheLights->theLights[1].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    gTheLights->theLights[1].diffuse = glm::vec4(0.9922f, 0.9843f, 0.8275f, 1.0f);
    gTheLights->TurnOnLight(1);
    

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

    //make a bullet model info
    sModelDrawInfo bulletInfo;
    if (!gVAOManager->LoadModelIntoVAO(MODEL_DIR + std::string("Bullet.ply"), bulletInfo, program))
    {
        std::cout << "Error: " << "bulletInfo.phy" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "bulletInfo.phy" << " loaded OK" << std::endl;
        std::cout << bulletInfo.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << bulletInfo.numberOfTriangles << " triangles loaded" << std::endl;
    }

    //Create a mesh for the firework
    cMesh* debugMesh = new cMesh;
    debugMesh->meshName = MODEL_DIR + std::string("Bullet.ply");
    debugMesh->transformXYZ = glm::vec3(0, 2, 0);
    debugMesh->bDontLight = true;

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
        deltaTime = previousTime - currentTime;
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


        // Screen is cleared and we are ready to draw the scene...
        for (unsigned int index = 0; index != g_vecMeshes.size(); index++)
        {
            // So the code is a little easier...
            cMesh* curMesh = g_vecMeshes[index];
            //curMesh->bDontLight = true;

            matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
            DrawObject(curMesh, matModel, matModel_Location, matModelInverseTranspose_Location, program, gVAOManager);
        }

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

    

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

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

void DrawObject(cMesh* curMesh, glm::mat4 matModel, GLint matModel_Location, GLint matModelInverseTranspose_Location, GLuint program, cVAOManager* pVAOManager)
{
    // Translate or "move" the object somewhere
    glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
        curMesh->transformXYZ);

    // Rotation around the Z axis
    glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
        curMesh->rotationXYZ.z,//(float)glfwGetTime(),
        glm::vec3(0.0f, 0.0f, 1.0f));

    // Rotation around the Y axis
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
        curMesh->rotationXYZ.y,
        glm::vec3(0.0f, 1.0f, 0.0f));

    // Rotation around the X axis
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
        curMesh->rotationXYZ.x,
        glm::vec3(1.0f, 0.0f, 0.0f));

    // Scale the model
    glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
        glm::vec3(curMesh->scale,  // Scale in X
            curMesh->scale,  // Scale in Y
            curMesh->scale));// Scale in Z

    if (curMesh->meshName == MODEL_DIR + std::string("Cannon.ply"))
    {
        canRotationX = rotateX;
        canRotationY = rotateY;
    }
    matModel = matModel * matTranslate;
    matModel = matModel * rotateZ;
    matModel = matModel * rotateY;
    matModel = matModel * rotateX;
    matModel = matModel * matScale;

    glUniformMatrix4fv(matModel_Location, 1, GL_FALSE, glm::value_ptr(matModel));

    // Used to calculate the normal location in vertex space, using only rotation
    glm::mat4 matInvTransposeModel = glm::inverse(glm::transpose(matModel));
    glUniformMatrix4fv(matModelInverseTranspose_Location, 1, GL_FALSE, glm::value_ptr(matInvTransposeModel));

    // If bUseDebugColour is TRUE, then the fragment colour is "objectDebugColour"
    GLint bUseDebugColour_Location = glGetUniformLocation(program, "bUseDebugColour");
    GLint objectDebugColour_Location = glGetUniformLocation(program, "objectDebugColour");

    // If true, then the lighting contribution is NOT used
    GLint bDontLightObject_Location = glGetUniformLocation(program, "bDontLightObject");

    // The "whole object" colour (diffuse and specular)
    GLint wholeObjectDiffuseColour_Location = glGetUniformLocation(program, "wholeObjectDiffuseColour");
    GLint bUseWholeObjectDiffuseColour_Location = glGetUniformLocation(program, "bUseWholeObjectDiffuseColour");
    GLint wholeObjectSpecularColour_Location = glGetUniformLocation(program, "wholeObjectSpecularColour");


    if (curMesh->bUseWholeObjectDiffuseColour)
    {
        glUniform1f(bUseWholeObjectDiffuseColour_Location, (float)GL_TRUE);
        glUniform4f(wholeObjectDiffuseColour_Location,
            curMesh->wholeObjectDiffuseRGBA.r,
            curMesh->wholeObjectDiffuseRGBA.g,
            curMesh->wholeObjectDiffuseRGBA.b,
            curMesh->wholeObjectDiffuseRGBA.a);
    }
    else
    {
        glUniform1f(bUseWholeObjectDiffuseColour_Location, (float)GL_FALSE);
    }

    glUniform4f(wholeObjectSpecularColour_Location,
        curMesh->wholeObjectSpecularRGB.r,
        curMesh->wholeObjectSpecularRGB.g,
        curMesh->wholeObjectSpecularRGB.b,
        curMesh->wholeObjectShininess_SpecPower);

    // See if mesh is wanting the vertex colour override (HACK) to be used?
    if (curMesh->bUseObjectDebugColour)
    {
        // Override the colour...
        glUniform1f(bUseDebugColour_Location, (float)GL_TRUE);
        glUniform4f(objectDebugColour_Location,
            curMesh->objectDebugColourRGBA.r,
            curMesh->objectDebugColourRGBA.g,
            curMesh->objectDebugColourRGBA.b,
            curMesh->objectDebugColourRGBA.a);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(bUseDebugColour_Location, (float)GL_FALSE);
    }

    // See if mesh wants to be lit
    if (curMesh->bDontLight)
    {
        // Override the light
        glUniform1f(bDontLightObject_Location, (float)GL_TRUE);
    }
    else
    {
        // DON'T override the light
        glUniform1f(bDontLightObject_Location, (float)GL_FALSE);
    }

    //Check if the 1 button has been pressed
    if (curMesh->bIsWireframe)                // GL_POINT, GL_LINE, and GL_FILL)
    {
        //Draw everything with only lines
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        //Fill up those polys
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    sModelDrawInfo modelInfo;

    if (gVAOManager->FindDrawInfoByModelName(curMesh->meshName, modelInfo))
    {
        glBindVertexArray(modelInfo.VAO_ID);

        glDrawElements(GL_TRIANGLES,
                       modelInfo.numberOfIndices,
                       GL_UNSIGNED_INT,
                       (void*)0);

        glBindVertexArray(0);
    }
}