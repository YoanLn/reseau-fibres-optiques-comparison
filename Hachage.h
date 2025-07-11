#include "Reseau.h"
typedef struct{
  int nbElement; //pas necessaire ici
  int tailleMax;
  CellNoeud** T;
} TableHachage ;

TableHachage* creerTableHachage(int taille);
int fonctionClef(double x, double y);
Noeud* rechercheCreeNoeudHachage(Reseau *R, TableHachage* H, double x, double y);
Reseau* reconstitueReseauHachage(Chaines *C, int M);
void libererTableHachage(TableHachage* H);
int fonctionHachage(int cle, int m);