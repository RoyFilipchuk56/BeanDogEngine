#pragma once
#include "GameObject.h"

class PlayerShip : public GameObject
{
public:
	// Gameobject will use the mesh but a different transform and rotation
	PlayerShip(cMesh* mesh, glm::vec3 transform, glm::vec3 rotation);
	// GameObject will use the mesh and its givin transform and rotation
	PlayerShip(cMesh* mesh);
	// GameObject doesnt need mesh
	PlayerShip(glm::vec3 transform);
	virtual ~PlayerShip();

	// From iEntity
	virtual void Update(float deltaTime);
	virtual void UpdateCommands(float deltaTime);
	virtual void SetFriendlyName(std::string name);
	virtual std::string GetFriendlyName();
	virtual cMesh* GetMesh();
	virtual void SetMesh(cMesh* mesh);

	float moveSpeed;
	bool isAlive;
};