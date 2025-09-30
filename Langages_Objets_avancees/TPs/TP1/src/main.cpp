/*
 * main.cpp
 *
 *  Created on: 21 févr. 2013
 *      Author: davidroussel
 */

#include <cstdlib>		// for EXIT_SUCCESS
#include <cmath>		// for fabs & sqrt
#include <vector>		// for vector
#include <deque>
#include <iostream>		// for cout
#include <fstream>		// for ofstream
#include <algorithm>	// for all std algorithms
#include <numeric>		// for numeric algorithms
#include <functional>	// for bind
#include <iterator>		// for ostream_iterator
#include <concepts> 	// for C++20 concepts
#include <type_traits>	// pour le is_same<T1, T2>
#include <sstream>		// for istringstream & ostringstream
#include <typeinfo>		// for typeid
#include <cassert>		// pour assert
#ifdef __cpp_lib_filesystem
	#include <filesystem>	// for current directory
#endif

using namespace std;

#include "functors/BoundedRandomGenerator.h"
#include "functors/StatFunctor.h"
#include "checkers/ExternalCheck.h"
#include "checkers/MatlabCheck.h"
#include "checkers/RCheck.h"

/*
 * Typedefs pour simplifier les déclarations de variables
 */
using NumType = double;
using Container = vector<NumType>;
// using container_iterator = Container::iterator;
using container_const_iterator = Container::const_iterator;

/*
 * Déclaration des templates externes (résolus dans d'autres unités de compilations)
 */
extern template class BoundedRandomGenerator<NumType>;
extern template class StatFunctor<NumType>;
extern template class ExternalCheck<container_const_iterator>;
extern template class MatlabCheck<container_const_iterator>;
extern template class RCheck<container_const_iterator>;

/**
 * Usage help message
 * @param programName program name
 */
void usage(char * programName);

/**
 * Parse arguments to set nbElements, bounds and external check
 * @param argc arguments count
 * @param argv arguments values
 * @param nbElements the number of elements to set from parameters
 * @param bounds to check the BoundedRandom Generator
 * @param external to use an external check (Matlab or R) to check mean and std values
 */
void parse(int argc, char ** argv, size_t & nbElements, bool & bounds, bool & external);

// DONE 01 Remplir les ... ci-dessous
/**
 * Vérification de l'ordre croissant sur sur [first, last)
 * @tparam Iterator un input itérateur (evt constant car on ne modifie
 * pas ce qui est pointé par les itérateurs)
 * @param first l'itérateur sur le premier élément
 * @param last l'itérateur au delà du dernier élément
 * @return vrai si [first, last) est trié par ordre croissant, false sinon.
 */
template <typename Iterator>
	requires input_iterator<Iterator>
bool mysteryCheck(const Iterator & first, const Iterator & last)
{
	if (first == last)
	{
		return true;
	}

	Iterator iter = first;
	Iterator next = first;

	while (++next != last)
	{
		if (*iter > *next)
		{
			return false;
		}
		++iter;
	}
	return true;
}

/**
 * Recherche de la médiane dans [first, last) trié
 * @tparam Iterator un random access iterator car on a besoin de
 * pouvoir utiliser l'arithmétique des itérateurs pour trouver le milieu de
 * [first, last)
 * @param first l'itérateur sur le premier élément
 * @param last l'itérateur au delà du dernier élément
 * @return une paire d'itérateur indiquant la position du ou des elements qui
 * constituent la médiane dans [first, last) :
 * Si first == last on renvoie une paire d'itérateur pointant sur <last, last>
 * Si [first, last) n'est pas trié on renvoie une paire d'itérateur pointant sur
 * <last, last>
 * Soit n = last - first le nombre d'éléments
 *	- si n est impair : pair<...> contient deux fois le même itérateur pointant
 *		sur le (n/2)^ième élément
 *	- si n est pair : pair<...> contient
 *		- dans first un itérateur pointant sur le (n/2)-1^ième élément
 *		- dans second un itérateur pointant sur le (n/2)^ième élément
 */
template <class Iterator>
	requires random_access_iterator<Iterator> ||
			 bidirectional_iterator<Iterator>
pair<Iterator, Iterator> median(const Iterator & first, const Iterator & last)
{
	/*
	 * Type de la différence entre deux itérateurs Iterator
	 * e.g. difference_type dist = last - first;
	 */
	// using difference_type = typename iterator_traits<Iterator>::difference_type;
	/*
	 * Catégorie de l'itérateur Iterator que l'on pourra comparer avec
	 * bidirectional_iterator_tag et random_access_iterator_tag :
	 * e.g. : is_same<category, bidirectional_iterator_tag>::value
	 */
	// using category = typename iterator_traits<Iterator>::iterator_category;

	pair<Iterator, Iterator> p(last, last);

	// DONE 11-a Complete median ...
	if (first == last)
	{
		return p; 
	}
	
	if (!mysteryCheck(first, last))
	{
		return p; 
	}

	auto n = std::distance(first, last);
	Iterator mid = first;
	std::advance(mid, n / 2);
	if (n % 2 == 1)
	{
		
		p = { mid, mid };
	}
	else
	{
		Iterator mid1 = mid;
		std::advance(mid1, -1);
		p = { mid1, mid };
	}

	return p;
}

/**
 * Recherche de la médiane dans [first, last) NON trié
 * @tparam Iterator un random access iterator car on a besoin de
 * pouvoir utiliser l'arithmétique des itérateurs pour trouver le milieu de
 * [first, last)
 * @param first l'itérateur sur le premier élément
 * @param last l'itérateur au delà du dernier élément
 * @return une paire d'itérateurs indiquant la position du ou des éléments qui
 * constituent la médiane dans [first, last)
 * @note Attention dans la paire d'itérateurs renvoyée, les itérateurs doivent
 * appartenir à [first, last) et non pas à un conteneur temporaire qui peut être
 * utilisé à l'intérieur de median2
 */
template <class Iterator>
	requires random_access_iterator<Iterator> ||
			 bidirectional_iterator<Iterator>
pair<Iterator, Iterator> median2(const Iterator & first, const Iterator & last)
{
	// using difference_type = typename iterator_traits<Iterator>::difference_type;
	/*
	 * Type de la valeur pointée par un itérateur Iterator
	 */
	// using value_type = typename iterator_traits<Iterator>::value_type;
	/*
	 * Type du pointeur sur la valeur pointée par un itérateur Iterator
	 */
	// using pointer = typename iterator_traits<Iterator>::pointer;
	// using category = typename iterator_traits<Iterator>::iterator_category;

	pair<Iterator, Iterator> p(last, last);

	// DONE  14-a Complete median2 ...
	if (first == last)
	{
		return p; // <last,last>
	}

	
	using value_type = typename std::iterator_traits<Iterator>::value_type;

	
	std::vector<std::pair<value_type, Iterator>> tmp;
	tmp.reserve(std::distance(first, last));
	for (auto it = first; it != last; ++it)
	{
		tmp.emplace_back(*it, it);
	}

	std::sort(tmp.begin(), tmp.end(),
			[](const auto &a, const auto &b) { return a.first < b.first; });

	const std::size_t n = tmp.size();
	if (n & 1U) // impair
	{
		p = { tmp[n / 2].second, tmp[n / 2].second };
	}
	else        // pair
	{
		p = { tmp[n / 2 - 1].second, tmp[n / 2].second };
	}

	return p;
}

/**
 * Compare numbers for equality up to epsilon
 * @tparam T the number type
 * @param a first number
 * @param b  second number
 * @param epsilon the epsilon encompassing the smallest difference between a & b
 * @return true when the absolute difference between a & b is less than epsilon
 * @return false otherwise
 */
template <typename T>
	requires floating_point<T>
bool compareNumbers(const T & a, const T & b, const T epsilon = T(1e-9))
{
	return (fabs(a - b) < epsilon);
}

/**
 * Programme principal
 * @param argc nombre d'arguments
 * @param argv arguments
 * @return EXIT_SUCCESS si tout se passe bien, EXIT_FAILURE sinon
 */
int main(int argc, char * argv[])
{
	const NumType minVal = NumType(-12.3);
	const NumType maxVal = NumType(25.7);
	const NumType defaultVal = NumType(0);
	const size_t DefaultNbElts = 10;
	size_t nbElts = DefaultNbElts;
	ofstream logStream;
	bool log = false;
	bool checkBounds = false; // TODO 02-f set to true to check BoundedRandomGenerator
	bool useExternalCheck = false; // TODO 26 change to true if available

	/*
	 * Récupération du répertoire courant pour les outils de vérification
	 * (Matlab ou R).
	 * Ces outils doivent être dans le PATH pour pouvoir être utilisés.
	 * On suppose que le répertoire courant est <projet>/bin puisque le
	 * programme est généré dans le sous répertoire bin.
	 */
#ifdef __cpp_lib_filesystem
	filesystem::path cwd = filesystem::current_path();
	filesystem::path scriptPath = cwd.parent_path() /= "scripts";
	if (!filesystem::exists(scriptPath))
	{
		cerr << " scripts directory " << scriptPath << " does not exist" << endl;
		return EXIT_FAILURE;
	}
	string baseDir(scriptPath);
#else
	string baseDir("../scripts");
#endif

	/*
	 * S'il y a des arguments au programme on recherche
	 *	- s'il faut tester le BoundedRandomGenerator [-b|--bounds]
	 *	- s'il faut utiliser un outil externe (Matlab ou R) [-e|--external] pour vérifier
	 *		- la médiane
	 *		- la moyenne
	 *		- l'écart type
	 *	- un nouveau nombre d'éléments pour le container v
	 */
	if (argc > 1)
	{
		parse(argc, argv, nbElts, checkBounds, useExternalCheck);
	}

	// conteneur utilisé dans le programme
	Container v(nbElts, defaultVal);

	// output iterator à utiliser pour afficher les valeurs d'une étendue
	// d'itérateurs sur la console
	ostream_iterator<NumType> printer(cout, " ");
	// si le nombre d'éléments est trop grand, on redirige vers un fichier de log
	if (nbElts > 100)
	{
		log = true;
		// Affichage dans un fichier de log
		logStream.open("main.log", ofstream::out);
		printer = ostream_iterator<NumType>(logStream, " ");
	}

	// ========================================================================
	// TODO 02 Compléter la classe BoundedRandomGenerator
	// ========================================================================
	BoundedRandomGenerator<NumType> generator(minVal, maxVal);

	if (checkBounds)
	{
		// On vérifie que le générateur ne peut pas générer des valeurs > maxVal ou < minVal
		cout << "Checking BoundedRandomGenerator ..." << endl;
		bool minReached = false;
		bool maxReached = false;
		size_t count = 0;
		while (!minReached || !maxReached)
		{
			count++;
			NumType value = generator();
			assert(value >= minVal && value <= maxVal);
			if (!minReached && compareNumbers(value, minVal))
			{
				minReached = true;
				cout << "\t" << minVal << " reached after " << count << " iterations" << endl;
			}
			if (!maxReached && compareNumbers(value, maxVal))
			{
				maxReached = true;
				cout << "\t" << maxVal << " reached after " << count << " iterations" << endl;
			}
		}
		cout << "both " << minVal << " and " << maxVal << " have been reached after " << count << " iterations" << endl;
	}

	// ========================================================================
	// DONE 03 remplissage de v avec des valeurs aléatoires comprises entre
	// minVal et maxVal en utilisant un algo avec BoundedRandomGenerator
	// ========================================================================
	// ...
	std::generate(v.begin(), v.end(), std::ref(generator));
	// On vérifie que les valeurs sont comprises dans [minVal, maxVal]
	for_each(v.cbegin(),
			 v.cend(),
			 [minVal, maxVal](const NumType & elt) -> void
	{
		assert(elt >= minVal && elt <= maxVal);
	});

	// ========================================================================
	// DONE 04 Affichage de v avec printer
	// ========================================================================
	cout.precision(4);
	cout << "v = ";
	std::copy(v.cbegin(), v.cend(), printer);
	cout << endl;

	// ========================================================================
	// DONE 05 Compléter la classe StatFunctor
	// DONE 06 Calcul de la moyenne et de l'écart type de v en utilisant
	// StatFunctor dans un algorithme et non pas dans une boucle for.
	// ========================================================================
	double meanValue = 0.0;
	double stdValue = 0.0;
	{
    StatFunctor<NumType> stats;
    std::for_each(v.cbegin(), v.cend(), std::ref(stats));
    meanValue = stats.mean();
    stdValue  = stats.std();
  	};
	cout << "moyenne des elts = " << meanValue << " ± " << stdValue << endl;

	// ========================================================================
	// DONE 07 Copie triée de v dans vt en utilisant un (et un seul) algorithme
	// ========================================================================
	Container vt(nbElts, defaultVal); // pour recevoir une copie triée de v
	// ...
	std::partial_sort_copy(v.cbegin(), v.cend(), vt.begin(), vt.end());

	cout << "vt trié = ";
	// DONE 08 Affichage de vt avec printer ...
	std::copy(vt.cbegin(), vt.cend(), printer);
	cout << endl;

	// ========================================================================
	// DONE 09 On vérifie que [vt.begin(), vt.end()) est trié
	// ========================================================================
	bool checkVtSorted = mysteryCheck(vt.cbegin(), vt.cend()); // vérifier ...

	if (checkVtSorted)
	{
		cout << "vt est trié";
	}
	else
	{
		cout << "vt n'est pas trié";
	}
	cout << endl;

	// ========================================================================
	// DONE 10 Et que [v.begin(), v.end()) n'est pas trié
	// ========================================================================
	bool checkVUnsorted = !mysteryCheck(v.cbegin(), v.cend()); // vérifier ...

	if (checkVUnsorted)
	{
		cout << "v n'est pas trié";
	}
	else
	{
		cout << "v est trié";
	}
	cout << endl;

	// ========================================================================
	// DONE 11 Compléter la fonction median(...)
	// DONE 12 On recherche la paire d'itérateurs correspondant à la médiane
	// dans vt (trié) : [vt.begin(), vt.end()) en utilisant median(...)
	// ========================================================================
	pair<container_const_iterator, container_const_iterator> sortedMedianItPair =
		median(vt.cbegin(), vt.cend());
	// ========================================================================
	// DONE 13 Affichage de la valeur de la médiane uniquement si vt était non
	// vide et trié:
	//	- si sortedMedianItPair.first == sortedMedianItPair.second
	//	on affiche une seule valeur : celle de *(sortedMedianItPair.first)
	//	- sinon
	//	on affiche les deux valeurs composant la médiane :
	//		*(sortedMedianItPair.first) & *(sortedMedianItPair.second)
	//	puis la valeur de la médiane est qui est alors
	//		(*(sortedMedianItPair.first) + *(sortedMedianItPair.second)) / 2.0
	// ========================================================================
    double medianValue = 0.0;
	if (sortedMedianItPair.first != vt.end())
	{
		cout << "La médiane de vt est = ";
		if (sortedMedianItPair.first == sortedMedianItPair.second)
		{
			medianValue = *(sortedMedianItPair.first);
			cout << medianValue;
		}
		else
		{
			double a = *(sortedMedianItPair.first);
			double b = *(sortedMedianItPair.second);
			medianValue = (a + b) / 2.0;
			cout << a << " et " << b << " -> " << medianValue;
		}
		cout << endl;
	}
	else
	{
		cerr << "Il n'y a pas de médiane : vide" << endl;
	}

	// ========================================================================
	// DONE 14 Compléter la fonction median2(...)
	// DONE 15 Recherche d'un itérateur sur la médiane dans [v.begin(), v.end())
	// On recherche la paire d'itérateurs correspondant à la médiane dans v
	// (non trié)
	//	- si medianItPair.first == medianItPair.second
	//	on affiche une seule valeur : celle de *(medianItPair.first)
	//	- sinon
	//	on affiche les deux valeurs composant la médiane :
	//		*(medianItPair.first) & *(medianItPair.second)
	//	puis la valeur de la médiane est qui est alors
	//		(*(medianItPair.first) + *(medianItPair.second)) / 2.0
	// ========================================================================
	pair<container_const_iterator, container_const_iterator> medianItPair =
		median2(v.cbegin(), v.cend());
	if (medianItPair.first != v.end())
	{
		cout << "La médiane de v est = ";
		if (medianItPair.first == medianItPair.second)
		{
			cout << *(medianItPair.first) << endl;
		}
		else
		{
			double a = *(medianItPair.first);
			double b = *(medianItPair.second);
			cout << a << " et " << b << " -> " << (a + b) / 2.0 << endl;
		}
	}
	else
	{
		cerr << "Médiane pas trouvée" << endl;
	}

	// ========================================================================
	// TODO 26 Vérification des min, max, moyenne, std et médiane en
	// utilisant R ou Matlab
	// ========================================================================
	if (useExternalCheck)
	{
		cout << "Checking using ";
		ExternalCheck<container_const_iterator> * checker =
				new RCheck<container_const_iterator>(baseDir,
													 v.cbegin(),
													 v.cend(),
													 minVal,
													 maxVal,
													 meanValue,
													 stdValue,
													 medianValue,
													 false);
		if (checker->available())
		{
			cout << "R";
			checker->run();
		}
		else
		{
			delete checker;
			checker = new MatlabCheck<container_const_iterator>(baseDir,
																v.cbegin(),
																v.cend(),
																minVal,
																maxVal,
																meanValue,
																stdValue,
																medianValue);

			if (checker->available())
			{
				cout << "Matlab";
				checker->run();
			}
		}
		cout << " ... Done" << endl;
		delete checker;
	}

	// ========================================================================
	// DONE 16 Vérification de la propriété
	// (abs(médiane - moyenne) / écart-type) <= sqrt (3/5)
	// ========================================================================
	double test = 0.0; // Replace by ...
	if (std::abs(stdValue) > 0.0)
  {
    test = std::abs(medianValue - meanValue) / stdValue;
  }
  double testMax = sqrt(3.0 / 5.0);
  cout << test << " " << (test <= testMax ? "<= " : "> ") << testMax << endl;


	// ========================================================================
	// DONE 17 Soustraction de la (moyenne des elts de v) à v
	// en utilisant **uniquement des algorithmes et des foncteurs standards**
	// ========================================================================
	// ...
	std::transform(v.begin(), v.end(), v.begin(),
                 std::bind(std::minus<NumType>(), std::placeholders::_1, meanValue));


	// ========================================================================
	// DONE 18 Affichage de v modifié avec printer
	// ========================================================================
	cout << "v - " << meanValue << " = ";
	 std::copy(v.cbegin(), v.cend(), printer);
	cout << endl;

	// ========================================================================
	// Copie des elts >= 0 de v dans vp et < 0 dans vn
	// en utilisant **uniquement des algorithmes et de foncteurs standards**
	// Attention ! vp & vn n'ont pas de taille préalable, il faudra donc
	// explicitement insérer les éléments dans vp & vn avec des itérateurs de
	// sortie particuliers destinés à insérer.
	// ========================================================================
	Container vp;
	Container vn;
	// ========================================================================
	// DONE 19 Copie des elts de v >= 0 dans vp
	// ========================================================================
	std::copy_if(v.cbegin(), v.cend(), std::back_inserter(vp),
               [](NumType x){ return x >= 0; });

	// ========================================================================
	// DONE 20 Copie des elts de v < 0 dans vn
	// ========================================================================
	 std::copy_if(v.cbegin(), v.cend(), std::back_inserter(vn),
               [](NumType x){ return x < 0; });

	// ========================================================================
	// DONE 21 Affichage des elts de vp avec printer
	// ========================================================================
	cout << "elts >= 0 de v : ";
	std::copy(vp.cbegin(), vp.cend(), printer);
	cout << endl;

	// ========================================================================
	// DONE 22 Affichage des elts de vn avec printer
	// ========================================================================
	cout << "elts < 0 de v : ";
	std::copy(vn.cbegin(), vn.cend(), printer);
	cout << endl;

	// ========================================================================
	// DONE 23 Peut on remplacer "vector" par "deque" au début du programme ?
	// Développez la réponse :Oui ici ça fonctionne, car on n’utilise que des opérations
	// compatibles et la destination de partial_sort_copy exige un itérateur
	// d’accès aléatoire : deque en fournit. Différences : deque alloue par blocs,
	// bonnes insertions en tête ; vector est contigu et souvent plus performant
	// pour parcours/accès séquentiel.
	// ========================================================================

	// ========================================================================
	// DONE 24 peut on remplacer "vector" par "list" au début du programme ?
	// Développez la réponse :  Non tel quel. Certaines étapes (partial_sort_copy) exigent un
	// RandomAccessIterator pour la destination ; std::list ne fournit que des
	// bidirectional iterators. Il faudrait adapter : p.ex. copier vers un vector,
	// trier, puis continuer ; ou utiliser list::sort et recalculer la médiane
	// avec std::next
	// ========================================================================

	// ========================================================================
	// DONE 25 Quelles sont les conditions pour que l'on puisse affecter à
	// Element un type quelconque (des CompteBanque par exemple)
	// - Insérable/assignable dans le conteneur
	// - Comparable (operator<) pour le tri/ordre
	// - Opérations arithmétiques utilisées par les stats (+, -, / scalaire)
	// - Soit convertible en type numérique, soit StatFunctor spécialisé
	// - Affichable (operator<<) si impression
	// - Support de fabs/sqrt (ou spécialisation adéquate pour l’écart-type)
	// ========================================================================

	if (log)
	{
		logStream.close();
	}

	return EXIT_SUCCESS;
}

/*
 * Usage help
 * @param name program name
 */
void usage(char * programName)
{
#ifdef __cpp_lib_filesystem
	string baseName = filesystem::path(string(programName)).filename();
#else
	string baseName(programName);
#endif

	cout << "usage: " << baseName << " "
		 << "\n\t[-b | --bounds]: to check for BoundedRandom bounds "
		 << "\n\t[-e | --external]: to use external checkers (such as Matlab or R) for mean and std"
		 << "\n\t<number of elements in the container>" << endl;
}

/*
 * Parse arguments to set nbElements, bounds and external check
 * @param argc arguments count
 * @param argv arguments values
 * @param nbElements the number of elements to set from parameters
 * @param bounds to check the BoundedRandom Generator
 * @param external to use an external check (Matlab or R) to check mean and std values
 */
void parse(int argc, char ** argv, size_t & nbElements, bool & bounds, bool & external)
{
	if (argc < 2)
	{
		usage(argv[0]);
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			string arg = argv[i];
			if (arg == "-b" || arg == "--bounds")
			{
				bounds = true;
			}
			else if (arg == "-e" || arg == "--external")
			{
				external = true;
			}
			else
			{
				istringstream iss(arg);
				iss >> nbElements;
				if (!iss || nbElements < 3)
				{
					nbElements = 10;	// 10 elts by default
					usage(argv[0]);
				}
			}
		}
	}
}
