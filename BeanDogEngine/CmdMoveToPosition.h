#pragma once
#include "iCommand.h"
#include "GameObject.h"
#include <glm/vec3.hpp>

class CmdMoveToPosition : public iCommand
{
public:
	CmdMoveToPosition(bool moveBySeconds, float timeOrSpeed, glm::vec3 toPosition, glm::vec3 fromPosition, bool relative);
	virtual ~CmdMoveToPosition();

	virtual void Initialize(sMessage startData);
	virtual void Start(sMessage startData);
	virtual void End(sMessage endData);
	virtual void Update(float deltaTime);
	virtual bool IsDone(void);

	float SmoothStep(float edge0, float edge1, float x);
	void SetEntity(GameObject* entity);

private:
	glm::vec3 from;
	glm::vec3 to;
	glm::vec3 relativeOffset;
	GameObject* entity;
	bool moveByTime;
	bool relative;
	float elapsedTime;
	float totalTime;
	float speed;
	float totalDistance;
};

