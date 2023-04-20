#include "mat_mul.h"
#include <iostream>
#include <stdlib.h>

void mat_mul(AXI_STREAM &in_strm, AXI_STREAM &out_strm);

void mat_mul_naive_soft(const int a[SIZE][SIZE], const int b[SIZE][SIZE],
                        int res[SIZE][SIZE]) {
  for (int i = 0; i < SIZE; ++i)
    for (int j = 0; j < SIZE; ++j) {
      res[i][j] = 0;
      for (int k = 0; k < SIZE; ++k)
        res[i][j] += a[i][k] * b[k][j];
    }
}

void mat_eq(const int res[SIZE][SIZE], const int expected[SIZE][SIZE]) {
  for (int i = 0; i < SIZE; ++i)
    for (int j = 0; j < SIZE; ++j)
      if (res[i][j] != expected[i][j]) {
        std::cerr << "res[" << i << "][" << j << "](" << res[i][j]
                  << ") != expected[i][j](" << expected[i][j] << ")" << '\n';
        std::exit(1);
      }
  std::cout << "SUCCESS" << '\n';
}

int main() {
  AP_AXIS matrix;
  const int mat_a[SIZE][SIZE] = {
      {1, 1, 1, 1},
      {1, 1, 1, 1},
      {1, 1, 1, 1},
      {1, 1, 1, 1},
  };
  const int mat_b[SIZE][SIZE] = {
      {3, 1, 2, 2},
      {3, 2, 2, 2},
      {4, 2, 2, 2},
      {4, 2, 3, 2},
  };

  // confirm the multiplication results
  {
    const int mat_ab[SIZE][SIZE] = {
        {14, 7, 9, 8},
        {14, 7, 9, 8},
        {14, 7, 9, 8},
        {14, 7, 9, 8},
    };
    int res_ab[SIZE][SIZE] = {0};
    mat_mul_naive_soft(mat_a, mat_b, res_ab);
    mat_eq(res_ab, mat_ab);
    const int mat_ba[SIZE][SIZE] = {
        {8, 8, 8, 8},
        {9, 9, 9, 9},
        {10, 10, 10, 10},
        {11, 11, 11, 11},
    };

    int res_ba[SIZE][SIZE] = {0};
    mat_mul_naive_soft(mat_b, mat_a, res_ba);
    mat_eq(res_ba, mat_ba);
  }

  // initialize test stream

  AXI_STREAM in_strm("in_strm");
  AXI_STREAM out_strm("out_strm");

  AP_AXIS elem;
  for (int i = 0; i < SIZE; ++i)
    for (int j = 0; j < SIZE; ++j) {
      elem.data = mat_a[i][j];
      in_strm << elem;
    }

  for (int i = 0; i < SIZE; ++i)
    for (int j = 0; j < SIZE; ++j) {
      elem.data = mat_b[i][j];
      in_strm << elem;
    }

  std::cout << "input end\n";

  mat_mul(in_strm, out_strm);

  int res[SIZE][SIZE] = {0};
  AP_AXIS out;
  for (int i = 0; i < SIZE; ++i)
    for (int j = 0; j < SIZE; ++j) {
      out = out_strm.read();
      if (i == SIZE - 1 && j == SIZE - 1 && out.last != 1)
        std::cerr << "last is not one\n";
      res[i][j] = out.data;
    }

  std::cout << "SUCCESS" << '\n';
  return 0;
}
