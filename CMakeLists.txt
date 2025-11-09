cmake_minimum_required(VERSION 3.24)
project(LAFARGE_GIRAULT_DESNOES_Projet_c LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(LAFARGE_GIRAULT_DESNOES_Projet_c
        main.c
        Partie_1.h
        Partie_1.c
        Partie_2.h  # <-- AJOUTER
        Partie_2.c  # <-- AJOUTER
        utils.h
        utils.c     # <-- AJOUTER (il manquait .c)
        hasse.h
        hasse.c
        Partie_2.c
)