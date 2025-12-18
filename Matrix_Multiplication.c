#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/time.h>
//for getting time in matrix multipliaction


typedef struct {
    int rows;
    int cols;
    int **data;
} Matrix;


//element wise struct
//meams that each number will be computed using thread 
/*
for example 1 2 3
            4 5 6
and 1 2
    3 4
    5 6 

    a[0][0]*b[0][0] + a[0][1]*b[1][0] + a[0][2]*b[2][0] = c[0][0]
    a[0][0]*b[0][1] + a[0][1]*b[1][1] + a[0][2]*b[2][1] = c[0][1]
    a[1][0]*b[0][0] + a[1][1]*b[1][0] + a[1][2]*b[2][0] = c[1][0]
    a[1][0]*b[0][1] + a[1][1]*b[1][1] + a[1][2]*b[2][1] = c[1][1]
    so, 4 threads as 4 numbers in the matrix 
*/
typedef struct {
    int row;
    int col;
    Matrix *mat1;
    Matrix *mat2;
    Matrix *result;
} element_args;

//normal row wise struct
//one thread per row
//so in the prev example 2 threads as only 2 rows in teh result matrix 
typedef struct {
    int row;
    Matrix *mat1;
    Matrix *mat2;
    Matrix *result;
} row_args;


// Function to allocate matrix
Matrix* createMatrix(int rows, int cols) {

    Matrix *mat = malloc (sizeof( Matrix));
    mat->rows = rows;
    mat->cols = cols;

    mat->data = malloc ( rows * sizeof(int *));

    for (int i = 0; i < rows; i++) {
        mat->data[i] = malloc ( cols * sizeof( int));
    }
    return mat;

}

// Function to free matrix
void freeMatrix(Matrix *mat) {
    if (mat) {

        for (int i = 0; i < mat->rows; i++) {

            free(mat->data[i]);
        }
        free(mat->data);
        free(mat);
    }

}


// Function to read matrices from file
int readMatrices(Matrix **mat1, Matrix **mat2) {
    
    FILE *file = fopen("matrices_input.txt", "r");
    if (!file) {
        return -1;
    }

    int r1, c1, r2, c2;
    fscanf(file, "%d %d", &r1, &c1);
    *mat1 = createMatrix(r1, c1);
    
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c1; j++) {
            fscanf(file, "%d", &((*mat1)->data[i][j]));
        }
    }

    fscanf(file, "%d %d", &r2, &c2);
    
    *mat2 = createMatrix(r2, c2);
    for (int i = 0; i < r2; i++) {
        for (int j = 0; j < c2; j++) {
            fscanf(file, "%d", &((*mat2)->data[i][j]));
        }
    }

    fclose(file);
    return 0;
    
}

// Function to print matrix
void printMatrix(Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%d", mat->data[i][j]);
            if (j < mat->cols - 1) printf(" ");
        }
        printf("\n");
    }
}


// Method 1: One thread per element
//changes the declarration to be struct for simplicuty instead
void *multiply_by_element(void *args) {

    element_args *data = (element_args *)args;
    int sum = 0;
    for (int i = 0; i < data->mat1->cols; i++) {
        sum += (data->mat1->data[data->row][i]) * (data->mat2->data[i][data->col]);
    }

    data->result->data[data->row][data->col] = sum;
    free(data);
    return NULL;
}

// Method 2: One thread per row
void *multiply_by_row(void *args) {
    row_args *data = (row_args *)args;

    for (int j = 0; j < data->mat2->cols; j++) {
        int sum = 0;
        for (int k = 0; k < data->mat1->cols; k++) {
            sum += data->mat1->data[data->row][k] * data->mat2->data[k][j];
        }
        data->result->data[data->row][j] = sum;
    }

    free(data);
    return NULL;
}



void multiply_by_element_threads(Matrix *mat1, Matrix *mat2, Matrix **result) {
    *result = createMatrix(mat1->rows, mat2->cols);

    pthread_t threads[mat1->rows][mat2->cols]; 
    //as we want threads=number of elments in the result matrix

    for (int i = 0; i < mat1->rows; i++) {
        for (int j = 0; j < mat2->cols; j++) {
            element_args *args = malloc(sizeof(element_args));

            args->row = i; //for result matrix
            args->col = j;
            args->mat1 = mat1;
            args->mat2 = mat2;
            args->result = *result;

            pthread_create(&threads[i][j], NULL, multiply_by_element, args);
            
        }
    }

    for (int i = 0; i < mat1->rows; i++) {
        for (int j = 0; j < mat2->cols; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }
}



void multiply_by_row_threads (Matrix *mat1, Matrix *mat2, Matrix **result) {
    *result = createMatrix(mat1->rows, mat2->cols);
    pthread_t threads[mat1->rows];

    for (int i = 0; i < mat1->rows; i++) {
        row_args *args = malloc(sizeof(row_args));
        args->row = i;
        args->mat1 = mat1;
        args->mat2 = mat2;
        args->result = *result;
        pthread_create(&threads[i], NULL, multiply_by_row, args);
    }

    for (int i = 0; i < mat1->rows; i++) {
        pthread_join(threads[i], NULL);
    }
}


int main() {

    Matrix *mat1, *mat2, *result1, *result2;

    struct timeval start, end;
    double total_time1;
    double total_time2;
    // Read matrices from file
    if (readMatrices(&mat1, &mat2) != 0) {
        fprintf(stderr, "Error reading matrices\n");
        return 1;
    }

    // Method 1: Per element
    gettimeofday(&start, NULL);
    multiply_by_element_threads(mat1, mat2, &result1);
    gettimeofday(&end, NULL);

    total_time1 = (end.tv_sec - start.tv_sec) * 1000.0;      // sec to ms
    total_time1 += (end.tv_usec - start.tv_usec) / 1000.0;   // us to ms
    printf("First method:\n");
    printMatrix(result1);
    printf("\n");
    printf("Time taken: %.4f ms\n", total_time1);
    printf("\n");

    
    
    

    // Method 2: Per row
    
    gettimeofday(&start, NULL);
    multiply_by_row_threads (mat1, mat2, &result2);
    gettimeofday(&end, NULL);

    total_time2 = (end.tv_sec - start.tv_sec) * 1000.0;      // sec to ms
    total_time2 += (end.tv_usec - start.tv_usec) / 1000.0;   // us to ms
    printf("Second method:\n");
    printMatrix(result2);
    printf("\n");
    printf("Time taken: %.4f ms\n", total_time2);
    printf("\n");

    
    
   


    // Cleanup
    freeMatrix(mat1);
    freeMatrix(mat2);
    freeMatrix(result1);
    freeMatrix(result2);

    return 0;
}