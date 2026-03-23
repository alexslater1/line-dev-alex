/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sumMatrixIncludeNaN.c
 *
 * Code generation for function 'sumMatrixIncludeNaN'
 *
 */

/* Include files */
#include "sumMatrixIncludeNaN.h"
#include "pfqn_xzabalow_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
real_T b_sumColumnB(const emxArray_real_T *x, int32_T vlen, int32_T vstart)
{
  const real_T *x_data;
  real_T y;
  int32_T b_k;
  int32_T k;
  x_data = x->data;
  if (vlen <= 1024) {
    y = x_data[vstart - 1];
    for (k = 0; k <= vlen - 2; k++) {
      y += x_data[vstart + k];
    }
  } else {
    real_T b_y;
    int32_T b_vstart;
    int32_T inb;
    int32_T nfb;
    nfb = (int32_T)((uint32_T)vlen >> 10);
    inb = nfb << 10;
    y = x_data[vstart - 1];
    for (k = 0; k < 1023; k++) {
      y += x_data[vstart + k];
    }
    for (k = 2; k <= nfb; k++) {
      b_vstart = vstart + ((k - 1) << 10);
      b_y = x_data[b_vstart - 1];
      for (b_k = 0; b_k < 1023; b_k++) {
        b_y += x_data[b_vstart + b_k];
      }
      y += b_y;
    }
    if (vlen > inb) {
      nfb = vstart + inb;
      b_y = x_data[nfb - 1];
      b_vstart = vlen - inb;
      for (k = 0; k <= b_vstart - 2; k++) {
        b_y += x_data[nfb + k];
      }
      y += b_y;
    }
  }
  return y;
}

real_T sumColumnB(const emxArray_real_T *x, int32_T vlen)
{
  const real_T *x_data;
  real_T y;
  int32_T b_k;
  int32_T k;
  x_data = x->data;
  if (vlen <= 1024) {
    int32_T vstart;
    y = x_data[0];
    vstart = (uint16_T)(vlen - 1);
    for (k = 0; k < vstart; k++) {
      y += x_data[k + 1];
    }
  } else {
    real_T b_y;
    int32_T inb;
    int32_T nfb;
    int32_T vstart;
    nfb = (int32_T)((uint32_T)vlen >> 10);
    inb = nfb << 10;
    y = x_data[0];
    for (k = 0; k < 1023; k++) {
      y += x_data[k + 1];
    }
    for (k = 2; k <= nfb; k++) {
      vstart = (k - 1) << 10;
      b_y = x_data[vstart];
      for (b_k = 0; b_k < 1023; b_k++) {
        b_y += x_data[(vstart + b_k) + 1];
      }
      y += b_y;
    }
    if (vlen > inb) {
      b_y = x_data[inb];
      vstart = vlen - inb;
      for (k = 0; k <= vstart - 2; k++) {
        b_y += x_data[(inb + k) + 1];
      }
      y += b_y;
    }
  }
  return y;
}

real_T sumColumnB4(const emxArray_real_T *x, int32_T vstart)
{
  const real_T *x_data;
  real_T psum2;
  real_T psum3;
  real_T psum4;
  real_T y;
  int32_T k;
  x_data = x->data;
  y = x_data[vstart - 1];
  psum2 = x_data[vstart + 1023];
  psum3 = x_data[vstart + 2047];
  psum4 = x_data[vstart + 3071];
  for (k = 0; k < 1023; k++) {
    int32_T psum1_tmp;
    psum1_tmp = vstart + k;
    y += x_data[psum1_tmp];
    psum2 += x_data[psum1_tmp + 1024];
    psum3 += x_data[psum1_tmp + 2048];
    psum4 += x_data[psum1_tmp + 3072];
  }
  return (y + psum2) + (psum3 + psum4);
}

/* End of code generation (sumMatrixIncludeNaN.c) */
