/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * det.c
 *
 * Code generation for function 'det'
 *
 */

/* Include files */
#include "det.h"
#include "pfqn_le_emxutil.h"
#include "pfqn_le_types.h"
#include "rt_nonfinite.h"
#include "lapacke.h"
#include "mwmathutil.h"
#include <stddef.h>

/* Variable Definitions */
static emlrtRSInfo cc_emlrtRSI = {
    21,                                                            /* lineNo */
    "det",                                                         /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/matfun/det.m" /* pathName
                                                                    */
};

static emlrtRSInfo dc_emlrtRSI =
    {
        27,       /* lineNo */
        "xgetrf", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgetrf.m" /* pathName */
};

static emlrtRSInfo ec_emlrtRSI =
    {
        91,             /* lineNo */
        "ceval_xgetrf", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgetrf.m" /* pathName */
};

static emlrtRSInfo fc_emlrtRSI =
    {
        58,             /* lineNo */
        "ceval_xgetrf", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgetrf.m" /* pathName */
};

static emlrtRTEInfo j_emlrtRTEI = {
    48,          /* lineNo */
    13,          /* colNo */
    "infocheck", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "infocheck.m" /* pName */
};

static emlrtRTEInfo k_emlrtRTEI = {
    45,          /* lineNo */
    13,          /* colNo */
    "infocheck", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "infocheck.m" /* pName */
};

static emlrtRTEInfo l_emlrtRTEI = {
    12,                                                            /* lineNo */
    15,                                                            /* colNo */
    "det",                                                         /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/matfun/det.m" /* pName */
};

static emlrtRTEInfo bc_emlrtRTEI =
    {
        1,        /* lineNo */
        37,       /* colNo */
        "xgetrf", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgetrf.m" /* pName */
};

static emlrtRTEInfo cc_emlrtRTEI =
    {
        58,       /* lineNo */
        29,       /* colNo */
        "xgetrf", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgetrf.m" /* pName */
};

static emlrtRTEInfo dc_emlrtRTEI =
    {
        89,       /* lineNo */
        27,       /* colNo */
        "xgetrf", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgetrf.m" /* pName */
};

static emlrtRTEInfo ec_emlrtRTEI = {
    1,                                                             /* lineNo */
    14,                                                            /* colNo */
    "det",                                                         /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/matfun/det.m" /* pName */
};

static emlrtRTEInfo fc_emlrtRTEI =
    {
        58,       /* lineNo */
        5,        /* colNo */
        "xgetrf", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgetrf.m" /* pName */
};

/* Function Definitions */
real_T det(const emlrtStack *sp, const emxArray_real_T *x)
{
  static const char_T fname[19] = {'L', 'A', 'P', 'A', 'C', 'K', 'E',
                                   '_', 'd', 'g', 'e', 't', 'r', 'f',
                                   '_', 'w', 'o', 'r', 'k'};
  ptrdiff_t *ipiv_t_data;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  emxArray_int32_T *ipiv;
  emxArray_ptrdiff_t *ipiv_t;
  emxArray_real_T *b_x;
  const real_T *x_data;
  real_T y;
  real_T *b_x_data;
  int32_T k;
  int32_T *ipiv_data;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  x_data = x->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  if (x->size[0] != x->size[1]) {
    emlrtErrorWithMessageIdR2018a(sp, &l_emlrtRTEI, "Coder:MATLAB:square",
                                  "Coder:MATLAB:square", 0);
  }
  if ((x->size[0] == 0) || (x->size[1] == 0)) {
    y = 1.0;
  } else {
    ptrdiff_t info_t;
    int32_T loop_ub;
    int32_T m;
    int32_T n;
    boolean_T isodd;
    m = x->size[0];
    n = x->size[1];
    st.site = &cc_emlrtRSI;
    emxInit_real_T(&st, &b_x, 2, &ec_emlrtRTEI);
    loop_ub = b_x->size[0] * b_x->size[1];
    b_x->size[0] = x->size[0];
    b_x->size[1] = x->size[1];
    emxEnsureCapacity_real_T(&st, b_x, loop_ub, &bc_emlrtRTEI);
    b_x_data = b_x->data;
    loop_ub = x->size[0] * x->size[1];
    for (k = 0; k < loop_ub; k++) {
      b_x_data[k] = x_data[k];
    }
    b_st.site = &dc_emlrtRSI;
    c_st.site = &fc_emlrtRSI;
    emxInit_ptrdiff_t(&c_st, &ipiv_t, &fc_emlrtRTEI);
    loop_ub = ipiv_t->size[0];
    ipiv_t->size[0] = muIntScalarMin_sint32(m, n);
    emxEnsureCapacity_ptrdiff_t(&c_st, ipiv_t, loop_ub, &cc_emlrtRTEI);
    ipiv_t_data = ipiv_t->data;
    info_t = LAPACKE_dgetrf_work(102, (ptrdiff_t)x->size[0],
                                 (ptrdiff_t)x->size[1], &b_x_data[0],
                                 (ptrdiff_t)x->size[0], &ipiv_t_data[0]);
    emxInit_int32_T(&b_st, &ipiv, 2, &ec_emlrtRTEI);
    loop_ub = ipiv->size[0] * ipiv->size[1];
    ipiv->size[0] = 1;
    m = ipiv_t->size[0];
    ipiv->size[1] = ipiv_t->size[0];
    emxEnsureCapacity_int32_T(&b_st, ipiv, loop_ub, &dc_emlrtRTEI);
    ipiv_data = ipiv->data;
    c_st.site = &ec_emlrtRSI;
    if ((int32_T)info_t < 0) {
      if ((int32_T)info_t == -1010) {
        emlrtErrorWithMessageIdR2018a(&c_st, &k_emlrtRTEI, "MATLAB:nomem",
                                      "MATLAB:nomem", 0);
      } else {
        emlrtErrorWithMessageIdR2018a(&c_st, &j_emlrtRTEI,
                                      "Coder:toolbox:LAPACKCallErrorInfo",
                                      "Coder:toolbox:LAPACKCallErrorInfo", 5, 4,
                                      19, &fname[0], 12, (int32_T)info_t);
      }
    }
    for (k = 0; k < m; k++) {
      ipiv_data[k] = (int32_T)ipiv_t_data[k];
    }
    emxFree_ptrdiff_t(&b_st, &ipiv_t);
    y = b_x_data[0];
    loop_ub = b_x->size[0];
    for (k = 0; k <= loop_ub - 2; k++) {
      y *= b_x_data[(k + b_x->size[0] * (k + 1)) + 1];
    }
    emxFree_real_T(sp, &b_x);
    isodd = false;
    for (k = 0; k <= m - 2; k++) {
      if (ipiv_data[k] > k + 1) {
        isodd = !isodd;
      }
    }
    emxFree_int32_T(sp, &ipiv);
    if (isodd) {
      y = -y;
    }
  }
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return y;
}

/* End of code generation (det.c) */
