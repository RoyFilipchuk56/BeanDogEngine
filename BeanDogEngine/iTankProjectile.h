#pragma once
#include "cMesh.h"
#include "iMessage.h"

class iTankProjectile : public iMessage
{
public:
	virtual ~iTankProjectile() {};
	virtual void Update(float deltaTime) = 0;

	virtual bool RecieveMessage(sMessage theMessage) = 0;
	virtual bool SetReciever(iMessage* pTheReciever) = 0;
	virtual cMesh* getMesh() = 0;
	virtual bool GetIsAlive() = 0;
	virtual void SetIsAlive(bool alive) = 0;

	// Syncronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply) = 0;
};


class TankBullet : public iTankProjectile
{
private:
	iMessage* mediator;

public:
	cMesh* myMesh;
	float speed;
	glm::vec3 position;
	glm::vec3 direction;
	bool isAlive;
	float timeSinceMove;
	float damage;
	//iTank* linkedTank;

	TankBullet(cMesh* myMesh, float speed, glm::vec3 position, glm::vec3 direction);
	virtual ~TankBullet();


	virtual cMesh* getMesh();

	virtual void Update(float deltaTime);
	bool Move(float deltaTime);
	virtual bool GetIsAlive();
	virtual void SetIsAlive(bool isAlive);

	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool SetReciever(iMessage* pTheReciever);

	// Syncronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply);
};


class TankLaser : public iTankProjectile
{
private:
	iMessage* mediator;

public:
	cMesh* myMesh;
	float chargeTime;
	float currentChargeTime;
	glm::vec3 position;
	glm::vec3 direction;
	float damage;

	bool isAlive;

	TankLaser(cMesh* myMesh, float chargeTime, glm::vec3 position, glm::vec3 direction, float damage);
	virtual ~TankLaser();

	virtual cMesh* getMesh();

	virtual void Update(float deltaTime);
	virtual bool GetIsAlive();
	virtual void SetIsAlive(bool isAlive);

	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool SetReciever(iMessage* pTheReciever);

	// Syncronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply);
};

class TankLaserDummy : public iTankProjectile
{
private:
	iMessage* mediator;

public:
	cMesh* myMesh;
	float chargeTime;
	float currentChargeTime;
	glm::vec3 position;
	glm::vec3 direction;
	float damage;

	bool isAlive;

	TankLaserDummy(cMesh* myMesh, float chargeTime, glm::vec3 position, glm::vec3 direction, float damage);
	virtual ~TankLaserDummy();

	virtual cMesh* getMesh();

	virtual void Update(float deltaTime);
	virtual bool GetIsAlive();
	virtual void SetIsAlive(bool isAlive);

	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool SetReciever(iMessage* pTheReciever);

	// Syncronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply);
};
