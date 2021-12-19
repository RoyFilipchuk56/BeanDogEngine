#pragma once
#include "iCommand.h"
#include "GameObject.h"
#include <vector>
#include <glm/vec3.hpp>

class CmdWaypoint : public iCommand
{
public:
	CmdWaypoint(bool moveBySeconds, float timeOrSpeed, std::vector<glm::vec3> waypoints);
	virtual ~CmdWaypoint();

	virtual void Initialize(sMessage startData);
	virtual void Start(sMessage startData);
	virtual void End(sMessage endData);
	virtual void Update(float deltaTime);
	virtual bool IsDone(void);

	float SmoothStep(float edge0, float edge1, float x);
	void SetEntity(GameObject* entity);

private:
	std::vector<glm::vec3> waypoints;
	int curWaypoint;
	GameObject* entity;
	bool moveByTime;
	float elapsedTime;
	float totalTime;
	float speed;
	float totalDistance;
	float distanceCompleted;
	float smoothstepMultiplier;
	glm::vec3 lastPos;
};