# 🎉 Projet Complet : Wrapper malloc → hp_malloc

## ✅ Travail Accompli

### 1. **Bibliothèque dynamique hp_allocator** 
- ✓ `libhp_allocator.so` - Allocateur avec huge pages (14 Mo)
- ✓ API : `hp_malloc()` et `hp_free()`
- ✓ Utilisation des huge pages Linux (2 Mo)
- ✓ Allocation séquentielle optimisée

### 2. **Wrapper malloc/free via LD_PRELOAD** ⭐
- ✓ `libmalloc_wrapper.so` - Intercepte malloc/free automatiquement
- ✓ Redirige vers hp_malloc sans modifier le code source
- ✓ Support de toutes les fonctions d'allocation (calloc, realloc, etc.)
- ✓ Fallback sur malloc système si mémoire pleine
- ✓ Gestion des appels récursifs pendant l'initialisation

### 3. **Scripts Utilitaires**
- ✓ `run_with_hp_malloc.sh` - Exécute n'importe quel programme avec hp_malloc
- ✓ `demo_wrapper.sh` - Démonstration rapide du wrapper
- ✓ `test_wrapper.sh` - Tests complets

### 4. **Benchmarks et Analyse**
- ✓ Intégration avec `svalat_bench_lib`
- ✓ Scripts Python pour analyse de performance
- ✓ Génération automatique de graphiques (PNG)
- ✓ Export CSV des résultats
- ✓ Comparaison malloc vs hp_malloc

### 5. **Documentation Complète**
- ✓ `README_WRAPPER.md` - Documentation technique détaillée
- ✓ `USAGE_WRAPPER.txt` - Guide d'utilisation rapide
- ✓ Exemples d'utilisation
- ✓ Explications des techniques (LD_PRELOAD, dlsym, etc.)

## 📁 Structure du Projet

```
hp_allocator_malloc/
├── lib/
│   ├── libhp_allocator.so      # Allocateur avec huge pages
│   └── libmalloc_wrapper.so    # Wrapper LD_PRELOAD
├── include/
│   └── hp_allocator.h          # Header public
├── SRC/
│   ├── hp_allocator.c          # Implémentation allocateur
│   ├── hp_allocator.h          # Header
│   ├── malloc_wrapper.c        # Implémentation wrapper ⭐
│   ├── main.c                  # Programme de test
│   ├── Makefile                # Compilation automatique
│   ├── libhp_allocator.so      # Bibliothèque locale
│   └── libmalloc_wrapper.so    # Wrapper local
├── run_with_hp_malloc.sh       # Script utilitaire principal ⭐
├── demo_wrapper.sh             # Démonstration
├── test_wrapper.sh             # Tests
├── README_WRAPPER.md           # Documentation complète
└── USAGE_WRAPPER.txt           # Guide rapide
```

## 🚀 Utilisation Simple

### Pour n'importe quel programme (SANS modifier son code) :

```bash
cd /path/to/hp_allocator_malloc

# Exécuter avec hp_malloc
./run_with_hp_malloc.sh /path/to/votre_programme [args...]
```

### Exemple concret avec le benchmark :

```bash
# Benchmark avec malloc standard
cd /path/to/svalat_bench
./bench_malloc > results_malloc.txt

# Benchmark avec hp_malloc (via wrapper)
/path/to/hp_allocator_malloc/run_with_hp_malloc.sh \
    ./bench_malloc > results_hp_malloc.txt

# Comparer les performances
diff results_malloc.txt results_hp_malloc.txt
```

## 🎯 Avantage Principal

**✅ AUCUNE MODIFICATION DU CODE SOURCE NÉCESSAIRE !**

Le wrapper utilise la technique **LD_PRELOAD** pour intercepter les appels à malloc/free au niveau du linker dynamique, avant même que le programme ne démarre. Cela permet de :

- Tester différents allocateurs facilement
- Comparer les performances sans recompiler
- Apprendre les techniques d'interposition système
- Expérimenter avec les huge pages

## 🔧 Technique : LD_PRELOAD

```
Programme → malloc() → ???
                       ↓
          LD_PRELOAD intercepte
                       ↓
          libmalloc_wrapper.so
                       ↓
              hp_malloc()
                       ↓
            Huge Pages (2 Mo)
```

Le wrapper :
1. S'initialise en premier (via LD_PRELOAD)
2. Fournit ses propres implémentations de malloc/free
3. Ces implémentations appellent hp_malloc/hp_free
4. Le programme ne voit aucune différence !

## 📊 Fonctions Interceptées

- `malloc()` → `hp_malloc()`
- `free()` → `hp_free()`
- `calloc()` → `hp_malloc()` + initialisation à zéro
- `realloc()` → réallocation avec copie
- `posix_memalign()` → allocation alignée
- `aligned_alloc()` → allocation alignée
- `memalign()`, `valloc()`, `pvalloc()` → allocation alignée

## ⚠️ Limitations

1. **Capacité limitée** : 14 Mo max (configurable dans hp_allocator.c)
2. **Pas de free individuel** : allocateur simple, mémoire libérée en fin de programme
3. **Messages de debug** : plusieurs initialisations possibles (bibliothèques dynamiques)
4. **Fallback** : utilise malloc système si mémoire pleine

## 🧪 Tests

```bash
# Test rapide
./demo_wrapper.sh

# Test complet
./test_wrapper.sh

# Test manuel
./run_with_hp_malloc.sh ls -la
```

## 📖 Pour Aller Plus Loin

Consultez `README_WRAPPER.md` pour :
- Comprendre LD_PRELOAD en détail
- Gérer les appels récursifs
- Déboguer les problèmes d'interposition
- Exemples avancés
- Cas d'usage pédagogiques

## 🎓 Apprentissages

Ce projet couvre :
- ✅ Allocation mémoire avec huge pages Linux
- ✅ Création de bibliothèques dynamiques (.so)
- ✅ Technique LD_PRELOAD
- ✅ Interposition de fonctions système
- ✅ Gestion des symboles dynamiques (dlsym)
- ✅ Compilation avec flags -fPIC, -shared
- ✅ RPATH et LD_LIBRARY_PATH
- ✅ Analyse de performance mémoire
- ✅ Benchmarking et visualisation

## 🎯 Cas d'Usage

1. **Recherche** : Tester différents allocateurs sans recompiler
2. **Optimisation** : Comparer les performances malloc vs custom
3. **Pédagogie** : Comprendre l'allocation mémoire et les huge pages
4. **Débogage** : Tracer les allocations sans modifier le code
5. **Production** : Remplacer malloc par un allocateur optimisé

## 👨‍💻 Auteur

Projet réalisé dans le cadre du cours **Architecture OS - 2A ENSIIE**

**Date** : Octobre 2025

---

## 🚀 Quick Start

```bash
# 1. Compiler
cd SRC && make && make install && cd ..

# 2. Tester
./demo_wrapper.sh

# 3. Utiliser avec votre programme
./run_with_hp_malloc.sh ./votre_programme
```

**C'est parti ! 🎉**
