#include "ArbreQuat.c"
#include "Hachage.h"
#include <time.h>


int main(){
    // Fonction 1: Reconstruction Liste
    FILE *f = fopen("00014_burma.cha", "r");    
    Chaines *C = lectureChaines(f);
    clock_t start1 = clock();
    Reseau *R1 = reconstitueReseauListe(C);
    clock_t end1 = clock();
    double time1 = (double)(end1 - start1) / CLOCKS_PER_SEC;

    // Fonction 2: Reconstruction Table de Hachage
    clock_t start2 = clock();
    
    Reseau *R2 = reconstitueReseauHachage(C, C->nbChaines);
    clock_t end2 = clock();
    double time2 = (double)(end2 - start2) / CLOCKS_PER_SEC;

    // Fonction 3: Reconstruction Arbre Quaternaire
    clock_t start3 = clock();
    Reseau *R3 = reconstitueReseauArbre(C);
    clock_t end3 = clock();
    double time3 = (double)(end3 - start3) / CLOCKS_PER_SEC;

    // Écriture des temps de calcul dans un fichier
    FILE *file = fopen("temps_de_calcul.txt", "w");
    if (file != NULL) {
        fprintf(file, "Liste chaînée: %f secondes\n", time1);
        fprintf(file, "Table de hachage: %f secondes\n", time2);
        fprintf(file, "Arbre Quaternaire: %f secondes\n", time3);
        fclose(file);
    } else {
        printf("Erreur d'ouverture du fichier.\n");
    }
    // Libération de la mémoire
    libererReseau(R3);
    libererReseau(R1);
    libererReseau(R2);
    libererChaines(C);
    fclose(f);
    return 0;
}
