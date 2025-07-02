#include "data_manager.h"  /* Include le definizioni delle struct */
#include "sort.h"          /* Include i prototipi delle funzioni di ordinamento */


/* === Funzioni di Quicksort === */			/* Doxygen++; :) */
/**
 * @brief Implementazione del quicksort per ordinare le componenti del clustering.
 * @param components Array di puntatori a componente.
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 * @param citizens_data Array di strutture cittadino (per confronto dei codici).
 */
void qsort_clustering(componente **components, int low, int high, cittadino **citizens_data) {
    if (low < high) {
        int pivot_pos = partition_cluster_components(components, low, high, citizens_data);
        qsort_clustering(components, low, pivot_pos - 1, citizens_data);
        qsort_clustering(components, pivot_pos + 1, high, citizens_data);
    }
}
/**
 * @brief Implementazione del quicksort per ordinare alfabeticamente un array di nomi di progetti.
 * @param project_names Array di puntatori a stringa (nomi di progetto).
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 */
void qsort_alfabetico(char **project_names, int low, int high) {
    if (low < high) {
        int pivot_pos = partition_alphabetical(project_names, low, high);
        qsort_alfabetico(project_names, low, pivot_pos - 1);
        qsort_alfabetico(project_names, pivot_pos + 1, high);
    }
}
/**
 * @brief Implementazione del quicksort per ordinare un array di contatori.
 * L'ordinamento è primariamente numerico (decrescente o crescente) e secondariamente per indice (crescente).
 * @param counters Array di puntatori a contatore.
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 * @param order Indica il tipo di ordinamento (0 per decrescente cont, crescente indice).
 */
void qsort_punt_lescg(contatore **counters, int low, int high, int order) {
    if (low < high) {
        int pivot_pos = partition_numerical_alphabetical(counters, low, high, order);
        qsort_punt_lescg(counters, low, pivot_pos - 1, order);
        qsort_punt_lescg(counters, pivot_pos + 1, high, order);
    }
}
/**
 * @brief Implementazione del quicksort per ordinare i contatori in base alle componenti connesse Condorcet.
 * @param counters Array di puntatori a contatore.
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 * @param matrix Matrice di Condorcet usata per i confronti tra componenti.
 */
void qsort_componenti(contatore **counters, int low, int high, vint *matrix) {
    if (low < high) {
        int pivot_pos = partition_components(counters, low, high, matrix);
        qsort_componenti(counters, low, pivot_pos - 1, matrix);
        qsort_componenti(counters, pivot_pos + 1, high, matrix);
    }
}


/* === Funzioni di Partizione === */

/**
 * @brief Funzione di partizione per il quicksort di contatori Condorcet.
 * Ordina per appartenenza alla componente connessa e poi lessicograficamente.
 * @param counters Array di puntatori a contatore.
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 * @param matrix Matrice di Condorcet usata per i confronti tra componenti.
 * @return La posizione finale del pivot.
 */
int partition_components(contatore **counters, int low, int high, vint *matrix) {
    int pivot_idx = low,i;
    for (i = low + 1; i <= high; ++i) {
        /* Stessa componente connessa (stesso valore 'cont'), ordina per indice (alfabeticamente implicito) */
        if (counters[i]->cont == counters[low]->cont) {
            if (counters[i]->indice < counters[low]->indice) {
                pivot_idx++;
                swap_counters(counters, i, pivot_idx);
            }
        }
        /* Componenti diverse, ordina in base alla relazione nella matrice Condorcet */
        else {
            /* Se i vince su s (pivot), lo si mette prima */
            if (matrix[counters[i]->indice][counters[low]->indice] > matrix[counters[low]->indice][counters[i]->indice]) {
                pivot_idx++;
                swap_counters(counters, i, pivot_idx);
            }
        }
    }
    swap_counters(counters, low, pivot_idx);
    return pivot_idx;
}



/**
 * @brief Funzione di partizione per il quicksort delle componenti del clustering.
 * Ordina per rappresentante del cluster (alfabeticamente) e poi per distanza minima.
 * @param components Array di puntatori a componente.
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 * @param citizens_data Array di strutture cittadino (per confronto dei codici).
 * @return La posizione finale del pivot.
 */
int partition_cluster_components(componente **components, int low, int high, cittadino **citizens_data) {
    int pivot_idx = low,i;
    for (i = low + 1; i <= high; ++i) {
        /* Se hanno lo stesso rappresentante */
        if (components[i]->rappr == components[low]->rappr) {
            /* Ordina prima il rappresentante (distanza min = 0), se il pivot non lo è */
            if (components[i]->min == 0) { /* Questo elemento è un rappresentante */
                pivot_idx++;
                swap_components(components, i, pivot_idx);
            }
            /* Se entrambi non sono rappresentanti o entrambi lo sono, ordina per codice cittadino */
            else if (strcmp(citizens_data[components[i]->ind]->CF, citizens_data[components[low]->ind]->CF) < 0) {
                pivot_idx++;
                swap_components(components, i, pivot_idx);
            }
        }
        /* Se hanno rappresentanti diversi, ordina per codice del rappresentante (alfabeticamente) */
        else {
            if (strcmp(citizens_data[components[i]->rappr]->CF, citizens_data[components[low]->rappr]->CF) < 0) {
                pivot_idx++;
                swap_components(components, i, pivot_idx);
            }
        }
    }
    swap_components(components, low, pivot_idx);
    return pivot_idx;
}
/**
 * @brief Funzione di partizione per il quicksort di contatori.
 * Ordina in base al campo 'cont' (decrescente) e poi 'indice' (crescente).
 * @param counters Array di puntatori a contatore.
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 * @param order Indica il tipo di ordinamento (0 per decrescente cont, crescente indice).
 * @return La posizione finale del pivot.
 */
int partition_numerical_alphabetical(contatore **counters, int low, int high, int order) {
    int pivot_idx = low, i;
    /** Si assume che 'order' sia 0 per ordinamento decrescente sul contatore e crescente sull'indice
     *	L'implementazione originale gestiva solo 'ord == 0' e un 'if' commentato per 'ord == 1'.
     *  Mantengo il comportamento originale (decrescente cont, crescente indice).
     */
    for (i = low + 1; i <= high; ++i) {
        /* Ordine decrescente per 'cont' */
        if (counters[i]->cont > counters[low]->cont) {
            pivot_idx++;
            swap_counters(counters, i, pivot_idx);
        }
        /* In caso di parità di 'cont', ordina per 'indice' (crescente) */
        else if (counters[i]->cont == counters[low]->cont) {
            if (counters[i]->indice < counters[low]->indice) {
                pivot_idx++;
                swap_counters(counters, i, pivot_idx);
            }
        }
    }
    swap_counters(counters, low, pivot_idx);
    return pivot_idx;
}

/**
 * @brief Funzione di partizione per il quicksort alfabetico di stringhe.
 * @param project_names Array di puntatori a stringa (nomi di progetto).
 * @param low Indice di inizio della sotto-array.
 * @param high Indice di fine della sotto-array.
 * @return La posizione finale del pivot.
 */
int partition_alphabetical(char **project_names, int low, int high) {
    int pivot_idx = low, i; /* Sceglie il primo elemento come pivot */
    for (i = low + 1; i <= high; ++i) {
        /* Ordine crescente alfabeticamente */
        if (strcmp(project_names[i], project_names[low]) < 0) {
            pivot_idx++;
            swap_string_pointers(&project_names[i], &project_names[pivot_idx]);
        }
    }
    swap_string_pointers(&project_names[low], &project_names[pivot_idx]);
    return pivot_idx;
}


/* === Funzioni di Ausiliarie - Swap === */


/**
 * @brief Scambia due puntatori a stringa (nomi di progetto).
 * @param a Puntatore al primo puntatore a stringa.
 * @param b Puntatore al secondo puntatore a stringa.
 */
void swap_string_pointers(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}
/**
 * @brief Scambia due puntatori a strutture 'componente'.
 * @param components Array di puntatori a componente.
 * @param i Indice del primo elemento da scambiare.
 * @param j Indice del secondo elemento da scambiare.
 */
void swap_components(componente **components, int i, int j) {  /* l'ho preferito a passare i puntatori a puntatore e scambiare gli indirizzi così puntati :o */
    componente *temp = components[i];
    components[i] = components[j];
    components[j] = temp;
}
/**
 * @brief Scambia due puntatori a strutture 'contatore'.
 * @param counters Array di puntatori a contatore.
 * @param i Indice del primo elemento da scambiare.
 * @param j Indice del secondo elemento da scambiare.
 */
void swap_counters(contatore **counters, int i, int j) { /* l'ho preferito a passare i puntatori a puntatore e scambiare gli indirizzi così puntati :o */
    contatore *temp = counters[i];
    counters[i] = counters[j];
    counters[j] = temp;
}

