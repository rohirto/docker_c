/**
 * Develop a multi-threaded application to perform 3X3 matrix multiplication, each cell of resultant matrix needs to be calculated in separate thread
*/

#include <stdio.h>
#include <pthread.h>

#define ROWS 3
#define COLS 3

int mat_a[ROWS][COLS] = 
{
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};

int mat_b[ROWS][COLS] = 
{
    {1, 4, 7},
    {2, 5, 8},
    {3, 6, 9}
};

int mat_res[ROWS][COLS];

// Struct to pass matrix indices to thread
typedef struct 
{
    int row;
    int col;
} matrix_index;

// Function to calculate a single cell in the resultant matrix
void* multiply(void* arg) 
{
    matrix_index* index = (matrix_index*)arg;
    int row = index->row;
    int col = index->col;

    int sum = 0;
    for (int i = 0; i < COLS; i++) 
    {
        sum += mat_a[row][i] * mat_b[i][col];
    }

    mat_res[row][col] = sum;

    pthread_exit(NULL);
}

int main() 
{
    pthread_t threads[ROWS][COLS];
    matrix_index indices[ROWS][COLS];

    // Create threads to calculate each cell in the mat_res
    for (int i = 0; i < ROWS; i++) 
    {
        for (int j = 0; j < COLS; j++) 
        {
            indices[i][j].row = i;
            indices[i][j].col = j;
            pthread_create(&threads[i][j], NULL, multiply, &indices[i][j]);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < ROWS; i++) 
    {
        for (int j = 0; j < COLS; j++) 
        {
            pthread_join(threads[i][j], NULL);
        }
    }

    // Display the result matrix
    printf("Resultant Matrix:\n");
    for (int i = 0; i < ROWS; i++) 
    {
        for (int j = 0; j < COLS; j++) 
        {
            printf("%d ", mat_res[i][j]);
        }
        printf("\n");
    }

    return 0;
}
