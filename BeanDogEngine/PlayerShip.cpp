#include "PlayerShip.h"

PlayerShip::PlayerShip(cMesh* mesh, glm::vec3 transform, glm::vec3 rotation) : GameObject(mesh, transform, rotation)
{
	moveSpeed = 500.0f;
	isAlive = true;
}

PlayerShip::PlayerShip(cMesh* mesh) : GameObject(mesh)
{
	moveSpeed = 500.0f;
	isAlive = true;
}

PlayerShip::PlayerShip(glm::vec3 transform) : GameObject(transform)
{
	moveSpeed = 500.0f;
	isAlive = true;
}

PlayerShip::~PlayerShip()
{
}

void PlayerShip::Update(float deltaTime)
{
}

void PlayerShip::UpdateCommands(float deltaTime)
{
	if (commands != NULL)
	{
		commands->Update(deltaTime);
	}
}

void PlayerShip::SetFriendlyName(std::string name)
{
	this->friendlyName = friendlyName;
}

std::string PlayerShip::GetFriendlyName()
{
	return friendlyName;
}

cMesh* PlayerShip::GetMesh()
{
	return mesh;
}

void PlayerShip::SetMesh(cMesh* mesh)
{
	this->mesh = mesh;
	this->rotationXYZ = glm::vec3(glm::degrees(mesh->rotationXYZ.x), glm::degrees(mesh->rotationXYZ.y), glm::degrees(mesh->rotationXYZ.z));
	this->position = mesh->rotationXYZ;
}
