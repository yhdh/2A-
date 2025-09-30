/*
 * ExternalCheck.h
 *
 *  Created on: 9 févr. 2017
 *      Author: davidroussel
 */

#ifndef EXTERNALCHECK_H_
#define EXTERNALCHECK_H_

#include <string>

using namespace std;

/**
 * Check values provided for mean, std and median using external math tools
 * such as matlab or R
 */
template<class Iterator, class T>
class ExternalCheck
{
	protected:
		/**
		 * Internal Iterator's value type
		 */
		using value_t = typename iterator_traits<Iterator>::value_type;
		using diff_t = typename iterator_traits<Iterator>::difference_type;

		/**
		 *  The tool to launch for this checker
		 */
		string tool;

		/**
		 * Check the availability to the required tool
		 */
		bool availability;

		/**
		 * The command string to be sent to the external tool
		 */
		string command;

		/**
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
		ExternalCheck(const string & tool,
		              const string & baseDir,
		              const Iterator & begin,
		              const Iterator & end,
		              const T & minValue,
		              const T & maxValue,
		              const double & meanValue,
		              const double & stdValue,
		              const double & medianValue);
	public:
		/**
		 * Destructor
		 */
		virtual ~ExternalCheck();

		/**
		 * Check if the required tool is available
		 * @return true if the required tool has been found in the path,
		 * false otherwise.
		 */
		bool available();

		/**
		 * Executes the command in a new shell and return value
		 * @return the return value of the system call with "command" argument
		 * @pre The tool is supposed to be available before launching the run
		 */
		int run();
};

#endif /* EXTERNALCHECK_H_ */
