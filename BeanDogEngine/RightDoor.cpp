#include "Doors.h"

RightDoor::RightDoor(std::vector<std::vector<cMesh*>> mesh, float distance) : Doors(mesh, distance)
{
	meshList = mesh;
	position = meshList[0][0]->transformXYZ;
	startingPoint1 = position;
	startingPoint2 = meshList[1][0]->transformXYZ;
	endPoint = position.x + distance;
	this->distance = distance;
	this->direction = 1;
	open = false;
	isDoorOneOpen = false;
	isDoorTwoOpen = false;
}

void RightDoor::update(float deltaTime)
{
	position += glm::vec3(0, 0, direction * 0.1);

	if (open)
	{
		if (meshList[0][0]->transformXYZ.z < endPoint)
		{
			isDoorOneOpen = true;
			for (cMesh* wall : meshList[0])
			{
				wall->transformXYZ += glm::vec3(0, 0, direction * 0.02);
			}
		}
		else
		{
			isDoorOneOpen = false;
		}

		if (meshList[1][0]->transformXYZ.z < endPoint)
		{
			isDoorTwoOpen = true;
			for (cMesh* wall : meshList[1])
			{
				wall->transformXYZ += glm::vec3(0, 0, direction * 0.01);
			}
		}
		else
		{
			isDoorTwoOpen = false;
		}

		if (isDoorOneOpen || isDoorTwoOpen)
		{
			isInAnimation = true;
			return;
		}
	}

	else if (!open)
	{
		if (meshList[0][0]->transformXYZ.z > startingPoint1.z)
		{
			isDoorOneOpen = true;
			for (cMesh* wall : meshList[0])
			{
				wall->transformXYZ += glm::vec3(0, 0, -direction * 0.02);
			}
		}
		else
		{
			isDoorOneOpen = false;
		}
		if (meshList[1][0]->transformXYZ.z > startingPoint2.z)
		{
			isDoorTwoOpen = true;
			for (cMesh* wall : meshList[1])
			{
				wall->transformXYZ += glm::vec3(0, 0, -direction * 0.01);
			}
		}
		else
		{
			isDoorTwoOpen = false;
		}
		if (isDoorOneOpen || isDoorTwoOpen)
		{
			isInAnimation = true;
			return;
		}
	}

	isInAnimation = false;
	//isInAnimation = false;
}