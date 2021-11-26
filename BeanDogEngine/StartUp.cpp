#include "GlobalItems.h"


bool StartUp(GLFWwindow* pWindow)
{

	g_pFlyCamera = new cFlyCamera();

	// Create the shader manager
	gShaderManager = new cShaderManager();

	gTheLights = new cLightManager();

	gVAOManager = new cVAOManager();

	gParticleWorld = new nPhysics::cParticleWorld(1000);

	return true;
}