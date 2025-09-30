/*
 * RCheck.cpp
 *
 *  Created on: 9 févr. 2017
 *      Author: davidroussel
 */

#include <sstream>	// For ostringstream
#include <algorithm>	// For copy algorihtm

#include "RCheck.h"

/*
 * Constructor for R checker
 * @param baseDir [not used] the base directory to run the external tool from
 * @param begin an iterator to the first value of the tested sequence
 * @param end an iterator past the last value of the tested sequence
 * @param minValue minimum possible value in the sequence
 * @param maxValue maximum possible value in the sequence
 * @param meanValue computed mean value of the sequence
 * @param stdValue computed std value of the sequence
 * @param medianValue computed median value in the sequence
 */
template<class Iterator, class T>
RCheck<Iterator, T>::RCheck(const string &,
                            const Iterator & begin,
                            const Iterator & end,
                            const T & minValue,
                            const T & maxValue,
                            const double & meanValue,
                            const double & stdValue,
                            const double & medianValue) :
	ExternalCheck<Iterator, T>("Rscript", "", begin, end, minValue, maxValue,
	                           meanValue, stdValue, medianValue)
{

	ostringstream commandStream;

	commandStream << tool;
	// commandStream << "-e 'frame = read.csv(\"test.csv\", header = TRUE, sep = \";\")'";
	commandStream << " -e 'values = c(";
	diff_t limit = (end - begin);
	diff_t i;
	Iterator it;
	for (i = 0, it = begin; i < limit && it != end; ++i, ++it)
	{
		commandStream << *it;
		if (i < (limit - 1))
		{
			commandStream << ", ";
		}
	}
	commandStream << ")'";
	commandStream << " -e 'minValue = " << minValue << "'";
	commandStream << " -e 'maxValue = " << maxValue << "'";
	commandStream << " -e 'meanValue = " << meanValue << "'";
	commandStream << " -e 'stdValue = " << stdValue << "'";
	commandStream << " -e 'medianValue = " << medianValue << "'";
	commandStream << " -e 'source(\"check_TP_STL.R\")'";
	commandStream
	    << " -e 'checkvalues(values, minValue, maxValue, meanValue, stdValue, medianValue)'";

	command = commandStream.str();
}

#include <vector>

/*
 * Instanciation du template ExternalCheck avec des <vector::const_iterator, int>
 */
template class RCheck<std::vector<int>::const_iterator, int> ;

/*
 * Instanciation du template ExternalCheck avec des <vector::const_iterator, float>
 */
template class RCheck<std::vector<float>::const_iterator, float> ;

/*
 * Instanciation du template ExternalCheck avec des <vector::const_iterator, float>
 */
template class RCheck<std::vector<double>::const_iterator, double> ;
