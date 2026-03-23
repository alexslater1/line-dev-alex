/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * nullAssignment.c
 *
 * Code generation for function 'nullAssignment'
 *
 */

/* Include files */
#include "nullAssignment.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_comomrm_data.h"
#include "pfqn_comomrm_emxutil.h"
#include "pfqn_comomrm_types.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRSInfo hc_emlrtRSI =
    {
        29,               /* lineNo */
        "nullAssignment", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "nullAssignment.m" /* pathName */
};

static emlrtRSInfo ic_emlrtRSI =
    {
        33,               /* lineNo */
        "nullAssignment", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "nullAssignment.m" /* pathName */
};

static emlrtRSInfo jc_emlrtRSI =
    {
        355,              /* lineNo */
        "delete_columns", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "nullAssignment.m" /* pathName */
};

static emlrtRSInfo kc_emlrtRSI =
    {
        356,              /* lineNo */
        "delete_columns", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "nullAssignment.m" /* pathName */
};

static emlrtRSInfo lc_emlrtRSI =
    {
        364,              /* lineNo */
        "delete_columns", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "nullAssignment.m" /* pathName */
};

static emlrtRSInfo mc_emlrtRSI =
    {
        366,              /* lineNo */
        "delete_columns", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "nullAssignment.m" /* pathName */
};

static emlrtRSInfo nc_emlrtRSI =
    {
        369,              /* lineNo */
        "delete_columns", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "nullAssignment.m" /* pathName */
};

static emlrtRSInfo oc_emlrtRSI =
    {
        371,              /* lineNo */
        "delete_columns", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "nullAssignment.m" /* pathName */
};

static emlrtRSInfo pc_emlrtRSI =
    {
        258,             /* lineNo */
        "make_bitarray", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "nullAssignment.m" /* pathName */
};

static emlrtRSInfo qc_emlrtRSI =
    {
        132,        /* lineNo */
        "num_true", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "nullAssignment.m" /* pathName */
};

static emlrtRTEInfo r_emlrtRTEI =
    {
        81,                /* lineNo */
        27,                /* colNo */
        "validate_inputs", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "nullAssignment.m" /* pName */
};

static emlrtRTEInfo s_emlrtRTEI =
    {
        378,              /* lineNo */
        1,                /* colNo */
        "delete_columns", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "nullAssignment.m" /* pName */
};

static emlrtRTEInfo le_emlrtRTEI =
    {
        364,              /* lineNo */
        9,                /* colNo */
        "nullAssignment", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "nullAssignment.m" /* pName */
};

static emlrtRTEInfo me_emlrtRTEI =
    {
        33,               /* lineNo */
        13,               /* colNo */
        "nullAssignment", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "nullAssignment.m" /* pName */
};

static emlrtRTEInfo ne_emlrtRTEI =
    {
        362,              /* lineNo */
        9,                /* colNo */
        "nullAssignment", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "nullAssignment.m" /* pName */
};

/* Function Definitions */
void b_nullAssignment(const emlrtStack *sp, emxArray_real_T *x,
                      const emxArray_int32_T *idx)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack st;
  emxArray_boolean_T *b;
  real_T *x_data;
  const int32_T *idx_data;
  int32_T b_j;
  int32_T j;
  int32_T ncols;
  int32_T ncolx;
  boolean_T exitg1;
  boolean_T p;
  boolean_T *b_data;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  idx_data = idx->data;
  x_data = x->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  st.site = &hc_emlrtRSI;
  p = true;
  j = 0;
  exitg1 = false;
  while ((!exitg1) && (j <= idx->size[1] - 1)) {
    if (idx_data[j] > x->size[1]) {
      p = false;
      exitg1 = true;
    } else {
      j++;
    }
  }
  if (!p) {
    emlrtErrorWithMessageIdR2018a(&st, &r_emlrtRTEI,
                                  "MATLAB:subsdeldimmismatch",
                                  "MATLAB:subsdeldimmismatch", 0);
  }
  st.site = &ic_emlrtRSI;
  ncolx = x->size[1];
  if (idx->size[1] == 1) {
    ncols = x->size[1] - 1;
    j = idx_data[0];
    b_st.site = &jc_emlrtRSI;
    for (b_j = j; b_j <= ncols; b_j++) {
      x_data[b_j - 1] = x_data[b_j];
    }
  } else {
    b_st.site = &lc_emlrtRSI;
    emxInit_boolean_T(&b_st, &b, &ne_emlrtRTEI);
    j = b->size[0] * b->size[1];
    b->size[0] = 1;
    b->size[1] = ncolx;
    emxEnsureCapacity_boolean_T(&b_st, b, j, &le_emlrtRTEI);
    b_data = b->data;
    for (b_j = 0; b_j < ncolx; b_j++) {
      b_data[b_j] = false;
    }
    j = idx->size[1];
    c_st.site = &pc_emlrtRSI;
    if (idx->size[1] > 2147483646) {
      d_st.site = &tb_emlrtRSI;
      check_forloop_overflow_error(&d_st);
    }
    for (b_j = 0; b_j < j; b_j++) {
      b_data[idx_data[b_j] - 1] = true;
    }
    b_st.site = &mc_emlrtRSI;
    j = 0;
    c_st.site = &qc_emlrtRSI;
    if (b->size[1] > 2147483646) {
      d_st.site = &tb_emlrtRSI;
      check_forloop_overflow_error(&d_st);
    }
    for (b_j = 0; b_j < ncolx; b_j++) {
      j += b_data[b_j];
    }
    ncols = x->size[1] - j;
    j = 0;
    b_st.site = &nc_emlrtRSI;
    if (x->size[1] > 2147483646) {
      c_st.site = &tb_emlrtRSI;
      check_forloop_overflow_error(&c_st);
    }
    for (b_j = 0; b_j < ncolx; b_j++) {
      if ((b_j + 1 > b->size[1]) || (!b_data[b_j])) {
        x_data[j] = x_data[b_j];
        j++;
      }
    }
    emxFree_boolean_T(&st, &b);
  }
  if (ncols > x->size[1]) {
    emlrtErrorWithMessageIdR2018a(&st, &s_emlrtRTEI,
                                  "Coder:builtins:AssertionFailed",
                                  "Coder:builtins:AssertionFailed", 0);
  }
  j = x->size[0] * x->size[1];
  if (ncols < 1) {
    x->size[1] = 0;
  } else {
    x->size[1] = ncols;
  }
  emxEnsureCapacity_real_T(&st, x, j, &me_emlrtRTEI);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

void c_nullAssignment(const emlrtStack *sp, emxArray_real_T *x,
                      const emxArray_int32_T *idx)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack st;
  emxArray_boolean_T *b;
  real_T *x_data;
  const int32_T *idx_data;
  int32_T b_j;
  int32_T i;
  int32_T j;
  int32_T ncols;
  int32_T ncolx;
  int32_T nrowx;
  boolean_T exitg1;
  boolean_T p;
  boolean_T *b_data;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  idx_data = idx->data;
  x_data = x->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  st.site = &hc_emlrtRSI;
  p = true;
  j = 0;
  exitg1 = false;
  while ((!exitg1) && (j <= idx->size[0] - 1)) {
    if (idx_data[j] > x->size[1]) {
      p = false;
      exitg1 = true;
    } else {
      j++;
    }
  }
  if (!p) {
    emlrtErrorWithMessageIdR2018a(&st, &r_emlrtRTEI,
                                  "MATLAB:subsdeldimmismatch",
                                  "MATLAB:subsdeldimmismatch", 0);
  }
  st.site = &ic_emlrtRSI;
  nrowx = x->size[0];
  ncolx = x->size[1];
  if (idx->size[0] == 1) {
    ncols = x->size[1] - 1;
    j = idx_data[0];
    b_st.site = &jc_emlrtRSI;
    for (b_j = j; b_j <= ncols; b_j++) {
      b_st.site = &kc_emlrtRSI;
      if (nrowx > 2147483646) {
        c_st.site = &tb_emlrtRSI;
        check_forloop_overflow_error(&c_st);
      }
      for (i = 0; i < nrowx; i++) {
        x_data[i + x->size[0] * (b_j - 1)] = x_data[i + x->size[0] * b_j];
      }
    }
  } else {
    b_st.site = &lc_emlrtRSI;
    emxInit_boolean_T(&b_st, &b, &ne_emlrtRTEI);
    j = b->size[0] * b->size[1];
    b->size[0] = 1;
    b->size[1] = ncolx;
    emxEnsureCapacity_boolean_T(&b_st, b, j, &le_emlrtRTEI);
    b_data = b->data;
    for (b_j = 0; b_j < ncolx; b_j++) {
      b_data[b_j] = false;
    }
    j = idx->size[0];
    c_st.site = &pc_emlrtRSI;
    if (idx->size[0] > 2147483646) {
      d_st.site = &tb_emlrtRSI;
      check_forloop_overflow_error(&d_st);
    }
    for (b_j = 0; b_j < j; b_j++) {
      b_data[idx_data[b_j] - 1] = true;
    }
    b_st.site = &mc_emlrtRSI;
    j = 0;
    c_st.site = &qc_emlrtRSI;
    if (b->size[1] > 2147483646) {
      d_st.site = &tb_emlrtRSI;
      check_forloop_overflow_error(&d_st);
    }
    for (b_j = 0; b_j < ncolx; b_j++) {
      j += b_data[b_j];
    }
    ncols = x->size[1] - j;
    j = 0;
    b_st.site = &nc_emlrtRSI;
    if (x->size[1] > 2147483646) {
      c_st.site = &tb_emlrtRSI;
      check_forloop_overflow_error(&c_st);
    }
    for (b_j = 0; b_j < ncolx; b_j++) {
      if ((b_j + 1 > b->size[1]) || (!b_data[b_j])) {
        b_st.site = &oc_emlrtRSI;
        if (nrowx > 2147483646) {
          c_st.site = &tb_emlrtRSI;
          check_forloop_overflow_error(&c_st);
        }
        for (i = 0; i < nrowx; i++) {
          x_data[i + x->size[0] * j] = x_data[i + x->size[0] * b_j];
        }
        j++;
      }
    }
    emxFree_boolean_T(&st, &b);
  }
  if (ncols > x->size[1]) {
    emlrtErrorWithMessageIdR2018a(&st, &s_emlrtRTEI,
                                  "Coder:builtins:AssertionFailed",
                                  "Coder:builtins:AssertionFailed", 0);
  }
  if (ncols < 1) {
    ncols = 0;
  }
  for (b_j = 0; b_j < ncols; b_j++) {
    for (i = 0; i < nrowx; i++) {
      x_data[i + nrowx * b_j] = x_data[i + x->size[0] * b_j];
    }
  }
  j = x->size[0] * x->size[1];
  x->size[1] = ncols;
  emxEnsureCapacity_real_T(&st, x, j, &me_emlrtRTEI);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

void d_nullAssignment(const emlrtStack *sp, emxArray_real_T *x,
                      const emxArray_int32_T *idx)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack st;
  emxArray_boolean_T *b;
  real_T *x_data;
  const int32_T *idx_data;
  int32_T b_j;
  int32_T j;
  int32_T ncols;
  int32_T ncolx;
  boolean_T exitg1;
  boolean_T p;
  boolean_T *b_data;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  idx_data = idx->data;
  x_data = x->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  st.site = &hc_emlrtRSI;
  p = true;
  j = 0;
  exitg1 = false;
  while ((!exitg1) && (j <= idx->size[0] - 1)) {
    if (idx_data[j] > x->size[1]) {
      p = false;
      exitg1 = true;
    } else {
      j++;
    }
  }
  if (!p) {
    emlrtErrorWithMessageIdR2018a(&st, &r_emlrtRTEI,
                                  "MATLAB:subsdeldimmismatch",
                                  "MATLAB:subsdeldimmismatch", 0);
  }
  st.site = &ic_emlrtRSI;
  ncolx = x->size[1];
  if (idx->size[0] == 1) {
    ncols = x->size[1] - 1;
    j = idx_data[0];
    b_st.site = &jc_emlrtRSI;
    for (b_j = j; b_j <= ncols; b_j++) {
      x_data[b_j - 1] = x_data[b_j];
    }
  } else {
    b_st.site = &lc_emlrtRSI;
    emxInit_boolean_T(&b_st, &b, &ne_emlrtRTEI);
    j = b->size[0] * b->size[1];
    b->size[0] = 1;
    b->size[1] = ncolx;
    emxEnsureCapacity_boolean_T(&b_st, b, j, &le_emlrtRTEI);
    b_data = b->data;
    for (b_j = 0; b_j < ncolx; b_j++) {
      b_data[b_j] = false;
    }
    j = idx->size[0];
    c_st.site = &pc_emlrtRSI;
    if (idx->size[0] > 2147483646) {
      d_st.site = &tb_emlrtRSI;
      check_forloop_overflow_error(&d_st);
    }
    for (b_j = 0; b_j < j; b_j++) {
      b_data[idx_data[b_j] - 1] = true;
    }
    b_st.site = &mc_emlrtRSI;
    j = 0;
    c_st.site = &qc_emlrtRSI;
    if (b->size[1] > 2147483646) {
      d_st.site = &tb_emlrtRSI;
      check_forloop_overflow_error(&d_st);
    }
    for (b_j = 0; b_j < ncolx; b_j++) {
      j += b_data[b_j];
    }
    ncols = x->size[1] - j;
    j = 0;
    b_st.site = &nc_emlrtRSI;
    if (x->size[1] > 2147483646) {
      c_st.site = &tb_emlrtRSI;
      check_forloop_overflow_error(&c_st);
    }
    for (b_j = 0; b_j < ncolx; b_j++) {
      if ((b_j + 1 > b->size[1]) || (!b_data[b_j])) {
        x_data[j] = x_data[b_j];
        j++;
      }
    }
    emxFree_boolean_T(&st, &b);
  }
  if (ncols > x->size[1]) {
    emlrtErrorWithMessageIdR2018a(&st, &s_emlrtRTEI,
                                  "Coder:builtins:AssertionFailed",
                                  "Coder:builtins:AssertionFailed", 0);
  }
  j = x->size[0] * x->size[1];
  if (ncols < 1) {
    x->size[1] = 0;
  } else {
    x->size[1] = ncols;
  }
  emxEnsureCapacity_real_T(&st, x, j, &me_emlrtRTEI);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

void nullAssignment(const emlrtStack *sp, emxArray_real_T *x,
                    const emxArray_int32_T *idx)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack st;
  emxArray_boolean_T *b;
  real_T *x_data;
  const int32_T *idx_data;
  int32_T b_j;
  int32_T i;
  int32_T j;
  int32_T ncols;
  int32_T ncolx;
  int32_T nrowx;
  boolean_T exitg1;
  boolean_T p;
  boolean_T *b_data;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  idx_data = idx->data;
  x_data = x->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  st.site = &hc_emlrtRSI;
  p = true;
  j = 0;
  exitg1 = false;
  while ((!exitg1) && (j <= idx->size[1] - 1)) {
    if (idx_data[j] > x->size[1]) {
      p = false;
      exitg1 = true;
    } else {
      j++;
    }
  }
  if (!p) {
    emlrtErrorWithMessageIdR2018a(&st, &r_emlrtRTEI,
                                  "MATLAB:subsdeldimmismatch",
                                  "MATLAB:subsdeldimmismatch", 0);
  }
  st.site = &ic_emlrtRSI;
  nrowx = x->size[0];
  ncolx = x->size[1];
  if (idx->size[1] == 1) {
    ncols = x->size[1] - 1;
    j = idx_data[0];
    b_st.site = &jc_emlrtRSI;
    for (b_j = j; b_j <= ncols; b_j++) {
      b_st.site = &kc_emlrtRSI;
      if (nrowx > 2147483646) {
        c_st.site = &tb_emlrtRSI;
        check_forloop_overflow_error(&c_st);
      }
      for (i = 0; i < nrowx; i++) {
        x_data[i + x->size[0] * (b_j - 1)] = x_data[i + x->size[0] * b_j];
      }
    }
  } else {
    b_st.site = &lc_emlrtRSI;
    emxInit_boolean_T(&b_st, &b, &ne_emlrtRTEI);
    j = b->size[0] * b->size[1];
    b->size[0] = 1;
    b->size[1] = ncolx;
    emxEnsureCapacity_boolean_T(&b_st, b, j, &le_emlrtRTEI);
    b_data = b->data;
    for (b_j = 0; b_j < ncolx; b_j++) {
      b_data[b_j] = false;
    }
    j = idx->size[1];
    c_st.site = &pc_emlrtRSI;
    if (idx->size[1] > 2147483646) {
      d_st.site = &tb_emlrtRSI;
      check_forloop_overflow_error(&d_st);
    }
    for (b_j = 0; b_j < j; b_j++) {
      b_data[idx_data[b_j] - 1] = true;
    }
    b_st.site = &mc_emlrtRSI;
    j = 0;
    c_st.site = &qc_emlrtRSI;
    if (b->size[1] > 2147483646) {
      d_st.site = &tb_emlrtRSI;
      check_forloop_overflow_error(&d_st);
    }
    for (b_j = 0; b_j < ncolx; b_j++) {
      j += b_data[b_j];
    }
    ncols = x->size[1] - j;
    j = 0;
    b_st.site = &nc_emlrtRSI;
    if (x->size[1] > 2147483646) {
      c_st.site = &tb_emlrtRSI;
      check_forloop_overflow_error(&c_st);
    }
    for (b_j = 0; b_j < ncolx; b_j++) {
      if ((b_j + 1 > b->size[1]) || (!b_data[b_j])) {
        b_st.site = &oc_emlrtRSI;
        if (nrowx > 2147483646) {
          c_st.site = &tb_emlrtRSI;
          check_forloop_overflow_error(&c_st);
        }
        for (i = 0; i < nrowx; i++) {
          x_data[i + x->size[0] * j] = x_data[i + x->size[0] * b_j];
        }
        j++;
      }
    }
    emxFree_boolean_T(&st, &b);
  }
  if (ncols > x->size[1]) {
    emlrtErrorWithMessageIdR2018a(&st, &s_emlrtRTEI,
                                  "Coder:builtins:AssertionFailed",
                                  "Coder:builtins:AssertionFailed", 0);
  }
  if (ncols < 1) {
    ncols = 0;
  }
  for (b_j = 0; b_j < ncols; b_j++) {
    for (i = 0; i < nrowx; i++) {
      x_data[i + nrowx * b_j] = x_data[i + x->size[0] * b_j];
    }
  }
  j = x->size[0] * x->size[1];
  x->size[1] = ncols;
  emxEnsureCapacity_real_T(&st, x, j, &me_emlrtRTEI);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (nullAssignment.c) */
