#include "cProjectile.h"

namespace nPhysics
{
	cBullet::cBullet(float mass, const glm::vec3& position) : cProjectile(mass, position)
	{

	}
	cBullet::~cBullet()
	{

	}
	void cBullet::Integrate(float deltaTime)
	{
		cProjectile::Integrate(deltaTime);
	}
}