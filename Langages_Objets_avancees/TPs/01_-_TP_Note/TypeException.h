/*
 * ComplexException.h
 *
 *  Created on: Nov 2, 2009
 *      Author: david.roussel
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <iostream>		// pour les flux d'entrée/sortie
#include <string>		// pour les string
#include <exception>	// pour la classe de base des exceptions
using namespace std;

/**
 * Classe d'exception à utiliser lorsqu'un type T n'est pas compatible avec
 * les opérations que l'on prévoit de faire sur ce type T
 */
class TypeException : public exception
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
		TypeException(string &&  message);

		/**
		 * Destructeur de l'exception : efface le message
		 */
		virtual ~TypeException() throw ();

		/**
		 * Description de l'exception
		 * @return une chaine de caractères décrivant l'exception
		 */
		virtual const char* what() const throw ();

		/**
		 * Opérateur de sortie standard pour ce type d'exception
		 * @param out le flux de sortie
		 * @param c l'exception à afficher
		 * @return une référence vers le flux de sortie
		 */
		friend ostream & operator << (ostream & out, const TypeException & c);
};

#endif /* EXCEPTION_H_ */
