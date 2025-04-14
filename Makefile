CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm

# Fichiers sources communs
COMMON_SRCS = Chaine.c SVGwriter.c Reseau.c
COMMON_OBJS = $(COMMON_SRCS:.c=.o)

# Tous les exécutables
all: ChaineMain ReconstitueReseau main6.1 main mainGraphe

# Programme pour tester les chaînes
ChaineMain: ChaineMain.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Programme pour reconstituer un réseau avec différentes méthodes
ReconstitueReseau: ReconstitueReseau.o Hachage.o ArbreQuat.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Programme pour comparer les temps d'exécution (question 6.1)
main6.1: main6.1.o Hachage.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Programme principal pour la comparaison des 3 méthodes
main: main.o ArbreQuat.o Hachage.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Programme pour l'optimisation du réseau
mainGraphe: mainGraph.o Graphe.o Struct_File.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Règle générique pour compiler les fichiers .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et exécutables
clean:
	rm -f *.o ChaineMain ReconstitueReseau main6.1 main mainGraphe

# Nettoyage des fichiers de résultats
clean-results:
	rm -f *.res *.html graph*.txt temps_de_calcul.txt

# Dépendances des fichiers d'en-tête
Chaine.o: Chaine.h SVGwriter.h
SVGwriter.o: SVGwriter.h
Reseau.o: Reseau.h Chaine.h
Hachage.o: Hachage.h Reseau.h
ArbreQuat.o: ArbreQuat.h Reseau.h
Graphe.o: Graphe.h Struct_File.h Reseau.h
Struct_File.o: Struct_File.h

# Phony targets
.PHONY: all clean clean-results 