/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * allOrAny.c
 *
 * Code generation for function 'allOrAny'
 *
 */

/* Include files */
#include "allOrAny.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_mmint2_data.h"
#include "pfqn_mmint2_types.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRSInfo jb_emlrtRSI = {
    56,                                                             /* lineNo */
    "@(x)coder.internal.allOrAny(op,x,coder.internal.indexInt(1))", /* fcnName
                                                                     */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/allOrAny.m" /* pathName
                                                                            */
};

static emlrtRSInfo kb_emlrtRSI = {
    143,        /* lineNo */
    "allOrAny", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/allOrAny.m" /* pathName
                                                                            */
};

/* Function Definitions */
boolean_T allOrAny_anonFcn1(const emlrtStack *sp, const emxArray_boolean_T *x)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  int32_T ix;
  const boolean_T *x_data;
  boolean_T exitg1;
  boolean_T varargout_1;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  x_data = x->data;
  st.site = &jb_emlrtRSI;
  varargout_1 = false;
  b_st.site = &kb_emlrtRSI;
  if (x->size[0] > 2147483646) {
    c_st.site = &j_emlrtRSI;
    check_forloop_overflow_error(&c_st);
  }
  ix = 1;
  exitg1 = false;
  while ((!exitg1) && (ix <= x->size[0])) {
    if (x_data[ix - 1]) {
      varargout_1 = true;
      exitg1 = true;
    } else {
      ix++;
    }
  }
  return varargout_1;
}

/* End of code generation (allOrAny.c) */
