#include "Torch.h"

Torch::Torch(cMesh* mesh, glm::vec3 transform, glm::vec3 rotation) : GameObject(mesh, transform, rotation)
{

}

Torch::Torch(cMesh* mesh, char location) : GameObject(mesh)
{
	this->location = location;
}

Torch::Torch(glm::vec3 transform) : GameObject(transform)
{

}

Torch::~Torch()
{
}

void Torch::Update(float deltaTime)
{
}

void Torch::UpdateCommands(float deltaTime)
{
	if (commands != NULL)
	{
		commands->Update(deltaTime);
	}
}

void Torch::SetFriendlyName(std::string name)
{
	this->friendlyName = friendlyName;
}

std::string Torch::GetFriendlyName()
{
	return friendlyName;
}

cMesh* Torch::GetMesh()
{
	return mesh;
}

void Torch::SetMesh(cMesh* mesh)
{
	this->mesh = mesh;
	this->rotationXYZ = glm::vec3(glm::degrees(mesh->rotationXYZ.x), glm::degrees(mesh->rotationXYZ.y), glm::degrees(mesh->rotationXYZ.z));
	this->position = mesh->rotationXYZ;
}

void Torch::UpdateLight(cLightManager* lightManager, int light)
{
	//add a flicker effect
	float attenVariation = nPhysics::getRandom(0.00001, 0.001);
	//Move the light with an offset
	if (location == 'u')
	{
		lightManager->theLights[light].position = glm::vec4(position.x-100, position.y + 100, position.z, 1.0f);
	}
	else if (location == 'd')
	{
		lightManager->theLights[light].position = glm::vec4(position.x + 100, position.y + 100, position.z, 1.0f);
	}
	else if (location == 'l')
	{
		lightManager->theLights[light].position = glm::vec4(position.x, position.y + 100, position.z + 100, 1.0f);
	}
	else if (location == 'r')
	{
		lightManager->theLights[light].position = glm::vec4(position.x, position.y + 100, position.z - 100, 1.0f);
	}

	lightManager->theLights[light].atten.y = attenVariation;
}
