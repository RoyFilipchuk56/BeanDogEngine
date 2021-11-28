#include "GlobalItems.h"


bool StartUp(GLFWwindow* pWindow)
{

	g_pFlyCamera = new cFlyCamera();

	gScene = new SceneManager();

	gShaderManager = new cShaderManager();

	gTheLights = new cLightManager();

	gTextureManager = new cBasicTextureManager();

	gVAOManager = new cVAOManager();

	gParticleWorld = new nPhysics::cParticleWorld(1000);

	return true;
}