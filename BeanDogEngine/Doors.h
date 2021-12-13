#pragma once
#include "iEntity.h"
#include "cMesh.h";
#include <glm/vec3.hpp>;
#include <vector>
class Doors : public iEntity
{
public:
	std::vector<std::vector<cMesh*>> meshList;
	glm::vec3 position;
	glm::vec3 startingPoint1;
	glm::vec3 startingPoint2;
	float endPoint;
	float distance;
	float direction;
	bool open;
	bool isInAnimation;
	bool isDoorOneOpen;
	bool isDoorTwoOpen;

	Doors(std::vector<std::vector<cMesh*>> mesh, float distance);
	virtual ~Doors() {};
	virtual void update(float deltaTime);
	virtual void UpdateCommands(float deltaTime) = 0;
	virtual void SetFriendlyName(std::string name) = 0;
	virtual std::string GetFriendlyName() = 0;
	virtual cMesh* GetMesh() = 0;
};

class LeftDoor : public Doors
{
public:

	LeftDoor(std::vector<std::vector<cMesh*>> mesh, float distance);
	virtual ~LeftDoor() {};
	virtual void update(float deltaTime);
};

class RightDoor : public Doors
{
public:

	RightDoor(std::vector<std::vector<cMesh*>> mesh, float distance);
	virtual ~RightDoor() {};
	virtual void update(float deltaTime);
};