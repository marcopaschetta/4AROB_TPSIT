#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define MAXITER 100

sem_t mutex, pieno, vuoto;
char arr[10];
int in, out;

void *produci(void *arg);
void *consuma(void *arg);

int main(int argc, char **argv){
    pthread_t prod, cons;
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    sem_init(&mutex, 0, 1); // sbloccato
    sem_init(&pieno, 0, 0); // bloccato
    sem_init(&vuoto, 0, 10); // sbloccato x 10 iterazioni

    /* condizione iniziale */
    in = 0;
    out = 0;

    pthread_create(&prod, NULL, (void*)produci, NULL);
    pthread_create(&cons, NULL, (void*)consuma, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}

void *produci(void *arg){
    int i = 0;
    char c;

    while(i<MAXITER){
        i++;
        c = (char)(rand()%26)+97;

        sem_wait(&vuoto); // per 10 iter lascio passare
        sem_wait(&mutex); /* RC */
        arr[in] = c;
        printf("Inserito %c\n", c);
        in = (in+1) %10;
        sem_post(&mutex); /* FINE RC */
        sem_post(&pieno);
    }
}

void *consuma(void *arg){
    int i = 0;
    while(i<MAXITER){
        i++;
        sem_wait(&pieno);
        sem_wait(&mutex);
        /* SC */
        char c = arr[out];
        printf("Prelevo %c\n", c);
        out = (out+1) %10;
        /* FINE SC */
        sem_post(&mutex);
        sem_post(&vuoto);
    }
}
