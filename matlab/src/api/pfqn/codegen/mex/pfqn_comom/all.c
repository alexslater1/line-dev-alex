/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * all.c
 *
 * Code generation for function 'all'
 *
 */

/* Include files */
#include "all.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_comom_data.h"
#include "pfqn_comom_emxutil.h"
#include "pfqn_comom_types.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRSInfo sb_emlrtRSI = {
    13,                                                         /* lineNo */
    "all",                                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/all.m" /* pathName */
};

static emlrtRSInfo tb_emlrtRSI = {
    143,        /* lineNo */
    "allOrAny", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/allOrAny.m" /* pathName
                                                                            */
};

static emlrtRSInfo ub_emlrtRSI = {
    16,                                                         /* lineNo */
    "all",                                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/all.m" /* pathName */
};

static emlrtRSInfo vb_emlrtRSI = {
    139,        /* lineNo */
    "allOrAny", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/allOrAny.m" /* pathName
                                                                            */
};

static emlrtRTEInfo o_emlrtRTEI = {
    18,                               /* lineNo */
    27,                               /* colNo */
    "eml_int_forloop_overflow_check", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/eml/"
    "eml_int_forloop_overflow_check.m" /* pName */
};

static emlrtRTEInfo wc_emlrtRTEI = {
    16,                                                         /* lineNo */
    5,                                                          /* colNo */
    "all",                                                      /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/all.m" /* pName */
};

/* Function Definitions */
boolean_T all(const emlrtStack *sp, const emxArray_boolean_T *x)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  int32_T ix;
  const boolean_T *x_data;
  boolean_T exitg1;
  boolean_T y;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  x_data = x->data;
  st.site = &sb_emlrtRSI;
  y = true;
  b_st.site = &tb_emlrtRSI;
  if (x->size[1] > 2147483646) {
    c_st.site = &cb_emlrtRSI;
    check_forloop_overflow_error(&c_st);
  }
  ix = 1;
  exitg1 = false;
  while ((!exitg1) && (ix <= x->size[1])) {
    if (!x_data[ix - 1]) {
      y = false;
      exitg1 = true;
    } else {
      ix++;
    }
  }
  return y;
}

void b_all(const emlrtStack *sp, const emxArray_boolean_T *x,
           emxArray_boolean_T *y)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  int32_T i1;
  int32_T i2;
  int32_T j;
  int32_T vstride;
  const boolean_T *x_data;
  boolean_T *y_data;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  x_data = x->data;
  st.site = &ub_emlrtRSI;
  vstride = y->size[0];
  y->size[0] = x->size[0];
  emxEnsureCapacity_boolean_T(&st, y, vstride, &wc_emlrtRTEI);
  y_data = y->data;
  vstride = x->size[0];
  for (j = 0; j < vstride; j++) {
    y_data[j] = true;
  }
  vstride = x->size[0];
  i2 = (x->size[1] - 1) * x->size[0];
  i1 = 1;
  b_st.site = &vb_emlrtRSI;
  if (x->size[0] > 2147483646) {
    c_st.site = &cb_emlrtRSI;
    check_forloop_overflow_error(&c_st);
  }
  for (j = 0; j < vstride; j++) {
    int32_T i1_tmp;
    int32_T ix;
    boolean_T exitg1;
    boolean_T overflow;
    i1_tmp = i1;
    i1++;
    i2++;
    b_st.site = &tb_emlrtRSI;
    if ((vstride == 0) || (i1_tmp > i2)) {
      overflow = false;
    } else {
      overflow = (i2 > MAX_int32_T - vstride);
    }
    if (vstride == 0) {
      emlrtErrorWithMessageIdR2018a(&b_st, &o_emlrtRTEI,
                                    "Coder:builtins:VectorStride",
                                    "Coder:builtins:VectorStride", 0);
    }
    if (overflow) {
      c_st.site = &cb_emlrtRSI;
      check_forloop_overflow_error(&c_st);
    }
    ix = i1_tmp;
    exitg1 = false;
    while ((!exitg1) && (ix <= i2)) {
      if (!x_data[ix - 1]) {
        y_data[i1_tmp - 1] = false;
        exitg1 = true;
      } else {
        ix += vstride;
      }
    }
  }
}

/* End of code generation (all.c) */
