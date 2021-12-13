#pragma once
#include "cMesh.h"
#include "iMessage.h"

class iEntity : public iMessage
{
public:
	virtual ~iEntity() {};
	virtual void Update(float deltaTime) = 0;
	virtual void UpdateCommands(float deltaTime) = 0;
	virtual void SetFriendlyName(std::string name) = 0;
	virtual std::string GetFriendlyName() = 0;
	virtual cMesh* GetMesh() = 0;
	virtual void SetMesh(cMesh* mesh) = 0;
};