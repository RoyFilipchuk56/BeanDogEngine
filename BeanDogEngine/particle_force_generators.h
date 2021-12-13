#pragma once
#include "iParticleForceGenerator.h"

namespace nPhysics
{
	class cParticleGravityGenerator : public iParticleForceGenerator
	{
	public:
		cParticleGravityGenerator(const glm::vec3& gravityAcceleration);

		cParticleGravityGenerator() = delete;
		cParticleGravityGenerator(cParticleGravityGenerator& other) = delete;
		cParticleGravityGenerator& operator=(cParticleGravityGenerator& other) = delete;

		virtual ~cParticleGravityGenerator();
		virtual void Update(cParticle* particle, float deltaTime);

	private:
		glm::vec3 mGravityAcceleration;
	};

	class cBuoyancyForceGenerator : public iParticleForceGenerator
	{
	private:
		float maxDepth;
		float waterHeight;
		float waterDensity;
		float volume;

	public:
		cBuoyancyForceGenerator(float maxDepth, float waterHeight, float waterDensity, float volume);

		cBuoyancyForceGenerator() = delete;
		cBuoyancyForceGenerator(cBuoyancyForceGenerator& other) = delete;
		cBuoyancyForceGenerator& operator=(cBuoyancyForceGenerator& other) = delete;

		virtual ~cBuoyancyForceGenerator();
		virtual void Update(cParticle* particle, float deltaTime);
	};
}