# Notes de cours — Langages Objets Avancés (C++)

Ce document synthétise les notions clés de C++ moderne (C++11 et +) avec définitions, explications et exemples courts et corrects. Il est prêt à être versionné sur Git.

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
- `friend` donne un accès privilégié à l’implémentation tout en gardant les champs privés.
- À utiliser avec parcimonie; utile pour `operator<<`, `operator==`, etc.

Exemple
```cpp
class Vect2D {
  double x_{}, y_{};
public:
  Vect2D(double x, double y) : x_{x}, y_{y} {}
  friend std::ostream& operator<<(std::ostream& os, const Vect2D& v) {
    return os << '(' << v.x_ << ',' << v.y_ << ')';
  }
};
```

---

## 10) Bonnes pratiques C++ moderne
- RAII: encapsuler les ressources (fichiers, mémoire, verrous) dans des objets.
- Éviter `new`/`delete` directs; préférer `std::unique_ptr`, `std::shared_ptr`.
- `explicit` pour les constructeurs à un argument; `override` pour le polymorphisme; `=delete` pour interdire des opérations.
- `noexcept` pour les opérations de mouvement quand c’est vrai.
- Préférer `std::array`/`std::vector` aux tableaux C.
- Const-correctness partout; passer par `const&` les objets lourds.
- Initialiser toujours les membres (valeurs par défaut ou listes d’initialisation).

---

## Pour dépôt Git
- Vérifier l’orthographe et la cohérence des exemples (compilables en C++11).
- Inclure `README.md`, `LICENSE`, `.gitignore`, et le cas échéant `Makefile`.
- Utiliser des messages de commit explicites (impératif court): « Ajoute exemples rvalue/ref », « Corrige const-correctness », …
