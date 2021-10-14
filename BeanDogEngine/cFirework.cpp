#include "cFirework.h"

namespace nPhysics
{
	cFirework::cFirework(float mass, const glm::vec3& position) : cParticle(mass, position)
	{
		age = 0;
		isAlive = true;
		stage = 0;
	}

	cFirework::~cFirework()
	{

	}

	void cFirework::GenerateChildren(std::vector<cFirework*>& childrenOut)
	{
		for (int i = 0; i < 50; i++)
		{
			nPhysics::cFirework* particle = new nPhysics::cFirework(1.0f, glm::vec3(0.f));
			particle->SetStageTwo();
			childrenOut.push_back(particle);
		}
	}

	bool cFirework::IsAlive()
	{
		return isAlive;
	}

	void cFirework::Integrate(float deltaTime)
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

		//Check if its still alive
		if (mVelocity.y <= 0 && stage == 1)
		{
			isAlive = false;
		}
		// clear applied forces
		ClearAppliedForces();
	}

	int cFirework::GetStage()
	{
		return stage;
	}

	void cFirework::SetStageOne()
	{
		stage = 1;
		this->SetVelocity(glm::vec3(0, 10, 0));
	}

	void cFirework::SetStageTwo()
	{
		stage = 2;
		this->SetVelocity(getRandomVec3(10));
	}
}