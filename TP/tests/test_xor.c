#include "../xor_crypto.h"
#include <stdio.h>
#include <string.h>

int main() {
    const char *key = "MY_SECRET_KEY";

    printf("=== Test XOR ===\n");

    // Créer un fichier de test avec du contenu
    FILE *test_file = fopen("tests/test.txt", "w");
    if (!test_file) {
        printf("[-] Impossible de créer le fichier de test\n");
        return 1;
    }
    fprintf(test_file, "HELLO WORLD 123\n");
    fclose(test_file);

    printf("Contenu original (tests/test.txt) :\n");
    system("cat tests/test.txt");

    printf("Test 1: Chiffrement...\n");
    if (xor_encrypt_file("tests/test.txt", "tests/test.enc", key) != 0) {
        printf("[-] Échec du chiffrement\n");
        return 1;
    }
    printf("[+] Fichier chiffré\n");
    printf("Contenu chiffré (tests/test.enc) :\n");
    system("xxd tests/test.enc | head -n 5");

    printf("\nTest 2: Déchiffrement...\n");
    if (xor_decrypt_file("tests/test.enc", "tests/test.dec", key) != 0) {
        printf("[-] Échec du déchiffrement\n");
        return 1;
    }
    printf("[+] Fichier déchiffré\n");
    printf("Contenu déchiffré (tests/test.dec) :\n");
    system("cat tests/test.dec");

    printf("\nVérification des résultats...\n");
    if (system("diff tests/test.txt tests/test.dec > /dev/null") == 0) {
        printf("[+] Les fichiers originaux et déchiffrés sont identiques\n");
    } else {
        printf("[-] Les fichiers ne sont pas identiques\n");
        return 1;
    }

    printf("\nTest XOR réussi !\n");
    return 0;
}
