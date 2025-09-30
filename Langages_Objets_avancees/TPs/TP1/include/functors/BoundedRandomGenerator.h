/*
 * BoundedRandomGenerator.h
 *
 *  Created on: 21 févr. 2013
 *      Author: davidroussel
 */

#ifndef BOUNDEDGENERATOR_H_
#define BOUNDEDGENERATOR_H_

#include "Arithmetic.h"

/**
 * Foncteur permettant de générer des valeurs aléatoires comprises entre un
 * minimum et un maximum
 * @tparam Arithmetic le type des valeurs
 */
template <typename Arithmetic>
class BoundedRandomGenerator
{
	private:
		/*
		 * Attributes
		 */
		// TODO 02-a attributes ...
		Arithmetic min;
   		Arithmetic max;
	public:
		/**
		 * Constructeur d'un Générateur de nombres aléatoires borné
		 * @param min la valeur minimale
		 * @param max la valeur maximale
		 */
		BoundedRandomGenerator(const Arithmetic & min, const Arithmetic & max);

		/**
		 * TODO 02-b Opérateur d'appel de fonction
		 * @return un nouveau nombre aléatoire compris entre min et max
		 */
		Arithmetic operator()();
};

#endif /* BOUNDEDGENERATOR_H_ */
