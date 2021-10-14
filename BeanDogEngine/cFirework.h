#pragma once
#include "cParticle.h"
#include "random_helpers.h"

namespace nPhysics
{
	class cFirework : public cParticle
	{
	protected:
		float age;
		int stage;
		bool isAlive;
	public:
		cFirework(float mass, const glm::vec3& position);
		virtual ~cFirework();

		virtual void GenerateChildren(std::vector<cFirework*>& childrenOut);
		virtual bool IsAlive();
		virtual void Integrate(float deltaTime);
		int GetStage();
		void SetStageOne();
		void SetStageTwo();
	};
}
