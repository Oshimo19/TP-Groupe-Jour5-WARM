#include "../scanner.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Fonction : print_usage
 * Description : Affiche l'utilisation du programme
 */
void print_usage(const char *program_name) {
    printf("Usage: %s <directory>\n", program_name);
    printf("Exemple: %s sandbox/\n", program_name);
}

/*
 * Fonction : main
 * Description : Point d'entrée du programme de test
 */
int main(int argc, char *argv[]) {
    // Vérifier le nombre d'arguments
    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }

    printf("=== Test Scanner ===\n");

    // Afficher le répertoire à scanner
    printf("Scanning directory: %s\n", argv[1]);

    // Allouer dynamiquement le tableau de fichiers
    char (*files)[MAX_PATH] = malloc(MAX_FILES * sizeof(*files));
    if (!files) {
        printf("[-] Erreur d'allocation mémoire\n");
        return 1;
    }

    // Scanner le répertoire
    int count = scan_recursive(argv[1], files, MAX_FILES, 0);

    // Afficher les résultats
    printf("[+] %d fichiers trouvés\n", count);

    // Afficher les 10 premiers fichiers
    for (int i = 0; i < count && i < 10; i++) {
        printf("  [%d] %s\n", i+1, files[i]);
    }

    // Afficher un message si plus de 10 fichiers
    if (count > 10) {
        printf("  ... et %d autres\n", count - 10);
    }

    // Libérer la mémoire
    free(files);

    printf("\nTest Scanner réussi !\n");
    return 0;
}
