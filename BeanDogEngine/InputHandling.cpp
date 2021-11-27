#include "GLCommon.h" 
#include "GlobalItems.h"

#include <sstream>
#include <iostream>

#include "cWinContextMenu.h"



// Handle async IO (keyboard, joystick, mouse, etc.)


// This is so the "fly camera" won't pay attention to the mouse if it's 
// not directly over the window. 
bool g_MouseIsInsideWindow = false;

bool isSpacePressed = false;
bool isTabPressed = false;
bool isTPressed = false;
bool isLeftBracketPressed = false;
bool isRightBracketPressed = false;

void handleAsyncKeyboard(GLFWwindow* pWindow, double deltaTime)
{
    float cameraMoveSpeed = ::g_pFlyCamera->movementSpeed;

    float objectMovementSpeed = 0.1f;
    float lightMovementSpeed = 10.0f;

    if ( cGFLWKeyboardModifiers::areAllModsUp(pWindow) )
    {
        // Use "fly" camera (keyboard for movement, mouse for aim)
        if ( glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS )
        {
            ::g_pFlyCamera->MoveForward_Z(+cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS )	// "backwards"
        {
            ::g_pFlyCamera->MoveForward_Z(-cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS )	// "left"
        {
            ::g_pFlyCamera->MoveLeftRight_X(-cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS )	// "right"
        {
            ::g_pFlyCamera->MoveLeftRight_X(+cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS )	// "up"
        {
            ::g_pFlyCamera->MoveUpDown_Y(-cameraMoveSpeed);
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS )	// "down"
        {
            ::g_pFlyCamera->MoveUpDown_Y(+cameraMoveSpeed);
        }
        
        //Cannon Movement

        //Toggle Debug
        if (glfwGetKey(pWindow,GLFW_KEY_T) == GLFW_PRESS)
        {
            if (!isTPressed)
            {
                isTPressed = true;
                isDebug = !isDebug;
                if (isDebug)
                {
                    std::cout << "Debug mode on" << std::endl;
                }
                
                else
                {
                    std::cout << "Debug mode off" << std::endl;
                }
            }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_T) == GLFW_RELEASE)
        {
            isTPressed = false;
        }

        //Control Object Placement in Debug
        if (glfwGetKey(pWindow, GLFW_KEY_I) == GLFW_PRESS)
        {
            if (isDebug)
            {
                if (debugObjType == 0)
                {
                    g_vecMeshes[curMesh]->transformXYZ += glm::vec3(0, objectMoveValue, 0);
                }

                else if (debugObjType == 1)
                {
                    gTheLights->theLights[curLight].position += glm::vec4(0, objectMoveValue, 0, 1);
                    std::cout << "Light " << curLight << " - x: " << gTheLights->theLights[curLight].position.x << " y: " << gTheLights->theLights[curLight].position.y << " z: " << gTheLights->theLights[curLight].position.z << std::endl;
                }
            }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_K) == GLFW_PRESS)
        {
            if (isDebug)
            {
                if (debugObjType == 0)
                {
                    g_vecMeshes[curMesh]->transformXYZ += glm::vec3(0, -objectMoveValue, 0);
                }

                else if (debugObjType == 1)
                {
                    gTheLights->theLights[curLight].position += glm::vec4(0, -objectMoveValue, 0, 1);
                    std::cout << "Light " << curLight << " - x: " << gTheLights->theLights[curLight].position.x << " y: " << gTheLights->theLights[curLight].position.y << " z: " << gTheLights->theLights[curLight].position.z << std::endl;
                }
            }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_J) == GLFW_PRESS)
        {
            if (isDebug)
            {
                if (debugObjType == 0)
                {
                    g_vecMeshes[curMesh]->transformXYZ += glm::vec3(objectMoveValue, 0, 0);
                }

                else if (debugObjType == 1)
                {
                    gTheLights->theLights[curLight].position += glm::vec4(objectMoveValue, 0, 0, 1);
                    std::cout << "Light " << curLight << " - x: " << gTheLights->theLights[curLight].position.x << " y: " << gTheLights->theLights[curLight].position.y << " z: " << gTheLights->theLights[curLight].position.z << std::endl;
                }
            }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_L) == GLFW_PRESS)
        {
            if (isDebug)
            {
                if (debugObjType == 0)
                {
                    g_vecMeshes[curMesh]->transformXYZ += glm::vec3(-objectMoveValue, 0, 0);
                }

                else if (debugObjType == 1)
                {
                    gTheLights->theLights[curLight].position += glm::vec4(-objectMoveValue, 0, 0, 1);
                    std::cout << "Light " << curLight << " - x: " << gTheLights->theLights[curLight].position.x << " y: " << gTheLights->theLights[curLight].position.y << " z: " << gTheLights->theLights[curLight].position.z << std::endl;
                }
            }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_O) == GLFW_PRESS)
        {
            if (isDebug)
            {
                if (debugObjType == 0)
                {
                    g_vecMeshes[curMesh]->transformXYZ += glm::vec3(0, 0, objectMoveValue);
                }

                else if (debugObjType == 1)
                {
                    gTheLights->theLights[curLight].position += glm::vec4(0, 0, objectMoveValue, 1);
                    std::cout << "Light " << curLight << " - x: " << gTheLights->theLights[curLight].position.x << " y: " << gTheLights->theLights[curLight].position.y << " z: " << gTheLights->theLights[curLight].position.z << std::endl;
                }
            }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_U) == GLFW_PRESS)
        {
            if (isDebug)
            {
                if (debugObjType == 0)
                {
                    g_vecMeshes[curMesh]->transformXYZ += glm::vec3(0, 0, -objectMoveValue);
                }

                else if (debugObjType == 1)
                {
                    gTheLights->theLights[curLight].position += glm::vec4(0, 0, -objectMoveValue, 1);
                    std::cout << "Light " << curLight << " - x: " << gTheLights->theLights[curLight].position.x << " y: " << gTheLights->theLights[curLight].position.y << " z: " << gTheLights->theLights[curLight].position.z << std::endl;
                }
            }
        }
        //change the object you control in debug
        if (glfwGetKey(pWindow, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
        {
            if (isDebug)
            {
                if (!isRightBracketPressed)
                {
                    isRightBracketPressed = true;
                    if (debugObjType == 0)
                    {
                        curMesh++;
                        if (curMesh >= g_vecMeshes.size())
                        {
                            curMesh = g_vecMeshes.size() - 1;
                        }
                        std::cout << "Current Mesh: " << curMesh << std::endl;
                    }

                    else if (debugObjType == 1)
                    {
                        curLight++;
                        if (curLight >= gTheLights->NUMBER_OF_LIGHTS)
                        {
                            curLight = cLightManager::NUMBER_OF_LIGHTS - 1;
                        }
                        std::cout << "Current Light: " << curLight << std::endl;
                    }
                }
            }
        }

        if (glfwGetKey(pWindow, GLFW_KEY_RIGHT_BRACKET) == GLFW_RELEASE)
        {
            isRightBracketPressed = false;
        }

        if (glfwGetKey(pWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
        {
            if (isDebug)
            {
                if (!isLeftBracketPressed)
                {
                    isLeftBracketPressed = true;

                    if (debugObjType == 0)
                    {
                        curMesh--;
                        if (curMesh >= g_vecMeshes.size())
                        {
                            curMesh = 0;
                        }
                        std::cout << "Current Mesh: " << curMesh << std::endl;
                    }

                    if (debugObjType == 1)
                    {
                        curLight--;
                        if (curLight >= cLightManager::NUMBER_OF_LIGHTS)
                        {
                            curLight = 0;
                        }
                        std::cout << "Current Light: " << curLight << std::endl;
                    }
                }
            }
        }

        if (glfwGetKey(pWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_RELEASE)
        {
            isLeftBracketPressed = false;
        }

        //Change what type of object you control in debug (lights or meshes)
        if (glfwGetKey(pWindow, GLFW_KEY_TAB) == GLFW_PRESS)
        {
            if (isDebug)
            {
                if (!isTabPressed)
                {
                    isTabPressed = true;
                    debugObjType++;
                    if (debugObjType == 1)
                    {
                        std::cout << "Debug Lights" << std::endl;
                    }
                    else
                    {
                        std::cout << "Debug Objects" << std::endl;
                    }

                    if (debugObjType > 1)
                    {
                        debugObjType = 0;
                    }
                }
            }
        }

        if (glfwGetKey(pWindow, GLFW_KEY_TAB) == GLFW_RELEASE)
        {
            isTabPressed = false;
        }

        // Reset the camera
        if (glfwGetKey(pWindow, GLFW_KEY_1) == GLFW_PRESS)
        {
            g_pFlyCamera->setEye(glm::vec3(0.0f, 30.09f, -116.68f));
            //g_pFlyCamera->m_at;
        }
    }


    // This stays for an example of modifier Keys
    // If JUST the shift is down, move the "selected" object
    
    /*
    if ( cGFLWKeyboardModifiers::isModifierDown(pWindow, true, false, false ) )
    {
        if ( glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS ) { ::g_vecMeshes[::g_selectedObject]->transformXYZ.x -= objectMovementSpeed; } // Go left
        if ( glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS ) { ::g_vecMeshes[::g_selectedObject]->transformXYZ.x += objectMovementSpeed; } // Go right
        if ( glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS ) { ::g_vecMeshes[::g_selectedObject]->transformXYZ.z += objectMovementSpeed; }// Go forward 
        if ( glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS ) { ::g_vecMeshes[::g_selectedObject]->transformXYZ.z -= objectMovementSpeed; }// Go backwards
        if ( glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS ) { ::g_vecMeshes[::g_selectedObject]->transformXYZ.y -= objectMovementSpeed; }// Go "Down"
        if ( glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS ) { ::g_vecMeshes[::g_selectedObject]->transformXYZ.y += objectMovementSpeed; }// Go "Up"

        std::stringstream strTitle;
        // std::cout << 
        strTitle << "::g_vec_pMeshes[" << ::g_selectedObject << "].positionXYZ : "
            << ::g_vecMeshes[::g_selectedObject]->transformXYZ.x << ", "
            << ::g_vecMeshes[::g_selectedObject]->transformXYZ.y << ", "
            << ::g_vecMeshes[::g_selectedObject]->transformXYZ.z;// << std::endl;

        ::g_TitleText = strTitle.str();

        // TODO: Add some controls to change the "selected object"
        // i.e. change the ::g_selectedObject value


    }//if ( cGFLWKeyboardModifiers::...
    */
    return;
}


// We call these every frame
void handleAsyncMouse(GLFWwindow* window, double deltaTime)
{

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    ::g_pFlyCamera->setMouseXY(x, y);

    const float MOUSE_SENSITIVITY = 5.0f;


    // Mouse left (primary?) button pressed? 
    // AND the mouse is inside the window...
    if ( (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        && ::g_MouseIsInsideWindow )
    {
        // Mouse button is down so turn the camera
        ::g_pFlyCamera->Yaw_LeftRight( ::g_pFlyCamera->getDeltaMouseX() * MOUSE_SENSITIVITY, deltaTime );

        ::g_pFlyCamera->Pitch_UpDown( -::g_pFlyCamera->getDeltaMouseY() * MOUSE_SENSITIVITY, deltaTime );

    }

    // Adjust the mouse speed
    if ( ::g_MouseIsInsideWindow )
    {
        const float MOUSE_WHEEL_SENSITIVITY = 0.1f;

        // Adjust the movement speed based on the wheel position
        ::g_pFlyCamera->movementSpeed -= ( ::g_pFlyCamera->getMouseWheel() * MOUSE_WHEEL_SENSITIVITY );

        // Clear the mouse wheel delta (or it will increase constantly)
        ::g_pFlyCamera->clearMouseWheelValue();


        if ( ::g_pFlyCamera->movementSpeed <= 0.0f )
        {
            ::g_pFlyCamera->movementSpeed = 0.0f;
        }
    }


    return;
}

void GLFW_cursor_enter_callback(GLFWwindow* window, int entered)
{
    if ( entered )
    {
        ::g_MouseIsInsideWindow = true;
    }
    else
    {
        ::g_MouseIsInsideWindow = false;
    }
    return;
}

// Called when the mouse scroll wheel is moved
void GLFW_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    float mouseScrollWheelSensitivity = 0.1f;

    ::g_pFlyCamera->setMouseWheelDelta(yoffset * mouseScrollWheelSensitivity);

    return;
}

void GLFW_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    return;
}


void GLFW_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    return;
}


