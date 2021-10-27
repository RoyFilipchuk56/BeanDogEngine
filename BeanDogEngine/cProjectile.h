#pragma once
#include "cParticle.h"

namespace nPhysics
{
	class cProjectile : public cParticle
	{
	protected:
		float age;
		bool isAlive;
	public:
		cProjectile(float mass, const glm::vec3& position);
		virtual ~cProjectile();

		bool IsAlive();
		virtual void Integrate(float deltaTime);
	};

	class cBullet : public cProjectile
	{
	public:
		cBullet(float mass, const glm::vec3& position);
		virtual ~cBullet();

		virtual void Integrate(float deltaTime);
	};

	class cLaser : public cProjectile
	{
	public:
		cLaser(float mass, const glm::vec3& position, float maxDistance);
		virtual ~cLaser();

		virtual void Integrate(float deltaTime);
		float distance;
		float maxDistance;
	};

	class cCannonBall : public cProjectile
	{
	public:
		cCannonBall(float mass, const glm::vec3& position);
		virtual ~cCannonBall();

		virtual void Integrate(float deltaTime);
	};

	class cEnergyBall : public cProjectile
	{
	public:
		cEnergyBall(float mass, const glm::vec3& position, float ageLimit);
		virtual ~cEnergyBall();

		virtual void Integrate(float deltaTime);
		float ageLimit;
	};
}