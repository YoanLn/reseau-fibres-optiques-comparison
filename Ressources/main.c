#include "ArbreQuat.h"
#include "Hachage.h"
#include "Reseau.h"
#include "Chaine.h"
#include <time.h>

int main(){
    int nbPointsChaine = 500, xmax = 5000, ymax = 5000, nbChaines = 100; // Paramètres
    FILE *f = fopen("graph.txt", "w"); // Fichier de sortie
    if(f == NULL){
        fprintf(stderr, "Erreur d'ouverture du fichier\n");
        exit(1);
    }
    fprintf(f, "Points\tListe\tHachage\tArbre\n");
    Chaines *C;
    int m = 1000;
    Reseau *R1;
    Reseau *R2;
    Reseau *R3;
    for(int i = nbPointsChaine; i <= xmax; i+=500){ // Boucle pour les points
        C = generationAleatoire(nbChaines, nbPointsChaine, xmax, ymax); // Génération des chaines
        //Liste
        clock_t start1 = clock();
        R1 = reconstitueReseauListe(C);
        clock_t end1 = clock();
        double time1 = (double)(end1 - start1) / CLOCKS_PER_SEC;
        //Hachage
        clock_t start2 = clock();
        R2 = reconstitueReseauHachage(C,m);
        clock_t end2 = clock();
        double time2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
        //Arbre
        clock_t start3 = clock();
        R3 = reconstitueReseauArbre(C);
        clock_t end3 = clock();
        double time3 = (double)(end3 - start3) / CLOCKS_PER_SEC;
        fprintf(f, "%d\t%f\t%f\t%f\n", i, time1, time2, time3); // Écriture dans le fichier
    }
    fclose(f);
    f = fopen("graph_hachage.txt", "w"); // Fichier de sortie
    fprintf(f,"Taille m\tTemps\n"); 
    for(int i = m; i <= 10000; i+=1000){ // Boucle pour variance de taille m
        C = generationAleatoire(nbChaines, nbPointsChaine, xmax, ymax);
        //Hachage
        clock_t start2 = clock();
        R2 = reconstitueReseauHachage(C,i); 
        clock_t end2 = clock();
        double time2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
        fprintf(f, "%d\t%f\n", i, time2); // Écriture dans le fichier
    
    }
    // Libération de la mémoire
    fclose(f);
    libererReseau(R1);
    libererReseau(R2);
    libererReseau(R3);
    libererChaines(C);
    return 0;
}