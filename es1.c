#include <stdio.h>
#include <pthread.h>

void *uno(void *arg){
    printf("Sono il thread che esegue la funzione 1\n");

    pthread_exit(0);
}
void due(void *arg){
    printf("Sono il thread che esegue la funzione 2\n");

    pthread_exit(0);
}
int main(int argc, char **argv){
    pthread_t t1, t2;

    printf("Padre: creo 2 thread\n");

    //Funzione di creazione thread
    pthread_create(&t1, NULL, (void *)uno, NULL);
    pthread_create(&t2, NULL, (void *)&due, NULL);

    printf("Padre: attendo la terminazione dei thread creati\n");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Padre: i thread sono terminati, chiudo applicazione\n");
    return 0;
}
