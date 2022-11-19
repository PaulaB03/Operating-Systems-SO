#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    char shm_name[] = "myshm";
    int shm_fd;

    // Creates a shared memory object
    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // Check for errors
    if(shm_fd < 0) {
        perror("Error on creating a shared memory object");
        return errno;
    }

    // Alocate memory for the object
    size_t shm_size = 1000;
    if(ftruncate(shm_fd, shm_size) == -1) {
        perror("Error when alocationg memory");
        shm_unlink(shm_name);
        return errno;
    }
    // Now the object when from 0 bytes to 1000 


    // The memory loads in the processor with mmap function
    char *shm_ptr;
    shm_ptr = mmap(0, shm_size, PROT_READ, MAP_SHARED, shm_fd, 0);
    // Check for error
    if (shm_ptr == MAP_FAILED) {
        perror("Error when truncating memory");
        shm_unlink(shm_name);
        return errno;
    }

    // Call the destructor and free memory
    shm_unlink(shm_name);
    munmap(shm_ptr, shm_size);
    return 0;
}