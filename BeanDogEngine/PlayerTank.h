#pragma once
#include "iTank.h"

class PlayerTank : public iTank
{
private:
	iMessage* mediator;
protected:
	//The tank mesh
	cMesh* myMesh;
	//The position in the world acording to mapManager array
	glm::vec3 myPosition;
	//The last know direction
	glm::vec3 myDirection;
	//Move speed
	float moveSpeed;
	//Time since last move
	float timeSinceMove;
	//the health
	float health;
	//max health/starting health
	float maxHealth;
	//If its alive
	bool isAlive;
	//The projectile manager
	TankProjectileManager* myProjectileManager;
	//Last bullet shot
	iTankProjectile* lastBullet;
	int lastBulletType;

public:
	PlayerTank();
	virtual ~PlayerTank();

	// Called every frame
	virtual void Update(double deltaTime);
	virtual bool MoveTank(glm::vec3 direction, glm::vec3 rotation);
	virtual cMesh* getMeshPointer(void);
	virtual void SetMeshPointer(cMesh* mesh);
	void SetPosition(glm::vec3 position);
	virtual glm::vec3 GetPosition();
	virtual void SetProjectileManager(TankProjectileManager* manager);

	//Gun stuff
	virtual void ShootGun(int type);

	//iMessage stuff
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool SetReciever(iMessage* pTheReciever);

	// Syncronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply);
};