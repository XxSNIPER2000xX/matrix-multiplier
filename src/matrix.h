#ifndef MATRIX_H
#define MATRIX_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct S_MATRIX {
    int rows, cols;
    int *data;
} mat;

typedef struct S_TASK {
    int row_start, row_end;
    mat *a, *b, *c;
} task;

static const int g_multithread_when_greater_than = 256;
static const int g_num_threads = 8;

static void __c_matrix(mat *mat1, mat *mat2, mat *out);
static void __matrix_multi_iterative(mat *mat1, mat *mat2, mat *out);
static void __matrix_multi_threads(mat *mat1, mat *mat2, mat *out);
static void *__worker(void *args);
void matrix_multi(mat *mat1, mat *mat2, mat *out);

#endif // MATRIX_H

#if defined(MATRIX_IMPLEMENTATION)

static void __c_matrix(mat *mat1, mat *mat2, mat *out) {
    out->rows = mat1->rows;
    out->cols = mat2->cols;
    out->data = malloc(out->rows * out->cols * sizeof(int));

    for(int i = 0; i < out->rows; i++) {
        for(int j = 0; j < out->cols; j++) out->data[i * out->cols + j] = 0;
    }
}

static void __matrix_multi_iterative(mat *mat1, mat *mat2, mat *out) {
    for(int i = 0; i < mat1->rows; i++) {
        for(int k = 0; k < mat1->cols; k++) {
            int A = mat1->data[i * mat1->cols + k];
            for(int j = 0; j < mat2->cols; j++)
                out->data[i * out->cols + j] += A * mat2->data[k * mat2->cols + j];
        }
    }
}

static void __matrix_multi_threads(mat *mat1, mat *mat2, mat *out) {
    pthread_t *threads = malloc(g_num_threads * sizeof(pthread_t));
    task *tasks = malloc(g_num_threads * sizeof(task));

    int rows_per_thread = mat1->rows / g_num_threads;
    for(int i = 0; i < g_num_threads; i++) {
        tasks[i].a = mat1;
        tasks[i].b = mat2;
        tasks[i].c = out;

        tasks[i].row_start = i * rows_per_thread;
        tasks[i].row_end = (i == g_num_threads - 1) ? mat1->rows : (i + 1) * rows_per_thread;

        pthread_create(&threads[i], NULL, __worker, &tasks[i]);
    }

    for(int i = 0; i < g_num_threads; i++) pthread_join(threads[i], NULL);

    free(threads);
    free(tasks);
}

static void *__worker(void *args) {
    task *tasks = (task *)args;

    for(int i = tasks->row_start; i < tasks->row_end; i++) {
        for(int k = 0; k < tasks->a->cols; k++) {
            int A = tasks->a->data[i * tasks->a->cols + k];
            for(int j = 0; j < tasks->b->cols; j++)
                tasks->c->data[i * tasks->c->cols + j] += A * tasks->b->data[k * tasks->b->cols + j];
        }
    }

    return NULL;
}

void matrix_multi(mat *mat1, mat *mat2, mat *out) {
    if(mat1->cols != mat2->rows) {
        fprintf(stderr, "invalid matrix!!!\n");
        exit(EXIT_FAILURE);
    }

    __c_matrix(mat1, mat2, out);

    if(mat1->cols < g_multithread_when_greater_than)
        __matrix_multi_iterative(mat1, mat2, out);
    else
        __matrix_multi_threads(mat1, mat2, out);
}

#endif // MATRIX_IMPLEMENTATION
