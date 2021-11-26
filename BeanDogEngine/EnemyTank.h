#pragma once
#include "iTank.h"
#include "GlobalItems.h"
#include "random_helpers.h"

enum TankState
{
	Waiting,
	Moving,
	Blocked,
	Shooting
};

enum Directions
{
	Up,
	Down,
	Left,
	Right
};

class EnemyTank : public iTank
{
private:
	iMessage* mediator;
protected:
	//The tank mesh
	cMesh* myMesh;
	//The position in the world acording to mapManager array
	glm::vec3 myPosition;
	//Move speed
	float moveSpeed;
	//Time since last move
	float timeSinceMove;
	//The current tank state
	TankState state;
	//The current chosen direction
	Directions curDirection;
	//The last known direction;
	glm::vec3 lastDirection;
	//Can the tank move
	bool canMove;
	//If we can move or not
	bool bulletIsAlive;
	//the health
	float health;
	//max health/starting health
	float maxHealth;
	//If its alive
	bool isAlive;
	//Bullet type 1 = bullet 2 = laser
	int bulletType;
	//Last bullet shot
	iTankProjectile* lastBullet;

	//The projectile manager
	TankProjectileManager* myProjectileManager;


public:
	EnemyTank();
	virtual ~EnemyTank();

	// Called every frame
	virtual void Update(double deltaTime);
	virtual bool MoveTank(glm::vec3 direction, glm::vec3 rotation);
	virtual cMesh* getMeshPointer(void);
	virtual void SetMeshPointer(cMesh* mesh);
	void SetPosition(glm::vec3 position);
	virtual glm::vec3 GetPosition();
	virtual void SetProjectileManager(TankProjectileManager* manager);
	void SetBulletType(int type);
	virtual void ShootGun(int type);

	//iMessage stuff
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool SetReciever(iMessage* pTheReciever);

	// Syncronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply);
};