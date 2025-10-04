#!/usr/bin/env python3
"""
Script pour afficher un résumé rapide des résultats du benchmark
"""

import pandas as pd

def display_summary(csv_file):
    """
    Affiche un résumé des résultats du benchmark
    """
    df = pd.read_csv(csv_file)
    df = df[df['size_ko'] > 0]
    
    print("\n" + "="*60)
    print("  RÉSUMÉ DU BENCHMARK HP_MALLOC")
    print("="*60 + "\n")
    
    print(f"📊 Nombre total de mesures : {len(df)}")
    print(f"📏 Plage de tailles testées : {df['size_ko'].min()} Ko → {df['size_ko'].max()} Ko")
    print(f"⏱️  Temps d'accès : {df['time_ms'].min():.3f} ms → {df['time_ms'].max():.3f} ms")
    print(f"📈 Temps moyen global : {df['time_ms'].mean():.3f} ms")
    
    print("\n" + "-"*60)
    print("  PERFORMANCE PAR NIVEAU DE CACHE")
    print("-"*60 + "\n")
    
    ranges = [
        (0, 128, "🔹 L1 Cache (0-128 Ko)", "green"),
        (128, 512, "🔸 L2 Cache (128-512 Ko)", "yellow"),
        (512, 4096, "🔶 L3 Cache (512 Ko-4 Mo)", "orange"),
        (4096, 14336, "🔴 Huge Pages (4-14 Mo)", "red"),
    ]
    
    for start, end, label, _ in ranges:
        subset = df[(df['size_ko'] >= start) & (df['size_ko'] < end)]
        if len(subset) > 0:
            print(f"{label}")
            print(f"  • Temps moyen  : {subset['time_ms'].mean():6.3f} ms")
            print(f"  • Temps médian : {subset['time_ms'].median():6.3f} ms")
            print(f"  • Écart-type   : {subset['time_ms'].std():6.3f} ms")
            print(f"  • Min / Max    : {subset['time_ms'].min():6.3f} / {subset['time_ms'].max():6.3f} ms")
            print(f"  • Mesures      : {len(subset)}")
            print()
    
    print("-"*60)
    print("  POINTS REMARQUABLES")
    print("-"*60 + "\n")
    
    # Trouver les pics de latence
    worst_5 = df.nlargest(5, 'time_ms')
    print("⚠️  Top 5 des latences les plus élevées :")
    for idx, row in worst_5.iterrows():
        print(f"  • {row['size_ko']:5.0f} Ko → {row['time_ms']:6.3f} ms")
    
    print()
    
    # Trouver les meilleures performances
    best_5 = df.nsmallest(5, 'time_ms')
    print("✅ Top 5 des latences les plus faibles :")
    for idx, row in best_5.iterrows():
        print(f"  • {row['size_ko']:5.0f} Ko → {row['time_ms']:6.3f} ms")
    
    print("\n" + "="*60)
    print("  📁 Fichiers disponibles :")
    print("="*60)
    print("  • results_hp_malloc.txt  - Résultats bruts")
    print("  • results_hp_malloc.csv  - Données CSV")
    print("  • benchmark_hp_malloc.png - Graphiques")
    print("="*60 + "\n")

if __name__ == "__main__":
    csv_file = "results_hp_malloc.csv"
    
    try:
        display_summary(csv_file)
    except FileNotFoundError:
        print(f"❌ Erreur : Le fichier {csv_file} n'existe pas.")
        print("Veuillez d'abord exécuter : ./run_benchmark_pipeline.sh")
    except Exception as e:
        print(f"❌ Erreur : {e}")
