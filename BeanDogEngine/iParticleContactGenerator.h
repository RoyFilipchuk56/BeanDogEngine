#pragma once
#include "cParticleContact.h"

namespace nPhysics
{
	// This is the basic polymorphic interface for contact generators that apply to particles.
	class iParticleContactGenerator
	{
	public:

		virtual ~iParticleContactGenerator() {}
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
		virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const = 0;
	};
}