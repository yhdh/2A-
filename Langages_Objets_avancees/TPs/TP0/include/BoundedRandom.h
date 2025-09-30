/*
 * BoundedRandom.h
 *
 *  Created on: Feb 4, 2020
 *      Author: davidroussel
 */

#ifndef BOUNDEDRANDOM_H_
#define BOUNDEDRANDOM_H_

#include <functional>	// for std::function
#include <type_traits>	// for type checking & is_arithmetic...

/**
 * Imposes that T is an arithmetic type
 */
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

/**
 * Bounded Random generator template using:
 * 	- The default random engine bound to
 * 	- A uniform distribution of type Arithmetic
 * @tparam Arithmetic an arithmetic type (int, float, double, ...)
 */
template<class Arithmetic>
class BoundedRandom
{
	private:
		/**
		 * Function resulting of bounding random engine to distribution
		 */
		std::function<Arithmetic(void)> random;

	public:
		/**
		 * Constructor
		 * @param min the minimum possible random value
		 * @param max the maximum possible random value
		 */
		BoundedRandom(const Arithmetic & min, const Arithmetic & max);

		/**
		 * Function call operator
		 * @return a new random value within [min..max]
		 */
		Arithmetic operator()();
};

#endif /* BOUNDEDRANDOM_H_ */
