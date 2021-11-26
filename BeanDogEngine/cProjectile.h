#pragma once
#include "cParticle.h"
#include "cMesh.h"

namespace nPhysics
{
	class cProjectile : public cParticle
	{
	protected:
		float age;
		bool isAlive;
	public:
		cMesh* myMesh;

		cProjectile(float mass, const glm::vec3& position, cMesh* myMesh);
		virtual ~cProjectile();

		bool IsAlive();
		virtual void Integrate(float deltaTime);
	};
}