#include "../config.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <config_file> <filepath>\n", argv[0]);
        return 1;
    }

    Config *cfg = load_config(argv[1]);
    if (!cfg) {
        printf("❌ Erreur chargement config\n");
        return 1;
    }

    printf("Whitelist (%d entrées): ", cfg->whitelist_count);
    for (int i = 0; i < cfg->whitelist_count; i++) {
        printf("%s ", cfg->whitelist[i]);
    }
    printf("\n");

    printf("Blacklist (%d entrées): ", cfg->blacklist_count);
    for (int i = 0; i < cfg->blacklist_count; i++) {
        printf("%s ", cfg->blacklist[i]);
    }
    printf("\n\n");

    if (is_allowed(cfg, argv[2])) {
        printf("✓ Autorisé: %s\n", argv[2]);
    } else {
        printf("✗ Bloqué: %s\n", argv[2]);
    }

    free_config(cfg);
    return 0;
}
