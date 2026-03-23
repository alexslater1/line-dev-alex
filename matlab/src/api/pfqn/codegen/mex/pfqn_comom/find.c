/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * find.c
 *
 * Code generation for function 'find'
 *
 */

/* Include files */
#include "find.h"
#include "eml_int_forloop_overflow_check.h"
#include "indexShapeCheck.h"
#include "pfqn_comom_data.h"
#include "pfqn_comom_types.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRSInfo xb_emlrtRSI = {
    138,                                                           /* lineNo */
    "eml_find",                                                    /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pathName
                                                                    */
};

static emlrtRSInfo yb_emlrtRSI = {
    396,                                                           /* lineNo */
    "find_first_indices",                                          /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pathName
                                                                    */
};

static emlrtRSInfo ac_emlrtRSI = {
    376,                                                           /* lineNo */
    "find_first_indices",                                          /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pathName
                                                                    */
};

static emlrtRTEInfo p_emlrtRTEI = {
    81,                                                            /* lineNo */
    1,                                                             /* colNo */
    "eml_find",                                                    /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pName */
};

static emlrtRTEInfo q_emlrtRTEI = {
    386,                                                           /* lineNo */
    1,                                                             /* colNo */
    "find_first_indices",                                          /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pName */
};

/* Function Definitions */
int32_T eml_find(const emlrtStack *sp, const emxArray_boolean_T *x,
                 int32_T i_data[])
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  int32_T i_size;
  int32_T idx;
  int32_T ii;
  const boolean_T *x_data;
  boolean_T exitg1;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  x_data = x->data;
  i_size = (x->size[0] >= 1);
  if (i_size > x->size[0]) {
    emlrtErrorWithMessageIdR2018a(sp, &p_emlrtRTEI,
                                  "Coder:builtins:AssertionFailed",
                                  "Coder:builtins:AssertionFailed", 0);
  }
  st.site = &xb_emlrtRSI;
  idx = 0;
  b_st.site = &ac_emlrtRSI;
  if (x->size[0] > 2147483646) {
    c_st.site = &cb_emlrtRSI;
    check_forloop_overflow_error(&c_st);
  }
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii <= x->size[0] - 1)) {
    if (x_data[ii]) {
      idx = 1;
      i_data[0] = ii + 1;
      exitg1 = true;
    } else {
      ii++;
    }
  }
  if (idx > i_size) {
    emlrtErrorWithMessageIdR2018a(&st, &q_emlrtRTEI,
                                  "Coder:builtins:AssertionFailed",
                                  "Coder:builtins:AssertionFailed", 0);
  }
  if (i_size == 1) {
    if (idx == 0) {
      i_size = 0;
    }
  } else {
    b_st.site = &yb_emlrtRSI;
    indexShapeCheck();
    i_size = (idx >= 1);
  }
  return i_size;
}

/* End of code generation (find.c) */
