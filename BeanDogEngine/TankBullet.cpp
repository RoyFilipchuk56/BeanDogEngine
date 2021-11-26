#include "iTankProjectile.h"

TankBullet::TankBullet(cMesh* myMesh, float speed, glm::vec3 position, glm::vec3 direction)
{
	this->myMesh = myMesh;
	this->speed = speed;
	this->position = position;
	this->direction = direction;
	isAlive = true;
	timeSinceMove = speed;
	//Damage = 10%
	damage = 0.1f;
}
TankBullet::~TankBullet()
{
	delete myMesh;
}
cMesh* TankBullet::getMesh()
{
	return myMesh;
}
void TankBullet::Update(float deltaTime)
{
	isAlive = Move(deltaTime);
	if (!isAlive)
	{
		isAlive = false;
	}
}

bool TankBullet::Move(float deltaTime)
{
	timeSinceMove += deltaTime;
	if (timeSinceMove >= speed)
	{
		sMessage send;
		send.command = "shoot check";
		send.vec_fData.push_back(glm::vec4(-direction.x, direction.y, 1, 0));
		send.vec_fData.push_back(glm::vec4(position.x, position.y, damage, 0));
		if (mediator->RecieveMessage(send) == false)
		{
			return false;
		}

		//std::cout << "Reply Message" << std::endl;
		myMesh->transformXYZ += direction;
		//move one down in the array
		position += glm::vec3(-direction.x, direction.y, 0);
		timeSinceMove = 0;
		return true;
	}
	return true;
}
bool TankBullet::GetIsAlive()
{
	return isAlive;
}
void TankBullet::SetIsAlive(bool isAlive)
{
	this->isAlive = isAlive;
}

bool TankBullet::RecieveMessage(sMessage theMessage)
{
	return false;
}

bool TankBullet::SetReciever(iMessage* pTheReciever)
{
	this->mediator = pTheReciever;
	return true;
}

// Syncronous (do return with something)
bool TankBullet::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return false;
}
