#include "Doors.h"

Doors::Doors(std::vector<std::vector<cMesh*>> mesh, float distance)
{
	meshList = mesh;
	//position = mesh->transformXYZ;
	startingPoint1 = position;
	this->distance = distance;
	this->direction = direction;
	open = false;
	bool isInAnimation = false;
}

void Doors::update(float deltaTime)
{
	/*
	if (open && position.x > startingPoint.x + (direction * distance))
	{
		position += glm::vec3(direction * deltaTime, 0, 0);
		mesh->transformXYZ = position;

	}

	else if (open && position.x < startingPoint.x + (direction * distance))
	{
		position += glm::vec3(direction * deltaTime, 0, 0);
		mesh->transformXYZ = position;
	}
	*/
}