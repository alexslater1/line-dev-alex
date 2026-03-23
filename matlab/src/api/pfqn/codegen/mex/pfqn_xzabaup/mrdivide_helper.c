/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * mrdivide_helper.c
 *
 * Code generation for function 'mrdivide_helper'
 *
 */

/* Include files */
#include "mrdivide_helper.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_xzabaup_data.h"
#include "pfqn_xzabaup_emxutil.h"
#include "pfqn_xzabaup_mexutil.h"
#include "pfqn_xzabaup_types.h"
#include "rt_nonfinite.h"
#include "warning.h"
#include "lapacke.h"
#include "mwmathutil.h"
#include <stddef.h>

/* Variable Definitions */
static emlrtRSInfo k_emlrtRSI = {
    61,        /* lineNo */
    "qrsolve", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo l_emlrtRSI = {
    72,        /* lineNo */
    "qrsolve", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo m_emlrtRSI = {
    85,        /* lineNo */
    "qrsolve", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo n_emlrtRSI =
    {
        63,       /* lineNo */
        "xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo o_emlrtRSI =
    {
        138,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo p_emlrtRSI =
    {
        143,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo q_emlrtRSI =
    {
        148,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo r_emlrtRSI = {
    173,          /* lineNo */
    "rankFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo s_emlrtRSI = {
    172,          /* lineNo */
    "rankFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo t_emlrtRSI = {
    106,              /* lineNo */
    "ceval_xunormqr", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xunormqr.m" /* pathName */
};

static emlrtRSInfo u_emlrtRSI = {
    119,         /* lineNo */
    "LSQFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo v_emlrtRSI = {
    128,         /* lineNo */
    "LSQFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo w_emlrtRSI = {
    40,         /* lineNo */
    "xunormqr", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xunormqr.m" /* pathName */
};

static emlrtRSInfo lb_emlrtRSI = {
    44,      /* lineNo */
    "mrdiv", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "mrdivide_helper.m" /* pathName */
};

static emlrtRSInfo mb_emlrtRSI = {
    138,         /* lineNo */
    "LSQFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtMCInfo emlrtMCI = {
    53,        /* lineNo */
    19,        /* colNo */
    "flt2str", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/shared/coder/coder/lib/+coder/+internal/"
    "flt2str.m" /* pName */
};

static emlrtRTEInfo f_emlrtRTEI = {
    45,          /* lineNo */
    13,          /* colNo */
    "infocheck", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "infocheck.m" /* pName */
};

static emlrtRTEInfo g_emlrtRTEI = {
    48,          /* lineNo */
    13,          /* colNo */
    "infocheck", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "infocheck.m" /* pName */
};

static emlrtRTEInfo m_emlrtRTEI = {
    44,                /* lineNo */
    32,                /* colNo */
    "mrdivide_helper", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "mrdivide_helper.m" /* pName */
};

static emlrtRTEInfo n_emlrtRTEI = {
    44,                /* lineNo */
    35,                /* colNo */
    "mrdivide_helper", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "mrdivide_helper.m" /* pName */
};

static emlrtRTEInfo o_emlrtRTEI = {
    119,       /* lineNo */
    5,         /* colNo */
    "qrsolve", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pName
                                                                           */
};

static emlrtRTEInfo p_emlrtRTEI = {
    25,                /* lineNo */
    14,                /* colNo */
    "mrdivide_helper", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "mrdivide_helper.m" /* pName */
};

static emlrtRSInfo nb_emlrtRSI = {
    53,        /* lineNo */
    "flt2str", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/shared/coder/coder/lib/+coder/+internal/"
    "flt2str.m" /* pathName */
};

/* Function Declarations */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               char_T y[14]);

static const mxArray *b_sprintf(const emlrtStack *sp, const mxArray *m,
                                const mxArray *m1, emlrtMCInfo *location);

static void emlrt_marshallIn(const emlrtStack *sp,
                             const mxArray *a__output_of_sprintf_,
                             const char_T *identifier, char_T y[14]);

static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, char_T ret[14]);

/* Function Definitions */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId, char_T y[14])
{
  g_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static const mxArray *b_sprintf(const emlrtStack *sp, const mxArray *m,
                                const mxArray *m1, emlrtMCInfo *location)
{
  const mxArray *pArrays[2];
  const mxArray *m2;
  pArrays[0] = m;
  pArrays[1] = m1;
  return emlrtCallMATLABR2012b((emlrtConstCTX)sp, 1, &m2, 2, &pArrays[0],
                               "sprintf", true, location);
}

static void emlrt_marshallIn(const emlrtStack *sp,
                             const mxArray *a__output_of_sprintf_,
                             const char_T *identifier, char_T y[14])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  b_emlrt_marshallIn(sp, emlrtAlias(a__output_of_sprintf_), &thisId, y);
  emlrtDestroyArray(&a__output_of_sprintf_);
}

static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, char_T ret[14])
{
  static const int32_T dims[2] = {1, 14};
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "char", false, 2U,
                          (const void *)&dims[0]);
  emlrtImportCharArrayR2015b((emlrtConstCTX)sp, src, &ret[0], 14);
  emlrtDestroyArray(&src);
}

real_T mrdiv(const emlrtStack *sp, const emxArray_real_T *A,
             const emxArray_real_T *B)
{
  static const int32_T iv[2] = {1, 6};
  static const char_T b_fname[14] = {'L', 'A', 'P', 'A', 'C', 'K', 'E',
                                     '_', 'd', 'o', 'r', 'm', 'q', 'r'};
  static const char_T fname[14] = {'L', 'A', 'P', 'A', 'C', 'K', 'E',
                                   '_', 'd', 'g', 'e', 'q', 'p', '3'};
  static const char_T rfmt[6] = {'%', '1', '4', '.', '6', 'e'};
  ptrdiff_t jpvt_t;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack st;
  emxArray_real_T *b_A;
  emxArray_real_T *b_B;
  const mxArray *m;
  const mxArray *y;
  const real_T *A_data;
  const real_T *B_data;
  real_T Y;
  real_T tau_data;
  real_T *b_A_data;
  real_T *b_B_data;
  int32_T b_i;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  e_st.prev = &d_st;
  e_st.tls = d_st.tls;
  B_data = B->data;
  A_data = A->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &b_B, 1, &n_emlrtRTEI);
  emxInit_real_T(sp, &b_A, 1, &p_emlrtRTEI);
  if ((A->size[1] == 0) || (B->size[1] == 0)) {
    Y = 0.0;
  } else if (B->size[1] == 1) {
    Y = A_data[0] / B_data[0];
  } else {
    real_T tol;
    int32_T i;
    int32_T loop_ub;
    int32_T rankA;
    boolean_T p;
    st.site = &lb_emlrtRSI;
    rankA = B->size[1];
    i = b_A->size[0];
    b_A->size[0] = B->size[1];
    emxEnsureCapacity_real_T(&st, b_A, i, &m_emlrtRTEI);
    b_A_data = b_A->data;
    for (b_i = 0; b_i < rankA; b_i++) {
      b_A_data[b_i] = B_data[b_i];
    }
    loop_ub = A->size[1];
    i = b_B->size[0];
    b_B->size[0] = A->size[1];
    emxEnsureCapacity_real_T(&st, b_B, i, &n_emlrtRTEI);
    b_B_data = b_B->data;
    for (b_i = 0; b_i < loop_ub; b_i++) {
      b_B_data[b_i] = A_data[b_i];
    }
    b_st.site = &k_emlrtRSI;
    c_st.site = &n_emlrtRSI;
    jpvt_t = (ptrdiff_t)0;
    jpvt_t =
        LAPACKE_dgeqp3(102, (ptrdiff_t)b_A->size[0], (ptrdiff_t)1, &b_A_data[0],
                       (ptrdiff_t)b_A->size[0], &jpvt_t, &tau_data);
    d_st.site = &o_emlrtRSI;
    if ((int32_T)jpvt_t != 0) {
      p = true;
      if ((int32_T)jpvt_t != -4) {
        if ((int32_T)jpvt_t == -1010) {
          emlrtErrorWithMessageIdR2018a(&d_st, &f_emlrtRTEI, "MATLAB:nomem",
                                        "MATLAB:nomem", 0);
        } else {
          emlrtErrorWithMessageIdR2018a(&d_st, &g_emlrtRTEI,
                                        "Coder:toolbox:LAPACKCallErrorInfo",
                                        "Coder:toolbox:LAPACKCallErrorInfo", 5,
                                        4, 14, &fname[0], 12, (int32_T)jpvt_t);
        }
      }
    } else {
      p = false;
    }
    if (p) {
      d_st.site = &p_emlrtRSI;
      if (b_A->size[0] > 2147483646) {
        e_st.site = &j_emlrtRSI;
        check_forloop_overflow_error(&e_st);
      }
      for (b_i = 0; b_i < rankA; b_i++) {
        b_A_data[b_i] = rtNaN;
      }
      d_st.site = &q_emlrtRSI;
      tau_data = rtNaN;
    }
    b_st.site = &l_emlrtRSI;
    rankA = 0;
    Y = muDoubleScalarAbs(b_A_data[0]);
    tol = muDoubleScalarMin(1.4901161193847656E-8,
                            2.2204460492503131E-15 * (real_T)b_A->size[0]) *
          Y;
    if (!(Y <= tol)) {
      rankA = 1;
    } else if (!emlrtSetWarningFlag(&b_st)) {
      char_T str[14];
      c_st.site = &r_emlrtRSI;
      y = NULL;
      m = emlrtCreateCharArray(2, &iv[0]);
      emlrtInitCharArrayR2013a(&c_st, 6, m, &rfmt[0]);
      emlrtAssign(&y, m);
      d_st.site = &nb_emlrtRSI;
      emlrt_marshallIn(&d_st,
                       b_sprintf(&d_st, y, emlrt_marshallOut(tol), &emlrtMCI),
                       "<output of sprintf>", str);
      c_st.site = &s_emlrtRSI;
      warning(&c_st, str);
    }
    b_st.site = &m_emlrtRSI;
    Y = 0.0;
    c_st.site = &u_emlrtRSI;
    d_st.site = &w_emlrtRSI;
    jpvt_t = (ptrdiff_t)b_B->size[0];
    jpvt_t = LAPACKE_dormqr(102, 'L', 'T', jpvt_t, (ptrdiff_t)1, (ptrdiff_t)1,
                            &b_A_data[0], (ptrdiff_t)b_A->size[0], &tau_data,
                            &b_B_data[0], jpvt_t);
    e_st.site = &t_emlrtRSI;
    if ((int32_T)jpvt_t != 0) {
      boolean_T b_p;
      p = true;
      b_p = false;
      if ((int32_T)jpvt_t == -7) {
        b_p = true;
      } else if ((int32_T)jpvt_t == -9) {
        b_p = true;
      } else if ((int32_T)jpvt_t == -10) {
        b_p = true;
      }
      if (!b_p) {
        if ((int32_T)jpvt_t == -1010) {
          emlrtErrorWithMessageIdR2018a(&e_st, &f_emlrtRTEI, "MATLAB:nomem",
                                        "MATLAB:nomem", 0);
        } else {
          emlrtErrorWithMessageIdR2018a(
              &e_st, &g_emlrtRTEI, "Coder:toolbox:LAPACKCallErrorInfo",
              "Coder:toolbox:LAPACKCallErrorInfo", 5, 4, 14, &b_fname[0], 12,
              (int32_T)jpvt_t);
        }
      }
    } else {
      p = false;
    }
    if (p) {
      loop_ub = b_B->size[0];
      i = b_B->size[0];
      b_B->size[0] = loop_ub;
      emxEnsureCapacity_real_T(&d_st, b_B, i, &o_emlrtRTEI);
      b_B_data = b_B->data;
      for (b_i = 0; b_i < loop_ub; b_i++) {
        b_B_data[b_i] = rtNaN;
      }
    }
    c_st.site = &v_emlrtRSI;
    for (b_i = 0; b_i < rankA; b_i++) {
      Y = b_B_data[0];
    }
    for (b_i = rankA; b_i >= 1; b_i--) {
      Y /= b_A_data[0];
      c_st.site = &mb_emlrtRSI;
    }
  }
  emxFree_real_T(sp, &b_A);
  emxFree_real_T(sp, &b_B);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return Y;
}

/* End of code generation (mrdivide_helper.c) */
