#define MATRIX_IMPLEMENTATION
#include "matrix.h"

void fill_matrix(mat *out);
void print_matrix(mat *mat, int index);

int main(void) {
    mat mat1 = {
        .rows = 500,
        .cols = 500,
        .data = malloc(mat1.rows * mat1.cols * sizeof(int))
    };

    mat mat2 = {
        .rows = 500,
        .cols = 500,
        .data = malloc(mat2.rows * mat2.cols * sizeof(int))
    };

    mat mat3;

    fill_matrix(&mat1);
    fill_matrix(&mat2);

    matrix_multi(&mat1, &mat2, &mat3);

    free(mat1.data);
    free(mat2.data);
    free(mat3.data);
    return 0;
}

void fill_matrix(mat *out) {
    for(int i = 0; i < out->rows; i++) {
        for(int j = 0; j < out->cols; j++)
            out->data[i * out->cols + j] = i * out->rows + j;
    }
}

void print_matrix(mat *mat, int index) {
    for(int i = 0; i < mat->rows; i++) {
        for(int j = 0; j <  mat->cols; j++)
            printf("matrix%d[%d][%d] = %d\n", index, i, j, mat->data[i * mat->cols + j]);
    }
}
