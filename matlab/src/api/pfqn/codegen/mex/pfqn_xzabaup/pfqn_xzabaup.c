/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_xzabaup.c
 *
 * Code generation for function 'pfqn_xzabaup'
 *
 */

/* Include files */
#include "pfqn_xzabaup.h"
#include "eml_int_forloop_overflow_check.h"
#include "mrdivide_helper.h"
#include "pfqn_xzabaup_data.h"
#include "pfqn_xzabaup_emxutil.h"
#include "pfqn_xzabaup_types.h"
#include "rt_nonfinite.h"
#include "sumMatrixIncludeNaN.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>

/* Variable Definitions */
static emlrtRSInfo emlrtRSI =
    {
        19,             /* lineNo */
        "pfqn_xzabaup", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_xzabaup.m" /* pathName */
};

static emlrtRSInfo b_emlrtRSI = {
    15,    /* lineNo */
    "max", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/max.m" /* pathName
                                                                     */
};

static emlrtRSInfo c_emlrtRSI = {
    73,         /* lineNo */
    "minOrMax", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

static emlrtRSInfo d_emlrtRSI = {
    108,       /* lineNo */
    "maximum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

static emlrtRSInfo e_emlrtRSI =
    {
        290,             /* lineNo */
        "unaryMinOrMax", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo f_emlrtRSI =
    {
        383,                     /* lineNo */
        "unaryMinOrMaxDispatch", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo g_emlrtRSI =
    {
        451,          /* lineNo */
        "minOrMax2D", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo h_emlrtRSI =
    {
        533,                         /* lineNo */
        "minOrMax2DColumnMajorDim1", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo i_emlrtRSI =
    {
        531,                         /* lineNo */
        "minOrMax2DColumnMajorDim1", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo x_emlrtRSI = {
    20,    /* lineNo */
    "sum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sum.m" /* pathName
                                                                     */
};

static emlrtRSInfo y_emlrtRSI =
    {
        99,        /* lineNo */
        "sumprod", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
        "sumprod.m" /* pathName */
};

static emlrtRSInfo ab_emlrtRSI = {
    86,                      /* lineNo */
    "combineVectorElements", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "combineVectorElements.m" /* pathName */
};

static emlrtRSInfo bb_emlrtRSI = {
    107,                /* lineNo */
    "blockedSummation", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo cb_emlrtRSI = {
    22,                    /* lineNo */
    "sumMatrixIncludeNaN", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtRSInfo db_emlrtRSI = {
    41,                 /* lineNo */
    "sumMatrixColumns", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtRSInfo eb_emlrtRSI = {
    42,                 /* lineNo */
    "sumMatrixColumns", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtRSInfo fb_emlrtRSI = {
    50,                 /* lineNo */
    "sumMatrixColumns", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtRSInfo gb_emlrtRSI = {
    57,                 /* lineNo */
    "sumMatrixColumns", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtRSInfo kb_emlrtRSI = {
    20,                /* lineNo */
    "mrdivide_helper", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "mrdivide_helper.m" /* pathName */
};

static emlrtRTEInfo emlrtRTEI =
    {
        76,        /* lineNo */
        9,         /* colNo */
        "sumprod", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
        "sumprod.m" /* pName */
};

static emlrtRTEInfo b_emlrtRTEI =
    {
        46,        /* lineNo */
        23,        /* colNo */
        "sumprod", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
        "sumprod.m" /* pName */
};

static emlrtRTEInfo c_emlrtRTEI =
    {
        198,             /* lineNo */
        27,              /* colNo */
        "unaryMinOrMax", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pName */
};

static emlrtRTEInfo d_emlrtRTEI =
    {
        90,              /* lineNo */
        27,              /* colNo */
        "unaryMinOrMax", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pName */
};

static emlrtRTEInfo e_emlrtRTEI = {
    16,                /* lineNo */
    19,                /* colNo */
    "mrdivide_helper", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "mrdivide_helper.m" /* pName */
};

static emlrtECInfo emlrtECI =
    {
        2,              /* nDims */
        19,             /* lineNo */
        25,             /* colNo */
        "pfqn_xzabaup", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_xzabaup.m" /* pName */
};

static emlrtRTEInfo i_emlrtRTEI =
    {
        19,             /* lineNo */
        25,             /* colNo */
        "pfqn_xzabaup", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_xzabaup.m" /* pName */
};

static emlrtRTEInfo j_emlrtRTEI = {
    35,                    /* lineNo */
    20,                    /* colNo */
    "sumMatrixIncludeNaN", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pName */
};

static emlrtRTEInfo k_emlrtRTEI =
    {
        523,             /* lineNo */
        21,              /* colNo */
        "unaryMinOrMax", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pName */
};

static emlrtRTEInfo l_emlrtRTEI =
    {
        19,             /* lineNo */
        15,             /* colNo */
        "pfqn_xzabaup", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_xzabaup.m" /* pName */
};

/* Function Declarations */
static void plus(const emlrtStack *sp, emxArray_real_T *in1,
                 const emxArray_real_T *in2);

/* Function Definitions */
static void plus(const emlrtStack *sp, emxArray_real_T *in1,
                 const emxArray_real_T *in2)
{
  jmp_buf *volatile emlrtJBStack;
  emxArray_real_T *b_in1;
  const real_T *in2_data;
  real_T *b_in1_data;
  real_T *in1_data;
  int32_T i;
  int32_T i1;
  int32_T loop_ub;
  int32_T plus_numThreads;
  int32_T stride_0_1;
  int32_T stride_1_1;
  in2_data = in2->data;
  in1_data = in1->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &b_in1, 2, &i_emlrtRTEI);
  stride_0_1 = b_in1->size[0] * b_in1->size[1];
  b_in1->size[0] = 1;
  if (in2->size[1] == 1) {
    loop_ub = in1->size[1];
  } else {
    loop_ub = in2->size[1];
  }
  b_in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, b_in1, stride_0_1, &i_emlrtRTEI);
  b_in1_data = b_in1->data;
  stride_0_1 = (in1->size[1] != 1);
  stride_1_1 = (in2->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      b_in1_data[i] = in1_data[i * stride_0_1] + in2_data[i * stride_1_1];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    plus_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(plus_numThreads)

    for (i = 0; i < loop_ub; i++) {
      b_in1_data[i] = in1_data[i * stride_0_1] + in2_data[i * stride_1_1];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[0] = 1;
  in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &i_emlrtRTEI);
  in1_data = in1->data;
  for (i1 = 0; i1 < loop_ub; i1++) {
    in1_data[i1] = b_in1_data[i1];
  }
  emxFree_real_T(sp, &b_in1);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

emlrtCTX emlrtGetRootTLSGlobal(void)
{
  return emlrtRootTLSGlobal;
}

void emlrtLockerFunction(EmlrtLockeeFunction aLockee, emlrtConstCTX aTLS,
                         void *aData)
{
  omp_set_lock(&emlrtLockGlobal);
  emlrtCallLockeeFunction(aLockee, aTLS, aData);
  omp_unset_lock(&emlrtLockGlobal);
}

real_T pfqn_xzabaup(const emlrtStack *sp, const emxArray_real_T *L,
                    const emxArray_real_T *N, const emxArray_real_T *Z)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack g_st;
  emlrtStack h_st;
  emlrtStack i_st;
  emlrtStack st;
  emxArray_real_T *B;
  emxArray_real_T *maxval;
  const real_T *L_data;
  const real_T *Z_data;
  real_T XN;
  real_T sz_idx_0;
  real_T *B_data;
  real_T *maxval_data;
  int32_T b_i;
  int32_T j;
  int32_T m;
  int32_T n;
  int32_T nfb;
  boolean_T overflow;
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
  h_st.prev = &g_st;
  h_st.tls = g_st.tls;
  i_st.prev = &h_st;
  i_st.tls = h_st.tls;
  Z_data = Z->data;
  L_data = L->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /* { */
  /*  % @file pfqn_xzabaup.m */
  /*  % @brief Upper asymptotic bound on throughput (Zahorjan-Balanced). */
  /* } */
  /* { */
  /*  % @brief Upper asymptotic bound on throughput (Zahorjan-Balanced). */
  /*  % @fn pfqn_xzabaup(L, N, Z) */
  /*  % @param L Service demand vector. */
  /*  % @param N Population. */
  /*  % @param Z Think time. */
  /*  % @return XN Upper bound on throughput. */
  /* } */
  st.site = &emlrtRSI;
  b_st.site = &x_emlrtRSI;
  overflow = ((L->size[0] == 1) && (L->size[1] == 1));
  if ((!overflow) && (L->size[0] == 1)) {
    emlrtErrorWithMessageIdR2018a(&b_st, &b_emlrtRTEI,
                                  "Coder:toolbox:autoDimIncompatibility",
                                  "Coder:toolbox:autoDimIncompatibility", 0);
  }
  if ((L->size[0] == 0) && (L->size[1] == 0)) {
    emlrtErrorWithMessageIdR2018a(&b_st, &emlrtRTEI,
                                  "Coder:toolbox:UnsupportedSpecialEmpty",
                                  "Coder:toolbox:UnsupportedSpecialEmpty", 0);
  }
  c_st.site = &y_emlrtRSI;
  d_st.site = &ab_emlrtRSI;
  emxInit_real_T(&d_st, &B, 2, &i_emlrtRTEI);
  if ((L->size[0] == 0) || (L->size[1] == 0)) {
    nfb = B->size[0] * B->size[1];
    B->size[0] = 1;
    m = L->size[1];
    B->size[1] = L->size[1];
    emxEnsureCapacity_real_T(&d_st, B, nfb, &i_emlrtRTEI);
    B_data = B->data;
    for (j = 0; j < m; j++) {
      B_data[j] = 0.0;
    }
  } else {
    int32_T i;
    e_st.site = &bb_emlrtRSI;
    f_st.site = &cb_emlrtRSI;
    nfb = B->size[0] * B->size[1];
    B->size[0] = 1;
    i = L->size[1];
    B->size[1] = L->size[1];
    emxEnsureCapacity_real_T(&f_st, B, nfb, &j_emlrtRTEI);
    B_data = B->data;
    if (L->size[0] < 4096) {
      g_st.site = &db_emlrtRSI;
      if (L->size[1] > 2147483646) {
        h_st.site = &j_emlrtRSI;
        check_forloop_overflow_error(&h_st);
      }
      for (j = 0; j < i; j++) {
        g_st.site = &eb_emlrtRSI;
        B_data[j] = sumColumnB(&g_st, L, j + 1, L->size[0]);
      }
    } else {
      nfb = (int32_T)((uint32_T)L->size[0] >> 12);
      m = nfb << 12;
      n = L->size[0] - m;
      g_st.site = &fb_emlrtRSI;
      if (L->size[1] > 2147483646) {
        h_st.site = &j_emlrtRSI;
        check_forloop_overflow_error(&h_st);
      }
      for (j = 0; j < i; j++) {
        sz_idx_0 = sumColumnB4(L, j + 1, 1);
        for (b_i = 2; b_i <= nfb; b_i++) {
          sz_idx_0 += sumColumnB4(L, j + 1, ((b_i - 1) << 12) + 1);
        }
        if (n > 0) {
          g_st.site = &gb_emlrtRSI;
          sz_idx_0 += b_sumColumnB(&g_st, L, j + 1, n, m + 1);
        }
        B_data[j] = sz_idx_0;
      }
    }
  }
  if ((B->size[1] != Z->size[1]) && ((B->size[1] != 1) && (Z->size[1] != 1))) {
    emlrtDimSizeImpxCheckR2021b(B->size[1], Z->size[1], &emlrtECI,
                                (emlrtConstCTX)sp);
  }
  emxInit_real_T(sp, &maxval, 2, &l_emlrtRTEI);
  st.site = &emlrtRSI;
  b_st.site = &b_emlrtRSI;
  c_st.site = &c_emlrtRSI;
  d_st.site = &d_emlrtRSI;
  if ((!overflow) && (L->size[0] == 1)) {
    emlrtErrorWithMessageIdR2018a(&d_st, &d_emlrtRTEI,
                                  "Coder:toolbox:autoDimIncompatibility",
                                  "Coder:toolbox:autoDimIncompatibility", 0);
  }
  if (L->size[0] < 1) {
    emlrtErrorWithMessageIdR2018a(&d_st, &c_emlrtRTEI,
                                  "Coder:toolbox:eml_min_or_max_varDimZero",
                                  "Coder:toolbox:eml_min_or_max_varDimZero", 0);
  }
  e_st.site = &e_emlrtRSI;
  f_st.site = &f_emlrtRSI;
  g_st.site = &g_emlrtRSI;
  m = L->size[0];
  n = L->size[1];
  nfb = maxval->size[0] * maxval->size[1];
  maxval->size[0] = 1;
  maxval->size[1] = L->size[1];
  emxEnsureCapacity_real_T(&g_st, maxval, nfb, &k_emlrtRTEI);
  maxval_data = maxval->data;
  if (L->size[1] >= 1) {
    h_st.site = &i_emlrtRSI;
    if (L->size[1] > 2147483646) {
      i_st.site = &j_emlrtRSI;
      check_forloop_overflow_error(&i_st);
    }
    overflow = (L->size[0] > 2147483646);
    for (j = 0; j < n; j++) {
      maxval_data[j] = L_data[L->size[0] * j];
      h_st.site = &h_emlrtRSI;
      if (overflow) {
        i_st.site = &j_emlrtRSI;
        check_forloop_overflow_error(&i_st);
      }
      for (b_i = 2; b_i <= m; b_i++) {
        boolean_T p;
        sz_idx_0 = maxval_data[j];
        XN = L_data[(b_i + L->size[0] * j) - 1];
        if (muDoubleScalarIsNaN(XN)) {
          p = false;
        } else if (muDoubleScalarIsNaN(sz_idx_0)) {
          p = true;
        } else {
          p = (sz_idx_0 < XN);
        }
        if (p) {
          maxval_data[j] = XN;
        }
      }
    }
  }
  st.site = &emlrtRSI;
  if (maxval->size[1] != 1) {
    emlrtErrorWithMessageIdR2018a(&st, &e_emlrtRTEI, "MATLAB:dimagree",
                                  "MATLAB:dimagree", 0);
  }
  st.site = &emlrtRSI;
  if (B->size[1] == Z->size[1]) {
    n = B->size[1] - 1;
    nfb = B->size[0] * B->size[1];
    B->size[0] = 1;
    emxEnsureCapacity_real_T(&st, B, nfb, &i_emlrtRTEI);
    B_data = B->data;
    nfb = (B->size[1] / 2) << 1;
    m = nfb - 2;
    for (j = 0; j <= m; j += 2) {
      __m128d r;
      r = _mm_loadu_pd(&B_data[j]);
      _mm_storeu_pd(&B_data[j], _mm_add_pd(r, _mm_loadu_pd(&Z_data[j])));
    }
    for (j = nfb; j <= n; j++) {
      B_data[j] += Z_data[j];
    }
  } else {
    b_st.site = &emlrtRSI;
    plus(&b_st, B, Z);
  }
  if (B->size[1] != N->size[1]) {
    emlrtErrorWithMessageIdR2018a(&st, &e_emlrtRTEI, "MATLAB:dimagree",
                                  "MATLAB:dimagree", 0);
  }
  b_st.site = &kb_emlrtRSI;
  XN = mrdiv(&b_st, N, B);
  emxFree_real_T(&st, &B);
  sz_idx_0 = 1.0 / maxval_data[0];
  emxFree_real_T(sp, &maxval);
  if ((!(sz_idx_0 > XN)) &&
      ((!muDoubleScalarIsNaN(sz_idx_0)) || muDoubleScalarIsNaN(XN))) {
    XN = sz_idx_0;
  }
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return XN;
}

/* End of code generation (pfqn_xzabaup.c) */
