#pragma once
#include "iEntity.h"
#include "cMesh.h"
#include "cProjectile.h"
//#include "GlobalItems.h"

class TheCannon : public iEntity
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

	TheCannon(cMesh* mesh);
	virtual ~TheCannon();

	virtual void Update(float deltaTime);
	virtual void UpdateCommands(float deltaTime) = 0;
	virtual void SetFriendlyName(std::string name) = 0;
	virtual std::string GetFriendlyName() = 0;
	virtual cMesh* GetMesh() = 0;
	nPhysics::cProjectile* ShootBullet();
};