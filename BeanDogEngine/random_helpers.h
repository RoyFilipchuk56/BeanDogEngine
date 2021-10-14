#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace nPhysics
{
	float getRandom();
	float getRandom(float high);
	float getRandom(float low, float high);
	glm::vec3 getRandomVec3(float length);
}