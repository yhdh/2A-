/*
 * RCheck.h
 *
 *  Created on: 9 févr. 2017
 *      Author: davidroussel
 */

#ifndef RCHECK_H_
#define RCHECK_H_

#include "ExternalCheck.h"

/**
 * R external checker
 */
template <class Iterator, class T>
class RCheck : public ExternalCheck<Iterator, T>
{
	/*
	 * Reminder of ExternalCheck elements reused here
	 */
	// using typename ExternalCheck<Iterator, T>::value_t;
	using typename ExternalCheck<Iterator, T>::diff_t;
	using ExternalCheck<Iterator, T>::tool;
	using ExternalCheck<Iterator, T>::availability;
	using ExternalCheck<Iterator, T>::command;

	public:
		/**
		 * Protected constructor for R checker
		 * @param baseDir the base directory to run the external tool from
		 * @param begin an iterator to the first value of the tested sequence
		 * @param end an iterator past the last value of the tested sequence
		 * @param minValue minimum possible value in the sequence
		 * @param maxValue maximum possible value in the sequence
		 * @param meanValue computed mean value of the sequence
		 * @param stdValue computed std value of the sequence
		 * @param medianValue computed median value in the sequence
		 */
		RCheck(const string & baseDir,
		       const Iterator & begin,
		       const Iterator & end,
		       const T & minValue,
		       const T & maxValue,
		       const double & meanValue,
		       const double & stdValue,
		       const double & medianValue);
};

#endif /* RCHECK_H_ */
