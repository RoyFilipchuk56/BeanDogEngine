#include "InvaderEnemy.h"

InvaderEnemy::InvaderEnemy(cMesh* mesh, glm::vec3 transform, glm::vec3 rotation) : GameObject(mesh, transform, rotation)
{
	moveAmount = 5.0f;
	curentMesh = 0;
}

InvaderEnemy::InvaderEnemy(cMesh* mesh) : GameObject(mesh)
{
	moveAmount = 5.0f;
	curentMesh = 0;
}

InvaderEnemy::InvaderEnemy(glm::vec3 transform) : GameObject(transform)
{
	moveAmount = 5.0f;
	curentMesh = 0;
}

InvaderEnemy::~InvaderEnemy()
{
}

void InvaderEnemy::Update(float deltaTime)
{
}

void InvaderEnemy::UpdateCommands(float deltaTime)
{
	if (commands != NULL)
	{
		commands->Update(deltaTime);
	}
}

void InvaderEnemy::SetFriendlyName(std::string name)
{
	this->friendlyName = friendlyName;
}

std::string InvaderEnemy::GetFriendlyName()
{
	return friendlyName;
}

cMesh* InvaderEnemy::GetMesh()
{
	return mesh;
}

void InvaderEnemy::SetMesh(cMesh* mesh)
{
	this->mesh = mesh;
	this->rotationXYZ = glm::vec3(glm::degrees(mesh->rotationXYZ.x), glm::degrees(mesh->rotationXYZ.y), glm::degrees(mesh->rotationXYZ.z));
	this->position = mesh->rotationXYZ;
}

void InvaderEnemy::SwapModels()
{
	if (meshNames.size() <= 0)
	{
		return;
	}
	curentMesh++;
	if (curentMesh > meshNames.size() - 1)
	{
		curentMesh = 0;
	}

	mesh->meshName = MODEL_DIR + meshNames[curentMesh];
}
