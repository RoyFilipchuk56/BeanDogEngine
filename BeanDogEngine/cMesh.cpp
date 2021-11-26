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
	// Default to 1.0f (all solid)
	this->alphaTransparency = 1.0f;

	// Clear all the textures
	for (unsigned int index = 0; index != cMesh::MAX_TEXTURES; index++)
	{
		this->textureNames[index] = "";
		this->textureRatios[index] = 0.0f;
	}

	//TODO: Change this to source engine texture for default
	this->textureNames[0] = "MissingTexture.bmp";
	this->textureRatios[0] = 1.0f;
}

void cMesh::SetTextureRatiosToZero()
{
	for (unsigned int index = 0; index != cMesh::MAX_TEXTURES; index++)
	{
		this->textureRatios[index] = 0.0f;
	}
	return;
}