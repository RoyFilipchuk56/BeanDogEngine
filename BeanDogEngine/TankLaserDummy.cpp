#include "iTankProjectile.h"

TankLaserDummy::TankLaserDummy(cMesh* myMesh, float chargeTime, glm::vec3 position, glm::vec3 direction, float damage)
{
	this->myMesh = myMesh;
	this->chargeTime = chargeTime;
	this->position = position;
	this->direction = direction;
	this->damage = damage;
	currentChargeTime = 0;
	isAlive = true;
}

TankLaserDummy::~TankLaserDummy()
{

}

cMesh* TankLaserDummy::getMesh()
{
	return myMesh;
}

void TankLaserDummy::Update(float deltaTime)
{
	currentChargeTime += deltaTime;
	if (currentChargeTime >= chargeTime)
	{
		isAlive = false;
	}
}

bool TankLaserDummy::GetIsAlive()
{
	return isAlive;
}

void TankLaserDummy::SetIsAlive(bool isAlive)
{
	isAlive = isAlive;
}

bool TankLaserDummy::RecieveMessage(sMessage theMessage)
{
	return false;
}

bool TankLaserDummy::SetReciever(iMessage* pTheReciever)
{
	return false;
}

// Syncronous (do return with something)
bool TankLaserDummy::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return false;
}