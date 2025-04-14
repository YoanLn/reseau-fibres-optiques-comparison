#include "Reseau.h"
#include "ArbreQuat.h"
#include "Hachage.h"


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input.cha> <int methode>\n", argv[0]);
        exit(1);
    }

    FILE *f1 = fopen(argv[1], "r");
    Chaines *C;

    if (f1 == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s\n", argv[1]);
        exit(1);
    }

    C = lectureChaines(f1);
    FILE *f = NULL;

    if(atoi(argv[2]) == 1){ // Liste chainee
        Reseau *R = reconstitueReseauListe(C);
        f = fopen("testListe.res", "w");
        if (f == NULL) {
            fprintf(stderr, "Erreur d'ouverture du fichier testReseau.res\n");
            exit(1);
        }
        ecrireReseau(R, f);
        afficheReseauSVG(R, "testListe");
        libererReseau(R);
        fclose(f);
    }   
    else if(atoi(argv[2]) == 2){ // Table de hachage
        Reseau *R = reconstitueReseauHachage(C, 10);
        f = fopen("testHachage.res", "w");
        if (f == NULL) {
            fprintf(stderr, "Erreur d'ouverture du fichier testHachage.res\n");
            exit(1);
        }
        ecrireReseau(R, f);
        afficheReseauSVG(R, "testHachage");
        libererReseau(R);
        fclose(f); 
    } // Arbre equilibre
    else if(atoi(argv[2]) == 3){
        Reseau *R = reconstitueReseauArbre(C);
        f = fopen("testArbre.res", "w");
        if (f == NULL) {
            fprintf(stderr, "Erreur d'ouverture du fichier testArbre.res\n");
            exit(1);
        }
        ecrireReseau(R, f);
        afficheReseauSVG(R, "testArbre");
        libererReseau(R);
        fclose(f);
    }
    else{ // Methode non reconnue
        fprintf(stderr, "Methode non reconnue\n");
        printf("Methode 1: Liste chainee\nMethode 2: Table de hachage\nMethode 3: Arbre equilibre\n");
    }

    libererChaines(C);
    fclose(f1);
    return 0;
}