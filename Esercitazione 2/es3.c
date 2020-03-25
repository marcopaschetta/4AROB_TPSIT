#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t m1, m2; // Variabili pthread_mutex_t 

void *ping(void *arg){ // funzione eseguita da thread t1 
    while(1){ // ciclo infinito
        // SEZIONE CRITICA
        pthread_mutex_lock(&m1); // blocco mutex m1 -> 0 -> Rosso
        sleep(1); 
        printf("ping\n");
	// FINE SEZIONE CRITICA
        pthread_mutex_unlock(&m2); // sblocco mutex m2 -> 1 -> Verde
    }
}

void *pong(void *arg){ // funzione eseguita da thread t2
    while(1){ // ciclo infinito
        // SEZIONE CRITICA
        pthread_mutex_lock(&m2); // blocco mutex m2 -> Rosso
        sleep(1);
        printf("pong\n");
	// FINE SEZIONE CRITICA
        pthread_mutex_unlock(&m1); // sblocco mutex m1 -> Verde
    }
}

int main(int argc, char **argv){
    pthread_t t1, t2;

    pthread_mutex_unlock(&m1); // sblocchiamo mutex m1 -> Verde
    pthread_mutex_lock(&m2); // mutex m2 bloccato -> Rosso

    pthread_create(&t1, NULL, (void*)ping, NULL);
    pthread_create(&t2, NULL, (void*)pong, NULL);

 // istruzioni superflue, i thread t1, t2 non termineranno mai [while(1)]
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
