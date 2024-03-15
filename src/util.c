#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>
#include <math.h>

#include "matmul.h"
#include "util.h"

void transpose_matrix(DTYPE*);
void print_matrix(DTYPE*);
void rand_matrix(DTYPE*);
void zero_matrix(DTYPE*);
bool check_matrix(DTYPE*, DTYPE*);
void test_program(const char* name, void (*func)(DTYPE*, DTYPE*, DTYPE*),
                  DTYPE* a, DTYPE* b, DTYPE* c, DTYPE* ans);

void transpose_matrix(DTYPE* mat) {
  for (int i = 0; i < N; i++) {
    for (int j = i; j < N; j++) {
      DTYPE tmp = mat[j*N+i];
      mat[j*N+i] = mat[i*N+j];
      mat[i*N+j] = tmp;
    }
  }
}

void rand_matrix(DTYPE* mat) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      mat[i*N+j] = (DTYPE) rand() / (DTYPE) RAND_MAX;
    }
  }
}

void zero_matrix(DTYPE* mat) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      mat[i*N+j] = 0;
    }
  }
}

bool check_matrix(DTYPE* mat, DTYPE* ans) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      DTYPE diff = fabsf(mat[i*N+j] - ans[i*N+j]);
      if (diff > ERR) {
        printf("failed: answer does not match. difference: %2f at (%d, %d)\n",
               diff, i, j);
        return false;
      }
    }
  }
  return true;
}

void print_matrix(DTYPE* mat) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%.2f ", mat[i*N+j]);
    }
    printf("\n");
  }
  printf("\n");
}

double time_to_gflops_s(const double seconds) {
  double total_flops = 2.0 * N * N * N;
  double gflops_second = total_flops / (seconds * 1e9);
  return gflops_second;
}

void test_program(const char* name, void (*func)(DTYPE*, DTYPE*, DTYPE*),
                  DTYPE* a, DTYPE* b, DTYPE* c, DTYPE* ans) {
  double begin = omp_get_wtime();
  func(a, b, c);
  double seconds = omp_get_wtime() - begin;
  #ifdef DEBUG
  printf("%s: %.3f s\n", name, seconds);
  #endif
  printf("%s: %.1f GFLOPS/s\n", name, time_to_gflops_s(seconds));
  #ifdef DEBUG
  print_matrix(c);
  printf("ans:\n");
  print_matrix(ans);
  #endif
  assert(check_matrix(c, ans));
  zero_matrix(c);
}

