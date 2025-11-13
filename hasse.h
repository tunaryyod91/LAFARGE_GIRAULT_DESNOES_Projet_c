#ifndef __HASSE_H__
#define __HASSE_H__



typedef struct {
    int from; // Classe de départ
    int to;   // Classe d'arrivée
} t_link;


typedef struct {
    t_link *links;
    int log_size; // Taille logique (utilisée par hasse.c)
    int phy_size; // Taille physique (pour nos realloc)
} t_link_array;


void removeTransitiveLinks(t_link_array *p_link_array);

#endif