#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#define NO_THREADS 10
#define INCREMENT_PER_THREAD 1e+6

static int g_count = 0;
pthread_mutex_t mtx;

// Thread function using mutex
void* threadFunction(void * params) {
    pthread_mutex_lock(&mtx);

    for(int i=0; i < INCREMENT_PER_THREAD; ++i) 
        ++g_count;
    
    pthread_mutex_unlock(&mtx);
    return NULL;
}

int main() {
    pthread_t thr[NO_THREADS];

    // Create mutex and check for error
    if(pthread_mutex_init(&mtx, NULL)) {
        perror("Error when executing mutex");
        return errno;
    }

    // Create the threads and check for errors
    for(int i=0; i < NO_THREADS; ++i) 
        if(pthread_create(&thr[i], NULL, threadFunction, NULL)) {
            perror("Error when creating a thread");
            return errno;
        }

    // Join thread and check for errors
    for(int i=0; i < NO_THREADS; ++i)
        if(pthread_join(thr[i], NULL)) {
            perror("Error when joining the thread");
            return errno;
        }
    
    // Destroy mutex
    pthread_mutex_destroy(&mtx);
    printf("%d\n", g_count);
    return 0;
}