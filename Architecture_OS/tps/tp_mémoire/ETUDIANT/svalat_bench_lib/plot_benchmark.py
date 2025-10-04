#!/usr/bin/env python3
"""
Script pour visualiser les performances du benchmark hp_malloc
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def plot_benchmark_results(csv_file):
    """
    Lit le fichier CSV et crée des graphiques de performance
    """
    # Lire les données
    df = pd.read_csv(csv_file)
    
    # Filtrer les données nulles
    df = df[df['size_ko'] > 0]
    
    print(f"✓ Données chargées : {len(df)} points")
    print(f"  Taille min : {df['size_ko'].min()} Ko")
    print(f"  Taille max : {df['size_ko'].max()} Ko")
    print(f"  Temps min : {df['time_ms'].min():.3f} ms")
    print(f"  Temps max : {df['time_ms'].max():.3f} ms")
    
    # Créer une figure avec 2 sous-graphiques
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(14, 10))
    
    # Graphique 1 : Temps d'accès en fonction de la taille (échelle linéaire)
    ax1.plot(df['size_ko'], df['time_ms'], 'b-', linewidth=1, alpha=0.7)
    ax1.scatter(df['size_ko'], df['time_ms'], c='blue', s=10, alpha=0.3)
    ax1.set_xlabel('Taille du buffer (Ko)', fontsize=12)
    ax1.set_ylabel('Temps d\'accès (ms)', fontsize=12)
    ax1.set_title('Performance du hp_malloc - Temps d\'accès mémoire', fontsize=14, fontweight='bold')
    ax1.grid(True, alpha=0.3)
    ax1.set_xlim(0, df['size_ko'].max())
    
    # Ajouter les seuils de cache (approximatifs)
    cache_levels = [
        (128, 'L1 Cache (~128 Ko)', 'red'),
        (512, 'L2 Cache (~512 Ko)', 'orange'),
        (4096, 'L3 Cache (~4 Mo)', 'green'),
        (14336, 'Huge Page (14 Mo)', 'purple')
    ]
    
    for level, label, color in cache_levels:
        if level <= df['size_ko'].max():
            ax1.axvline(x=level, color=color, linestyle='--', linewidth=2, alpha=0.6, label=label)
    
    ax1.legend(loc='upper left', fontsize=10)
    
    # Graphique 2 : Échelle log-log pour voir les tendances
    ax2.loglog(df['size_ko'], df['time_ms'], 'g-', linewidth=1, alpha=0.7)
    ax2.scatter(df['size_ko'], df['time_ms'], c='green', s=10, alpha=0.3)
    ax2.set_xlabel('Taille du buffer (Ko) - Échelle log', fontsize=12)
    ax2.set_ylabel('Temps d\'accès (ms) - Échelle log', fontsize=12)
    ax2.set_title('Performance du hp_malloc - Échelle logarithmique', fontsize=14, fontweight='bold')
    ax2.grid(True, alpha=0.3, which='both')
    
    # Ajouter les seuils de cache
    for level, label, color in cache_levels:
        if level <= df['size_ko'].max():
            ax2.axvline(x=level, color=color, linestyle='--', linewidth=2, alpha=0.6, label=label)
    
    ax2.legend(loc='upper left', fontsize=10)
    
    plt.tight_layout()
    
    # Sauvegarder le graphique
    output_file = 'benchmark_hp_malloc.png'
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"\n✓ Graphique sauvegardé : {output_file}")
    
    # Afficher le graphique
    plt.show()
    
    # Statistiques supplémentaires
    print("\n=== Statistiques par plage de taille ===")
    ranges = [
        (0, 128, "0-128 Ko (L1 Cache)"),
        (128, 512, "128-512 Ko (L2 Cache)"),
        (512, 4096, "512-4096 Ko (L3 Cache)"),
        (4096, 14336, "4-14 Mo (Huge Page)"),
        (14336, df['size_ko'].max(), ">14 Mo")
    ]
    
    for start, end, label in ranges:
        subset = df[(df['size_ko'] >= start) & (df['size_ko'] < end)]
        if len(subset) > 0:
            print(f"\n{label}:")
            print(f"  Temps moyen : {subset['time_ms'].mean():.3f} ms")
            print(f"  Temps médian : {subset['time_ms'].median():.3f} ms")
            print(f"  Écart-type : {subset['time_ms'].std():.3f} ms")
            print(f"  Nombre de points : {len(subset)}")

if __name__ == "__main__":
    csv_file = "results_hp_malloc.csv"
    
    try:
        plot_benchmark_results(csv_file)
    except FileNotFoundError:
        print(f"Erreur : Le fichier {csv_file} n'existe pas.")
        print("Veuillez d'abord exécuter le script clean_and_convert_to_csv.py")
    except Exception as e:
        print(f"Erreur : {e}")
