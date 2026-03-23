/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * mtimes.c
 *
 * Code generation for function 'mtimes'
 *
 */

/* Include files */
#include "mtimes.h"
#include "pfqn_comom_types.h"
#include "rt_nonfinite.h"
#include "blas.h"
#include <stddef.h>

/* Function Definitions */
real_T mtimes(const emxArray_real_T *A, const emxArray_real_T *B)
{
  ptrdiff_t incx_t;
  ptrdiff_t incy_t;
  ptrdiff_t n_t;
  const real_T *A_data;
  const real_T *B_data;
  real_T C;
  B_data = B->data;
  A_data = A->data;
  if (A->size[1] < 1) {
    C = 0.0;
  } else {
    n_t = (ptrdiff_t)A->size[1];
    incx_t = (ptrdiff_t)1;
    incy_t = (ptrdiff_t)1;
    C = ddot(&n_t, (real_T *)&A_data[0], &incx_t, (real_T *)&B_data[0],
             &incy_t);
  }
  return C;
}

/* End of code generation (mtimes.c) */
