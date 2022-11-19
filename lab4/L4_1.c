#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // Create fork process
    pid_t pid = fork();

    // Check for fork error
    if(pid < 0) {
        perror("Error when executing fork");
        return errno;
    }

    // Parent instruction
    else if(pid > 0) {
        // Print Parent and Child id
        printf("My PID=%d, Child PID=%d\n", getppid(), getpid());

        // Wait for the child
        pid_t pid = wait(NULL);
        printf("Child %d finished\n", getpid());
    }

    // Child instruction
    else {
        // Print the files in the current directory
        char *argv[] = {"ls", NULL};
        execve("/bin/ls", argv, NULL);

        perror(NULL);
    }

    return 0;
}