/*
 * MatlabCheck.cpp
 *
 *  Created on: 9 févr. 2017
 *      Author: davidroussel
 */

#include <sstream>		// For ostringstream
#include <algorithm>	// For copy algorihtm
#include <iterator>		// For ostream_iterator

#include "checkers/MatlabCheck.h"

/*
 * Protected constructor for external checker
 * @param baseDir the base directory to run the external tool from
 * @param begin an iterator to the first value of the tested sequence
 * @param end an iterator past the last value of the tested sequence
 * @param minValue minimum possible value in the sequence
 * @param maxValue maximum possible value in the sequence
 * @param meanValue computed mean value of the sequence
 * @param stdValue computed std value of the sequence
 * @param medianValue computed median value in the sequence
 */
template<class iterator>
MatlabCheck<iterator>::MatlabCheck(const string & baseDir,
								   const iterator & begin,
								   const iterator & end,
								   const value_type & minValue,
								   const value_type & maxValue,
								   const double & meanValue,
								   const double & stdValue,
								   const double & medianValue) :
	ExternalCheck<iterator>("matlab",
							baseDir,
							begin,
							end,
							minValue,
							maxValue,
							meanValue,
							stdValue,
							medianValue)
{
	ostringstream commandStream;
	commandStream << tool;
//	commandStream << " -nodisplay -nojvm -nosplash -r \"";
	commandStream << " -batch \"";
	commandStream << "cd " << baseDir << "; ";
	commandStream << "check_TP_STL([";

	copy(begin, end, ostream_iterator<value_type>(commandStream, " "));

	commandStream << "], " << minValue << ", " << maxValue << ", "
				  << meanValue << ", " << stdValue << ", " << medianValue
				  << "); exit;\"";
	commandStream << " | tail -n 6"; // avoid matlab ascii splash art

	command = commandStream.str();
}

// ----------------------------------------------------------------------------
// Proto instantiation of MatlabCheck
// WARNING : explicit template instantiation "template class MatlabCheck<xx>;"
// should appear in one place and one place only in order to avoid
// emitting multiple vtable in multiple translation units.
// All other references to explicit template instantiation should be done
// through "extern template class MatlabCheck<xx>;"
// ----------------------------------------------------------------------------

#include <vector>
#include <deque>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-template-vtables"
#endif

/*
 * Instanciation du template MatlabCheck avec des <vector::const_iterator>
 */
extern template class ExternalCheck<std::vector<int>::const_iterator>;
template class MatlabCheck<std::vector<int>::const_iterator>;
extern template class ExternalCheck<std::deque<int>::const_iterator>;
template class MatlabCheck<std::deque<int>::const_iterator>;

/*
 * Instanciation du template MatlabCheck avec des <vector::const_iterator>
 */
extern template class ExternalCheck<std::vector<float>::const_iterator>;
template class MatlabCheck<std::vector<float>::const_iterator>;
extern template class ExternalCheck<std::deque<float>::const_iterator>;
template class MatlabCheck<std::deque<float>::const_iterator>;

/*
 * Instanciation du template MatlabCheck avec des <vector::const_iterator>
 */
extern template class ExternalCheck<std::vector<double>::const_iterator>;
template class MatlabCheck<std::vector<double>::const_iterator>;
extern template class ExternalCheck<std::deque<double>::const_iterator>;
template class MatlabCheck<std::deque<double>::const_iterator>;

#ifdef __clang__
#pragma clang diagnostic pop
#endif
