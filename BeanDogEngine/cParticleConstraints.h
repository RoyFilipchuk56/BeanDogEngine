#pragma once
#include "iParticleContactGenerator.h"

namespace nPhysics
{
	//The base for all contraints between particles
	class cParticleConstraint : public iParticleContactGenerator
	{
	protected:
		cParticle* myParticleA;
		cParticle* myParticleB;
	public:
		cParticleConstraint(cParticle* particleA, cParticle* particleB);

		cParticleConstraint() = delete;
		cParticleConstraint(const cParticleConstraint& other) = delete;
		cParticleConstraint& operator=(const cParticleConstraint& other) = delete;

		virtual ~cParticleConstraint();

		float CurrentSeparation() const;

		cParticle* GetParticleA();
		cParticle* GetParticleB();
	};

	//Rod particle contraint, acts like theres a connecting rod between the two particles
	class cParticleRodConstraint : public cParticleConstraint
	{
	protected:
		float myLength;
	public:
		cParticleRodConstraint(cParticle* particleA, cParticle* particleB);

		cParticleRodConstraint() = delete;
		cParticleRodConstraint(const cParticleRodConstraint& other) = delete;
		cParticleRodConstraint& operator=(const cParticleRodConstraint& other) = delete;

		virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
	};
}