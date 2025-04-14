# Guide d'installation

Ce document fournit des instructions détaillées pour installer et configurer le projet d'Optimisation de Réseau de Fibres Optiques.

## Prérequis

- Compilateur GCC
- Bibliothèque mathématique (généralement incluse avec le système)
- Make (pour utiliser le Makefile)

## Installation sous Linux/macOS

1. Clonez le dépôt:
   ```bash
   git clone https://github.com/votre-utilisateur/optimisation-reseau-fibre.git
   cd optimisation-reseau-fibre
   ```

2. Compilez le projet:
   ```bash
   make all
   ```

   Cette commande va générer tous les exécutables nécessaires:
   - `ChaineMain` - Pour tester les chaînes
   - `ReconstitueReseau` - Pour reconstituer un réseau avec différentes méthodes
   - `main6.1` - Pour comparer les temps d'exécution (question 6.1)
   - `main` - Programme principal pour comparer les 3 méthodes
   - `mainGraphe` - Pour l'optimisation du réseau

## Installation sous Windows

### Avec MinGW/MSYS2

1. Assurez-vous d'avoir MinGW ou MSYS2 installé
2. Ouvrez le terminal MinGW/MSYS2
3. Clonez et compilez comme indiqué dans la section Linux/macOS

### Avec Visual Studio

1. Créez un nouveau projet C
2. Ajoutez tous les fichiers .c et .h au projet
3. Assurez-vous que la bibliothèque mathématique est liée
4. Compilez le projet

## Structure des répertoires

- `output/` - Dossier où seront stockés les fichiers de sortie
- Les fichiers d'entrée (.cha) doivent être placés à la racine du projet

## Vérification de l'installation

Pour vérifier que tout fonctionne correctement, exécutez le test sur un petit exemple:

```bash
./ReconstitueReseau 00014_burma.cha 3
```

Cela devrait générer les fichiers `testArbre.res` et `testArbre.html`. Vous pouvez ouvrir le fichier HTML dans un navigateur pour visualiser le réseau.

## Résolution des problèmes communs

- **Erreur de compilation "math.h non trouvé"**: Installez les paquets de développement de votre distribution
- **Erreur d'exécution "fichier non trouvé"**: Vérifiez que les fichiers d'entrée sont bien dans le bon répertoire
- **Problèmes de visualisation**: Assurez-vous que votre navigateur supporte SVG 