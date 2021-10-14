#include "cParticle.h"

namespace nPhysics
{
	/*
	cParticle::cParticle()
		: mPosition(glm::vec3(0,0,0))
		, mVelocity(0.f)
		, mAcceleration(0.f)
		, mDamping(0.995f)
		, mAppliedForce(0.f)
	{
		mInverseMass = 0.5;
	}
	*/
	cParticle::cParticle(float mass, const glm::vec3& position)
		: mPosition(position)
		, mVelocity(0.f)
		, mAcceleration(0.f)
		, mDamping(0.995f)
		, mAppliedForce(0.f)
	{
		if (mass <= 0.f)
		{
			mInverseMass = 0.f;
		}
		else
		{
			mInverseMass = 1.f / mass;
		}
	}
	cParticle::~cParticle()
	{

	}

	void cParticle::SetDamping(float damping)
	{
		mDamping = glm::clamp(damping, 0.f, 1.f);
	}
	float cParticle::GetDamping() const
	{
		return mDamping;
	}

	float cParticle::GetMass() const
	{
		if (mInverseMass == 0.f)
		{
			return mInverseMass;
		}
		return 1.0f / mInverseMass;
	}

	float cParticle::GetInverseMass() const
	{
		return mInverseMass;
	}

	bool cParticle::IsStaticObject() const
	{
		return mInverseMass == 0.f;
	}

	glm::vec3 cParticle::GetPosition() const
	{
		return mPosition;
	}
	void cParticle::GetPosition(glm::vec3& position)
	{
		position = mPosition;
	}
	void cParticle::SetPosition(const glm::vec3& position)
	{
		mPosition = position;
	}

	glm::vec3 cParticle::GetVelocity() const
	{
		return mVelocity;
	}
	void cParticle::GetVelocity(glm::vec3& velocity)
	{
		velocity = mVelocity;
	}
	void cParticle::SetVelocity(const glm::vec3& velocity)
	{
		mVelocity = velocity;
	}

	glm::vec3 cParticle::GetAcceleration() const
	{
		return mAcceleration;
	}
	void cParticle::GetAcceleration(glm::vec3& acceleration)
	{
		acceleration = mAcceleration;
	}
	void cParticle::SetAcceleration(const glm::vec3& acceleration)
	{
		mAcceleration = acceleration;
	}

	void cParticle::ApplyForce(const glm::vec3& force)
	{
		mAppliedForce += force;
	}
	void cParticle::ClearAppliedForces()
	{
		mAppliedForce.x = 0; 
		mAppliedForce.y = 0;
		mAppliedForce.z = 0;
	}

	void cParticle::Integrate(float deltaTime)
	{
		if (mInverseMass == 0.f)
		{
			return; // static things don't move!
		}

		mPosition += mVelocity * deltaTime;
		// F*(1/m) = a
		mVelocity += (mAcceleration + mAppliedForce * mInverseMass) * deltaTime;

		// apply damping
		mVelocity *= glm::pow(mDamping, deltaTime);

		// clear applied forces
		ClearAppliedForces();
	}
}