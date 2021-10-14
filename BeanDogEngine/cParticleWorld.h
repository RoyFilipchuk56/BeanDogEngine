#pragma once
#include "cParticle.h"
#include "cParticleForceRegistry.h"
#include <vector>

namespace nPhysics
{
	class cParticleWorld
	{
	private:
		std::vector<cParticle*> mParticles;
		cParticleForceRegistry* mForceRegistry;
	public:
		cParticleWorld();
		virtual ~cParticleWorld();

		bool AddParticle(cParticle* particle);
		bool RemoveParticle(cParticle* particle);

		void TimeStep(float deltaTime);

		void IntegrateParticles(float deltaTime);

		cParticleForceRegistry* GetForceRegistry() const;
		
	};
}

