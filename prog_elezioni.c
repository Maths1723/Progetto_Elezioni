/* classic libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* support code*/
#include "data_manager.h"
#include "sort.h"
#include "metodi.h"
/* Doxygen++; :) */


/* Permette di ricevere nome file di input da linea di comando o durante l'esecuzione */
void handle_arguments(int argc, char *argv[], char *input_filepath);

/* programma principale */
int main(int argc, char *argv[]) {
	
    /* ----------- */
    /* allocazione */
    /* ----------- */
    char input_filepath[ROW_LENGTH];
    cittadino **citizens_data = NULL;
    char **project_names_list = NULL;
    ParametriInput app_params;
    int i;
    /* creazione delle matrici per i metodi di Borda, Condorcet e di calcolo nel clustering */
    vint *borda_pref_matrix = NULL, *condorcet_comparison_matrix = NULL, *cluster_distance_matrix = NULL;
	contatore **method_score_counters = NULL;

    /* ------------ */
    /* elaborazione */
    /* ------------ */
    handle_arguments(argc, argv, input_filepath);
    fetch_all_data(input_filepath, &app_params, &citizens_data, &project_names_list);

    /* creazione array di puntatori a struct contatori */
    method_score_counters = initialize_counter_array(app_params.num_progetti);
    
    borda_pref_matrix = allocate_int_matrix(app_params.num_abitanti, app_params.num_progetti);
    condorcet_comparison_matrix = allocate_int_matrix(app_params.num_progetti, app_params.num_progetti);
    cluster_distance_matrix = allocate_int_matrix(app_params.num_abitanti, app_params.num_abitanti);

    /* raccolta dei dati per i tre metodi */
    for (i = 0; i < app_params.num_abitanti; ++i) {
        aggregate_plurality_data(*citizens_data[i], method_score_counters);
        insert_borda_scores(*citizens_data[i], i, borda_pref_matrix, app_params.num_progetti);
        aggregate_condorcet_data(*citizens_data[i], condorcet_comparison_matrix, app_params.num_progetti);
    }

    /* elaborazione dei dati raccolti e display dei risultati */
    calculate_and_display_plurality(method_score_counters, project_names_list, app_params.num_progetti);
    calculate_and_display_borda(borda_pref_matrix, method_score_counters, project_names_list, app_params.num_abitanti, app_params.num_progetti);
    calculate_and_display_condorcet(condorcet_comparison_matrix, method_score_counters, project_names_list, app_params.num_progetti);
    perform_greedy_clustering(borda_pref_matrix, cluster_distance_matrix, app_params.num_abitanti, app_params.num_progetti, app_params.num_gruppi, citizens_data);

    /* ------------------------ */
    /* deallocazione e chiusura */
    /* ------------------------ */
    free_all_resources(app_params.num_abitanti, app_params.num_progetti, project_names_list, citizens_data,
                       method_score_counters, borda_pref_matrix, condorcet_comparison_matrix, cluster_distance_matrix);
    
    return EXIT_SUCCESS;
}

/**
 * @brief Gestisce gli argomenti da linea di comando o richiede il nome del file di input.
 * @param argc Il numero di argomenti da linea di comando.
 * @param argv L'array degli argomenti da linea di comando.
 * @param input_filepath Buffer dove memorizzare il percorso del file.
 */
void handle_arguments(int argc, char *argv[], char *input_filepath) {
    if (argc != 2) {
        fprintf(stderr, "Nessun file di input specificato. Inserire il nome del file: ");
        /*
         * Per conformità a C89 e per evitare sistemi troppo complicati, si deve assumere
         * una dimensione massima di input, ad esempio 255 caratteri + terminatore. POSSIBILE BUFFER OVERFLOW
         */
        if (fgets(input_filepath, 256, stdin) == NULL) { /* Assuming a max length for the filename to fit in a common buffer */
            fprintf(stderr, "ERRORE: Errore durante la lettura del nome del file.\n");
            exit(EXIT_FAILURE);
        }
        input_filepath[strcspn(input_filepath, "\n")] = '\0'; /* Rimuove il newline finale */

        if (strlen(input_filepath) == 0) {
            fprintf(stderr, "ERRORE: Nome del file non valido o vuoto.\n");
            fprintf(stderr, "Utilizzo: %s [nome_file_input]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    } else {
        /*
         * Copia il nome del file dall'argomento della riga di comando.
         * Si assume che il buffer input_filepath sia sufficientemente grande
         * per contenere argv[1].
         */
        strcpy(input_filepath, argv[1]);
    }
}
