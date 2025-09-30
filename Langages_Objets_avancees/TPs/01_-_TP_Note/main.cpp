/*
 * main.cpp
 *
 *  Created on: 21 févr. 2013
 *      Author: davidroussel
 */

#include <cstdlib>		// for EXIT_SUCCESS
#include <unistd.h>		// for getcwd
#include <cmath>		// for fabs & sqrt
#include <vector>		// for vector
#include <iostream>		// for cout
#include <fstream>		// for ofstream
#include <algorithm>	// for all std algorithms
#include <iterator>		// for ostream_iterator
#include <sstream>		// for istringstream & ostringstream
#include <typeinfo>		// for typeid

using namespace std;

#include "BoundedRandomGenerator.h"
#include "StatFunctor.h"
#include "ExternalCheck.h"
#include "RCheck.h"

/**
 * Vérification de ... sur [first, last)
 * @tparam InputIterator un input itérateur (evt constant car on ne modifie
 * pas ce qui est pointé par les itérateurs)
 * @param first l'itérateur sur le premier élément
 * @param last l'itérateur au delà du dernier élément
 * @return vrai si ..., false sinon.
 */
template<class InputIterator>
bool mysteryCheck(const InputIterator & first, const InputIterator & last)
{
	if (first == last)
	{
		return true;
	}

	InputIterator iter = first;
	InputIterator next = first;

	while (++next != last)
	{
		if (*next < *iter)
		{
			return false;
		}
		++iter;
	}
	return true;
}

/**
 * Recherche de la médiane dans [first, last) trié
 * @tparam RAIterator un random access iterator car on a besoin de
 * pouvoir utiliser l'arithmétique des itérateurs pour trouver le milieu de
 * [first, last)
 * @param first l'itérateur sur le premier élément
 * @param last l'itérateur au delà du dernier élément
 * @return une paire d'itérateur indiquant la position du ou des elements qui
 * constituent la médiane :
 * Si first == last on renvoie une paire d'itérateur pointant sur <last, last>
 * Si [first, last) n'est pas trié on renvoie une paire d'itérateur pointant sur
 * <last, last>
 * Soit n = last - first le nombre d'éléments
 *	- si n est impair : pair<...> contient deux fois le même itérateur pointant
 *		sur le ((n+1)/2)^ième élément
 *	- si n est pair : pair<...> contient
 *		- dans first un itérateur pointant sur le (n/2)^ième élément
 *		- dans second un itérateur pointant sur le ((n/2) + 1)^ième élément
 */
template<class RAIterator>
pair<RAIterator, RAIterator> median(const RAIterator & first,
                                    const RAIterator & last)
{
	using diff_t = typename iterator_traits<RAIterator>::difference_type;
	using category = typename iterator_traits<RAIterator>::iterator_category;

	pair<RAIterator, RAIterator> p(last, last);

	// TODO A compléter ...

	return p;
}

/**
 * Recherche de la médiane dans [first, last) NON trié
 * @tparam RAIterator un random access iterator car on a besoin de
 * pouvoir utiliser l'arithmétique des itérateurs pour trouver le milieu de
 * [first, last)
 * @param first l'itérateur sur le premier élément
 * @param last l'itérateur au delà du dernier élément
 * @return une paire d'itérateurs indiquant la position du ou des éléments qui
 * constituent la médiane.
 */
template<class RAIterator>
pair<RAIterator, RAIterator> median2(const RAIterator & first,
                                     const RAIterator & last)
{
	using diff_t = typename iterator_traits<RAIterator>::difference_type;
	using value_t = typename iterator_traits<RAIterator>::value_type;
	using pointer = typename iterator_traits<RAIterator>::pointer;
	using category = typename iterator_traits<RAIterator>::iterator_category;

	pair<RAIterator, RAIterator> p(last, last);

	// TODO A compléter ...

	return p;
}

/**
 * Programme principal
 * @param argc nombre d'arguments
 * @param argv arguments
 * @return EXIT_SUCCESS si tout se passe bien, EXIT_FAILURE sinon
 */
int main(int argc, char * argv[])
{
	/*
	 * Typedefs pour simplifier les déclarations de variables
	 */
	using Element = double;
	using Container = vector<Element>;
	//	using iterator = Container::iterator;
	using const_iterator = Container::const_iterator;
	using Printer = ostream_iterator<Element>;

	// nombre d'éléments par défaut
	const size_t DefaultNbElts = 10;
	const Element minVal = Element(-12.3);
	const Element maxVal = Element(25.7);
	const Element defaultVal = Element(0);
	size_t nbElts = DefaultNbElts;
	size_t buffSize = 256;
	char buff[buffSize];
	ofstream logStream;
	bool log = false;
	bool useExtrenalCheck = true;

	/*
	 * Récupération du répertoire courant pour les outils de vérif (matlab ou R)
	 */
	getcwd(buff, buffSize);
	string baseDir(buff);

	/*
	 * S'il y a un argument au programme on recherche un nouveau nombre
	 * d'éléments
	 */
	if (argc > 1)
	{
		istringstream iss(argv[1]);
		iss >> nbElts;
		if (!iss || nbElts < 3)
		{
			nbElts = DefaultNbElts;
		}
	}

	// conteneur utilisé dans le programme
	Container v(nbElts, defaultVal);

	// output iterator utilisé pour afficher les valeurs d'une étendue
	// d'itérateurs sur la console
	Printer printer(cout, " ");
	// si le nombre d'éléments est trop grand, on redirige vers un fichier de log
	if (nbElts > 100)
	{
		log = true;
		// Affichage dans un fichier de log
		logStream.open("main.log", ofstream::out);
		printer = Printer(logStream, " ");
	}

	// ========================================================================
	// remplissage de v avec des valeurs comprises entre -12.3 et +25.7
	// en utilisant un algo avec BoundedRandomGenerator
	// ========================================================================

	// TODO à compléter ...

	// ========================================================================
	// Affichage de v
	// ========================================================================
	cout.precision(4); // affichage des nombres flottants avec 4 chiffres apv
	cout << "v = ";
	// TODO à compléter ...
	cout << endl;

	// ========================================================================
	// Calcul de la moyenne et de l'écart type de v en utilisant
	// StatFunctor dans un algorithme
	// ========================================================================
	StatFunctor<Element> stat; // TODO à compléter ...
	cout << "moyenne des elts = " /* TODO << moyenne */ << " ± " /* << TODO écart type */
		<< endl;

	// ========================================================================
	// Copie triée de v dans vt en utilisant un algorithme
	// ========================================================================
	Container vt(v.size()); // pour recevoir une copie triée de v
	// TODO à compléter ...

	cout << "v trié : vt = ";
	// TODO à compléter ...
	cout << endl;

	// ========================================================================
	// Recherche d'un couple d'itérateurs sur la médiane de
	// [vt.begin(), vt.end())
	// ========================================================================
	/*
	 * On vérifie que [vt.begin(), vt.end()) est trié
	 */
	bool sorted; // TODO à compléter par sorted = XXXX(...);
	if (sorted)
	{
		cout << "vt est trié";
	}
	else
	{
		cout << "vt n'est pas trié";
	}
	cout << endl;

	/*
	 * On recherche la paire d'itérateurs correspondant à la médiane dans vt
	 * (trié) en utilisant median<const_iterator>(...)
	 */
	pair<const_iterator, const_iterator> sortedMedianItPair; // TODO à compléter par = median(...)

	/*
	 * Affichage de la valeur de la médiane uniquement si vt était non vide
	 * et trié:
	 * 	- si sortedMedianItPair.first == sortedMedianItPair.second
	 * 		une seule valeur : celle de *(sortedMedianItPair.first)
	 *	- sinon
	 * 	 	deux valeurs : la valeur de la médiane est alors
	 * 	 	(*(sortedMedianItPair.first) + *(sortedMedianItPair.second)) / 2.0
	 */
	double medianValue = 0.0;

	cout << "La médiane de vt est = ";
	// TODO à compléter ...
	cout << endl;

	// ========================================================================
	// Recherche d'un itérateur sur la médiane dans [v.begin(), v.end())
	// ========================================================================
	/*
	 * On recherche la paire d'itérateurs correspondant à la médiane dans v
	 * (non trié)
	 */
	pair<const_iterator, const_iterator> medianItPair; // TODO à compléter par = median2(...);

	/*
	 * Affichage de la valeur de la médiane uniquement si vt était non vide
	 * et trié:
	 */
	cout << "La médiane de v est = ";
	// TODO à compléter ...
	cout << endl;

	// ========================================================================
	// Vérification des min, max, moyenne, std et mediane en utilisant matlab
	// ========================================================================
	/*
	 * TODO décommentez les lignes ci-dessous lorsque StatFunctor est prêt et
	 * que vous disposez d'une valeur pour medianValue.
	 * Si Matlab et/ou R sont installés, ils sont utilisés pour vérifier les
	 * valeurs de
	 * 	- contenu de v / [min,max]
	 * 	- mean
	 * 	- std
	 * 	- median
	 */
//	if (useExtrenalCheck)
//	{
//		ExternalCheck<const_iterator, Element> * checker =
//			new RCheck<const_iterator, Element>(baseDir,
//			                                    v.cbegin(),
//			                                    v.cend(),
//			                                    minVal,
//			                                    maxVal,
//			                                    stat.mean(),
//			                                    stat.std(),
//			                                    medianValue);
//		if (checker->available())
//		{
//			checker->run();
//		}
//
//		delete checker;
//	}

	// ========================================================================
	// Vérification de la propriété
	// (abs(mediane - moyenne) / ecarttype) <= sqrt (3/5)
	// ========================================================================
	double test;  // TODO à compléter ...
	double testMax = sqrt(3.0/5.0);
	cout << test << " ";
	if (test <= testMax)
	{
		cout << "<= ";
	}
	else
	{
		cout << "> ";
	}
	cout << testMax << endl;

	// ========================================================================
	// Soustraction de la (moyenne des elts de v) à v
	// en utilisant uniquement des algorithmes et de foncteurs standards
	// ========================================================================
	/*
	 * Soustraction de la moyenne de v à tous les elts de v
	 */
	// TODO à compléter ...

	/*
	 * Afichage de v modifié
	 */
	cout << "v - " /*<< TODO  moyenne */ << " = ";
	// TODO à compléter ...
	cout << endl;

	// ========================================================================
	// Copie des elts >= 0 de v dans vp et < 0 dans vn
	// en utilisant uniquement des algorithmes et de foncteurs standards
	// Attention ! vp & vn n'ont pas de taille préalable, il faudra donc
	// explicitement insérer les éléments dans vp & vn avec des itérateurs de
	// sortie particuliers destinés à insérer.
	// ========================================================================
	Container vp;
	Container vn;

	/*
	 * Copie des elts de v >= 0 dans vp
	 */
	// TODO à compléter ...

	/*
	 * Copie des elts de v < 0 dans vn
	 */
	// TODO à compléter ...

	/*
	 * Affichage des elts de vp
	 */
	cout << "elts >= 0 de v : ";
	// TODO à compléter ...
	cout << endl;

	/*
	 * Affichage des elts de vn
	 */
	cout << "elts < 0 de v : ";
	// TODO à compléter ...
	cout << endl;

	/* ========================================================================
	 * Peut on remplacer "vector" par "deque" au début du programme ?
	 * TODO Développez la réponse :
	 */

	/* ========================================================================
	 * Peut on remplacer "vector" par "list" au début du programme ?
	 * TODO Développez la réponse :
	 */

	/* ========================================================================
	 * Quelles sont les conditions pour que l'on puisse affecter à Element
	 * un type quelconque (des CompteBanque par exemple)
	 * TODO Listez les opérations nécessaires
	 * 	-
	 */

	if (log)
	{
		logStream.close();
	}

	return EXIT_SUCCESS;
}
