#include "cProjectile.h"

namespace nPhysics
{
	cEnergyBall::cEnergyBall(float mass, const glm::vec3& position, float ageLimit) : cProjectile(mass, position)
	{
		this->ageLimit = ageLimit;
	}
	cEnergyBall::~cEnergyBall()
	{

	}
	void cEnergyBall::Integrate(float deltaTime)
	{
		cProjectile::Integrate(deltaTime);
		if (age >= ageLimit)
		{
			isAlive = false;
		}
	}
}