#ifndef __metodi_h
#define __metodi_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_manager.h"
#include "sort.h" 
#include "grafo.h"   /* Doxygen++; :) */

/* Metodi di votazione e procedure di preparazione dati */

/**
 * @brief Implements the plurality system and displays the results.
 * @param counters Array of counters for projects.
 * @param project_names Array of project names.
 * @param num_projects Total number of projects.
 */
void calculate_and_display_plurality(contatore **counters, vchar *project_names, int num_projects);

/**
 * @brief Calculates Borda method scores and displays the results.
 * @param borda_matrix Borda score matrix per citizen.
 * @param counters Array of counters for projects.
 * @param project_names Array of project names.
 * @param num_citizens Number of citizens.
 * @param num_projects Number of projects.
 */
void calculate_and_display_borda(vint *borda_matrix, contatore **counters, vchar *project_names, int num_citizens, int num_projects);

/**
 * @brief Calculates Condorcet method results and displays the winner/order.
 * @param condorcet_matrix Condorcet comparison matrix.
 * @param counters Array of counters for projects.
 * @param project_names Array of project names.
 * @param num_projects Number of projects.
 */
void calculate_and_display_condorcet(vint *condorcet_matrix, contatore **counters, vchar *project_names, int num_projects);

/**
 * @brief Executes the greedy clustering algorithm and prints the resulting groups.
 * @param borda_scores_matrix Borda scores matrix (used to calculate distances).
 * @param distance_matrix Distance matrix between citizens.
 * @param num_citizens Number of citizens.
 * @param num_projects Number of projects.
 * @param desired_groups Number of desired groups.
 * @param citizens_data Array of citizen structures.
 */
void perform_greedy_clustering(vint *borda_scores_matrix, vint *distance_matrix, int num_citizens, int num_projects, int desired_groups, cittadino **citizens_data);

/* Functions for data collection by methods */

/**
 * @brief Aggregates a citizen's preference data for the plurality method.
 * @param citizen The citizen's structure whose preferences are to be added.
 * @param counters Array of counters to update.
 */
void aggregate_plurality_data(cittadino citizen, contatore **counters);

/**
 * @brief Inserts a citizen's Borda scores into the Borda matrix.
 * @param citizen The citizen's structure.
 * @param citizen_idx The citizen's index in the matrix.
 * @param borda_matrix The Borda matrix to populate.
 * @param num_projects Total number of projects.
 */
void insert_borda_scores(cittadino citizen, int citizen_idx, vint *borda_matrix, int num_projects);

/**
 * @brief Aggregates a citizen's preference data for the Condorcet method.
 * @param citizen The citizen's structure.
 * @param condorcet_matrix The Condorcet matrix to update.
 * @param num_projects Total number of projects.
 */
void aggregate_condorcet_data(cittadino citizen, vint *condorcet_matrix, int num_projects);

/* Auxiliary and printing functions */

/**
 * @brief Prints the results for voting methods (Plurality, Borda, Condorcet).
 * @param counters Array of counters with results.
 * @param project_names Array of project names.
 * @param num_elements Number of elements to print (projects).
 * @param method_type Type of method (0=Plurality, 1=Borda, 2=Condorcet).
 */
void display_method_results(contatore **counters, vchar *project_names, int num_elements, int method_type);

/**
 * @brief Prints the clustering results.
 * @param min_components Array of clustered components.
 * @param citizens_data Array of citizen structures.
 * @param num_citizens Number of citizens.
 * @param num_actual_groups The actual number of groups found.
 */
void print_cluster_output(componente **min_components, cittadino **citizens_data, int num_citizens, int num_actual_groups);

/* Auxiliary */

/**
 * @brief Resets project counters to zero and their indices.
 * @param counters Array of counters to reset.
 * @param num_projects Number of projects.
 */
void reset_project_counters(contatore **counters, int num_projects);

#endif /* __metodi_h */
