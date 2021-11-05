#include "GLCommon.h" 
#include "GlobalItems.h"

#include <sstream>
#include <iostream>

#include "cWinContextMenu.h"



// Handle async IO (keyboard, joystick, mouse, etc.)


// This is so the "fly camera" won't pay attention to the mouse if it's 
// not directly over the window. 
bool g_MouseIsInsideWindow = false;

bool vKeyAlreadyPressed = false;
bool bKeyAlreadyPressed = false;

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

        //Toggle Debug
        if (glfwGetKey(pWindow,GLFW_KEY_T) == GLFW_PRESS)
        {
            isDebug = !isDebug;
        }

        //Control Object Placement in Debug
        if (glfwGetKey(pWindow, GLFW_KEY_I) == GLFW_PRESS)
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
        if (glfwGetKey(pWindow, GLFW_KEY_K) == GLFW_PRESS)
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
        if (glfwGetKey(pWindow, GLFW_KEY_J) == GLFW_PRESS)
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
        if (glfwGetKey(pWindow, GLFW_KEY_L) == GLFW_PRESS)
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
        if (glfwGetKey(pWindow, GLFW_KEY_O) == GLFW_PRESS)
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
        if (glfwGetKey(pWindow, GLFW_KEY_U) == GLFW_PRESS)
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
        if (glfwGetKey(pWindow, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
        {
            if (debugObjType == 1)
            {
                curLight++;
                if (curLight >= gTheLights->NUMBER_OF_LIGHTS)
                {
                    curLight = cLightManager::NUMBER_OF_LIGHTS - 1;
                }
            }
        }
        if (glfwGetKey(pWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
        {
            if (debugObjType == 1)
            {
                curLight--;
                if (curLight >= cLightManager::NUMBER_OF_LIGHTS)
                {
                    curLight = 0;
                }
            }
        }

        //Change what type of object you control in debug (lights or meshes)
        if (glfwGetKey(pWindow, GLFW_KEY_TAB) == GLFW_PRESS)
        {
            debugObjType = 1;
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

        // The start
        if (glfwGetKey(pWindow, GLFW_KEY_1) == GLFW_PRESS)
        {
            g_pFlyCamera->eye = glm::vec3(0, 0, 17);
            //g_pFlyCamera->m_at;
        }

        //The Hanger
        if (glfwGetKey(pWindow, GLFW_KEY_2) == GLFW_PRESS)
        {
            g_pFlyCamera->eye = glm::vec3(22.5, 0, 60.0);
        }

        std::stringstream strTitle;
        // std::cout << 
        glm::vec3 cameraEye = ::g_pFlyCamera->getEye();
        strTitle << "Camera: "
            << cameraEye.x << ", "
            << cameraEye.y << ", "
            << cameraEye.z; //<< std::endl;

        ::g_TitleText = strTitle.str();

    }


    
    // If JUST the shift is down, move the "selected" object
    if ( cGFLWKeyboardModifiers::isModifierDown(pWindow, true, false, false ) )
    {
        std::cout << "Is this thing woprking" << std::endl;
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


    // If JUST the ALT is down, move the "selected" light
    if ( cGFLWKeyboardModifiers::isModifierDown(pWindow, false, true, false) )
    {
        if ( glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS ) { 
            std::cout << "POG" << std::endl;
            ::gTheLights->theLights[::g_selectedLight].position.x -= lightMovementSpeed; 
        } // Go left
        if ( glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS ) { ::gTheLights->theLights[::g_selectedLight].position.x += lightMovementSpeed; } // Go right
        if ( glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS ) { ::gTheLights->theLights[::g_selectedLight].position.z += lightMovementSpeed; }// Go forward 
        if ( glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS ) { ::gTheLights->theLights[::g_selectedLight].position.z -= lightMovementSpeed; }// Go backwards
        if ( glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS ) { ::gTheLights->theLights[::g_selectedLight].position.y -= lightMovementSpeed; }// Go "Down"
        if ( glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS ) { ::gTheLights->theLights[::g_selectedLight].position.y += lightMovementSpeed; }// Go "Up"

        // constant attenuation
        if ( glfwGetKey(pWindow, GLFW_KEY_1) == GLFW_PRESS )
        {
            ::gTheLights->theLights[::g_selectedLight].atten.x *= 0.99f; // -1% less
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_2) == GLFW_PRESS )
        {
            // Is it at (or below) zero?
            if ( ::gTheLights->theLights[::g_selectedLight].atten.x <= 0.0f )
            {
                // Set it to some really small initial attenuation
                ::gTheLights->theLights[::g_selectedLight].atten.x = 0.001f;
            }
            ::gTheLights->theLights[::g_selectedLight].atten.x *= 1.01f; // +1% more
        }
        // linear attenuation
        if ( glfwGetKey(pWindow, GLFW_KEY_3) == GLFW_PRESS )
        {
            ::gTheLights->theLights[::g_selectedLight].atten.y *= 0.99f; // -1% less
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_4) == GLFW_PRESS )
        {
            // Is it at (or below) zero?
            if ( ::gTheLights->theLights[::g_selectedLight].atten.y <= 0.0f )
            {
                // Set it to some really small initial attenuation
                ::gTheLights->theLights[::g_selectedLight].atten.y = 0.001f;
            }
            ::gTheLights->theLights[::g_selectedLight].atten.y *= 1.01f; // +1% more
        }
        // quadratic attenuation
        if ( glfwGetKey(pWindow, GLFW_KEY_5) == GLFW_PRESS )
        {
            ::gTheLights->theLights[::g_selectedLight].atten.z *= 0.99f; // -1% less
        }
        if ( glfwGetKey(pWindow, GLFW_KEY_6) == GLFW_PRESS )
        {
            // Is it at (or below) zero?
            if ( ::gTheLights->theLights[::g_selectedLight].atten.z <= 0.0f )
            {
                // Set it to some really small initial attenuation
                ::gTheLights->theLights[::g_selectedLight].atten.z = 0.001f;
            }
            ::gTheLights->theLights[::g_selectedLight].atten.z *= 1.01f; // +1% more
        }

        if ( glfwGetKey(pWindow, GLFW_KEY_U) == GLFW_PRESS ) { ::gTheLights->theLights[0].param1.y -= 0.5f; }   // Inner
        if ( glfwGetKey(pWindow, GLFW_KEY_I) == GLFW_PRESS ) { ::gTheLights->theLights[0].param1.y += 0.5f; }   // Inner
        if ( glfwGetKey(pWindow, GLFW_KEY_O) == GLFW_PRESS ) { ::gTheLights->theLights[0].param1.z -= 0.5f; }   // Outer
        if ( glfwGetKey(pWindow, GLFW_KEY_P) == GLFW_PRESS ) { ::gTheLights->theLights[0].param1.z += 0.5f; }   // Outer


        std::stringstream strTitle;
        // std::cout << 
        strTitle << "Light # " << ::g_selectedLight << " positionXYZ : "
            << ::gTheLights->theLights[::g_selectedLight].position.x << ", "
            << ::gTheLights->theLights[::g_selectedLight].position.y << ", "
            << ::gTheLights->theLights[::g_selectedLight].position.z << "  "
            << "attenuation (C, L, Q): "
            << ::gTheLights->theLights[::g_selectedLight].atten.x << ", "        // Const
            << ::gTheLights->theLights[::g_selectedLight].atten.y << ", "        // Linear
            << ::gTheLights->theLights[::g_selectedLight].atten.z << "  "        // Quadratic
            << (::gTheLights->theLights[::g_selectedLight].param2.x > 0.0f ? " is on" : " is off");
        //<< std::endl;
        ::g_TitleText = strTitle.str();


    }//if ( cGFLWKeyboardModifiers::...
    
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
        std::cout << "Mouse cursor is over the window" << std::endl;
        ::g_MouseIsInsideWindow = true;
    }
    else
    {
        std::cout << "Mouse cursor is no longer over the window" << std::endl;
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

#ifdef YO_NERDS_WE_USING_WINDOWS_CONTEXT_MENUS_IN_THIS_THANG
    // Right button is pop-up
    if ( button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS )
    {
        ShowWindowsContextMenu(window, button, action, mods);
    }
#endif

    return;
}


void GLFW_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    return;
}


