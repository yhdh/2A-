/*
 * ExternalCheck.cpp
 *
 *  Created on: 9 févr. 2017
 *      Author: davidroussel
 */

//#include <cstdlib>	// for system command
#include <cstdio>	// for popen
#include <iostream>	// for cerr
#include <sstream>	// for ostringstream
#include <fstream>	// for ofstream
#include <iterator>	// for ostream_iterator

#include "checkers/ExternalCheck.h"

/*
 * Protected constructor for external checker
 * @param tool the tool to launch for this checker
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
template<class iterator>
ExternalCheck<iterator>::ExternalCheck(const string & tool,
									   const string &,
									   const iterator & begin,
									   const iterator & end,
									   const value_type &,
									   const value_type &,
									   const double &,
									   const double &,
									   const double &,
									   const bool useCSV) :
	tool(tool),
	availability(false),
	useCSV(useCSV)
{
	// Check if this tool is available on the path
	ostringstream oss;
	oss << "command -v " << tool;

	const int bufSize = 128;
	char buffer[bufSize];
	ostringstream resultStream;
	string oString = oss.str();
	const char * testCommand = oString.c_str();

	FILE * pipe = popen(testCommand, "r");
	if (pipe)
	{
		while(fgets(buffer, bufSize, pipe) != nullptr)
		{
			resultStream << buffer;
		}

		if (!resultStream.str().empty())
		{
			availability = true;
			if (this->useCSV)
			{
				saveToCSV(begin, end);
			}
		}
//		else
//		{
//			cerr << "command " << tool << " is not available" << endl;
//		}

		pclose(pipe);
	}
	else
	{
		cerr << __func__ << ".popen(" << testCommand << ") failed" << endl;
	}
}

/*
 * Destructor
 */
template<class iterator>
ExternalCheck<iterator>::~ExternalCheck()
{
	command.clear();
}

/*
 * Save values contained in [first, last) into a CSV file
 * @param first the iterator pointing to the first element
 * @param last the iterator pointing past the last element
 * @param fileName the filename to save in
 * @param baseDir the base directory ot the file to save in
 * @param sep the separator to use in the csv file
 */
template<class iterator>
void ExternalCheck<iterator>::saveToCSV(const iterator & first,
										const iterator & last,
										const string & fileName,
										const string & baseDir,
										const string & sep)
{
	string cfileName = baseDir + fileName;

	ofstream ofs(cfileName, ofstream::out);
	if (ofs.is_open())
	{
		ofs << "Values"<< sep;
		/*
		 * Note : copy algorithm does not work here because of the extra
		 * separator at the end
		 */
//		copy(first, last, ostream_iterator<value_t>(ofs, sep.c_str()));
		difference_type nbElements = last - first;
		difference_type i;
		iterator it;
		for (i = 0, it = first; i < nbElements && it != last; ++i, ++it)
		{
			ofs << *it;
			if (i < (nbElements - 1))
			{
				ofs << sep;
			}
		}
		ofs << endl;
		ofs.close();
	}
	else
	{
		cerr << __func__ << "(" << cfileName << ") : Unable to open file" << endl;
	}

}

/*
 * Check if the required tool is available
 * @return true if the required tool has been found in the path,
 * false otherwise.
 */
template<class iterator>
bool ExternalCheck<iterator>::available()
{
	return availability;
}

/*
 * Executes the command in a new shell and return value
 * @return the return value of the system call with "command" argument
 * @pre The tool is supposed to be available before launching the run
 */
template<class iterator>
int ExternalCheck<iterator>::run()
{
	const char * cCommand = command.c_str();

//	cout << "Check::run(" << command << ")" << endl;

	int ret = 0;
	if (system(nullptr))
	{
		ret = system(cCommand);
	}
	else
	{
		cerr << __func__
			 << " Could not find command interpreter on your system, sorry" << endl;
	}
	return ret;
}

// ----------------------------------------------------------------------------
// Proto instantiation of ExternalCheck
// WARNING : explicit template instantiation "template class ExternalCheck<xx>;"
// should appear in one place and one place only in order to avoid
// emitting multiple vtable in multiple translation units.
// All other references to explicit template instantiation should be done
// through "extern template class ExternalCheck<xx>;"
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
template class ExternalCheck<std::vector<int>::const_iterator>;
template class ExternalCheck<std::deque<int>::const_iterator>;

/*
 * Instanciation du template ExternalCheck avec des <vector::const_iterator>
 */
template class ExternalCheck<std::vector<float>::const_iterator>;
template class ExternalCheck<std::deque<float>::const_iterator>;

/*
 * Instanciation du template ExternalCheck avec des <vector::const_iterator>
 */
template class ExternalCheck<std::vector<double>::const_iterator>;
template class ExternalCheck<std::deque<double>::const_iterator>;

#ifdef __clang__
#pragma clang diagnostic pop
#endif
