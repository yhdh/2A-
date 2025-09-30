/*
 * StatFunctor.cpp
 *
 *  Created on: 21 févr. 2013
 *      Author: davidroussel
 */

#include "StatFunctor.h"

#include <cmath>	// for sqrt

/*
 * TODO implémentation des méthodes
 * template <class T>
 * retour StatFunctor<T>::méthode(args...)
 * {
 * 	...
 * }
 * N'oubliez pas les const à chaque fois que nécessaire dans
 * 	- les arguments
 * 	- les types de retour
 * 	- le qualificateur de la méthode
 */

// ----------------------------------------------------------------------------
// Proto instanciations
// ----------------------------------------------------------------------------
/*
 * Instanciation du template StatFunctor avec des <int>
 */
template class StatFunctor<int>;

/*
 * Instanciation du template StatFunctor avec des <float>
 */
template class StatFunctor<float>;

/*
 * Instanciation du template StatFunctor avec des <double>
 */
template class StatFunctor<double>;
