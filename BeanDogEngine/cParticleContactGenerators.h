#pragma once
#include "iParticleContactGenerator.h"
#include "cParticleContact.h"

namespace nPhysics
{
	/// <summary>
	/// This is a plane that that is defined by its position from the origin and its normal
	/// Objects will cantact at its position and in the direction of its normal
	/// </summary>
	class cPlaneParticleContactGenerator : public iParticleContactGenerator
	{
	private:
		glm::vec3 myPosition;
		glm::vec3 myNormal;
	public:
		cPlaneParticleContactGenerator(glm::vec3 position, glm::vec3 normal);

		virtual ~cPlaneParticleContactGenerator();

		/// <summary>
		/// Fills the given contact structure with the generated contact(s).
		/// The contact pointer should point to the first available contact in the contact array,
		/// where limit is the maximum number of contacts in the array that can be written to.
		/// The method returns the number of contacts generated.
		/// </summary>
		/// <param name="contact">The current contact in the array</param>
		/// <param name="limit">The size of the array</param>
		/// <param name="particles">The vector of particles</param>
		/// <returns></returns>
		virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
	};

	class cSphereParticleContactGenerator : public iParticleContactGenerator
	{
	public:
		cSphereParticleContactGenerator();

		virtual ~cSphereParticleContactGenerator();

		/// <summary>
		/// Fills the given contact structure with the generated contact(s).
		/// The contact pointer should point to the first available contact in the contact array,
		/// where limit is the maximum number of contacts in the array that can be written to.
		/// The method returns the number of contacts generated.
		/// </summary>
		/// <param name="contact">The current contact in the array</param>
		/// <param name="limit">The size of the array</param>
		/// <param name="particles">The vector of particles</param>
		/// <returns></returns>
		virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
	};
}