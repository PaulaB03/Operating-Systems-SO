#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

int **A, **B;
int r1, c1, r2, c2;

struct pos {
    int x, y;
};

// Function that calculates  the 
void* multiply(void *position) {
    struct pos *index = position;
    int *sum = (int *)malloc(sizeof(int));
    *sum = 0;

    for(int i=0; i < c1; ++i) 
        *sum += A[index->x][i] * B[i][index->y];

    return sum;    
}

// Function that prints a matrix
void print(int** M, int r, int c) {
    for(int i=0; i < r; ++i) {
        for(int j=0; j < c; ++j)
            printf("%d ", M[i][j]);
        printf("\n");
    }
}

int main() {
    // Read matrix A dimesions
    printf("Number of rows of the matrix A: ");
    scanf("%d", &r1);
    printf("Number of columns if the matrix A: ");
    scanf("%d", &c1);

    // Read matrix A elements
    printf("Elements in the matrix A:\n");
    A = (int**)malloc(sizeof(int) *r1);
    for(int i=0; i < r1; ++i) {
        A[i] = (int *)malloc(sizeof(int) *c1);

        for(int j=0; j < c1; ++j) 
            scanf("%d", &A[i][j]);
    }


    // Read matrix B dimesions
    printf("\nNumber of rows of the matrix B: ");
    scanf("%d", &r2);
    printf("Number of columns if the matrix B: ");
    scanf("%d", &c2);

    // Read matrix B elements
    printf("Elements in the matrix B:\n");
    B = (int**)malloc(sizeof(int) *r2);
    for(int i=0; i < r2; ++i) {
        B[i] = (int *)malloc(sizeof(int) *c2);

        for(int j=0; j < c2; ++j) 
            scanf("%d", &B[i][j]);
    }

    // Check if we can create matrix C
    if(c1 != r2) {
        printf("\nWe can't multiply the matrices\n");

        for(int i=0; i < r1; ++i) free(A[i]);
        for(int i=0; i < r2; ++i) free(B[i]);
        return 0;
    }

    // Print the matrices
    printf("\nMatrix A is:\n");
    print(A,r1,c1);
    printf("\nMatrix B is:\n");
    print(B,r2,c2);


    void *result;
    pthread_t thr[c2 * r1];
    int thr_index = 0;
    int C[r1][c2];

    // Create threads for every element in matrix C
    for(int i=0; i < r1; ++i)
    for(int j=0; j < c2; ++j) {
        struct pos* index = calloc(1, sizeof(struct pos));
        index-> x = i;
        index-> y = j;

        // Create the thread and check for errors
        if(pthread_create(&thr[thr_index++], NULL, multiply, index)) {
            perror("Error when creating a thread");
            return errno;
        }
    }

    thr_index = 0;
    // Join threads for every element in matrix C
    for(int i=0; i < r1; ++i)
    for(int j=0; j < c2; ++j) {
        // Join the thread and check for errors
        if(pthread_join(thr[thr_index++], &result)) {
            perror("Error when joining the thread");
            return errno;
        }

        C[i][j] = *((int *)result);
    }

    // Print matrix C
    printf("\nMatrix C is:\n");
    for(int i=0; i < r1; ++i) {
        for(int j=0; j < c2; ++j)
            printf("%d ", C[i][j]);
        printf("\n");
    }

    // Free memory
    for(int i=0; i < r1; ++i) free(A[i]);
    for(int i=0; i < r2; ++i) free(B[i]);
    free(A);
    free(B);
    return 0;
}