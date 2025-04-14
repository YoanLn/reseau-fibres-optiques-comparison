# Projet d'Optimisation de Réseau de Fibres Optiques

Ce projet a pour objectif d'optimiser la distribution des services de connectivité dans un réseau de fibres optiques d'une agglomération. Il s'articule autour de deux parties principales:

1. **Reconstitution du réseau** - Reconstruction numérique du réseau existant
2. **Réorganisation du réseau** - Optimisation de l'attribution des fibres optiques entre opérateurs

## Description du projet

Dans ce projet, nous modélisons un réseau de fibres optiques où:
- Un réseau est un ensemble de câbles, chacun contenant γ fibres optiques
- Ces câbles relient des clients et des concentrateurs
- Une chaîne de fibres optiques relie deux points du plan (une commodité)

L'objectif est de comparer différentes structures de données pour stocker et manipuler efficacement ces réseaux, puis de réorganiser l'attribution des fibres pour minimiser la longueur totale des chaînes.

## Structures de données implémentées

Trois méthodes de stockage sont comparées:

1. **Liste chaînée** - Méthode simple mais moins efficace pour des réseaux de grande taille
2. **Table de hachage** - Méthode avec fonction de hachage pour accélérer la recherche
3. **Arbre quaternaire** - Méthode de partitionnement spatial de l'espace 2D

## Principaux fichiers source

- `Chaine.c/h` - Implémentation des chaînes de points
- `Reseau.c/h` - Implémentation de la structure de réseau
- `Hachage.c/h` - Implémentation de la table de hachage
- `ArbreQuat.c/h` - Implémentation de l'arbre quaternaire
- `Graphe.c/h` - Implémentation du graphe pour l'optimisation
- `SVGwriter.c/h` - Fonctions pour la visualisation des réseaux
- `Struct_File.c/h` - Structure de file pour les parcours en largeur
- `ReconstitueReseau.c` - Programme principal pour la reconstitution de réseau
- `main.c` - Programme de test pour comparer les performances
- `mainGraph.c` - Programme pour l'optimisation du réseau

## Résultats des comparaisons

Après tests sur des réseaux de différentes tailles, nous avons constaté que:

- La **liste chaînée** est significativement plus lente (> 5 secondes) que les autres méthodes
- La **table de hachage** offre de bonnes performances (0,04-0,08 secondes) avec une taille adaptée
- L'**arbre quaternaire** s'avère être la méthode la plus efficace (0,02-0,04 secondes)

Pour la table de hachage, une taille plus grande réduit les collisions et améliore les performances pour les réseaux complexes.

## Comment compiler et exécuter

### Reconstitution du réseau
```bash
gcc -Wall -o ReconstitueReseau ReconstitueReseau.c Hachage.c Reseau.c ArbreQuat.c Chaine.c SVGwriter.c -lm
./ReconstitueReseau fichier.cha [1-3]
```
Options:
- 1: Liste chaînée
- 2: Table de hachage
- 3: Arbre quaternaire

### Comparaison des performances
```bash
gcc -Wall -o main main.c ArbreQuat.c Reseau.c Hachage.c Chaine.c SVGwriter.c -lm
./main
```

### Optimisation du réseau
```bash
gcc -Wall -o mainGraphe mainGraph.c Graphe.c Chaine.c Reseau.c SVGwriter.c Struct_File.c -lm
./mainGraphe fichier.cha
```

## Visualisation des résultats

Les fichiers .res générés peuvent être convertis en .html (SVG) pour visualiser les réseaux graphiquement:
```bash
./ReconstitueReseau fichier.cha [1-3]
```
Cette commande génère un fichier .res et un fichier .html correspondant.

## Auteurs

- Yoan LE NEVEZ
- Anh Tu NGUYEN

## Améliorations possibles

- Réduire la complexité de la fonction reorganiseReseau (actuellement O(n²))
- Utiliser une table de hachage ou un arbre quaternaire plutôt qu'une matrice pour stocker le graphe
- Implémenter d'autres algorithmes d'optimisation pour minimiser davantage la longueur des chaînes 