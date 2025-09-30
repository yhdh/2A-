/*
 * MatlabCheck.h
 *
 *  Created on: 9 févr. 2017
 *      Author: davidroussel
 */

#ifndef MATLABCHECK_H_
#define MATLABCHECK_H_

#include "checkers/ExternalCheck.h"

/**
 * Matlab external checker
 * @tparam iterator le type d'itérateur à utiliser pour parcourir les valeurs
 */
template <class iterator>
class MatlabCheck : public ExternalCheck<iterator>
{
	/*
	 * Reminder of ExternalCheck elements reused here
	 */
	using typename ExternalCheck<iterator>::value_type;
	using ExternalCheck<iterator>::tool;
	using ExternalCheck<iterator>::availability;
	using ExternalCheck<iterator>::command;

	public:
		/**
		 * Protected constructor for Matlab checker
		 * @param baseDir the base directory to run the external tool from
		 * @param begin an iterator to the first value of the tested sequence
		 * @param end an iterator past the last value of the tested sequence
		 * @param minValue minimum possible value in the sequence
		 * @param maxValue maximum possible value in the sequence
		 * @param meanValue computed mean value of the sequence
		 * @param stdValue computed std value of the sequence
		 * @param medianValue computed median value in the sequence
		 */
		MatlabCheck(const string & baseDir,
					const iterator & begin,
					const iterator & end,
					const value_type & minValue,
					const value_type & maxValue,
					const double & meanValue,
					const double & stdValue,
					const double & medianValue);
};

#endif /* MATLABCHECK_H_ */
