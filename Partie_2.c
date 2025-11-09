#include "Partie_2.h"
#include <stdlib.h>
#include <string.h>

// Macro simple pour trouver le minimum
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// ========================================
// === IMPLÉMENTATION - UTILS PILE ========
// ========================================

t_pile* creer_pile(int capacite_initiale) {
    t_pile *p = malloc(sizeof(t_pile));
    if (!p) {
        perror("Erreur allocation pile");
        exit(EXIT_FAILURE);
    }
    p->valeurs = malloc(capacite_initiale * sizeof(int));
    if (!p->valeurs) {
        perror("Erreur allocation tableau pile");
        free(p);
        exit(EXIT_FAILURE);
    }
    p->sommet_pile = 0;
    p->capacite = capacite_initiale;
    return p;
}

void empiler(t_pile *p, int id_sommet) {
    // Agrandir si nécessaire
    if (p->sommet_pile == p->capacite) {
        p->capacite *= 2;
        p->valeurs = realloc(p->valeurs, p->capacite * sizeof(int));
        if (!p->valeurs) {
            perror("Erreur re-allocation pile");
            exit(EXIT_FAILURE);
        }
    }
    p->valeurs[p->sommet_pile] = id_sommet;
    p->sommet_pile++;
}

int depiler(t_pile *p) {
    if (est_vide(p)) {
        fprintf(stderr, "Erreur : la pile est vide.\n");
        return -1;
    }
    p->sommet_pile--;
    return p->valeurs[p->sommet_pile];
}

int est_vide(t_pile *p) {
    return p->sommet_pile == 0;
}

void liberer_pile(t_pile *p) {
    if (p) {
        free(p->valeurs);
        free(p);
    }
}

// ================================================
// === IMPLÉMENTATION - UTILS CLASSE/PARTITION ====
// ================================================

// Initialise une classe (vide)
t_classe creer_classe(char* nom) {
    t_classe c;
    strncpy(c.nom, nom, 10);
    c.nb_sommets = 0;
    c.cap_sommets = 10; // Capacité initiale
    c.sommets = malloc(c.cap_sommets * sizeof(int));
    if (!c.sommets) {
        perror("Erreur allocation sommets classe");
        exit(EXIT_FAILURE);
    }
    return c;
}

// Ajoute un sommet à une classe (avec realloc si besoin)
void ajouter_sommet_classe(t_classe *c, int id_sommet) {
    if (c->nb_sommets == c->cap_sommets) {
        c->cap_sommets *= 2;
        c->sommets = realloc(c->sommets, c->cap_sommets * sizeof(int));
        if (!c->sommets) {
            perror("Erreur re-allocation sommets classe");
            exit(EXIT_FAILURE);
        }
    }
    c->sommets[c->nb_sommets] = id_sommet;
    c->nb_sommets++;
}

// Initialise une partition (vide)
t_partition creer_partition() {
    t_partition p;
    p.nb_classes = 0;
    p.cap_classes = 10; // Capacité initiale
    p.classes = malloc(p.cap_classes * sizeof(t_classe));
    if (!p.classes) {
        perror("Erreur allocation partition");
        exit(EXIT_FAILURE);
    }
    return p;
}

// Ajoute une classe à la partition (avec realloc si besoin)
void ajouter_classe_partition(t_partition *p, t_classe nouvelle_classe) {
    if (p->nb_classes == p->cap_classes) {
        p->cap_classes *= 2;
        p->classes = realloc(p->classes, p->cap_classes * sizeof(t_classe));
        if (!p->classes) {
            perror("Erreur re-allocation partition");
            exit(EXIT_FAILURE);
        }
    }
    p->classes[p->nb_classes] = nouvelle_classe;
    p->nb_classes++;
}

// Affiche la partition (Validation Étape 1)
void afficher_partition(t_partition p) {
    printf("\n=== Partition du graphe (Algorithme de Tarjan) ===\n");
    for (int i = 0; i < p.nb_classes; i++) {
        printf("%s: {", p.classes[i].nom);
        for (int j = 0; j < p.classes[i].nb_sommets; j++) {
            printf("%d", p.classes[i].sommets[j]);
            if (j < p.classes[i].nb_sommets - 1) {
                printf(",");
            }
        }
        printf("}\n");
    }
}

// Libère la mémoire de la partition et de ses classes
void liberer_partition(t_partition *p) {
    if (p) {
        for (int i = 0; i < p->nb_classes; i++) {
            free(p->classes[i].sommets); // Libère le tableau de sommets de chaque classe
        }
        free(p->classes); // Libère le tableau de classes
    }
}

// ========================================
// === IMPLÉMENTATION - ÉTAPE 1 (TARJAN) ==
// ========================================

//
t_tarjan_vertex* initialiser_tarjan_data(liste_adjacence G) {
    t_tarjan_vertex* data = malloc(G.taille * sizeof(t_tarjan_vertex));
    if (!data) {
        perror("Erreur allocation tarjan_data");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < G.taille; i++) {
        data[i].identifiant = i + 1; // ID 1-indexé
        data[i].numero = -1;         // Non visité
        data[i].numero_accessible = -1; //
        data[i].dans_pile = 0;       //
    }
    return data;
}

//
void tarjan_parcours(int u_id, liste_adjacence G, t_tarjan_vertex *data,
                     t_pile *pile, int *p_index, t_partition *partition)
{
    // u_idx est l'indice 0-indexé du sommet (ex: 0 pour sommet 1)
    int u_idx = u_id - 1;

    // 1. Marquer le sommet 'u' comme visité
    data[u_idx].numero = *p_index;
    data[u_idx].numero_accessible = *p_index;
    (*p_index)++;
    empiler(pile, u_id);
    data[u_idx].dans_pile = 1; // <-- [CORRIGÉ]

    // 2. Parcourir les voisins 'v' de 'u'
    cellule *voisin = G.tab[u_idx].head; // <-- [CORRIGÉ]
    while (voisin != NULL) {
        int v_id = voisin->sommet_arrivee;
        int v_idx = v_id - 1;

        if (data[v_idx].numero == -1) {
            // Cas 1: Voisin 'v' non visité
            tarjan_parcours(v_id, G, data, pile, p_index, partition);
            // Mettre à jour le num_accessible de 'u'
            data[u_idx].numero_accessible = MIN(data[u_idx].numero_accessible, data[v_idx].numero_accessible);
        }
        else if (data[v_idx].dans_pile) {
            // Cas 2: Voisin 'v' déjà visité ET dans la pile
            // C'est un "back-edge"
            data[u_idx].numero_accessible = MIN(data[u_idx].numero_accessible, data[v_idx].numero);
        }
        voisin = voisin->suiv;
    }

    // 3. Vérifier si 'u' est la racine d'une Composante Fortement Connexe (CFC)
    if (data[u_idx].numero_accessible == data[u_idx].numero) {
        // 'u' est une racine. Dépiler jusqu'à 'u' pour former la classe.
        char nom_classe[10];
        sprintf(nom_classe, "C%d", partition->nb_classes + 1);
        t_classe cfc = creer_classe(nom_classe);

        int w_id;
        do {
            w_id = depiler(pile);
            int w_idx = w_id - 1;
            data[w_idx].dans_pile = 0;
            ajouter_sommet_classe(&cfc, w_id);
        } while (w_id != u_id);

        // Ajouter la nouvelle classe à la partition
        ajouter_classe_partition(partition, cfc);
    }
}


//
t_partition algorithme_tarjan(liste_adjacence G) {
    // Initialisations
    t_partition partition = creer_partition();
    t_tarjan_vertex *data = initialiser_tarjan_data(G);
    t_pile *pile = creer_pile(G.taille);
    int index = 0; // Compteur global pour les 'numero'

    // Boucle principale : s'assure qu'on visite tous les sommets
    for (int i = 0; i < G.taille; i++) {
        if (data[i].numero == -1) { // Si sommet (i+1) non visité
            tarjan_parcours(i + 1, G, data, pile, &index, &partition);
        }
    }

    // Nettoyage
    free(data);
    liberer_pile(pile);

    return partition;
}

// ==================================================
// === IMPLÉMENTATION - ÉTAPE 2 & 3 (À FAIRE) ======
// ==================================================
// (Ces fonctions seront implémentées dans les prochaines étapes)

int* creer_tableau_appartenance(t_partition partition, int nb_sommets_graphe) {
    printf("\n[TODO: Étape 2 - Création tableau appartenance]\n");
    return NULL;
}

t_link_array* creer_diagramme_hasse(liste_adjacence G, t_partition partition, int* tab_appartenance) {
    printf("\n[TODO: Étape 2 - Création diagramme Hasse]\n");
    return NULL;
}

void afficher_diagramme_mermaid(t_partition partition, t_link_array *liens) {
    printf("\n[TODO: Étape 2 - Affichage Mermaid]\n");
}

void analyser_graphe(t_partition partition, t_link_array *liens) {
    printf("\n[TODO: Étape 3 - Analyse du graphe]\n");
}