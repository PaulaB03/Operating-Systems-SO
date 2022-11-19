#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to generate the collatz operation
void collatz(int number) {
    printf("%d ", number);

    if(number >1) 
        if(number % 2 == 0)
            collatz(number /2);
        else
            collatz(3*number + 1);
}

int main(int argc, char ** argv) {
    // Check if the format is "./collatz 24"
    if(argc != 2) {
        printf("Wrong format, the program needs an argument.\n");
        return 0;
    }
    
    // Get number and create fork process
    int num = atoi(argv[1]);
    pid_t pid = fork();

    // Check for fork error
    if(pid < 0) {
        perror("Error when executing fork");
        return errno;
    }
    
    // Child instruction
    else if(pid == 0) {
        printf("%d: ", num);
        collatz(num);
    }

    // Parent instruction
    else {
        wait(NULL);
        printf("\nChild %d finished\n", getpid());
    }

    return 0;
}