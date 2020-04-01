 /*
        Creare un programma che simuli l'acquisto dei biglietti di un concerto
        I biglietti disponibili sono prefissati (risorsa condivisa)
        All'avvio vengono generati n clienti/threads che "vanno alla cassa" per procedere all'acquisto.
        Ogni cliente:
            - accede alla risorsa "biglietti disponibili" in modalità ESCLUSIVA
            - random, acquista un numero di biglietti compreso tra 1 e 10 
            - infine, stampa un messaggio a video indicando 
                1. Quanti biglietti ha tentato di acquistare
                2. Quanti biglietti ha di fatto acquistato
                3. Quanti biglietti sono ancora disponibili
            - prevedere la possibilità in cui:
                1. non tutti i clienti riescano ad acquistare x biglietti (in questo caso acquistare i biglietti residui)
                2. i biglietti siano completamente esauriti
        Esempio esecuzione:
        > Sono il cliente 489737546 e voglio acquistare 4 biglietti:
        > biglietti acquistati: 4
        > biglietti disponibili: 96
        ....
        > Sono il cliente 792837528 e voglio acquistare 8 biglietti:
        > biglietti acquistati: 2
        > biglietti disponibili: 0
        > Sono il cliente 876865756 e voglio acquistare 10 biglietti:
        > biglietti acquistati: 0
        > biglietti disponibili: 0
    */    

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int n_biglietti = 100; // Risorsa condivisa: biglietti decrementati ogni volta che vengono acquistati con successo
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; // init della mutex

void *cassa(void *argv){
    int rnd;

    /* SEZIONE CRITICA */
    pthread_mutex_lock(&m); // accesso in mutua esclusione 

    rnd = 1 + rand()%10; // valore random (1<= rnd <=10) 
    printf("Sono il cliente %u e voglio acquistare %d biglietti\n", pthread_self(), rnd);
    if(n_biglietti == 0){ // se biglietti esauriti 
        printf("Biglietti acquistati 0\n");
    }else if(n_biglietti>0 && rnd<=n_biglietti){ // se biglietti non esauriti e biglietti da acquistare inferiori a quelli disponibili
        printf("Biglietti acquistati %d\n", rnd);
        n_biglietti = n_biglietti - rnd;
    }else if(n_biglietti>0 && rnd>n_biglietti){ // (1° condizione superflua) se biglietti non esauriti e biglietti da acquistare superiori a quelli disponibili 
        printf("Biglietti acquistati %d\n", n_biglietti);
        n_biglietti = 0;
    }

    printf("Biglietti disponibili %d\n", n_biglietti);

    /* FINE SEZIONE CRITICA */
    pthread_mutex_unlock(&m); // libero accesso in mutua eslusione a sezione critica
	pthread_exit(NULL); // terminazione thread 
}

int main(int argc, char **argv){
	pthread_t t[10]; // vettore di tipo pthread_t per 'clienti' che accedono alla 'cassa'
	int i;

	for(i=3; i>0; i--){
        printf("Apertura cassa tra %d...\n", i);
        sleep(1);
	}
    srand(time(NULL)); // inizializzazione random => affinchè, ad ogni nuova esecuzione del programma, vengano genereati valori random diversi
	for(i=0; i<10; i++){
        pthread_create(&t[i], NULL, (void*)cassa, NULL); // creazione 'cliente' e posizionamento in 'fila alla cassa' 
	}

	for(i=0; i<10; i++){
        pthread_join(t[i], NULL); // attendo terminazione di ogni 'cliente'
	}
    printf("PADRE: Acquisto terminato\n");
	return 0;
}
