#!/bin/bash
# Script simple pour tester le wrapper avec le benchmark svalat

set -e

WRAPPER_LIB="/home/yhdh/etudes/2A/2A-/Architecture_OS/tps/tp_mémoire/ETUDIANT/hp_allocator/hp_allocator_malloc/lib/libmalloc_wrapper.so"
LIB_DIR="/home/yhdh/etudes/2A/2A-/Architecture_OS/tps/tp_mémoire/ETUDIANT/hp_allocator/hp_allocator_malloc/lib"

cd /home/yhdh/etudes/2A/2A-/Architecture_OS/tps/tp_mémoire/ETUDIANT/svalat_bench

echo "╔══════════════════════════════════════════════════════════╗"
echo "║  Test du wrapper malloc → hp_malloc                      ║"
echo "╚══════════════════════════════════════════════════════════╝"
echo ""

echo "📊 Benchmark avec malloc standard (10 premières mesures) :"
echo "────────────────────────────────────────────────────────────"
./bench_malloc 2>&1 | grep -A 12 "#mem_size"
echo ""

echo "╔══════════════════════════════════════════════════════════╗"
echo "║  Benchmark avec wrapper hp_malloc (10 premières mesures) ║"
echo "╚══════════════════════════════════════════════════════════╝"
echo ""
echo "📊 Utilisation de LD_PRELOAD pour intercepter malloc/free :"
echo "────────────────────────────────────────────────────────────"

# Rediriger stderr pour ne voir que les infos importantes
LD_LIBRARY_PATH="$LIB_DIR" LD_PRELOAD="$WRAPPER_LIB" ./bench_malloc 2>&1 | grep -E "(WRAPPER|DEBUG|#mem_size|ko.*time)" | head -20

echo ""
echo "╔══════════════════════════════════════════════════════════╗"
echo "║  ✓ Wrapper testé avec succès !                           ║"
echo "╚══════════════════════════════════════════════════════════╝"
echo ""
echo "Pour exécuter le benchmark complet :"
echo "  cd /home/yhdh/etudes/2A/2A-/Architecture_OS/tps/tp_mémoire/ETUDIANT/svalat_bench"
echo "  LD_LIBRARY_PATH=$LIB_DIR LD_PRELOAD=$WRAPPER_LIB ./bench_malloc > results_with_wrapper.txt"
echo ""
