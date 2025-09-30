/*
 * IncrementalMeanOperator.h
 *
 *  Created on: Feb 4, 2020
 *      Author: davidroussel
 */

#ifndef INCREMENTALMEANOPERATOR_H_
#define INCREMENTALMEANOPERATOR_H_

#include <cstddef>	// for size_t

/**
 * A binary functor to compute  mean values incrementally
 */
template <class T> class IncrementalMeanOperator
{
	private:
		size_t count;

	public:
		/**
		 * Constructor
		 * @throws exception if T is not an arithmetic type
		 */
		IncrementalMeanOperator() noexcept(false);

		/**
		 * Function call operator computing the next mean value
		 * based on previous mean value an new value in the sequence.
		 * E(X_n+1) = (n*E(X_n) + x_n+1) / (n+1)
		 * @param currentMean The current mean value
		 * @param newValue New value to add to the sequence
		 * @return the new mean value based on previous mean value,
		 * counted values and new value added to the sequence
		 * @note This call operator should be a Binary operator with the
		 * following prototype: <new mean> op(<previous mean>, <new value>),
		 * since this operator is bound to be used in a std::transform
		 * algorithm which requires a binary operator.
		 */
		T operator ()(const T & currentMean, const T & newValue);
};

#endif /* INCREMENTALMEANOPERATOR_H_ */
