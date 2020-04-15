#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t s1, s2;

void *ping(void *arg){
    while(1){
        sem_wait(&s2);
        printf("Ping\n");
        sleep(1);
        sem_post(&s1);
    }
    pthread_exit(0);
}
void *pong(void *arg){
    while(1){
        sem_wait(&s1);
        printf("Pong\n");
        sleep(1);
        sem_post(&s2);
    }
    pthread_exit(0);
}

int main(int argc, char **argv){
    pthread_t t1, t2;

    sem_init(&s1, 0, 0); // Bloccato
    sem_init(&s2, 0, 2); // Sbloccato

    pthread_create(&t1, NULL, ping, NULL);
    pthread_create(&t2, NULL, pong, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
