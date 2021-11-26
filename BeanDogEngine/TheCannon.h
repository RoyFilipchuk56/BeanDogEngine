#pragma once
#include "cMesh.h"
#include "cProjectile.h"
//#include "GlobalItems.h"

class TheCannon
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
	~TheCannon();

	void Update(float deltaTime);
	nPhysics::cProjectile* ShootBullet();
};