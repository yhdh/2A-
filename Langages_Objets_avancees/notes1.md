# Notes de cours — Langages Objets Avancés (C++)

Ce document synthétise les notions clés de C++ moderne (C++11 et +) avec définitions, explications et exemples courts et corrects. 

---

## Table des matières
- Rappels C et spécificités C++
- Passage des arguments (valeur, pointeur, référence)
- Lvalues, Rvalues et références de rvalue
- Pointeurs, références et const-correctness
- Définition d’une classe, instances et visibilité
- Accès aux membres, `this` et encapsulation
- Constructeurs, listes d’initialisation et règle des 0/3/5
- Copie, déplacement et sémantique de mouvement
- Fonctions et classes amies (`friend`), surcharge d’opérateurs
- Bonnes pratiques C++ moderne
- Pour dépôt Git

---

## 1) Rappels C et spécificités C++
- Fichiers
  - Interface: `.h`/`.hpp`, Implémentation: `.cpp`.
  - Protéger les en-têtes: `#pragma once` ou gardes d’inclusion.
- Noms
  - Éviter `using namespace std;` dans les en-têtes.
- Compilation
  - Standard du cours: C++11. Exemple: `-std=c++11 -Wall -Wextra -pedantic`.

---

## 2) Passage des arguments
- Par valeur (`T x`): copie de l’argument. À privilégier pour les types petits/triviaux (int, double, petits POD).
- Par pointeur (`T* p`): peut être nul, exprime « optionnel » ou « sortie ». Utiliser `nullptr`. Nécessite `*p`.
- Par référence (`T& r`): non nulle par construction; sémantique d’alias. 
  - Lecture seule: `const T&` (évite une copie pour les objets lourds).
  - Sortie/mutation: `T&`.
- Java: passage par valeur de la référence (la référence est copiée, l’objet référencé peut être modifié).

Exemple
```cpp
void f_val(std::string s);            // copie
void f_ref(const std::string& s);     // pas de copie, lecture seule
void f_out(double& y) { y = 42.0; }   // sortie
void f_opt(const int* p);             // peut être nullptr
```

---

## 3) Lvalues, Rvalues et références de rvalue
- lvalue: a une identité (adresse stable), peut être à gauche d’une affectation.
- rvalue: valeur temporaire, sans identité durable (littéraux, résultats d’expressions).
- Référence de lvalue: `T&`. Référence de rvalue: `T&&`.

Exemples
```cpp
int a = 3;         // a: lvalue
int& lr = a;       // OK
// int& bad = 15;  // Erreur: 15 est un rvalue
const int& cr = 15;   // OK: liaison d’un rvalue à une ref const
int&& rr = 15;     // OK: ref de rvalue (C++11)
```

Mouvement et `std::move`
```cpp
std::string s = "abc";
std::string t = std::move(s); // t vole les ressources de s
// s reste valide mais dans un état non spécifié (souvent vide)
```

---

## 4) Pointeurs, références et const-correctness
- `const` qualifie l’objet visé: `const T* p` (pointeur vers T constant), `T* const p` (pointeur constant vers T), `const T* const p` (les deux).
- Méthode `const`: ne modifie pas l’état observable de l’objet.
- `mutable`: autorise la mutation de champs dans une méthode `const` (pour caches, compteurs, etc.).

---

## 5) Définition d’une classe, instances et visibilité
Exemple minimal, clair et correct:
```cpp
// CompteBanque.hpp
#pragma once
#include <string>
#include <iosfwd>

class CompteBanque {
public:
  using montant_t = double;

private:
  int numero_ {0};
  std::string titulaire_ {};
  montant_t solde_ {0.0};

public:
  // Constructeurs
  CompteBanque() = default; // valeurs par défaut ci-dessus
  explicit CompteBanque(int numero, std::string titulaire, montant_t versementInitial = 0.0);

  // Accès en lecture (const-correct)
  int numero() const noexcept { return numero_; }
  const std::string& titulaire() const noexcept { return titulaire_; }
  montant_t solde() const noexcept { return solde_; }

  // Opérations
  void depot(montant_t montant);
  bool retrait(montant_t montant); // false si fonds insuffisants

  // Affichage
  friend std::ostream& operator<<(std::ostream& os, const CompteBanque& c);
};
```
```cpp
// CompteBanque.cpp
#include "CompteBanque.hpp"
#include <stdexcept>
#include <ostream>

CompteBanque::CompteBanque(int numero, std::string titulaire, montant_t versementInitial)
  : numero_{numero}, titulaire_{std::move(titulaire)}, solde_{0.0}
{
  if (versementInitial < 0) throw std::invalid_argument("Versement initial négatif");
  solde_ = versementInitial;
}

void CompteBanque::depot(montant_t montant) {
  if (montant <= 0) throw std::invalid_argument("Montant invalide");
  solde_ += montant;
}

bool CompteBanque::retrait(montant_t montant) {
  if (montant <= 0 || montant > solde_) return false;
  solde_ -= montant; return true;
}

std::ostream& operator<<(std::ostream& os, const CompteBanque& c) {
  return os << "Compte #" << c.numero_ << " (" << c.titulaire_ << ") — solde = " << c.solde_;
}
```

Remarques
- Visibilité: `public` (interface), `private` (données cachées), `protected` (héritage).
- Instances
  - statique (pile): `CompteBanque c1{1, "Alice", 100.0};`
  - dynamique (tas): `auto p = std::make_unique<CompteBanque>(2, "Bob", 50.0);`

---

## 6) Accès aux membres et pointeur `this`
- Dans une méthode, `this` pointe l’instance courante; `(*this)` est l’objet.
- Préférer les accesseurs/mutateurs explicites et la validation des invariants.

Exemple
```cpp
void CompteBanque::depot(montant_t m) {
  if (m <= 0) throw std::invalid_argument("Montant invalide");
  this->solde_ += m; // équivaut à solde_ += m;
}
```

---

## 7) Constructeurs, listes d’initialisation et règle des 0/3/5
- Liste d’initialisation: initialiser les membres AVANT l’entrée dans le corps du constructeur; respecte l’ordre de déclaration.
- Constructeurs usuels
  - par défaut, valué, de copie, de déplacement.
- Destructeur: libère les ressources détenues.
- Règles
  - Règle des 0: si votre classe ne gère pas de ressources, laissez le compilateur générer (=`default`).
  - Règle des 3: si vous définissez l’un (copie ctor, op=, dtor), définissez les trois.
  - Règle des 5: + versions « move » (C++11).

Exemple de liste d’initialisation
```cpp
struct S { int a; std::string s; S(int x, std::string y) : a{x}, s{std::move(y)} {} };
```

---

## 8) Copie, déplacement et sémantique de mouvement
```cpp
struct Buffer {
  size_t n{}; double* data{};
  Buffer(size_t n_) : n{n_}, data{ new double[n]{} } {}
  ~Buffer() { delete[] data; }
  Buffer(const Buffer& b) : n{b.n}, data{ new double[n] } { std::copy(b.data, b.data+n, data); }
  Buffer& operator=(const Buffer& b) {
    if (this != &b) { Buffer tmp(b); std::swap(n, tmp.n); std::swap(data, tmp.data); }
    return *this;
  }
  Buffer(Buffer&& b) noexcept : n{b.n}, data{b.data} { b.n=0; b.data=nullptr; }
  Buffer& operator=(Buffer&& b) noexcept {
    if (this != &b) { delete[] data; n=b.n; data=b.data; b.n=0; b.data=nullptr; }
    return *this;
  }
};
```

---

## 9) Fonctions et classes amies; surcharge d’opérateurs

### Fonctions amies — `friend`
- Les fonctions amies ont accès aux membres protégés et privés de la classe dans laquelle elles sont déclarées amies.
- Cas d’usage typique: surcharge des opérateurs d’entrée (`>>`) et de sortie (`<<`) standards pour une classe donnée.

Exemple (l’opérateur n’est PAS membre; il est déclaré ami dans la classe):
```cpp
#include <ostream>
#include <istream>
#include <string>

class CompteBanque {
public:
  // ...interface publique...
  friend std::ostream& operator<<(std::ostream& out, const CompteBanque& cpt);
  friend std::istream& operator>>(std::istream& in, CompteBanque& cpt);
protected:
  int numero{};
  double solde{};
};

inline std::ostream& operator<<(std::ostream& out, const CompteBanque& cpt) {
  out << "Compte n° " << cpt.numero << " — solde = " << cpt.solde;
  return out; // accès aux membres protégés
}

inline std::istream& operator>>(std::istream& in, CompteBanque& cpt) {
  in >> cpt.numero >> cpt.solde; // accès aux membres protégés
  return in;
}

// Usage
// CompteBanque c(/*...*/);
// std::cout << c; // OK: operator<< ami
```

### Classes amies — `friend class`
- Les classes amies ont accès aux membres privés et protégés de la classe dans laquelle elles sont déclarées amies.
- Remarques: la relation d’amitié n’est ni symétrique, ni transitive; il faut la déclarer explicitement côté « hôte ».

Exemple (accès d’une classe Rectangle aux membres privés de Square):
```cpp
class Square; // forward declaration

class Rectangle {
  int width{}, height{};
public:
  int area() const { return width * height; }
  void convert(const Square& s); // utilisera l’accès ami
};

class Square {
  friend class Rectangle; // Rectangle peut accéder à side
private:
  int side{};
public:
  void set_side(int a) { side = a; }
};

inline void Rectangle::convert(const Square& s) {
  width  = s.side;  // accès au membre privé de Square
  height = s.side;
}
```

---

## 10) Bonnes pratiques C++ moderne
- Toujours préférer les constructeurs et les listes d'initialisation pour l'initialisation des membres.
- Éviter les méthodes d'initialisation séparées (`init()`), qui peuvent conduire à des états d'objet partiellement initialisés.
- Utiliser `nullptr` plutôt que `0` ou `NULL` pour les pointeurs nuls.
- Préférer les `enum class` aux `enum` traditionnels pour une meilleure sécurité de type.
- Utiliser les `smart pointers` (`std::unique_ptr`, `std::shared_ptr`) pour la gestion automatique de la mémoire.
- Privilégier les algorithmes et structures de données de la STL (Standard Template Library) pour une meilleure efficacité et lisibilité.
- Utiliser les `constexpr` et `inline` pour des fonctions et des variables lorsque cela est possible, afin d'optimiser les performances.
- Écrire des tests unitaires pour valider le comportement des classes et fonctions.
- Utiliser des outils d'analyse statique et de formatage de code pour maintenir la qualité du code.

---

## 11) Initialisations (d’après le cours)

Comment initialiser des membres protégés ou privés ? Deux approches existent, mais elles ne sont pas équivalentes.

### A) Méthode d’initialisation « init » (possible mais déconseillée)
- Doit être publique pour pouvoir être appelée de l’extérieur.
- Problème: un objet peut exister dans un état partiellement initialisé entre la construction et l’appel de `init()`.
- En pratique, on lui préfère les constructeurs qui garantissent les invariants dès la création de l’objet.

Exemple (à connaître, mais à éviter en production):
```cpp
class CompteBanque {
public:
  double consultation() const;
  double retrait(double);
  double ajout(double);
  void init(int n, double s); // possible mais pas recommandé
protected:
  int numero = 0;       // C++11: initialisation en-ligne
  double solde = 0.0;   // C++11: initialisation en-ligne
};

inline void CompteBanque::init(int n, double s) {
  numero = n; solde = s; // l’objet était peut-être utilisé avant cet appel
}
```

Pourquoi c’est risqué ?
- On peut écrire: `CompteBanque c; /* utilisation */ c.init(1,100.0);`
- Entre la construction et `init`, les invariants ne sont pas garantis.

### B) Constructeurs (recommandé)
- Encapsulent l’initialisation; garantissent les invariants dès la construction.
- Permettent l’usage de la liste d’initialisation (plus efficace, nécessaire pour `const` et références).

Exemple correct et sûr:
```cpp
class CompteBanque {
public:
  CompteBanque() = default; // utilise les valeurs en-ligne
  CompteBanque(int n, double s) : numero{n}, solde{s} { /* validations si besoin */ }
  double consultation() const { return solde; }
  bool retrait(double m) { if (m<=0 || m>solde) return false; solde -= m; return true; }
  void depot(double m) { if (m<=0) throw std::invalid_argument("montant"); solde += m; }
protected:
  int numero = 0;      // C++11: valeurs par défaut en-ligne
  double solde = 0.0;  // C++11: valeurs par défaut en-ligne
};
```

Points clés
- Initialisation en-ligne (C++11): définit des valeurs par défaut robustes et lisibles.
- Liste d’initialisation du constructeur: obligatoire pour `const` et `T&`, recommandée pour tout membre.
- L’ordre d’initialisation suit l’ordre de déclaration des membres, pas l’ordre dans la liste.

Exemple sur l’ordre d’initialisation:
```cpp
struct P { std::string a; std::string b; P(): b{"B"}, a{"A"} {} }; // a est initialisé avant b malgré l’ordre de la liste
```

Membres `const` et références:
```cpp
struct S {
  const int k;         // doit être initialisé en liste
  int& ref;            // doit référencer un objet existant
  S(int val, int& r) : k{val}, ref{r} {} // impossible de les affecter dans le corps
};
```

Initialisation d’objets membres et des classes de base:
```cpp
struct Buffer { Buffer(size_t n); /*...*/ };
struct Base { Base(int); };
struct D : Base {
  Buffer buf; // objet membre
  D(int x, size_t n) : Base{x}, buf{n} { /* corps vide si possible */ }
};
```

Résumé pratique
- Préférer: valeurs par défaut en-ligne + constructeurs avec liste d’initialisation.
- Éviter: méthodes `init()` post-construction qui retardent l’initialisation.
- Toujours valider les invariants dans les constructeurs.


