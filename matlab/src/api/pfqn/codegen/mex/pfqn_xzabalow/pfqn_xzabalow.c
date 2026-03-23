/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_xzabalow.c
 *
 * Code generation for function 'pfqn_xzabalow'
 *
 */

/* Include files */
#include "pfqn_xzabalow.h"
#include "pfqn_xzabalow_types.h"
#include "rt_nonfinite.h"
#include "sumMatrixIncludeNaN.h"

/* Function Definitions */
real_T pfqn_xzabalow(const emlrtStack *sp, const emxArray_real_T *L, real_T N,
                     real_T Z)
{
  emxArray_real_T b_L;
  real_T XN;
  int32_T c_L;
  int32_T d_L;
  int32_T e_L;
  int32_T f_L;
  int32_T ib;
  (void)sp;
  /* { */
  /*  % @file pfqn_xzabalow.m */
  /*  % @brief Lower asymptotic bound on throughput (Zahorjan-Balanced). */
  /* } */
  /* { */
  /*  % @brief Lower asymptotic bound on throughput (Zahorjan-Balanced). */
  /*  % @fn pfqn_xzabalow(L, N, Z) */
  /*  % @param L Service demand vector. */
  /*  % @param N Population. */
  /*  % @param Z Think time. */
  /*  % @return XN Lower bound on throughput. */
  /* } */
  if (L->size[1] == 0) {
    XN = 0.0;
  } else if (L->size[1] < 4096) {
    b_L = *L;
    c_L = L->size[1];
    b_L.size = &c_L;
    b_L.numDimensions = 1;
    XN = sumColumnB(&b_L, L->size[1]);
  } else {
    int32_T inb;
    int32_T nfb;
    int32_T nleft;
    nfb = (int32_T)((uint32_T)L->size[1] >> 12);
    inb = nfb << 12;
    nleft = L->size[1] - inb;
    b_L = *L;
    d_L = L->size[1];
    b_L.size = &d_L;
    b_L.numDimensions = 1;
    XN = sumColumnB4(&b_L, 1);
    for (ib = 2; ib <= nfb; ib++) {
      b_L = *L;
      e_L = L->size[1];
      b_L.size = &e_L;
      b_L.numDimensions = 1;
      XN += sumColumnB4(&b_L, ((ib - 1) << 12) + 1);
    }
    if (nleft > 0) {
      b_L = *L;
      f_L = L->size[1];
      b_L.size = &f_L;
      b_L.numDimensions = 1;
      XN += b_sumColumnB(&b_L, nleft, inb + 1);
    }
  }
  return N / (Z + XN * N);
}

/* End of code generation (pfqn_xzabalow.c) */
