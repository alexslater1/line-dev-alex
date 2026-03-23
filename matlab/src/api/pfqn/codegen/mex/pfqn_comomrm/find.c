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
#include "pfqn_comomrm_data.h"
#include "pfqn_comomrm_emxutil.h"
#include "pfqn_comomrm_types.h"
#include "rt_nonfinite.h"
#include "warning.h"

/* Variable Definitions */
static emlrtRSInfo rc_emlrtRSI = {
    152,                                                           /* lineNo */
    "eml_find",                                                    /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pathName
                                                                    */
};

static emlrtRSInfo sc_emlrtRSI = {
    396,                                                           /* lineNo */
    "find_first_indices",                                          /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pathName
                                                                    */
};

static emlrtRTEInfo ec_emlrtRTEI = {
    138,                                                           /* lineNo */
    9,                                                             /* colNo */
    "find",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pName */
};

static emlrtRTEInfo fc_emlrtRTEI = {
    363,                                                           /* lineNo */
    24,                                                            /* colNo */
    "find",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pName */
};

/* Function Definitions */
void b_eml_find(const emlrtStack *sp, const emxArray_boolean_T *x,
                emxArray_int32_T *i)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  int32_T idx;
  int32_T ii;
  int32_T nx;
  int32_T *i_data;
  const boolean_T *x_data;
  boolean_T exitg1;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  x_data = x->data;
  nx = x->size[0] * x->size[1];
  st.site = &rb_emlrtRSI;
  idx = 0;
  ii = i->size[0];
  i->size[0] = nx;
  emxEnsureCapacity_int32_T(&st, i, ii, &fc_emlrtRTEI);
  i_data = i->data;
  b_st.site = &sb_emlrtRSI;
  if (nx > 2147483646) {
    c_st.site = &tb_emlrtRSI;
    check_forloop_overflow_error(&c_st);
  }
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii <= nx - 1)) {
    if (x_data[ii]) {
      idx++;
      i_data[idx - 1] = ii + 1;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }
  if (idx > nx) {
    emlrtErrorWithMessageIdR2018a(&st, &g_emlrtRTEI,
                                  "Coder:builtins:AssertionFailed",
                                  "Coder:builtins:AssertionFailed", 0);
  }
  if (nx == 1) {
    if (idx == 0) {
      i->size[0] = 0;
    }
  } else {
    int32_T iv[2];
    if (idx < 1) {
      idx = 0;
    }
    iv[0] = 1;
    iv[1] = idx;
    b_st.site = &sc_emlrtRSI;
    indexShapeCheck(&b_st, i->size[0], iv);
    ii = i->size[0];
    i->size[0] = idx;
    emxEnsureCapacity_int32_T(&st, i, ii, &ec_emlrtRTEI);
  }
  if ((x->size[0] == 1) && (i->size[0] != 1) && (x->size[1] >= 2)) {
    st.site = &rc_emlrtRSI;
    warning(&st);
  }
}

void eml_find(const emlrtStack *sp, const emxArray_boolean_T *x,
              emxArray_int32_T *i)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  int32_T idx;
  int32_T ii;
  int32_T nx;
  int32_T *i_data;
  const boolean_T *x_data;
  boolean_T exitg1;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  x_data = x->data;
  nx = x->size[1];
  st.site = &rb_emlrtRSI;
  idx = 0;
  ii = i->size[0] * i->size[1];
  i->size[0] = 1;
  i->size[1] = x->size[1];
  emxEnsureCapacity_int32_T(&st, i, ii, &dc_emlrtRTEI);
  i_data = i->data;
  b_st.site = &sb_emlrtRSI;
  if (x->size[1] > 2147483646) {
    c_st.site = &tb_emlrtRSI;
    check_forloop_overflow_error(&c_st);
  }
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii <= nx - 1)) {
    if (x_data[ii]) {
      idx++;
      i_data[idx - 1] = ii + 1;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }
  if (idx > x->size[1]) {
    emlrtErrorWithMessageIdR2018a(&st, &g_emlrtRTEI,
                                  "Coder:builtins:AssertionFailed",
                                  "Coder:builtins:AssertionFailed", 0);
  }
  if (x->size[1] == 1) {
    if (idx == 0) {
      i->size[0] = 1;
      i->size[1] = 0;
    }
  } else {
    ii = i->size[0] * i->size[1];
    if (idx < 1) {
      i->size[1] = 0;
    } else {
      i->size[1] = idx;
    }
    emxEnsureCapacity_int32_T(&st, i, ii, &ec_emlrtRTEI);
  }
}

/* End of code generation (find.c) */
