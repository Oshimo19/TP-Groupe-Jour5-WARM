// ============================================================================
// MODULE 9 : SERVEUR C2 (GESTION MULTI-CLIENTS AVEC THREADS)
// ============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "c2_client.h"  // Pour C2Command et C2Response

#define MAX_CLIENTS 10
#define BACKLOG 5

// Traite un client dans un thread séparé
void* handle_client(void *arg) {
    int client_sock = *(int*)arg;
    free(arg);  // Libérer la mémoire allouée pour le socket

    C2Command cmd;
    C2Response resp;

    printf("[C2] Client connecté\n");

    while (1) {
        // Recevoir commande
        ssize_t received = recv(client_sock, &cmd, sizeof(cmd), 0);
        if (received <= 0) break;  // Client déconnecté

        // Initialiser la réponse
        memset(&resp, 0, sizeof(resp));
        resp.status = 0;

        // Traiter selon cmd.command
        if (strcmp(cmd.command, "STATUS") == 0) {
            strcpy(resp.message, "Server OK");
        }
        else if (strcmp(cmd.command, "ENCRYPT") == 0) {
            snprintf(resp.message, sizeof(resp.message),
                     "Encrypted %s with mode %d", cmd.target, cmd.mode);
        }
        else if (strcmp(cmd.command, "DECRYPT") == 0) {
            snprintf(resp.message, sizeof(resp.message),
                     "Decrypted %s with mode %d", cmd.target, cmd.mode);
        }
        else {
            strcpy(resp.message, "Unknown command");
            resp.status = -1;
        }

        // Envoyer réponse
        send(client_sock, &resp, sizeof(resp), 0);
    }

    printf("[C2] Client déconnecté\n");
    close(client_sock);
    return NULL;
}

int main(int argc, char *argv[]) {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Créer socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Configuration serveur
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Écoute sur toutes les interfaces
    server_addr.sin_port = htons(C2_SERVER_PORT);

    // Bind
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // Listen
    if (listen(server_sock, BACKLOG) < 0) {
        perror("Listen failed");
        return 1;
    }

    printf("[C2] Server listening on port %d\n", C2_SERVER_PORT);

    // Boucle principale : accepter les clients
    while (1) {
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_len);
        if (client_sock < 0) continue;

        // Créer un thread par client
        pthread_t thread;
        int *sock_ptr = malloc(sizeof(int));
        *sock_ptr = client_sock;

        if (pthread_create(&thread, NULL, handle_client, sock_ptr) != 0) {
            perror("pthread_create failed");
            close(client_sock);
            free(sock_ptr);
            continue;
        }

        pthread_detach(thread);  // Thread autonome (ne pas attendre join)
    }

    close(server_sock);
    return 0;
}
