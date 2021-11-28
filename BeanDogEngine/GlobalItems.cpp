#include "GlobalItems.h"

cFlyCamera* g_pFlyCamera = NULL;

SceneManager* gScene = NULL;
cVAOManager* gVAOManager = NULL;
cShaderManager* gShaderManager = NULL;
cLightManager* gTheLights = NULL;
cBasicTextureManager* gTextureManager = NULL;

nPhysics::cParticleWorld* gParticleWorld = NULL;

// List of objects to draw
std::vector< cMesh* > g_vecMeshes;

cMesh* g_pDebugSphere = NULL;
bool g_bShowDebugShere = true;

unsigned int g_selectedObject = 0;
unsigned int g_selectedLight = 0;

// This will be printed in the title bar
std::string g_TitleText = "";

//Is debug mode on
bool isDebug = false;
//Current selected debug light
int curLight = 0;
//current selected debug Mesh
int curMesh = 0;
//Type of object curently controlling
//0 = Meshes 1 = Lights
int debugObjType = 0;
//Debug object move amount
float objectMoveValue = 0.03f;