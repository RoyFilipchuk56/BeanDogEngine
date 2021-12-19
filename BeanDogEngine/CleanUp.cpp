#include "GlobalItems.h"

bool CleanUp(GLFWwindow* pWindow)
{
	//The managers
	delete gVAOManager;
	delete gShaderManager;
	delete gTheLights;
	delete gParticleWorld;

	//game objects
	delete g_pDebugSphere;

	return true;
}