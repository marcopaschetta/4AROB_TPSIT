#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 5
#define PENSA 0
#define HAFAME 1
#define MANGIA 2
#define DESTRA (fil_num+1)%N
#define SINISTRA (fil_num-1+N)%N

pthread_mutex_t mutex, mutex_f[N];  /* semafori per RC e forchette */
int stato[N]; /* variabile condivisa con lo stato dei filosofi */
int fil[N] = {0, 1, 2, 3, 4};

void controllaPosate(int fil_num);

void prendi(int fil_num){
    pthread_mutex_lock(&mutex);
    printf("Filosofo %d: ho fame...\n", fil_num+1);
    stato[fil_num] = HAFAME;

    controllaPosate(fil_num);

    pthread_mutex_unlock(&mutex);
    // se non può mangiare, si mette in attesa di essere abilitato
    /**/
    pthread_mutex_lock(&mutex_f[fil_num]);
}
void controllaPosate(int fil_num){
    if(stato[fil_num]==HAFAME && stato[DESTRA]!=MANGIA && stato[SINISTRA]!=MANGIA){
        stato[fil_num] = MANGIA;
        printf("FILOSOFO %d: prendo forchetta %d e %d.. sto mangiando...\n", fil_num+1, SINISTRA+1, fil_num+1);
        sleep(rand()%N);
        /**/
        pthread_mutex_unlock(&mutex_f[fil_num]);
    }
}
void posa(int fil_num){
    pthread_mutex_lock(&mutex);
    stato[fil_num] = PENSA;
    printf("FILOSOFO %d: poso forchetta %d e %d.. sto pensando...\n", fil_num+1, SINISTRA+1, fil_num+1);
    /**/
    controllaPosate(SINISTRA);
    controllaPosate(DESTRA);

    pthread_mutex_unlock(&mutex);
}
void *filosofo(void *arg){
    while(1){
        int *i = arg;
        sleep(rand()%N);
        prendi(*i);
        posa(*i);
    }
    pthread_exit(0);
}

int main(int argc, char **argv){
    int i;
    pthread_t filo[N];

    pthread_mutex_init(&mutex, NULL);
    for (i=0; i<N; i++){
        pthread_mutex_init(&mutex_f[i], NULL);
        pthread_mutex_lock(&mutex_f[i]);
    }

    for (i=0; i<N; i++){
        pthread_create(&filo[i], NULL, (void*) filosofo, (void*)&fil[i]); /* creazione thread */
        printf("FILOSOFO %d: sto pensando...\n", i+1);
    }
    for (i=0; i<N; i++){
        pthread_join(filo[i], NULL);
    }

    return 0;
}
