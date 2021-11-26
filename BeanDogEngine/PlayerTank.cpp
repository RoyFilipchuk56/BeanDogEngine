#include "PlayerTank.h"
#include <iostream>

PlayerTank::PlayerTank()
{
	//Move speed
	moveSpeed = 0.25f;
	//Time since last move
	timeSinceMove = 1.0f;
	lastBullet = NULL;
	//Set your health
	health = 100;
	//set max health
	maxHealth = health;
	//is alive
	isAlive = true;
}

PlayerTank::~PlayerTank() 
{

}

// Called every frame
void PlayerTank::Update(double deltaTime)
{
	timeSinceMove += deltaTime;
}

bool PlayerTank::MoveTank(glm::vec3 direction, glm::vec3 rotation)
{
	if (isAlive)
	{
		//If last bullet is null than there want one
		if (lastBullet != NULL)
		{
			//If the last one was a laser that dont move
			if (lastBulletType == 2 && lastBullet->GetIsAlive())
			{
				return false;
			}
		}
		myMesh->rotationXYZ = rotation;
		myDirection = direction;
		if (timeSinceMove >= moveSpeed)
		{
			sMessage send;
			send.command = "wall check";
			send.vec_fData.push_back(glm::vec4(-direction.x, direction.y, 1, 0));
			send.vec_fData.push_back(glm::vec4(myPosition.x, myPosition.y, 0, 0));
			if (mediator->RecieveMessage(send) == false)
			{
				std::cout << "Wall's been hit" << std::endl;
				return false;
			}

			//std::cout << "Reply Message" << std::endl;
			myMesh->transformXYZ += direction;
			//move one down in the array
			myPosition += glm::vec3(-direction.x, direction.y, 0);
			timeSinceMove = 0;
			std::cout << myPosition.x << " " << myPosition.y << std::endl;
			return true;
		}
	}
	return false;
}

cMesh* PlayerTank::getMeshPointer(void)
{
	return myMesh;
}

void PlayerTank::SetMeshPointer(cMesh* mesh)
{
	myMesh = mesh;
}

void PlayerTank::SetPosition(glm::vec3 position)
{
	myPosition = position;
}
glm::vec3 PlayerTank::GetPosition()
{
	return myPosition;
}

void PlayerTank::ShootGun(int type)
{
	if (lastBullet == NULL)
	{
		//bullet
		if (type == 1)
		{
			//Create a mesh for the debug
			cMesh* debugMesh = new cMesh;
			debugMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
			debugMesh->transformXYZ = glm::vec3(-myPosition.x, myPosition.y, 0);
			debugMesh->bDontLight = true;

			TankBullet* pog = new TankBullet(debugMesh, moveSpeed / 2, glm::vec3(myPosition.x, myPosition.y, 0), myDirection);
			pog->SetReciever(mediator);
			myProjectileManager->AddTankProjectile(pog);
			lastBullet = pog;
		}
		//laser
		else
		{
			glm::vec3 targetPos = myPosition;

			//delete the last bullet and make a new one
			delete lastBullet;

			//Create a mesh for the laster indicater
			cMesh* debugMesh = new cMesh;
			debugMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
			debugMesh->transformXYZ = glm::vec3(-myPosition.x, myPosition.y, 0);
			debugMesh->bDontLight = true;

			TankLaser* pog = new TankLaser(debugMesh, 1.0f, glm::vec3(myPosition.x, myPosition.y, -1), myDirection, .10f);
			pog->SetReciever(mediator);
			myProjectileManager->AddTankProjectile(pog);
			lastBullet = pog;

			sMessage send;
			send.command = "wall check";
			send.vec_fData.push_back(glm::vec4(-myDirection.x, myDirection.y, 1, 0));
			send.vec_fData.push_back(glm::vec4(myPosition.x, myPosition.y, 0, 0));

			while (mediator->RecieveMessage(send))
			{
				//Create a mesh for the dummy laser
				cMesh* dummyMesh = new cMesh;
				dummyMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
				dummyMesh->transformXYZ = glm::vec3(-targetPos.x, targetPos.y, 0);
				dummyMesh->bDontLight = true;

				TankLaserDummy* tempDummy = new TankLaserDummy(dummyMesh, 1.0f, glm::vec3(targetPos.x, targetPos.y, -1), myDirection, .10f);
				tempDummy->SetReciever(mediator);
				myProjectileManager->AddTankProjectile(tempDummy);
				targetPos.x += -myDirection.x;
				targetPos.y += myDirection.y;
				send.vec_fData[1] = glm::vec4(targetPos.x, targetPos.y, 1, 0);
			}
			//create an extra one at the end to hit the wall
			cMesh* dummyMesh = new cMesh;
			dummyMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
			dummyMesh->transformXYZ = glm::vec3(-targetPos.x, targetPos.y, 0);
			dummyMesh->bDontLight = true;

			TankLaserDummy* tempDummy = new TankLaserDummy(dummyMesh, 1.0f, glm::vec3(targetPos.x, targetPos.y, -1), myDirection, .10f);
			tempDummy->SetReciever(mediator);
			myProjectileManager->AddTankProjectile(tempDummy);
			targetPos.x += -myDirection.x;
			targetPos.y += myDirection.y;
			send.vec_fData[1] = glm::vec4(targetPos.x, targetPos.y, 1, 0);

		}
		lastBulletType = type;
	}
	else
	{
		if (lastBullet->GetIsAlive())
		{
			return;
		}
		else
		{
			//bullet
			if (type == 1)
			{
				//delete the last bullet and make a new one
				delete lastBullet;
				//Create a mesh for the debug
				cMesh* debugMesh = new cMesh;
				debugMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
				debugMesh->transformXYZ = glm::vec3(-myPosition.x, myPosition.y, 0);
				debugMesh->bDontLight = true;

				TankBullet* pog = new TankBullet(debugMesh, moveSpeed / 2, glm::vec3(myPosition.x, myPosition.y, 0), myDirection);
				pog->SetReciever(mediator);
				myProjectileManager->AddTankProjectile(pog);
				lastBullet = pog;
			}
			//laser
			else
			{
				glm::vec3 targetPos = myPosition;

				//delete the last bullet and make a new one
				delete lastBullet;
				//Create a mesh for the laser
				cMesh* debugMesh = new cMesh;
				debugMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
				debugMesh->transformXYZ = glm::vec3(-myPosition.x, myPosition.y, 0);
				debugMesh->bDontLight = true;

				TankLaser* pog = new TankLaser(debugMesh, 1.0f, glm::vec3(myPosition.x, myPosition.y, -1), myDirection, .10f);
				pog->SetReciever(mediator);
				myProjectileManager->AddTankProjectile(pog);
				lastBullet = pog;

				sMessage send;
				send.command = "wall check";
				send.vec_fData.push_back(glm::vec4(-myDirection.x, myDirection.y, 1, 0));
				send.vec_fData.push_back(glm::vec4(myPosition.x, myPosition.y, 0, 0));
				
				while (mediator->RecieveMessage(send))
				{
					//Create a mesh for the laser
					cMesh* dummyMesh = new cMesh;
					dummyMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
					dummyMesh->transformXYZ = glm::vec3(-targetPos.x, targetPos.y, 0);
					dummyMesh->bDontLight = true;

					TankLaserDummy* tempDummy = new TankLaserDummy(dummyMesh, 1.0f, glm::vec3(targetPos.x, targetPos.y, -1), myDirection, .10f);
					myProjectileManager->AddTankProjectile(tempDummy);
					targetPos.x += -myDirection.x;
					targetPos.y += myDirection.y;
					send.vec_fData[1] = glm::vec4(targetPos.x, targetPos.y, 1, 0);
				}
				//create an extra one at the end to hit the wall
				cMesh* dummyMesh = new cMesh;
				dummyMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
				dummyMesh->transformXYZ = glm::vec3(-targetPos.x, targetPos.y, 0);
				dummyMesh->bDontLight = true;

				TankLaserDummy* tempDummy = new TankLaserDummy(dummyMesh, 1.0f, glm::vec3(targetPos.x, targetPos.y, -1), myDirection, .10f);
				myProjectileManager->AddTankProjectile(tempDummy);
				targetPos.x += -myDirection.x;
				targetPos.y += myDirection.y;
				send.vec_fData[1] = glm::vec4(targetPos.x, targetPos.y, 1, 0);
			}
		}
		lastBulletType = type;
	}
}

void PlayerTank::SetProjectileManager(TankProjectileManager* manager)
{
	myProjectileManager = manager;
}

//iMessage stuff
bool PlayerTank::RecieveMessage(sMessage theMessage)
{
	if (theMessage.command == "take damage")
	{
		health -= maxHealth * theMessage.vec_fData[0].x;
		myMesh->objectDebugColourRGBA -= glm::vec4(theMessage.vec_fData[0].x, theMessage.vec_fData[0].x, theMessage.vec_fData[0].x, 0);
		if (health <= 0)
		{
			isAlive = false;
		}
	}
	return true;
}
bool PlayerTank::SetReciever(iMessage* pTheReciever)
{
	this->mediator = pTheReciever;
	return true;
}

// Syncronous (do return with something)
bool PlayerTank::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return false;
}