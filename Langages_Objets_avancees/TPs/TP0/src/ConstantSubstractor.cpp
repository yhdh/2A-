/*
 * ConstantSubstractor.cpp
 *
 *  Created on: Feb 4, 2020
 *      Author: davidroussel
 */

#include <type_traits>
#include <exception>

#include "ConstantSubstractor.h"

/*
 * Constructor
 * @param value the constant value to subtract
 * @throw exception if T is not an arithmetic type
 */
template<class T>
ConstantSubstractor<T>::ConstantSubstractor(const T & value) noexcept(false) :
	subtractValue(value)
{
	if (!std::is_arithmetic<T>::value)
	{
		throw std::exception();
	}
}

/*
 * Function call operator
 * @param value the value to apply op to
 * @return the provided value - the constant value
 */
template<class T>
T ConstantSubstractor<T>::operator ()(const T & value) const
{
	return value - subtractValue;
}

// -----------------------------------------------------------------------------
// Proto instanciations
// -----------------------------------------------------------------------------
template class ConstantSubstractor<double>;
