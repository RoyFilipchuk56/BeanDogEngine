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

		virtual void GenerateChildren(std::vector<cFirework*>& childrenOut, glm::vec3 location);
		virtual bool IsAlive();
		virtual void Integrate(float deltaTime);
		int GetStage();
		virtual void SetStageOne();
		virtual void SetStageTwo();
	};

	class cBigChungus : public cFirework
	{
	public:
		cBigChungus(float mass, const glm::vec3& position);
		virtual ~cBigChungus();
		virtual void GenerateChildren(std::vector<cFirework*>& childrenOut, glm::vec3 location);
		virtual void Integrate(float deltaTime);

		virtual void SetStageOne();
		virtual void SetStageTwo();
	};

	class cVortex : public cFirework
	{
	public:
		cVortex(float mass, const glm::vec3& position);
		virtual ~cVortex();
		virtual void GenerateChildren(std::vector<cFirework*>& childrenOut, glm::vec3 location);
		virtual void Integrate(float deltaTime);

		virtual void SetStageOne();
		virtual void SetStageTwo();
	};

	class cSpiral : public cFirework
	{
	public:
		cSpiral(float mass, const glm::vec3& position);
		virtual ~cSpiral();
		virtual void GenerateChildren(std::vector<cFirework*>& childrenOut, glm::vec3 location);
		virtual void Integrate(float deltaTime);

		virtual void SetStageOne();
		virtual void SetStageTwo(int num);
	};
}
