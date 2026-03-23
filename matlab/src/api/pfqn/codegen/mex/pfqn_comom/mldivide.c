/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * mldivide.c
 *
 * Code generation for function 'mldivide'
 *
 */

/* Include files */
#include "mldivide.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_comom_data.h"
#include "pfqn_comom_emxutil.h"
#include "pfqn_comom_mexutil.h"
#include "pfqn_comom_types.h"
#include "rt_nonfinite.h"
#include "warning.h"
#include "lapacke.h"
#include "mwmathutil.h"
#include <emmintrin.h>
#include <stddef.h>

/* Variable Definitions */
static emlrtRSInfo ie_emlrtRSI = {
    20,         /* lineNo */
    "mldivide", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/mldivide.m" /* pathName
                                                                      */
};

static emlrtRSInfo je_emlrtRSI = {
    42,      /* lineNo */
    "mldiv", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/mldivide.m" /* pathName
                                                                      */
};

static emlrtRSInfo ke_emlrtRSI = {
    44,      /* lineNo */
    "mldiv", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/mldivide.m" /* pathName
                                                                      */
};

static emlrtRSInfo le_emlrtRSI = {
    67,        /* lineNo */
    "lusolve", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/lusolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo me_emlrtRSI = {
    109,          /* lineNo */
    "lusolveNxN", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/lusolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo ne_emlrtRSI = {
    112,          /* lineNo */
    "lusolveNxN", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/lusolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo oe_emlrtRSI = {
    124,          /* lineNo */
    "InvAtimesX", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/lusolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo pe_emlrtRSI = {
    19,        /* lineNo */
    "xgetrfs", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xgetrfs.m" /* pathName */
};

static emlrtRSInfo qe_emlrtRSI = {
    108,      /* lineNo */
    "cmldiv", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xgetrfs.m" /* pathName */
};

static emlrtRSInfo re_emlrtRSI = {
    70,       /* lineNo */
    "cmldiv", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xgetrfs.m" /* pathName */
};

static emlrtRSInfo se_emlrtRSI = {
    90,              /* lineNo */
    "warn_singular", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/lusolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo te_emlrtRSI = {
    61,        /* lineNo */
    "qrsolve", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo ue_emlrtRSI = {
    72,        /* lineNo */
    "qrsolve", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo ve_emlrtRSI = {
    85,        /* lineNo */
    "qrsolve", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo we_emlrtRSI =
    {
        63,       /* lineNo */
        "xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo xe_emlrtRSI =
    {
        138,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo ye_emlrtRSI =
    {
        141,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo af_emlrtRSI =
    {
        143,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo bf_emlrtRSI =
    {
        148,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo cf_emlrtRSI =
    {
        151,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo df_emlrtRSI =
    {
        154,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo ef_emlrtRSI =
    {
        158,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo ff_emlrtRSI = {
    173,          /* lineNo */
    "rankFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo gf_emlrtRSI = {
    172,          /* lineNo */
    "rankFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo hf_emlrtRSI = {
    119,         /* lineNo */
    "LSQFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo if_emlrtRSI = {
    128,         /* lineNo */
    "LSQFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo jf_emlrtRSI = {
    138,         /* lineNo */
    "LSQFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo kf_emlrtRSI = {
    40,         /* lineNo */
    "xunormqr", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xunormqr.m" /* pathName */
};

static emlrtRSInfo lf_emlrtRSI = {
    106,              /* lineNo */
    "ceval_xunormqr", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xunormqr.m" /* pathName */
};

static emlrtRTEInfo bb_emlrtRTEI = {
    48,          /* lineNo */
    13,          /* colNo */
    "infocheck", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "infocheck.m" /* pName */
};

static emlrtRTEInfo cb_emlrtRTEI = {
    45,          /* lineNo */
    13,          /* colNo */
    "infocheck", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "infocheck.m" /* pName */
};

static emlrtRTEInfo db_emlrtRTEI = {
    16,         /* lineNo */
    19,         /* colNo */
    "mldivide", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/mldivide.m" /* pName */
};

static emlrtRTEInfo rd_emlrtRTEI = {
    20,         /* lineNo */
    5,          /* colNo */
    "mldivide", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/mldivide.m" /* pName */
};

static emlrtRTEInfo sd_emlrtRTEI =
    {
        1,        /* lineNo */
        32,       /* colNo */
        "xgeqp3", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pName */
};

static emlrtRTEInfo td_emlrtRTEI =
    {
        61,       /* lineNo */
        9,        /* colNo */
        "xgeqp3", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pName */
};

static emlrtRTEInfo ud_emlrtRTEI = {
    48,        /* lineNo */
    37,        /* colNo */
    "xgetrfs", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xgetrfs.m" /* pName */
};

static emlrtRTEInfo vd_emlrtRTEI =
    {
        92,       /* lineNo */
        22,       /* colNo */
        "xgeqp3", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pName */
};

static emlrtRTEInfo wd_emlrtRTEI =
    {
        105,      /* lineNo */
        1,        /* colNo */
        "xgeqp3", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pName */
};

static emlrtRTEInfo xd_emlrtRTEI = {
    70,        /* lineNo */
    23,        /* colNo */
    "xgetrfs", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xgetrfs.m" /* pName */
};

static emlrtRTEInfo yd_emlrtRTEI = {
    85,        /* lineNo */
    26,        /* colNo */
    "qrsolve", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pName
                                                                           */
};

static emlrtRTEInfo ae_emlrtRTEI = {
    119,       /* lineNo */
    5,         /* colNo */
    "qrsolve", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pName
                                                                           */
};

static emlrtRTEInfo be_emlrtRTEI = {
    1,          /* lineNo */
    14,         /* colNo */
    "mldivide", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/mldivide.m" /* pName */
};

/* Function Declarations */
static void c_emlrt_marshallIn(const emlrtStack *sp,
                               const mxArray *a__output_of_sprintf_,
                               const char_T *identifier, char_T y[14]);

static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               char_T y[14]);

static void l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, char_T ret[14]);

/* Function Definitions */
static void c_emlrt_marshallIn(const emlrtStack *sp,
                               const mxArray *a__output_of_sprintf_,
                               const char_T *identifier, char_T y[14])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  d_emlrt_marshallIn(sp, emlrtAlias(a__output_of_sprintf_), &thisId, y);
  emlrtDestroyArray(&a__output_of_sprintf_);
}

static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId, char_T y[14])
{
  l_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static void l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, char_T ret[14])
{
  static const int32_T dims[2] = {1, 14};
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "char", false, 2U,
                          (const void *)&dims[0]);
  emlrtImportCharArrayR2015b((emlrtConstCTX)sp, src, &ret[0], 14);
  emlrtDestroyArray(&src);
}

void mldivide(const emlrtStack *sp, const emxArray_real_T *A,
              const emxArray_real_T *B, emxArray_real_T *Y)
{
  static const int32_T offsets[4] = {0, 1, 2, 3};
  static const int32_T iv[2] = {1, 6};
  static const char_T b_fname[19] = {'L', 'A', 'P', 'A', 'C', 'K', 'E',
                                     '_', 'd', 'g', 'e', 't', 'r', 'f',
                                     '_', 'w', 'o', 'r', 'k'};
  static const char_T c_fname[14] = {'L', 'A', 'P', 'A', 'C', 'K', 'E',
                                     '_', 'd', 'o', 'r', 'm', 'q', 'r'};
  static const char_T fname[14] = {'L', 'A', 'P', 'A', 'C', 'K', 'E',
                                   '_', 'd', 'g', 'e', 'q', 'p', '3'};
  static const char_T rfmt[6] = {'%', '1', '4', '.', '6', 'e'};
  ptrdiff_t *jpvt_t_data;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack g_st;
  emlrtStack st;
  emxArray_int32_T *jpvt;
  emxArray_ptrdiff_t *jpvt_t;
  emxArray_real_T *b_A;
  emxArray_real_T *b_B;
  emxArray_real_T *tau;
  const mxArray *m;
  const mxArray *y;
  const real_T *A_data;
  const real_T *B_data;
  real_T *Y_data;
  real_T *b_A_data;
  real_T *b_B_data;
  real_T *tau_data;
  int32_T i;
  int32_T j;
  int32_T *jpvt_data;
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
  f_st.prev = &e_st;
  f_st.tls = e_st.tls;
  g_st.prev = &f_st;
  g_st.tls = f_st.tls;
  B_data = B->data;
  A_data = A->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  if (B->size[0] != A->size[0]) {
    emlrtErrorWithMessageIdR2018a(sp, &db_emlrtRTEI, "MATLAB:dimagree",
                                  "MATLAB:dimagree", 0);
  }
  st.site = &ie_emlrtRSI;
  emxInit_real_T(&st, &b_A, 2, &be_emlrtRTEI);
  emxInit_int32_T(&st, &jpvt, 2, &be_emlrtRTEI);
  emxInit_real_T(&st, &tau, 1, &be_emlrtRTEI);
  emxInit_ptrdiff_t(&st, &jpvt_t, &wd_emlrtRTEI);
  emxInit_real_T(&st, &b_B, 1, &yd_emlrtRTEI);
  if ((A->size[0] == 0) || (A->size[1] == 0) || (B->size[0] == 0)) {
    int32_T ma;
    int32_T na;
    na = A->size[1];
    ma = Y->size[0];
    Y->size[0] = A->size[1];
    emxEnsureCapacity_real_T(&st, Y, ma, &rd_emlrtRTEI);
    Y_data = Y->data;
    for (j = 0; j < na; j++) {
      Y_data[j] = 0.0;
    }
  } else if (A->size[0] == A->size[1]) {
    ptrdiff_t INFO;
    ptrdiff_t LDA;
    ptrdiff_t N;
    int32_T ma;
    int32_T na;
    int32_T rankA;
    b_st.site = &je_emlrtRSI;
    c_st.site = &le_emlrtRSI;
    rankA = B->size[0];
    ma = Y->size[0];
    Y->size[0] = B->size[0];
    emxEnsureCapacity_real_T(&c_st, Y, ma, &rd_emlrtRTEI);
    Y_data = Y->data;
    for (j = 0; j < rankA; j++) {
      Y_data[j] = B_data[j];
    }
    d_st.site = &me_emlrtRSI;
    e_st.site = &oe_emlrtRSI;
    ma = A->size[0];
    na = A->size[1];
    ma = muIntScalarMin_sint32(ma, na);
    na = muIntScalarMin_sint32(rankA, ma);
    f_st.site = &pe_emlrtRSI;
    ma = b_A->size[0] * b_A->size[1];
    b_A->size[0] = A->size[0];
    b_A->size[1] = A->size[1];
    emxEnsureCapacity_real_T(&f_st, b_A, ma, &ud_emlrtRTEI);
    b_A_data = b_A->data;
    ma = A->size[0] * A->size[1];
    for (j = 0; j < ma; j++) {
      b_A_data[j] = A_data[j];
    }
    g_st.site = &re_emlrtRSI;
    ma = jpvt_t->size[0];
    jpvt_t->size[0] = na;
    emxEnsureCapacity_ptrdiff_t(&g_st, jpvt_t, ma, &xd_emlrtRTEI);
    jpvt_t_data = jpvt_t->data;
    N = (ptrdiff_t)na;
    LDA = (ptrdiff_t)b_A->size[0];
    INFO = LAPACKE_dgetrf_work(102, N, N, &b_A_data[0], LDA, &jpvt_t_data[0]);
    g_st.site = &qe_emlrtRSI;
    if ((int32_T)INFO < 0) {
      if ((int32_T)INFO == -1010) {
        emlrtErrorWithMessageIdR2018a(&g_st, &cb_emlrtRTEI, "MATLAB:nomem",
                                      "MATLAB:nomem", 0);
      } else {
        emlrtErrorWithMessageIdR2018a(&g_st, &bb_emlrtRTEI,
                                      "Coder:toolbox:LAPACKCallErrorInfo",
                                      "Coder:toolbox:LAPACKCallErrorInfo", 5, 4,
                                      19, &b_fname[0], 12, (int32_T)INFO);
      }
    }
    LAPACKE_dgetrs_work(102, 'N', N, (ptrdiff_t)1, &b_A_data[0], LDA,
                        &jpvt_t_data[0], &Y_data[0], (ptrdiff_t)B->size[0]);
    if (((A->size[0] != 1) || (A->size[1] != 1)) && ((int32_T)INFO > 0)) {
      d_st.site = &ne_emlrtRSI;
      if (!emlrtSetWarningFlag(&d_st)) {
        e_st.site = &se_emlrtRSI;
        b_warning(&e_st);
      }
    }
  } else {
    ptrdiff_t N;
    real_T tol;
    int32_T loop_ub;
    int32_T ma;
    int32_T na;
    int32_T rankA;
    boolean_T p;
    b_st.site = &ke_emlrtRSI;
    c_st.site = &te_emlrtRSI;
    rankA = A->size[0];
    ma = b_A->size[0] * b_A->size[1];
    b_A->size[0] = A->size[0];
    loop_ub = A->size[1];
    b_A->size[1] = A->size[1];
    emxEnsureCapacity_real_T(&c_st, b_A, ma, &sd_emlrtRTEI);
    b_A_data = b_A->data;
    ma = A->size[0] * A->size[1];
    for (j = 0; j < ma; j++) {
      b_A_data[j] = A_data[j];
    }
    ma = jpvt->size[0] * jpvt->size[1];
    jpvt->size[0] = 1;
    jpvt->size[1] = A->size[1];
    emxEnsureCapacity_int32_T(&c_st, jpvt, ma, &td_emlrtRTEI);
    jpvt_data = jpvt->data;
    for (j = 0; j < loop_ub; j++) {
      jpvt_data[j] = 0;
    }
    d_st.site = &we_emlrtRSI;
    na = muIntScalarMin_sint32(rankA, loop_ub);
    ma = tau->size[0];
    tau->size[0] = na;
    emxEnsureCapacity_real_T(&d_st, tau, ma, &vd_emlrtRTEI);
    tau_data = tau->data;
    ma = jpvt_t->size[0];
    jpvt_t->size[0] = A->size[1];
    emxEnsureCapacity_ptrdiff_t(&d_st, jpvt_t, ma, &wd_emlrtRTEI);
    jpvt_t_data = jpvt_t->data;
    for (j = 0; j < loop_ub; j++) {
      jpvt_t_data[j] = (ptrdiff_t)0;
    }
    N = LAPACKE_dgeqp3(102, (ptrdiff_t)b_A->size[0], (ptrdiff_t)b_A->size[1],
                       &b_A_data[0], (ptrdiff_t)b_A->size[0], &jpvt_t_data[0],
                       &tau_data[0]);
    e_st.site = &xe_emlrtRSI;
    if ((int32_T)N != 0) {
      p = true;
      if ((int32_T)N != -4) {
        if ((int32_T)N == -1010) {
          emlrtErrorWithMessageIdR2018a(&e_st, &cb_emlrtRTEI, "MATLAB:nomem",
                                        "MATLAB:nomem", 0);
        } else {
          emlrtErrorWithMessageIdR2018a(&e_st, &bb_emlrtRTEI,
                                        "Coder:toolbox:LAPACKCallErrorInfo",
                                        "Coder:toolbox:LAPACKCallErrorInfo", 5,
                                        4, 14, &fname[0], 12, (int32_T)N);
        }
      }
    } else {
      p = false;
    }
    if (p) {
      e_st.site = &ye_emlrtRSI;
      if (b_A->size[1] > 2147483646) {
        f_st.site = &cb_emlrtRSI;
        check_forloop_overflow_error(&f_st);
      }
      for (j = 0; j < loop_ub; j++) {
        e_st.site = &af_emlrtRSI;
        if (rankA > 2147483646) {
          f_st.site = &cb_emlrtRSI;
          check_forloop_overflow_error(&f_st);
        }
        for (i = 0; i < rankA; i++) {
          b_A_data[j * rankA + i] = rtNaN;
        }
      }
      e_st.site = &bf_emlrtRSI;
      if (na > 2147483646) {
        f_st.site = &cb_emlrtRSI;
        check_forloop_overflow_error(&f_st);
      }
      for (j = 0; j < na; j++) {
        tau_data[j] = rtNaN;
      }
      ma = na + 1;
      e_st.site = &cf_emlrtRSI;
      for (j = ma; j <= na; j++) {
        tau_data[j - 1] = 0.0;
      }
      e_st.site = &df_emlrtRSI;
      ma = (b_A->size[1] / 4) << 2;
      na = ma - 4;
      for (j = 0; j <= na; j += 4) {
        _mm_storeu_si128(
            (__m128i *)&jpvt_data[j],
            _mm_add_epi32(
                _mm_add_epi32(_mm_set1_epi32(j),
                              _mm_loadu_si128((const __m128i *)&offsets[0])),
                _mm_set1_epi32(1)));
      }
      for (j = ma; j < loop_ub; j++) {
        jpvt_data[j] = j + 1;
      }
    } else {
      e_st.site = &ef_emlrtRSI;
      if (b_A->size[1] > 2147483646) {
        f_st.site = &cb_emlrtRSI;
        check_forloop_overflow_error(&f_st);
      }
      for (j = 0; j < loop_ub; j++) {
        jpvt_data[j] = (int32_T)jpvt_t_data[j];
      }
    }
    c_st.site = &ue_emlrtRSI;
    rankA = 0;
    if (b_A->size[0] < b_A->size[1]) {
      ma = b_A->size[0];
      na = b_A->size[1];
    } else {
      ma = b_A->size[1];
      na = b_A->size[0];
    }
    tol = muDoubleScalarMin(1.4901161193847656E-8,
                            2.2204460492503131E-15 * (real_T)na) *
          muDoubleScalarAbs(b_A_data[0]);
    while (
        (rankA < ma) &&
        (!(muDoubleScalarAbs(b_A_data[rankA + b_A->size[0] * rankA]) <= tol))) {
      rankA++;
    }
    if ((rankA < ma) && (!emlrtSetWarningFlag(&c_st))) {
      char_T str[14];
      d_st.site = &ff_emlrtRSI;
      y = NULL;
      m = emlrtCreateCharArray(2, &iv[0]);
      emlrtInitCharArrayR2013a(&d_st, 6, m, &rfmt[0]);
      emlrtAssign(&y, m);
      e_st.site = &qg_emlrtRSI;
      c_emlrt_marshallIn(&e_st,
                         b_sprintf(&e_st, y, emlrt_marshallOut(tol), &emlrtMCI),
                         "<output of sprintf>", str);
      d_st.site = &gf_emlrtRSI;
      c_warning(&d_st, rankA, str);
    }
    c_st.site = &ve_emlrtRSI;
    na = B->size[0];
    ma = b_B->size[0];
    b_B->size[0] = B->size[0];
    emxEnsureCapacity_real_T(&c_st, b_B, ma, &yd_emlrtRTEI);
    b_B_data = b_B->data;
    for (j = 0; j < na; j++) {
      b_B_data[j] = B_data[j];
    }
    na = b_A->size[1];
    ma = Y->size[0];
    Y->size[0] = b_A->size[1];
    emxEnsureCapacity_real_T(&c_st, Y, ma, &rd_emlrtRTEI);
    Y_data = Y->data;
    for (j = 0; j < na; j++) {
      Y_data[j] = 0.0;
    }
    d_st.site = &hf_emlrtRSI;
    e_st.site = &kf_emlrtRSI;
    N = (ptrdiff_t)b_B->size[0];
    N = LAPACKE_dormqr(
        102, 'L', 'T', N, (ptrdiff_t)1,
        (ptrdiff_t)muIntScalarMin_sint32(b_A->size[0], b_A->size[1]),
        &b_A_data[0], (ptrdiff_t)b_A->size[0], &tau_data[0], &b_B_data[0], N);
    f_st.site = &lf_emlrtRSI;
    if ((int32_T)N != 0) {
      boolean_T b_p;
      p = true;
      b_p = false;
      if ((int32_T)N == -7) {
        b_p = true;
      } else if ((int32_T)N == -9) {
        b_p = true;
      } else if ((int32_T)N == -10) {
        b_p = true;
      }
      if (!b_p) {
        if ((int32_T)N == -1010) {
          emlrtErrorWithMessageIdR2018a(&f_st, &cb_emlrtRTEI, "MATLAB:nomem",
                                        "MATLAB:nomem", 0);
        } else {
          emlrtErrorWithMessageIdR2018a(&f_st, &bb_emlrtRTEI,
                                        "Coder:toolbox:LAPACKCallErrorInfo",
                                        "Coder:toolbox:LAPACKCallErrorInfo", 5,
                                        4, 14, &c_fname[0], 12, (int32_T)N);
        }
      }
    } else {
      p = false;
    }
    if (p) {
      na = b_B->size[0];
      ma = b_B->size[0];
      b_B->size[0] = na;
      emxEnsureCapacity_real_T(&e_st, b_B, ma, &ae_emlrtRTEI);
      b_B_data = b_B->data;
      for (j = 0; j < na; j++) {
        b_B_data[j] = rtNaN;
      }
    }
    d_st.site = &if_emlrtRSI;
    if (rankA > 2147483646) {
      e_st.site = &cb_emlrtRSI;
      check_forloop_overflow_error(&e_st);
    }
    for (j = 0; j < rankA; j++) {
      Y_data[jpvt_data[j] - 1] = b_B_data[j];
    }
    for (j = rankA; j >= 1; j--) {
      ma = jpvt_data[j - 1];
      Y_data[ma - 1] /= b_A_data[(j + b_A->size[0] * (j - 1)) - 1];
      d_st.site = &jf_emlrtRSI;
      for (i = 0; i <= j - 2; i++) {
        Y_data[jpvt_data[i] - 1] -=
            Y_data[ma - 1] * b_A_data[i + b_A->size[0] * (j - 1)];
      }
    }
  }
  emxFree_real_T(&st, &b_B);
  emxFree_ptrdiff_t(&st, &jpvt_t);
  emxFree_real_T(&st, &tau);
  emxFree_int32_T(&st, &jpvt);
  emxFree_real_T(&st, &b_A);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (mldivide.c) */
