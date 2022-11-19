#include <stdio.h>
#include <pthread.h>
#include <errno.h>

// Function to print hello world
void* hello (void *v) {
    char *who = (char *) v;
    printf("Hello, %s!\n", who);
    return NULL;
}

int main() {
    pthread_t thr;
    void *result;

    // Create thread and check for errors
    if(pthread_create(&thr, NULL, hello, "world!")) {
        perror("Error when creating a thread");
        return errno;
    }

    // Join thread and check for errors
    if(pthread_join(thr, &result)) {
        perror("Error when joining the thread");
        return errno;
    }

    return 0;
}