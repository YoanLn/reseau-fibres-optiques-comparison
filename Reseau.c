#include <stdio.h>
#include <stdlib.h>
#include "Reseau.h"
#include "SVGwriter.h"

Reseau* creer_Reseau(int gamma){
    Reseau* r = (Reseau*)malloc(sizeof(Reseau));
    if(r==NULL){
        printf("Erreur malloc Reseau\n");
        return NULL;
    }

    r->nbNoeuds = 0;
    r->gamma = gamma;
    r->noeuds = NULL;
    r->commodites = NULL;

    return r;
}
CellNoeud *creer_CellNoeud(Noeud* n){
    if(n == NULL){
        printf("Erreur création CellNoeud: noeud n'existe pas\n");
        return NULL;
    }

    CellNoeud* c = (CellNoeud*)malloc(sizeof(CellNoeud));
    if(c==NULL){
        printf("Erreur malloc CellNoeud\n");
        return NULL;
    }

    c->nd = n;
    c->suiv = NULL;

    return c;
}

CellCommodite* creer_CellCommodite(Noeud* extrA, Noeud* extrB){
    if(extrA == NULL || extrB == NULL){
        printf("Erreur création CellCommodite: extremité n'existe pas\n");
        return NULL;
    }

    CellCommodite* C = (CellCommodite*)malloc(sizeof(CellCommodite));
    if(C==NULL){
        printf("Erreur malloc CellCommodite\n");
        return NULL;
    }

    C->extrA = extrA;
    C->extrB = extrB;

    return C;
}


void maj_voisins(Reseau *R, Noeud *v, Noeud *n){
    if(n==NULL){
        printf("Impossible de mettre a jour noeud qui n'existe pas\n");
        return;
    }

    if(v==NULL){ // Si le voisin n'existe pas, on ne fait rien
        return;
    }

    // Verification si le voisin a deja été ajouté
    CellNoeud *vu = n->voisins;
    while(vu){
        if(vu->nd->num == v->num){
            // Si le voisin a deja été ajouté, on ne fait rien
            return;
        }
        vu = vu->suiv;
    }

    //Ajout du noeud voisin dans la liste des voisins
    CellNoeud *voisin = creer_CellNoeud(v);
    voisin->suiv = n->voisins;
    n->voisins = voisin;

    //Mise a jour des voisins du voisin
    CellNoeud *voisin2 = creer_CellNoeud(n);
    voisin2->suiv = v->voisins;
    v->voisins = voisin2;
}


Noeud *rechercheCreeNoeudListe(Reseau *R, double x, double y){
    if(R == NULL){
        return NULL;
    }
    CellNoeud *currentCellNoeud = R->noeuds;
    while(currentCellNoeud != NULL){ // Parcourir la liste de noeuds
        Noeud *noeud = currentCellNoeud->nd;
        if(noeud->x == x && noeud->y == y){ // Si le noeud existe
            return noeud;
        }
        currentCellNoeud = currentCellNoeud->suiv;
    }// Si le noeud n'existe pas
    Noeud *newNoeud = (Noeud*)malloc(sizeof(Noeud));
    newNoeud->x = x;
    newNoeud->y = y;
    newNoeud->num = R->nbNoeuds + 1;
    newNoeud->voisins = NULL;
    CellNoeud *newCellNoeud = (CellNoeud*)malloc(sizeof(CellNoeud));
    newCellNoeud->nd = newNoeud;
    newCellNoeud->suiv = R->noeuds;
    R->noeuds = newCellNoeud;
    R->nbNoeuds++;
    return newNoeud;
}

Reseau* reconstitueReseauListe(Chaines *C){
    if(C==NULL){
        printf("Impossible de construire reseau a partir d'une chaine qui n'existe pas\n");
        return NULL;
    }

    Reseau* R = creer_Reseau(C->gamma);
    if(R==NULL){ 
        printf("Erreur création Réseau reconstitueReseauListe\n");
        return NULL;
    }

    CellChaine *c = C->chaines; 
    CellPoint *prec, *p; //Points des chaines
    Noeud *extrA, *extrB, *cour; //Noeuds: extremites des commodites et le noeud courant
    Noeud *voisin = NULL; //La cellule du noeud ajouté précédemment

    // Boucle passant par toutes les chaines
    for(int i=0; i<(C->nbChaines); i++){
        p = c->points;
        prec = NULL;
        voisin = NULL;
        while(p){ // Boucle passant par tous les points d'une chaine
            cour = rechercheCreeNoeudListe(R, p->x, p->y); // Recherche ou ajout du noeud correspondant
            if(cour == NULL){
                printf("Erreur recherche noeud dans reconstitution du reseau\n");
                libererReseau(R);
                return NULL;
            }
            if(prec == NULL){ // Recuperation extremite A de la commodite
                extrA = cour;
            }
            //Mise a jour des voisins
            maj_voisins(R, voisin, cour);
            // Le voisin du prochain noeud est celui-ci
            voisin = cour;
            prec = p;
            p = p->suiv;
        }

        //Recuperation extremite B de la commodite
        extrB = cour;

        //Creation et ajout de la commodite
        CellCommodite* C = creer_CellCommodite(extrA, extrB);
        if(C==NULL){
            printf("Erreur création commodite reconstitueReseauListe\n");
            libererReseau(R);
            return NULL;
        }
        C->suiv = R->commodites;
        R->commodites = C;

        c = c->suiv;
    }

    return R;
}


// Calcul du nombre de commodites
int nbCommondites(Reseau *R){
    CellCommodite * currentCellCo = R->commodites;
    int nb = 0;

    while( currentCellCo != NULL){
        nb++;
        currentCellCo = currentCellCo-> suiv;
    }
    return nb;

}
// Calcul du nombre de liaisons
int nbLiaisons(Reseau *R){
    int nb= 0;
    CellNoeud *currentCellNoeud = R->noeuds;
    while(currentCellNoeud != NULL){
        CellNoeud * voisin = currentCellNoeud -> nd -> voisins;
        while(voisin != NULL){
            nb++;
            voisin = voisin ->suiv;
        }

        currentCellNoeud =  currentCellNoeud -> suiv;

    }return nb/2; // Chaque liaison est comptée deux fois
}
// Ecris le réseau dans un fichier
void ecrireReseau(Reseau* R, FILE *f){
    if(R == NULL || f == NULL){
        exit(1);
    }
    fprintf(f,"NbNoeuds: %d\n", R->nbNoeuds);
    fprintf(f,"NbLiaisons: %d\n",nbLiaisons(R));
    fprintf(f,"NbCommodites: %d\n",nbCommondites(R));
    fprintf(f,"Gamma: %d\n\n", R->gamma);

    CellNoeud *currentNoeud = R->noeuds;
    while(currentNoeud != NULL){ // Parcourir la liste de noeuds
        Noeud *noeud = currentNoeud-> nd;
        fprintf(f,"v %d %lf %lf\n", noeud->num,noeud->x,noeud->y);
        currentNoeud = currentNoeud -> suiv;
    }
    fprintf(f,"\n");

    currentNoeud = R-> noeuds;
    while(currentNoeud!= NULL){ // Parcourir la liste de noeuds
        Noeud *noeud = currentNoeud-> nd;
        CellNoeud *lesvoisins = noeud->voisins;
        while(lesvoisins!= NULL){ // Parcourir la liste de voisins
            Noeud *voisin = lesvoisins-> nd;
            if(noeud->num < voisin -> num){
                fprintf(f,"l %d %d\n",noeud->num, voisin->num);
            }
            lesvoisins = lesvoisins->suiv;
        }
        currentNoeud = currentNoeud-> suiv;
    }
    fprintf(f,"\n");

    CellCommodite *currentCellCo = R-> commodites;
    while(currentCellCo!= NULL){ // Parcourir la liste de commodites
        fprintf(f,"k %d %d\n",currentCellCo->extrA->num,currentCellCo->extrB->num);
        currentCellCo = currentCellCo->suiv;
    }
}
void libererCellNoeud(CellNoeud *c){
    if(c==NULL){
        printf("CellNoeud déjà libéré\n");
        return;
    }
    // Liberation du noeud
    c->nd = NULL;
    free(c);
}
void libererNoeud(Noeud* n){
    if(n==NULL){
        printf("Noeud deja libéré\n");
        return;
    }
    //Liberation des voisins
    CellNoeud *temp, *c = n->voisins;
    while(c){// Parcourir la liste de voisins
        temp = c;
        c = c->suiv;
        libererCellNoeud(temp); // Libérer le voisin
    }
    free(n); // Libérer le noeud
}
void libererCellCommodite(CellCommodite *C){
    if(C==NULL){
        printf("Commodite déjà libérée\n");
        return;
    }
    // Liberation des extremites
    C->extrA = NULL;
    C->extrB = NULL;

    free(C);
}

void libererReseau(Reseau *R){
    if(R==NULL){
        printf("Reseau deja libéré\n");
        return;
    }

    //Liberation Noeuds
    CellNoeud *tempc, *c = R->noeuds;
    while(c){
        tempc = c;
        c = c->suiv;
        libererNoeud(tempc->nd);
        libererCellNoeud(tempc);
    }

    //Liberation Commodites
    CellCommodite *tempC, *C = R->commodites;
    while(C){
        tempC = C;
        C = C->suiv;
        libererCellCommodite(tempC);
    }

    free(R);
}


void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}