#include "cParticleContactGenerators.h"
#include <iostream>
namespace nPhysics
{
	cPlaneParticleContactGenerator::cPlaneParticleContactGenerator(glm::vec3 position, glm::vec3 normal)
		: myPosition(position)
		, myNormal(glm::normalize(normal))
	{

	}

	cPlaneParticleContactGenerator::~cPlaneParticleContactGenerator()
	{

	}

	size_t cPlaneParticleContactGenerator::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const
	{
		size_t numContactsCreated = 0;
		for (std::vector<cParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
		{
			//Get the position of the particle
			glm::vec3 particlePos = (*it)->GetPosition();
			//Get the distance between the plane and the position
			glm::vec3 distanceBetween = particlePos - myPosition;
			//Comapir the two vectors
			float theDot = glm::dot(myNormal, distanceBetween);
			//If the particle is currently under the plane
			if (theDot <= 0)
			{
				//create the contact
				contact->mContactNormal = myNormal;
				contact->mParticle0 = *it;
				contact->mParticle1 = nullptr;
				contact->mPenetration = theDot * -1;
				contact->mRestitution = 0.4f;
				contact++;
				numContactsCreated++;
			}
			if (numContactsCreated >= limit)
			{
				return numContactsCreated;
			}
		}
		return numContactsCreated;
	}

	
	cSphereParticleContactGenerator::cSphereParticleContactGenerator()
	{

	}

	cSphereParticleContactGenerator::~cSphereParticleContactGenerator()
	{

	}

	size_t cSphereParticleContactGenerator::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const
	{
		size_t numContactsCreated = 0;
		int particleSize = particles.size();

		//Program gets stuck in an infinate loop if i have particles.size() - 1
		for (int particleAId = 0; particleAId < particleSize - 1; particleAId++)
		{
			for (int particleBId = particleAId + 1; particleBId < particleSize; particleBId++)
			{
				//compare particles
				nPhysics::cParticle* particleA = particles[particleAId];
				nPhysics::cParticle* particleB = particles[particleBId];

				glm::vec3 midline = particleA->GetPosition() - particleB->GetPosition();
				float mag = glm::length(midline);

				//TODO: replace 0.5 with sphere radius
				if (mag <= 0.0f || mag >= 0.5 + 0.5)
				{
					continue;
				}
				
				glm::vec3 normal = midline * mag;

				contact->mContactNormal = normal;
				contact->mParticle0 = particleA;
				contact->mParticle1 = particleB;
				contact->mPenetration = 0.5 + 0.5 - mag;
				contact->mRestitution = 0.7f;
				contact++;
				numContactsCreated++;

				if (numContactsCreated >= limit)
				{
					return numContactsCreated;
				}

			}
		}
		return numContactsCreated;
	}
}