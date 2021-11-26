#include "GlobalItems.h"

bool CleanUp(GLFWwindow* pWindow)
{
	//The managers
	delete gVAOManager;
	delete gShaderManager;
	delete gTheLights;
	delete gParticleWorld;
	
	//Drawn meshes
	for (cMesh* mesh : g_vecMeshes)
	{
		delete mesh;
	}

	//game objects
	delete g_pDebugSphere;
	delete cannon;

	for (nPhysics::cProjectile* proj : projectiles)
	{
		delete proj;
	}
	return true;
}