/*
 * ConstantSubstractor.h
 *
 *  Created on: Feb 4, 2020
 *      Author: davidroussel
 */

#ifndef CONSTANTSUBSTRACTOR_H_
#define CONSTANTSUBSTRACTOR_H_

#include <functional>

/**
 * Unary function to subtract a constant value to all values provided
 * through the function call operator
 * @warning unary_function<T, T> was obsolete in C++11  and
 * has been removed in C++17
 */
template<class T>
class ConstantSubstractor // : public unary_function<T, T>
{
	private:
		/**
		 * The value to subtract
		 */
		T subtractValue;

	public:
		/**
		 * Constructor
		 * @param value the constant value to subtract
		 * @throw exception if T is not an arithmetic type
		 */
		ConstantSubstractor(const T & value) noexcept(false);

		/**
		 * Function call operator
		 * @param value the value to apply op to
		 * @return the provided value - the constant value
		 */
		T operator ()(const T & value) const;
};

#endif /* CONSTANTSUBSTRACTOR_H_ */
