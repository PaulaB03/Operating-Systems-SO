#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    // Check if the format is "./shmcollatz 9 16 ..."
    if(argc < 2) {
        printf("Wrong format, the program needs at least an argument\n");
        return 0;
    }

    printf("Starting Parent: %d\n", getpid());    
    
    // Create the shared memory
    char shm_name[] = "collatz_shm";
    int shm_fd = open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    // Check for error when creating shm
    if(shm_fd < 0) {
        perror("Error when creating a shared memory object");
        return errno;
    }

    // Alocate memory for the object
    size_t shm_size = getpagesize() * argc;
    if(ftruncate(shm_fd, shm_size) == -1) {
        perror("Error when alocating memory");
        shm_unlink(shm_name);
        return errno;
    }


    char *shm_ptr;
    for(int i=1; i <= argc; ++i) {
        // Trunking memory
        shm_ptr = mmap(0, getpagesize(), PROT_WRITE, MAP_SHARED, shm_fd, getpagesize() * (i-1));
        
        // Check for  trunk memory error
        if(shm_ptr == MAP_FAILED) {
            perror("Error when trunking memory");
            shm_unlink(shm_name);
            return errno;
        }

        // Create fork process
        pid_t pid = fork();

        // Check for fork error
        if(pid < 0 ) {
            perror("Error when executing fork");
            return errno;
        }

        // Child instruction
        else if(pid == 0) {
            int num = atoi(argv[i]);

            // Load collantz in to the memory
            shm_ptr += sprintf(shm_ptr, "%d: %d ", num, num);

            while(num != 1) {
                if(num % 2 == 0 )
                    num /= 2;
                else
                    num = 3 * num + 1;
                
                shm_ptr += sprintf(shm_ptr, "%d ", num);
            }

            // The child finished it's instrutions
            printf("Done Parent %d Me %d\n", getppid(), getpid());
            return 1;
        }

        // Free the memory
        munmap(shm_ptr, getpagesize());
    }

    for(int i=1; i < argc; ++i) {
        wait(NULL);

        // Print collatz_shm from the memory
        shm_ptr = mmap(0, getpagesize(), PROT_READ, MAP_SHARED, shm_fd, getpagesize() * (i-1));
        printf("%s\n", shm_ptr);
        munmap(shm_ptr, getpagesize());
    }

    // Call the destructor
    shm_unlink(shm_name);
    printf("Done Parent %d Me %d\n", getppid(), getpid());
    return 0;
}
