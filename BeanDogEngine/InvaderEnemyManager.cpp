#include "InvaderEnemyManager.h"
#include <iostream>

InvaderEnemyManager::InvaderEnemyManager()
{
	/*
	for (int i = 0; i < MAX_COLUMN_COUNT; i++)
	{
		std::vector<InvaderEnemy*> enemyColumn;
		nGameObject::enemys.push_back(enemyColumn);
		for (int j = 0; j < MAX_ROW_COUNT; j++)
		{
			if (i <= 0)
			{
				//Create an enemy
				cMesh* enemyMesh = new cMesh;
				enemyMesh->meshName = MODEL_DIR + std::string("SpaceInvader_I2_block_pose_1.ply");
				enemyMesh->transformXYZ = glm::vec3(ENEMY_SPAWN_X_START - (ENEMY_X_OFFSET * j), ENEMY_SPAWN_Y_START - (ENEMY_Y_OFFSET * i), 0);
				enemyMesh->textureNames[0] = "Grey.bmp";
				enemyMesh->textureRatios[0] = 1.0f;
				g_vecMeshes.push_back(enemyMesh);
				InvaderEnemy* enemy = new InvaderEnemy(enemyMesh);
				enemy->meshNames.push_back("SpaceInvader_I2_block_pose_1.ply");
				enemy->meshNames.push_back("SpaceInvader_I2_block_pose_2.ply");
				enemy->SetFriendlyName("enemy");

				nGameObject::enemys[i].push_back(enemy);
			}
			else if (i <= 2)
			{
				//Create an enemy
				cMesh* enemyMesh = new cMesh;
				enemyMesh->meshName = MODEL_DIR + std::string("SpaceInvader_I1_block_pose_1.ply");
				enemyMesh->transformXYZ = glm::vec3(ENEMY_SPAWN_X_START - (ENEMY_X_OFFSET * j), ENEMY_SPAWN_Y_START - (ENEMY_Y_OFFSET * i), 0);
				enemyMesh->textureNames[0] = "Grey.bmp";
				enemyMesh->textureRatios[0] = 1.0f;
				g_vecMeshes.push_back(enemyMesh);
				InvaderEnemy* enemy = new InvaderEnemy(enemyMesh);
				enemy->meshNames.push_back("SpaceInvader_I1_block_pose_1.ply");
				enemy->meshNames.push_back("SpaceInvader_I1_block_pose_2.ply");
				enemy->SetFriendlyName("enemy");

				nGameObject::enemys[i].push_back(enemy);
			}
			else
			{
				//Create an enemy
				cMesh* enemyMesh = new cMesh;
				enemyMesh->meshName = MODEL_DIR + std::string("SpaceInvader_I3_block_pose_1.ply");
				enemyMesh->transformXYZ = glm::vec3(ENEMY_SPAWN_X_START - (ENEMY_X_OFFSET * j), ENEMY_SPAWN_Y_START - (ENEMY_Y_OFFSET * i), 0);
				enemyMesh->textureNames[0] = "Grey.bmp";
				enemyMesh->textureRatios[0] = 1.0f;
				g_vecMeshes.push_back(enemyMesh);
				InvaderEnemy* enemy = new InvaderEnemy(enemyMesh);
				enemy->meshNames.push_back("SpaceInvader_I3_block_pose_1.ply");
				enemy->meshNames.push_back("SpaceInvader_I3_block_pose_2.ply");
				enemy->SetFriendlyName("enemy");

				nGameObject::enemys[i].push_back(enemy);
			}
		}
	}
	isMovingLeft = false;
	areEnemysMoving = false;
	timeUntilNextMove = 1.0f;
	timeSinceLastMove = 0.0f;
	timeSinceLastColumbMove = 0.0f;
	columnToMove = MAX_COLUMN_COUNT - 1;
	*/
}

InvaderEnemyManager::~InvaderEnemyManager()
{
}

void InvaderEnemyManager::update(float deltaTime)
{
	/*
	MoveEnemies(deltaTime);

	//Collision checking
	//Collision checking for player lazer
	if (nGameObject::playerLazer != nullptr)
	{
		bool didLazerHit = false;
		//Go through the enemys and check for player bullet collision
		for (int i = 0; i < nGameObject::enemys.size(); i++)
		{
			for (int j = 0; j < nGameObject::enemys[i].size(); j++)
			{
				//Replace with enemys radius
				float radius = 75.0f;
				glm::vec3 midline = nGameObject::playerLazer->GetPosition() - nGameObject::enemys[i][j]->GetPosition();
				float mag = glm::length(midline);
				//we we aint colliding
				if (mag <= 0.0f || mag >= radius)
				{
					continue;
				}
				didLazerHit = true;
				nGameObject::DeletePlayerLazer();
				nGameObject::DeleteEnemy(i, j);
				timeUntilNextMove *= 0.99;
				break;
				//TODO: Increase enemy speed

			}
			if (didLazerHit)
			{
				break;
			}
		}
	}

	//Make enemys shoot the lazers
	for (int i = 0; i < nGameObject::enemys.size(); i++)
	{
		for (int j = 0; j < nGameObject::enemys[i].size(); j++)
		{
			float random = nPhysics::getRandom(100);
			if (random < 0.01)
			{
				cMesh* lazerMesh = new cMesh;
				lazerMesh->meshName = MODEL_DIR + std::string("SpaceInvader_Bomb_pose_1.ply");
				lazerMesh->transformXYZ = nGameObject::enemys[i][j]->GetPosition();
				lazerMesh->textureNames[0] = "Grey.bmp";
				lazerMesh->textureRatios[0] = 1.0f;
				g_vecMeshes.push_back(lazerMesh);
				InvaderLazer* lazer = new InvaderLazer(lazerMesh, -500.0f);
				lazer->meshNames.push_back("SpaceInvader_Bomb_pose_1.ply");
				lazer->meshNames.push_back("SpaceInvader_Bomb_pose_2.ply");
				lazer->meshNames.push_back("SpaceInvader_Bomb_pose_3.ply");
				lazer->SetFriendlyName("PlayerLazer");
				nGameObject::enemyLazers.push_back(lazer);
			}
		}
	}

	//Check enemy lazers 
	for (int i = 0; i < nGameObject::enemyLazers.size(); i++)
	{
		bool isCollided = false;
		//TODO: Replace with enemys radius
		float radius = 85.0f;
		glm::vec3 midline;
		float mag = 0;
		//Do a check for collision with bunkers
		for (int j = 0; j < nGameObject::bunkers.size(); j++)
		{
			midline = nGameObject::enemyLazers[i]->GetPosition() - nGameObject::bunkers[j]->GetPosition();
			mag = glm::length(midline);
			//we we aint colliding
			if (mag <= 0.0f || mag >= radius)
			{
				continue;
			}
			isCollided = true;
		}

		if (isCollided)
		{
			nGameObject::DeleteEnemyLazer(nGameObject::enemyLazers[i]);
			continue;
		}

		//Do the check for players if it didnt hit a bunker
		midline = nGameObject::enemyLazers[i]->GetPosition() - nGameObject::player->GetPosition();
		mag = glm::length(midline);
		//we we aint colliding
		if (mag <= 0.0f || mag >= radius)
		{
			continue;
		}
		nGameObject::DeleteEnemyLazer(nGameObject::enemyLazers[i]);
		nGameObject::KillPlayer();
		break;
	}

	//send messag for mediator to check collisions
	sMessage message;
	message.command = "Player Bullet Collision";
	mediator->RecieveMessage(message);
	message.command = "Enemy Bullet Collision";
	for (int i = 0; i < nGameObject::enemys.size(); i++)
	{
		for (int j = 0; i < nGameObject::enemys[i].size(); j++)
		{
			//Update fdata with the enemys bullet location
		}
	}
	*/
}

void InvaderEnemyManager::MoveEnemies(float deltaTime)
{
	/*
	//if the enemys arent in the moving animation
	if (!areEnemysMoving)
	{
		//Move the enemies
		timeSinceLastMove += deltaTime;
		if (timeUntilNextMove < timeSinceLastMove)
		{
			timeSinceLastMove = 0;
			//Check if were going to have to move down
			for (int i = 0; i < nGameObject::enemys.size(); i++)
			{
				if (nGameObject::enemys[i].size() > 0)
				{
					//Collision check for the end of the map
					if (isMovingLeft)
					{
						//If we hit the edge then swap directions
						glm::vec3 newPos = nGameObject::enemys[i][0]->GetPosition() + glm::vec3(ENEMY_X_OFFSET, 0, 0);
						if (newPos.x > MAP_MAX_X)
						{
							//Check if enemys are at the bottom
							int maxColSize = nGameObject::enemys.size() - 1;
							if (nGameObject::enemys[maxColSize][0]->GetPosition().y - ENEMY_Y_OFFSET <= ENEMY_WIN_Y)
							{
								nGameObject::KillPlayer();
								return;
							}

							isMovingLeft = false;
							columnToMove = nGameObject::enemys.size() - 1;
							areEnemysMoving = false;
							//Move enemys down
							for (int col = 0; col < nGameObject::enemys.size(); col++)
							{
								for (int row = 0; row < nGameObject::enemys[col].size(); row++)
								{
									nGameObject::enemys[col][row]->SetPosition(nGameObject::enemys[col][row]->GetPosition() + glm::vec3(0, -ENEMY_Y_OFFSET, 0));
								}
							}
						}
					}
					else
					{
						//If we hit the edge then swap directions
						glm::vec3 newPos = nGameObject::enemys[i][nGameObject::enemys[i].size() - 1]->GetPosition() + glm::vec3(-ENEMY_X_OFFSET, 0, 0);
						if (newPos.x < MAP_MIN_X)
						{
							isMovingLeft = true;
							columnToMove = nGameObject::enemys.size() - 1;
							areEnemysMoving = false;
							//Move enemys down
							for (int col = 0; col < nGameObject::enemys.size(); col++)
							{
								for (int row = 0; row < nGameObject::enemys[col].size(); row++)
								{
									nGameObject::enemys[col][row]->SetPosition(nGameObject::enemys[col][row]->GetPosition() + glm::vec3(0, -ENEMY_Y_OFFSET, 0));
								}
							}
						}
					}
				}
				else
				{
					nGameObject::enemys.erase(std::remove(nGameObject::enemys.begin(), nGameObject::enemys.end(), nGameObject::enemys[i]));
					columnToMove = nGameObject::enemys.size() - 1;
				}
			}
			areEnemysMoving = true;
		}
	}
	//If they are then update the moving animation
	else
	{
		if (nGameObject::enemys.size() > 0)
		{
			//make sure the columb isnt empty
			if (nGameObject::enemys[columnToMove].size() > 0)
			{
				timeSinceLastColumbMove += deltaTime;
				if (timeSinceLastColumbMove > 0.1)
				{
					for (InvaderEnemy* enemy : nGameObject::enemys[columnToMove])
					{
						if (isMovingLeft)
						{
							enemy->SetPosition(enemy->GetPosition() + glm::vec3(ENEMY_X_OFFSET, 0, 0));
							enemy->SwapModels();
						}
						else
						{
							enemy->SetPosition(enemy->GetPosition() + glm::vec3(-ENEMY_X_OFFSET, 0, 0));
							enemy->SwapModels();
						}
					}
					columnToMove--;
					timeSinceLastColumbMove = 0;
				}
				if (columnToMove < 0)
				{
					areEnemysMoving = false;
					columnToMove = nGameObject::enemys.size() - 1;
				}
			}
			else
			{
				if (!(columnToMove <= 0))
				{
					columnToMove--;
				}
			}
		}
	}
	*/
}

bool InvaderEnemyManager::SetReciever(iMessage* receiver)
{
	mediator = receiver;
	return true;
}

bool InvaderEnemyManager::RecieveMessage(sMessage message)
{
	return false;
}

bool InvaderEnemyManager::RecieveMessage(sMessage message, sMessage& reply)
{
	return false;
}
