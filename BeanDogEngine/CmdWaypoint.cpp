#include "CmdWaypoint.h"
#include "MoveTowards.h"
#include <iostream>

CmdWaypoint::CmdWaypoint(bool moveBySeconds, float timeOrSpeed, std::vector<glm::vec3> waypoints)
	: elapsedTime(0), waypoints(waypoints), curWaypoint(0), totalDistance(0), entity(NULL), totalTime(0), speed(0), distanceCompleted(0)
	, lastPos(glm::vec3(0)), smoothstepMultiplier(0)
{
	//Load in default values 
	//these will be changed later since the entity may move with other commands before this one
	this->moveByTime = moveBySeconds;

	if (moveBySeconds)
	{
		totalTime = timeOrSpeed / (float)waypoints.size();
	}
	else
		speed = timeOrSpeed;
}

CmdWaypoint::~CmdWaypoint()
{
}

void CmdWaypoint::Initialize(sMessage startData)
{
}

void CmdWaypoint::Start(sMessage startData)
{
	//Updating the first waypoint which is the entitys starting position
	waypoints.insert(waypoints.begin(), entity->GetPosition());
	lastPos = entity->GetPosition();
	//Set the total distance
	for (int i = 0; i < waypoints.size() - 1; i++)
	{
		totalDistance += glm::distance(waypoints[i + 1], waypoints[i]);
	}
	smoothstepMultiplier = glm::clamp(totalDistance / 15.f, 0.f, 80.f);
	curWaypoint = 0;
}

void CmdWaypoint::End(sMessage endData)
{
}

void CmdWaypoint::Update(float deltaTime)
{
	if (moveByTime)
	{
		// Move to position over time
		elapsedTime += deltaTime;

		float weight = SmoothStep(0.f, 1.f, elapsedTime / totalTime);
		float x = waypoints[curWaypoint].x + (waypoints[curWaypoint + 1].x - waypoints[curWaypoint].x) * weight;
		float y = waypoints[curWaypoint].y + (waypoints[curWaypoint + 1].y - waypoints[curWaypoint].y) * weight;
		float z = waypoints[curWaypoint].z + (waypoints[curWaypoint + 1].z - waypoints[curWaypoint].z) * weight;
		glm::vec3 currentPos = entity->GetPosition();
		float distance = glm::distance(currentPos, waypoints[curWaypoint + 1]);
		if (distance <= 0.01f)
		{
			elapsedTime = 0;
			curWaypoint++;
		}
		entity->SetPosition(glm::vec3(x, y, z));
	}
	else
	{ 
		// Move to position by speed
		glm::vec3 currentPos = entity->GetPosition();
		distanceCompleted += glm::distance(currentPos, lastPos);

		float distToGo = glm::distance(currentPos, waypoints[curWaypoint+1]);
		if (distToGo <= 0.0001f)
		{
			curWaypoint++;
			return;
		}

		// Ease-in and out
		float t = distanceCompleted / totalDistance;
		if (t > 0.5f)
			t = 1 - t;
		float weight = glm::clamp(SmoothStep(0.f, 1.f, t * smoothstepMultiplier), 0.1f, 1.f);

		// Move towards point without passing it
		glm::vec3 delta = MoveTowards(currentPos, waypoints[curWaypoint + 1], speed * weight * deltaTime);
		entity->SetPosition(delta);
		lastPos = currentPos;
	}
}

bool CmdWaypoint::IsDone(void)
{
	glm::vec3 currentPos = entity->GetPosition();
	float distance = glm::distance(currentPos, waypoints[curWaypoint]);
	if (curWaypoint == waypoints.size() - 1 && distance <= 0.01f)
		return true;

	return false;
}

float CmdWaypoint::SmoothStep(float edge0, float edge1, float x)
{
	float t = glm::clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
	return t * t * (3.0f - 2.0f * t);
}

void CmdWaypoint::SetEntity(GameObject* entity)
{
	this->entity = entity;
}
