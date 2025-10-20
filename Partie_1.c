//
// Created by Paul Girault on 20/10/2025.
//
#include "Partie_1.h"



//question1
typedef struct cellule {
    int sommet_arrivee;
    float proba;
    struct cellule *suiv;
} cellule;

typedef struct {
    cellule *head;
} liste;

typedef struct {
    int taille;
    liste *tab; // tableau dynamique de listes
} liste_adjacence;

//question 2
cellule *creer_cellule(int arrivee, float proba) {
    cellule *c = malloc(sizeof(cellule));
    c->sommet_arrivee = arrivee;
    c->proba = proba;
    c->suiv = NULL;
    return c;
}

liste creer_liste_vide() {
    liste l;
    l.head = NULL;
    return l;
}

void ajouter_cellule(liste *l, int arrivee, float proba) {
    cellule *c = creer_cellule(arrivee, proba);
    c->suiv = l->head;
    l->head = c;
}

void afficher_liste(liste l) {
    cellule *tmp = l.head;
    while (tmp != NULL) {
        printf("(%d, %.2f) -> ", tmp->sommet_arrivee, tmp->proba);
        tmp = tmp->suiv;
    }
    printf("NULL\n");
}

//question 3
liste_adjacence readGraph(const char *filename){
    FILE *file = fopen(filename, "rt");
    if (file==NULL){
        perror("Erreur d'ouverture");
        exit(EXIT_FAILURE);
    }
    int nbvert;
    int depart;
    int arrivee;
    float proba;
    fscanf(file, "%d", &nbvert);

    liste_adjacence G;
    G.taille = nbvert;
    G.tab = malloc(nbvert *sizeof(liste));
    for (int i=0; i< nbvert;i++)
        G.tab[i] = creer_liste_vide();

    while(fscanf(file,"%d %d %f", &depart, &arrivee, &proba)==3)
        ajouter_cellule(&G.tab[depart-1], arrivee,proba);

    fclose(file);
    return G;
}

// question 4
void verifier_markov(liste_adjacence G) {
    for (int i = 0; i < G.taille; i++) {
        cellule *tmp = G.tab[i].head;
        float somme = 0;
        while (tmp != NULL) {
            somme += tmp->proba;
            tmp = tmp->suiv;
        }
        if (somme < 0.99 || somme > 1.01)
            printf("Sommet %d : somme = %.2f ❌\n", i + 1, somme);
        else
            printf("Sommet %d : somme = %.2f ✅\n", i + 1, somme);
    }
}