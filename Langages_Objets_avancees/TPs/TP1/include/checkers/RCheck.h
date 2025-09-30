/*
 * RCheck.h
 *
 *  Created on: 9 févr. 2017
 *      Author: davidroussel
 */

#ifndef RCHECK_H_
#define RCHECK_H_

#include "checkers/ExternalCheck.h"

/**
 * R external checker
 * @tparam iterator le type d'itérateur à utiliser pour parcourir les valeurs
 */
template <class Iterator>
class RCheck : public ExternalCheck<Iterator>
{
	/*
	 * Reminder of ExternalCheck elements reused here
	 */
	using typename ExternalCheck<Iterator>::value_type;
	using typename ExternalCheck<Iterator>::difference_type;
	using ExternalCheck<Iterator>::tool;
	using ExternalCheck<Iterator>::availability;
	using ExternalCheck<Iterator>::command;

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
		 * @param useCSV use intermediate CSV file to save values before
		 * checking (the check tool will then need to load this file).
		 */
		RCheck(const string & baseDir,
			   const Iterator & begin,
			   const Iterator & end,
			   const value_type & minValue,
			   const value_type & maxValue,
			   const double & meanValue,
			   const double & stdValue,
			   const double & medianValue,
			   const bool useCSV = false);
};

#endif /* RCHECK_H_ */
