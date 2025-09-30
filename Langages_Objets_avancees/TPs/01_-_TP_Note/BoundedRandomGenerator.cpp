/*
 * BoundedRandomGenerator.cpp
 *
 *  Created on: 21 févr. 2013
 *      Author: davidroussel
 */

#include "BoundedRandomGenerator.h"
#include <cstdlib>	// for random & srandom
#include <ctime>	// for time

/*
 * TODO implémentation des méthodes
 * template <class T>
 * retour BoundedRandomGenerator<T>::méthode(args...)
 * {
 * 	...
 * }
 * N'oubliez pas les const à chaque fois que nécessaire dans
 * 	- les arguments : maméthode(const Type & v)
 * 	- les types de retour : const Type maméthode(...)
 * 	- le qualificateur de la méthode : maméthode(...) const
 */

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
