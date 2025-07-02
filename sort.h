#ifndef __sort_h
#define __sort_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_manager.h" /* Include la definizione delle struct e costanti*/ /* Doxygen++; :) */



/* === Funzioni di Quicksort per Nomi di Progetto (alfabetico) === */


/**
 * @brief Funzione di partizione per il quicksort alfabetico di stringhe.
 * @param project_names Array di puntatori a stringa (nomi di progetto).
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 * @return La posizione finale del pivot.
 */
int partition_alphabetical(char **project_names, int low, int high);

/**
 * @brief Implementazione del quicksort per ordinare alfabeticamente un array di nomi di progetti.
 * @param project_names Array di puntatori a stringa (nomi di progetto).
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 */
void qsort_alfabetico(char **project_names, int low, int high);


/* === Funzioni di Quicksort per Contatori (numerico e alfabetico) === */

/**
 * @brief Funzione di partizione per il quicksort di contatori.
 * Ordina in base al campo 'cont' (decrescente) e poi 'indice' (crescente).
 * @param counters Array di puntatori a contatore.
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 * @param order Indica il tipo di ordinamento (0 per decrescente cont, crescente indice).
 * @return La posizione finale del pivot.
 */
int partition_numerical_alphabetical(contatore **counters, int low, int high, int order);

/**
 * @brief Implementazione del quicksort per ordinare un array di contatori.
 * L'ordinamento è primariamente numerico (decrescente o crescente) e secondariamente per indice (crescente).
 * @param counters Array di puntatori a contatore.
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 * @param order Indica il tipo di ordinamento (0 per decrescente cont, crescente indice).
 */
void qsort_punt_lescg(contatore **counters, int low, int high, int order);

/**
 * @brief Funzione di partizione per il quicksort di contatori Condorcet.
 * Ordina per appartenenza alla componente connessa e poi lessicograficamente.
 * @param counters Array di puntatori a contatore.
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 * @param matrix Matrice di Condorcet usata per i confronti tra componenti.
 * @return La posizione finale del pivot.
 */
int partition_components(contatore **counters, int low, int high, vint *matrix);

/**
 * @brief Implementazione del quicksort per ordinare i contatori in base alle componenti connesse Condorcet.
 * @param counters Array di puntatori a contatore.
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 * @param matrix Matrice di Condorcet usata per i confronti tra componenti.
 */
void qsort_componenti(contatore **counters, int low, int high, vint *matrix);


/* === Funzioni di Quicksort per Componenti del Clustering === */

/**
 * @brief Scambia due puntatori a strutture 'componente'.
 * @param components Array di puntatori a componente.
 * @param i Indice del primo elemento da scambiare.
 * @param j Indice del secondo elemento da scambiare.
 */
void swap_components(componente **components, int i, int j);

/**
 * @brief Scambia due puntatori a stringa (nomi di progetto).
 * @param a Puntatore al primo puntatore a stringa.
 * @param b Puntatore al secondo puntatore a stringa.
 */
void swap_string_pointers(char **a, char **b);
/**
 * @brief Scambia due puntatori a strutture 'contatore'.
 * @param counters Array di puntatori a contatore.
 * @param i Indice del primo elemento da scambiare.
 * @param j Indice del secondo elemento da scambiare.
 */
void swap_counters(contatore **counters, int i, int j);

/**
 * @brief Funzione di partizione per il quicksort delle componenti del clustering.
 * Ordina per rappresentante del cluster (alfabeticamente) e poi per distanza minima.
 * @param components Array di puntatori a componente.
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 * @param citizens_data Array di strutture cittadino (per confronto dei codici).
 * @return La posizione finale del pivot.
 */
 
int partition_cluster_components(componente **components, int low, int high, cittadino **citizens_data);

/**
 * @brief Implementazione del quicksort per ordinare le componenti del clustering.
 * @param components Array di puntatori a componente.
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 * @param citizens_data Array di strutture cittadino (per confronto dei codici).
 */
void qsort_clustering(componente **components, int low, int high, cittadino **citizens_data);

#endif /* __sort_h */
