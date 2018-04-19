// See LICENSE for license details.

#include "dataset.h"
#include "util.h"
#include <stddef.h>
#include <math.h>

void matmul(const size_t coreid, const size_t ncores, const size_t lda,  const data_t A[], const data_t B[], data_t C[])
{
  size_t i, j, k;

  for (i = 0; i < lda; i++) {
    for (j = coreid; j < lda; j += ncores) {
      data_t sum = 0;
      for (k = 0; k < lda; k++)
        sum += A[j*lda + k] * B[k*lda + i];
      C[i + j*lda] = sum;
    }
  }
}

void matmul_opt(const size_t coreid, const size_t ncores, const size_t lda,  const data_t A[], const data_t B[], data_t C[])
{
  size_t i, j, k;
  for (j = coreid; j < lda; j += ncores) {
    for (k = 0; k < lda; k++) {
      data_t A_val = A[j*lda + k];
      for (i = 0; i < lda - 3; i+=4) {
        C[i + j*lda] += A_val * B[k*lda + i];
        C[i + j*lda + 1] += A_val * B[k*lda + i + 1];
        C[i + j*lda + 2] += A_val * B[k*lda + i + 2];
        C[i + j*lda + 3] += A_val * B[k*lda + i + 3];
      }
      for (; i < lda; i++)
        C[i + j*lda] += A_val * B[k*lda + i];
    }
  }
}
