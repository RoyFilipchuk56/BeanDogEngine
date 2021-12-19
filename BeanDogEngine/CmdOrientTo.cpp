#include "CmdOrientTo.h"
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Constructor for creating an orientTo command without a point to look at.
// It will probably need to be given an entity too look at.
CmdOrientTo::CmdOrientTo(bool moveBySeconds, float timeOrSpeed, bool relative) : relative(relative), elapsedTime(0), entity(NULL)
	, lookAtEntity(NULL), to(glm::vec3(0)), speed(0), totalTime(0)
{
	this->moveByTime = moveBySeconds;

	if (moveBySeconds)
	{
		totalTime = timeOrSpeed;
	}
	else
		speed = timeOrSpeed;

	this->toRotation = quaternion::Identity();
	this->fromRotation = quaternion::Identity();
}

// Constructor for creating an orientTo command with a known point to look at
CmdOrientTo::CmdOrientTo(bool moveBySeconds, float timeOrSpeed, glm::vec3 lookLocation, bool relative) : relative(relative), elapsedTime(0), entity(NULL)
, lookAtEntity(NULL), speed(0), totalTime(0)
{
	this->moveByTime = moveBySeconds;

	if (moveBySeconds)
	{
		totalTime = timeOrSpeed;
	}
	else
		speed = timeOrSpeed;

	this->to = lookLocation;
	this->toRotation = quaternion::Identity();
	this->fromRotation = quaternion::Identity();
}

CmdOrientTo::~CmdOrientTo()
{
}

void CmdOrientTo::Initialize(sMessage startData)
{
}

void CmdOrientTo::Start(sMessage startData)
{
	// Get the start rotations when this command starts
	fromRotation = entity->GetRotationQuat();
	elapsedTime = 0;

	// If looking at a position, get the position based on the entity location when the command starts
	if (lookAtEntity == NULL)
	{
		glm::vec3 pos = glm::normalize(to - entity->GetPosition());

		if (relative)
		{
			toRotation = fromRotation * quaternion::QuatFromAngles(glm::vec3(glm::radians(to.x), glm::radians(to.y), glm::radians(to.z)));
			totalAngle = glm::angle(toRotation) - glm::angle(fromRotation);
		}
		else
			toRotation = glm::quatLookAt(pos, glm::vec3(0, 1, 0));
	}
}

void CmdOrientTo::End(sMessage endData)
{
}

void CmdOrientTo::Update(float deltaTime)
{
	elapsedTime += deltaTime;

	// Look at entity
	if (lookAtEntity != NULL)
	{
		if (moveByTime)
		{
			// rotate by time
			toRotation = glm::quatLookAt(glm::normalize(lookAtEntity->GetPosition() - entity->GetPosition()), glm::vec3(0,1,0));

			float weight = SmoothStep(0, 1.f, elapsedTime / totalTime);
			entity->SetRotationQuat(glm::slerp(fromRotation, toRotation, weight));
		}
		else
		{
			// rotate by speed
			toRotation = glm::quatLookAt(glm::normalize(lookAtEntity->GetPosition() - entity->GetPosition()), glm::vec3(0, 1, 0));
			//fromRotation = entity->GetRotationQuat();
			glm::quat diff = toRotation * glm::inverse(fromRotation);
			glm::quat rotation = glm::angleAxis(glm::radians(speed * deltaTime), glm::axis(glm::normalize(diff)));

			//glm::quat newRotation = glm::slerp(entity->GetRotationQuat(), toRotation, deltaTime);

			//entity->SetRotationQuat(newRotation);
			entity->SetRotationQuat(entity->GetRotationQuat() * rotation);
		}
	}

	// Look at position
	else
	{
		if (moveByTime)
		{
			// rotate by time
			float weight = SmoothStep(0, 1.f, elapsedTime / totalTime);
			entity->SetRotationQuat(glm::slerp(fromRotation, toRotation, weight));
		}
		else
		{
			// rotate by speed
			// very annoying math
			glm::quat diff = toRotation * glm::inverse(fromRotation);

			glm::quat rotation = glm::angleAxis(glm::radians(speed * deltaTime), glm::axis(glm::normalize(diff)));
			//glm::quat newRotation = glm::slerp(entity->GetRotationQuat(), entity->GetRotationQuat() * rotation, deltaTime);

			entity->SetRotationQuat(entity->GetRotationQuat() * rotation);
		}

	}
}

bool CmdOrientTo::IsDone(void)
{
	if (quaternion::Equals(entity->GetRotationQuat(), toRotation, 0.5f))
	{
		return true;
	}

	return false;
}

float CmdOrientTo::SmoothStep(float edge0, float edge1, float x)
{
	float t = glm::clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
	return t * t * (3.0f - 2.0f * t);
}

void CmdOrientTo::SetEntity(GameObject* entity)
{
	this->entity = entity;
}

void CmdOrientTo::SetLookAtEntity(GameObject* lookat)
{
	this->lookAtEntity = lookat;
}
