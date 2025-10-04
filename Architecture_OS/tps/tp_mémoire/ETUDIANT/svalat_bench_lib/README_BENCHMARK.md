# Benchmark hp_malloc - Analyse de Performance

Ce dossier contient un benchmark pour analyser les performances de l'allocateur mémoire `hp_malloc` utilisant les huge pages.

## Fichiers

### Exécutables
- `bench_hp_malloc` : Exécutable du benchmark (compilé avec libhp_allocator.so)
- `run_benchmark_pipeline.sh` : Script bash automatique pour exécuter tout le pipeline

### Scripts Python
- `clean_and_convert_to_csv.py` : Nettoie les résultats bruts et les convertit en CSV
- `plot_benchmark.py` : Génère des graphiques de performance à partir du CSV

### Résultats
- `results_hp_malloc.txt` : Résultats bruts du benchmark
- `results_hp_malloc.csv` : Données nettoyées au format CSV
- `benchmark_hp_malloc.png` : Graphiques de performance (2 vues : linéaire et logarithmique)

## Utilisation

### Option 1 : Pipeline automatique (recommandé)
```bash
./run_benchmark_pipeline.sh
```

### Option 2 : Étape par étape
```bash
# 1. Exécuter le benchmark
./bench_hp_malloc > results_hp_malloc.txt 2>&1

# 2. Convertir en CSV
python3 clean_and_convert_to_csv.py

# 3. Générer les graphiques
python3 plot_benchmark.py
```

## Dépendances Python

Les packages Python suivants sont requis :
- pandas
- matplotlib
- numpy

Installation :
```bash
pip install pandas matplotlib numpy
```

## Résultats

Le benchmark teste l'allocation mémoire pour différentes tailles de buffer (de 1 Ko à ~14 Mo) et mesure le temps d'accès mémoire.

### Statistiques observées

**0-128 Ko (L1 Cache)**
- Temps moyen : ~7.1 ms
- Temps médian : ~6.6 ms

**128-512 Ko (L2 Cache)**
- Temps moyen : ~11.7 ms
- Temps médian : ~12.2 ms

**512-4096 Ko (L3 Cache)**
- Temps moyen : ~12.5 ms
- Temps médian : ~12.3 ms

**4-14 Mo (Huge Page)**
- Temps moyen : ~15.4 ms
- Temps médian : ~13.7 ms

### Observations

- Les temps d'accès augmentent avec la taille des buffers
- On observe des pics de latence aux transitions entre niveaux de cache
- Les huge pages permettent une allocation efficace pour les gros buffers (>4 Mo)
- La variance augmente pour les très gros buffers en raison des page faults

## Architecture du système

- **Architecture** : aarch64
- **OS** : Linux (kernel 6.1.0-25-arm64)
- **Allocateur** : hp_malloc avec huge pages (libhp_allocator.so)

## Configuration requise

Avant d'exécuter le benchmark, assurez-vous que les huge pages sont configurées :

```bash
# Allouer des huge pages (nécessite root)
sudo sh -c "echo 200 > /proc/sys/vm/nr_hugepages"

# Créer le point de montage
sudo mkdir -p /mnt/huge

# Monter le système de fichiers hugetlbfs
sudo mount -t hugetlbfs -o mode=0777 none /mnt/huge
```

## Liens avec la bibliothèque

Le benchmark utilise la bibliothèque dynamique `libhp_allocator.so` située dans :
```
../hp_allocator/hp_allocator_malloc/lib/libhp_allocator.so
```

## Auteur

Projet réalisé dans le cadre du cours d'Architecture OS - 2A ENSIIE
