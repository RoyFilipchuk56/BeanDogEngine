#pragma once
#include "GameObject.h"

class InvaderLazer : public GameObject
{
public:
	// Gameobject will use the mesh but a different transform and rotation
	InvaderLazer(cMesh* mesh, glm::vec3 transform, glm::vec3 rotation);
	// GameObject will use the mesh and its givin transform and rotation
	InvaderLazer(cMesh* mesh, int movementSpeed);
	// GameObject doesnt need mesh
	InvaderLazer(glm::vec3 transform);
	virtual ~InvaderLazer();

	void SwapMeshes();


	// From iEntity
	virtual void Update(float deltaTime);
	virtual void UpdateCommands(float deltaTime);
	virtual void SetFriendlyName(std::string name);
	virtual std::string GetFriendlyName();
	virtual cMesh* GetMesh();
	virtual void SetMesh(cMesh* mesh);

	float movementSpeed;
	float timeSinceLastSwap;
	float swapTime;
	int curentMesh;
	std::vector<std::string> meshNames;
};