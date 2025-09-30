/*
 * RunException.cpp
 *
 *  Created on: Sep 16, 2025
 *      Author: david.roussel
 */

#include "RunException.h"

/*
 * Constructeur de l'exception
 * @param message le message de l'exception
 */
RunException::RunException(string && message) :
	message(std::forward<string>(message))
{
}

/*
 * Destructeur de l'exception : efface le message
 */
RunException::~RunException() noexcept
{
	message.clear();
}

/*
 * Description de l'exception
 * @return une chaîne de caractères décrivant l'exception
 */
const char* RunException::what() const noexcept
{
	return message.c_str();
}

/*
 * Opérateur de sortie standard pour ce type d'exception
 * @param out le flux de sortie
 * @param c l'exception à afficher
 * @return une référence vers le flux de sortie
 */
ostream & operator << (ostream & out, const RunException & c)
{
	out << "RunException::" << c.message;
	return out;
}
