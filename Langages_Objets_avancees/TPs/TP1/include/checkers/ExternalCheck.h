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
 * @tparam iterator le type d'itérateur à utiliser pour parcourir les valeurs
 */
template<class iterator>
class ExternalCheck
{
	protected:
		/**
		 * Internal Iterator's value type
		 */
		using value_type = typename iterator_traits<iterator>::value_type;
		/**
		 * Internal Iterator's difference type
		 */
		using difference_type = typename iterator_traits<iterator>::difference_type;

		/**
		 *  The tool to launch for this checker
		 */
		string tool;

		/**
		 * Check the availability to the required tool
		 */
		bool availability;

		/**
		 * Use CSV file (test.csv) to store values before checking
		 */
		bool useCSV;

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
		 * @param useCSV use intermediate CSV file to save values before
		 * checking (the check tool will then need to load this file).
		 */
		ExternalCheck(const string & tool,
					  const string & baseDir,
					  const iterator & begin,
					  const iterator & end,
					  const value_type & minValue,
					  const value_type & maxValue,
					  const double & meanValue,
					  const double & stdValue,
					  const double & medianValue,
					  const bool useSCV = false);

		/**
		 * Save values contained in [first, last) into a CSV file
		 * @param first the iterator pointing to the first element
		 * @param last the iterator pointing past the last element
		 * @param fileName the filename to save in [default = test.csv]
		 * @param baseDir the base directory ot the file to save in [default = ""]
		 * @param sep the separator to use in the csv file [default = ";\n"]
		 */
		void saveToCSV(const iterator & first,
					   const iterator & last,
					   const string & fileName = "test.csv",
					   const string & baseDir = "",
					   const string & sep = ";\n");
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
