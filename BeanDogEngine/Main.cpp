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
#include "cFirework.h"
#include "particle_force_generators.h"

// Generate a random number between 0 and 1
float getRandom();

// Generate a random number between zero and a given high value
float getRandom(float high);

// Generate a random number in a given range
float getRandom(float low, float high);

// Returns a vector laying on the x-z plane, randomized in direction and magnitude.
// The output is designed to be linearly independent from the output of getRandomZVector()
glm::vec3 getRandomXVector();

// Returns a vector laying on the x-z plane, randomized in direction and magnitude.
// The output is designed to be linearly independent from the output of getRandomXVector()
glm::vec3 getRandomZVector();

// Determine from the parameters if the particle is currently above the ground.
bool particleIsAboveGround(glm::mat3& axes, float& deltaTime, float& timeElapsed, glm::vec3& position, glm::vec3& velocity);

// Determine from the parameters if the particle is currently moving "up".
bool particleIsMovingUpward(glm::mat3& axes, float& deltaTime, float& timeElapsed, glm::vec3& position, glm::vec3& velocity);

glm::mat3 orthonormalBasis(const glm::vec3& xVec, const glm::vec3& zVec);

void InitProject1Variables(glm::mat3& axes, nPhysics::cParticle* particle);

// Global Variables
glm::vec3 cameraEye;
cShaderManager  gShaderManager;
cVAOManager     gVAOManager;
std::vector<cMesh> g_vecMeshes;
bool isWireframe = false;
nPhysics::cParticleWorld* world;
nPhysics::cParticleGravityGenerator gravityGenerator(glm::vec3(0.0f, -9.81f, 0.0f));
std::vector<nPhysics::cFirework*> fireworks;
std::map<nPhysics::cParticle*, cMesh*> particleMap;


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

    float cameraSpeed = 0.1f;

    // Basic camera controls
    if (key == GLFW_KEY_A)
    {
        cameraEye.x -= cameraSpeed;     // Go left
    }
    if (key == GLFW_KEY_D)
    {
        cameraEye.x += cameraSpeed;     // Go right
    }

    if (key == GLFW_KEY_W)
    {
        cameraEye.z += cameraSpeed;     // Go forward
    }
    if (key == GLFW_KEY_S)
    {
        cameraEye.z -= cameraSpeed;     // Go backwards
    }

    if (key == GLFW_KEY_Q)
    {
        cameraEye.y -= cameraSpeed;     // Go "Down"
    }
    if (key == GLFW_KEY_E)
    {
        cameraEye.y += cameraSpeed;     // Go "Up"
    }
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        isWireframe = !isWireframe;     //toggle wireframe
    }
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        cMesh shpereMesh;
        shpereMesh.meshName = "sphere.ply";
        shpereMesh.scale = 0.1f;
        g_vecMeshes.push_back(shpereMesh);

        //create particle
        nPhysics::cFirework* particle = new nPhysics::cFirework(1.0f, glm::vec3(0.f));
        particle->SetStageOne();
        if (world->AddParticle(particle))
        {
            std::cout << "Hurray!" << std::endl;
        }
        world->GetForceRegistry()->Register(particle, &gravityGenerator);
        fireworks.push_back(particle);


        particleMap[particle] = &(g_vecMeshes[g_vecMeshes.size() - 1]);
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

    //load in each models in the scene
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
        cMesh tempMesh;
        tempMesh.meshName = MODEL_DIR + scene.currentLevel.models[i].fileName;
        tempMesh.transformXYZ = scene.currentLevel.models[i].transform;
        tempMesh.rotationXYZ = scene.currentLevel.models[i].rotation;
        tempMesh.scale = scene.currentLevel.models[i].scale;
        g_vecMeshes.push_back(tempMesh);
    }

    //make a sphere model info
    sModelDrawInfo sphereInfo;
    if (!gVAOManager.LoadModelIntoVAO("sphere.ply", sphereInfo, program))
    {
        std::cout << "Error: " << "sphere.phy" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "sphere.phy" << " loaded OK" << std::endl;
        std::cout << sphereInfo.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << sphereInfo.numberOfTriangles << " triangles loaded" << std::endl;
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
        float deltaTime = currentTime - previousTime;
        previousTime = currentTime;


        //timestep
        world->TimeStep(deltaTime);
        //update firework positions
        /*
        int particleNum = 0;
        std::map<nPhysics::cParticle*, cMesh*>::iterator particleIterator;
        for (particleIterator = particleMap.begin(); particleIterator != particleMap.end(); particleIterator++)
        {
            particleIterator->second->transformXYZ = particleIterator->first->GetPosition();
            std::cout << "Particle " << particleNum++ << " Mesh: " << particleIterator->second->rotationXYZ.y << " Particle: " << particleIterator->first->GetPosition().y << " g_Mesh: " << g_vecMeshes[1].transformXYZ.y << std::endl;
        }
        */
        if (fireworks.size() > 0)
        {
            for (int i = 0; i < fireworks.size(); i++)
            {
                //If the firework isnt alive
                if (!fireworks[i]->IsAlive())
                {
                    //if the firework is in stage 1
                    if (fireworks[i]->GetStage() == 1)
                    {
                        //create a new list
                        std::vector<nPhysics::cFirework*> stageTwoList;
                        //get the children
                        fireworks[i]->GenerateChildren(stageTwoList);
                        if (stageTwoList.size() > 0)
                        {
                            for (nPhysics::cFirework* p : stageTwoList)
                            {
                                cMesh sphereMesh;
                                sphereMesh.meshName = "sphere.ply";
                                sphereMesh.scale = 0.1f;
                                g_vecMeshes.push_back(sphereMesh);

                                nPhysics::cFirework* particle = new nPhysics::cFirework(1.0f, fireworks[i]->GetPosition());
                                particle->SetStageTwo();
                                if (world->AddParticle(particle))
                                {
                                    std::cout << "Hurray!" << std::endl;
                                }
                                world->GetForceRegistry()->Register(particle, &gravityGenerator);
                                fireworks.push_back(particle);
                            }
                        }
                    }
                    //remove particle from world, delete it and erase all records of its existance
                    world->RemoveParticle(fireworks[i]);
                    delete fireworks[i];
                    fireworks.erase(fireworks.begin() + i);
                    g_vecMeshes.erase(g_vecMeshes.begin() + i + 1);
                }
                else
                {
                    g_vecMeshes[i + 1].transformXYZ = fireworks[i]->GetPosition();
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
            cMesh curMesh = g_vecMeshes[index];


            matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
            //mat4x4_identity(m);


            // *****************************************************
            // Translate or "move" the object somewhere
            glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
                curMesh.transformXYZ);

            //matModel = matModel * matTranslate;
            // *****************************************************


            // *****************************************************
            // Rotation around the Z axis
            glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
                curMesh.rotationXYZ.z,//(float)glfwGetTime(),
                glm::vec3(0.0f, 0.0f, 1.0f));

            //matModel = matModel * rotateZ;
            // *****************************************************

            // *****************************************************
            // Rotation around the Y axis
            glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
                curMesh.rotationXYZ.y,
                glm::vec3(0.0f, 1.0f, 0.0f));

            //matModel = matModel * rotateY;
            // *****************************************************

            // *****************************************************
            // Rotation around the X axis
            glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
                curMesh.rotationXYZ.x,
                glm::vec3(1.0f, 0.0f, 0.0f));

            //matModel = matModel * rotateX;
            // *****************************************************


            // *****************************************************
            // Scale the model
            glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                glm::vec3(curMesh.scale,  // Scale in X
                    curMesh.scale,  // Scale in Y
                    curMesh.scale));// Scale in Z

//matModel = matModel * matScale;
// *****************************************************

// *****************************************************
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

            if (gVAOManager.FindDrawInfoByModelName(g_vecMeshes[index].meshName, modelInfo))
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
    for (nPhysics::cFirework* p : fireworks)
    {
        world->RemoveParticle(p);
        delete p;
    }
    fireworks.clear();
    delete world;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

float getRandom()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float getRandom(float high)
{
    return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / high);
}

float getRandom(float low, float high)
{
    return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (high - low));
}

// Use as-is.  Do not change.
glm::vec3 getRandomXVector()
{
    return glm::vec3(getRandom(0.1f, 1.f), 0.f, getRandom(0.1f, 1.f));
}

// Use as-is.  Do not change.
glm::vec3 getRandomZVector()
{
    return glm::vec3(-getRandom(0.1f, 1.f), 0.f, getRandom(0.1f, 1.f));
}

bool particleIsAboveGround(glm::mat3& axes, float& deltaTime, float& timeElapsed, glm::vec3& position, glm::vec3& velocity)
{
    // TODO: Use the parameters to determine if the particle
    //       is currently above the ground.
    //       Return true if the particle is above the ground, false otherwise.
    return position.y > 1.0f; // because our "sphere" has a radius of 1
}

bool particleIsMovingUpward(glm::mat3& axes, float& deltaTime, float& timeElapsed, glm::vec3& position, glm::vec3& velocity)
{
    // TODO: Use the parameters to determine if the particle
    //       is currently moving upward.
    //       Return true if the particle is above the ground, false otherwise.
    return glm::dot(velocity, axes[1]) > 0;
}

glm::mat3 orthonormalBasis(const glm::vec3& xVec, const glm::vec3& zVec)
{
    // TODO: Generate an orthonormal basis, using xVec and zVec.
    //       The input vectors may be manipulated, however the 
    //       returned axes must essentially be:
    //       x-axis: sourced from xVec
    //       y-axis: generated using math!
    //       z-axis: sourced from zVec

    // Generate y, by crossing z and x.
    glm::vec3 x(xVec);
    glm::vec3 z(zVec);
    glm::vec3 y(glm::cross(z, x));
    // Ensure z is orthogonal to both x and y.
    z = glm::cross(x, y);
    // Normalize everything.
    x = glm::normalize(x);
    y = glm::normalize(y);
    z = glm::normalize(z);
    // Return the result.
    return glm::mat3(x, y, z);
}

void InitProject1Variables(glm::mat3& axes, nPhysics::cParticle* particle)
{
    axes = orthonormalBasis(getRandomXVector(), getRandomZVector());
    // because our "sphere" has a radius of 1
    glm::vec3 position(0.0, 1.1f, 0.0);
    glm::vec3 velocity = { 0, 2, 0 };
    velocity = glm::normalize(velocity);
    velocity *= 10.f;
    particle->SetPosition(position);
    particle->SetVelocity(velocity);
}