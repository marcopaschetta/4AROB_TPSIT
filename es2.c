#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> // per malloc
#include <unistd.h> // per sleep

int cnt = 0;

void stampaMsg(void *arg){
    int tID = pthread_self();
    // LETTURA ARGOMENTO PASSATO (A)
    int dato = *((int*)arg);
    // LETTURA ARGOMENTO PASSATO (B)
    int *dato_din = (int*)malloc(sizeof(int));
    dato_din = arg;
    // STAMPA MESSAGGIO IDENTIFICATIVO THREAD E DATO PASSATO
    printf("Sono il thread %u. Parametro passato: %d\n", tID, dato);
    // INCREMENTO CONTATORE "CONDIVISO" TRA THREAD E PROCESSO PADRE
    cnt++;
    // TERMINO IL THREAD E RESTITUISCO INDIRIZZO CELLA PUNTATA DA dato_din
    pthread_exit(dato_din);
}

int main(int argc, char **argv){
    pthread_t t[10];
    int i;
    int *ret; // PER LETTURA

    printf("Padre: creo 10 thread\n");
    for(i=0; i<10; i++){
        pthread_create(&t[i], NULL, (void*) &stampaMsg, (void *) &i);
        //sleep(1);
    }
    printf("Padre: attendo i 10 thread creati\n");
    for(i=0; i<10; i++){
        pthread_join(t[i], (void**)&ret); // ATTENDO TERMINAZIONE THREAD E LEGGO VALORE RESTITUITO (tipo void)
        printf("Restituzione pthread_exit: %d\n", *ret); // STAMPO CONTENUTO CELLA PUNTATA DA ret (identico a cella puntata da dato_din)
    }
    printf("Tutti i thread sono terminati e cnt vale %d, termino\n", cnt);

}
