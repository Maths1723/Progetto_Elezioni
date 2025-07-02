# README: Analisi di Preferenze e Clustering - Progetto Elezioni, Corso Algoritmi e Strutture Dati, Giugno 2025

Questo repository contiene il **progetto finale** del corso universitario di **Algoritmi e Strutture Dati** per il Corso di Laurea in Matematica. Realizzato in **C89**, il software si propone di esplorare e implementare diverse metodologie per l'aggregazione di preferenze individuali e il clustering di dati, fornendo uno strumento completo per l'analisi di insiemi di preferenze complesse.

---

### Note Importanti - Taglio del Progetto

Il progetto ha un **taglio puramente accademico**, con scelte mirate a dimostrare l'acquisizione dei contenuti del corso. Talvolta, alla pura efficienza spaziale e temporale sono state preferite leggibilità, strutturazione, modularità o l'integrazione di strutture viste a lezione. Le sezioni 2 e 5 invece, come scritto in relazione, contengono per lo più dettagli tecnici, che possono fornire nota di constestualizzazione del problema ma soprattutto completare il progetto in caso di inserzione in un portfolio puramente lavorativo.

* **Leggibilità**: È stato scelto uno stile relativamente verboso per i nomi delle variabili. La commentazione è prevalentemente in stile Doxygen e varia in base alla complessità percepita delle sezioni o all'implementazione di concetti meno approfonditi a lezione.
* **Strutturazione e Modularità**: Durante lo sviluppo, è stata data enfasi alla programmazione modulare, inserendo file come `listaarchi.h/.c` visti a lezione. Ogni metodo implementato ha una propria funzione di preparazione della struttura dati e una per la sua implementazione specifica.
* **Efficienza**: Sono state effettuate scelte ragionate e motivate per ottimizzare le performance.
* **Gestione degli Errori**: Il codice include meccanismi basilari di gestione degli errori (ad esempio, controlli di `NULL` dopo le `calloc`, verifica dell'apertura dei file) per garantire una certa robustezza.
* **Ottimizzazioni**: Sono state implementate diverse micro-ottimizzazioni, come l'uso efficiente dei vettori di puntatori per evitare riallocazioni costose e la pre-ordinazione dei nomi dei progetti per velocizzare le ricerche.
* **Come fosse un problema reale**: Supponendo che una parte di cui venga richiesta padronanza sia il saper risolvere situazioni come in ambito lavorativo, ho scritto le sezioni 2 e 5 calandole in una applicazione pratica tipo, sebbene di interesse accademico, contengono consigli pratici per gestione, manutenzione e upgrade del software. Sono perciò saltabili.

---

### Panoramica del Progetto

Il cuore di questo progetto risiede nell'implementazione di algoritmi classici ed euristici per affrontare problemi di scelta sociale e raggruppamento. L'enfasi è stata posta sull'**efficienza computazionale** e sulla **chiarezza del codice**, aspetti fondamentali in un contesto accademico di strutture dati.

Le funzionalità principali includono:

* **Acquisizione e Gestione Dati**: Un robusto sistema per leggere e strutturare i dati di preferenza da file esterni.
* **Metodi di Aggregazione delle Preferenze**: Implementazione dei sistemi di voto di **Pluralità**, **Borda** e **Condorcet** per la derivazione di un ordinamento collettivo da preferenze individuali.
* **Clustering K-Medians**: Un'euristica greedy per il raggruppamento di individui in $k$ cluster, basata sulla minimizzazione delle distanze tra le preferenze.
* **Gestione della Memoria**: Funzioni dedicate per l'allocazione e deallocazione dinamica della memoria, cruciale per la stabilità e l'efficienza.

---

### Requisiti

Dato che il progetto è stato sviluppato in **C89**, è garantita la compatibilità con la maggior parte dei compilatori C standard.

* **Compilatore C**: È necessario un compilatore conforme allo standard C89.
* **Sistema Operativo**: Qualsiasi sistema operativo che supporti la compilazione C.

---

### Struttura del Progetto

Il progetto è organizzato in diversi file sorgente C e header, ciascuno con una specifica responsabilità:

* **`prog_elezioni.c`**: Contiene la funzione principale che orchestra l'esecuzione del programma, chiamando le varie fasi (caricamento, algoritmi, deallocazione).
* **`data_management.h` / `data_management.c`**: Gestisce le strutture dati principali (individui, progetti, preferenze) e le funzioni per il loro caricamento e la successiva deallocazione. Qui si trovano anche le funzioni per la costruzione della matrice di Borda.
* **`metodi.h` / `metodi.c`**: Contengono l'implementazione dei metodi di clustering, la preparazione delle strutture di calcolo e il loro display a terminale.
* **`listaarchi.h/.c grafo.h/.c`**: Contengono l'implementazione delle strutture e dei metodi che utilizzano le strutture astratte degli archi necessarie per il progetto
* **`sort.h/.c`**: Contiene funzioni di utilità generale, come ordinamenti (**Quicksort**) e altre operazioni comuni a più moduli.

---
