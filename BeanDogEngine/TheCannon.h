#pragma once
#include "GameObject.h"
#include "cMesh.h"
#include "cProjectile.h"
//#include "GlobalItems.h"

class TheCannon : public GameObject
{
private:
	glm::vec4 initialVec;

public:
	bool isMovingLeft;
	bool isMovingRight;
	bool isMovingUp;
	bool isMovingDown;
	
	cMesh* myMesh;
	float movementSpeed;
	float pitchMin, pitchMax, yawMin, yawMax;
	float projectileRadius;
	// Gameobject will use the mesh but a different transform and rotation
	TheCannon(cMesh* mesh, glm::vec3 transform, glm::vec3 rotation);
	// GameObject will use the mesh and its givin transform and rotation
	TheCannon(cMesh* mesh);
	// GameObject doesnt need mesh
	TheCannon(glm::vec3 transform);
	virtual ~TheCannon();

	virtual void Update(float deltaTime);
	virtual void UpdateCommands(float deltaTime);
	virtual void SetFriendlyName(std::string name);
	virtual std::string GetFriendlyName();
	virtual cMesh* GetMesh();
	virtual void SetMesh(cMesh* mesh);
	nPhysics::cProjectile* ShootBullet();
	void SetCannonConfig(float pitchMin, float pitchMax, float yawMin, float yawMax, float maxSpeed);
	void SetProjectileSize(float radius);
};