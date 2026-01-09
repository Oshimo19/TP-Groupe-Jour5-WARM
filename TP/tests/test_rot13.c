#include "../rot13_crypto.h"
#include <stdio.h>

int main() {
    printf("=== Test ROT13 ===\n");

    // Créer un fichier de test
    FILE *test_file = fopen("tests/test.txt", "w");
    if (!test_file) {
        printf("[-] Impossible de créer le fichier de test\n");
        return 1;
    }
    fprintf(test_file, "The Quick Brown Fox\n");
    fclose(test_file);

    printf("Contenu original (tests/test.txt) :\n");
    system("cat tests/test.txt");

    // Appliquer ROT13 une première fois
    printf("\n1. Première application de ROT13...\n");
    if (rot13_file("tests/test.txt", "tests/test.enc") != 0) {
        printf("[-] Échec du chiffrement\n");
        return 1;
    }
    printf("[+] Fichier chiffré\n");
    printf("Contenu après ROT13 (tests/test.enc) :\n");
    system("cat tests/test.enc");

    // Appliquer ROT13 une deuxième fois (déchiffrement)
    printf("\n2. Deuxième application de ROT13 (déchiffrement)...\n");
    if (rot13_file("tests/test.enc", "tests/test.dec") != 0) {
        printf("[-] Échec du déchiffrement\n");
        return 1;
    }
    printf("[+] Fichier déchiffré (même opération!)\n");
    printf("Contenu après double ROT13 (tests/test.dec) :\n");
    system("cat tests/test.dec");

    // Vérifier que le fichier est revenu à l'original
    printf("\nVérification des résultats...\n");
    if (system("diff tests/test.txt tests/test.dec > /dev/null") == 0) {
        printf("[+] Les fichiers originaux et déchiffrés sont identiques\n");
    } else {
        printf("[-] Les fichiers ne sont pas identiques\n");
        return 1;
    }

    printf("\nTest ROT13 réussi !\n");
    return 0;
}
