#pragma once
#include "cParticleContact.h"

namespace nPhysics
{
	
	// The contact resolution routine for particle contacts.
	// One resolver instance is generally shared for the entire simulation.
	class cParticleContactResolver
	{
	protected:
		// Holds the number of iterations allowed.
		size_t  mIterations;
		// This is a performance tracking value - we keep a record
		// of the actual number of iterations used.
		size_t mIterationsUsed;
	public:
		/// <summary>
		/// Creates a new contact resolver
		/// </summary>
		/// <param name="iterations">The number of iterations to use when solving contacts.</param>
		cParticleContactResolver(size_t iterations);
		/// <summary>
		/// Sets the number of iterations that can be used.
		/// </summary>
		/// <param name="iterations">The number of iterations to use when solving contacts.</param>
		void SetIterations(size_t iterations);

		/// <summary>
		/// Resolves a set of particle contacts for both penetration and velocity.
		/// </summary>
		/// <param name="contactArray">The array of contacts to be resolved</param>
		/// <param name="numContact">The number of contacts in the array</param>
		/// <param name="deltaTime">Its DeltaTime</param>
		void ResolveContacts(cParticleContact* contactArray, size_t numContacts, float deltaTime);
	};
}