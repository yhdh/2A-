#ifndef ARITHMETIC_H_
#define ARITHMETIC_H_

#include <type_traits>	   // for is_arithmetic

/**
 * C++20 concept pour vérifier qu'un type est arithmétique
 */
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

#endif /* ARITHMETIC_H_ */