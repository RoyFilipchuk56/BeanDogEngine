#include "cMesh.h"

cMesh::cMesh()
{
	this->transformXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);

	this->scale = 1.0f;

	this->bIsWireframe = false;
	this->bDisableDepthBufferCheck = false;

	this->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);	// White
	this->bUseObjectDebugColour = false;
	this->bDontLight = false;

	// These are for colouring the entire object as one colour
	//Color
	this->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//Specular highlight
	this->wholeObjectSpecularRGB = glm::vec3(1.0f, 1.0f, 1.0f);
	// Specular highlight power
	this->wholeObjectShininess_SpecPower = 1.0f;
}