#pragma once
#include "cMesh.h"
#include <glm/vec3.hpp>
#include "iMessage.h"
#include "TankProjectileManager.h"

class iTank : public iMessage
{
public:
	virtual ~iTank() {};

	// Called every frame
	virtual void Update(double deltaTime) = 0;
	virtual bool MoveTank(glm::vec3 direction, glm::vec3 rotation) = 0;

	virtual cMesh* getMeshPointer(void) = 0;
	virtual void SetMeshPointer(cMesh* mesh) = 0;
	virtual void SetPosition(glm::vec3 position) = 0;
	virtual glm::vec3 GetPosition() = 0;
	virtual void SetProjectileManager(TankProjectileManager* manager) = 0;

	virtual void ShootGun(int type) = 0;
	virtual bool RecieveMessage(sMessage theMessage) = 0;
	virtual bool SetReciever(iMessage* pTheReciever) = 0;
	
	// Syncronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply) = 0;
};