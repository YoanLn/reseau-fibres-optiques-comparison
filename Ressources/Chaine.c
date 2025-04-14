#include "Chaine.h"

Chaines *lectureChaines(FILE *f) {
    Chaines *chaines = (Chaines *)malloc(sizeof(Chaines));
    if (chaines == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(1);
    }
    // Récupère le nombre de chaînes et le gamma
    fscanf(f, "NbChain: %d\n", &chaines->nbChaines);
    fscanf(f, "Gamma: %d\n", &chaines->gamma);
    chaines->chaines = NULL;
    CellChaine *chaine = NULL;
    CellPoint *point = NULL;
    int i, j, nbPoints;
    // Récupère les chaînes
    for (i = 0; i < chaines->nbChaines; i++) {
        chaine = (CellChaine *)malloc(sizeof(CellChaine));
        if (chaine == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire\n");
            exit(1);
        }
        // Récupère le numéro de la chaîne
        fscanf(f, " %d", &chaine->numero);
        chaine->suiv = NULL;
        if (chaines->chaines == NULL) {
            chaines->chaines = chaine; // Si c'est la première chaîne, la chaîne est la tête
        } else {
            // Trouve la dernière chaîne et lie la nouvelle chaîne à celle-ci
            CellChaine *last_chain = chaines->chaines;
            while (last_chain->suiv != NULL) {
                last_chain = last_chain->suiv;
            }
            last_chain->suiv = chaine;
        }
        chaine->points = NULL;
        fscanf(f, " %d", &nbPoints);
        for (j = 0; j < nbPoints; j++) {
            point = (CellPoint *)malloc(sizeof(CellPoint));
            if (point == NULL) {
                fprintf(stderr, "Erreur d'allocation de mémoire\n");
                exit(1);
            }
            fscanf(f, "%lf %lf ", &point->x, &point->y);
            point->suiv = NULL; 
            if (chaine->points == NULL) {
                chaine->points = point; // Si c'est le premier point, le point est la tête
            } else {
                // Trouve le dernier point et lie le nouveau point à celui-ci
                CellPoint *last_point = chaine->points;
                while (last_point->suiv != NULL) {
                    last_point = last_point->suiv;
                }
                last_point->suiv = point;
            }
        }
        fscanf(f, "\n");
    }
    return chaines;
}


void ecrireChaines(Chaines *C, FILE *f){
    CellChaine *chaine = C->chaines;
    CellPoint *point = NULL;
    // Écrit le nombre de chaînes et le gamma
    fprintf(f, "NbChain: %d\n", C->nbChaines);
    fprintf(f, "Gamma: %d\n", C->gamma);
    while (chaine != NULL) {
        // Écrit le numéro de la chaîne et le nombre de points
        fprintf(f, "%d %d ", chaine->numero, comptePoints(chaine));
        point = chaine->points;
        while (point != NULL) {
            // Écrit les coordonnées des points
            fprintf(f, "%.2f %.2f ", point->x, point->y);
            point = point->suiv;
        }
        fprintf(f, "\n");
        chaine = chaine->suiv;
    }
}

int comptePoints(CellChaine *chaine){
    int nbPoints = 0;
    CellPoint *point = chaine->points;
    while (point != NULL) {
        nbPoints++;
        point = point->suiv;
    }
    return nbPoints;
}

void afficheChainesSVG(Chaines *C, char* nomInstance){
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}
// Calcul de la longueur d'une chaine
int longueurChaine(CellPoint *points){
    int longueur = 0;
    CellPoint *point = points;
    while (point->suiv != NULL) {
        longueur += sqrt(pow(point->x - point->suiv->x, 2) + pow(point->y - point->suiv->y, 2));
        point = point->suiv;
    }
    return longueur;
}
// Calcul de la longueur totale des chaines
double longueurTotale(Chaines *C){
    double longueur = 0;
    CellChaine *chaine = C->chaines;
    while (chaine != NULL) {
        longueur += longueurChaine(chaine->points);
        chaine = chaine->suiv;
    }
    return longueur;
}
// Calcul du nombre total de points
int comptePointsTotal(Chaines *C){
    int nbPoints = 0;
    CellChaine *chaine = C->chaines;
    while (chaine != NULL) {
        nbPoints += comptePoints(chaine);
        chaine = chaine->suiv;
    }
    return nbPoints;
}


void libererCellPoint(CellPoint *points){
    CellPoint *point = points;
    while (point != NULL) {
        CellPoint *temp = point;
        point = point->suiv;
        free(temp);
    }
}

void libererCellChaine(CellChaine *chaines){
    CellChaine *chaine = chaines;
    while (chaine != NULL) {
        CellChaine *temp = chaine;
        libererCellPoint(chaine->points);
        chaine = chaine->suiv;
        free(temp);
    }
}

void libererChaines(Chaines *C){
    libererCellChaine(C->chaines);
    free(C);
}

// Génération aléatoire de chaines
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax) {
    Chaines* newChaines = (Chaines*) malloc(sizeof(Chaines));
    if (!newChaines) return NULL;
    // Initialisation des valeurs
    newChaines->nbChaines = nbChaines;
    newChaines->gamma = 0;  
    newChaines->chaines = NULL;

    CellChaine* lastChain = NULL;
    // Génération des chaines
    for (int i = 0; i < nbChaines; i++) {
        CellChaine* currentChain = (CellChaine*) malloc(sizeof(CellChaine));
        if (!currentChain) { 
            return NULL;
        }
        // Initialisation des valeurs
        currentChain->numero = i;
        currentChain->points = NULL;
        currentChain->suiv = NULL;

        if (lastChain == NULL) { // Si c'est la première chaine
            newChaines->chaines = currentChain;
        } else { 
            lastChain->suiv = currentChain;
        }
        lastChain = currentChain;

        CellPoint* lastPoint = NULL;
        // Génération des points
        for (int j = 0; j < nbPointsChaine; j++) {
            CellPoint* currentPoint = (CellPoint*) malloc(sizeof(CellPoint));
            if (!currentPoint) {
                return NULL;
            }
            // Initialisation des valeurs aléatoires
            currentPoint->x = rand() % (xmax + 1);
            currentPoint->y = rand() % (ymax + 1);
            currentPoint->suiv = NULL;
            
            if (lastPoint == NULL) { // Si c'est le premier point
                currentChain->points = currentPoint;
            } else { // Sinon, on lie le point précédent au point courant
                lastPoint->suiv = currentPoint;
            }
            lastPoint = currentPoint;
        }
    }

    return newChaines;
}