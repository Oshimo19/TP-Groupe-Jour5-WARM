# Ransomware Éducatif – Tests des Modules

**TP Sécurité Offensive – Jour 5 (Janvier 2026)**  
**Espace de Michel WU**

---

## Organisation du Projet

### Auteurs

- **Walid HOUBABI** — Modules 1 à 8
- **Antoine NYEE MACKI** — Modules 1 à 8
- **Rayane SLIMANI** — Modules 1 à 9
- **Michel WU** — Modules 1 à 9 et rédaction du README.md

### Organisation du travail

> Nous avons essayé, dans un premier temps, de développer **chaque module individuellement**, afin que chacun puisse comprendre et implémenter l’ensemble des fonctionnalités.  

> Lorsque l’un d’entre nous rencontrait une difficulté (notamment pour les **modules 8 et 9 – Communication C2**), nous nous sommes **entraidés** pour débloquer la situation et finaliser l’implémentation.  

> Cette méthode nous a permis de valider les modules un par un avant leur intégration dans le programme principal.

---

## Objectif

Ce projet est un **ransomware éducatif** dont le but est de tester et comprendre 
les principaux mécanismes techniques vus pendant le TP :

- Scanner de fichiers
- Chiffrement (XOR, César, ROT13)
- Checksum (CRC32)
- Timer
- Chargement de configuration
- Communication C2 (client / serveur)

Chaque module dispose :
- soit d’un **script bash de test automatisé**
- soit d’un **test via le menu interactif** du programme principal

---

## Structure du Projet

```bash
TP-Groupe-Jour5-WARM/
└── TP/
    ├── ransomware            # Binaire principal
    ├── ransomware.c          # Programme principal (menu interactif)
    ├── modules.c             # Implémentation des modules
    ├── c2_server.c           # Serveur C2
    ├── sandbox/              # Environnement de test (généré par le script "setup_sandbox.sh")
    ├── tests/                # Scripts et tests unitaires
    │   ├── test_*.c          # Tests unitaires modules 1 à 7
    │   ├── run_tests.sh      # Lance tous les tests modules 1 à 7
    │   └── run_test_8_9.sh   # Test communication C2
    └── setup_sandbox.sh      # Génération du sandbox
```

---

## Clonage de l’Environnement

Cloner le dépôt du TP :

```bash
git clone https://github.com/Oshimo19/TP-Groupe-Jour5-WARM.git
```

Se placer dans le dossier du projet :

```bash
cd TP-Groupe-Jour5-WARM/
```

---

## Environnement de Test

Générer le dossier sandbox :

```bash
bash setup_sandbox.sh
```

Contient :

* fichiers texte
* fichiers binaires
* données sensibles simulées

---

## Compilation

### Compiler le ransomware

```bash
gcc -I. -o ransomware ransomware.c modules.c
```

### Compiler le serveur C2

```bash
gcc -pthread -I. -o c2_server c2_server.c
```

---

## Tests Automatisés (Recommandé)

### Modules 1 à 7

Lance tous les tests unitaires automatiquement :

```bash
bash tests/run_tests.sh
```

Modules testés :

* XOR
* César
* ROT13
* Scanner
* Checksum
* Timer
* Configuration

---

### Modules 8–9 : Communication C2

```bash
bash tests/run_test_8_9.sh
```

Ce script :

* Lance le serveur C2
* Test la connexion client
* Vérifie l’échange de commandes

---

## Tests Manuels (Menu Interactif)

Pour tester manuellement un module :

### [1] Compiler le programme

```bash
gcc -I. -o ransomware ransomware.c modules.c
```

### [2] Lancer le ransomware

```bash
./ransomware
```

### [3] Suivre les instructions à l’écran

Un menu interactif s’affiche :

```
1. Scanner un répertoire
2. Chiffrer XOR
3. Chiffrer César
4. Chiffrer ROT13
5. Calculer checksum
6. Tester timer
7. Charger config
8. Tester C2 client
0. Quitter
```

> Chaque option demande les paramètres nécessaires (fichier, clé, dossier, etc.)
> Il suffit de **suivre ce qui est affiché à l’écran**

---

## Avertissement

> Projet **strictement éducatif** réalisé dans le cadre d’un TP.
> Toute utilisation malveillante est interdite.

© 2026 — Espace de Michel WU
*Apprendre la sécurité par la pratique pour mieux se défendre.*

---
