#ifndef __grafo_h
#define __grafo_h

#include <stdio.h>
#include <stdlib.h>

#include "data_manager.h"
#include "listaarchi.h"
/* Doxygen++; :) */

/**
 * @brief Definizione della costante TRUE.
 */
#define TRUE  1
/**
 * @brief Definizione della costante FALSE.
 */
#define FALSE 0
/**
 * @brief Alias per il tipo intero per rappresentare valori booleani.
 */
#define boolean int


/**
 * @brief Struttura che rappresenta un grafo.
 *
 * @var grafo::n
 * Numero di nodi nel grafo.
 * @var grafo::m
 * Numero di archi nel grafo.
 * @var grafo::FS
 * Array di liste di adiacenza (forward stars) per ciascun nodo.
 */
typedef struct _grafo grafo;

struct _grafo
{
  int n;
  int m;
  listaarchi *FS;
};


/* ----------------------- */
/* Prototipi delle funzioni */
/* ----------------------- */

/**
 * @brief Crea un grafo con 'n' nodi e senza archi.
 *
 * @param n Il numero di nodi da creare nel grafo.
 * @param pG Puntatore alla struttura grafo da inizializzare.
 */
void creagrafo (int n, grafo *pG);

/**
 * @brief Dealloca tutta la memoria utilizzata dal grafo specificato.
 *
 * @param pG Puntatore alla struttura grafo da distruggere.
 */
void distruggegrafo (grafo *pG);

/**
 * @brief Aggiunge un arco diretto dal nodo 'o' al nodo 'd' nel grafo.
 * Non esegue alcuna operazione se l'arco esiste già.
 *
 * @param o Il nodo di origine dell'arco.
 * @param d Il nodo di destinazione dell'arco.
 * @param pG Puntatore alla struttura grafo in cui inserire l'arco.
 */
void insarco (int o, int d, grafo *pG);



/* Le seguenti funzioni sono commentate nel codice originale,
   ma ho incluso i loro prototipi con commenti Doxygen
   nel caso volessi riattivarle in futuro. */

/**
 * @brief Cancella l'arco (o,d) dal grafo *pG.
 * Non fa nulla se l'arco non esiste.
 * @param o Il nodo di origine dell'arco da cancellare.
 * @param d Il nodo di destinazione dell'arco da cancellare.
 * @param pG Puntatore alla struttura grafo da cui cancellare l'arco.
 */
/*void cancarco (int o, int d, grafo *pG);*/

/**
 * @brief Determina se l'arco (o,d) appartiene al grafo *pG.
 *
 * @param o Il nodo di origine dell'arco da controllare.
 * @param d Il nodo di destinazione dell'arco da controllare.
 * @param pG Puntatore alla struttura grafo.
 * @return TRUE se l'arco esiste, FALSE altrimenti.
 */
/*boolean esistearco (int o, int d, grafo *pG);*/


/**
 * @brief Restituisce la posizione del primo arco nella forward star (lista di adiacenza) del nodo 'o'.
 *
 * @param pG Puntatore alla struttura grafo.
 * @param o Il nodo di cui si vuole ottenere la forward star.
 * @return La posizione del primo arco nella forward star.
 */
posarco primoarcoFS (grafo *pG, nodo o);

/**
 * @brief Restituisce la posizione dell'arco successivo a 'pa' nella forward star del nodo 'o'.
 *
 * @param pG Puntatore alla struttura grafo.
 * @param o Il nodo di cui si sta esplorando la forward star.
 * @param pa La posizione corrente dell'arco.
 * @return La posizione dell'arco successivo.
 */
posarco succarcoFS (grafo *pG, nodo o, posarco pa);

/**
 * @brief Determina se la posizione 'pa' è al di fuori della forward star del nodo 'o'.
 *
 * @param pG Puntatore alla struttura grafo.
 * @param o Il nodo di cui si sta esplorando la forward star.
 * @param pa La posizione corrente dell'arco.
 * @return TRUE se la posizione è la fine della lista, FALSE altrimenti.
 */
boolean finearchiFS (grafo *pG, nodo o, posarco pa);

/**
 * @brief Restituisce il nodo di destinazione dell'arco in posizione 'pa' nella forward star del nodo 'o'.
 *
 * @param pG Puntatore alla struttura grafo.
 * @param o Il nodo di origine dell'arco.
 * @param pa La posizione dell'arco.
 * @return Il nodo di destinazione dell'arco.
 */
nodo leggedestarco (grafo *pG, nodo o, posarco pa);


/**
 * @brief Esegue una visita ricorsiva in profondità (DFS) a partire dal nodo sorgente 's'.
 * I vertici visitati vengono marcati nel vettore 'C' con l'indice 'c'.
 *
 * @param pG Puntatore al grafo da visitare.
 * @param s Il nodo sorgente da cui iniziare la visita.
 * @param C Vettore dei componenti, dove verranno memorizzati gli indici dei componenti visitati.
 * @param c L'indice del componente da assegnare ai nodi visitati in questa DFS.
 */
void DFSricorsiva (grafo *pG, nodo s, vint C, int c);

/**
 * @brief Costruisce il grafo trasposto 'pGT' a partire dal grafo originale 'pG'.
 * Il grafo trasposto ha gli stessi nodi ma tutti gli archi sono invertiti.
 *
 * @param pG Puntatore al grafo originale.
 * @param pGT Puntatore alla struttura grafo che verrà inizializzata come trasposto.
 */
void CostruisceGrafoTrasposto (grafo *pG, grafo *pGT);

/**
 * @brief Calcola l'intersezione tra due vettori di incidenza 'U1' e 'U2' di lunghezza 'n'.
 * I valori comuni (dove entrambi sono > 0) vengono copiati nel vettore 'C' (contatore **C).
 *
 * @param U1 Il primo vettore di incidenza.
 * @param U2 Il secondo vettore di incidenza.
 * @param C Vettore di puntatori a strutture contatore dove verranno memorizzati i risultati.
 * @param n La lunghezza dei vettori.
 */
void Intersezione (vint U1, vint U2, contatore **C, int n);

/**
 * @brief Determina le componenti fortemente connesse del grafo 'pG'.
 * Il risultato viene memorizzato nel vettore 'C', e il numero totale di componenti
 * connesse viene memorizzato in 'pnc'.
 *
 * @param pG Puntatore al grafo su cui calcolare le componenti.
 * @param C Vettore di puntatori a strutture contatore dove verranno memorizzati i risultati delle componenti.
 * @param pnc Puntatore a un intero che conterrà il numero di componenti fortemente connesse trovate.
 */
void ComponentiFortementeConnesse (grafo *pG, contatore **C, int *pnc);


#endif
