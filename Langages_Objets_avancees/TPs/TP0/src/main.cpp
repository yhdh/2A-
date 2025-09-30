/*
 * classroom_example.cpp
 *
 *  Created on: 20 févr. 2013
 *  Updated on: 05 sep. 2025
 *      Author: davidroussel
 */

#include <cstdlib>		// for EXIT_SUCCESS
#include <algorithm>	// for all standard algos
#include <vector>
#include <list>
#include <iostream>		// for cout
#include <iterator>		// for ostream_iterator, and iterator_traits
#include <functional>	// for all functors such as greater<...>, minus<...> etc
#include <random>		// for decent random engine
#include <utility>		// for std::pair
#include <cmath>		// for fabs
#include <cassert>		// for assertions
#include <concepts>		// for C++20 concepts
#include <numeric>		// for accumulate
using namespace std;

#include "BoundedRandom.h"
extern template class BoundedRandom<int>;
extern template class BoundedRandom<double>;

#include "ConstantSubstractor.h"
extern template class ConstantSubstractor<double>;

#include "IncrementalMeanOperator.h"
extern template class IncrementalMeanOperator<double>;
/**
 * DONE 03 Concept defining forward iterator over arithmetic types
 * @tparam T the iterator type to check
 */
template <typename T> concept NumberIterator =
	forward_iterator<T> &&
	is_arithmetic_v<typename iterator_traits<T>::value_type>;

/**
 * Greater difference of values in an iterator range.
 * @par Algorithm
 * 1. Find the positions of the min & max elements in the range
 * 2. Return the difference max - min
 * @tparam NumberIterator a forward iterator over an arithmetic type
 * @param first iterator to the first elt in the iterator range
 * @param last iterator past the last element in the iterator range
 * @return the biggest difference in elements within [first, last)
 */
template <class NumberIterator>
const iterator_traits<NumberIterator>::value_type
greater_diff(const NumberIterator & first, const NumberIterator & last)
{
	// DONE 01 Find positions of min & max elements
	const auto minmaxPair = minmax_element(first, last);
	const auto & minIt = minmaxPair.first;
	const auto & maxIt = minmaxPair.second;
	// DONE 02 Then computes and return max - min
	return *maxIt - *minIt;
}

/**
 * "Convertible To" concept defining types that can be safely casted from one
 * type to another.
 * @tparam From the source type to convert from
 * @tparam To the destination type to convert to
 * @see convertible_to
 */
template <typename From, typename To>
concept ConvertibleTo =
	convertible_to<From, To> &&
	requires(From (&f)())
	{
    	static_cast<To>(f());
	};

/**
 * "Ordered" concept defining types that can be compared with relational
 * operators "<", ">", "<=" & ">=" with a boolean result.
 * @tparam T the type that should be ordered
 */
template <typename T>
concept LessThanComparable =
	requires(T a, T b)
	{
        { a < b } -> ConvertibleTo<bool>;
        { a <= b } -> ConvertibleTo<bool>;
        { a > b } -> ConvertibleTo<bool>;
        { a >= b } -> ConvertibleTo<bool>;
	};

/**
 * Comparator using only absolute values
 * @param value1 the first value to compare
 * @param value2 the second value to compare
 * @return true if fabs(value1) > fabs(value2), false otherwise
 */
template <typename LessThanComparable>
bool greater_mag(const LessThanComparable & value1,
				const LessThanComparable & value2)
{
	return fabs(value1) > fabs(value2);
}

int main()
{
	size_t nbElts = 10;
	vector<int> vi(nbElts);

	// Output iterator to display ints on standard output stream
	ostream_iterator<int> ioutputter(cout, ", ");
	// Output iterator to display doubles on standard output stream
	ostream_iterator<double> doutputter(cout, ", ");

	// DONE 04 : Fill vi with random numbers between 0 and 5 using BoundedRandom<int> and standard algorithm
	int imin = 0;
	int imax = 5;
	BoundedRandom<int> intRandom(imin, imax);
	generate(vi.begin(), vi.end(), intRandom);

	/*
	 * Assert values are within bounds
	 */
	for_each(vi.cbegin(), vi.cend(), [&imin, &imax](const int val)->void{
		assert(val >= imin);
		assert(val <= imax);
	});

	// DONE 05 : copy vi elements to standard output
	cout << "vector of " << vi.size() << " random numbers between ["
		 << imin << "..." << imax << "] : ";
	copy(vi.cbegin(), vi.cend(), ioutputter);
	cout << endl;

	// DONE 06 : Compute greater difference between vi elts with greater_diff
	int gdI = 0;
	gdI = greater_diff(vi.cbegin(), vi.cend());
	cout << "greater diff is : " << gdI << endl;

	// ========================================================================
	vector<double> vd(nbElts);

	// DONE 07 : Fill vd elements with random numbers between -25.7 and 45.5
	double dmin = -25.7;
	double dmax = 45.5;
	double maxGreaterDiff = dmax - dmin;
	double tolerance = 1e-14;
	BoundedRandom<double> doubleRandom(dmin, dmax);
	generate(vd.begin(), vd.end(), doubleRandom);

	/*
	 * Assert values are within bounds
	 */
	for_each(vd.cbegin(),
			 vd.cend(),
			 [&dmin, &dmax](const double val) -> void
			 {
		assert(val >= dmin);
		assert(val <= dmax);
	});

	cout << "vector of " << vd.size() << " random numbers within ["
		 << dmin << "..." << dmax << "] : ";
	// DONE 08 : copy vd elements to standard output with 4 digits after decimal point
	cout.precision(4);
	copy(vd.cbegin(), vd.cend(), doutputter);
	cout << endl;

	// DONE 09 : Compute greater difference between vd elts with greater_diff
	double gdD = 0.0;
	gdD = greater_diff(vd.cbegin(), vd.cend());
	cout << endl << "greater diff is : " << gdD << endl;

	/*
	 * Assert greater difference is smaller or equal to maxGreaterDiff
	 */
	assert(gdD <= maxGreaterDiff);

    // DONE 10 : Sort vd in descending order of absolute magnitude using a
    // standard algorithm with 'greaterMag' comparator.
	sort(vd.begin(), vd.end(), greater_mag<double>);

	/*
	 * Compute adjacent differences of vd in lDiff
	 */
	list<double> lDiff(vd.size());
	adjacent_difference(vd.cbegin(),
							 vd.cend(),
							 lDiff.begin(),
							 [](const double v1, const double v2) -> double
							 {
								return abs(v1) - abs(v2);
							 });
	/*
	 * lDiff[i] = vd[i] - vd[i-1]
	 * but lDiff[0] == vd[0] which is useless ==> pop it.
	 */
	lDiff.pop_front();
	/*
	 * Assert all differences should be negative since vd is sorted
	 * in decreasing order of magnitude
	 */
	for_each(lDiff.cbegin(),
				  lDiff.cend(),
				  [](const double v) -> void
				  {
					  assert(v < 0.0); // Magnitude décroissante
				  });

	// DONE 11 : copy vd elements to standard output
	cout << "vd après tri décroissant de la magnitude = ";
	copy(vd.cbegin(), vd.cend(), doutputter);
	cout << endl;

	// DONE 12 : Compute the mean of vd elts with standard algo and display it
	double vmean1 = 0.0;
	vmean1 = accumulate(vd.cbegin(), vd.cend(), 0.0) / vd.size();
	cout << endl << "Elements mean = " << vmean1 << endl;

	// DONE 14 : Compute mean of vd elts with standard algo and IncrementalMeanOperator as functor
	double vmean2 = 0.0;
	IncrementalMeanOperator<double> meanOp;
	vmean2 = accumulate(vd.cbegin(), vd.cend(), 0.0, meanOp);
	cout << "Incremental Elements mean = " << vmean2 << endl;

	/*
	 * Assert incremental mean is the same as standard mean within tolerance
	 */
	assert(abs(vmean2 - vmean1) < tolerance);

	/*
	 * Use only one of 15, 16 or 17 since they all perform the same operation
	 */
	// DONE 15 : Subtract mean value to all elements of vd using ConstantSubstractor
	ConstantSubstractor<double> subtractor(vmean2);
	transform(vd.cbegin(), vd.cend(), vd.begin(), subtractor);

	// DONE 16 : Subtract mean value to all elements of vd using binded standard functor "minus"
	 transform(vd.cbegin(), vd.cend(), vd.begin(), bind(minus<double>(), placeholders::_1, vmean2));

	// DONE 17 : Subtract mean value to all elements of vd using a lambda function
	 transform(vd.cbegin(), vd.cend(), vd.begin(), [vmean2](const double& x) -> double { return x - vmean2; });

	// DONE 18 : copy vd elements to standard output
	cout << "Elements after mean subtraction : ";
	copy(vd.cbegin(), vd.cend(), doutputter);
	cout << endl;

	// DONE 19 : Recompute mean of vd elements using standard algo. and IncrementalMeanOperator
	IncrementalMeanOperator<double> meanOp2;
	vmean2 = accumulate(vd.cbegin(), vd.cend(), 0.0, meanOp2);
	cout << endl << "New elements mean = " << vmean2 << endl;

	/*
	 * Assert vd is sorted and new mean is 0 within tolerance
	 */
	assert(!is_sorted(vd.cbegin(), vd.cend()));
	assert(abs(vmean2) < tolerance);

	return EXIT_SUCCESS;
}
