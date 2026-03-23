/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mmint2_gausslaguerre.c
 *
 * Code generation for function 'pfqn_mmint2_gausslaguerre'
 *
 */

/* Include files */
#include "pfqn_mmint2_gausslaguerre.h"
#include "applyScalarFunctionInPlace.h"
#include "gammaln.h"
#include "pfqn_mmint2_gausslaguerre_data.h"
#include "pfqn_mmint2_gausslaguerre_emxutil.h"
#include "pfqn_mmint2_gausslaguerre_types.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>
#include <string.h>

/* Variable Definitions */
static real_T gausslaguerreNodes[300];

static real_T gausslaguerreWeights[300];

static emlrtRSInfo emlrtRSI = {
    38,                          /* lineNo */
    "pfqn_mmint2_gausslaguerre", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m" /* pathName */
};

static emlrtRSInfo b_emlrtRSI = {
    41,                          /* lineNo */
    "pfqn_mmint2_gausslaguerre", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m" /* pathName */
};

static emlrtRSInfo c_emlrtRSI = {
    44,                          /* lineNo */
    "pfqn_mmint2_gausslaguerre", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m" /* pathName */
};

static emlrtRSInfo d_emlrtRSI = {
    48,                          /* lineNo */
    "pfqn_mmint2_gausslaguerre", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m" /* pathName */
};

static emlrtRSInfo e_emlrtRSI = {
    49,                          /* lineNo */
    "pfqn_mmint2_gausslaguerre", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m" /* pathName */
};

static emlrtRSInfo f_emlrtRSI = {
    51,                          /* lineNo */
    "pfqn_mmint2_gausslaguerre", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m" /* pathName */
};

static emlrtRSInfo r_emlrtRSI = {
    17,                                                           /* lineNo */
    "log",                                                        /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/log.m" /* pathName */
};

static emlrtRSInfo s_emlrtRSI = {
    44,                           /* lineNo */
    "applyScalarFunctionInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunctionInPlace.m" /* pathName */
};

static emlrtRSInfo t_emlrtRSI = {
    8,                                                             /* lineNo */
    "factln",                                                      /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/factln.m" /* pathName
                                                                    */
};

static emlrtRSInfo u_emlrtRSI = {
    10,        /* lineNo */
    "gammaln", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/gammaln.m" /* pathName
                                                                         */
};

static emlrtRSInfo cb_emlrtRSI = {
    17,                           /* lineNo */
    "applyScalarFunctionInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunctionInPlace.m" /* pathName */
};

static emlrtRSInfo db_emlrtRSI = {
    10,                                                           /* lineNo */
    "exp",                                                        /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/exp.m" /* pathName */
};

static emlrtRSInfo eb_emlrtRSI = {
    15,          /* lineNo */
    "logsumexp", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/logsumexp.m" /* pathName
                                                                       */
};

static emlrtRSInfo fb_emlrtRSI = {
    24,          /* lineNo */
    "logsumexp", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/logsumexp.m" /* pathName
                                                                       */
};

static emlrtRSInfo gb_emlrtRSI = {
    17,    /* lineNo */
    "max", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/max.m" /* pathName
                                                                     */
};

static emlrtRSInfo hb_emlrtRSI = {
    67,         /* lineNo */
    "minOrMax", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

static emlrtRSInfo ib_emlrtRSI = {
    106,       /* lineNo */
    "maximum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

static emlrtDCInfo emlrtDCI = {
    39,                          /* lineNo */
    13,                          /* colNo */
    "pfqn_mmint2_gausslaguerre", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m", /* pName */
    4                              /* checkKind */
};

static emlrtDCInfo b_emlrtDCI = {
    39,                          /* lineNo */
    13,                          /* colNo */
    "pfqn_mmint2_gausslaguerre", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m", /* pName */
    1                              /* checkKind */
};

static emlrtECInfo emlrtECI = {
    2,                           /* nDims */
    48,                          /* lineNo */
    5,                           /* colNo */
    "pfqn_mmint2_gausslaguerre", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m" /* pName */
};

static emlrtBCInfo emlrtBCI = {
    -1,          /* iFirst */
    -1,          /* iLast */
    19,          /* lineNo */
    19,          /* colNo */
    "x",         /* aName */
    "logsumexp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/logsumexp.m", /* pName
                                                                        */
    0 /* checkKind */
};

static emlrtRTEInfo b_emlrtRTEI =
    {
        198,             /* lineNo */
        27,              /* colNo */
        "unaryMinOrMax", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pName */
};

static emlrtRTEInfo c_emlrtRTEI = {
    14,                                                           /* lineNo */
    9,                                                            /* colNo */
    "log",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/log.m" /* pName */
};

static emlrtBCInfo b_emlrtBCI = {
    -1,                          /* iFirst */
    -1,                          /* iLast */
    41,                          /* lineNo */
    7,                           /* colNo */
    "F",                         /* aName */
    "pfqn_mmint2_gausslaguerre", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m", /* pName */
    0                              /* checkKind */
};

static emlrtBCInfo c_emlrtBCI = {
    -1,                          /* iFirst */
    -1,                          /* iLast */
    43,                          /* lineNo */
    14,                          /* colNo */
    "N",                         /* aName */
    "pfqn_mmint2_gausslaguerre", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m", /* pName */
    0                              /* checkKind */
};

static emlrtBCInfo d_emlrtBCI = {
    -1,                          /* iFirst */
    -1,                          /* iLast */
    44,                          /* lineNo */
    40,                          /* colNo */
    "Z",                         /* aName */
    "pfqn_mmint2_gausslaguerre", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m", /* pName */
    0                              /* checkKind */
};

static emlrtBCInfo e_emlrtBCI = {
    -1,                          /* iFirst */
    -1,                          /* iLast */
    44,                          /* lineNo */
    45,                          /* colNo */
    "L",                         /* aName */
    "pfqn_mmint2_gausslaguerre", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m", /* pName */
    0                              /* checkKind */
};

static emlrtBCInfo f_emlrtBCI = {
    -1,                          /* iFirst */
    -1,                          /* iLast */
    44,                          /* lineNo */
    22,                          /* colNo */
    "F",                         /* aName */
    "pfqn_mmint2_gausslaguerre", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m", /* pName */
    0                              /* checkKind */
};

static emlrtBCInfo g_emlrtBCI = {
    -1,                          /* iFirst */
    -1,                          /* iLast */
    44,                          /* lineNo */
    29,                          /* colNo */
    "N",                         /* aName */
    "pfqn_mmint2_gausslaguerre", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m", /* pName */
    0                              /* checkKind */
};

static emlrtBCInfo h_emlrtBCI = {
    -1,                          /* iFirst */
    -1,                          /* iLast */
    44,                          /* lineNo */
    15,                          /* colNo */
    "F",                         /* aName */
    "pfqn_mmint2_gausslaguerre", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mmint2_gausslaguerre.m", /* pName */
    0                              /* checkKind */
};

static emlrtBCInfo i_emlrtBCI = {
    -1,          /* iFirst */
    -1,          /* iLast */
    19,          /* lineNo */
    7,           /* colNo */
    "w",         /* aName */
    "logsumexp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/logsumexp.m", /* pName
                                                                        */
    0 /* checkKind */
};

static emlrtBCInfo j_emlrtBCI = {
    -1,          /* iFirst */
    -1,          /* iLast */
    21,          /* lineNo */
    19,          /* colNo */
    "w",         /* aName */
    "logsumexp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/logsumexp.m", /* pName
                                                                        */
    0 /* checkKind */
};

static emlrtRTEInfo g_emlrtRTEI = {
    8,                                                             /* lineNo */
    1,                                                             /* colNo */
    "factln",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/factln.m" /* pName */
};

static emlrtRTEInfo h_emlrtRTEI = {
    8,                                                             /* lineNo */
    14,                                                            /* colNo */
    "factln",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/factln.m" /* pName */
};

/* Function Declarations */
static void binary_expand_op(const emlrtStack *sp, real_T in1_data[],
                             int32_T in1_size[2], const real_T in2_data[],
                             const int32_T in2_size[2], const emlrtRSInfo in3,
                             const emxArray_real_T *in4, real_T in5);

/* Function Definitions */
static void binary_expand_op(const emlrtStack *sp, real_T in1_data[],
                             int32_T in1_size[2], const real_T in2_data[],
                             const int32_T in2_size[2], const emlrtRSInfo in3,
                             const emxArray_real_T *in4, real_T in5)
{
  emlrtStack st;
  real_T b_in1_data[300];
  real_T d;
  int32_T i;
  int32_T loop_ub;
  int32_T stride_0_1;
  int32_T stride_1_1;
  st.prev = sp;
  st.tls = sp->tls;
  st.site = (emlrtRSInfo *)&in3;
  d = sum(&st, in4);
  if (in2_size[1] == 1) {
    loop_ub = in1_size[1];
  } else {
    loop_ub = in2_size[1];
  }
  stride_0_1 = (in1_size[1] != 1);
  stride_1_1 = (in2_size[1] != 1);
  for (i = 0; i < loop_ub; i++) {
    b_in1_data[i] =
        ((in1_data[i * stride_0_1] + in2_data[i * stride_1_1]) - d) - in5;
  }
  in1_size[0] = 1;
  in1_size[1] = loop_ub;
  if (loop_ub - 1 >= 0) {
    memcpy(&in1_data[0], &b_in1_data[0], (uint32_T)loop_ub * sizeof(real_T));
  }
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

void pfqn_mmint2_gausslaguerre(const emlrtStack *sp, const emxArray_real_T *L,
                               const emxArray_real_T *N,
                               const emxArray_real_T *Z, real_T m, real_T *G,
                               real_T *lG)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack st;
  emxArray_real_T b_F_data;
  emxArray_real_T *x;
  real_T F_data[300];
  real_T g_data[300];
  const real_T *L_data;
  const real_T *N_data;
  const real_T *Z_data;
  real_T b_x;
  real_T n;
  real_T s;
  real_T *x_data;
  int32_T F_size[2];
  int32_T g_size[2];
  int32_T b_loop_ub;
  int32_T c_loop_ub;
  int32_T i;
  int32_T loop_ub;
  int32_T r;
  int32_T scalarLB;
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
  Z_data = Z->data;
  N_data = N->data;
  L_data = L->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /* { */
  /*  % @file pfqn_mmint2_gausslaguerre.m */
  /*  % @brief McKenna-Mitra integral with Gauss-Laguerre quadrature. */
  /* } */
  /* { */
  /*  % @brief McKenna-Mitra integral with Gauss-Laguerre quadrature. */
  /*  % @fn pfqn_mmint2_gausslaguerre(L, N, Z, m) */
  /*  % @param L Service demand vector. */
  /*  % @param N Population vector. */
  /*  % @param Z Think time vector. */
  /*  % @param m Replication factor (default: 1). */
  /*  % @return G Normalizing constant. */
  /*  % @return lG Logarithm of normalizing constant. */
  /* } */
  /*  [G,LOGG] = PFQN_MMINT2_GAUSSLAGUERRE(L,N,Z,m) */
  /*  */
  /*  Integrate with Gauss-Laguerre */
  st.site = &emlrtRSI;
  s = 2.0 * sum(&st, N) + 1.0;
  n = muDoubleScalarMin(300.0, s);
  if (!(n >= 0.0)) {
    emlrtNonNegativeCheckR2012b(n, &emlrtDCI, (emlrtConstCTX)sp);
  }
  if (n != muDoubleScalarFloor(n)) {
    emlrtIntegerCheckR2012b(n, &b_emlrtDCI, (emlrtConstCTX)sp);
  }
  F_size[0] = 1;
  loop_ub = (int32_T)n;
  F_size[1] = (int32_T)n;
  if (loop_ub - 1 >= 0) {
    memset(&F_data[0], 0, (uint32_T)loop_ub * sizeof(real_T));
  }
  for (i = 0; i < loop_ub; i++) {
    st.site = &b_emlrtRSI;
    if (gausslaguerreNodes[i] < 0.0) {
      emlrtErrorWithMessageIdR2018a(
          &st, &c_emlrtRTEI, "Coder:toolbox:ElFunDomainError",
          "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
    }
    if (i + 1 > F_size[1]) {
      emlrtDynamicBoundsCheckR2012b(i + 1, 1, F_size[1], &b_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    F_data[i] = (m - 1.0) * muDoubleScalarLog(gausslaguerreNodes[i]);
    scalarLB = N->size[1];
    for (r = 0; r < scalarLB; r++) {
      if (r + 1 > scalarLB) {
        emlrtDynamicBoundsCheckR2012b(r + 1, 1, scalarLB, &c_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      s = N_data[r];
      if (s != 0.0) {
        st.site = &c_emlrtRSI;
        if (r + 1 > Z->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, Z->size[1], &d_emlrtBCI, &st);
        }
        c_loop_ub = L->size[0] * L->size[1];
        if (r + 1 > c_loop_ub) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, c_loop_ub, &e_emlrtBCI, &st);
        }
        b_x = Z_data[r] + L_data[r] * gausslaguerreNodes[i];
        if (b_x < 0.0) {
          emlrtErrorWithMessageIdR2018a(
              &st, &c_emlrtRTEI, "Coder:toolbox:ElFunDomainError",
              "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
        }
        b_x = muDoubleScalarLog(b_x);
        if (i + 1 > F_size[1]) {
          emlrtDynamicBoundsCheckR2012b(i + 1, 1, F_size[1], &f_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > scalarLB) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, scalarLB, &g_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (i + 1 > F_size[1]) {
          emlrtDynamicBoundsCheckR2012b(i + 1, 1, F_size[1], &h_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        F_data[i] += s * b_x;
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  if (n < 1.0) {
    b_loop_ub = 0;
  } else {
    b_loop_ub = (int32_T)n;
  }
  st.site = &d_emlrtRSI;
  g_size[0] = 1;
  g_size[1] = b_loop_ub;
  if (b_loop_ub - 1 >= 0) {
    memcpy(&g_data[0], &gausslaguerreWeights[0],
           (uint32_T)b_loop_ub * sizeof(real_T));
  }
  b_st.site = &r_emlrtRSI;
  c_st.site = &s_emlrtRSI;
  for (i = 0; i < b_loop_ub; i++) {
    g_data[i] = muDoubleScalarLog(g_data[i]);
  }
  if ((b_loop_ub != F_size[1]) && ((b_loop_ub != 1) && (F_size[1] != 1))) {
    emlrtDimSizeImpxCheckR2021b(b_loop_ub, F_size[1], &emlrtECI,
                                (emlrtConstCTX)sp);
  }
  emxInit_real_T(sp, &x, &h_emlrtRTEI);
  st.site = &d_emlrtRSI;
  /*  lf=FACTLN(n) */
  /*  Compure the logarithm of n!        */
  /*  */
  /*  Copyright (c) 2012-2026, Imperial College London */
  /*  All rights reserved.   */
  b_st.site = &t_emlrtRSI;
  loop_ub = x->size[0] * x->size[1];
  x->size[0] = 1;
  c_loop_ub = N->size[1];
  x->size[1] = N->size[1];
  emxEnsureCapacity_real_T(&b_st, x, loop_ub, &g_emlrtRTEI);
  x_data = x->data;
  scalarLB = (N->size[1] / 2) << 1;
  loop_ub = scalarLB - 2;
  for (i = 0; i <= loop_ub; i += 2) {
    _mm_storeu_pd(&x_data[i],
                  _mm_add_pd(_mm_loadu_pd(&N_data[i]), _mm_set1_pd(1.0)));
  }
  for (i = scalarLB; i < c_loop_ub; i++) {
    x_data[i] = N_data[i] + 1.0;
  }
  c_st.site = &u_emlrtRSI;
  applyScalarFunctionInPlace(&c_st, x);
  st.site = &d_emlrtRSI;
  /*  lf=FACTLN(n) */
  /*  Compure the logarithm of n!        */
  /*  */
  /*  Copyright (c) 2012-2026, Imperial College London */
  /*  All rights reserved.   */
  b_st.site = &t_emlrtRSI;
  c_st.site = &u_emlrtRSI;
  s = (m - 1.0) + 1.0;
  d_st.site = &cb_emlrtRSI;
  scalar_gammaln(&d_st, &s);
  if (b_loop_ub == F_size[1]) {
    st.site = &d_emlrtRSI;
    n = sum(&st, x);
    c_loop_ub = b_loop_ub - 1;
    loop_ub = (b_loop_ub / 2) << 1;
    scalarLB = loop_ub - 2;
    for (i = 0; i <= scalarLB; i += 2) {
      __m128d b_r;
      __m128d r1;
      b_r = _mm_loadu_pd(&g_data[i]);
      r1 = _mm_loadu_pd(&F_data[i]);
      _mm_storeu_pd(&g_data[i],
                    _mm_sub_pd(_mm_sub_pd(_mm_add_pd(b_r, r1), _mm_set1_pd(n)),
                               _mm_set1_pd(s)));
    }
    for (i = loop_ub; i <= c_loop_ub; i++) {
      g_data[i] = ((g_data[i] + F_data[i]) - n) - s;
    }
  } else {
    st.site = &d_emlrtRSI;
    binary_expand_op(&st, g_data, g_size, F_data, F_size, d_emlrtRSI, x, s);
  }
  emxFree_real_T(sp, &x);
  st.site = &e_emlrtRSI;
  F_size[0] = 1;
  scalarLB = g_size[1];
  F_size[1] = g_size[1];
  if (scalarLB - 1 >= 0) {
    memcpy(&F_data[0], &g_data[0], (uint32_T)scalarLB * sizeof(real_T));
  }
  b_st.site = &db_emlrtRSI;
  c_st.site = &s_emlrtRSI;
  for (i = 0; i < scalarLB; i++) {
    F_data[i] = muDoubleScalarExp(F_data[i]);
  }
  st.site = &e_emlrtRSI;
  b_F_data.data = &F_data[0];
  b_F_data.size = &F_size[0];
  b_F_data.allocatedSize = 300;
  b_F_data.numDimensions = 2;
  b_F_data.canFreeData = false;
  b_st.site = &e_emlrtRSI;
  *lG = sum(&b_st, &b_F_data);
  if (*lG < 0.0) {
    emlrtErrorWithMessageIdR2018a(
        &st, &c_emlrtRTEI, "Coder:toolbox:ElFunDomainError",
        "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
  }
  *lG = muDoubleScalarLog(*lG);
  if (muDoubleScalarIsInf(*lG) || muDoubleScalarIsNaN(*lG)) {
    /*  if numerical difficulties switch to logsumexp trick */
    st.site = &f_emlrtRSI;
    /*  L = LOGSUMEXP(X) */
    /*  Approximate the logarithm of a sum of exponentials */
    /*  */
    /*  Copyright (c) 2012-2026, Imperial College London */
    /*  All rights reserved. */
    /* xstar = max(x); */
    /* lse = xstar + log(sum(exp(x-xstar))); */
    /*  Implementation described in: */
    /*  P. Blanchard et al., ACCURATE COMPUTATION OF THE LOG-SUM-EXP AND SOFTMAX
     */
    /*  FUNCTIONS, IMA Journal of Numerical Analysis, draa038,
     * https://doi.org/10.1093/imanum/draa038 */
    b_st.site = &eb_emlrtRSI;
    c_st.site = &gb_emlrtRSI;
    d_st.site = &hb_emlrtRSI;
    e_st.site = &ib_emlrtRSI;
    if (g_size[1] < 1) {
      emlrtErrorWithMessageIdR2018a(
          &e_st, &b_emlrtRTEI, "Coder:toolbox:eml_min_or_max_varDimZero",
          "Coder:toolbox:eml_min_or_max_varDimZero", 0);
    }
    if (g_size[1] <= 2) {
      if (g_size[1] == 1) {
        b_x = g_data[0];
        c_loop_ub = 1;
      } else if ((g_data[0] < g_data[1]) ||
                 (muDoubleScalarIsNaN(g_data[0]) &&
                  (!muDoubleScalarIsNaN(g_data[1])))) {
        b_x = g_data[1];
        c_loop_ub = 2;
      } else {
        b_x = g_data[0];
        c_loop_ub = 1;
      }
    } else {
      if (!muDoubleScalarIsNaN(g_data[0])) {
        c_loop_ub = 1;
      } else {
        boolean_T exitg1;
        c_loop_ub = 0;
        loop_ub = 2;
        exitg1 = false;
        while ((!exitg1) && (loop_ub <= scalarLB)) {
          if (!muDoubleScalarIsNaN(g_data[loop_ub - 1])) {
            c_loop_ub = loop_ub;
            exitg1 = true;
          } else {
            loop_ub++;
          }
        }
      }
      if (c_loop_ub == 0) {
        b_x = g_data[0];
        c_loop_ub = 1;
      } else {
        b_x = g_data[c_loop_ub - 1];
        loop_ub = c_loop_ub + 1;
        for (i = loop_ub; i <= scalarLB; i++) {
          n = g_data[i - 1];
          if (b_x < n) {
            b_x = n;
            c_loop_ub = i;
          }
        }
      }
    }
    loop_ub = g_size[1];
    s = 0.0;
    for (i = 0; i < scalarLB; i++) {
      if (i + 1 > scalarLB) {
        emlrtDynamicBoundsCheckR2012b(i + 1, 1, scalarLB, &emlrtBCI, &st);
      }
      if (i + 1 > loop_ub) {
        emlrtDynamicBoundsCheckR2012b(i + 1, 1, loop_ub, &i_emlrtBCI, &st);
      }
      if (i + 1 != c_loop_ub) {
        if (i + 1 > loop_ub) {
          emlrtDynamicBoundsCheckR2012b(i + 1, 1, loop_ub, &j_emlrtBCI, &st);
        }
        s += muDoubleScalarExp(g_data[i] - b_x);
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b(&st);
      }
    }
    b_st.site = &fb_emlrtRSI;
    if ((s > 4.503599627370496E+15) || muDoubleScalarIsNaN(s)) {
      s = muDoubleScalarLog(s + 1.0);
    } else if (!(s < 2.2204460492503131E-16)) {
      s = muDoubleScalarLog(s + 1.0) * (s / ((s + 1.0) - 1.0));
    }
    *lG = b_x + s;
  }
  *G = muDoubleScalarExp(*lG);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

void pfqn_mmint2_gausslaguerre_init(void)
{
  static const real_T dv[300] = {
      0.0048113069972288858, 0.025350634360810493, 0.062303050023644412,
      0.11567777099836181,   0.18547726133237027,  0.2717036690560572,
      0.37435943160859031,   0.49344738896632484,  0.62897081403452759,
      0.78093342310379954,   0.94933938028472742,  1.1341932998281188,
      1.3355002476300382,    1.553265742412262,    1.787495756782473,
      2.0381967182671956,    2.3053755103620084,   2.5890394736225781,
      2.889196406808705,     3.2058545680889132,   3.5390226763097221,
      3.88870991233258,      4.2549259204403285,   4.6376808098146229,
      5.03698515608546,      5.4528500029537978,   5.8852868638881191,
      6.3343077238956349,    6.7999250413691588,   7.282151750010228,
      7.781001260829238,     8.2964874642236115,   8.8286247321346085,
      9.37742792028361,      9.942912370489,       10.525093913064243,
      11.123988869298168,    11.739614054018642,   12.371986778240286,
      13.021124851897467,    13.687046586663643,   14.369770798858077,
      15.069316812441063,    15.785704462098908,   16.518954096419911,
      17.269086581162377,    18.036123302616353,   18.820086171060058,
      19.620997624312533,    20.438880631384063,   21.273758696225705,
      22.125655861579432,    22.994596712930534,   23.880606382564018,
      24.783710553726294,    25.7039354648944,     26.641307914154073,
      27.595855263688811,    28.56760544438162,    29.556586960531611,
      30.562828894687165,    31.58636091259805,    32.627213268288259,
      33.6854168092523,      34.761002981776464,   35.854003836388195,
      36.964452033435272,    38.092380848797774,   39.237824179735178,
      40.400816550871,       41.581393120318232,   42.779589685947734,
      43.995442691802879,    45.22898923466331,    46.480267070760618,
      47.749314622649472,    49.036170986237046,   50.340875937974317,
      51.663469942212586,    53.003994158728673,   54.36249045042257,
      55.73900139119106,     57.133570273981547,   58.546241119029567,
      59.977058682284536,    61.426068464027679,   62.893316717686567,
      64.378850458850721,    65.882717474493035,   67.404966332401543,
      68.945646390826667,    70.504807808348829,   72.082501553971753,
      73.678779417446918,    75.293694019834433,   76.92729882430632,
      78.579648147198043,    80.250797169314339,   81.940801947495657,
      83.649719426451583,    85.377607450868226,   87.124524777796253,
      88.890531089326515,    90.675687005561187,   92.4800540978875,
      94.303694902562128,    96.146672934614315,   98.00905270207609,
      99.8908997205484,      101.79228052811165,   103.71326270059036,
      105.65391486718136,    107.61430672645513,   109.59450906274081,
      111.59459376290526,    113.61463383353711,   115.65470341854707,
      117.71487781719624,    119.79523350256433,   121.89584814047049,
      124.01680060885954,    126.15817101766702,   128.32004072917692,
      130.50249237888656,    132.70560989689324,   134.9294785298182,
      137.17418486328407,    139.43981684496194,   141.72646380820532,
      144.03421649628905,    146.36316708727119,   148.71340921949732,
      151.08503801776681,    153.47815012018174,   155.89284370570016,
      158.32921852241503,    160.78737591658285,   163.26741886242505,
      165.76945199272751,    168.29358163026365,   170.8399158200682,
      173.40856436258903,    175.99963884774672,   178.61325268993119,
      181.2495211639673,     183.90856144208192,   186.5904926319059,
      189.295435815547,      192.02351408977,      194.77485260732254,
      197.54957861944621,    200.34782151961511,   203.16971288854447,
      206.01538654051512,    208.88497857106032,   211.77862740606446,
      214.69647385232508,    217.63866114963113,   220.60533502441368,
      223.59664374502773,    226.61273817872538,   229.65377185038463,
      232.71990100306027,    235.81128466042622,   238.92808469118265,
      242.07046587550343,    245.23859597360445,   248.43264579651554,
      251.6527892791442,     254.89920355572215,   258.17206903773115,
      261.47156949440881,    264.79789213593978,   268.15122769944338,
      271.53177053787397,    274.93971871195595,   278.37527408528263,
      281.83864242271216,    285.33003349220422,   288.8496611702451,
      292.39774355101861,    295.97450305948803,   299.58016656856267,
      303.21496552053219,    306.87913605296205,   310.57291912925331,
      314.29656067408115,    318.05031171393938,   321.83442852302971,
      325.64917277474831,    329.494811699037,     333.37161824588128,
      337.27987125525419,    341.21985563382106,   345.19186253874182,
      349.19618956892367,    353.23314096410138,   357.30302781214391,
      361.40616826500991,    365.54288776380264,   369.71351927340157,
      373.91840352717782,    378.15788928233468,   382.43233358644733,
      386.74210205581437,    391.08756916627431,   395.46911855718247,
      399.88714334929165,    404.34204647732946,   408.83424103812064,
      413.3641506551611,     417.93220986061294,   422.53886449576186,
      427.18457213104836,    431.86980250686975,   436.5950379964346,
      441.36077409204751,    446.16751991630736,   451.01579875981207,
      455.9061486470892,     460.83912293260335,   465.81529092883915,
      470.83523856861814,    475.89956910398291,   481.00890384417431,
      486.16388293543639,    491.36516618561672,   496.61343393678095,
      501.9093879893428,     507.25375258151524,   512.64727542823073,
      518.09072882405246,    523.58491081501381,   529.13064644478345,
      534.72878908106452,    540.38022182870168,   546.08585903660423,
      551.84664790629427,    557.66357021067779,   563.53764413251406,
      569.46992623304391,    575.46151356234566,   581.51354592422751,
      587.62720830987257,    593.80373351602793,   600.04440496532811,
      606.35055974836644,    612.72359190943882,   619.16495600051223,
      625.676170930966,      632.258824144094,     638.91457615528623,
      645.64516549134828,    652.45241407563776,   659.33823310974788,
      666.30462950948413,    673.35371296104836,   680.48770367287887,
      687.70894090977356,    695.01989240905675,   702.42316479406156,
      709.9215151185615,     717.51786369764272,   725.215308406604,
      733.017140660794,      740.92686332704875,   748.94821086313391,
      757.08517203729969,    765.342015648263,     773.72331974994177,
      782.23400498936746,    790.87937279606263,   799.66514932426219,
      808.59753625574319,    817.68326983421946,   826.92968984078152,
      836.34482065919178,    845.93746715559462,   855.71732885966912,
      865.69513695548437,    875.88281997506044,   886.29370599054175,
      896.94277175436389,    907.8469529966884,    919.02553551210417,
      930.50065464331715,    942.29794275936433,   954.447382797394,
      966.98445519411632,    979.95171336127794,   993.40100394515582,
      1007.3966915320392,    1022.0205141693713,   1037.3792227922447,
      1053.6172775282357,    1070.9394870102738,   1089.6553632469481,
      1110.2784281884099,    1133.7998051516558,   1162.7974897209454};
  static const real_T dv1[300] = {0.012288119574648568,
                                  0.028023199844838316,
                                  0.042435117450012438,
                                  0.054859157496388408,
                                  0.064805647289991733,
                                  0.071971333479652219,
                                  0.07624951830301889,
                                  0.077722069300112148,
                                  0.076634833564472371,
                                  0.073360594547748459,
                                  0.068354872839697628,
                                  0.062110123122989136,
                                  0.055113340477928353,
                                  0.047810952692892747,
                                  0.040583397357733163,
                                  0.033730234557259028,
                                  0.027465265030497007,
                                  0.021920077851865536,
                                  0.017153823950906057,
                                  0.013166802637175654,
                                  0.00991559322277417,
                                  0.00732785860813644,
                                  0.0053154752460032413,
                                  0.0037851968330612413,
                                  0.0026465548621114193,
                                  0.0018170868009861807,
                                  0.0012252419956618392,
                                  0.00081145124114362333,
                                  0.00052787968271730942,
                                  0.00033734355522389723,
                                  0.00021178919195325257,
                                  0.00013063347509375818,
                                  7.91674669983885E-5,
                                  4.7141141208368784E-5,
                                  2.7582365750189952E-5,
                                  1.585826948317857E-5,
                                  8.9595399273282448E-6,
                                  4.9743028375546736E-6,
                                  2.7139675213621134E-6,
                                  1.455160154581347E-6,
                                  7.6675585823703442E-7,
                                  3.9705323891725946E-7,
                                  2.0206435997146821E-7,
                                  1.0106085216760541E-7,
                                  4.9674141654739839E-8,
                                  2.3995673739268886E-8,
                                  1.1391759101475946E-8,
                                  5.3149989930092092E-9,
                                  2.4370735614001346E-9,
                                  1.0982088085954446E-9,
                                  4.8635022537998594E-10,
                                  2.1166919390379038E-10,
                                  9.0532876412463055E-11,
                                  3.805312281890288E-11,
                                  1.5718251139582822E-11,
                                  6.3803153388023609E-12,
                                  2.5450574679400157E-12,
                                  9.9761641657083215E-13,
                                  3.84266604179677E-13,
                                  1.4544432913971247E-13,
                                  5.40938138612645E-14,
                                  1.9768611987920057E-14,
                                  7.0986077315207976E-15,
                                  2.5045449282054513E-15,
                                  8.682262646633394E-16,
                                  2.95716005603347E-16,
                                  9.8956317329049572E-17,
                                  3.2533257854839521E-17,
                                  1.0507879377360582E-17,
                                  3.33421694339267E-18,
                                  1.0393247016630765E-18,
                                  3.1825427389807208E-19,
                                  9.5730303541001209E-20,
                                  2.8285446454734791E-20,
                                  8.2092009386440149E-21,
                                  2.3401755566112663E-21,
                                  6.5522548177707843E-22,
                                  1.801825026893377E-22,
                                  4.8662933106397788E-23,
                                  1.2907191171041121E-23,
                                  3.3619913051230515E-24,
                                  8.5995560004232466E-25,
                                  2.1599958429598762E-25,
                                  5.3273314108572065E-26,
                                  1.2901152916483705E-26,
                                  3.0675458231979629E-27,
                                  7.1610823074340856E-28,
                                  1.6412408049175422E-28,
                                  3.692772592062886E-29,
                                  8.1564211019199125E-30,
                                  1.7684522955745618E-30,
                                  3.7636837254215877E-31,
                                  7.86208969853951E-32,
                                  1.6119316217296131E-32,
                                  3.2435248800046395E-33,
                                  6.4051263558591544E-34,
                                  1.2412391590605258E-34,
                                  2.3603563651085755E-35,
                                  4.4042368118951796E-36,
                                  8.0632652039343719E-37,
                                  1.4483523386704917E-37,
                                  2.5523244787623425E-38,
                                  4.412359411410817E-39,
                                  7.4826207506890865E-40,
                                  1.2446823537728815E-40,
                                  2.0307615085768141E-41,
                                  3.2495865359013786E-42,
                                  5.0996319384998625E-43,
                                  7.8480794836431912E-44,
                                  1.1843324196665874E-44,
                                  1.7524307627456715E-45,
                                  2.5423540535627805E-46,
                                  3.6160090768525E-47,
                                  5.0418651847724123E-48,
                                  6.8911325856857831E-49,
                                  9.2320163733026424E-50,
                                  1.2122090566881122E-50,
                                  1.5599166026809772E-51,
                                  1.9671410930063291E-52,
                                  2.4307861959584019E-53,
                                  2.9430757460922936E-54,
                                  3.4911242546307994E-55,
                                  4.0569865386059866E-56,
                                  4.6182864056923946E-57,
                                  5.1494549433336573E-58,
                                  5.6235201288669919E-59,
                                  6.0142950889387755E-60,
                                  6.2987300211220979E-61,
                                  6.4591403502493395E-62,
                                  6.4850149069772552E-63,
                                  6.3741488006201964E-64,
                                  6.1329322571257259E-65,
                                  5.7757451894915958E-66,
                                  5.3235365685471091E-67,
                                  4.8017839186506973E-68,
                                  4.2381102040517043E-69,
                                  3.6598690466899196E-70,
                                  3.0919909065741827E-71,
                                  2.555319431758472E-72,
                                  2.0655739210258145E-73,
                                  1.6329704055536695E-74,
                                  1.2624396324103161E-75,
                                  9.543102386068619E-77,
                                  7.0528773227679881E-78,
                                  5.09555124005607E-79,
                                  3.5984344699271217E-80,
                                  2.4836033985176E-81,
                                  1.675116025010869E-82,
                                  1.1039475545296668E-83,
                                  7.1078604734081873E-85,
                                  4.4705539339667612E-86,
                                  2.7463755907298595E-87,
                                  1.6476989742830769E-88,
                                  9.6528907321845511E-90,
                                  5.5212793682294206E-91,
                                  3.0829401149097932E-92,
                                  1.6802452408323033E-93,
                                  8.9371820282704583E-95,
                                  4.63858929247007E-96,
                                  2.3489001991433125E-97,
                                  1.1603028040237395E-98,
                                  5.590361651212764E-100,
                                  2.6266531130862764E-101,
                                  1.2033474888136632E-102,
                                  5.37446119311972E-104,
                                  2.3397157968009243E-105,
                                  9.9266577398352991E-107,
                                  4.1037499749188874E-108,
                                  1.6528042620581402E-109,
                                  6.4840750591385645E-111,
                                  2.4773233188835554E-112,
                                  9.2160730066782551E-114,
                                  3.3377722367805746E-115,
                                  1.1766100429798494E-116,
                                  4.0363552430349271E-118,
                                  1.3472292316525515E-119,
                                  4.37422938745714E-121,
                                  1.381276155959934E-122,
                                  4.2411888067168165E-124,
                                  1.2659899918468349E-125,
                                  3.6729424727803213E-127,
                                  1.0354828326618447E-128,
                                  2.8360795498575773E-130,
                                  7.5446737648131063E-132,
                                  1.9489776617124703E-133,
                                  4.8877984634372042E-135,
                                  1.1897433750618745E-136,
                                  2.8100769048093924E-138,
                                  6.43868348911871E-140,
                                  1.4307923919349155E-141,
                                  3.0827701617562363E-143,
                                  6.4383187705517252E-145,
                                  1.3030163935422225E-146,
                                  2.5547593740150775E-148,
                                  4.851173115522204E-150,
                                  8.9189008191308772E-152,
                                  1.5871279812984613E-153,
                                  2.7328381313657754E-155,
                                  4.5517609791187669E-157,
                                  7.331074642806808E-159,
                                  1.1413906258209838E-160,
                                  1.7172434795077103E-162,
                                  2.4957974469223561E-164,
                                  3.5027735418192149E-166,
                                  4.7454971390205261E-168,
                                  6.2037763115953714E-170,
                                  7.8229249111071221E-172,
                                  9.5115339280169867E-174,
                                  1.1146174271903174E-175,
                                  1.2583923293688117E-177,
                                  1.36816286807045E-179,
                                  1.4318675724153538E-181,
                                  1.4418435533766677E-183,
                                  1.3963217197226526E-185,
                                  1.2998746632451722E-187,
                                  1.1626719343866685E-189,
                                  9.9870686999273579E-192,
                                  8.2342402277226562E-194,
                                  6.5130895766611124E-196,
                                  4.9396432760995021E-198,
                                  3.5901243982250228E-200,
                                  2.4990919194626963E-202,
                                  1.6651755983875734E-204,
                                  1.0614063327136604E-206,
                                  6.468120107923361E-209,
                                  3.765928098240741E-211,
                                  2.0935232533126563E-213,
                                  1.1104544605482042E-215,
                                  5.6161281449478938E-218,
                                  2.7062813045760654E-220,
                                  1.2416087338154606E-222,
                                  5.419235879725281E-225,
                                  2.2484708488220131E-227,
                                  8.8608448346710029E-230,
                                  3.3138447665740858E-232,
                                  1.1751031086355713E-234,
                                  3.9473958706316243E-237,
                                  1.2549492809594679E-239,
                                  3.7722232125913105E-242,
                                  1.0709852507568606E-244,
                                  2.8689770232468313E-247,
                                  7.2435787059561339E-250,
                                  1.7217432688947212E-252,
                                  3.8482184588614045E-255,
                                  8.0778074293287184E-258,
                                  1.5904285600930612E-260,
                                  2.9332086567936636E-263,
                                  5.0603023737840587E-266,
                                  8.1542707273859118E-269,
                                  1.2254949596535739E-271,
                                  1.7150178349799451E-274,
                                  2.2311965382288727E-277,
                                  2.6938085504526008E-280,
                                  3.0127765470970308E-283,
                                  3.1153826608545815E-286,
                                  2.9725677915315415E-289,
                                  2.6116514685991307E-292,
                                  2.1081483582783834E-295,
                                  1.5598311458043031E-298,
                                  1.055302375860759E-301,
                                  6.5113581680100867E-305,
                                  3.6540040795315211E-308,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0,
                                  0.0};
  memcpy(&gausslaguerreNodes[0], &dv[0], 300U * sizeof(real_T));
  memcpy(&gausslaguerreWeights[0], &dv1[0], 300U * sizeof(real_T));
}

/* End of code generation (pfqn_mmint2_gausslaguerre.c) */
