#include "Reseau.h"
#include "Struct_File.h"
#include "Graphe.h"
#include "stdlib.h"
#include "stdio.h"


//creer graphe from reseau
Graphe *creerGraphe(Reseau *r){
    if( r ==  NULL) return NULL;

    Graphe *g = malloc(sizeof(Graphe));
    g->nbsom = r->nbNoeuds;
    g->T_som = malloc(g->nbsom *sizeof(Sommet*));

    //initi la liste de sommets

    for(int i = 0; i < g-> nbsom; i++ ){
        g->T_som[i] = malloc(sizeof(Sommet));
        g->T_som[i] ->num = i;
        g->T_som[i] ->L_voisin = NULL;

    }

    //ajoute arete dans la liste
    CellNoeud* ptr = r->noeuds;
    while(ptr != NULL){
        Noeud* node = ptr->nd;
        int idx = node->num - 1;  // tab commence à 0

        // coordonnes sommet
        g->T_som[idx]->x = node->x;
        g->T_som[idx]->y = node->y;

        // ajoute arete
        CellNoeud* voisins = node->voisins;
        while (voisins) {
            int idx = voisins->nd->num - 1;  // tab commence à 0
            Cellule_arete* new_arete = malloc(sizeof(Cellule_arete));
            new_arete->a = malloc(sizeof(Arete));
            new_arete->a->u = node->num;
            new_arete->a->v = voisins->nd->num;
            new_arete->suiv = g->T_som[idx]->L_voisin;
            g->T_som[idx]->L_voisin = new_arete;
            voisins = voisins->suiv;
        }
        ptr = ptr->suiv;
    }

    return g;

}



int bfs(Graphe* g, int u, int v) {

    int* visited = malloc(g->nbsom * sizeof(int));  
    int* distance = malloc(g->nbsom * sizeof(int)); 

    // initialiser les tableaux
    for (int i = 0; i < g->nbsom; i++) {
        visited[i] = 0;    // marquer les sommets sont pas vu
        distance[i] = -1;  // la distance entre u et i est -1
    }

    S_file queue;  // file d'attente
    Init_file(&queue);  // initialiser la file d'attente
    enfile(&queue, u);  // rajoute u dans la file
    visited[u] = 1;  // marquer u comme visité
    distance[u] = 0;  // la distance entre u et u est 0 

    while (!estFileVide(&queue)) {
        int current = defile(&queue);  // on prends le premier element et le suprimer dans la liste s_file
        Cellule_arete* tmp = g->T_som[current]->L_voisin;
        while (tmp!=NULL){// ici on essaie de rechecher le v dans les voisins(les proches)
            int neighbor = tmp->a->v - 1;
            if (!visited[neighbor]) {
                visited[neighbor] = 1;
                distance[neighbor] = distance[current] + 1;
                enfile(&queue, neighbor);
                if (neighbor == v) {  // si on trouve le v on return la distance 
                   
                    return distance[v];
                }
            }
            // si on trouve pas v on continue dans la liste s_file et pour la premier element(il etiat un des voisins)
            tmp = tmp->suiv;
        }
    }//ici on essaie de rechercher dans les voisins qui pourra etre compliquer le "v" et ca va return toute de suite si on trouvera si non on va supprimer lui dans la file d'attente

    
    return -1;  // return  -1 si on trouve pas la moyen u à v
}

//fonction qui va retourner le chemin entre u et v
Cellule_file* bfs_path(Graphe* g, int u, int v) {
    if (!g || u >= g->nbsom || v >= g->nbsom || u < 0 || v < 0) return NULL; // Vérifiez les limites

    int* visited = malloc(g->nbsom * sizeof(int));
    int* parent = malloc(g->nbsom * sizeof(int));
    for (int i = 0; i < g->nbsom; i++) { // Initialiser les tableaux
        visited[i] = 0;
        parent[i] = -1;
    }
    
    S_file queue;
    Init_file(&queue);
    enfile(&queue, u);
    visited[u] = 1;

    while (!estFileVide(&queue)) { // Parcours en largeur
        int current = defile(&queue);
        Cellule_arete* tmp = g->T_som[current]->L_voisin; // Parcourir les voisins
        while (tmp != NULL) { // Parcourir les voisins
            int neighbor = tmp->a->v - 1;
            if (!visited[neighbor]) { // Si le voisin n'est pas visité
                visited[neighbor] = 1;
                parent[neighbor] = current;
                enfile(&queue, neighbor);
                if (neighbor == v) { // Si on trouve le sommet v
                    Cellule_file* path = NULL;
                    // Construire le chemin à l'envers
                    for (int step = v; step != -1; step = parent[step]) { // Remonter le chemin
                        Cellule_file* new_node = malloc(sizeof(Cellule_file));
                        new_node->val = step + 1;  // Convertir l'index en numéro de sommet
                        new_node->suiv = path;
                        path = new_node;
                    }
                    free(visited);
                    free(parent);
                    return path;
                }
            }
            tmp = tmp->suiv;
        }
    }

    free(visited);
    free(parent);
    return NULL; 
}

void libererGraphe(Graphe* g) {
    if (!g) return;  // Vérifiez si le graphe est NULL

    for (int i = 0; i < g->nbsom; i++) {
        Cellule_arete* current = g->T_som[i]->L_voisin;
        while (current) { // Libérer les arêtes
            Cellule_arete* temp = current;
            current = current->suiv;
            free(temp->a);
            free(temp); 
        } 
        free(g->T_som[i]); // Libérer les sommets
    }
    free(g->T_som); // Libérer le tableau de sommets
    free(g); // Libérer le graphe
}

int reorganiseReseau(Reseau *r) {
    if (!r) return 0;  // Vérifiez que le réseau n'est pas NULL

    Graphe *g = creerGraphe(r);  // Créez un graphe à partir du réseau
    if (!g) return 0;  // Vérifiez que le graphe a été créé avec succès

    // Tableau pour compter l'utilisation des fibres sur chaque arête
    int **utilisation = malloc(g->nbsom * sizeof(int *));
    for (int i = 0; i < g->nbsom; i++) {
        utilisation[i] = calloc(g->nbsom, sizeof(int));  // Initialisez avec des zéros
    }

    // Parcourir toutes les commodités pour calculer les chemins et compter l'utilisation des fibres
    CellCommodite *commod = r->commodites;
    while (commod) {
        int src = commod->extrA->num;
        int dest = commod->extrB->num;

        // Utiliser BFS pour trouver le chemin
        Cellule_file *path = bfs_path(g, src - 1, dest - 1);  // Ajustez les indices selon la fonction bfs_path

        // Parcourir le chemin pour mettre à jour l'utilisation des arêtes
        Cellule_file *current = path;
        while (current && current->suiv) {
            int u = current->val - 1;  // Ajustez les indices
            int v = current->suiv->val - 1;
            utilisation[u][v]++;
            utilisation[v][u]++;
            current = current->suiv;
        }

        // Libérer le chemin
        while (path) {
            Cellule_file *temp = path;
            path = path->suiv;
            free(temp);
        }

        commod = commod->suiv;
    }

    // Vérifiez si une arête dépasse la capacité gamma
    for (int i = 0; i < g->nbsom; i++) {
        for (int j = 0; j < g->nbsom; j++) {
            if (utilisation[i][j] > r->gamma) {
                // Libération de la mémoire
                for (int k = 0; k < g->nbsom; k++) {
                    free(utilisation[k]);
                }
                free(utilisation);
                return 0;  // Si une arête dépasse gamma, retournez 0
            }
        }
    }

    // Libération de la mémoire
    for (int i = 0; i < g->nbsom; i++) {
        free(utilisation[i]);
    }
    free(utilisation);
    libererGraphe(g);

    return 1;  // Retourne 1 si le réseau est correctement organisé
}
