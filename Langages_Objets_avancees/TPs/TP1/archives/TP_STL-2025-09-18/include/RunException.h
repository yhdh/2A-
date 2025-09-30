/*
 * RunException.h
 *
 *  Created on: Sep 16, 2025
 *      Author: david.roussel
 */

#ifndef RUN_EXCEPTION_H_
#define RUN_EXCEPTION_H_

#include <iostream>		// pour les flux d'entrée/sortie
#include <string>		// pour les string
#include <exception>	// pour la classe de base des exceptions
using namespace std;

/**
 * Classe d'exception à utiliser lorsque l'exécution ne peut pas continuer
 * en raison d'un problème non résolvable
 */
class RunException : public exception
{
	protected:
		/**
		 * Le message de l'exception : la raison de sa création
		 */
		string message;
	public:
		/**
		 * Constructeur de l'exception
		 * @param message le message de l'exception
		 */
		RunException(string && message);

		/**
		 * Destructeur de l'exception : efface le message
		 */
		virtual ~RunException() noexcept override;

		/**
		 * Description de l'exception
		 * @return une chaîne de caractères décrivant l'exception
		 */
		virtual const char* what() const noexcept override;

		/**
		 * Opérateur de sortie standard pour ce type d'exception
		 * @param out le flux de sortie
		 * @param c l'exception à afficher
		 * @return une référence vers le flux de sortie
		 */
		friend ostream & operator << (ostream & out, const RunException & c);
};

#endif /* RUN_EXCEPTION_H_ */
