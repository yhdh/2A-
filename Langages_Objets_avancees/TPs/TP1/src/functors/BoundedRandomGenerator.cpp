/*
 * BoundedRandomGenerator.cpp
 *
 *  Created on: 21 févr. 2013
 *      Author: davidroussel
 */
#include "functors/BoundedRandomGenerator.h"
#include <cstdlib>	// for random & srandom (see man 3 random)
#include <ctime>	// for time

// ----------------------------------------------------------------------------
// BoundedRandomGenerator implementation
// ----------------------------------------------------------------------------

/*
 * Constructeur d'un Générateur de nombres aléatoires borné
 * @param min la valeur minimale
 * @param max la valeur maximale
 */
template <typename Arithmetic>
BoundedRandomGenerator<Arithmetic>::BoundedRandomGenerator(const Arithmetic & min, const Arithmetic & max)
	// DONE 02-c Liste d'initialisation
	: min(min), max(max)
{
	//  02-d initialisation du random generator
	static bool seeded = false;
    if (!seeded) {
        srandom(static_cast<unsigned>(time(nullptr)));
        seeded = true;
	}
}

/*
 * Opérateur d'appel de fonction
 * @return un nouveau nombre aléatoire compris entre min et max
 */
template <typename Arithmetic>
Arithmetic BoundedRandomGenerator<Arithmetic>::operator()()
{
	//DONE 02-e renvoyer une valeur aléatoire entre min et max
	// Cas trivial
	if (this->min == this->max)
	{
		return this->min;
	}

	if constexpr (std::is_integral_v<Arithmetic>)
	{
		// Intervalle inclusif
		using ULL = unsigned long long;
		const ULL lo = static_cast<ULL>(this->min);
		const ULL hi = static_cast<ULL>(this->max);
		const ULL range = hi - lo;          // >= 1
		const ULL pick  = static_cast<ULL>(std::rand()) % (range + 1ULL);
		return static_cast<Arithmetic>(lo + pick);
	}
	else if constexpr (std::is_floating_point_v<Arithmetic>)
	{
		// Uniforme continu [min, max], bornes inclusives en pratique :
		// rand() ∈ [0, RAND_MAX], donc u ∈ [0,1], u==0 -> min, u==1 -> max
		const double u = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
		const double val = static_cast<double>(this->min)
		                 + (static_cast<double>(this->max) - static_cast<double>(this->min)) * u;
		return static_cast<Arithmetic>(val);
	}
	else
	{
		// Type non arithmétique : comportement indéfini dans le cadre du TP,
		// on renvoie min par défaut.
		return this->min;
	}
	return Arithmetic(0);
}

// ----------------------------------------------------------------------------
// Proto instanciations
// ----------------------------------------------------------------------------
/*
 * Instanciation du template BoundedRandomGenerator avec des <int>
 */
template class BoundedRandomGenerator<int>;

/*
 * Instanciation du template BoundedRandomGenerator avec des <float>
 */
template class BoundedRandomGenerator<float>;

/*
 * Instanciation du template BoundedRandomGenerator avec des <double>
 */
template class BoundedRandomGenerator<double>;
