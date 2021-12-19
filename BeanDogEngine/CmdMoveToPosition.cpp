#include "CmdMoveToPosition.h"
#include "MoveTowards.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

CmdMoveToPosition::CmdMoveToPosition(bool moveBySeconds, float timeOrSpeed, glm::vec3 toPosition, glm::vec3 fromPosition, bool relative) 
	: elapsedTime(0), from(fromPosition), to(toPosition), entity(NULL), totalTime(0), speed(0), relative(relative), relativeOffset(glm::vec3(0))
{
	this->moveByTime = moveBySeconds;
	this->totalDistance = glm::distance(toPosition, fromPosition);
	if (relative)
		relativeOffset = to;

	if (moveBySeconds)
	{
		totalTime = timeOrSpeed;
	}
	else
		speed = timeOrSpeed;
}

CmdMoveToPosition::~CmdMoveToPosition()
{
}

void CmdMoveToPosition::Initialize(sMessage startData)
{
}

void CmdMoveToPosition::Start(sMessage startData)
{
	from = entity->GetPosition();

	if (relative)
		to = from + relativeOffset;

	this->totalDistance = glm::distance(to, from);
	elapsedTime = 0;

}

void CmdMoveToPosition::End(sMessage endData)
{
}

void CmdMoveToPosition::Update(float deltaTime)
{
	if (moveByTime)
	{
		// Move to position over time
		elapsedTime += deltaTime;

		float weight = SmoothStep(0.f, 1.f, elapsedTime / totalTime);
		float x = from.x + (to.x - from.x) * weight;
		float y = from.y + (to.y - from.y) * weight;
		float z = from.z + (to.z - from.z) * weight;

		entity->SetPosition(glm::vec3(x, y, z));
	}
	else
	{
		// Move to position at a certain speed
		glm::vec3 currentPos = entity->GetPosition();
		float distance = glm::distance(currentPos, to);
		float t = (totalDistance - distance) / totalDistance;

		if (t > 0.5f)
			t = glm::abs(t - 1.f);	// Invert t to get an ease-out

		float weight = glm::clamp(SmoothStep(0.f, 1.f, t) * (totalDistance / 15.0f), 0.05f, 1.f);
		
		entity->SetPosition(MoveTowards(currentPos, to, speed * weight * deltaTime));

	}

}

bool CmdMoveToPosition::IsDone(void)
{
	glm::vec3 currentPos = entity->GetPosition();
	float distance = glm::distance(currentPos, to);
	if (distance <= 0.01f)
		return true;

	return false;
}

float CmdMoveToPosition::SmoothStep(float edge0, float edge1, float x)
{
	float t = glm::clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
	return t * t * (3.0f - 2.0f * t);
}

void CmdMoveToPosition::SetEntity(GameObject* entity)
{
	this->entity = entity;
}
