#ifndef C2_SERVER_H
#define C2_SERVER_H

// Démarre le serveur C2
int c2_server_start(int port);

// Arrête le serveur C2
void c2_server_stop(int sockfd);

#endif
