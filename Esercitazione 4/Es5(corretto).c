 /*
        Creare un programma che simuli l'acquisto dei biglietti di un cinema avente due casse
        I biglietti disponibili sono prefissati (risorsa condivisa)
        All'avvio vengono generati n clienti/threads "per la cassa1 e per la cassa2" per procedere all'acquisto.
            - Gestire la situazione in cui vengono generati x clienti per cassa1 e y clienti per cassa2 
        Ogni cliente:
            - accede alla risorsa "biglietti disponibili" in modalità ESCLUSIVA
            - random, acquista un numero di biglietti compreso tra 1 e 5 
            - infine, stampa un messaggio a video indicando 
                1. Quanti biglietti ha tentato di acquistare
                2. Quanti biglietti ha di fatto acquistato
                3. Quanti biglietti sono ancora disponibili
            - prevedere la possibilità in cui:
                1. non tutti i clienti riescano ad acquistare x biglietti (in questo caso acquistare i biglietti residui)
                2. i biglietti siano completamente esauriti
        La rispettiva cassa, al termine di ogni acquisto (o tentativo di acquisto), valuta, con probabilità 1/2 (50% o random val 0-1), 
        se far 'passare' il cliente subito successivo alla medesima fila oppure dare la precedenza al cliente in fila sull'altra cassa 

        Esempio esecuzione:
        > Sono il cliente 489737546 in fila alla cassa1 e voglio acquistare 4 biglietti:
        > biglietti acquistati: 4
        > biglietti disponibili: 96
        > Sono il cliente 794632639 in fila alla cassa2 e voglio acquistare 3 biglietti:
        > biglietti acquistati: 3
        > biglietti disponibili: 93
        > Sono il cliente 876432834 in fila alla cassa2 e voglio acquistare 10 biglietti:
        > biglietti acquistati: 10
        > biglietti disponibili: 83
        ....
        > Sono il cliente 982374984 in fila alla cassa1 e voglio acquistare 8 biglietti:
        > biglietti acquistati: 2
        > biglietti disponibili: 0
        > Sono il cliente 876865756 in fila alla cassa2 e voglio acquistare 10 biglietti:
        > biglietti acquistati: 0
        > biglietti disponibili: 0
*/   
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int n_biglietti = 100;

int filaA = 10;
int filaB = 6;

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

void *cassa1(void *argv){
    int rnd, rndM;

    /* SEZIONE CRITICA */
    pthread_mutex_lock(&m1);
    filaA--;
    rnd = 1 + rand()%10;
    printf("Sono il cliente %u alla cassa 1 e voglio acquistare %d biglietti\n", pthread_self(), rnd);
    if(n_biglietti == 0){
        printf("Biglietti acquistati 0\n");
    }else if(n_biglietti>0 && rnd<=n_biglietti){
        printf("Biglietti acquistati %d\n", rnd);
        n_biglietti = n_biglietti - rnd;
    }else if(n_biglietti>0 && rnd>n_biglietti){ // maggiore dei biglietti disponibili
        printf("Biglietti acquistati %d\n", n_biglietti);
        n_biglietti = 0;
    }

    rndM = rand()%2;
    printf("Biglietti disponibili %d\nRAND %d\n", n_biglietti, rndM);
    if(rndM == 0){ // Prosegue cassa A
        if(filaA>0){ // se ancora clienti in coda
            pthread_mutex_unlock(&m1);
        }else{ // proseguo con fila B
            pthread_mutex_unlock(&m2);
        }
    }else{ // Prosegue cassa A
        if(filaB>0){ // se ancora clienti in coda
            pthread_mutex_unlock(&m2);
        }else{ // proseguo con fila A
            pthread_mutex_unlock(&m1);
        }
    }
    /* FINE SEZIONE CRITICA */
	pthread_exit(NULL);
}
void *cassa2(void *argv){
      int rnd, rndM;

    /* SEZIONE CRITICA */
    pthread_mutex_lock(&m2);
    filaB--;
    rnd = 1 + rand()%10;
    printf("Sono il cliente %u alla cassa 2 e voglio acquistare %d biglietti\n", pthread_self(), rnd);
    if(n_biglietti == 0){
        printf("Biglietti acquistati 0\n");
    }else if(n_biglietti>0 && rnd<=n_biglietti){
        printf("Biglietti acquistati %d\n", rnd);
        n_biglietti = n_biglietti - rnd;
    }else if(n_biglietti>0 && rnd>n_biglietti){ // maggiore dei biglietti disponibili
        printf("Biglietti acquistati %d\n", n_biglietti);
        n_biglietti = 0;
    }
    rndM = rand()%2;
    printf("Biglietti disponibili %d\nRAND %d\n", n_biglietti, rndM);

    if(rndM == 0){ // Prosegue cassa B
        if(filaB>0){ // se ancora clienti in coda
            pthread_mutex_unlock(&m2);
        }else{ // proseguo con fila A
            pthread_mutex_unlock(&m1);
        }
    }else{ // Prosegue cassa A
        if(filaA>0){ // se ancora clienti in coda
            pthread_mutex_unlock(&m1);
        }else{ // proseguo con fila B
            pthread_mutex_unlock(&m2);
        }
    }
    /* FINE SEZIONE CRITICA */

	pthread_exit(NULL);
}

int main(int argc, char **argv){
	pthread_t t1[10];
	pthread_t t2[10];
	int i;

	pthread_mutex_unlock(&m1);
	pthread_mutex_lock(&m2);
	for(i=3; i>0; i--){
        printf("Apertura casse tra %d...\n", i);
        sleep(1);
	}
    srand(time(NULL));
	for(i=0; i<10; i++){
        pthread_create(&t1[i], NULL, (void*)cassa1, NULL);
        pthread_create(&t2[i], NULL, (void*)cassa2, NULL);
	}
	for(i=0; i<10; i++){
        pthread_join(t1[i], NULL);
        pthread_join(t2[i], NULL);
	}
    printf("PADRE: Acquisto terminato\n");
	return 0;
}
