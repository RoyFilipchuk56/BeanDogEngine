#include "GameObject.h"

GameObject::GameObject(cMesh* mesh, glm::vec3 transform, glm::vec3 rotation) : 
	mesh(mesh), friendlyName(""), receiver(NULL)
{
	//Set the position to the given position
	SetPosition(position);
	//Set both rotations to the given rotations
	SetRotation(rotation);
	SetRotationQuat(quaternion::QuatFromAngles(rotation));
}

GameObject::GameObject(cMesh* mesh) : 
	mesh(mesh), friendlyName(""), receiver(NULL)
{
	//Make sure the mesh isnt null
	if (mesh != NULL)
	{
		position = mesh->transformXYZ;
		rotationXYZ = glm::vec3(glm::degrees(mesh->rotationXYZ.x), glm::degrees(mesh->rotationXYZ.y), glm::degrees(mesh->rotationXYZ.z));
		rotationXYZQuat = mesh->rotationXYZQuat;
	}
	//If its null then set zero values
	else
	{
		position = glm::vec3(0);
		rotationXYZ = glm::vec3(0);
		rotationXYZQuat = glm::quat(glm::vec3(0.0));
	}
}

GameObject::GameObject(glm::vec3 transform) : 
	mesh(NULL), position(position), rotationXYZ(glm::vec3(0)), friendlyName(""), receiver(NULL)
{
	rotationXYZQuat = glm::quat(glm::vec3(0.0, 0.0, 0.0));
}

GameObject::~GameObject()
{
	//Delete the mesh if it exists
	if (mesh != NULL)
	{
		delete mesh;
	}

	/*
	if (commands != NULL)
		delete commands;
	*/
}

void GameObject::Update(float deltaTime)
{
}

void GameObject::UpdateCommands(float deltaTime)
{
	/*
	if (commands != NULL)
	{
		commands->Update(deltaTime);
	}
	*/
}

void GameObject::SetFriendlyName(std::string friendlyName)
{
	this->friendlyName = friendlyName;
}

std::string GameObject::GetFriendlyName()
{
	return friendlyName;
}

cMesh* GameObject::GetMesh()
{
	if (mesh != NULL)
		return mesh;

	return nullptr;
}

void GameObject::SetMesh(cMesh* mesh)
{
	this->mesh = mesh;
	this->rotationXYZ = glm::vec3(glm::degrees(mesh->rotationXYZ.x), glm::degrees(mesh->rotationXYZ.y), glm::degrees(mesh->rotationXYZ.z));
	this->position = mesh->rotationXYZ;
}

void GameObject::SetPosition(glm::vec3 position)
{
	this->position = position;
	if (mesh != NULL)
		mesh->transformXYZ = position;
}

glm::vec3 GameObject::GetPosition()
{
	return position;
}

void GameObject::SetRotation(glm::vec3 rotationXYZ)
{
	this->rotationXYZ = rotationXYZ;
	this->rotationXYZQuat = quaternion::QuatFromAngles(rotationXYZ);
	if (mesh != NULL)
	{
		mesh->rotationXYZ = glm::vec3(glm::radians(rotationXYZ.x), glm::radians(rotationXYZ.y), glm::radians(rotationXYZ.z));
		mesh->rotationXYZQuat = quaternion::QuatFromAngles(rotationXYZ);
	}
}

void GameObject::SetRotationRadians(glm::vec3 rotationXYZ)
{
	this->rotationXYZ = glm::vec3(glm::degrees(rotationXYZ.x), glm::degrees(rotationXYZ.y), glm::degrees(rotationXYZ.z));
	this->rotationXYZQuat = quaternion::QuatFromAngles(rotationXYZ);
	if (mesh != NULL)
	{
		mesh->rotationXYZ = glm::vec3(rotationXYZ.x, rotationXYZ.y, rotationXYZ.z);
		mesh->rotationXYZQuat = quaternion::QuatFromAngles(rotationXYZ);
	}
}

void GameObject::SetRotationQuat(glm::quat rotationXYZ)
{
	rotationXYZQuat = rotationXYZ;
	if (mesh != NULL)
		mesh->rotationXYZQuat = rotationXYZQuat;
}

glm::quat GameObject::GetRotationQuat()
{
	return rotationXYZQuat;
}

glm::vec3 GameObject::GetRotation()
{
	return rotationXYZ;
}

void GameObject::SetScale(glm::vec3 scale)
{
	if (mesh != NULL)
		mesh->scaleXYZ = scale;
}

void GameObject::SetScale(float scale)
{
	mesh->scale = scale;
	mesh->scaleXYZ = glm::vec3(scale, scale, scale);
}

bool GameObject::SetReciever(iMessage* receiver)
{
	this->receiver = receiver;

	return true;
}

bool GameObject::RecieveMessage(sMessage message)
{
	return false;
}

bool GameObject::RecieveMessage(sMessage message, sMessage& reply)
{
	return false;
}
