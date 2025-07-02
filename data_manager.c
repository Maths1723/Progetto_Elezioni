#include "data_manager.h"
#include "sort.h"          
/* Doxygen++; :) */
/* === Sezione di Gestione Iniziale Dati e Allocazione === */
/* Funzioni ausiliarie per la gestione dei dati iniziali */
static FILE *attempt_file_open(const char *path_to_file);
static void read_initial_params(FILE *fp, ParametriInput *params);
static cittadino **allocate_citizen_array(int count);
static char **allocate_project_names_array(int count);
static void load_project_names(FILE *fp, int num_progetti, char **project_names);
static int find_project_index_binary(char **project_list, int low, int high, const char *target_name);
static void populate_citizen_data(FILE *fp, cittadino **citizens, int num_citizens, int num_projects, char **project_names);
static void close_data_file(FILE *fp);

/**
 * @brief Tenta l'apertura di un file in modalità lettura.
 * @param path_to_file Percorso del file da aprire.
 * @return Puntatore al FILE aperto. Esci con errore se l'apertura fallisce.
 */
static FILE *attempt_file_open(const char *path_to_file) {
    FILE *fp = fopen(path_to_file, "r");
    if (!fp) {
        fprintf(stderr, "ERRORE: Impossibile aprire il file '%s'. Controllare percorso o permessi.\n", path_to_file);
        exit(EXIT_FAILURE);
    }
    return fp;
}

/**
 * @brief Legge i parametri fondamentali (numero abitanti, progetti, gruppi) dal file.
 * @param fp Il file stream.
 * @param params Puntatore alla struttura dove memorizzare i parametri letti.
 */
static void read_initial_params(FILE *fp, ParametriInput *params) {
    if (fscanf(fp, "%d %d %d", &params->num_abitanti, &params->num_progetti, &params->num_gruppi) != NUM_PARAM_INIZIALI) {
        fprintf(stderr, "ERRORE: Formato dati iniziale non valido. Attesi %d parametri.\n", NUM_PARAM_INIZIALI);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Alloca memoria per un array di puntatori a strutture 'cittadino'.
 * @param count Il numero di cittadini da allocare.
 * @return Array di puntatori a 'cittadino' allocato e inizializzato.
 */
static cittadino **allocate_citizen_array(int count) {
	int i;
    cittadino **citizens = (cittadino **)calloc(count, sizeof(cittadino *));
    if (!citizens) {
        fprintf(stderr, "ERRORE: Allocazione fallita per l'array principale dei cittadini.\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < count; ++i) {
        citizens[i] = (cittadino *)calloc(1, sizeof(cittadino));
        if (!citizens[i]) {
            fprintf(stderr, "ERRORE: Allocazione fallita per un singolo cittadino all'indice %d.\n", i);
            exit(EXIT_FAILURE);
        }
    }
    return citizens;
}

/**
 * @brief Alloca e inizializza un array di stringhe per i nomi dei progetti.
 * @param count Il numero di progetti.
 * @return Array di stringhe (char**) allocato.
 */
static char **allocate_project_names_array(int count) {
    char **project_names = (char **)calloc(count, sizeof(char *));
    int i;
    if (!project_names) {
        fprintf(stderr, "ERRORE: Allocazione fallita per l'array di puntatori ai nomi dei progetti.\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < count; ++i) {
        project_names[i] = (char *)calloc(PROG_LENGTH + 1, sizeof(char));
        if (!project_names[i]) {
            fprintf(stderr, "ERRORE: Allocazione fallita per la stringa del nome del progetto all'indice %d.\n", i);
            exit(EXIT_FAILURE);
        }
    }
    return project_names;
}

/**
 * @brief Carica i nomi dei progetti dal file e li inserisce nell'array.
 * @param fp Il file stream.
 * @param num_progetti Il numero totale di progetti da leggere.
 * @param project_names L'array di stringhe dove verranno memorizzati i nomi.
 */
static void load_project_names(FILE *fp, int num_progetti, char **project_names) {
    /* Salta la prima stringa e legge il primo nome del progetto */
    int i;
    if (fscanf(fp, "%*s %s", project_names[0]) != 1) {
        fprintf(stderr, "ERRORE: Impossibile leggere il primo nome del progetto.\n");
        exit(EXIT_FAILURE);
    }
    for (i = 1; i < num_progetti; ++i) {
        /* Salta l'etichetta del progetto e legge il nome */
        if (fscanf(fp, "%*s %s", project_names[i]) != 1) {
            fprintf(stderr, "ERRORE: Impossibile leggere il nome del progetto all'indice %d.\n", i);
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * @brief Esegue una ricerca binaria per trovare l'indice di un progetto nell'array ordinato.
 * @param project_list L'array ordinato dei nomi dei progetti.
 * @param low L'indice di partenza per la ricerca.
 * @param high L'indice finale per la ricerca.
 * @param target_name La stringa con il nome del progetto da cercare.
 * @return L'indice del progetto se trovato, altrimenti -1.
 */
static int find_project_index_binary(char **project_list, int low, int high, const char *target_name) {
    while (low <= high) {
        int mid = low + (high - low) / 2; /* Previene overflow rispetto a (low+high)/2 */
        int cmp_result = strcmp(target_name, project_list[mid]);

        if (cmp_result == 0) {
            return mid; /* Progetto trovato */
        } else if (cmp_result > 0) {
            low = mid + 1; 
        } else {
            high = mid - 1; 
        }
    }
    return -1; /* Progetto non trovato*/
}

/**
 * @brief Popola le strutture dei cittadini con i loro codici e le preferenze sui progetti.
 * @param fp Il file stream.
 * @param citizens L'array di strutture cittadino da popolare.
 * @param num_citizens Il numero totale di cittadini.
 * @param num_projects Il numero totale di progetti.
 * @param project_names L'array ordinato dei nomi dei progetti per la lookup.
 */
static void populate_citizen_data(FILE *fp, cittadino **citizens, int num_citizens, int num_projects, char **project_names) {
    char project_buffer[PROG_LENGTH + 1];
    int c, c_idx, p_idx;
    /* Move the file pointer to the beginning of the file, first line already read once to create projs array, this does not consume extra memory */
    fseek(fp, 0, SEEK_SET);
    fscanf(fp, "%*[^\n]");

    while ((c = fgetc(fp)) != '\n' && c != EOF);

    for (c_idx = 0; c_idx < num_citizens; ++c_idx) {
        /* Read citizen code and the first preferred project. */
        fscanf(fp, "%s %s", citizens[c_idx]->CF, project_buffer);

        /* Allocate memory for project preferences (integers). */
        citizens[c_idx]->progs = (int *)calloc(num_projects, sizeof(int));

        /* Allocate memory for preference operators (chars). */
        citizens[c_idx]->pref = (char *)calloc(num_projects - 1, sizeof(char));

        /* Find the index of the first preferred project. */
        citizens[c_idx]->progs[0] = find_project_index_binary(project_names, 0, num_projects - 1, project_buffer);

        /* Read the remaining project preferences and operators. */
        for (p_idx = 1; p_idx < num_projects; ++p_idx) {
            fscanf(fp, " %c %s", &citizens[c_idx]->pref[p_idx - 1], project_buffer);
            citizens[c_idx]->progs[p_idx] = find_project_index_binary(project_names, 0, num_projects - 1, project_buffer);
        }
    }
}
/**
 * @brief Chiude il file stream.
 * @param fp Il puntatore al FILE da chiudere.
 */
static void close_data_file(FILE *fp) {
    fclose(fp);
}

/**
 * @brief Funzione principale per l'acquisizione di tutti i dati necessari all'applicazione.
 * @param filename Il nome del file da cui leggere i dati.
 * @param params Puntatore alla struttura ParametriInput dove saranno memorizzati i parametri letti.
 * @param citizens Puntatore al puntatore dell'array di cittadini.
 * @param projects Puntatore al puntatore dell'array di nomi dei progetti.
 */
void fetch_all_data(const char *filename, ParametriInput *params, cittadino ***citizens, char ***projects) {
    FILE *data_source = attempt_file_open(filename);

    read_initial_params(data_source, params);

    *citizens = allocate_citizen_array(params->num_abitanti);
    *projects = allocate_project_names_array(params->num_progetti);

    load_project_names(data_source, params->num_progetti, *projects);
    qsort_alfabetico(*projects, 0, params->num_progetti - 1);

    populate_citizen_data(data_source, *citizens, params->num_abitanti, params->num_progetti, *projects);

    close_data_file(data_source);
}

/* === Sezione di Allocazione per Metodi di Votazione e Clustering === */

/**
 * @brief Inizializza e alloca un array di puntatori a strutture 'contatore'.
 * @param count La dimensione dell'array.
 * @return L'array allocato di puntatori a contatori.
 */
contatore **initialize_counter_array(int count) {
    contatore **counters = (contatore **)calloc(count, sizeof(contatore *));
    int i;
    if (!counters) {
        fprintf(stderr, "ERRORE: Allocazione fallita per l'array di puntatori a contatori.\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < count; ++i) {
        counters[i] = (contatore *)calloc(1, sizeof(contatore));
        if (!counters[i]) {
            fprintf(stderr, "ERRORE: Allocazione fallita per un contatore all'indice %d.\n", i);
            exit(EXIT_FAILURE);
        }
        counters[i]->indice = i;
        counters[i]->cont = 0;
    }
    return counters;
}

/**
 * @brief Alloca una matrice di interi e la inizializza a zero.
 * @param rows Il numero di righe.
 * @param cols Il numero di colonne.
 * @return La matrice allocata.
 */
vint *allocate_int_matrix(int rows, int cols) {
    vint *matrix = (int **)calloc(rows, sizeof(vint));
    int r;
    if (!matrix) {
        fprintf(stderr, "ERRORE: Allocazione fallita per la matrice principale.\n");
        exit(EXIT_FAILURE);
    }
    for (r = 0; r < rows; ++r) {
        matrix[r] = (int *)calloc(cols, sizeof(int));
        if (!matrix[r]) {
            fprintf(stderr, "ERRORE: Allocazione fallita per la riga %d della matrice.\n", r);
            exit(EXIT_FAILURE);
        }
    }
    return matrix;
}

/**
 * @brief Alloca e inizializza un array di puntatori a strutture 'componente'.
 * @param count La dimensione dell'array.
 * @return L'array allocato di puntatori a componenti.
 */
componente **initialize_component_array(int count) {
    componente **components = (componente **)calloc(count, sizeof(componente *));
    int i;
    if (!components) {
        fprintf(stderr, "ERRORE: Allocazione fallita per l'array di puntatori a componenti.\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < count; ++i) {
        components[i] = (componente *)calloc(1, sizeof(componente));
        if (!components[i]) {
            fprintf(stderr, "ERRORE: Allocazione fallita per una componente all'indice %d.\n", i);
            exit(EXIT_FAILURE);
        }
        components[i]->ind = i;
    }
    return components;
}

/* === Sezione di Deallocazione Completa === */

/**
 * @brief Libera tutta la memoria dinamica allocata durante l'esecuzione del programma.
 * @param num_citizens Numero di abitanti.
 * @param num_projects Numero di progetti.
 * @param projects_data Array di nomi di progetti.
 * @param citizens_data Array di strutture cittadino.
 * @param method_counters Array di contatori per i metodi di votazione.
 * @param borda_matrix Matrice di Borda.
 * @param condorcet_matrix Matrice di Condorcet.
 * @param distance_matrix Matrice delle distanze per il clustering.
 */
void free_all_resources(int num_citizens, int num_projects, char **projects_data, cittadino **citizens_data,
                        contatore **method_counters, vint *borda_matrix, vint *condorcet_matrix, vint *distance_matrix) {
	int i;
    /* Libera memoria per i nomi dei progetti */
    if (projects_data) {
        for (i = 0; i < num_projects; ++i) {
            free(projects_data[i]);
        }
        free(projects_data);
    }
    /* Libera memoria per le strutture cittadino */
    /* Deallocate citizens_data */
    if (citizens_data != NULL) {
        for (i = 0; i < num_citizens; ++i) {
            if (citizens_data[i] != NULL) {
                /* Free dynamically allocated members within each cittadino struct */
                if (citizens_data[i]->progs != NULL) {
                    free(citizens_data[i]->progs);
                    citizens_data[i]->progs = NULL;
                }
                if (citizens_data[i]->pref != NULL) {
                    free(citizens_data[i]->pref);
                    citizens_data[i]->pref = NULL;
                }
                /* Free the cittadino struct itself */
                free(citizens_data[i]);
                citizens_data[i] = NULL;
            }
        }
        /* Free the array of cittadino pointers */
        free(citizens_data);
        citizens_data = NULL;
    }

    /* Libera memoria per i contatori dei metodi di votazione */
    if (method_counters) {
        for (i = 0; i < num_projects; ++i) {
            free(method_counters[i]);
        }
        free(method_counters);
    }
    /* Libera memoria per la matrice di Borda */
    if (borda_matrix) {
        for (i = 0; i < num_citizens; ++i) { /* Matrice [num_citizens][num_projects] */
            free(borda_matrix[i]);
        }
        free(borda_matrix);
    }

    /* Libera memoria per la matrice di Condorcet */
    if (condorcet_matrix) {
        for (i = 0; i < num_projects; ++i) {
            free(condorcet_matrix[i]);
        }
        free(condorcet_matrix);
    }

    /* Libera memoria per la matrice delle distanze (clustering) */
    if (distance_matrix) {
        for (i = 0; i < num_citizens; ++i) {
            free(distance_matrix[i]);
        }
        free(distance_matrix);
    }
}
