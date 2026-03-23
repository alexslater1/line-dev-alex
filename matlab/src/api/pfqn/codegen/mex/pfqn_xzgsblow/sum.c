/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sum.c
 *
 * Code generation for function 'sum'
 *
 */

/* Include files */
#include "sum.h"
#include "pfqn_xzgsblow_types.h"
#include "rt_nonfinite.h"
#include "sumMatrixIncludeNaN.h"

/* Function Definitions */
real_T sum(const emxArray_real_T *x)
{
  emxArray_real_T b_x;
  real_T y;
  int32_T c_x;
  int32_T d_x;
  int32_T e_x;
  int32_T f_x;
  int32_T ib;
  if (x->size[1] == 0) {
    y = 0.0;
  } else if (x->size[1] < 4096) {
    b_x = *x;
    c_x = x->size[1];
    b_x.size = &c_x;
    b_x.numDimensions = 1;
    y = sumColumnB(&b_x, x->size[1]);
  } else {
    int32_T inb;
    int32_T nfb;
    int32_T nleft;
    nfb = (int32_T)((uint32_T)x->size[1] >> 12);
    inb = nfb << 12;
    nleft = x->size[1] - inb;
    b_x = *x;
    d_x = x->size[1];
    b_x.size = &d_x;
    b_x.numDimensions = 1;
    y = sumColumnB4(&b_x, 1);
    for (ib = 2; ib <= nfb; ib++) {
      b_x = *x;
      e_x = x->size[1];
      b_x.size = &e_x;
      b_x.numDimensions = 1;
      y += sumColumnB4(&b_x, ((ib - 1) << 12) + 1);
    }
    if (nleft > 0) {
      b_x = *x;
      f_x = x->size[1];
      b_x.size = &f_x;
      b_x.numDimensions = 1;
      y += b_sumColumnB(&b_x, nleft, inb + 1);
    }
  }
  return y;
}

/* End of code generation (sum.c) */
