#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

// Function to reverse and print a string
void* reverse(void *str) {
    char *str1 = (char *)str;
    char *rvs = (char *)malloc(strlen(str1));

    for(int i=0; i < strlen(str1); ++i) 
        rvs[strlen(str1)-1-i] = str1[i];
    
    printf("%s\n", rvs);

    // Free memory
    free(str1);
    free(rvs);
    return NULL;
}

int main(int argc, char **argv) {
    // Check if the format is "./strrev hello"
    if(argc != 2) {
        printf("Wrong format, the program needs an argument\n");
        return 0;
    }

    void *result;
    pthread_t thr;

    // Create the thread and check for errors
    if(pthread_create(&thr, NULL, reverse, argv[1])) {
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