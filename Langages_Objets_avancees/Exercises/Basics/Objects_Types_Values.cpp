/* 2.1 For each of the declarations below, do the following. State whether the declaration is also a definition. If the
declaration is not a definition, write a definition for the entity being declared. If the declaration is a definition,
write a declaration for the entity being declared that is not also a definition.
(a) int i = 5;  --> definition car elle alloue de la mémoire et initialise i à 5
    Declaration: extern int i;  --> déclaration car elle n'alloue pas de mémoire
(b) int abs(int x); --> déclaration car elle n'alloue pas de la mémoire et ne définit pas le corps de la fonction 
    Definition: int abs(int x) { return (x < 0) ? -x : x; } --> définition car elle alloue de la mémoire pour la fonction et définit son corps
(c) float sqr(float x) {return x * x;} --> définition car elle alloue de la mémoire pour la fonction et définit son corps
    Declaration: float sqr(float x); --> déclaration car elle n'alloue pas de mémoire et ne définit pas le corps de la fonction
(d) extern const double pi; --> déclaration car elle n'alloue pas de mémoire
    Definition: const double pi = 3.14159; --> définition car elle alloue de la mémoire et initialise pi à 3.14159
(e) char s[] = "Hello"; --> définition car elle alloue de la mémoire pour le tableau de caractères et initialise son contenu
    Declaration: extern char s[]; --> déclaration car elle n'alloue pas de mémoire
(f) double x; --> définition car elle alloue par default  de la  mémoire et initialise x à O.0
    Declaration: extern double x; --> déclaration car elle n'alloue pas de mémoire
(g) int (*func)(int, int); --> définition car ça définit un pointeur vers une fonction et alloue de la mémoire pour ce pointeur
    declaration: extern int (*func)(int, int); --> déclaration car elle n'alloue pas de mémoire pour le pointeur et ne définit pas la fonction pointée
(h) template <typename T> T max(T x, T y); --> déclaration car elle n'alloue pas de mémoire et ne définit pas le corps de la template
    Definition: template <typename T> T max(T x, T y) { return (x > y) ? x : y; } --> définition car elle alloue de la mémoire pour la template et définit son corps
(i) template <typename T> bool is_negative(T x) {return x < 0;} --> définition car elle alloue de la mémoire pour la template et définit son corps
(j) auto meaning_of_life = 42; --> définition car elle alloue de la mémoire et initialise meaning_of_life à 42
    Declaration: extern int meaning_of_life; --> déclaration car elle n'alloue pas de mémoire; Nb: La déclaration seule proposée extern auto meaning_of_life; est illégale (on ne peut pas déduire un type avec auto sans initialiseur).
*/
/* 2.2 For each of the declarations below, do the following. State whether the declaration is also an initialization. If it
is not an initialization, modify it so that it is one.
(a) int x;  --> initialization par default à 0 FAUX en fait cette variable x ici est définie mais pas initialisée, elle contient une valeur indéterminée
    initialization: int x = 0; (initialisation à zéro)
(b) void (*f)(); --> initialization: void (*f)() = nullptr; (initialisation à un pointeur nul) FAUX en fait le pointeur de fonction f ici est définie mais pas initialisée, elle contient une valeur indéterminée
    initialization: void (*f)() = nullptr; (initialisation à un pointeur nul)
(c) const double pi = 3.14; --> initialization: const double pi = 3.14; (initialisation à une valeur constante)
*/
/*2.3 State whether each line of code below corresponds to a type declaration or definition.
(a) struct Point {double x; double y;}; ---> definition 
(b) struct Thing; ---> dèclaration
(c) enum Fruit : int {apple, orange, banana}; --> definition
(d) enum Color : int; --> déclaration
*/
/*2.4 Write a declaration for each of the entities listed below. Initialize each one. Do not use the null pointer in any
initializations.
(a) a pointer to a char ---> char* p = new char('a');
(b) a pointer to a constant char ---> const char* p = new char('a');
(c) a constant pointer to a char ---> char* const p = new char('a');
(d) a constant pointer to a constant char ---> const char* const p = new char('a');
(e) a pointer to a function taking a double parameter and returning an int ---> int (*func)(double) = [](double x) { return static_cast<int>(x); };
(f) a pointer to a pointer to an int ---> int** p = new int*(new int(5));
(g) an lvalue reference to an array of 16 ints ---> int a[16]{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int (&arr)[16] = a;
(h) a pointer to an array of 10 elements of type std::string  ---> std::string a[10]{};
std::string (*p)[10] = &a;
(i) an lvalue reference to an array of 8 ints  ---> int a[8]{0,1,2,3,4,5,6,7};
int (&arr)[8] = a;
*/
/* 2.5 Explain what is potentially wrong with the line of code below.
char c = -1;
La première erreur qui saute au yeux est le fait que -1 n'est pas un char 

*/