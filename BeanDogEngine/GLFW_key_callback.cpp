#include "GlobalItems.h"

#include <sstream>
#include <iostream>

/*static*/ void GLFW_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    float cameraSpeed = 1.0f;
    float objectMovementSpeed = 1.0f;
    float lightMovementSpeed = 1.0f;

    bool bShiftDown = false;
    bool bControlDown = false;
    bool bAltDown = false;

    //    // Shift down?
    //    if ( mods == GLFW_MOD_SHIFT )       // 0x0001   0000 0001
    //    {
    //        // ONLY shift is down
    //    }
    //    // Control down?
    //    if ( mods == GLFW_MOD_CONTROL  )    // 0x0002   0000 0010
    //    // Alt down?
    //    if ( mods == GLFW_MOD_ALT   )       // 0x0004   0000 0100

        //   0000 0111 
        // & 0000 0001
        // -----------
        //   0000 0001 --> Same as the shift mask

        // Use bitwise mask to filter out just the shift
    if ( (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT )
    {
        // Shift is down and maybe other things, too
        bShiftDown = true;
    }
    if ( (mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL )
    {
        // Shift is down and maybe other things, too
        bControlDown = true;
    }
    if ( (mods & GLFW_MOD_ALT) == GLFW_MOD_ALT )
    {
        // Shift is down and maybe other things, too
        bAltDown = true;
    }

    if ((!bShiftDown) && (bControlDown) && (!bAltDown))
    {
        if (key == GLFW_KEY_S && action == GLFW_PRESS)
        {
            std::cout << "Scene Saved" << std::endl;
            gScene->SaveScene("SceneTwo.xml", g_vecMeshes);
        }
    }

    return;
}
