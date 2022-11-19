#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>


int copy_file(int f1, int f2) {
    const int BUFFSIZE = 1024;
    char * buffer = malloc(sizeof(char) * BUFFSIZE);
    int in, out;

    // Read from the read file
    while(in=read(f1, buffer, BUFFSIZE) > 0) {
        // Write function
        out = write(f2, buffer, BUFFSIZE);
        printf("%s", buffer);

        // Check for write error
        if(out == -1) {
            perror("Error when writing the file");
            return errno;
        }
    }

    // Check for read error
    if(in == -1) {
        perror("Error when reading the file");
        return errno;
    }
}


int main(int argc, char** argv) {
    // Check if the format is "./mycp foo bar"
    if(argc != 3) {
        printf("Wrong format, the program needs two arguments.\n");
        return 0;
    }
    
    char* file1 = argv[1];
    char* file2 = argv[2];
    int f1, f2;

    // Open the file
    f1 = open(file1, O_RDONLY);
    f2 = open(file2, O_WRONLY | O_CREAT, S_IRWXU);

    // Check if the read file exist
    if(f1 == -1) {
        perror("Error when opening read file");
        return errno;
    }

    // Check if the write file exist
    if (f2 == -1) {
        close(f1);
        perror("Error when opening/creating write file");
        return errno;
    }

    copy_file(f1, f2);
    printf("\n");

    // Close files
    close(f1);
    close(f2);
    return 0;
}