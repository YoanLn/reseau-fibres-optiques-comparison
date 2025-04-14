#include "Chaine.h"
#include "Reseau.h"
#include "SVGwriter.h"
#include <stdio.h>
#include <stdlib.h>
#include "Graphe.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: %s <fichier de chaines> <fichier de resultat>\n", argv[0]);
        return 1;
    }
    // Lire les chaines à partir du fichier
    char* fichierChaine = argv[1];
    char* fichierResultat = argv[2];
    FILE* f = fopen(fichierChaine, "r");
    if (!f) {
        printf("Erreur lors de l'ouverture du fichier %s\n", fichierChaine);
        return 1;
    }

    Chaines* chaines = lectureChaines(f);
    fclose(f);

    if (!chaines) {
        printf("Erreur lors de la lecture des chaines.\n");
        return 1;
    }

    Reseau* reseau = reconstitueReseauListe(chaines);
    if (!reseau) {
        printf("Erreur lors de la reconstitution du réseau.\n");
        return 1;
    
    }
    int resultat = reorganiseReseau(reseau); // Réorganisation du réseau
    printf("Resultat de la reorganisation: %s\n", resultat ? "Succès" : "Échec");

    // Enregistrer le réseau dans un fichier SVG pour visualisation
    afficheReseauSVG(reseau, "output_graph.svg");

    // Enregistrer le résultat dans un fichier texte
    FILE* fout = fopen(fichierResultat, "w");
    if (!fout) {
        printf("Erreur lors de l'ouverture du fichier de sortie %s\n", fichierResultat);
        return 1;
    }
    // Écrire le réseau dans le fichier de sortie et libérer la mémoire
    ecrireReseau(reseau, fout);
    fclose(fout);
    libererChaines(chaines);
    libererReseau(reseau);

    return 0;
}


