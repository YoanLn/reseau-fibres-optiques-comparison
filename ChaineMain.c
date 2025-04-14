#include "Chaine.h"

int main(int argc, char *argv[]){
    FILE *f;
    Chaines *C;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <fichier.cha>\n", argv[0]);
        exit(1);
    }
    f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s\n", argv[1]);
        exit(1);
    }
    C = lectureChaines(f);
    fclose(f);
    f = fopen("testChaines.cha", "w");
    ecrireChaines(C, f);

    afficheChainesSVG(C, "testChaines");

    fclose(f);
}