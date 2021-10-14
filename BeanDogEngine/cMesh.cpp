#include "cMesh.h"

cMesh::cMesh()
{
	this->transformXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);

	this->scale = 1.0f;

	this->bIsWireframe = false;

	this->vertexColourOverrideHACK = glm::vec3(1.0f, 1.0f, 1.0f);	// White
	this->bOverriveVertexColourHACK = false;
}