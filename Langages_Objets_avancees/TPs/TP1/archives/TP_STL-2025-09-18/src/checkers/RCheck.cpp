/*
 * RCheck.cpp
 *
 *  Created on: 9 févr. 2017
 *      Author: davidroussel
 */

#include <sstream>	// For ostringstream
#include <algorithm>	// For copy algorihtm

#include "checkers/RCheck.h"

/*
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
template<class Iterator>
RCheck<Iterator>::RCheck(const string &,
						 const Iterator & begin,
						 const Iterator & end,
						 const value_type & minValue,
						 const value_type & maxValue,
						 const double & meanValue,
						 const double & stdValue,
						 const double & medianValue,
						 const bool useCSV) :
	ExternalCheck<Iterator>("Rscript",
							"",
							begin,
							end,
							minValue,
							maxValue,
							meanValue,
							stdValue,
							medianValue,
							useCSV)
{

	ostringstream commandStream;

	commandStream << tool;
	if (useCSV)
	{
		commandStream << " -e 'frame = read.csv(\"test.csv\", header = TRUE, sep = \";\")'";
		commandStream << " -e 'values = frame$Values'";
	}
	else
	{
		commandStream << " -e 'values = c(";
		difference_type limit = (end - begin);
		difference_type i;
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
	}
	commandStream << " -e 'minValue = " << minValue << "'";
	commandStream << " -e 'maxValue = " << maxValue << "'";
	commandStream << " -e 'meanValue = " << meanValue << "'";
	commandStream << " -e 'stdValue = " << stdValue << "'";
	commandStream << " -e 'medianValue = " << medianValue << "'";
	commandStream << " -e 'source(\"check_TP_STL.R\")'";
	commandStream << " -e 'checkvalues(values, minValue, maxValue, meanValue, stdValue, medianValue)'";
	command = commandStream.str();
}

// ----------------------------------------------------------------------------
// Proto instantiation of RCheck
// WARNING : explicit template instantiation "template class RCheck<xx>;"
// should appear in one place and one place only in order to avoid
// emitting multiple vtable in multiple translation units.
// All other references to explicit template instantiation should be done
// through "extern template class RCheck<xx>;"
// ----------------------------------------------------------------------------

#include <vector>
#include <deque>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-template-vtables"
#endif

/*
 * Instanciation du template ExternalCheck avec des <vector::const_iterator>
 */
extern template class ExternalCheck<std::vector<int>::const_iterator>;
template class RCheck<std::vector<int>::const_iterator>;
extern template class ExternalCheck<std::deque<int>::const_iterator>;
template class RCheck<std::deque<int>::const_iterator>;

/*
 * Instanciation du template ExternalCheck avec des <vector::const_iterator>
 */
extern template class ExternalCheck<std::vector<float>::const_iterator>;
template class RCheck<std::vector<float>::const_iterator>;
extern template class ExternalCheck<std::deque<float>::const_iterator>;
template class RCheck<std::deque<float>::const_iterator>;

/*
 * Instanciation du template ExternalCheck avec des <vector::const_iterator>
 */
extern template class ExternalCheck<std::vector<double>::const_iterator>;
template class RCheck<std::vector<double>::const_iterator>;
extern template class ExternalCheck<std::deque<double>::const_iterator>;
template class RCheck<std::deque<double>::const_iterator>;

#ifdef __clang__
#pragma clang diagnostic pop
#endif
