//
// Created by Paul Girault on 20/10/2025.
//

#include "Partie_1.h"

// Création d'une cellule
cellule *creer_cellule(int arrivee, float proba) {
    cellule *c = malloc(sizeof(cellule));
    if (c == NULL) {
        perror("Erreur d'allocation mémoire pour une cellule");
        exit(EXIT_FAILURE);
    }
    c->sommet_arrivee = arrivee;
    c->proba = proba;
    c->suiv = NULL;
    return c;
}

// Création d'une liste vide
liste creer_liste_vide(void) {
    liste l;
    l.head = NULL;
    return l;
}

// Ajout d'une cellule au début de la liste
void ajouter_cellule(liste *l, int arrivee, float proba) {
    cellule *c = creer_cellule(arrivee, proba);
    c->suiv = l->head;
    l->head = c;
}

// Affichage d'une liste
void afficher_liste(liste l) {
    cellule *tmp = l.head;
    printf("[head]");
    while (tmp != NULL) {
        printf(" -> (%d, %.2f)", tmp->sommet_arrivee, tmp->proba);
        tmp = tmp->suiv;
    }
    printf(" -> NULL\n");
}

//QUESTION 3


// Lecture fichier texte et création de liste d'adjacence
liste_adjacence readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    int nbvert, depart, arrivee;
    float proba;

    // Lecture nombre sommets
    if (fscanf(file, "%d", &nbvert) != 1) {
        fprintf(stderr, "Erreur : impossible de lire le nombre de sommets.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Initialisation du graphe
    liste_adjacence G;
    G.taille = nbvert;
    G.tab = malloc(nbvert * sizeof(liste));
    if (G.tab == NULL) {
        perror("Erreur d'allocation mémoire pour les listes");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Initialiser toutes les listes vides
    for (int i = 0; i < nbvert; i++) {
        G.tab[i] = creer_liste_vide();
    }

    // Lecture des arêtes
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        if (depart < 1 || depart > nbvert || arrivee < 1 || arrivee > nbvert) {
            fprintf(stderr, "Erreur : sommet hors bornes (%d -> %d)\n", depart, arrivee);
            continue;
        }
        ajouter_cellule(&G.tab[depart - 1], arrivee, proba);
    }

    fclose(file);
    return G;
}

// QUESTION 4


// Vérification de la propriété de Markov
void verifier_markov(liste_adjacence G) {
    printf("\n=== Vérification du graphe de Markov ===\n");
    for (int i = 0; i < G.taille; i++) {
        cellule *tmp = G.tab[i].head;
        float somme = 0.0f;

        // Calcul de la somme des probabilités sortantes du sommet i+1
        while (tmp != NULL) {
            somme += tmp->proba;
            tmp = tmp->suiv;
        }

        // Vérification
        if (somme < 0.99f || somme > 1.01f)
            printf("Sommet %d : somme = %.2f ❌\n", i + 1, somme);
        else
            printf("Sommet %d : somme = %.2f ✅\n", i + 1, somme);
    }
}