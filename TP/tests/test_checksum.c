#include "../checksum.h"
#include <stdio.h>

int main() {
    const char *file = "tests/test.txt";

    printf("=== Test Checksum ===\n");

    // Créer un fichier de test
    FILE *test_file = fopen(file, "w");
    if (!test_file) {
        printf("[-] Impossible de créer le fichier de test\n");
        return 1;
    }
    fprintf(test_file, "test data for checksum calculation\n");
    fclose(test_file);

    printf("Contenu du fichier (tests/test.txt) :\n");
    system("cat tests/test.txt");

    printf("\n1. Calcul du CRC32...\n");
    uint32_t crc = calculate_crc32(file);
    printf("CRC32 calculé: 0x%08X\n", crc);

    printf("\n2. Vérification d'intégrité...\n");
    if (verify_integrity(file, crc) == 0) {
        printf("[+] Fichier intègre\n");
    } else {
        printf("[-] Fichier modifié !\n");
        return 1;
    }

    printf("\n3. Test de détection de modification...\n");
    test_file = fopen(file, "a");
    fprintf(test_file, "modification\n");
    fclose(test_file);

    printf("Fichier modifié (tests/test.txt) :\n");
    system("cat tests/test.txt");

    if (verify_integrity(file, crc) != 0) {
        printf("[+] Modification détectée avec succès\n");
    } else {
        printf("[-] Modification non détectée\n");
        return 1;
    }

    printf("\nTest Checksum réussi !\n");
    return 0;
}
