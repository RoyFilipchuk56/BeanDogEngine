#pragma once
#include "GLCommon.h"

#include "cFlyCamera.h"
#include "SceneManager.h"
#include "cVAOManager.h"
#include "cShaderManager.h"
#include "cLightManager.h"
#include "cBasicTextureManager.h"
#include "cParticleWorld.h"
#include "cProjectile.h"
#include "cMesh.h"
#include "TheCannon.h"
#include <vector>

// These are all things that are "project global", 
//	so need to be visible to many files and parts

// Creates global things, etc. (to control the order)
bool StartUp(GLFWwindow* pWindow);

// Deletes global things, etc. 
bool CleanUp(GLFWwindow* pWindow);


//Fly Camera                          
extern cFlyCamera* g_pFlyCamera; // = NULL;

// This the keyboard based eye location
// Like if you AREN'T using the fly camera
//extern glm::vec3 g_cameraEye;	//	= glm::vec3(0.0f, 0.0f, +4.0f);
//extern glm::vec3 g_cameraAt;	//	= glm::vec3(0.0f, 0.0f, 0.0f);
// ************************************************


extern SceneManager* gScene;
extern cVAOManager* gVAOManager;
extern cShaderManager* gShaderManager;
extern cLightManager* gTheLights;
extern cBasicTextureManager* gTextureManager;
extern nPhysics::cParticleWorld* gParticleWorld;

extern cMesh* g_pDebugSphere;
extern bool g_bShowDebugShere;

extern unsigned int g_selectedObject;
extern unsigned int g_selectedLight;


// This will be printed in the title bar
extern std::string g_TitleText;

// List of objects to draw
extern std::vector< cMesh* > g_vecMeshes;
//List of transparent Objects
extern std::vector<cMesh*> g_transMeshes;

//Is debug mode on
extern bool isDebug;
//Current selected debug light
extern int curLight;
//current selected debug Mesh
extern int curMesh;
//Type of object curently controlling
//0 = Meshes 1 = Lights
extern int debugObjType;
//Debug object move amount
extern float objectMoveValue;
//Debug object wireframe or not
extern bool isWireframe;

//Projectile list
extern std::vector<nPhysics::cProjectile*> projectiles;

//Keyboard Modifiers
class cGFLWKeyboardModifiers
{
public:
    static bool isLeftShiftKeyDown(GLFWwindow* pWindow);
    static bool isRightShiftKeyDown(GLFWwindow* pWindow);
    static bool isShiftKeyDown(GLFWwindow* pWindow);        // Either down

    static bool isLeftControlKeyDown(GLFWwindow* pWindow);
    static bool isRightControlKeyDown(GLFWwindow* pWindow);
    static bool isControlKeyDown(GLFWwindow* pWindow);      // Either down

    static bool isLeftAltKeyDown(GLFWwindow* pWindow);
    static bool isRightAltKeyDown(GLFWwindow* pWindow);
    static bool isAltKeyDown(GLFWwindow* pWindow);          // Either down

    // Returns true if this pattern matches
    // example: (true, false, false) returns true if ONLY shift is down
    static bool isModifierDown(GLFWwindow* pWindow, bool bShift, bool bControl, bool bAlt);

    static bool areAllModsUp(GLFWwindow* pWindow);
};

void handleAsyncKeyboard(GLFWwindow* pWindow, double deltaTime);
void handleAsyncMouse(GLFWwindow* pWindow, double deltaTime);

//GLFW Callbacks
void GLFW_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLFW_error_callback(int error, const char* description);
void GLFW_cursor_enter_callback(GLFWwindow* window, int entered);
void GLFW_cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void GLFW_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void GLFW_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void GLFW_window_size_callback(GLFWwindow* window, int width, int height);