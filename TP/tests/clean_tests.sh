#!/bin/bash

# Script simplifié pour nettoyer les fichiers de test et les binaires

echo -e "\n=== NETTOYAGE DES FICHIERS DE TEST ==="

# Supprimer les fichiers de test
rm -f test.txt test.enc test.dec 2>/dev/null

# Supprimer les binaires (tous les fichiers exécutables sauf les scripts .sh et .c)
find . -maxdepth 1 -type f -executable ! -name "*.sh" ! -name "*.c" -exec rm -f {} \; 2>/dev/null

echo "[+] Nettoyage terminé"
