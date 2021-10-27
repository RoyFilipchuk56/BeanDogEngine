#include "cProjectile.h"

namespace nPhysics
{
	cLaser::cLaser(float mass, const glm::vec3& position, float maxDistance) : cProjectile(mass, position)
	{
		distance = 0;
		this->maxDistance = maxDistance;
	}
	cLaser::~cLaser()
	{

	}
	void cLaser::Integrate(float deltaTime)
	{
		glm::vec3 posBefore = this->GetPosition();
		cProjectile::Integrate(deltaTime);
		glm::vec3 difference = this->GetPosition() - posBefore;
		distance += difference.length();
		//TODO: get distance from xml
		if (distance >= maxDistance)
		{
			isAlive = false;
		}
	}
}