#include "cFirework.h"

namespace nPhysics
{
	cSpiral::cSpiral(float mass, const glm::vec3& position) : cFirework(mass, position)
	{

	}
	cSpiral::~cSpiral()
	{

	}
	void cSpiral::GenerateChildren(std::vector<cFirework*>& childrenOut, glm::vec3 location)
	{
		//Genereate 50 new stage 2 particles
		for (int i = 0; i < 50; i++)
		{
			nPhysics::cSpiral* particle = new nPhysics::cSpiral(1.0f, location);
			particle->SetStageTwo(i);
			childrenOut.push_back(particle);
		}
	}
	void cSpiral::Integrate(float deltaTime)
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

		if (age >= 1.5f && stage == 2)
		{
			isAlive = false;
		}
		// clear applied forces
		ClearAppliedForces();
	}

	void cSpiral::SetStageOne()
	{
		//set the stage
		stage = 1;
		//launch it in the air
		this->SetVelocity(glm::vec3(0, getRandom(10, 15), 0));
	}
	void cSpiral::SetStageTwo(int num)
	{
		//set the stage
		stage = 2;
		//send them in a random direction
		this->SetVelocity(getSpiralVec(num, 1));
		this->SetAcceleration(glm::vec3(0.0f, 9.8f, 0.0f));
	}
}