#pragma once
#include "iCommand.h"
#include "GameObject.h"
#include <glm/vec3.hpp>
#include "QuaternionHelpers.h"

class CmdOrientTo : public iCommand
{
public:
	CmdOrientTo(bool moveBySeconds, float timeOrSpeed, bool relative);
	CmdOrientTo(bool moveBySeconds, float timeOrSpeed, glm::vec3 lookLocation, bool relative);
	virtual ~CmdOrientTo();

	virtual void Initialize(sMessage startData);
	virtual void Start(sMessage startData);
	virtual void End(sMessage endData);
	virtual void Update(float deltaTime);
	virtual bool IsDone(void);

	float SmoothStep(float edge0, float edge1, float x);
	void SetEntity(GameObject* entity);
	void SetLookAtEntity(GameObject* lookat);

private:
	glm::vec3 to;
	GameObject* entity;
	GameObject* lookAtEntity;
	bool moveByTime;
	bool relative;
	float elapsedTime;
	float totalTime;
	float speed;
	glm::quat toRotation;
	glm::quat fromRotation;
	float totalAngle;
};