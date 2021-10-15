#include "cFirework.h"

namespace nPhysics
{
	cFirework::cFirework(float mass, const glm::vec3& position) : cParticle(mass, position)
	{
		age = 0;
		isAlive = true;
		stage = 0;
		this->SetAcceleration(glm::vec3(0, -9.3f, 0));
	}

	cFirework::~cFirework()
	{

	}

	void cFirework::GenerateChildren(std::vector<cFirework*>& childrenOut, glm::vec3 location)
	{
		for (int i = 0; i < 50; i++)
		{
			nPhysics::cFirework* particle = new nPhysics::cFirework(1.0f, location);
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

		// clear applied forces
		ClearAppliedForces();
	}

	int cFirework::GetStage()
	{
		return stage;
	}

	void cFirework::SetStageOne()
	{
		
	}

	void cFirework::SetStageTwo()
	{
		
	}
}