# Suite du cours — Constructeurs (1/3)

Ce qui suit prolonge les notes de `notes1.md` et reprend le contenu de la diapo fournie sur les constructeurs en C++.

## Rôle, nom et surcharge
- Rôle: membres dédiés à l’initialisation des instances (établir les invariants de l’objet).
- Nom: identique à celui de la classe; aucun type de retour (même pas `void`).
- Surchargeables: plusieurs constructeurs avec des signatures différentes (par défaut, valué, copie, etc.).

## Exemple (d’après la diapo)
### Prototype
```cpp
class CompteBanque {
public:
  CompteBanque();
  CompteBanque(const int n, const double s);
  CompteBanque(const CompteBanque& c);
  /* ... */
protected:
  int numero;
  double solde;
};
```

### Implémentation — affectation dans le corps (possible mais déconseillé)
```cpp
CompteBanque::CompteBanque() {
  numero = 0;  solde = 0.0;   // fonctionne mais « bad practice » en C++
}

CompteBanque::CompteBanque(const int n, const double s) {
  numero = n;  solde  = s;    // idem
}

CompteBanque::CompteBanque(const CompteBanque& c) {
  numero = c.numero;          // copie champ par champ
  solde  = c.solde;
}
```
Pourquoi c’est déconseillé ?
- Les membres sont d’abord construits avec leurs valeurs par défaut, puis ré-affectés dans le corps → double travail.
- Impossible d’initialiser ainsi des membres `const` ou des références.
- Moins clair pour l’initialisation de classes de base et d’objets membres non triviaux.

### Implémentation recommandée — listes d’initialisation (bonnes pratiques)
```cpp
CompteBanque::CompteBanque()
  : numero{0}, solde{0.0} {}

CompteBanque::CompteBanque(const int n, const double s)
  : numero{n}, solde{s} {}

CompteBanque::CompteBanque(const CompteBanque& c)
  : numero{c.numero}, solde{c.solde} {}
```
Avantages des listes d’initialisation
- Construction directe des membres (pas d’étape « valeur par défaut » suivie d’une affectation).
- Indispensables pour les membres `const` et `T&` et pour choisir le constructeur des objets membres.
- Syntaxe uniforme, efficace et explicite.

## Points d’attention
- Ordre d’initialisation: c’est l’ordre de déclaration des membres dans la classe qui compte (pas l’ordre dans la liste).
- Toujours garantir les invariants dans les constructeurs.
- Éviter d’appeler des fonctions virtuelles depuis un constructeur (l’objet n’est pas encore de type dérivé « complet »).
- Penser à `explicit` pour éviter les conversions implicites indésirables sur les constructeurs mon-argument.

Exemple d’ordre d’initialisation
```cpp
struct P {
  std::string a;
  std::string b;
  P() : b{"B"}, a{"A"} {} // a est malgré tout initialisé avant b (ordre de déclaration)
};
```

Ceci complète la partie « Constructeurs (1/3) » en cohérence avec vos notes principales.

# Constructeurs (2/3) — Vocabulaire et exemples

Vocabulaire (d’après la diapo)
- Constructeur par défaut: constructeur sans arguments initialisant les attributs avec des valeurs par défaut.
- Constructeur valué: constructeur possédant un ou plusieurs arguments pour initialiser tout ou partie des attributs.
- Constructeur de copie: constructeur possédant un seul argument du même type que la classe et copiant (profondeur pertinente) les attributs d’une instance dans une autre.
- Variante C++11 — Constructeur de déplacement: constructeur possédant un seul argument du même type, en référence de rvalue (`T&&`), et permettant de « transférer » les ressources d’une instance (vouée à disparaître) vers une autre; améliore les performances en évitant des copies coûteuses.

Exemples concis
```cpp
#include <algorithm>
#include <cstddef>
#include <utility>

struct Buffer {
  std::size_t n{};
  double* data{};

  // Par défaut
  Buffer() = default; // n=0, data=nullptr

  // Valué
  explicit Buffer(std::size_t size)
    : n{size}, data{ new double[n]{} } {}

  // Destructeur
  ~Buffer() { delete[] data; }

  // Copie (duplication des ressources)
  Buffer(const Buffer& other)
    : n{other.n}, data{ new double[n] }
  {
    std::copy(other.data, other.data + n, data);
  }

  // Déplacement (transfert des ressources)
  Buffer(Buffer&& other) noexcept
    : n{other.n}, data{other.data}
  {
    other.n = 0; other.data = nullptr; // laisse l’objet source valide mais « vide »
  }
};
```

Utilisation et effets
```cpp
Buffer a(1'000'000);     // construit (valué)
Buffer b = a;            // copie: alloue et recopie 1e6 doubles
Buffer c = std::move(a); // déplacement: c prend le pointeur; a devient « vide »
```

Notes pratiques
- Le déplacement doit idéalement être `noexcept` (optimisations de containers).
- Préférer les containers standard (ex.: `std::vector<double>`) pour éviter de gérer la mémoire à la main; ils implémentent déjà copie et déplacement efficaces.
- Le compilateur peut générer automatiquement les constructeurs (règle des 0/3/5); ne les écrivez que si vous gérez des ressources.

# Constructeurs (3/3) — Retour et appel

Retour
- Un constructeur n’a pas de type de retour explicite et ne retourne pas de valeur; il « produit » l’instance en cours de construction (type de retour implicite = la classe elle‑même, mais rien n’est écrit après le nom du constructeur).

Appel (quand un constructeur est-il invoqué ?)
- Uniquement lors de la création d’une instance (une seule fois par instance):
  - À la déclaration (pile):
    ```cpp
    CompteBanque c1(1, 100.0), c2(2), c3; // c3 utilise le ctor par défaut
    // Forme uniforme recommandée
    CompteBanque c4{3, 50.0};
    int i(3);        // généralisé aux types standards
    int j{3};
    ```
  - Lors d’une allocation dynamique:
    ```cpp
    auto p = new CompteBanque(4, 75.0); // à éviter en C++ moderne
    delete p;
    // Préférer
    auto up = std::make_unique<CompteBanque>(5, 20.0);
    ```
  - Par recopie/déplacement implicite dans la pile:
    ```cpp
    void f(CompteBanque x);  // passage par valeur → appelle le ctor de copie/déplacement
    CompteBanque g() { return CompteBanque{6, 10.0}; } // retour par valeur (copy elision)
    ```

Interdit
- On ne « rappelle » pas un constructeur sur un objet déjà construit:
  ```cpp
  CompteBanque c1(1, 100.0);
  // c1.CompteBanque(1, 100.0); // ERREUR: un constructeur n’est pas une méthode
  ```
- Pour réinitialiser, utilisez l’affectation, reconstruisez un nouvel objet, ou réinitialisez les champs via une API dédiée.

Bonnes pratiques
- Utiliser l’initialisation par accolades `{}` pour éviter les conversions de rétrécissement.
- Préférer la construction directe à l’affectation post‑construction.
- Éviter `new/delete`; utiliser `std::make_unique` / `std::make_shared`.
- Préférer le passage par `const&` pour éviter des copies, et retourner par valeur (copy elision C++17).

## Appels implicites aux constructeurs

Que se passe‑t‑il lorsqu’on passe un objet en argument d’une fonction ? Cela dépend du mode de passage.

- Par valeur (`Type`)
  - La valeur de l’objet est recopiée dans la pile d’appel.
  - Appel implicite au constructeur de copie (ou de déplacement si l’argument est un rvalue).
- Par adresse (`Type*`) ou par référence (`Type&`/`const Type&`)
  - Seule l’adresse est copiée dans la pile d’appel.
  - Pas d’appel au constructeur de copie/déplacement.

Exemple minimal avec traçage
```cpp
#include <iostream>
struct Tracer {
  Tracer() { std::cout << "ctor\n"; }
  Tracer(const Tracer&) { std::cout << "copy-ctor\n"; }
  Tracer(Tracer&&) noexcept { std::cout << "move-ctor\n"; }
};

void by_value(Tracer t) { (void)t; }          // copy/move-ctor
void by_ref(const Tracer& t) { (void)t; }     // aucun ctor
void by_ptr(const Tracer* p) { (void)p; }     // aucun ctor

int main() {
  Tracer a;              // ctor
  by_value(a);           // copy-ctor (passage par valeur)
  by_value(Tracer{});    // move-ctor (rvalue)
  by_ref(a); by_ptr(&a); // aucun ctor
}
```

Situations classiques d’appels implicites aux constructeurs (C++)
- Déclaration d’une instance statique (pile)
  ```cpp
  MaClasse o1; // constructeur par défaut
  ```
- Déclaration suivie d’une « affectation » (copy-initialization)
  ```cpp
  MaClasse o2 = o1; // appel du constructeur de copie
  ```
- Passage d’arguments par valeur
  ```cpp
  int maMethode(MaClasse o, int valeur); // copie/déplacement de o
  ```
- Valeur de retour d’une fonction (retour par valeur)
  ```cpp
  MaClasse fabrique() {
    MaClasse o; // construit
    return o;  // copy elision (C++17) ou déplacement/copie selon les cas
  }
  ```

Bonnes pratiques
- Pour les objets lourds: passer par `const&`; ne passer par valeur que si nécessaire.
- Retourner par valeur: bénéficier de la copy elision (C++17) ou d’un move efficace.
- Rendre le constructeur de déplacement `noexcept` pour de meilleures optimisations (containers).

## Constructeur(s) par défaut

Lorsque aucun constructeur n’est défini
- Java: il existe un constructeur par défaut (sans argument).
- C++: sont implicitement fournis (selon les règles du langage):
  - un constructeur par défaut (sans argument),
  - un constructeur de copie par défaut,
  - un constructeur de déplacement par défaut (C++11+),
  - un opérateur d’affectation par copie par défaut,
  - un opérateur d’affectation par déplacement par défaut (C++11+),
  - un destructeur par défaut.

S’il existe au moins un constructeur (quel qu’il soit)
- En Java et en C++: les constructeurs par défaut fournis automatiquement n’existent plus; il faut les écrire explicitement si on en a besoin.

### Exemple — constructeur sans arguments
Prototype
```cpp
class CompteBanque {
public:
  CompteBanque();
  /* ... */
private:
  int numero;
  double solde;
};
```
Implémentation (mauvaise pratique: affectation dans le corps)
```cpp
CompteBanque::CompteBanque() {
  numero = 0;  // Bad practice → préférer la liste d’initialisation
  solde  = 0;
}
```
Implémentation recommandée (liste d’initialisation)
```cpp
CompteBanque::CompteBanque() : numero{0}, solde{0.0} {}
```
Appel implicite (pile)
```cpp
int main() {
  CompteBanque cpt; // appel implicite au constructeur par défaut
}
```

### Astuces C++ moderne
- Forcer/autoriser les opérations spéciales:
```cpp
struct X {
  X() = default;          // demande explicite du ctor par défaut
  X(const X&) = delete;   // interdit la copie
  X& operator=(const X&) = delete;
};
```
- Préférer les listes d’initialisation et l’initialisation en-ligne des membres (`int x{0}; double y{0.0};`).

## Initialisation des attributs

Idée générale 
- Opération simple → utiliser la liste d’initialisation des attributs (qui appelle aussi, au besoin, les constructeurs des membres).
- Opération complexe (validation, logique conditionnelle, I/O) → effectuer l’affectation/traitement dans le corps du constructeur.

Exemple  (adapté en C++ moderne avec `std::string`)
```cpp
class CompteBanque {
public:
  CompteBanque(int n, double val, const std::string& nom);
protected:
  int numero;            // déclaré en premier
  std::string titulaire; // déclaré en second
  double solde = 0.0;    // valeur par défaut en-ligne (C++11)
};

CompteBanque::CompteBanque(int n, double val, const std::string& nom)
  : numero{n},             // liste d’initialisation → opérations simples
    titulaire{nom},
    solde{0.0}
{
  // Corps du constructeur → opérations « complexes » (validation, logs, etc.)
  if (val >= 500.0) {
    solde = val;          // écrasement après validation
  } else {
    std::cerr << "Versement initial trop faible" << std::endl;
    // selon les conventions du projet, on pourrait lever une exception
    // throw std::invalid_argument("Versement initial trop faible");
  }
}
```

Notes importantes
- L’ordre effectif d’initialisation suit l’ordre de déclaration des membres dans la classe (ici: `numero`, puis `titulaire`, puis `solde`), pas l’ordre écrit dans la liste.
- Préférer l’initialisation en-ligne pour des valeurs par défaut simples (`double solde = 0.0;`).
- Utiliser la liste d’initialisation pour: membres `const`, références, et pour choisir le bon constructeur des membres objets.

Variante robuste (validation en amont, sans ré‑affectation)
```cpp
class CompteBanque2 {
public:
  CompteBanque2(int n, double val, std::string nom)
    : numero{n}, titulaire{std::move(nom)}, solde{val}
  {
    if (solde < 500.0) {
      throw std::invalid_argument("Versement initial trop faible");
    }
  }
private:
  int numero{};
  std::string titulaire{};
  double solde{};
};
```
- Ici, on initialise directement `solde` à `val`, puis on valide et on jette une exception si la contrainte n’est pas satisfaite (invariants garantis).

# Constructeur de copie — d’après le cours

Syntaxe
```cpp
NomClasse(const NomClasse& objet);
```
Par défaut (si non défini), le compilateur peut générer un constructeur de copie effectuant une copie membre‑à‑membre.

Deux types d’appels
- Initialisation par copie (explicite/implicite):
  ```cpp
  CompteBanque c1;      // appel implicite au ctor par défaut
  CompteBanque c2(c1);  // appel explicite au ctor de copie
  CompteBanque c3 = c1; // copy‑initialization → ctor de copie
  ```
- Recopie implicite dans la pile (passage/retour par valeur):
  ```cpp
  void AfficheCompte(CompteBanque cpt); // argument par valeur → ctor de copie
  CompteBanque f() { CompteBanque tmp; return tmp; } // retour par valeur (copy elision / move)
  ```

Exemple complet
```cpp
class CompteBanque {
public:
  int numero{};
  double solde{};
  // Autres constructeurs …
  // Ctor de copie explicite (facultatif si la copie par défaut suffit)
  CompteBanque(const CompteBanque& other) = default;
};

void AfficheCompte(CompteBanque cpt) {
  // cpt est une copie passée par valeur
}

int main() {
  CompteBanque c1;              // ctor par défaut
  CompteBanque c2(c1);          // ctor de copie (init directe)
  CompteBanque c3 = c1;         // ctor de copie (copy‑init)
  AfficheCompte(c1);            // passage par valeur → ctor de copie
}
```

Initialisation par copie vs affectation
```cpp
CompteBanque a;
CompteBanque b = a; // ctor de copie
CompteBanque c;
c = a;              // opérateur d’affectation par copie (≠ ctor de copie)
```

Avertissements
- Pour les classes qui gèrent des ressources (pointeurs, fichiers, verrous), définir explicitement copie/affectation (profonde) ou les interdire (`=delete`) et fournir des opérations de déplacement. Préférer RAII et smart pointers.

# Gestion mémoire — d’après la diapo

Gestion explicite (C++)
- Création d’instances
  - Statiques (pile): déclaration → durée de vie limitée au bloc
  - Dynamiques (tas): `new` → durée de vie contrôlée par le programmeur
- Destruction d’instances (destructeur)
  - Statiques: à la sortie du bloc (même en cas d’exception)
  - Dynamiques: `delete` explicite qui appelle le destructeur

Exemple minimal
```cpp
struct MaClasse {
  MaClasse();
  ~MaClasse(); // appelé automatiquement à la destruction
};

int main() {
  MaClasse maValeur(/*...*/);       // pile → ~MaClasse() à la fin du bloc
  MaClasse* monPtr = new MaClasse(/*...*/); // tas
  // ...
  delete monPtr; // appelle ~MaClasse(); ne pas oublier !
}
```

Tableaux dynamiques
```cpp
auto p = new MaClasse[10];
// ...
delete[] p; // utiliser delete[] avec new[]
```

Gestion implicite (Java, SmallTalk, C#)
- Pas de `delete` explicite; un Garbage Collector détruit automatiquement les objets non référencés.
- Les finaliseurs n’offrent pas les mêmes garanties que les destructeurs C++.

Bonnes pratiques en C++ moderne
- Éviter `new/delete` directs; préférer RAII et les smart pointers:
  ```cpp
  auto up = std::make_unique<MaClasse>(/*...*/); // destruction automatique
  auto vec = std::vector<MaClasse>(10);          // préfèrez un conteneur
  ```
- Ne jamais mélanger `new`/`delete[]` (mauvais appariement), éviter double-delete, mettre les pointeurs à `nullptr` si on gère manuellement.

# Sémantique de déplacement — Problème (C++03) et solution (C++11)

Idée générale de la diapo
- Problème (C++03): seules existent les opérations « par défaut / valué / copie / destructeur » → les rvalues (temporaires) provoquent des créations, copies et destructions supplémentaires, coûteuses en mémoire et en temps.
- Objectif: « déplacer » les rvalues dans des lvalues pour éviter les copies inutiles; les lvalues « subtilisent » les ressources des rvalues via des opérations spécifiques.

Rappels
- lvalue: objet nommé, adresseable, persistant au‑delà de l’expression courante.
- rvalue: valeur temporaire (ex: `MyObject(input)`), résultat d’expression, non durable.

Cartographie des appels (d’après la diapo)
```cpp
MyObject o0;                 // constructeur par défaut
MyObject o1(input);          // constructeur valué
MyObject o2(o1);             // constructeur de copie
MyObject o3 = o1;            // copy-initialization → constructeur de copie

MyObject o4 = MyObject(input); // rvalue → en C++03: temp (valué) + copie (souvent élaguée)
                               // C++11+: move-ctor ou elision; C++17: elision garantie

MyObject o5;                  // défaut
o5 = MyObject(input);         // C++03: copie via op= ; C++11+: move-assign si dispo
```

Solution C++11+: move constructor et move assignment
```cpp
#include <algorithm>
#include <cstddef>
#include <utility>

struct MyObject {
  std::size_t n{};
  double* data{};

  MyObject() = default;                       // défaut
  explicit MyObject(std::size_t size)         // valué
    : n{size}, data{ new double[n]{} } {}
  ~MyObject() { delete[] data; }              // destructeur

  // Copie
  MyObject(const MyObject& other)             // copy-ctor
    : n{other.n}, data{ new double[n] }
  { std::copy(other.data, other.data + n, data); }
  MyObject& operator=(const MyObject& other) { // copy-assign
    if (this != &other) {
      double* tmp = new double[other.n];
      std::copy(other.data, other.data + other.n, tmp);
      delete[] data; data = tmp; n = other.n;
    }
    return *this;
  }

  // Déplacement (transfert de ressources)
  MyObject(MyObject&& other) noexcept          // move-ctor
    : n{other.n}, data{other.data}
  { other.n = 0; other.data = nullptr; }
  MyObject& operator=(MyObject&& other) noexcept { // move-assign
    if (this != &other) {
      delete[] data;                 // libère l’ancienne ressource
      n = other.n; data = other.data; // vole la ressource
      other.n = 0; other.data = nullptr; // laisse une source valide mais « vide »
    }
    return *this;
  }
};
```

Utilisation et effets
```cpp
MyObject o0;                    // défaut
MyObject o1(1024);              // valué
MyObject o2(o1);                // copie
MyObject o3 = o1;               // copie
MyObject o4 = MyObject(2048);   // C++11+: move-ctor (ou elision)

MyObject o5;                    // défaut
o5 = MyObject(4096);            // C++11+: move-assign (au lieu d’une copie)

// Déplacer un objet nommé (lvalue) → cast en rvalue
o5 = std::move(o1);             // move-assign; o1 reste valide mais « vide »
```

Notes pratiques
- `std::move` ne déplace pas par lui‑même: il convertit en rvalue; le déplacement a lieu si des opérations de move existent, sinon on retombe sur la copie.
- Marquer les opérations de déplacement `noexcept` pour de meilleures optimisations (containers).
- Après un move, l’objet source doit rester valide (mais son état est non spécifié). Fournir des invariants sûrs (ex.: pointeur `nullptr`, taille 0).
- Règle de 5: si vous gérez une ressource, implémentez/décidez explicitement défaut, copie (ctor/op=), déplacement (ctor/op=), destructeur; sinon, suivez la règle de 0 et laissez le compilateur générer.

## Sémantique de déplacement — Solutions (C++11)

Élision de copie (copy elision)
- Idée: si un temporaire doit être "copié/déplacé" vers un objet du même type, la copie/le déplacement peut être omis: on construit directement l’objet final dans sa mémoire cible.
- C++11/14: optimisation permise, non garantie → « dépend du compilateur ».
- C++17: élision garantie dans des cas précis (initialisation à partir d’un prvalue, `return T{...};`, etc.).

Exemple (diapo)
```cpp
MyObject o4 = MyObject(input);
// C++11/14: soit temp + (copy/move) vers o4, soit élision → construction directe de o4
// C++17: pas de temp matériel, o4 est construit directement par le ctor valué
```

Utilisation du move-ctor / move-assign (s’ils existent)
- Lorsque l’objet cible existe déjà, l’élision n’est pas applicable: on affecte depuis un temporaire.
```cpp
MyObject o5;                 // déjà construit (ctor par défaut)
o5 = MyObject(input);        // construit un temporaire (ctor valué)
                             // puis move-assign vers o5 (si défini),
                             // enfin destruction du temporaire
```
- S’il n’y a pas d’opérations de déplacement, le compilateur retombe sur la copie (copy-assign/ctor de copie).

Bonnes pratiques
- Préférer la construction directe (ex.: `MyObject o{input};`) pour favoriser l’élision et éviter des copies/moves inutiles.
- Fournir `noexcept` sur move-ctor/move-assign pour de meilleures optimisations dans les conteneurs.
- Retourner par valeur (`T f(){ return T{...}; }`) et laisser l’élision (C++17) faire le travail.
- Utiliser `std::move` pour exprimer l’intention de déplacer un objet nommé.

# Destructeur — d’après la diapo

Rôle
- Fonction membre dédiée au « démontage » d’une instance: libérer/fermer les ressources et restaurer les invariants.

Syntaxe
```cpp
virtual ~NomClasse(); // peut être virtuel (recommandé pour les bases polymorphes)
```
- Non surchargeable: une seule forme par classe (prototype figé); peut être `virtual`, `override` (implicite), `=default`, `=delete` (rare), et généralement `noexcept`.

Appel
- Implicite automatique: quand l’instance sort de portée (fin du bloc où elle a été déclarée), y compris en cas d’exception.
- Appelé aussi pour chacun des attributs de la classe et pour les classes de base (ordre inverse de construction).
- Explicite pour les instances allouées dynamiquement: `delete p;` et `delete[] p;` (pour les tableaux). `delete` appelle d’abord le destructeur puis libère la mémoire.

Exemples
Destruction automatique (pile)
```cpp
struct Log { ~Log(){ std::cerr << "~Log\n"; } };
void f(){ Log x; /* ... */ } // ~Log appelé à la fin de f()
```

Dynamique (tas): delete et delete[]
```cpp
struct MaClasse { ~MaClasse() {/* libérer/fermer */} };
int main(){
  MaClasse* p = new MaClasse{};   // construction
  delete p;                       // appelle ~MaClasse puis libère la mémoire

  MaClasse* t = new MaClasse[3];  // tableau dynamique
  delete[] t;                     // utiliser delete[] pour les tableaux
}
```

Polymorphisme: destructeur virtuel requis
```cpp
struct Base { virtual ~Base() = default; };
struct Derive : Base { ~Derive() override = default; };

void g(){
  Base* p = new Derive{};
  delete p; // appelle ~Derive() puis ~Base() car ~Base est virtuel
}
```

Bonnes pratiques
- Mettre un destructeur virtuel pour toute classe destinée à être manipulée via un pointeur/référence de base polymorphe.
- Ne pas lancer d’exception depuis un destructeur (les destructeurs sont `noexcept` par défaut depuis C++11).
- Préférer RAII et les conteneurs/smart pointers pour éviter d’écrire du code de destruction manuel.
- L’ordre de destruction est l’inverse de l’ordre d’initialisation: membres puis base(s).

## Destructeur — Exemple (d’après la diapo)

Code
```cpp
#include <iostream>

struct Toto {
  Toto()  { std::cout << "construction\n"; }
  ~Toto() { std::cout << "destruction\n"; }
};

void f() {
  Toto tin;                           // construit à l’entrée de f
  std::cout << "appel de f\n";        // puis message de f
}                                      // tin détruit à la sortie de f

int main() {
  f();                                 // crée/détruit tin dans f
  Toto tex;                            // construit après l’appel à f
  return 0;                            // tex détruit à la fin de main
}
```

Sortie attendue
```
construction   // tin
appel de f
destruction    // tin
construction   // tex
destruction    // tex
```

À retenir
- Les objets automatiques (sur la pile) sont détruits quand ils sortent de leur portée (LIFO), y compris en cas d’exception.
- Chaque bloc `{ ... }` définit une portée: on peut anticiper une destruction en créant un sous‑bloc.

Variante — portée imbriquée
```cpp
int main() {
  {
    Toto a;                          // construction
    std::cout << "dans le bloc\n";
  }                                   // destruction de a ici
  std::cout << "après le bloc\n";
}
```

# Construction / Destruction — Instances statiques, dynamiques et tableaux

Instances statiques (automatiques)
- Créées lors de leur déclaration.
- Détruites à la fin du bloc où elles sont déclarées (LIFO), même en cas d’exception.
```cpp
{
  Type t1(/*...*/);
  Type t2{ /*...*/ };
  Type t3 = /* ... */;
} // dtor de t3, puis t2, puis t1
```

Instances dynamiquement allouées
- Un pointeur n’est pas un objet possédé: construire/détruire le pointeur ne construit/détruit PAS l’objet pointé.
- Il faut appeler explicitement les opérateurs `new` et `delete` pour l’objet pointé.
```cpp
Type* pt = new Type(/*...*/);
// ...
delete pt; // appelle ~Type() puis libère la mémoire
```

Tableaux
- Sur la pile: tous les éléments sont construits; le constructeur par défaut est appelé pour chaque élément non initialisé explicitement.
- À la destruction, le destructeur est appelé pour chaque élément du tableau (ordre inverse).
```cpp
const std::size_t size = 5;
Type tab1[size];                 // construction de size éléments (ctor par défaut)
Type tab2[size] = { Type{}, /*...*/ }; // init de chaque élément

Type* dytab = new Type[size];    // tableau dynamique
// ...
delete[] dytab;                  // détruit chaque élément, puis libère la mémoire
```

Remarques importantes
- Les attributs de type pointeur d’une classe ne déclenchent pas les ctor/dtor des objets pointés; seul le pointeur est « géré ». Utiliser RAII pour gérer la ressource pointée.
- Préférer les alternatives modernes:
  - `auto p = std::make_unique<Type>(/*...*/);` au lieu de `new/delete`.
  - `std::vector<Type> tab(size);` au lieu de `new Type[size]` / `delete[]`.
- Éviter les fuites mémoire, les double-delete, et les mésappariements `new`/`delete[]`.

## Variable ≠ instance — portée, pointeurs et fuites

Idée clé (d’après la diapo)
- Une variable pointeur et l’instance qu’elle désigne n’ont pas la même durée de vie.
- « Statique » dans la diapo signifie « allocation automatique sur la pile » (à ne pas confondre avec le mot‑clé `static`).

Exemple (fuite de mémoire illustrée)
```cpp
#include <iostream>

struct A {
  A()  { std::cout << "A()\n"; }
  ~A() { std::cout << "~A()\n"; }
};

int main() {
  A* pa1 = nullptr;        // pointeur (variable) du bloc extérieur
  {
    A a;                    // instance automatique (pile) — détruite fin de bloc
    A* pa2 = nullptr;       // pointeur local au sous‑bloc

    pa1 = new A();          // instance dynamique 1
    pa2 = new A();          // instance dynamique 2 (non possédée)
    // ...
  } // fin du sous‑bloc:
    // - ~A() pour 'a' (objet automatique)
    // - 'pa2' (la variable) disparaît MAIS pas l’objet pointé → fuite mémoire

  delete pa1;               // détruit l’objet pointé par pa1 → ~A()
  // fin du bloc extérieur: 'pa1' (la variable) disparaît
}
```
Sortie typique
```
A()         // a
A()         // *pa1
A()         // *pa2
~A()        // a (fin de sous‑bloc)
~A()        // *pa1 (delete pa1)
// pas de ~A() pour *pa2 → fuite
```
Explication
- La fin de portée d’un pointeur ne détruit pas l’objet pointé. Il faut appeler `delete` (ou utiliser RAII) pour chaque `new`.

Solution moderne (RAII avec smart pointers)
```cpp
#include <memory>

int main() {
  std::unique_ptr<A> pa1 = std::make_unique<A>(); // détruit auto. fin de bloc
  {
    auto pa2 = std::make_unique<A>(); // détruit auto. fin du sous‑bloc
    // ...
  } // ~A() pour *pa2 ici
}   // ~A() pour *pa1 ici
```

Bonnes pratiques
- Éviter `new`/`delete` bruts; préférer `std::unique_ptr`, `std::shared_ptr` et les conteneurs.
- Si un pointeur brut est inévitable, documenter l’ownership et appairer strictement `new`/`delete` et `new[]`/`delete[]`.
