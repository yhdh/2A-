#!/usr/bin/env python3
"""
Script pour nettoyer les résultats du benchmark et les convertir en CSV
"""

import re
import csv

def clean_and_convert(input_file, output_file):
    """
    Lit le fichier de résultats, extrait les données valides et les écrit en CSV
    """
    data = []
    
    with open(input_file, 'r') as f:
        for line in f:
            line = line.strip()
            
            # Ignorer les lignes de commentaires et les lignes vides
            if line.startswith('#') or line.startswith('____') or not line:
                continue
            
            # Parser les lignes de données
            # Format attendu: "123 ko   15744   0.007970        0"
            parts = line.split()
            if len(parts) >= 4:
                try:
                    # Extraire la taille en ko (premier nombre)
                    size_ko = int(parts[0])
                    # Extraire le nombre de doubles
                    nb_doubles = int(parts[2])
                    # Extraire le temps
                    time = float(parts[3])
                    
                    data.append({
                        'size_ko': size_ko,
                        'nb_doubles': nb_doubles,
                        'time_ms': time * 1000  # Convertir en millisecondes
                    })
                except (ValueError, IndexError):
                    # Ignorer les lignes mal formées
                    continue
    
    # Écrire en CSV
    with open(output_file, 'w', newline='') as f:
        if data:
            fieldnames = ['size_ko', 'nb_doubles', 'time_ms']
            writer = csv.DictWriter(f, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(data)
    
    print(f"✓ Fichier CSV créé : {output_file}")
    print(f"✓ Nombre de lignes de données : {len(data)}")
    return len(data)

if __name__ == "__main__":
    input_file = "results_hp_malloc.txt"
    output_file = "results_hp_malloc.csv"
    
    count = clean_and_convert(input_file, output_file)
    
    if count > 0:
        print(f"\nAperçu des premières lignes :")
        with open(output_file, 'r') as f:
            for i, line in enumerate(f):
                if i < 10:
                    print(line.strip())
