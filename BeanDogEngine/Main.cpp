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

#include "cShaderManager.h"
#include "cVAOManager.h"
#include "cMesh.h"
#include "SceneManager.h"
#include "cParticleWorld.h"
#include "cProjectile.h"
#include "ProjectileManager.h"
#include "particle_force_generators.h"
#include "random_helpers.h"

//function identifiers
bool createProjectile(std::string name);
//check if the string is a number
bool IsANumber(std::string number);

//Static locations
const std::string PROJECTILE_XML_LOCATION = CONFIG_DIR + std::string("Projectile.xml");
// Global Variables

glm::vec3 cameraEye;
cShaderManager  gShaderManager;
cVAOManager     gVAOManager;
//A vector of meshes to be drawn in the scene
std::vector<cMesh*> g_vecMeshes;
bool isWireframe = false;
nPhysics::cParticleWorld* world;
//default gravity value
nPhysics::cParticleGravityGenerator gravityGenerator(glm::vec3(0.0f, -9.81f, 0.0f));
ProjectileManager* projectileManager;
//A list of fireworks currently alive
std::vector<nPhysics::cProjectile*> projectiles;
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

//Handles key input
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    float cameraSpeed = 50.0f;

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
    if (key == GLFW_KEY_LEFT)
    {
        float tempRotation = glm::clamp<float>(cannonMesh->rotationXYZ.y + (1.0f * deltaTime), cannonValues.currentCannon.yawMin, cannonValues.currentCannon.yawMax);
        cannonMesh->rotationXYZ.y = tempRotation;
    }
    if (key == GLFW_KEY_RIGHT)
    {
        float tempRotation = glm::clamp<float>(cannonMesh->rotationXYZ.y - (1.0f * deltaTime), cannonValues.currentCannon.yawMin, cannonValues.currentCannon.yawMax);
        cannonMesh->rotationXYZ.y = tempRotation;
    }
    if (key == GLFW_KEY_UP)
    {
        float tempRotation = glm::clamp<float>(cannonMesh->rotationXYZ.x - (1.0f * deltaTime), cannonValues.currentCannon.pitchMin, cannonValues.currentCannon.pitchMax);
        cannonMesh->rotationXYZ.x = tempRotation;
    }
    if (key == GLFW_KEY_DOWN)
    {
        float tempRotation = glm::clamp<float>(cannonMesh->rotationXYZ.x + (1.0f * deltaTime), cannonValues.currentCannon.pitchMin, cannonValues.currentCannon.pitchMax);
        cannonMesh->rotationXYZ.x = tempRotation;
    }
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        isWireframe = !isWireframe;     //toggle wireframe
    }
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (!createProjectile("Bullet"))
        {
            std::cout << "Bullet couldnt be created" << std::endl;
        }
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (!createProjectile("Laser"))
        {
            std::cout << "Laser couldnt be created" << std::endl;
        }
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (!createProjectile("CannonBall"))
        {
            std::cout << "Cannon Ball couldnt be created" << std::endl;
        }
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (!createProjectile("EnergyBall"))
        {
            std::cout << "Energy Ball couldnt be created" << std::endl;
        }
    }
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

    //Create a particle world
    world = new nPhysics::cParticleWorld();
    //Create a new projectile manager
    projectileManager = new ProjectileManager(PROJECTILE_XML_LOCATION);
    //Load Cannon info from xml
    cannonValues.LoadCannonFromXML(CONFIG_DIR + std::string("Cannon.xml"));
    //load scene from xml
    SceneManager scene;
    scene.LoadSceneFromXML("SceneOne.xml");
    //set camera 
    cameraEye = scene.currentLevel.camera.tranform;



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

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    cShaderManager::cShader vertShader;
    vertShader.fileName = SHADER_DIR;
    vertShader.fileName.append("vertShader_01.glsl");

    cShaderManager::cShader fragShader;
    fragShader.fileName = SHADER_DIR;
    fragShader.fileName.append("fragShader_01.glsl");

    if (gShaderManager.createProgramFromFile("Shader#1", vertShader, fragShader))
    {
        std::cout << "Shader compiled OK" << std::endl;
        // 
        // Set the "program" variable to the one the Shader Manager used...
        program = gShaderManager.getIDFromFriendlyName("Shader#1");
    }
    else
    {
        std::cout << "Error making shader program: " << std::endl;
        std::cout << gShaderManager.getLastError() << std::endl;
    }

    // Get "uniform locations" (aka the registers these are in)
    GLint matModel_Location = glGetUniformLocation(program, "matModel");
    GLint matView_Location = glGetUniformLocation(program, "matView");
    GLint matProjection_Location = glGetUniformLocation(program, "matProjection");

    //load in each model in the scene
    for (int i = 0; i < scene.currentLevel.models.size(); i++)
    {
        //make a temp model info
        sModelDrawInfo tempInfo;
        if (!gVAOManager.LoadModelIntoVAO(MODEL_DIR + scene.currentLevel.models[i].fileName, tempInfo, program))
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

    //making the cannon info
    sModelDrawInfo cannonInfo;
    if (!gVAOManager.LoadModelIntoVAO(MODEL_DIR + std::string("Cannon.ply"), cannonInfo, program))
    {
        std::cout << "Error: " << "Cannon.phy" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "Cannon.phy" << " loaded OK" << std::endl;
        std::cout << cannonInfo.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << cannonInfo.numberOfTriangles << " triangles loaded" << std::endl;
    }
    //make a cannon mesh and load in all the attributes
    cannonMesh = new cMesh;
    cannonMesh->meshName = MODEL_DIR + std::string("Cannon.ply");
    cannonMesh->transformXYZ = glm::vec3(0, 1, 0);
    g_vecMeshes.push_back(cannonMesh);


    //make a bullet model info
    sModelDrawInfo bulletInfo;
    if (!gVAOManager.LoadModelIntoVAO(MODEL_DIR + std::string("Bullet.ply"), bulletInfo, program))
    {
        std::cout << "Error: " << "bulletInfo.phy" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "bulletInfo.phy" << " loaded OK" << std::endl;
        std::cout << bulletInfo.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << bulletInfo.numberOfTriangles << " triangles loaded" << std::endl;
    }

    //make a laser model info
    sModelDrawInfo laserInfo;
    if (!gVAOManager.LoadModelIntoVAO(MODEL_DIR + std::string("Laser.ply"), laserInfo, program))
    {
        std::cout << "Error: " << "laser.phy" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "laser.phy" << " loaded OK" << std::endl;
        std::cout << laserInfo.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << laserInfo.numberOfTriangles << " triangles loaded" << std::endl;
    }

    //make a cannonBall model info
    sModelDrawInfo cannonBallInfo;
    if (!gVAOManager.LoadModelIntoVAO(MODEL_DIR + std::string("CannonBall.ply"), laserInfo, program))
    {
        std::cout << "Error: " << "cannonBall.phy" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "cannonBall.phy" << " loaded OK" << std::endl;
        std::cout << cannonBallInfo.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << cannonBallInfo.numberOfTriangles << " triangles loaded" << std::endl;
    }

    //make a energyBall model info
    sModelDrawInfo energyBallInfo;
    if (!gVAOManager.LoadModelIntoVAO(MODEL_DIR + std::string("EnergyBall.ply"), energyBallInfo, program))
    {
        std::cout << "Error: " << "energyBall.phy" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "energyBall.phy" << " loaded OK" << std::endl;
        std::cout << energyBallInfo.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << energyBallInfo.numberOfTriangles << " triangles loaded" << std::endl;
    }

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


        //timestep
        world->TimeStep(deltaTime);

        if (projectiles.size() > 0)
        {
            for (int i = 0; i < projectiles.size(); i++)
            {
                //If the firework isnt alive
                if (!projectiles[i]->IsAlive())
                {
                    //remove particle from world, delete it and erase all records of its existance
                    world->RemoveParticle(projectiles[i]);
                    delete projectiles[i];
                    projectiles.erase(projectiles.begin() + i);
                    g_vecMeshes.erase(g_vecMeshes.begin() + i + 2);
                }
                //If its alive then update its position
                else
                {
                    g_vecMeshes[i + 2]->transformXYZ = projectiles[i]->GetPosition();
                }
            }
        }
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        //turn on depth buffer
        glEnable(GL_DEPTH);     //turns on depth buffer
        glEnable(GL_DEPTH_TEST); // check if the pixel is already closer

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Screen is cleared and we are ready to draw the scene...
        for (unsigned int index = 0; index != g_vecMeshes.size(); index++)
        {
            // So the code is a little easier...
            cMesh* curMesh = g_vecMeshes[index];


            matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
            //mat4x4_identity(m);


            // *****************************************************
            // Translate or "move" the object somewhere
            glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
                curMesh->transformXYZ);

            //matModel = matModel * matTranslate;
            // *****************************************************


            // *****************************************************
            // Rotation around the Z axis
            glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
                curMesh->rotationXYZ.z,//(float)glfwGetTime(),
                glm::vec3(0.0f, 0.0f, 1.0f));

            //matModel = matModel * rotateZ;
            // *****************************************************

            // *****************************************************
            // Rotation around the Y axis
            glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
                curMesh->rotationXYZ.y,
                glm::vec3(0.0f, 1.0f, 0.0f));

            //matModel = matModel * rotateY;
            // *****************************************************

            // *****************************************************
            // Rotation around the X axis
            glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
                curMesh->rotationXYZ.x,
                glm::vec3(1.0f, 0.0f, 0.0f));

            //matModel = matModel * rotateX;
            // *****************************************************


            // *****************************************************
            // Scale the model
            glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                glm::vec3(curMesh->scale,  // Scale in X
                    curMesh->scale,  // Scale in Y
                    curMesh->scale));// Scale in Z

//matModel = matModel * matScale;
// *****************************************************

// *****************************************************
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

            p = glm::perspective(0.6f,
                ratio,
                0.1f,
                1000.0f);

            v = glm::mat4(1.0f);

            //glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
            glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

            v = glm::lookAt(cameraEye,     // "eye"
                cameraTarget,  // "at"
                upVector);

            glUseProgram(program);

            glUniformMatrix4fv(matModel_Location, 1, GL_FALSE, glm::value_ptr(matModel));
            glUniformMatrix4fv(matView_Location, 1, GL_FALSE, glm::value_ptr(v));
            glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(p));



            // Michael won't push this until later...
            GLint bUseVertexColour_Location = glGetUniformLocation(program, "bUseVertexColour");
            GLint vertexColourOverride_Location = glGetUniformLocation(program, "vertexColourOverride");

            glUniform3f(vertexColourOverride_Location, 0.0f, 1.0f, 1.0f);
            glUniform1f(bUseVertexColour_Location, (float)GL_TRUE);

            
            //Set Vertex Color
            glUniform1f(bUseVertexColour_Location, (float)GL_FALSE);

            //Check if the 1 button has been pressed
            if (isWireframe)                // GL_POINT, GL_LINE, and GL_FILL)
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

            if (gVAOManager.FindDrawInfoByModelName(g_vecMeshes[index]->meshName, modelInfo))
            {
                glBindVertexArray(modelInfo.VAO_ID);

                glDrawElements(GL_TRIANGLES,
                    modelInfo.numberOfIndices,
                    GL_UNSIGNED_INT,
                    (void*)0);

                glBindVertexArray(0);
            }
        }
        // "Present" what we've drawn.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up!
    for (nPhysics::cProjectile* p : projectiles)
    {
        world->RemoveParticle(p);
        delete p;
    }
    delete projectileManager;
    projectiles.clear();
    delete world;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

bool createProjectile(std::string name)
{
    //An iterator to use later on
    std::map<std::string, std::string>::iterator it; 
    sProjectile projectileInfo;
    if (!projectileManager->GetProjectile(name, projectileInfo))
    {
        std::cout << name << " doesnt exist" << std::endl;
        return false;
    }

    //Create a mesh for the firework
    cMesh* tempMesh = new cMesh;
    tempMesh->meshName = MODEL_DIR + projectileInfo.modelFileName;
    
    //find the mass
    float mass = 1.0f;
    it = projectileInfo.attributes.find("Mass");
    if (it != projectileInfo.attributes.end())
    {
        if (IsANumber(it->second))
        {
            mass = std::stof(it->second);
        }
    }

    //find out what type of projectile it is
    nPhysics::cProjectile* particle;
    if (name == "Bullet")
    {
        particle = new nPhysics::cBullet(mass, glm::vec3(0, 1.0f, 0));
    }
    else if (name == "Laser")
    {
        bool isMaxDistance = true;
        //set the max distance
        it = projectileInfo.attributes.find("MaxDistance");
        if (it != projectileInfo.attributes.end())
        {
            if (!IsANumber(it->second))
            {
                isMaxDistance = false;
            }
        }
        else
        {
            isMaxDistance = false;
        }
        //If it finds it set it otherwise use a default value
        if (isMaxDistance)
        {
            particle = new nPhysics::cLaser(mass, glm::vec3(0, 1.0f, 0), std::stof(it->second));
        }
        else
        {
            particle = new nPhysics::cLaser(mass, glm::vec3(0, 1.0f, 0), 1000.0f);
        }
    }
    else if (name == "CannonBall")
    {
        particle = new nPhysics::cCannonBall(mass, glm::vec3(0, 1.0f, 0));
    }
    else if (name == "EnergyBall")
    {
        bool isMaxTime = true;
        //set the max age
        it = projectileInfo.attributes.find("AgeLimit");
        if (it != projectileInfo.attributes.end())
        {
            if (!IsANumber(it->second))
            {
                isMaxTime = false;
            }
        }
        else
        {
            isMaxTime = false;
        }
        //If it finds it set it otherwise use a default value
        if (isMaxTime)
        {
            particle = new nPhysics::cEnergyBall(mass, glm::vec3(0, 1.0f, 0), std::stof(it->second));
        }
        else
        {
            particle = new nPhysics::cEnergyBall(mass, glm::vec3(0, 1.0f, 0), 3.0f);
        }
    }
    else
    {
        //If it doesnt exist just return
        return false;
    }
    //set the size
    it = projectileInfo.attributes.find("Size");
    if (it != projectileInfo.attributes.end())
    {
        if (IsANumber(it->second))
        {
            tempMesh->scale = std::stof(it->second);
        }
    }
    //set the damping
    it = projectileInfo.attributes.find("Damping");
    if (it != projectileInfo.attributes.end())
    {
        if (IsANumber(it->second))
        {
            particle->SetDamping(std::stof(it->second));
        }
    }
    //set the Velocity
    it = projectileInfo.attributes.find("Velocity");
    if (it != projectileInfo.attributes.end())
    {
        if (IsANumber(it->second))
        {
            //Get the rotation matrix
            glm::mat4 tempRotation = canRotationY * canRotationX;
            //Multiply that by the initial position and make it a vec4
            glm::vec4 finalRotation = cannonInitialVec * tempRotation;
            //set the velocity and ignore the 4th value in the vec4
            particle->SetVelocity(glm::normalize(glm::vec3(-finalRotation.x, -finalRotation.y, finalRotation.z)) * std::stof(it->second));
        }
    }

    //set the Acceleration
    it = projectileInfo.attributes.find("Acceleration");
    if (it != projectileInfo.attributes.end())
    {
        if (IsANumber(it->second))
        {
            //Right now were only using y in the acceleration, later we'll add the x,y,z for it
            particle->SetAcceleration(glm::normalize(glm::vec3(0, 1, 0)) * std::stof(it->second));
        }
    }

    //add to particle work
    world->AddParticle(particle);
    //add to projectiles
    projectiles.push_back(particle);
    //add to rendered meshes
    g_vecMeshes.push_back(tempMesh);

    return true;
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