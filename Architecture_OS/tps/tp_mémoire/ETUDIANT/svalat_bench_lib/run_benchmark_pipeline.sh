#!/bin/bash
# Script complet pour exécuter le benchmark, nettoyer les données et générer les graphiques

set -e  # Arrêter en cas d'erreur

echo "=========================================="
echo "  Benchmark hp_malloc - Pipeline complet"
echo "=========================================="
echo ""

# 1. Exécuter le benchmark
echo "➤ Étape 1/3 : Exécution du benchmark..."
./bench_hp_malloc > results_hp_malloc.txt 2>&1
echo "  ✓ Résultats sauvegardés dans results_hp_malloc.txt"
echo ""

# 2. Nettoyer et convertir en CSV
echo "➤ Étape 2/3 : Conversion en CSV..."
/home/yhdh/etudes/2A/2A-/.venv/bin/python clean_and_convert_to_csv.py
echo ""

# 3. Générer les graphiques
echo "➤ Étape 3/3 : Génération des graphiques..."
/home/yhdh/etudes/2A/2A-/.venv/bin/python plot_benchmark.py
echo ""

echo "=========================================="
echo "  ✓ Pipeline terminé avec succès !"
echo "=========================================="
echo ""
echo "Fichiers générés :"
echo "  - results_hp_malloc.txt  : Résultats bruts"
echo "  - results_hp_malloc.csv  : Données nettoyées"
echo "  - benchmark_hp_malloc.png : Graphiques"
echo ""
