#include "data_manager.h"
#include "sort.h"
#include "grafo.h"      /* Contiene le funzioni relative al grafo per Condorcet */
#include "metodi.h"		/* Doxygen++; :) */

/* ------------------------------Procedure sui dati preparati------------------------------ */
/* ----------------------------Garantisce modifiche più semplici--------------------------- */

/*
 * @brief Implementa il sistema di pluralità e stampa i risultati.
 * @param counters Array di contatori per i progetti.
 * @param project_names Array dei nomi dei progetti.
 * @param num_projects Numero totale di progetti.
 */
void calculate_and_display_plurality(contatore **counters, vchar *project_names, int num_projects) {
    qsort_punt_lescg(counters, 0, num_projects - 1, 0); /* Ordina per conteggio, in caso di parità alfabeticamente */
    display_method_results(counters, project_names, num_projects, 0);
}

/*
 * @brief Calcola i punteggi del metodo Borda e stampa i risultati.
 * @param borda_matrix Matrice dei punteggi Borda per cittadino.
 * @param counters Array di contatori per i progetti.
 * @param project_names Array dei nomi dei progetti.
 * @param num_citizens Numero di abitanti.
 * @param num_projects Numero di progetti.
 */
void calculate_and_display_borda(vint *borda_matrix, contatore **counters, vchar *project_names, int num_citizens, int num_projects) {
	    int i,j;
    reset_project_counters(counters, num_projects); /* Resetta i contatori prima di accumulare i punteggi */
    for (j = 0; j < num_projects; ++j) {
        for (i = 0; i < num_citizens; ++i) {
            counters[j]->cont += borda_matrix[i][j]; /* Somma i punteggi Borda per ciascun progetto */
        }
    }
    qsort_punt_lescg(counters, 0, num_projects - 1, 0); /* Ordina i risultati finali di Borda */

    display_method_results(counters, project_names, num_projects, 1);
}
/*
 * @brief Calcola i risultati del metodo Condorcet e stampa il vincitore/ordine.
 * @param condorcet_matrix Matrice di confronto Condorcet.
 * @param counters Array di contatori per i progetti.
 * @param project_names Array dei nomi dei progetti.
 * @param num_projects Numero di progetti.
 */
void calculate_and_display_condorcet(vint *condorcet_matrix, contatore **counters, vchar *project_names, int num_projects) {
    grafo condorcet_graph; /* Istanza del grafo per Condorcet */
    int i,j;
    int num_connected_components = 0;
    /* Inizializzazione del grafo */
    creagrafo(num_projects, &condorcet_graph);
    for (i = 0; i < num_projects - 1; ++i) {
        for (j = i + 1; j < num_projects; ++j) {
            if (condorcet_matrix[i][j] > condorcet_matrix[j][i]) {
                insarco(i, j, &condorcet_graph); /* i "vince" su j */
            } else if (condorcet_matrix[i][j] == condorcet_matrix[j][i]) {
                insarco(i, j, &condorcet_graph); /* Parità: arco in entrambe le direzioni */
                insarco(j, i, &condorcet_graph);
            } else {
                insarco(j, i, &condorcet_graph);
            }
        }
    }

    reset_project_counters(counters, num_projects); /* Resetta i contatori per l'uso con le componenti connesse */


    /* Determina le componenti fortemente connesse del grafo (i cicli Condorcet), equivalente al prob rich. */
    ComponentiFortementeConnesse(&condorcet_graph, counters, &num_connected_components);

    /* Ordina i contatori in base alle componenti connesse e poi per indice */
    qsort_componenti(counters, 0, num_projects - 1, condorcet_matrix);

    /* Stampa e liberazione memoria */
    display_method_results(counters, project_names, num_projects, 2);
    distruggegrafo(&condorcet_graph);
}

/*
 * @brief Esegue l'algoritmo di clustering greedy e stampa i gruppi risultanti.
 * @param borda_scores_matrix Matrice dei punteggi Borda (usata per calcolare le distanze).
 * @param distance_matrix Matrice delle distanze tra i cittadini.
 * @param num_citizens Numero di abitanti.
 * @param num_projects Numero di progetti.
 * @param desired_groups Numero di gruppi desiderati.
 * @param citizens_data Array di strutture cittadino.
 */
void perform_greedy_clustering(vint *borda_scores_matrix, vint *distance_matrix, int num_citizens, int num_projects, int desired_groups, cittadino **citizens_data) {
    /* Calcolo della matrice delle distanze tra i cittadini */
    int i,j,k, new_representative_idx;
    int min_total_distance = 1000000;
    int initial_representative_idx = -1;
    int remaining_groups_to_find = desired_groups - 1;
    contatore **distance_sum_counters = NULL;
    componente **cluster_components = NULL;
        
    for (i = 0; i < num_citizens - 1; ++i) {
        for (j = i + 1; j < num_citizens; ++j) {
            int current_distance = 0;
            for (k = 0; k < num_projects; ++k) {
                /* Distanza  == MANHATTAN == (o L1 norm) tra i vettori di punteggi Borda */
                current_distance += abs(borda_scores_matrix[i][k] - borda_scores_matrix[j][k]);
            }
            distance_matrix[i][j] = current_distance;
            distance_matrix[j][i] = current_distance; /* La matrice delle distanze è simmetrica */
        }
    }


    distance_sum_counters = initialize_counter_array(num_citizens); /* Inizializza contatori per le somme delle distanze */

    /* Calcola la somma delle distanze di ogni cittadino da tutti gli altri */
    for (i = 0; i < num_citizens; ++i) {
        for (k = 0; k < num_citizens; ++k) {
            distance_sum_counters[i]->cont += distance_matrix[i][k];
        }
        distance_sum_counters[i]->indice = i; /* L'indice rappresenta l'ID del cittadino */
    }
    
    /* Trova il cittadino con la somma delle distanze minima (primo rappresentante del cluster) */


    for (i = 0; i < num_citizens; ++i) {
        int current_score = distance_sum_counters[i]->cont;
        if (initial_representative_idx == -1 || current_score < min_total_distance) {
            min_total_distance = current_score;
            initial_representative_idx = distance_sum_counters[i]->indice;
        } else if (current_score == min_total_distance) {
            /* In caso di parità, ordina lessicograficamente per codice cittadino */
            if (strcmp(citizens_data[distance_sum_counters[i]->indice]->CF, citizens_data[initial_representative_idx]->CF) < 0) {
                initial_representative_idx = distance_sum_counters[i]->indice;
            }
        }
    }
    
    /* Inizializza le componenti dei cluster */
    cluster_components = initialize_component_array(num_citizens);

    /* Assegna tutti i cittadini al primo rappresentante */
    for (k = 0; k < num_citizens; ++k) {
        cluster_components[k]->rappr = initial_representative_idx;
        cluster_components[k]->min = distance_matrix[initial_representative_idx][k]; /* Distanza dal rappresentante */
    }

    while (remaining_groups_to_find > 0) {
        int active_citizens_count = 0;
        /* Ricostruisce distance_sum_counters solo per i cittadini non ancora assegnati al loro cluster finale */
        for (i = 0; i < num_citizens; ++i) {
            /* Se min è 0, significa che 'i' è già un rappresentante o è a distanza 0 da un rappresentante */
            if (cluster_components[i]->min != 0) {
                /* Calcola la somma delle distanze per ogni cittadino rispetto ai rappresentanti attuali */
                distance_sum_counters[active_citizens_count]->cont = 0;
                distance_sum_counters[active_citizens_count]->indice = i;
                for (k = 0; k < num_citizens; ++k) {
                    if (distance_matrix[i][k] < cluster_components[k]->min) {
                        distance_sum_counters[active_citizens_count]->cont += distance_matrix[i][k];
                    } else {
                        distance_sum_counters[active_citizens_count]->cont += cluster_components[k]->min;
                    }
                }
                active_citizens_count++;
            }
        }

        /* Trova il nuovo rappresentante tra i cittadini non ancora assegnati, si poteva inserire nel while ma si è preferita la leggibilità del codice */
        min_total_distance = -1;
        new_representative_idx = -1;
        
        for (i = 0; i < active_citizens_count; ++i) {
            int current_score = distance_sum_counters[i]->cont;
            if (new_representative_idx == -1 || current_score < min_total_distance) {
                min_total_distance = current_score;
                new_representative_idx = distance_sum_counters[i]->indice;
            } else if (current_score == min_total_distance) {
                if (strcmp(citizens_data[distance_sum_counters[i]->indice]->CF, citizens_data[new_representative_idx]->CF) < 0) {
                    new_representative_idx = distance_sum_counters[i]->indice;
                }
            }
        }

        /* Aggiorna le assegnazioni dei cluster con il nuovo rappresentante */
        for (k = 0; k < num_citizens; ++k) {
            if (distance_matrix[new_representative_idx][k] < cluster_components[k]->min) {
                cluster_components[k]->rappr = new_representative_idx;
                cluster_components[k]->min = distance_matrix[new_representative_idx][k];
            } else if (distance_matrix[new_representative_idx][k] == cluster_components[k]->min) {
                /* In caso di parità di distanza, preferisci il rappresentante con codice lessicograficamente minore */
                if (strcmp(citizens_data[new_representative_idx]->CF, citizens_data[cluster_components[k]->rappr]->CF) < 0) {
                    cluster_components[k]->rappr = new_representative_idx;
                }
            }
        }
        remaining_groups_to_find--;
    }
    
    /* Ordina i componenti del cluster per la stampa */
    qsort_clustering(cluster_components, 0, num_citizens - 1, citizens_data);
    print_cluster_output(cluster_components, citizens_data, num_citizens, desired_groups); /* desired_groups è il numero finale di gruppi */
    
    /* Libera la memoria allocata internamente a questa funzione */
    /* La memoria per `indici` non è più necessaria dato che `distance_sum_counters` è usato per lo stesso scopo. */
    for (i = 0; i < num_citizens; ++i) {
        free(distance_sum_counters[i]);
        free(cluster_components[i]);
    }
    free(distance_sum_counters);
    free(cluster_components);
}


/* ------------------------------Preparazione dati------------------------------ */

/*
 * @brief Aggrega i dati di preferenza di un cittadino per il metodo di pluralità.
 * @param citizen La struttura del cittadino le cui preferenze sono da aggiungere.
 * @param counters Array di contatori da aggiornare.
 */
void aggregate_plurality_data(cittadino citizen, contatore **counters) {
    int i = 0;
    do {
        counters[citizen.progs[i]]->cont++; /* Incrementa il conteggio per il progetto preferito */
        i++;
    } while (i < PROG_LENGTH && citizen.pref[i - 1] == '='); /* Continua se ci sono preferenze pari */
}

/*
 * @brief Aggrega i dati di preferenza di un cittadino per il metodo Condorcet.
 * @param citizen La struttura del cittadino.
 * @param condorcet_matrix La matrice Condorcet da aggiornare.
 * @param num_projects Numero totale di progetti.
 */
void aggregate_condorcet_data(cittadino citizen, vint *condorcet_matrix, int num_projects) {
	int i,j,l;
    for (i = 0; i < num_projects; ++i) {
        for (j = i + 1; j < num_projects; ++j) {
            /* Trova la posizione relativa di persona.progs[i] e persona.progs[j] nelle preferenze */
            /* Se persona.progs[i] è preferito a persona.progs[j] */
            /* Incrementa matrice[persona.progs[i]][persona.progs[j]] */
            /* Questa logica dipende dalla struttura esatta di 'op' e 'progs'. */
            if (citizen.pref[i] != '=') { /* Se non c'è parità con il progetto precedente */
                condorcet_matrix[citizen.progs[i]][citizen.progs[j]]++;
            } else { /* Se c'è parità */
                int k = i;
                /* Trova il primo progetto non in parità con persona.progs[i] */
                while (k < num_projects - 1 && citizen.pref[k] == '=') {
                    k++;
                }
                /* Tutti i progetti da persona.progs[i] fino a k (esclusi) sono in parità. */
                /* Incrementa la preferenza per persona.progs[i] vs tutti i progetti successivi a k. */
                for (l = k; l < num_projects; ++l) {
                    condorcet_matrix[citizen.progs[i]][citizen.progs[l]]++;
                }
            }
        }
    }
}

/*
 * @brief Inserisce i punteggi Borda di un cittadino nella matrice Borda.
 * @param citizen La struttura del cittadino.
 * @param citizen_idx L'indice del cittadino nella matrice.
 * @param borda_matrix La matrice Borda da popolare.
 * @param num_projects Numero totale di progetti.
 */
void insert_borda_scores(cittadino citizen, int citizen_idx, vint *borda_matrix, int num_projects) {
    int project_iter = 0;
    int current_points = num_projects; /* Inizia con il massimo dei punti (num_projects per la prima posizione) */

    do {
        int start_group_idx = project_iter;
        /* Trova tutti i progetti con lo stesso ranking (legati da '=') */
        while (project_iter < num_projects && (project_iter == start_group_idx || citizen.pref[project_iter - 1] == '=')) {
            /* Assegna lo stesso punteggio a tutti i progetti con lo stesso ranking */
            borda_matrix[citizen_idx][citizen.progs[project_iter]] = current_points;
            project_iter++;
        }
        /* I punti per il prossimo gruppo di progetti sono 'num_projects' meno il numero di progetti già assegnati */
        current_points = num_projects - project_iter;
    } while (current_points >= 1 && project_iter < num_projects); /* Continua finché ci sono punti da assegnare e progetti */
}

/* Funzioni ausiliarie e di stampa */

/*
 * @brief Stampa i risultati per i metodi di votazione (Pluralità, Borda, Condorcet).
 * @param counters Array di contatori con i risultati.
 * @param project_names Array dei nomi dei progetti.
 * @param num_elements Numero di elementi da stampare (progetti).
 * @param method_type Tipo di metodo (1=Pluralità, 2=Borda, 3=Condorcet).
 */
void display_method_results(contatore **counters, vchar *project_names, int num_elements, int method_type) {
	int current_idx = 0;
    if (method_type == 0) {
        printf("Risultati: Sistema di Pluralita'\n");
    } else if (method_type == 1) {
        printf("Risultati: Metodo di Borda\n");
    } else if (method_type == 2) {
        printf("Risultati: Metodo di Condorcet\n");
    }


    do {
        if (method_type == 0 || method_type == 1) {
            printf("%d ", counters[current_idx]->cont); /* Stampa il punteggio */
        }
        
        /* Stampa i nomi dei progetti che hanno lo stesso punteggio */
        do {
            printf("%s ", project_names[counters[current_idx]->indice]);
            current_idx++;
        } while (current_idx < num_elements && counters[current_idx]->cont == counters[current_idx - 1]->cont);

        printf("\n");
    } while (current_idx < num_elements);

    printf("\n");
}

/*
 * @brief Stampa i risultati del clustering.
 * @param min_components Array di componenti clusterizzate.
 * @param citizens_data Array di strutture cittadino.
 * @param num_citizens Numero di abitanti.
 * @param num_actual_groups Il numero effettivo di gruppi trovati.
 */
void print_cluster_output(componente **min_components, cittadino **citizens_data, int num_citizens, int num_actual_groups) {
    /* Allocate memory for group distance sums, plus an element for the total sum (index 0) */
    int group_idx, representative_identifier, current_component_index;
    int *group_sums = (int *)calloc(num_actual_groups + 1, sizeof(int)); /* +1 for total_sum at sums[0] */
    if (!group_sums) {
        fprintf(stderr, "ERROR: Failed to allocate memory for group sum array.\n");
        exit(EXIT_FAILURE);
    }

    current_component_index = 0;
    
    /* Calculate the sums of distances within each group */
    for (group_idx = 1; group_idx <= num_actual_groups; ++group_idx) {
        	representative_identifier = min_components[current_component_index]->rappr;
        
        /* Iterate through components belonging to the current representative */
        for (; current_component_index < num_citizens && min_components[current_component_index]->rappr == representative_identifier; ++current_component_index) {
            group_sums[group_idx] += min_components[current_component_index]->min;
            group_sums[0] += min_components[current_component_index]->min; /* Add to overall total distance */
        }
    }

    printf("Gruppi (%d)\n", group_sums[0]); /* Print the total sum of distances */

    current_component_index = 0; /* Reset index for the printing phase */
    
    /* Print the details of each group */
    for (group_idx = 1; group_idx <= num_actual_groups; ++group_idx) {
        	representative_identifier = min_components[current_component_index]->rappr;
        printf("%s (%d) ", citizens_data[representative_identifier]->CF, group_sums[group_idx]);
        
        /* Print members of the current group */
        for (; current_component_index < num_citizens && min_components[current_component_index]->rappr == representative_identifier; ++current_component_index) {
            /* Only print if the member is not the representative itself */
            if (min_components[current_component_index]->ind != representative_identifier) {
                printf("%s ", citizens_data[min_components[current_component_index]->ind]->CF);
            }
        }
        printf("\n"); /* Newline after each group's members */
    }

    printf("\n");
    free(group_sums); /* Free the allocated memory */
}


/* ------------------------------Ausiliarie------------------------------ */

/*
 * @brief Resetta i contatori dei progetti a zero e i loro indici.
 * @param counters Array di contatori da resettare.
 * @param num_projects Numero di progetti.
 */
void reset_project_counters(contatore **counters, int num_projects) {
	int j;
    for (j = 0; j < num_projects; ++j) {
        counters[j]->indice = j; /* Resetta l'indice al suo valore originale */
        counters[j]->cont = 0;    /* Azzera il conteggio */
    }
}
