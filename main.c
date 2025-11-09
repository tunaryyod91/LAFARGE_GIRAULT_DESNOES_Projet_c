#include "Partie_1.h"
#include "Partie_2.h" // <-- AJOUTER
#include <string.h>

int main() {
    int choix;
    liste_adjacence G;       // structure du graphe
    int graphe_charge = 0;   // drapeau pour savoir si un graphe est déjà chargé
    char chemin[256];        // pour stocker le chemin du fichier

    // Pour stocker les résultats de la Partie 2
    t_partition partition;
    int partition_calculee = 0;

    do {
        printf("\n=============================\n");
        printf("     PROJET GRAPHES DE MARKOV\n");
        printf("=============================\n");
        printf("--- Partie 1 ---\n");
        printf("1. Charger un graphe depuis un fichier\n");
        printf("2. Afficher la liste d'adjacence\n");
        printf("3. Vérifier si le graphe est de Markov\n");
        printf("\n--- Partie 2 ---\n");
        printf("4. Calculer les classes (Tarjan)\n");
        printf("-----------------------------\n");
        printf("0. Quitter\n");
        printf("-----------------------------\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        getchar(); // consomme le \n

        switch (choix) {
            case 1: {
                printf("\nEntrez le chemin du fichier à charger (ex: ../data/exemple1.txt) : ");
                fgets(chemin, sizeof(chemin), stdin);
                // Supprimer le retour à la ligne (\n)
                chemin[strcspn(chemin, "\n")] = '\0';

                // Si un ancien graphe était chargé, libérer sa mémoire
                if (graphe_charge && partition_calculee) {
                    liberer_partition(&partition);
                    partition_calculee = 0;
                    // Note : Il faudrait aussi libérer le graphe G si readGraph allouait dynamiquement
                }

                G = readGraph(chemin);
                graphe_charge = 1;
                partition_calculee = 0; // Réinitialiser
                printf("\n✅ Graphe chargé avec succès depuis '%s'.\n", chemin);
                break;
            }

            case 2:
                if (!graphe_charge) {
                    printf("\n⚠️  Aucun graphe n'est chargé. Utilisez l'option 1 d'abord.\n");
                } else {
                    printf("\n=== Liste d'adjacence du graphe ===\n");
                    for (int i = 0; i < G.taille; i++) {
                        printf("Sommet %d : ", i + 1);
                        afficher_liste(G.tab[i]);
                    }
                }
                break;

            case 3:
                if (!graphe_charge) {
                    printf("\n⚠️  Aucun graphe n'est chargé. Utilisez l'option 1 d'abord.\n");
                } else {
                    verifier_markov(G);
                }
                break;

                // --- NOUVEAU CASE ---
            case 4:
                if (!graphe_charge) {
                    printf("\n⚠️  Aucun graphe n'est chargé. Utilisez l'option 1 d'abord.\n");
                } else {
                    // Si on a déjà calculé, on libère l'ancienne partition
                    if (partition_calculee) {
                        liberer_partition(&partition);
                    }
                    printf("\n⚙️  Calcul des composantes fortement connexes (Tarjan)...\n");
                    partition = algorithme_tarjan(G);
                    partition_calculee = 1;
                    afficher_partition(partition); // [cite: 144]
                }
                break;

            case 0:
                // Libérer la dernière partition calculée avant de quitter
                if (partition_calculee) {
                    liberer_partition(&partition);
                }
                printf("\n👋 Fin du programme. Au revoir !\n");
                break;

            default:
                printf("\n❌ Choix invalide. Réessayez.\n");
        }
    } while (choix != 0);

    return 0;
}