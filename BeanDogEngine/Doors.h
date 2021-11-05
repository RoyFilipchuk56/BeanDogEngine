#pragma once
#include "cMesh.h";
#include <glm/vec3.hpp>;
#include <vector>
class Doors
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
	virtual void update(float deltaTime);
};

class LeftDoor : public Doors
{
public:

	LeftDoor(std::vector<std::vector<cMesh*>> mesh, float distance);
	virtual void update(float deltaTime);
};

class RightDoor : public Doors
{
public:

	RightDoor(std::vector<std::vector<cMesh*>> mesh, float distance);
	virtual void update(float deltaTime);
};