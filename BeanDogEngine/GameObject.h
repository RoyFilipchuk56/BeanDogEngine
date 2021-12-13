#pragma once
#include "iEntity.h"
#include "QuaternionHelpers.h"

class GameObject : public iEntity
{
public:
	// Gameobject will use the mesh but a different transform and rotation
	GameObject(cMesh* mesh, glm::vec3 transform, glm::vec3 rotation);
	// GameObject will use the mesh and its givin transform and rotation
	GameObject(cMesh* mesh);
	// GameObject doesnt need mesh
	GameObject(glm::vec3 transform);
	virtual ~GameObject();

	// From iEntity
	virtual void Update(float deltaTime);
	virtual void UpdateCommands(float deltaTime);
	virtual void SetFriendlyName(std::string name);
	virtual std::string GetFriendlyName();
	virtual cMesh* GetMesh();
	virtual void SetMesh(cMesh* mesh);

	//Methods for controlling the mesh
	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition();
	void SetRotation(glm::vec3 rotationXYZ);
	void SetRotationRadians(glm::vec3 rotationXYZ);
	void SetRotationQuat(glm::quat rotationXYZ);
	glm::quat GetRotationQuat();
	glm::vec3 GetRotation();
	void SetScale(glm::vec3 scale);
	void SetScale(float scale);

	//Animation commands
	//TODO: Implament the animation controller
	//CommandController* GetCommandController();

	//iMessage 
	virtual bool SetReciever(iMessage* receiver);
	virtual bool RecieveMessage(sMessage message);
	virtual bool RecieveMessage(sMessage message, sMessage& reply);

protected:
	cMesh* mesh;
	glm::vec3 position;
	glm::vec3 rotationXYZ;
	glm::quat rotationXYZQuat;
	std::string friendlyName;
	iMessage* receiver;
	//CommandController* commands;
};