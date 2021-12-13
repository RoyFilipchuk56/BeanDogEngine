#include "cParticleConstraints.h"

namespace nPhysics
{
	//Particle contraint base class
	cParticleConstraint::cParticleConstraint(cParticle* particleA, cParticle* particleB)
		: myParticleA(particleA), myParticleB(particleB)
	{

	}

	cParticleConstraint::~cParticleConstraint() {}

	float cParticleConstraint::CurrentSeparation() const
	{
		return glm::distance(myParticleA->GetPosition(), myParticleB->GetPosition());
	}

	cParticle* cParticleConstraint::GetParticleA()
	{
		return myParticleA;
	}

	cParticle* cParticleConstraint::GetParticleB()
	{
		return myParticleB;
	}

	//Rod contraint
	cParticleRodConstraint::cParticleRodConstraint(cParticle* particleA, cParticle* particleB)
		: cParticleConstraint(particleA, particleB)
	{
		myLength = CurrentSeparation();
	}

	size_t cParticleRodConstraint::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const
	{
		//Get the length of the rod
		float currentLen = CurrentSeparation();

		//Check if the rod is over extended
		if (currentLen == myLength)
		{
			return 0;
		}

		//If not then set the contact particles
		contact->mParticle0 = myParticleA;
		contact->mParticle1 = myParticleB;

		//Calculate the normal
		glm::vec3 normal = glm::normalize(myParticleB->GetPosition() - myParticleA->GetPosition());

		//The contact normal depends on if we are extending or compressing
		if (currentLen > myLength)
		{
			contact->mContactNormal = normal;
			contact->mPenetration = currentLen - myLength;
		}
		else
		{
			contact->mContactNormal = normal * -1.0f;
			contact->mPenetration = myLength - currentLen;
		}

		// Set the restitution so theres its not bouncy
		contact->mRestitution = 0.0f;

		return 1;
	}
}