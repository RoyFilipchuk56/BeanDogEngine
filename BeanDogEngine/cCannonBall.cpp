#include "cProjectile.h"

namespace nPhysics
{
	cCannonBall::cCannonBall(float mass, const glm::vec3& position) : cProjectile(mass, position)
	{

	}
	cCannonBall::~cCannonBall()
	{

	}
	void cCannonBall::Integrate(float deltaTime)
	{
		cProjectile::Integrate(deltaTime);
	}
}