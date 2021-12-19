#pragma once
#include "GameObject.h"
#include "cLightManager.h"
#include "random_helpers.h"

class Torch : public GameObject
{
public:
	// Gameobject will use the mesh but a different transform and rotation
	Torch(cMesh* mesh, glm::vec3 transform, glm::vec3 rotation);
	// GameObject will use the mesh and its givin transform and rotation
	Torch(cMesh* mesh, char location);
	// GameObject doesnt need mesh
	Torch(glm::vec3 transform);
	virtual ~Torch();

	// From iEntity
	virtual void Update(float deltaTime);
	virtual void UpdateCommands(float deltaTime);
	virtual void SetFriendlyName(std::string name);
	virtual std::string GetFriendlyName();
	virtual cMesh* GetMesh();
	virtual void SetMesh(cMesh* mesh);

	void UpdateLight(cLightManager* lightManager, int light);

	char location;
};
