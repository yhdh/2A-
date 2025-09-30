/*
 * StatFunctor.cpp
 *
 *  Created on: 21 févr. 2013
 *      Author: davidroussel
 */

#include "functors/StatFunctor.h"
#include <cmath>	// for sqrt

/*
 * Constructeur par défaut.
 * Initialise la somme des éléments, la somme des éléments au carré
 * ainsi que le nombre d'éléments à 0.
 */
template<typename Arithmetic>
StatFunctor<Arithmetic>::StatFunctor() 
    // DONE 05-f Liste d'initialisation ...
     : sum(0.0), sumSquares(0.0), nbElements(0)
{
}

/*
 * DONE 05-g operator()(...): Opérateur d'appel de fonction
 * @param value valeur à ajouter à la somme des valeurs et à la somme
 * des valeurs au carré.
 * @post le nombre d'éléments est incrémenté.
 */
template<typename Arithmetic>
void StatFunctor<Arithmetic>::operator()(const Arithmetic & value)
{
    sum        += static_cast<double>(value);
    sumSquares += static_cast<double>(value) * static_cast<double>(value);
    ++nbElements;
}
/*
 * DONE 05-h mean() : Calcul de la valeur de la moyenne :
 * E(X) = sum/nbElements
 * @return la valeur moyenne des valeurs additionnées.
 * sous la forme d'un double.
 */
template<typename Arithmetic>
double StatFunctor<Arithmetic>::mean() const
{
    if (nbElements == 0)
    {
        return 0.0;
    }
    return sum / static_cast<double>(nbElements);
}
/*
 * DONE 05-i std() : Calcul de l'écart type : sqrt(E(X^2) - E(X)^2)
 * @return l'écart type des valeurs additionnées
 * sous la forme d'un double.
 */
template<typename Arithmetic>
double StatFunctor<Arithmetic>::std() const
{
    if (nbElements == 0)
    {
        return 0.0;
    }
    const double m   = mean();
    const double ex2 = sumSquares / static_cast<double>(nbElements);
    double var = ex2 - m * m;
    // Protection contre les petites erreurs numériques (var légèrement négative)
    if (var < 0.0) var = 0.0;
    return std::sqrt(var);
}
/*
 * DONE 05-j reset() : Remise à zéro des statistiques
 * @post la somme des éléments, la somme des éléments au carré ainsi
 * que le nombre d'éléments ont été remis à 0
 */
template<typename Arithmetic>
void StatFunctor<Arithmetic>::reset()
{
    sum = 0.0;
    sumSquares = 0.0;
    nbElements = 0;
}
// ----------------------------------------------------------------------------
// Proto instanciations
// ----------------------------------------------------------------------------
/*
 * Instanciation du template StatFunctor avec des <int>
 */
template class StatFunctor<int>;

/*
 * Instanciation du template StatFunctor avec des <float>
 */
template class StatFunctor<float>;

/*
 * Instanciation du template StatFunctor avec des <double>
 */
template class StatFunctor<double>;
