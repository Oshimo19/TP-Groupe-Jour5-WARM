#!/bin/bash

# Se placer dans le bon répertoire
cd ~/Distribution_Jour5_Pour_Etudiants/TP/ || { echo "Erreur : Impossible de se placer dans le répertoire TP/"; exit 1; }

# Créer un fichier de test
echo "HELLO WORLD 123" > tests/test.txt

# Compiler et exécuter tous les tests
echo -e "\n=== EXÉCUTION DES TESTS ==="

echo -e "\n=== Test du Module 1 (Scanner) ==="
gcc -I. -o tests/test_scanner tests/test_scanner.c modules.c
if [ $? -eq 0 ]; then
    ./tests/test_scanner sandbox/
else
    echo "Échec de la compilation du test Scanner"
fi

echo -e "\n=== Test du Module 2 (XOR) ==="
gcc -I. -o tests/test_xor tests/test_xor.c modules.c
if [ $? -eq 0 ]; then
    ./tests/test_xor
else
    echo "Échec de la compilation du test XOR"
fi

echo -e "\n=== Test du Module 3 (César) ==="
gcc -I. -o tests/test_caesar tests/test_caesar.c modules.c
if [ $? -eq 0 ]; then
    ./tests/test_caesar
else
    echo "Échec de la compilation du test César"
fi

echo -e "\n=== Test du Module 4 (ROT13) ==="
gcc -I. -o tests/test_rot13 tests/test_rot13.c modules.c
if [ $? -eq 0 ]; then
    ./tests/test_rot13
else
    echo "Échec de la compilation du test ROT13"
fi

echo -e "\n=== Test du Module 5 (Checksum) ==="
gcc -I. -o tests/test_checksum tests/test_checksum.c modules.c
if [ $? -eq 0 ]; then
    ./tests/test_checksum
else
    echo "Échec de la compilation du test Checksum"
fi

echo -e "\n=== Test du Module 6 (Timer) ==="
gcc -I. -o tests/test_timer tests/test_timer.c modules.c
if [ $? -eq 0 ]; then
    ./tests/test_timer 1
else
    echo "Échec de la compilation du test Timer"
fi

echo -e "\n=== Test du Module 7 (Config) ==="
gcc -I. -o tests/test_config tests/test_config.c modules.c
if [ $? -eq 0 ]; then
    echo "Test 1: Fichier autorisé (notes.md)"
    ./tests/test_config config.txt test_files/documents/notes.md
    echo -e "\nTest 2: Fichier bloqué (config.conf)"
    ./tests/test_config config.txt test_files/.hidden/config.conf
else
    echo "Échec de la compilation du test Config"
fi
