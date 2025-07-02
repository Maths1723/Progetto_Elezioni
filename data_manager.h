#ifndef __data_manager_h
#define __data_manager_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Definizione delle costanti */
#ifndef NUM_PARAM_INIZIALI
#define NUM_PARAM_INIZIALI 3
#endif
#ifndef ROW_LENGTH
#define ROW_LENGTH 256
#endif
#ifndef PROG_LENGTH
#define PROG_LENGTH 30
#endif
#ifndef CF_LENGTH
#define CF_LENGTH 16
#endif
/* Strutture dati astratte */

typedef int* vint;
typedef char* vchar;

/* Definizione della struttura per i parametri di input iniziali */
typedef struct {
    int num_abitanti;
    int num_progetti;
    int num_gruppi;
} ParametriInput;
struct cittadino {
    char CF[CF_LENGTH + 1];
    vint progs; /* Array di indici dei progetti preferiti */
    vchar pref;  /* Array di operatori di preferenza (es. '>', '<') */
};
struct componente {
    int min; /* Valore minimo di distanza */
    int ind; /* Indice dell'elemento */
    int rappr; /* Rappresentante del cluster */
};
struct contatore {
    int cont;   /* Contatore del punteggio */
    int indice; /* Indice associato (es. indice del progetto) */
};
typedef struct cittadino cittadino;
typedef struct componente componente;
typedef struct contatore contatore;



/* ------------------------ */
/* Prototipi delle funzioni */
/* ------------------------ */

/* === Sezione di Gestione Iniziale Dati e Allocazione === */

/* Procedura principale per l'acquisizione di tutti i dati dal file */
void fetch_all_data(const char *filename, ParametriInput *params, cittadino ***citizens, char ***projects);



/* === Sezione di Allocazione per Metodi di Votazione e Clustering === */

/* Alloca e inizializza un array di puntatori a strutture 'contatore' */
contatore **initialize_counter_array(int count);

/* Alloca una matrice di interi di dimensioni rows x cols */
vint *allocate_int_matrix(int rows, int cols);

/* Alloca e inizializza un array di puntatori a strutture 'componente' */
componente **initialize_component_array(int count);


/* === Sezione di Deallocazione Completa === */

/* Dealloca tutta la memoria dinamica utilizzata dal programma */
void free_all_resources(int num_citizens, int num_projects, char **projects_data, cittadino **citizens_data,
                        contatore **method_counters, vint *borda_matrix, vint *condorcet_matrix, vint *distance_matrix);

#endif
