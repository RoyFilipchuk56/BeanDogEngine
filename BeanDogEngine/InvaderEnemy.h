#pragma once
#include "GameObject.h"

class InvaderEnemy : public GameObject
{
public:
	// Gameobject will use the mesh but a different transform and rotation
	InvaderEnemy(cMesh* mesh, glm::vec3 transform, glm::vec3 rotation);
	// GameObject will use the mesh and its givin transform and rotation
	InvaderEnemy(cMesh* mesh);
	// GameObject doesnt need mesh
	InvaderEnemy(glm::vec3 transform);
	virtual ~InvaderEnemy();

	// From iEntity
	virtual void Update(float deltaTime);
	virtual void UpdateCommands(float deltaTime);
	virtual void SetFriendlyName(std::string name);
	virtual std::string GetFriendlyName();
	virtual cMesh* GetMesh();
	virtual void SetMesh(cMesh* mesh);

	void SwapModels();

	float moveAmount;
	std::vector<std::string> meshNames;
	int curentMesh;
};