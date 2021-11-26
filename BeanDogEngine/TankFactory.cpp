#include "TankFactory.h"

// Create ship message
//
//Command			| vec_fdata[0]			|
//type of tank		| starting position		|
//					| starting position		|
TankFactory::TankFactory()
{

}

TankFactory::~TankFactory()
{

}

void TankFactory::SetMediator(iMediator* mediator)
{
	this->theMediator = mediator;
}

iTank* TankFactory::CreateTank(sMessage typeOfTank)
{
	if (typeOfTank.command == "player")
	{
		//TODO: Check if player is null because only one player
		
		cMesh* playerTankMesh = new cMesh;
		playerTankMesh->meshName = MODEL_DIR + std::string("midterm/Low Poly Tank Model 3D model.ply");
		playerTankMesh->transformXYZ = glm::vec3(typeOfTank.vec_fData[0].x, typeOfTank.vec_fData[0].y, 0);
		playerTankMesh->rotationXYZ = glm::vec3(0, 3.14159f, 0);
		playerTankMesh->bUseObjectDebugColour = true;
		playerTankMesh->objectDebugColourRGBA = typeOfTank.vec_fData[1];
		playerTankMesh->scale = 0.25f;
		g_vecMeshes.push_back(playerTankMesh);
		//create the player tank
		PlayerTank* tempPlayerTank = new PlayerTank();
		tempPlayerTank->SetPosition(glm::vec3(-playerTankMesh->transformXYZ.x, playerTankMesh->transformXYZ.y - 0.25f, 0));
		tempPlayerTank->SetMeshPointer(playerTankMesh);
		tempPlayerTank->SetReciever(theMediator);
		//tempPlayerTank->SetProjectileManager(projectileManager);
		
		return tempPlayerTank;
	}
	else if (typeOfTank.command == "enemy")
	{
		//TODO: Check if player is null because only one player
		cMesh* tankMesh = new cMesh;
		tankMesh->meshName = MODEL_DIR + std::string("midterm/Low Poly Tank Model 3D model.ply");
		tankMesh->transformXYZ = glm::vec3(typeOfTank.vec_fData[0].x, typeOfTank.vec_fData[0].y, 0);
		tankMesh->rotationXYZ = glm::vec3(0, 3.14159f, 0);
		tankMesh->bUseObjectDebugColour = true;
		tankMesh->objectDebugColourRGBA = typeOfTank.vec_fData[1];
		tankMesh->scale = 0.25f;
		g_vecMeshes.push_back(tankMesh);
		//create the player tank
		EnemyTank* tank = new EnemyTank();
		tank->SetPosition(glm::vec3(-tankMesh->transformXYZ.x, tankMesh->transformXYZ.y - 0.25f, 0));
		tank->SetMeshPointer(tankMesh);
		tank->SetReciever(theMediator);
		//tank->SetProjectileManager(projectileManager);
		return tank;
	}
	else if (typeOfTank.command == "super enemy")
	{

	}
}

void TankFactory::SetUpTanks()
{
	sMessage playerMessage;
	playerMessage.command = "player";
	playerMessage.vec_fData.push_back(glm::vec4(-18, 46.25, 0, 0));
	playerMessage.vec_fData.push_back(glm::vec4(1, 1, 1, 1));
	//allTanks.push_back(CreateTank(playerMessage));
	sMessage enemyMessage;
	enemyMessage.command = "enemy";
	enemyMessage.vec_fData.push_back(glm::vec4(-50, 46.25, 0, 0));
	enemyMessage.vec_fData.push_back(glm::vec4(1, 0, 0, 1));
	//allTanks.push_back(CreateTank(enemyMessage));
	enemyMessage.vec_fData[0] = glm::vec4(-84, 46.25, 0, 0);
	enemyMessage.vec_fData[1] = glm::vec4(0, 1, 0, 1);
	//allTanks.push_back(CreateTank(enemyMessage));
	enemyMessage.vec_fData[0] = glm::vec4(-18, 15.25, 0, 0);
	enemyMessage.vec_fData[1] = glm::vec4(0, 0, 1, 1);
	//allTanks.push_back(CreateTank(enemyMessage));
	enemyMessage.vec_fData[0] = glm::vec4(-50, 15.25, 0, 0);
	enemyMessage.vec_fData[1] = glm::vec4(0, 1, 1, 1);
	//allTanks.push_back(CreateTank(enemyMessage));
	enemyMessage.vec_fData[0] = glm::vec4(-84, 15.25, 0, 0);
	enemyMessage.vec_fData[1] = glm::vec4(1, 1, 0, 1);
	//allTanks.push_back(CreateTank(enemyMessage));
}