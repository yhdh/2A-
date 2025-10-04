#!/bin/bash
#
# Script utilitaire pour exécuter n'importe quel programme avec hp_malloc
# via le wrapper LD_PRELOAD
#
# Usage: ./run_with_hp_malloc.sh <programme> [arguments...]
#

# Déterminer le chemin absolu du script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LIB_DIR="$SCRIPT_DIR/lib"

# Vérifier que les bibliothèques existent
if [ ! -f "$LIB_DIR/libmalloc_wrapper.so" ]; then
    echo "❌ Erreur : libmalloc_wrapper.so n'existe pas dans $LIB_DIR"
    echo "   Compilez d'abord avec: cd SRC && make && make install"
    exit 1
fi

if [ ! -f "$LIB_DIR/libhp_allocator.so" ]; then
    echo "❌ Erreur : libhp_allocator.so n'existe pas dans $LIB_DIR"
    echo "   Compilez d'abord avec: cd SRC && make && make install"
    exit 1
fi

# Vérifier qu'un programme a été fourni
if [ $# -eq 0 ]; then
    echo "Usage: $0 <programme> [arguments...]"
    echo ""
    echo "Exemples:"
    echo "  $0 ./bench_malloc"
    echo "  $0 ./my_program arg1 arg2"
    echo "  $0 ls -la"
    echo ""
    echo "Le programme sera exécuté avec hp_malloc au lieu de malloc."
    exit 1
fi

# Exécuter le programme avec LD_PRELOAD
echo "🚀 Exécution de: $@"
echo "📚 Avec hp_malloc (LD_PRELOAD)"
echo ""

LD_LIBRARY_PATH="$LIB_DIR:$LD_LIBRARY_PATH" \
LD_PRELOAD="$LIB_DIR/libmalloc_wrapper.so" \
"$@"

exit_code=$?

echo ""
echo "✓ Programme terminé avec le code: $exit_code"
exit $exit_code
