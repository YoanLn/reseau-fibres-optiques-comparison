# Contribuer au projet d'Optimisation de Réseau de Fibres Optiques

Nous sommes ravis que vous envisagiez de contribuer à notre projet ! Voici quelques lignes directrices pour vous aider.

## Comment contribuer

1. **Forker le dépôt** - Commencez par créer votre propre copie du projet
2. **Créer une branche** - Pour chaque nouvelle fonctionnalité ou correction
3. **Faire vos modifications** - N'oubliez pas de documenter votre code
4. **Tester vos modifications** - Assurez-vous que votre code fonctionne et ne génère pas de fuites mémoire
5. **Soumettre une Pull Request** - Décrivez clairement les modifications apportées

## Normes de codage

- Respectez le style de code existant
- Commentez votre code de manière appropriée
- Vérifiez l'absence de fuites mémoire avec des outils comme Valgrind
- Incluez des tests pour les nouvelles fonctionnalités

## Structure du projet

- Évitez de modifier directement les interfaces (fichiers .h) existantes
- Pour ajouter une nouvelle structure de données, créez un nouveau fichier .c/.h
- Mettez à jour le Makefile si vous ajoutez de nouveaux fichiers

## Optimisations possibles

Si vous souhaitez contribuer à l'amélioration du projet, voici quelques pistes :

- Réduire la complexité de la fonction `reorganiseReseau()`
- Expérimenter avec d'autres structures de données pour le stockage du graphe
- Implémenter des algorithmes plus efficaces pour trouver les chemins les plus courts
- Améliorer la visualisation des résultats
- Ajouter des tests de performance et de validation supplémentaires

## Questions et discussions

Si vous avez des questions ou des suggestions, n'hésitez pas à ouvrir une issue pour en discuter avant de commencer à travailler sur une contribution majeure. 