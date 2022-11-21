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
    if(argc < 2) {
        printf("Wrong format, the program needs at least an argument.\n");
        return 0;
    }
    
    printf("Starting parent %d\n", getppid());
    pid_t pid[argc];

    for(int i=1; i <= argc; ++i) {
        pid[i] = fork();

        // Check for fork error
        if(pid[i] < 0) {
            perror("Error when executing fork");
            return errno;
        }

        // Child instruction
        else if (pid[i] == 0) {
            int num = atoi(argv[i]);
            printf("%d: ", num);
            collatz(num);
            printf("\n");
            exit(0);
        }
    }

    // Parent instruction
    for(int i=1; i <= argc; ++i) 
            printf("Done Parent %d Me %d\n", getppid(), pid[i]);

    return 0;
}
