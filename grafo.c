#include "data_manager.h"
#include "grafo.h"


void creagrafo (int n, grafo *pG)
{
  nodo o;

  pG->n = n;
  pG->m = 0;
  pG->FS = (listaarchi *) calloc(pG->n,sizeof(listaarchi));
  if (pG->FS == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione del vettore delle forward star!\n");
    exit(EXIT_FAILURE);
  }

  for (o = 0; o < pG->n; o++)
    pG->FS[o] = crealistaarchi();
}

void distruggegrafo (grafo *pG)
{
  nodo o;

  for (o = 0; o < pG->n; o++)
    distruggelistaarchi(&pG->FS[o]);
  free(pG->FS);

  pG->n = 0;
  pG->m = 0;
}
void insarco (nodo o, nodo d, grafo *pG)
{
  posarco pa, q;
  nodo orig, dest;

  for (pa = primolistaarchi(pG->FS[o]); !finelistaarchi(pG->FS[o],pa); pa = succlistaarchi(pG->FS[o],pa))
  {
    leggearco(pG->FS[o],pa,&orig,&dest);
    if ( (o == orig) && (d == dest) )
        break;
  }

  if (finelistaarchi(pG->FS[o],pa))
  {
    q = succlistaarchi(pG->FS[o],ultimolistaarchi(pG->FS[o]));
    inslistaarchi(pG->FS[o],q,o,d);
    pG->m++;
  }
}

posarco primoarcoFS (grafo *pG, nodo o)
{
  return primolistaarchi(pG->FS[o]);
}

posarco succarcoFS (grafo *pG, nodo o, posarco pa)
{
  return succlistaarchi(pG->FS[o],pa);
}

boolean finearchiFS (grafo *pG, nodo o, posarco pa)
{
  return finelistaarchi(pG->FS[o],pa);
}

nodo leggedestarco (grafo *pG, nodo o, posarco pa)
{
  int orig, d;

  leggearco(pG->FS[o],pa,&orig,&d);
  
  return d;
}

void DFSricorsiva (grafo *pG, nodo s, vint C, int c)
{
  nodo w;
  posarco pa;
  C[s] = c;

  for (pa = primoarcoFS(pG,s); !finearchiFS(pG,s,pa); pa = succarcoFS(pG,s,pa))
  {
    w = leggedestarco(pG,s,pa);
    if (C[w] == 0)
        DFSricorsiva(pG,w,C,c);
  }
}

void CostruisceGrafoTrasposto (grafo *pG, grafo *pGT)
{
  nodo o, d;
  posarco pa;

  creagrafo(pG->n,pGT);

  for (o = 0; o < pG->n; o++)
  {
    for (pa = primoarcoFS(pG,o); !finearchiFS(pG,o,pa); pa = succarcoFS(pG,o,pa))
    {
      d = leggedestarco(pG,o,pa);
      insarco(d,o,pGT);
    }
  }
}

void Intersezione (vint U1, vint U2, contatore **C, int n)
{
  int i;

  for (i = 0; i < n; i++)
    if ( (U1[i] > 0) && (U2[i] > 0) )
        (*C[i]).cont = U1[i];
}

void ComponentiFortementeConnesse (grafo *pG, contatore **C, int *pnc)
{
  nodo s, v;
  vint U1, U2;
  grafo GT;

  U1 = (vint) calloc(pG->n+1,sizeof(int));
  if (U1 == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione del vettore U1!\n");
    exit(EXIT_FAILURE);
  }

  U2 = (vint) calloc(pG->n+1,sizeof(int));
  if (U2 == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione del vettore U2!\n");
    exit(EXIT_FAILURE);
  }

  CostruisceGrafoTrasposto(pG,&GT);

  *pnc = 0;
  for (s = 0; s < pG->n; s++)
    if ((*C[s]).cont == 0)
    {
      (*pnc)++;

      for (v = 0; v < pG->n; v++)
        U1[v] = 0;
      DFSricorsiva(pG,s,U1,*pnc);

      for (v = 0; v < pG->n; v++)
        U2[v] = 0;
      DFSricorsiva(&GT,s,U2,*pnc);

      Intersezione(U1,U2,C,pG->n);
    }
  free(U1);
  free(U2);
  distruggegrafo(&GT);
}

