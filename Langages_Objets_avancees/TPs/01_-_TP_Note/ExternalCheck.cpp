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
#include <array>	// for array
#include <memory>	// for shared pointer

#include "ExternalCheck.h"

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
 */
template<class Iterator, class T>
ExternalCheck<Iterator, T>::ExternalCheck(const string & tool,
                                          const string &,
                    		              const Iterator &,
                    		              const Iterator &,
                    		              const T &,
                    		              const T &,
                    		              const double &,
                    		              const double &,
                    		              const double &) :
	tool(tool),
	availability(false)
{
	// Check if this tool is available on the path
	ostringstream oss;
	oss << "command -v " << tool;

	const int bufSize = 128;
	char buffer[bufSize];
	ostringstream resultStream;
	const char * testcommand = oss.str().c_str();

	FILE * pipe = popen(testcommand, "r");
	if (pipe)
	{
		while(fgets(buffer, bufSize, pipe) != nullptr)
		{
			resultStream << buffer;
		}

		if (!resultStream.str().empty())
		{
			availability = true;
		}
		else
		{
			cerr << "command " << tool << " is not available" << endl;
		}

		pclose(pipe);
	}
	else
	{
		cerr << "popen(" << testcommand << ") failed" << endl;
	}
}

/*
 * Destructor
 */
template<class Iterator, class T>
ExternalCheck<Iterator, T>::~ExternalCheck()
{
	command.clear();
}

/*
 * Check if the required tool is available
 * @return true if the required tool has been found in the path,
 * false otherwise.
 */
template<class Iterator, class T>
bool ExternalCheck<Iterator, T>::available()
{
	return availability;
}

/*
 * Executes the command in a new shell and return value
 * @return the return value of the system call with "command" argument
 * @pre The tool is supposed to be available before launching the run
 */
template<class Iterator, class T>
int ExternalCheck<Iterator, T>::run()
{
	const char * ccommand = command.c_str();

	int ret = 0;
	if (system(NULL))
	{
		ret = system(ccommand);
	}
	else
	{
		cerr << "Could not find command interpreter on your system, sorry"
		     << endl;
	}
	return ret;
}

// ----------------------------------------------------------------------------
// Proto instanciations
// ----------------------------------------------------------------------------
#include <vector>

/*
 * Instanciation du template ExternalCheck avec des <vector::const_iterator, int>
 */
template class ExternalCheck<std::vector<int>::const_iterator, int>;

/*
 * Instanciation du template ExternalCheck avec des <vector::const_iterator, float>
 */
template class ExternalCheck<std::vector<float>::const_iterator, float>;

/*
 * Instanciation du template ExternalCheck avec des <vector::const_iterator, float>
 */
template class ExternalCheck<std::vector<double>::const_iterator, double>;
