#include "ArbreQuat.h"

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
    CellChaine* c = C->chaines;
    CellPoint* p = c->points;
    *xmin = p->x;
    *xmax = p->x;
    *ymin = p->y;
    *ymax = p->y;
    while(c != NULL){ // Parcourir la liste de chaines
        p = c->points;
        while(p != NULL){ // Parcourir la liste de points
            if(p->x < *xmin){
                *xmin = p->x; 
            }
            if(p->x > *xmax){
                *xmax = p->x;
            }
            if(p->y < *ymin){
                *ymin = p->y;
            }
            if(p->y > *ymax){
                *ymax = p->y;
            }
            p = p->suiv;
        }
        c = c->suiv;
    }
}
// Créer un arbre quaternaire
ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){
    ArbreQuat* a = (ArbreQuat*)malloc(sizeof(ArbreQuat));
    a->xc = xc; 
    a->yc = yc;
    a->coteX = coteX;
    a->coteY = coteY;
    a->noeud = NULL;
    a->so = NULL;
    a->se = NULL;
    a->no = NULL;
    a->ne = NULL;
    return a;
}
// Insérer un noeud dans un arbre quaternaire
void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent){
    if(*a == NULL){ // Si l'arbre est vide
        *a = creerArbreQuat(n->x, n->y, parent->coteX/2, parent->coteY/2);
        (*a)->noeud = n;
    }else{ // Si l'arbre n'est pas vide
        if(n->x < (*a)->xc){ // Si le noeud est à gauche
            if(n->y < (*a)->yc){ // Si le noeud est en bas
                insererNoeudArbre(n, &((*a)->so), *a);
            }else{ // Si le noeud est en haut
                insererNoeudArbre(n, &((*a)->no), *a);
            }
        }else{ // Si le noeud est à droite
            if(n->y < (*a)->yc){ // Si le noeud est en bas
                insererNoeudArbre(n, &((*a)->se), *a);
            }else{ // Si le noeud est en haut
                insererNoeudArbre(n, &((*a)->ne), *a);
            }
        }
    }
}
// Recherche ou crée un noeud dans un arbre quaternaire
Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y){
    if(*a == NULL){ // Si l'arbre est vide
        Noeud* n = (Noeud*)malloc(sizeof(Noeud));
        n->x = x;
        n->y = y;
        n->num = R->nbNoeuds;
        n->voisins = NULL;
        insererNoeudArbre(n, a, parent); // Insérer le noeud dans l'arbre
        CellNoeud* new = (CellNoeud*)malloc(sizeof(CellNoeud)); 
        new->nd = n;
        new->suiv = R->noeuds;
        R->noeuds = new;
        R->nbNoeuds++;
        return n;
    }else{ // Si l'arbre n'est pas vide
        if((*a)->noeud != NULL){ // Si le noeud existe
            if((*a)->noeud->x == x && (*a)->noeud->y == y){
                return (*a)->noeud;
            }
        }
        
        if(x < (*a)->xc){ // Si le noeud est à gauche
            if(y < (*a)->yc){ // Si le noeud est en bas
                return rechercheCreeNoeudArbre(R, &((*a)->so), *a, x, y);
            }else{ // Si le noeud est en haut
                return rechercheCreeNoeudArbre(R, &((*a)->no), *a, x, y);
            }
        }else{ // Si le noeud est à droite
            if(y < (*a)->yc){ // Si le noeud est en bas
                return rechercheCreeNoeudArbre(R, &((*a)->se), *a, x, y);
            }else{ // Si le noeud est en haut
                return rechercheCreeNoeudArbre(R, &((*a)->ne), *a, x, y);
            }
        }
    }
}
// Libérer un arbre quaternaire
void libererArbre(ArbreQuat* a){
    if(a != NULL){ // Si l'arbre n'est pas vide
        libererArbre(a->so);
        libererArbre(a->se);
        libererArbre(a->no);
        libererArbre(a->ne);
        free(a);
    }
}
// Reconstituer un réseau à partir d'une liste de chaînes
Reseau *reconstitueReseauArbre(Chaines *C){
    if(C == NULL){
        return NULL;
    }

    Reseau *R = (Reseau*)malloc(sizeof(Reseau));
    R-> nbNoeuds = 0;
    R-> gamma = C-> gamma;
    R->noeuds = NULL;
    R->commodites = NULL;
    // Créer un arbre quaternaire
    double xmin, ymin, xmax, ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
    ArbreQuat* a = creerArbreQuat((xmin + xmax)/2, (ymin + ymax)/2, xmax - xmin, ymax - ymin);

    CellChaine *c = C->chaines; 
    CellPoint *prec, *p; //Points des chaines
    Noeud *extrA, *extrB, *cour; //Noeuds: extremites des commodites et le noeud courant
    Noeud *voisin = NULL; //La cellule du noeud ajouté précédemment
    // Parcourir la liste de chaînes
    for(int i=0; i<(C->nbChaines); i++){
        // Parcourir la liste de points
        p = c->points;
        prec = NULL;
        voisin = NULL;
        while(p){ 
            cour = rechercheCreeNoeudArbre(R, &a, NULL, p->x, p->y); // Recherche ou crée un noeud dans l'arbre
            if(cour == NULL){
                printf("Erreur recherche noeud dans reconstitution du reseau\n");
                libererReseau(R);
                return NULL;
            }

            // Recuperation extremite A de la commodite
            if(prec == NULL){
                extrA = cour;
            }

            // Mise a jour des voisins
            maj_voisins(R, voisin, cour);

            // Le voisin du prochain noeud est celui-ci
            voisin = cour;
            prec = p;
            p = p->suiv;
        }

        // Recuperation extremite B de la commodite
        extrB = cour;

        // Créer une commodité
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
    libererArbre(a); // Libérer l'arbre
    return R;
}