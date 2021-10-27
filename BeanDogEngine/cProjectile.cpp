#include "cProjectile.h"


namespace nPhysics
{
	cProjectile::cProjectile(float mass, const glm::vec3& position) : cParticle(mass, position)
	{
		age = 0;
		isAlive = true;
	}

	cProjectile::~cProjectile()
	{

	}

	bool cProjectile::IsAlive()
	{
		return isAlive;
	}

	void cProjectile::Integrate(float deltaTime)
	{
		if (mInverseMass == 0.f)
		{
			return; // static things don't move!
		}
		//Set the new age
		age += deltaTime;

		mPosition += mVelocity * deltaTime;
		// F*(1/m) = a
		mVelocity += (mAcceleration + mAppliedForce * mInverseMass) * deltaTime;

		// apply damping
		mVelocity *= glm::pow(mDamping, deltaTime);

		// clear applied forces
		ClearAppliedForces();

		//All Projectiles die when they hit the ground
		if (mPosition.y <= 0)
		{
			isAlive = false;
		}
	}
}