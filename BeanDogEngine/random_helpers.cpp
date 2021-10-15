#include "random_helpers.h" // function definitions, and glm includes

namespace nPhysics
{
	// Generate a random number between 0 and 1
	float getRandom()
	{
		return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	}
	// Generate a random number between zero and a given high value
	float getRandom(float high)
	{
		return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / high);
	}
	// Generate a random number in a given range
	float getRandom(float low, float high)
	{
		return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (high - low));
	}

	// Generate a glm::vec3 with a random direction and specified length
	glm::vec3 getRandomVec3(float length)
	{
		// play around with the range of alpha and delta to get arcs, hemispheres, etc...
		float alpha = getRandom(glm::pi<float>());
		float delta = getRandom(-glm::pi<float>(), glm::pi<float>());
		float sinAlpha = glm::sin(alpha);
		float cosAlpha = glm::cos(alpha);
		float sinDelta = glm::sin(delta);
		float cosDelta = glm::cos(delta);
		// play around with ordering to change the directionality of hemispheres, arcs, etc...
		// eg. (x,y,z), (y,z,x), (x,z,y)
		return glm::vec3(length * sinAlpha * cosDelta, length * sinAlpha * sinDelta, length * cosAlpha);
	}
	// Generate a glm::vec3 with a random direction of the x and z axis and specified length
	glm::vec3 getRandomVec3XZ(float length)
	{
		// play around with the range of alpha and delta to get arcs, hemispheres, etc...
		float alpha = getRandom(glm::pi<float>());
		float delta = getRandom(-glm::half_pi<float>(), glm::pi<float>());
		float sinAlpha = glm::sin(alpha);
		float cosAlpha = glm::cos(alpha);
		float sinDelta = glm::sin(delta);
		float cosDelta = glm::cos(delta);
		// play around with ordering to change the directionality of hemispheres, arcs, etc...
		// eg. (x,y,z), (y,z,x), (x,z,y)
		return glm::vec3(length * sinAlpha * cosDelta, 0.0f, length * cosAlpha);
	}

	glm::vec3 getSpiralVec(int num, int space)
	{
		float angle = 0.2f * num;
		return glm::vec3((space * angle) * cos(angle), 0, (space * angle) * sin(angle));
	}
}