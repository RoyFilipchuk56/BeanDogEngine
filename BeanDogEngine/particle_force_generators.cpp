#include "particle_force_generators.h"

namespace nPhysics
{
	cParticleGravityGenerator::cParticleGravityGenerator(const glm::vec3& gravityAcceleration)
		: mGravityAcceleration(gravityAcceleration)
	{

	}

	cParticleGravityGenerator::~cParticleGravityGenerator()
	{

	}

	void cParticleGravityGenerator::Update(cParticle* particle, float deltaTime)
	{
		// F = M*A
		particle->ApplyForce(mGravityAcceleration * particle->GetMass());
	}

	cBuoyancyForceGenerator::cBuoyancyForceGenerator(float maxDepth, float waterHeight, float waterDensity, float volume) : maxDepth(maxDepth), waterHeight(waterHeight), waterDensity(waterDensity), volume(volume)
	{
		//TODO: calculate volume
	}
	cBuoyancyForceGenerator::~cBuoyancyForceGenerator()
	{
	}
	void cBuoyancyForceGenerator::Update(cParticle* particle, float deltaTime)
	{		
		//Get the position of the particle
		float particleYPos = particle->GetPosition().y;

		//The force were going to apply to our particle
		glm::vec3 theForce(0.0f, 0.0f, 0.0f);

		//If were above the water do nothing
		if (particleYPos >= waterHeight + maxDepth)
		{
			particle->ApplyForce(theForce);
			return;
		}

		//Check if the object is fully submerged
		if (particleYPos <= waterHeight - maxDepth)
		{
			theForce.y = waterDensity * volume;
			particle->ApplyForce(theForce);
			return;
		}

		//Otherwise its only partialy submerged
		theForce.y = waterDensity * volume * (particleYPos - maxDepth - waterHeight) / 2 * maxDepth;
		particle->ApplyForce(theForce);
	}
}