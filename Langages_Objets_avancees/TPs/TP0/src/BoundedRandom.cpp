/*
 * BoundedRandom.cpp
 *
 *  Created on: Feb 4, 2020
 *      Author: davidroussel
 */

#include <random>		// for std::random_device
#include <iostream>		// for I/O

#include "BoundedRandom.h"

/*
 * Constructor
 * @param min the minimum possible random value
 * @param max the maximum possible random value
 */
template <class Arithmetic>
BoundedRandom<Arithmetic>::BoundedRandom(const Arithmetic & min, const Arithmetic & max)
{
	const std::type_info & ti = typeid(Arithmetic);

		std::random_device d; // random seed
		std::default_random_engine engine(d());

		/*
		 * The following binds a uniform distribution's () operator with the
		 * engine created above into a random function template : Later call to
		 * this function template will trigger the uniform distribution's ()
		 * operator with the provided engine which will return a new random
		 * number enclosed in [min, max] range.
		 */
	if (std::is_same<Arithmetic, int>::value)
	{
		random = bind(std::uniform_int_distribution<int>(min, max), engine);
	}
	else if (std::is_same<Arithmetic, float>::value)
	{
		random = bind(std::uniform_real_distribution<float>(min, max), engine);
	}
	else if (std::is_same<Arithmetic, double>::value)
	{
		random = bind(std::uniform_real_distribution<double>(min, max), engine);
	}
	else
	{
		std::cerr << __func__ << " : Unknown value type " << ti.name()
				  << std::endl;
	}
}

/*
 * Function call operator
 * @return a new random value within [min..max]
 */
template <class Arithmetic>
Arithmetic BoundedRandom<Arithmetic>::operator()()
{
	return random();
}

// -----------------------------------------------------------------------------
// Proto instanciations
// -----------------------------------------------------------------------------
template class BoundedRandom<int>;
template class BoundedRandom<double>;
