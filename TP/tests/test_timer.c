#include "../timer.h"
#include <stdio.h>
#include <stdlib.h>  // Ajout de stdlib.h pour atoi

int main(int argc, char *argv[]) {
    int delay = 3;
    if (argc > 1) {
        delay = atoi(argv[1]);
    }
    
    printf("=== Test Timer ===\n");

    printf("\nTest 1: Attendre %d secondes...\n", delay);
    long start = get_current_timestamp();
    wait_seconds(delay);
    long end = get_current_timestamp();
    printf("[+] Durée réelle: %ld secondes\n", end - start);

    printf("\nTest 2: Attendre jusqu'à un timestamp...\n");
    long target = get_current_timestamp() + 2;
    printf("Timestamp cible: %ld (dans 2 secondes)\n", target);
    wait_until(target);
    printf("[+] Cible atteinte !\n");

    printf("\nTest 3: Vérification de la précision...\n");
    start = get_current_timestamp();
    wait_seconds(1);
    end = get_current_timestamp();
    long elapsed = end - start;
    if (elapsed >= 1) {
        printf("[+] Précision acceptable: %ld seconde(s) écoulée(s)\n", elapsed);
    } else {
        printf("[-] Précision insuffisante: %ld seconde(s) écoulée(s)\n", elapsed);
    }

    printf("\nTest Timer réussi !\n");
    return 0;
}
