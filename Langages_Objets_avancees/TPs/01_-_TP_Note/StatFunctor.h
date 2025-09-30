/*
 * StatFunctor.h
 *
 *  Created on: 21 févr. 2013
 *      Author: davidroussel
 */

#ifndef STATFUNCTOR_H_
#define STATFUNCTOR_H_

#include <functional>	// for unary_function
using namespace std;

#include "TypeException.h"

/**
 * Foncteur unaire pour calculer la moyenne et l'écart type d'un ensemble
 * de valeurs
 * @tparam T type des valeurs
 * @note Attention, la valeur moyenne et l'écart type doivent renvoyer des
 * valeurs "double" quel que soit le type T.
 */
template<class T>
class StatFunctor: public unary_function<T, void>
{
	private:
		/*
		 * TODO attributs
		 */
	public:
		/*
		 * TODO méthodes
		 * 	- constructeur --> throws TypeException si T n'est pas is_arithmetic<T>
		 * 	- operator ()(const T&) : void
		 * 	- mean() : double
		 * 	- std() : double
		 * 	- reset()
		 */
};

#endif /* STATFUNCTOR_H_ */
