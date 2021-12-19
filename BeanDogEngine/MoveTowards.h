#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>


// Move towards a vector without passing it
inline glm::vec3 MoveTowards(glm::vec3 current, glm::vec3 target, float maxDistance)
{

	glm::vec3 a = target - current;
	float magnitude = glm::length(a);

	if (magnitude <= maxDistance || magnitude == 0)
	{
		return target;
	}

	return current + (a / magnitude) * maxDistance;
}
