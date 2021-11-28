#pragma once

// This represents a single mesh object we are drawing

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

class cMesh
{
public:
	cMesh();
	std::string meshName;
	std::string meshFriendlyName;

	//Human readable and changeable values
	glm::vec3 transformXYZ;
	glm::vec3 rotationXYZ;		// "angle"
	float scale;

	//Current rotation maxtrix of model
	glm::mat4 matRotationX;
	glm::mat4 matRotationY;
	glm::mat4 matRotationZ;
	
	// Sets the overall colour of the object
	//	by overwriting the colour variable
	// HACK: We'll get rid of this once we have textures
	glm::vec4 objectDebugColourRGBA;
	bool bUseObjectDebugColour;
	bool bDontLight;
	// Changes polygon mode to LINES (instead of solid)
	bool bIsWireframe;
	// Turns of the depth buffer check when drawn
	bool bDisableDepthBufferCheck;

	// These are for colouring the ENTIRE object as one colour.
	// Later, these will be replaced:
	// * The diffuse will be replaced by textures
	// * The specular will be replaced by specular and gloss maps
	glm::vec4 wholeObjectDiffuseRGBA;		// The "colour" of the object
	bool bUseWholeObjectDiffuseColour;		// If true, then wholeObject colour (not model vertex) is used

	glm::vec3 wholeObjectSpecularRGB;		// Specular HIGHLIGHT colour (usually the same as the light, or white)
	float wholeObjectShininess_SpecPower;	// 1.0 to ??

	// Alpha Transparency (0.0 to 1.0f)
	float alphaTransparency;

	static const unsigned int MAX_TEXTURES = 8;
	std::string textureNames[MAX_TEXTURES];
	float textureRatios[MAX_TEXTURES];

	// All the "child" mesh objects
	std::vector< cMesh* > vec_pChildMeshes;

	void SetTextureRatiosToZero();
};
