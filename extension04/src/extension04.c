#include <stdint.h>
#include <stdio.h>

#include "qutyserial.h"
#include "extension04.h"

// Ex E4.0
// 
// Matrix arithmetic operations are commonly required in many
// algorithms. In this exercise you are required to write four 
// functions which will implement some common matrix operations.
//
// Specifically:
//     matrix_sum()   - Matrix addition, A+B
//     matrix_add()   - Add a scalar to a matrix A+k
//     matrix_scale() - Scale (multiply) a matrix by a scalar, A*k
//     matrix_mul()   - Matrix multiplication, A*B
//
// A matrix of dimensions m x n has m rows and n columns, so we can
// represent one in C as a 2-dimensional array of the form matrix[m][n].
// 
// Example. Consider a matrix A of dimension 2 x 2, and a matrix B 
// of dimension 2 x 3:
//
// A = 
// | 1 2 |
// | 3 4 |
//
// B =
// | 1 2 3 |
// | 4 5 6 |
//
// In matrix addition we take element-by element sums, e.g.:
//
// B+B = 
// | 1 2 3 | + | 1 2 3 | = | 1+1 2+2 3+3 | = | 2  4  6 |
// | 4 5 6 |   | 4 5 6 |   | 4+4 5+5 6+6 |   | 8 10 12 |
//
// The two input matricies must have the same dimension and
// the result will be of the same dimension.
//
// In matrix-scalar addition we add a scalar to every element, 
// e.g.:
//
// B+1 = 
// | 1 2 3 | + 1 = | 1+1 2+1 3+1 | = | 2  3  4 |
// | 4 5 6 |       | 4+1 5+1 6+1 |   | 5  6  7 |
//
// The result will be of the same dimension as the input.
//
// To scale a matrix, we multiply every element by a scalar, 
// e.g.:
//
// B*2 = 
// | 1 2 3 | * 2 = | 1*2 2*2 3*2 | = | 2  4  6 |
// | 4 5 6 |       | 4*2 5*2 6*2 |   | 8 10 12 |
//
// The result will be of the same dimension as the input.
//
// In matrix multiplication we take the dot-product of the rows
// of the LHS matrix with the columns of the RHS matrix. The dot-
// product is an element by element multiplication. e.g.:
//
// A*B = 
// [ 1 2 ] * [ 1 2 3 ] = [ (1*1 + 1*4)  (1*2 + 2*5)  (1*3 + 2*6) ] = [  9 12 15 ]  
// [ 3 4 ]   [ 4 5 6 ]   [ (3*1 + 4*4)  (3*2 + 4*5)  (3*3 + 4*6) ]   [ 19 26 33 ]
//
// The inner dimensions of the matricies must be the same and the
// result will be a matrix of dimension equal to the outer dimensions. 
// e.g. the result of matrix multiplication of an m x n matrix and n x p matrix 
// will be an m x p matrix.
//
// You can find lots of resources on-line which define these operations in detail, 
// e.g. https://online.stat.psu.edu/statprogram/reviews/matrix-algebra/arithmetic
//
// A matrix shall be represented as a 2-dimensional array of 16-bit
// signed integers. When passing a matrix as an argument to a function,
// a pointer to the first element of the array should be used. As matricies
// can have different dimensions, when passing a matrix as an argument you
// must also pass the pointer to a two-element 8-bit unsigned integer array 
// containing the number of rows in the first element and number of columns 
// in the second element. Scalars shall be passed as a 16-bit signed integer.
// it is not necessary to pass the dimension of the result matrix as this is
// fully defined by the dimension of the input operands.
// 
// The order of arguments for your functions shall be as follows:
// 
// matrix_sum()   - matrix A, dimension of A, matrix B, result matrix
// matrix_add()   - matrix A, dimension of A, scalar addend, result matrix
// matrix_scale() - matrix A, dimension of A, scalar multiplicand, result matrix
// matrix_mul()   - matrix A, dimension of A, matrix B, dimension of B, result matrix
//
// You must define prototypes for your functions in extension06.h so that these
// may be called by external code. The implementation of your functions should be
// in extension06.c.
//
// You can write code to test your functions in main.c. We've provided some example
// code to call the functions you write, and print matricies via the serial interface.
// main.c will be replaced by our own test code when we test your functions.

int main(void) {

    serial_init();

    int16_t matrix_a[2][2] = {
        {1, 2},
        {3, 4}
    };
    int16_t* a = (int16_t*) matrix_a;

    int16_t matrix_b[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };
    int16_t* b = (int16_t*) matrix_b;

    int16_t matrix_identity[2][2] = {
        {1, 0},
        {0, 1}
    };
    int16_t* identity = (int16_t*) matrix_identity;


    int16_t result[6];
    
    int16_t multiplicand = 2;
    int16_t addend = 2;

    uint8_t dim_2x2[2] = {2, 2};
    uint8_t dim_2x3[2] = {2, 3};

    zero(result, 6);

    while(!serial_bytes_available());

    printf("A =\n");
    printm(a, dim_2x2);

    printf("B =\n");
    printm(b, dim_2x3);

    zero(result, 6);
    matrix_add(a, dim_2x2, addend, result);
    printf("A+k =\n");
    printm(result, dim_2x2);

    zero(result, 6);
    matrix_sum(a, dim_2x2, identity, result);
    printf("A+I =\n");
    printm(result, dim_2x2);

    zero(result, 6);
    matrix_scale(a, dim_2x2, multiplicand, result);
    printf("A*k =\n");
    printm(result, dim_2x2);

    zero(result, 6);
    matrix_mul(identity, dim_2x2, a, dim_2x2, result);
    printf("I*A =\n");
    printm(result, dim_2x2);

    zero(result, 6);
    matrix_mul(identity, dim_2x2, b, dim_2x3, result);
    printf("I*B =\n");
    printm(result, dim_2x3);

    while (1); 
}

// Write your code for Ex E4.0 above this line.
