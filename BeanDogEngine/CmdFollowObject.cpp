#include "CmdFollowObject.h"
#include "MoveTowards.h"

CmdFollowObject::CmdFollowObject(glm::vec3 offset, float maxSpeed, float distance, float maxTime, GameObject* followEntity) : speed(maxSpeed), followEntity(followEntity)
	, offset(offset), entity(NULL), elapsedTime(0), timeLimit(maxTime), distance(distance)
{

}

CmdFollowObject::~CmdFollowObject()
{
}

void CmdFollowObject::Initialize(sMessage startData)
{
}

void CmdFollowObject::Start(sMessage startData)
{
}

void CmdFollowObject::End(sMessage endData)
{
}

void CmdFollowObject::Update(float deltaTime)
{
	elapsedTime += deltaTime;

	glm::vec3 goalPosition = followEntity->GetPosition() + glm::normalize(offset) * distance;

	entity->SetPosition(MoveTowards(entity->GetPosition(), goalPosition, speed * deltaTime));
}

bool CmdFollowObject::IsDone(void)
{
	if (elapsedTime >= timeLimit)
		return true;

	return false;
}

float CmdFollowObject::SmoothStep(float edge0, float edge1, float x)
{
	return 0.0f;
}

void CmdFollowObject::SetEntity(GameObject* entity)
{
	this->entity = entity;
}
