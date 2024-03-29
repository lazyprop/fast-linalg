#include <iostream>
#include <iomanip>
#include <omp.h>

#include "matmul.h"
#include "util.h"

int main() {
  const int N = 1024;

  float* a = static_cast<float*>(std::aligned_alloc(32, sizeof(float) * N * N));
  float* b = static_cast<float*>(std::aligned_alloc(32, sizeof(float) * N * N));
  float* c = static_cast<float*>(std::aligned_alloc(32, sizeof(float) * N * N));
  float* ans = static_cast<float*>(std::aligned_alloc(32, sizeof(float) * N * N));

  rand_matrix<float, N>(a);
  rand_matrix<float, N>(b);

  #ifdef DEBUG
  std::cout << "a:\n";
  print_matrix(a);
  std::cout << "b:\n";
  print_matrix(b);
  #endif

  std::cout << std::fixed << std::setprecision(2);

  test_program<float, N>("baseline", baseline<float, N>, a, b, ans, ans);

  zero_matrix<float, N>(c);
  transpose_matrix<float, N>(b);
  test_program<float, N>("transposed", transposed<float, N>, a, b, c, ans);
  test_program<float, N>("transpose_simd", transpose_simd<float, N>, a, b, c, ans);
  transpose_matrix<float, N>(b);

  //test_program<float, N>("blocked_2x2", blocked_2x2<float, N>, a, b, c, ans);
  //test_program<float, N>("blocked_16x16", blocked<float, N, 16>, a, b, c, ans);

  //test_program<float, N>("blocked2_16x16", blocked2<float, N, 16>, a, b, c, ans);
  test_program<float, N>("blocked3_8x8", blocked3<float, N, 8>, a, b, c, ans);

  //test_program<float, N>("parallel", parallel<float, N>, a, b, c, ans);
  transpose_matrix<float, N>(b);
  test_program<float, N>("parallel_tranposed_simd",
                         parallel_tranposed_simd<float, N>, a, b, c, ans);

  return 0;
}
