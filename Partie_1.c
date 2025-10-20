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