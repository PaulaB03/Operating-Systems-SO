#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#define NO_THREADS 10
#define INCREMENT_PER_THREAD 1e+6
#define MAX_RESOURCES 5

int available_resources = MAX_RESOURCES;
pthread_mutex_t mtx;

// Decrease function
int decrease_count(int count) {
    pthread_mutex_lock(&mtx);

    if(available_resources < count) {
        pthread_mutex_unlock(&mtx);
        return -1;
    }
    else {
        available_resources -= count;
        printf("Got %d resources, remaining %d resources\n", count, available_resources);
    }
    
    pthread_mutex_unlock(&mtx);
    return 0;
}


// Increase function
int increase_count(int count) {
    pthread_mutex_lock(&mtx);

    available_resources += count;
    printf("Released %d resources, remaining %d resources\n", count, available_resources);

    pthread_mutex_unlock(&mtx);
    return 0;
}

// Thread function
void* threadFunction(void *params) {
    int res = rand()%MAX_RESOURCES+1;

    // Check if we have enough resources
    if(decrease_count(res) == -1) {
        // We'll wait a second to see if some resourses get released
        sleep(1);

        // If we still don't have enough resources, we stop the function
        if(decrease_count(res) == -1) 
            return NULL;
    }

    // Wait a second so that the resources aren't realased immediately
    sleep(1);
    increase_count(res);
}

int main() {
    // Create mutex and check for error
    if(pthread_mutex_init(&mtx, NULL)) {
        perror("Error when executing mutex");
        return errno;
    }

    printf("MAX_RESOURCES = %d\n", MAX_RESOURCES);
    pthread_t thr[NO_THREADS];

    // Create the threads and check for errors
    for(int i=0; i < NO_THREADS; ++i) {
        if(pthread_create(&thr[i], NULL, threadFunction, NULL)) {
            perror("Error when creating a thread");
            return errno;
        }
    }
    
    // Join thread and check for errors
    for(int i=0; i < NO_THREADS; ++i) 
        if(pthread_join(thr[i], NULL)) {
            perror("Error when joining the thread");
            return errno;
        }
    
    // Destroy mutex
    pthread_mutex_destroy(&mtx);
    return 0;
}
