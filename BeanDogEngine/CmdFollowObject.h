#pragma once
#include "iCommand.h"
#include "GameObject.h"
#include <glm/vec3.hpp>

class CmdFollowObject : public iCommand
{
public:
	CmdFollowObject(glm::vec3 offset, float maxSpeed, float distance, float maxTime, GameObject* followEntity);
	virtual ~CmdFollowObject();

	virtual void Initialize(sMessage startData);
	virtual void Start(sMessage startData);
	virtual void End(sMessage endData);
	virtual void Update(float deltaTime);
	virtual bool IsDone(void);

	float SmoothStep(float edge0, float edge1, float x);
	void SetEntity(GameObject* entity);


private:
	glm::vec3 offset;
	GameObject* entity;
	GameObject* followEntity;
	float speed;
	float distance;
	float timeLimit;
	float elapsedTime;
};