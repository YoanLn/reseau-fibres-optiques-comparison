#include "Hachage.h"

// Créer une table de hachage
TableHachage* creerTableHachage(int taille){
    TableHachage* H = (TableHachage*)malloc(sizeof(TableHachage));
    H->tailleMax = taille; 
    H->nbElement = 0;
    H->T = (CellNoeud**)malloc(taille * sizeof(CellNoeud*));
    for(int i = 0; i < taille; i++){ // Initialiser la table de hachage
        H->T[i] = NULL;
    }
    return H;

}
// Fonction de clé
int fonctionClef(double x, double y){
    return (int)(y + (x + y) * (x + y + 1) / 2); 
}
// Fonction de hachage
int fonctionHachage(int cle, int m){
    double A = (sqrt(5) - 1) / 2; 
    return floor(m * (cle * A - floor(cle * A)));
}
// Recherche ou crée un noeud dans la table de hachage
Noeud* rechercheCreeNoeudHachage(Reseau *R, TableHachage* H, double x, double y){
    int cle = fonctionClef(x, y);
    int h = fonctionHachage(cle, H->tailleMax); 
    CellNoeud* c = H->T[h];
    while(c != NULL){ // Parcourir la liste de noeuds
        if(c->nd->x == x && c->nd->y == y){ // Si le noeud existe
            return c->nd;
        }
        c = c->suiv;
    } // Si le noeud n'existe pas
    Noeud* n = (Noeud*)malloc(sizeof(Noeud));
    n->x = x;
    n->y = y;
    n->num = R->nbNoeuds;
    n->voisins = NULL;
    CellNoeud* new = (CellNoeud*)malloc(sizeof(CellNoeud));
    new->nd = n;
    new->suiv = H->T[h];
    H->T[h] = new;
    CellNoeud* new2 = (CellNoeud*)malloc(sizeof(CellNoeud));
    new2->nd = n;
    new2->suiv = R->noeuds;
    R->noeuds = new2;
    R->nbNoeuds++;
    return n;
}


Reseau *reconstitueReseauHachage(Chaines *C, int M) {
    Reseau *R = (Reseau*)malloc(sizeof(Reseau)); 
    R-> nbNoeuds = 0;
    R-> gamma = C-> gamma;
    R->noeuds = NULL;
    R->commodites = NULL;

    TableHachage *H = creerTableHachage(M);
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
            cour = rechercheCreeNoeudHachage(R, H, p->x, p->y); // Recherche ou ajout du noeud correspondant
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

        // Creation et ajout de la commodite
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
    libererTableHachage(H);
    return R;
}

// Libérer la mémoire allouée pour la table de hachage
void libererTableHachage(TableHachage *H){
    if(H==NULL){
        printf("Table de Hachage deja libérée\n");
        return;
    }

    CellNoeud *temp, *c;
    for(int i=0; i<(H->tailleMax); i++){ // Parcourir la table de hachage
        c = H->T[i];
        temp = NULL; 
        while(c){ // Parcourir la liste de noeuds
            temp = c;
            c = c->suiv;
            libererCellNoeud(temp);
        }
    }

    free(H->T);
    free(H);
}