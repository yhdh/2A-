/*
 * ComplexException.cpp
 *
 *  Created on: Nov 2, 2009
 *      Author: david.roussel
 */

#include "TypeException.h"

/*
 * Constructeur de l'exception
 * @param message le message de l'exception
 */
TypeException::TypeException(string && message) :
	message(std::forward<string>(message))
{
}

/*
 * Destructeur de l'exception : efface le message
 */
TypeException::~TypeException() throw ()
{
	message.clear();
}

/*
 * Description de l'exception
 * @return une chaine de caractères décrivant l'exception
 */
const char* TypeException::what() const throw()
{
	return message.c_str();
}

/*
 * Opérateur de sortie standard pour ce type d'exception
 * @param out le flux de sortie
 * @param c l'exception à afficher
 * @return une référence vers le flux de sortie
 */
ostream & operator << (ostream & out, const TypeException & c)
{
	out << "TypeException::" << c.message;
	return out;
}
