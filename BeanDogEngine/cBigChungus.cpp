#include "cFirework.h"

namespace nPhysics
{
	cBigChungus::cBigChungus(float mass, const glm::vec3& position) : cFirework(mass, position)
	{

	}
	cBigChungus::~cBigChungus()
	{

	}
	void cBigChungus::GenerateChildren(std::vector<cFirework*>& childrenOut, glm::vec3 location)
	{
		//Genereate 50 new stage 2 particles
		for (int i = 0; i < 50; i++)
		{
			nPhysics::cBigChungus* particle = new nPhysics::cBigChungus(1.0f, location);
			particle->SetStageTwo();
			childrenOut.push_back(particle);
		}
	}
	void cBigChungus::Integrate(float deltaTime)
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

	void cBigChungus::SetStageOne()
	{
		//set the stage
		stage = 1;
		//launch it in the air
		this->SetVelocity(glm::vec3(0, 15, 0));
	}
	void cBigChungus::SetStageTwo()
	{
		//set the stage
		stage = 2;
		//send them in a random direction
		this->SetVelocity(getRandomVec3(10));
	}
}