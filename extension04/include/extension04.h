#include <stdint.h>

// matrix_sum()   - matrix A, dimension of A, matrix B, result matrix
// matrix_add()   - matrix A, dimension of A, scalar addend, result matrix
// matrix_scale() - matrix A, dimension of A, scalar multiplicand, result matrix
// matrix_mul()   - matrix A, dimension of A, matrix B, dimension of B, result matrix

void zero(int16_t* matrix, uint8_t elements) {
    for (uint8_t i = 0; i < elements; i++) {
        *(matrix+i) = 0;
    }
}

void printm(int16_t* matrix, uint8_t* dim) {
    for (uint8_t i = 0; i < dim[0]; i++) {
        for (uint8_t j = 0; j < dim[1]; j++) {
            printf("% 6d ", matrix[i*dim[1]+j]);
        }
        printf("\n");
    }
}

void matrix_sum(int a[][2], int dim[], int b[][2], int result[][2]) {
    int i, j;
    for (i = 0; i < dim[0]; i++) {
        for (j = 0; j < dim[1]; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void matrix_add(int a[][2], int dim[], int addend, int result[][2]) {
    int i, j;
    for (i = 0; i < dim[0]; i++) {
        for (j = 0; j < dim[1]; j++) {
            result[i][j] = a[i][j] + addend;
        }
    }
}


void matrix_scale(int a[][2], int dim[], int multiplicand, int result[][2]) {
    int i, j;
    for (i = 0; i < dim[0]; i++) {
        for (j = 0; j < dim[1]; j++) {
            result[i][j] = a[i][j] * multiplicand;
        }
    }
}

void matrix_mul(int a[][2], int dimA[], int b[][2], int dimB[], int result[][2]) {
    if (dimA[1] != dimB[0]) {
        printf("Invalid matrix dimensions\n");
        return;
    }
    
    int i, j, k;
    for (i = 0; i < dimA[0]; i++) {
        for (j = 0; j < dimB[1]; j++) {
            result[i][j] = 0;
            for (k = 0; k < dimA[1]; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}