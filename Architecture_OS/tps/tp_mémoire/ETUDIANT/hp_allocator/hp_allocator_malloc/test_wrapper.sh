#!/bin/bash
# Script pour tester le wrapper malloc avec le benchmark svalat_bench

set -e

echo "=========================================="
echo "  Test du wrapper malloc/free"
echo "=========================================="
echo ""

# Chemin vers la bibliothèque wrapper
WRAPPER_LIB="/home/yhdh/etudes/2A/2A-/Architecture_OS/tps/tp_mémoire/ETUDIANT/hp_allocator/hp_allocator_malloc/lib/libmalloc_wrapper.so"
LIB_DIR="/home/yhdh/etudes/2A/2A-/Architecture_OS/tps/tp_mémoire/ETUDIANT/hp_allocator/hp_allocator_malloc/lib"

# Vérifier que la bibliothèque existe
if [ ! -f "$WRAPPER_LIB" ]; then
    echo "❌ Erreur : La bibliothèque wrapper n'existe pas : $WRAPPER_LIB"
    exit 1
fi

echo "✓ Bibliothèque wrapper trouvée : $WRAPPER_LIB"
echo "✓ Répertoire des bibliothèques : $LIB_DIR"
echo ""

# Aller dans le répertoire du benchmark
cd /home/yhdh/etudes/2A/2A-/Architecture_OS/tps/tp_mémoire/ETUDIANT/svalat_bench

echo "➤ Exécution du benchmark avec malloc standard..."
echo ""
./bench_malloc 2>&1 | head -20
echo ""
echo "..."
echo ""

echo "=========================================="
echo "➤ Exécution du benchmark avec wrapper (hp_malloc)..."
echo ""
LD_LIBRARY_PATH="$LIB_DIR" LD_PRELOAD="$WRAPPER_LIB" ./bench_malloc 2>&1 | head -25
echo ""
echo "..."
echo ""

echo "=========================================="
echo "✓ Test terminé !"
echo "=========================================="
echo ""
echo "Pour exécuter le benchmark complet avec le wrapper :"
echo "  LD_LIBRARY_PATH=$LIB_DIR LD_PRELOAD=$WRAPPER_LIB ./bench_malloc"
echo ""
