/*
 * StatFunctor.h
 *
 *  Created on: 21 févr. 2013
 *      Author: davidroussel
 */

#ifndef STATFUNCTOR_H_
#define STATFUNCTOR_H_

#include <concepts>	   // for C++20 concepts
#include <functional>	// for unary_function
#include <type_traits> // for is_arithmetic
using namespace std;

#include "RunException.h"
#include "Arithmetic.h"

/**
 * Foncteur unaire pour calculer la moyenne et l'écart type d'un ensemble
 * de valeurs
 * @tparam Arithmetic type des valeurs
 * @note Attention, la valeur moyenne et l'écart type doivent renvoyer des
 * valeurs "double" quel que soit le type Arithmetic.
 */
template <typename Arithmetic>
class StatFunctor
{
	private:
		/*
		 * TODO 05-a Attributs
		 * 	- sum : somme des éléments
		 *  - sum2 : somme des éléments au carré
		 *  - count : nombre d'éléments
		 */
		double sum;
		double sumSquares;
		size_t nbElements;

	public:
		/**
		 * Constructeur par défaut.
		 * Initialise la somme des éléments, la somme des éléments au carré
		 * ainsi que le nombre d'éléments à 0.
		 */
        StatFunctor();

		/**
		 * TODO 05-b operator()(...): Opérateur d'appel de fonction
		 * @param value valeur à ajouter à la somme des valeurs et à la somme
		 * des valeurs au carré.
		 * @post le nombre d'éléments accumulé a été incrémenté.
		 * @note Cet opérateur est typiquement utilisé dans un algorithme de
		 * type UnaryFunction for_each( InputIt first, InputIt last,
		 * UnaryFunction f )
		 */
		void operator()(const Arithmetic & value);

		/**
		 * TODO 05-c mean() : Calcul de la valeur de la moyenne :
		 * E(X) = sum/nbElements
		 * @return la valeur moyenne des valeurs additionnées
		 * sous la forme d'un double.
         * @throws RunException si le nombre d'éléments est nul
		 */
		double mean() const;

		/**
		 * TODO 05-d std() : Calcul de l'écart type : sqrt(E(X^2) - E(X)^2)
		 * @return l'écart type des valeurs additionnées
		 * sous la forme d'un double.
         * @throws RunException si le nombre d'éléments est nul
		 */
		double std() const;

		/**
		 * TODO 05-e reset() : Remise à zéro des statistiques
		 * @post la somme des éléments, la somme des éléments au carré ainsi
		 * que le nombre d'éléments ont été remis à 0
		 */
		void reset();
};

#endif /* STATFUNCTOR_H_ */
