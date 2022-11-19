#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#define NTHRS 5

pthread_mutex_t mtx;
sem_t sem;
int visited = 0;

void barrier_point() {
    pthread_mutex_lock(&mtx);

    // Check if all wire reached the barrier
    if(++visited == NTHRS){
        pthread_mutex_unlock(&mtx);
        sem_post(&sem);
        return;
    }
    pthread_mutex_unlock(&mtx);

    sem_wait(&sem);
    sem_post(&sem);
}

// Thread function
void* tfun(void *v) {
    int *tid = (int *)v;

    printf("%d reached the barrier\n", *tid);
    barrier_point();
    printf("%d passed the barrier\n", *tid);

    free(tid);
    return NULL;
}

int main() {
    pthread_t thr[NTHRS];

    // Create semaphore and check for errors
    if(sem_init(&sem, 0, 0)) {
        perror("Error when creating semaphore");
        return errno;
    }

    // Create mutex and check for error
    if(pthread_mutex_init(&mtx, NULL)) {
        perror("Error when executing mutex");
        return errno;
    }

    // Create the threads and check for errors
    for(int i=0; i < NTHRS; ++i) {
        int *current = malloc(sizeof(int));
        *current = i;
        if(pthread_create(&thr[i], NULL, tfun, current)) {
            perror("Error when creating a thread");
            return errno;
        }
    }

    // Join thread and check for errors
    for(int i=0; i < NTHRS; ++i) 
        if(pthread_join(thr[i], NULL)) {
            perror("Error when joining the thread");
            return errno;
        }
    
    // Destroy mutex
    sem_destroy(&sem);
    pthread_mutex_destroy(&mtx);
    return 0;
}