#include "EnemyTank.h"

EnemyTank::EnemyTank()
{
	//Move speed
	moveSpeed = 1.0f;
	//Time since last move
	timeSinceMove = 1.0f;
	//Set inital state
	state = TankState::Blocked;
	//Set it to false
	canMove = false;
	//Set your move
	bulletIsAlive = false;
	//Set your health
	health = 100;
	//set max health
	maxHealth = health;
	//is alive
	isAlive = true;
	SetBulletType(nPhysics::getRandom(1, 3));
	lastDirection = glm::vec3(0, 1, 0);
}

EnemyTank::~EnemyTank()
{
}

void EnemyTank::Update(double deltaTime)
{
	//if (startMovingTanks && isAlive)
	//{
		timeSinceMove += deltaTime;

		if (lastBullet != NULL)
		{
			bulletIsAlive = lastBullet->GetIsAlive();
		}
		else
		{
			bulletIsAlive == false;
		}

		//Check for tank targets
		if (timeSinceMove >= moveSpeed && !bulletIsAlive)
		{
			sMessage checkForPeople;
			checkForPeople.command = "tank check";
			checkForPeople.vec_fData.push_back(glm::vec4(-lastDirection.x, lastDirection.y, 1, 0));
			checkForPeople.vec_fData.push_back(glm::vec4(myPosition.x, myPosition.y, 0, 0));
			if (mediator->RecieveMessage(checkForPeople))
			{
				//TODO: change this to its gun type
				ShootGun(bulletType);
				return;
			}
		}
		if (bulletType == 2 && bulletIsAlive)
		{
			return;
		}

		switch (state)
		{
			//were waiting for take to move again
		case Waiting:
			if (timeSinceMove >= moveSpeed)
			{
				state = TankState::Moving;
			}
			break;

			//Just keep on moving
		case Moving:

			switch (curDirection)
			{
			case Up:
				canMove = MoveTank(glm::vec3(0, 1, 0), glm::vec3(0, 0, 3.14159));
				lastDirection = glm::vec3(0, 1, 0);
				break;
			case Down:
				canMove = MoveTank(glm::vec3(0, -1, 0), glm::vec3(0, 0, 0));
				lastDirection = glm::vec3(0, -1, 0);
				break;
			case Left:
				canMove = MoveTank(glm::vec3(1, 0, 0), glm::vec3(0, 0, 1.5708));
				lastDirection = glm::vec3(1, 0, 0);
				break;
			case Right:
				canMove = MoveTank(glm::vec3(-1, 0, 0), glm::vec3(0, 0, -1.5708));
				lastDirection = glm::vec3(-1, 0, 0);
				break;
			}
			//Check if we were blocked
			if (!canMove)
			{
				state = TankState::Blocked;
			}
			//If we arent blocking then wait for the next one
			else
			{
				state = TankState::Waiting;
			}
			break;

		case Blocked:
			float randDirection = nPhysics::getRandom(0, 100);
			if (randDirection <= 25)
			{
				curDirection = Directions::Up;
			}
			else if (randDirection <= 50)
			{
				curDirection = Directions::Down;
			}
			else if (randDirection <= 75)
			{
				curDirection = Directions::Left;
			}
			else if (randDirection <= 100)
			{
				curDirection = Directions::Right;
			}
			state = TankState::Moving;
			break;
		}
	//}
}

bool EnemyTank::MoveTank(glm::vec3 direction, glm::vec3 rotation)
{
	sMessage send;
	send.command = "wall check";
	send.vec_fData.push_back(glm::vec4(-direction.x, direction.y, 1, 0));
	send.vec_fData.push_back(glm::vec4(myPosition.x, myPosition.y, 0, 0));
	if (mediator->RecieveMessage(send) == false)
	{
		return false;
	}

	myMesh->rotationXYZ = rotation;
	myMesh->transformXYZ += direction;
	//move one down in the array
	myPosition += glm::vec3(-direction.x, direction.y, 0);
	timeSinceMove = 0;
	return true;
}

cMesh* EnemyTank::getMeshPointer(void)
{
	return myMesh;
}

void EnemyTank::SetMeshPointer(cMesh* mesh)
{
	myMesh = mesh;
}

void EnemyTank::SetPosition(glm::vec3 position)
{
	myPosition = position;
}

glm::vec3 EnemyTank::GetPosition()
{
	return myPosition;
}

void EnemyTank::SetProjectileManager(TankProjectileManager* manager)
{
	myProjectileManager = manager;
}

void EnemyTank::ShootGun(int type)
{
	if (lastBullet == NULL)
	{
		if (type == 1)
		{
			//Create a mesh for the debug
			cMesh* debugMesh = new cMesh;
			debugMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
			debugMesh->transformXYZ = glm::vec3(-myPosition.x, myPosition.y, 0);
			debugMesh->bDontLight = true;

			TankBullet* pog = new TankBullet(debugMesh, moveSpeed / 2, glm::vec3(myPosition.x, myPosition.y, 0), lastDirection);
			pog->SetReciever(mediator);
			myProjectileManager->AddTankProjectile(pog);
			lastBullet = pog;
		}
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

			TankLaser* pog = new TankLaser(debugMesh, 1.0f, glm::vec3(myPosition.x, myPosition.y, -1), lastDirection, .10f);
			pog->SetReciever(mediator);
			myProjectileManager->AddTankProjectile(pog);
			lastBullet = pog;

			sMessage send;
			send.command = "wall check";
			send.vec_fData.push_back(glm::vec4(-lastDirection.x, lastDirection.y, 1, 0));
			send.vec_fData.push_back(glm::vec4(myPosition.x, myPosition.y, 0, 0));

			while (mediator->RecieveMessage(send))
			{
				//Create a mesh for the laser
				cMesh* dummyMesh = new cMesh;
				dummyMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
				dummyMesh->transformXYZ = glm::vec3(-targetPos.x, targetPos.y, 0);
				dummyMesh->bDontLight = true;

				TankLaserDummy* tempDummy = new TankLaserDummy(dummyMesh, 1.0f, glm::vec3(targetPos.x, targetPos.y, -1), lastDirection, .10f);
				myProjectileManager->AddTankProjectile(tempDummy);
				targetPos.x += -lastDirection.x;
				targetPos.y += lastDirection.y;
				send.vec_fData[1] = glm::vec4(targetPos.x, targetPos.y, 1, 0);
			}
			//create an extra one at the end to hit the wall
			cMesh* dummyMesh = new cMesh;
			dummyMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
			dummyMesh->transformXYZ = glm::vec3(-targetPos.x, targetPos.y, 0);
			dummyMesh->bDontLight = true;

			TankLaserDummy* tempDummy = new TankLaserDummy(dummyMesh, 1.0f, glm::vec3(targetPos.x, targetPos.y, -1), lastDirection, .10f);
			myProjectileManager->AddTankProjectile(tempDummy);
			targetPos.x += -lastDirection.x;
			targetPos.y += lastDirection.y;
			send.vec_fData[1] = glm::vec4(targetPos.x, targetPos.y, 1, 0);
		}
	}
	else
	{
		if (lastBullet->GetIsAlive())
		{
			return;
		}
		else
		{
			if (type == 1)
			{
				//delete the last bullet and make a new one
				delete lastBullet;
				//Create a mesh for the debug
				cMesh* debugMesh = new cMesh;
				debugMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
				debugMesh->transformXYZ = glm::vec3(-myPosition.x, myPosition.y, 0);
				debugMesh->bDontLight = true;

				TankBullet* pog = new TankBullet(debugMesh, moveSpeed / 2, glm::vec3(myPosition.x, myPosition.y, 0), lastDirection);
				pog->SetReciever(mediator);
				myProjectileManager->AddTankProjectile(pog);
				lastBullet = pog;
			}
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

				TankLaser* pog = new TankLaser(debugMesh, 1.0f, glm::vec3(myPosition.x, myPosition.y, -1), lastDirection, .10f);
				pog->SetReciever(mediator);
				myProjectileManager->AddTankProjectile(pog);
				lastBullet = pog;

				sMessage send;
				send.command = "wall check";
				send.vec_fData.push_back(glm::vec4(-lastDirection.x, lastDirection.y, 1, 0));
				send.vec_fData.push_back(glm::vec4(myPosition.x, myPosition.y, 0, 0));

				while (mediator->RecieveMessage(send))
				{
					//Create a mesh for the laser
					cMesh* dummyMesh = new cMesh;
					dummyMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
					dummyMesh->transformXYZ = glm::vec3(-targetPos.x, targetPos.y, 0);
					dummyMesh->bDontLight = true;

					TankLaserDummy* tempDummy = new TankLaserDummy(dummyMesh, 1.0f, glm::vec3(targetPos.x, targetPos.y, -1), lastDirection, .10f);
					myProjectileManager->AddTankProjectile(tempDummy);
					targetPos.x += -lastDirection.x;
					targetPos.y += lastDirection.y;
					send.vec_fData[1] = glm::vec4(targetPos.x, targetPos.y, 1, 0);
				}
				//create an extra one at the end to hit the wall
				cMesh* dummyMesh = new cMesh;
				dummyMesh->meshName = MODEL_DIR + std::string("WhiteBall.ply");
				dummyMesh->transformXYZ = glm::vec3(-targetPos.x, targetPos.y, 0);
				dummyMesh->bDontLight = true;

				TankLaserDummy* tempDummy = new TankLaserDummy(dummyMesh, 1.0f, glm::vec3(targetPos.x, targetPos.y, -1), lastDirection, .10f);
				myProjectileManager->AddTankProjectile(tempDummy);
				targetPos.x += -lastDirection.x;
				targetPos.y += lastDirection.y;
				send.vec_fData[1] = glm::vec4(targetPos.x, targetPos.y, 1, 0);
			}
		}
	}
}

void EnemyTank::SetBulletType(int type)
{
	bulletType = type;
}


bool EnemyTank::RecieveMessage(sMessage theMessage)
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

bool EnemyTank::SetReciever(iMessage* pTheReciever)
{
	this->mediator = pTheReciever;
	return true;
}

bool EnemyTank::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return false;
}
