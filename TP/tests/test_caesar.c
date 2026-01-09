#include "../caesar_crypto.h"
#include <stdio.h>

int main() {
    printf("=== Test César ===\n");

    // Créer un fichier de test
    FILE *test_file = fopen("tests/test.txt", "w");
    if (!test_file) {
        printf("[-] Impossible de créer le fichier de test\n");
        return 1;
    }
    fprintf(test_file, "HELLO WORLD 123\n");
    fclose(test_file);

    printf("Contenu original (tests/test.txt) :\n");
    system("cat tests/test.txt");

    printf("Test 1: Chiffrement César (shift=3)...\n");
    if (caesar_encrypt_file("tests/test.txt", "tests/test.enc", 3) != 0) {
        printf("[-] Échec du chiffrement\n");
        return 1;
    }
    printf("[+] Fichier chiffré\n");
    printf("Contenu chiffré (tests/test.enc) :\n");
    system("cat tests/test.enc");

    printf("\nTest 2: Déchiffrement...\n");
    if (caesar_decrypt_file("tests/test.enc", "tests/test.dec", 3) != 0) {
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

    printf("\nTest César réussi !\n");
    return 0;
}
