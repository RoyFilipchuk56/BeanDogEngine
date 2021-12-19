#include "InvaderLazer.h"

InvaderLazer::InvaderLazer(cMesh* mesh, glm::vec3 transform, glm::vec3 rotation) : GameObject(mesh, transform, rotation)
{
	movementSpeed = 500.0f;
	timeSinceLastSwap = 0;
	swapTime = 0.4f;
	curentMesh = 0;
}

InvaderLazer::InvaderLazer(cMesh* mesh, int movementSpeed) : GameObject(mesh)
{
	this->movementSpeed = movementSpeed;
	timeSinceLastSwap = 0;
	swapTime = 0.4f;
	curentMesh = 0;
}

InvaderLazer::InvaderLazer(glm::vec3 transform) : GameObject(transform)
{
	movementSpeed = 500.0f;
	timeSinceLastSwap = 0;
	swapTime = 0.4f;
	curentMesh = 0;
}

InvaderLazer::~InvaderLazer()
{
}

void InvaderLazer::Update(float deltaTime)
{
	timeSinceLastSwap += deltaTime;
	if (timeSinceLastSwap > swapTime)
	{
		timeSinceLastSwap = 0;
		SwapMeshes();
	}
	SetPosition(GetPosition() + glm::vec3(0, movementSpeed * deltaTime, 0));
}

void InvaderLazer::UpdateCommands(float deltaTime)
{
	if (commands != NULL)
	{
		commands->Update(deltaTime);
	}
}

void InvaderLazer::SetFriendlyName(std::string name)
{
	this->friendlyName = friendlyName;
}

std::string InvaderLazer::GetFriendlyName()
{
	return friendlyName;
}

cMesh* InvaderLazer::GetMesh()
{
	return mesh;
}

void InvaderLazer::SetMesh(cMesh* mesh)
{
	this->mesh = mesh;
	this->rotationXYZ = glm::vec3(glm::degrees(mesh->rotationXYZ.x), glm::degrees(mesh->rotationXYZ.y), glm::degrees(mesh->rotationXYZ.z));
	this->position = mesh->rotationXYZ;
}

void InvaderLazer::SwapMeshes()
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
