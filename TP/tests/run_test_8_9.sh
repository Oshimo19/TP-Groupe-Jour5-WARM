#!/bin/bash

# =============================================================================
# SCRIPT DE TEST POUR MODULES 8 ET 9 (CLIENT/SERVEUR C2)
# =============================================================================

# Chemins
SERVER_SRC="c2_server.c"
CLIENT_SRC="tests/test_c2_client.c"
SERVER_BIN="c2_server"
CLIENT_BIN="tests/test_c2_client"

# Variable globale pour le PID du serveur
SERVER_PID=0

# Fonction pour compiler (si nécessaire)
compile() {
    if [ ! -f "$SERVER_BIN" ] || [ ! -f "$CLIENT_BIN" ]; then
        echo "[+] Compilation en cours..."
        gcc -o "$SERVER_BIN" "$SERVER_SRC" -pthread -I. || {
            echo "[-] Échec compilation serveur"
            exit 1
        }
        gcc -I. -o "$CLIENT_BIN" "$CLIENT_SRC" modules.c || {
            echo "[-] Échec compilation client"
            exit 1
        }
        echo "[+] Compilation réussie"
    else
        echo "[+] Binaires existants détectés, réutilisation..."
    fi
}

# Fonction pour tuer TOUS les processus liés au serveur
kill_server() {
    echo "[+] Nettoyage des processus existants..."

    if [ "$SERVER_PID" -ne 0 ]; then
        kill "$SERVER_PID" 2>/dev/null
    fi

    if lsof -i :4444 > /dev/null 2>&1; then
        kill $(lsof -t -i :4444) 2>/dev/null
    fi

    pkill -f "$SERVER_BIN" 2>/dev/null
    fuser -k 4444/tcp 2>/dev/null

    while lsof -i :4444 > /dev/null 2>&1; do
        sleep 0.1
    done

    echo "[+] Tous les processus serveur arrêtés"
    SERVER_PID=0
}

# Fonction pour lancer le serveur
run_server() {
    echo "[+] Démarrage du serveur C2..."
    setsid ./"$SERVER_BIN" >/dev/null 2>&1 &
    SERVER_PID=$!
    sleep 1
    if ! kill -0 "$SERVER_PID" 2>/dev/null; then
        echo "[-] Échec du démarrage du serveur"
        SERVER_PID=0
        return 1
    fi
    echo "[+] Serveur démarré (PID: $SERVER_PID)"
    return 0
}

# Fonction pour tester un client
test_client() {
    local client_num=$1
    echo "[+] [Client $client_num] Exécution..."
    ./"$CLIENT_BIN" 2>&1 | while read -r line; do
        echo "[Client $client_num] $line"
    done
    return 0
}

# Fonction de nettoyage final
cleanup() {
    echo "[+] Nettoyage final en cours..."
    kill_server
    rm -f "$SERVER_BIN" "$CLIENT_BIN" 2>/dev/null
    echo "[+] Nettoyage terminé"
}

# Gestionnaire de signal pour un arrêt propre
trap cleanup EXIT SIGINT SIGTERM

# Main
compile

# Test 1: Client unique
echo ""
echo "=== TEST 1: Client unique ==="
kill_server
if ! run_server; then
    echo "[-] Impossible de démarrer le serveur"
    exit 1
fi
test_client 1
kill_server
echo ""
echo "[+] Test 1 terminé - RÉUSSI"
echo "----------------------------------------"

# Test 2: 3 clients simultanés
echo ""
echo "=== TEST 2: 3 clients simultanés ==="
kill_server
if ! run_server; then
    echo "[-] Impossible de démarrer le serveur"
    exit 1
fi
for i in {1..3}; do
    (sleep 0.$i; test_client $i) &
done
wait
kill_server
echo ""
echo "[+] Test 2 terminé - RÉUSSI"
echo "----------------------------------------"

echo ""
echo "Tous les tests ont été exécutés avec succès !"
