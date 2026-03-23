/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sort.c
 *
 * Code generation for function 'sort'
 *
 */

/* Include files */
#include "sort.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_comom_data.h"
#include "pfqn_comom_emxutil.h"
#include "pfqn_comom_types.h"
#include "rt_nonfinite.h"
#include "sortIdx.h"

/* Variable Definitions */
static emlrtRSInfo nf_emlrtRSI = {
    76,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pathName
                                                                        */
};

static emlrtRSInfo of_emlrtRSI = {
    79,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pathName
                                                                        */
};

static emlrtRSInfo pf_emlrtRSI = {
    81,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pathName
                                                                        */
};

static emlrtRSInfo qf_emlrtRSI = {
    84,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pathName
                                                                        */
};

static emlrtRSInfo rf_emlrtRSI = {
    87,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pathName
                                                                        */
};

static emlrtRSInfo sf_emlrtRSI = {
    90,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pathName
                                                                        */
};

static emlrtRTEInfo de_emlrtRTEI = {
    56,     /* lineNo */
    24,     /* colNo */
    "sort", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pName
                                                                        */
};

static emlrtRTEInfo ee_emlrtRTEI = {
    56,     /* lineNo */
    1,      /* colNo */
    "sort", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pName
                                                                        */
};

static emlrtRTEInfo fe_emlrtRTEI = {
    1,      /* lineNo */
    20,     /* colNo */
    "sort", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pName
                                                                        */
};

/* Function Definitions */
void sort(const emlrtStack *sp, emxArray_real_T *x)
{
  emlrtStack b_st;
  emlrtStack st;
  emxArray_int32_T *wg_emlrtRSI;
  emxArray_real_T *vwork;
  real_T *vwork_data;
  real_T *x_data;
  int32_T dim;
  int32_T i;
  int32_T i1;
  int32_T j;
  int32_T k;
  int32_T vstride;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  x_data = x->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  dim = 2;
  if (x->size[0] != 1) {
    dim = 1;
  }
  if (dim <= 1) {
    i = x->size[0];
  } else {
    i = 1;
  }
  emxInit_real_T(sp, &vwork, 1, &ee_emlrtRTEI);
  i1 = vwork->size[0];
  vwork->size[0] = i;
  emxEnsureCapacity_real_T(sp, vwork, i1, &de_emlrtRTEI);
  vwork_data = vwork->data;
  st.site = &nf_emlrtRSI;
  vstride = 1;
  i1 = dim - 2;
  for (k = 0; k <= i1; k++) {
    vstride *= x->size[0];
  }
  st.site = &of_emlrtRSI;
  st.site = &pf_emlrtRSI;
  if (vstride > 2147483646) {
    b_st.site = &cb_emlrtRSI;
    check_forloop_overflow_error(&b_st);
  }
  emxInit_int32_T(sp, &wg_emlrtRSI, 1, &fe_emlrtRTEI);
  for (j = 0; j < vstride; j++) {
    st.site = &qf_emlrtRSI;
    if (i > 2147483646) {
      b_st.site = &cb_emlrtRSI;
      check_forloop_overflow_error(&b_st);
    }
    for (k = 0; k < i; k++) {
      vwork_data[k] = x_data[j + k * vstride];
    }
    st.site = &rf_emlrtRSI;
    sortIdx(&st, vwork, wg_emlrtRSI);
    vwork_data = vwork->data;
    st.site = &sf_emlrtRSI;
    for (k = 0; k < i; k++) {
      x_data[j + k * vstride] = vwork_data[k];
    }
  }
  emxFree_int32_T(sp, &wg_emlrtRSI);
  emxFree_real_T(sp, &vwork);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (sort.c) */
