# Wrapper malloc/free → hp_malloc/hp_free

Ce wrapper permet d'intercepter automatiquement les appels à `malloc()` et `free()` pour les rediriger vers `hp_malloc()` et `hp_free()`, **sans modifier le code source** des programmes.

## 📋 Principe

Le wrapper utilise la technique **LD_PRELOAD** pour charger une bibliothèque dynamique avant toutes les autres. Cette bibliothèque fournit ses propres implémentations de `malloc`, `free`, `calloc`, `realloc`, etc., qui redirigent vers l'allocateur `hp_malloc`.

## 📁 Fichiers

- `malloc_wrapper.c` - Code source du wrapper
- `libmalloc_wrapper.so` - Bibliothèque dynamique compilée (dans `lib/`)
- `libhp_allocator.so` - Allocateur avec huge pages (dans `lib/`)
- `test_wrapper.sh` - Script de test complet
- `demo_wrapper.sh` - Démonstration rapide

## 🚀 Utilisation

### Option 1 : Avec LD_PRELOAD (recommandé)

```bash
# Définir les chemins
export LIB_DIR="/home/yhdh/etudes/2A/2A-/Architecture_OS/tps/tp_mémoire/ETUDIANT/hp_allocator/hp_allocator_malloc/lib"
export LD_LIBRARY_PATH="$LIB_DIR"
export LD_PRELOAD="$LIB_DIR/libmalloc_wrapper.so"

# Exécuter votre programme
./votre_programme

# Ou en une seule ligne :
LD_LIBRARY_PATH="$LIB_DIR" LD_PRELOAD="$LIB_DIR/libmalloc_wrapper.so" ./votre_programme
```

### Option 2 : Script wrapper

Créez un script `run_with_hp_malloc.sh` :

```bash
#!/bin/bash
LIB_DIR="/chemin/vers/lib"
LD_LIBRARY_PATH="$LIB_DIR" \
LD_PRELOAD="$LIB_DIR/libmalloc_wrapper.so" \
"$@"
```

Puis utilisez-le :
```bash
chmod +x run_with_hp_malloc.sh
./run_with_hp_malloc.sh ./votre_programme arg1 arg2
```

## 🎯 Exemple : Benchmark svalat_bench

```bash
cd /home/yhdh/etudes/2A/2A-/Architecture_OS/tps/tp_mémoire/ETUDIANT/svalat_bench

# Avec malloc standard
./bench_malloc > results_malloc.txt

# Avec hp_malloc (via wrapper)
LIB_DIR="../hp_allocator/hp_allocator_malloc/lib"
LD_LIBRARY_PATH="$LIB_DIR" \
LD_PRELOAD="$LIB_DIR/libmalloc_wrapper.so" \
./bench_malloc > results_hp_malloc_wrapped.txt
```

## ⚙️ Compilation

Le Makefile compile automatiquement le wrapper :

```bash
cd SRC
make              # Compile tout (hp_allocator + wrapper)
make install      # Copie dans lib/ et include/
make clean        # Nettoie les fichiers compilés
```

## 🔍 Fonctions interceptées

Le wrapper intercepte toutes les fonctions d'allocation standard :

- `malloc(size)` → `hp_malloc(size)`
- `free(ptr)` → `hp_free(ptr)`
- `calloc(n, size)` → allocation + initialisation à zéro
- `realloc(ptr, size)` → réallocation (avec copie)
- `posix_memalign()` → allocation alignée
- `aligned_alloc()` → allocation alignée
- `memalign()` → allocation alignée
- `valloc()` → allocation alignée sur page
- `pvalloc()` → allocation alignée sur page

## ⚠️ Limitations

1. **Taille limitée** : L'allocateur hp_malloc a une capacité de 14 Mo au total
2. **Pas de libération individuelle** : `hp_free()` ne libère pas vraiment la mémoire (allocateur simple)
3. **Initialisation multiple** : Chaque bibliothèque chargée peut initialiser le wrapper
4. **Fallback** : Si hp_malloc échoue, le wrapper utilise le malloc système en secours

## 🐛 Débogage

Pour voir les messages de debug :

```bash
LD_LIBRARY_PATH="$LIB_DIR" \
LD_PRELOAD="$LIB_DIR/libmalloc_wrapper.so" \
./votre_programme 2>&1 | grep -E "(WRAPPER|DEBUG)"
```

Messages affichés :
- `[WRAPPER] malloc/free interceptés...` - Le wrapper est actif
- `DEBUG: Huge pages allouées à...` - Les huge pages sont allouées

## 📊 Comparaison de performances

Pour comparer malloc standard vs hp_malloc :

```bash
# Script automatique
./demo_wrapper.sh

# Ou manuellement
time ./bench_malloc > results_standard.txt
time LD_PRELOAD="$LIB_DIR/libmalloc_wrapper.so" ./bench_malloc > results_wrapper.txt

# Comparer les résultats
diff results_standard.txt results_wrapper.txt
```

## 🔧 Configuration requise

Avant d'utiliser le wrapper, assurez-vous que les huge pages sont configurées :

```bash
sudo sh -c "echo 200 > /proc/sys/vm/nr_hugepages"
sudo mkdir -p /mnt/huge
sudo mount -t hugetlbfs -o mode=0777 none /mnt/huge
```

## 📖 Références

- **LD_PRELOAD** : Technique d'injection de bibliothèque dynamique
- **dlsym()** : Récupération des symboles de bibliothèques
- **Huge Pages** : Pages mémoire de grande taille (2 Mo ou 1 Go)

## 🎓 Cas d'usage pédagogiques

Ce wrapper est utile pour :
- Comparer les performances d'allocateurs différents
- Tester un allocateur custom sans modifier le code existant
- Apprendre les techniques d'interposition de bibliothèques
- Expérimenter avec les huge pages

## ✅ Tests

```bash
# Test rapide
./demo_wrapper.sh

# Test complet
./test_wrapper.sh
```

## 📝 Notes techniques

### Pourquoi LD_PRELOAD ?

`LD_PRELOAD` indique au linker dynamique de charger certaines bibliothèques avant toutes les autres. Ainsi, nos implémentations de `malloc/free` sont trouvées en premier et masquent celles de la libc.

### Gestion de l'initialisation

Le wrapper utilise un buffer temporaire pendant son initialisation pour éviter les appels récursifs infinis (malloc appelé pendant l'initialisation du wrapper).

### Fallback sur malloc système

Si hp_malloc retourne NULL (mémoire épuisée), le wrapper utilise le malloc système en secours pour éviter les crashs.

---

**Auteur** : Projet Architecture OS - 2A ENSIIE  
**Date** : Octobre 2025
