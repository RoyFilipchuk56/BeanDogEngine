#include "iTankProjectile.h"

TankLaser::TankLaser(cMesh* myMesh, float chargeTime, glm::vec3 position, glm::vec3 direction, float damage)
{
	this->myMesh = myMesh;
	this->chargeTime = chargeTime;
	this->position = position;
	this->direction = direction;
	this->damage = damage;
	currentChargeTime = 0;
	isAlive = true;
}

TankLaser::~TankLaser()
{

}

cMesh* TankLaser::getMesh()
{
	return myMesh;
}

void TankLaser::Update(float deltaTime)
{
	currentChargeTime += deltaTime;
	if (currentChargeTime >= chargeTime)
	{
		sMessage send;
		send.command = "laser check";
		send.vec_fData.push_back(glm::vec4(-direction.x, direction.y, 1, 0));
		send.vec_fData.push_back(glm::vec4(position.x, position.y, damage, 0));
		mediator->RecieveMessage(send) == false;
		isAlive = false;
	}
}

bool TankLaser::GetIsAlive()
{
	return isAlive;
}

void TankLaser::SetIsAlive(bool isAlive)
{
	isAlive = isAlive;
}

bool TankLaser::RecieveMessage(sMessage theMessage)
{
	return false;
}

bool TankLaser::SetReciever(iMessage* pTheReciever)
{
	mediator = pTheReciever;
	return true;
}

// Syncronous (do return with something)
bool TankLaser::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return false;
}