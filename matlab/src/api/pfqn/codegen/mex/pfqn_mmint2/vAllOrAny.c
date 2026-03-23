/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * vAllOrAny.c
 *
 * Code generation for function 'vAllOrAny'
 *
 */

/* Include files */
#include "vAllOrAny.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_mmint2_data.h"
#include "pfqn_mmint2_types.h"
#include "rt_nonfinite.h"
#include "mwmathutil.h"

/* Function Definitions */
boolean_T vAllOrAny(const emlrtStack *sp, const emxArray_real_T *x)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  const real_T *x_data;
  int32_T k;
  int32_T nx;
  boolean_T p;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  x_data = x->data;
  st.site = &cb_emlrtRSI;
  nx = x->size[1];
  p = false;
  b_st.site = &db_emlrtRSI;
  if (x->size[1] > 2147483646) {
    c_st.site = &j_emlrtRSI;
    check_forloop_overflow_error(&c_st);
  }
  for (k = 0; k < nx; k++) {
    if (p) {
      p = true;
    } else {
      real_T b;
      b = x_data[k];
      if ((!muDoubleScalarIsNaN(b)) && (muDoubleScalarFloor(b) != b)) {
        p = true;
      }
    }
  }
  return p;
}

/* End of code generation (vAllOrAny.c) */
