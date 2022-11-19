#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if(pid < 0) {
        perror("Error when executing fork");
        return errno;
    }
    else if(pid == 0) {
        // Child instruction
        printf("First!\n");

        // Execve instruction
        char *argv[] = {"pwd", NULL};
        execve("/bin/pwd", argv, NULL);
    }
    else {
        // Parent instruction
        wait(NULL);
        printf("Last!\n");
    }

    return 0;
}