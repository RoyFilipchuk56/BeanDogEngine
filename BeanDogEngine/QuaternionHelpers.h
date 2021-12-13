#pragma once
#include <glm/gtx/quaternion.hpp>

namespace quaternion
{
	inline glm::quat QuatFromAngles(const glm::vec3& angles)
	{
		return glm::angleAxis(angles.x, glm::vec3(1, 0, 0)) *
			glm::angleAxis(angles.y, glm::vec3(0, 1, 0)) *
			glm::angleAxis(angles.z, glm::vec3(0, 0, 1));
	}

	inline glm::quat Identity()
	{
		return glm::quat(glm::vec3(0.f));
	}

	// Approxamates if two quaternions are equals based on a range
	inline bool Equals(glm::quat a, glm::quat b, float minRange)
	{
		return ((1 - glm::abs(glm::dot(a, b))) <= minRange);
	}
}