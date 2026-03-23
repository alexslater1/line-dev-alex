/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_xzgsbup.c
 *
 * Code generation for function 'pfqn_xzgsbup'
 *
 */

/* Include files */
#include "pfqn_xzgsbup.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_xzgsbup_data.h"
#include "pfqn_xzgsbup_emxutil.h"
#include "pfqn_xzgsbup_types.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "mwmathutil.h"

/* Variable Definitions */
static emlrtRSInfo emlrtRSI =
    {
        20,             /* lineNo */
        "pfqn_xzgsbup", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_xzgsbup.m" /* pathName */
};

static emlrtRSInfo b_emlrtRSI =
    {
        21,             /* lineNo */
        "pfqn_xzgsbup", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_xzgsbup.m" /* pathName */
};

static emlrtRSInfo c_emlrtRSI =
    {
        24,             /* lineNo */
        "pfqn_xzgsbup", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_xzgsbup.m" /* pathName */
};

static emlrtRSInfo d_emlrtRSI =
    {
        27,             /* lineNo */
        "pfqn_xzgsbup", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_xzgsbup.m" /* pathName */
};

static emlrtRSInfo e_emlrtRSI =
    {
        31,             /* lineNo */
        "pfqn_xzgsbup", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_xzgsbup.m" /* pathName */
};

static emlrtRSInfo f_emlrtRSI = {
    15,    /* lineNo */
    "max", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/max.m" /* pathName
                                                                     */
};

static emlrtRSInfo g_emlrtRSI = {
    73,         /* lineNo */
    "minOrMax", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

static emlrtRSInfo h_emlrtRSI = {
    108,       /* lineNo */
    "maximum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

static emlrtRSInfo i_emlrtRSI =
    {
        255,             /* lineNo */
        "unaryMinOrMax", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo j_emlrtRSI =
    {
        966,                    /* lineNo */
        "maxRealVectorOmitNaN", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo k_emlrtRSI = {
    73,                      /* lineNo */
    "vectorMinOrMaxInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "vectorMinOrMaxInPlace.m" /* pathName */
};

static emlrtRSInfo l_emlrtRSI = {
    65,                      /* lineNo */
    "vectorMinOrMaxInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "vectorMinOrMaxInPlace.m" /* pathName */
};

static emlrtRSInfo m_emlrtRSI = {
    114,         /* lineNo */
    "findFirst", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "vectorMinOrMaxInPlace.m" /* pathName */
};

static emlrtRSInfo n_emlrtRSI = {
    20,                               /* lineNo */
    "eml_int_forloop_overflow_check", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/eml/"
    "eml_int_forloop_overflow_check.m" /* pathName */
};

static emlrtRSInfo o_emlrtRSI = {
    131,                        /* lineNo */
    "minOrMaxRealVectorKernel", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "vectorMinOrMaxInPlace.m" /* pathName */
};

static emlrtRSInfo q_emlrtRSI =
    {
        20,            /* lineNo */
        "pfqn_qzgbup", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_qzgbup.m" /* pathName */
};

static emlrtRSInfo r_emlrtRSI =
    {
        21,            /* lineNo */
        "pfqn_qzgbup", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_qzgbup.m" /* pathName */
};

static emlrtRSInfo s_emlrtRSI =
    {
        23,            /* lineNo */
        "pfqn_qzgbup", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_qzgbup.m" /* pathName */
};

static emlrtRSInfo t_emlrtRSI = {
    71,                                                           /* lineNo */
    "power",                                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

static emlrtRSInfo u_emlrtRSI =
    {
        19,             /* lineNo */
        "pfqn_xzabaup", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_xzabaup.m" /* pathName */
};

static emlrtRSInfo v_emlrtRSI = {
    44,       /* lineNo */
    "mpower", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/matfun/mpower.m" /* pathName
                                                                       */
};

static emlrtBCInfo emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    24,             /* lineNo */
    16,             /* colNo */
    "L",            /* aName */
    "pfqn_xzgsbup", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_xzgsbup.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo b_emlrtBCI =
    {
        -1,            /* iFirst */
        -1,            /* iLast */
        21,            /* lineNo */
        6,             /* colNo */
        "L",           /* aName */
        "pfqn_qzgbup", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_qzgbup.m", /* pName */
        0                /* checkKind */
};

static emlrtBCInfo c_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    23,             /* lineNo */
    10,             /* colNo */
    "L",            /* aName */
    "pfqn_xzgsbup", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_xzgsbup.m", /* pName */
    0                 /* checkKind */
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
    82,                                                           /* lineNo */
    5,                                                            /* colNo */
    "fltpower",                                                   /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pName */
};

static emlrtRTEInfo f_emlrtRTEI =
    {
        31,            /* lineNo */
        30,            /* colNo */
        "unsafeSxfun", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unsafeSxfun.m" /* pName */
};

/* Function Definitions */
real_T pfqn_xzgsbup(const emlrtStack *sp, const emxArray_real_T *L, real_T N,
                    real_T Z)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack g_st;
  emlrtStack h_st;
  emlrtStack i_st;
  emlrtStack j_st;
  emlrtStack k_st;
  emlrtStack st;
  emxArray_real_T *r;
  const real_T *L_data;
  real_T Lmax;
  real_T R;
  real_T X;
  real_T *r1;
  int32_T a;
  int32_T idx;
  int32_T ist;
  int32_T k;
  int32_T last;
  boolean_T exitg1;
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
  j_st.prev = &i_st;
  j_st.tls = i_st.tls;
  k_st.prev = &j_st;
  k_st.tls = j_st.tls;
  L_data = L->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /* { */
  /*  % @file pfqn_xzgsbup.m */
  /*  % @brief Upper asymptotic bound on throughput
   * (Zahorjan-Gittelsohn-Schweitzer-Bryant). */
  /* } */
  /* { */
  /*  % @brief Upper asymptotic bound on throughput
   * (Zahorjan-Gittelsohn-Schweitzer-Bryant). */
  /*  % @fn pfqn_xzgsbup(L, N, Z) */
  /*  % @param L Service demand vector. */
  /*  % @param N Population. */
  /*  % @param Z Think time. */
  /*  % @return X Upper bound on throughput. */
  /* } */
  st.site = &emlrtRSI;
  b_st.site = &f_emlrtRSI;
  c_st.site = &g_emlrtRSI;
  d_st.site = &h_emlrtRSI;
  if (L->size[1] < 1) {
    emlrtErrorWithMessageIdR2018a(&d_st, &b_emlrtRTEI,
                                  "Coder:toolbox:eml_min_or_max_varDimZero",
                                  "Coder:toolbox:eml_min_or_max_varDimZero", 0);
  }
  e_st.site = &i_emlrtRSI;
  f_st.site = &j_emlrtRSI;
  last = L->size[1];
  if (L->size[1] <= 2) {
    if (L->size[1] == 1) {
      Lmax = L_data[0];
    } else if ((L_data[0] < L_data[1]) || (muDoubleScalarIsNaN(L_data[0]) &&
                                           (!muDoubleScalarIsNaN(L_data[1])))) {
      Lmax = L_data[1];
    } else {
      Lmax = L_data[0];
    }
  } else {
    g_st.site = &l_emlrtRSI;
    if (!muDoubleScalarIsNaN(L_data[0])) {
      idx = 1;
    } else {
      idx = 0;
      h_st.site = &m_emlrtRSI;
      if (L->size[1] > 2147483646) {
        i_st.site = &n_emlrtRSI;
        check_forloop_overflow_error(&i_st);
      }
      a = 2;
      exitg1 = false;
      while ((!exitg1) && (a <= last)) {
        if (!muDoubleScalarIsNaN(L_data[a - 1])) {
          idx = a;
          exitg1 = true;
        } else {
          a++;
        }
      }
    }
    if (idx == 0) {
      Lmax = L_data[0];
    } else {
      g_st.site = &k_emlrtRSI;
      Lmax = L_data[idx - 1];
      a = idx + 1;
      h_st.site = &o_emlrtRSI;
      if ((idx + 1 <= L->size[1]) && (L->size[1] > 2147483646)) {
        i_st.site = &n_emlrtRSI;
        check_forloop_overflow_error(&i_st);
      }
      for (k = a; k <= last; k++) {
        X = L_data[k - 1];
        if (Lmax < X) {
          Lmax = X;
        }
      }
    }
  }
  st.site = &b_emlrtRSI;
  R = (Z + sum(L)) + Lmax * (N - 1.0);
  emxInit_real_T(sp, &r, &f_emlrtRTEI);
  for (ist = 0; ist < last; ist++) {
    real_T d;
    if (ist + 1 > last) {
      emlrtDynamicBoundsCheckR2012b(ist + 1, 1, last, &c_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    d = L_data[ist];
    if (d < Lmax) {
      real_T b_ex;
      real_T ex;
      real_T sigma;
      real_T varargin_1_idx_0;
      st.site = &c_emlrtRSI;
      /* { */
      /*  % @file pfqn_qzgbup.m */
      /*  % @brief Upper asymptotic bound on queue length
       * (Zahorjan-Gittelsohn-Bryant). */
      /* } */
      /* { */
      /*  % @brief Upper asymptotic bound on queue length
       * (Zahorjan-Gittelsohn-Bryant). */
      /*  % @fn pfqn_qzgbup(L, N, Z, i) */
      /*  % @param L Service demand vector. */
      /*  % @param N Population. */
      /*  % @param Z Think time. */
      /*  % @param i Station index. */
      /*  % @return Qgb Upper bound on mean queue length at station i. */
      /* } */
      b_st.site = &q_emlrtRSI;
      c_st.site = &t_emlrtRSI;
      a = r->size[0] * r->size[1];
      r->size[0] = 1;
      r->size[1] = last;
      emxEnsureCapacity_real_T(&st, r, a, &f_emlrtRTEI);
      r1 = r->data;
      for (k = 0; k < last; k++) {
        X = L_data[k];
        r1[k] = X * X;
      }
      b_st.site = &q_emlrtRSI;
      sigma = sum(r) / sum(L);
      b_st.site = &r_emlrtRSI;
      c_st.site = &f_emlrtRSI;
      d_st.site = &g_emlrtRSI;
      e_st.site = &h_emlrtRSI;
      f_st.site = &i_emlrtRSI;
      g_st.site = &j_emlrtRSI;
      if (L->size[1] <= 2) {
        if (L->size[1] == 1) {
          ex = L_data[0];
        } else if ((L_data[0] < L_data[1]) ||
                   (muDoubleScalarIsNaN(L_data[0]) &&
                    (!muDoubleScalarIsNaN(L_data[1])))) {
          ex = L_data[1];
        } else {
          ex = L_data[0];
        }
      } else {
        h_st.site = &l_emlrtRSI;
        if (!muDoubleScalarIsNaN(L_data[0])) {
          idx = 1;
        } else {
          idx = 0;
          i_st.site = &m_emlrtRSI;
          if (L->size[1] > 2147483646) {
            j_st.site = &n_emlrtRSI;
            check_forloop_overflow_error(&j_st);
          }
          a = 2;
          exitg1 = false;
          while ((!exitg1) && (a <= last)) {
            if (!muDoubleScalarIsNaN(L_data[a - 1])) {
              idx = a;
              exitg1 = true;
            } else {
              a++;
            }
          }
        }
        if (idx == 0) {
          ex = L_data[0];
        } else {
          h_st.site = &k_emlrtRSI;
          ex = L_data[idx - 1];
          a = idx + 1;
          i_st.site = &o_emlrtRSI;
          if ((idx + 1 <= L->size[1]) && (L->size[1] > 2147483646)) {
            j_st.site = &n_emlrtRSI;
            check_forloop_overflow_error(&j_st);
          }
          for (k = a; k <= last; k++) {
            X = L_data[k - 1];
            if (ex < X) {
              ex = X;
            }
          }
        }
      }
      b_st.site = &r_emlrtRSI;
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
      c_st.site = &u_emlrtRSI;
      d_st.site = &f_emlrtRSI;
      e_st.site = &g_emlrtRSI;
      f_st.site = &h_emlrtRSI;
      g_st.site = &i_emlrtRSI;
      h_st.site = &j_emlrtRSI;
      if (L->size[1] <= 2) {
        if (L->size[1] == 1) {
          b_ex = L_data[0];
        } else if ((L_data[0] < L_data[1]) ||
                   (muDoubleScalarIsNaN(L_data[0]) &&
                    (!muDoubleScalarIsNaN(L_data[1])))) {
          b_ex = L_data[1];
        } else {
          b_ex = L_data[0];
        }
      } else {
        i_st.site = &l_emlrtRSI;
        if (!muDoubleScalarIsNaN(L_data[0])) {
          idx = 1;
        } else {
          idx = 0;
          j_st.site = &m_emlrtRSI;
          if (L->size[1] > 2147483646) {
            k_st.site = &n_emlrtRSI;
            check_forloop_overflow_error(&k_st);
          }
          a = 2;
          exitg1 = false;
          while ((!exitg1) && (a <= last)) {
            if (!muDoubleScalarIsNaN(L_data[a - 1])) {
              idx = a;
              exitg1 = true;
            } else {
              a++;
            }
          }
        }
        if (idx == 0) {
          b_ex = L_data[0];
        } else {
          i_st.site = &k_emlrtRSI;
          b_ex = L_data[idx - 1];
          a = idx + 1;
          j_st.site = &o_emlrtRSI;
          if ((idx + 1 <= L->size[1]) && (L->size[1] > 2147483646)) {
            k_st.site = &n_emlrtRSI;
            check_forloop_overflow_error(&k_st);
          }
          for (k = a; k <= last; k++) {
            X = L_data[k - 1];
            if (b_ex < X) {
              b_ex = X;
            }
          }
        }
      }
      c_st.site = &u_emlrtRSI;
      X = sum(L);
      varargin_1_idx_0 = 1.0 / b_ex;
      X = ((N - 1.0) - 1.0) / (X + Z);
      if ((!(varargin_1_idx_0 > X)) &&
          ((!muDoubleScalarIsNaN(varargin_1_idx_0)) ||
           muDoubleScalarIsNaN(X))) {
        X = varargin_1_idx_0;
      }
      b_st.site = &r_emlrtRSI;
      b_ex = sum(L);
      varargin_1_idx_0 = 1.0 / ex;
      X = (N - 1.0) / ((Z + b_ex) + sigma * (((N - 1.0) - 1.0) - Z * X));
      if ((!(varargin_1_idx_0 > X)) &&
          ((!muDoubleScalarIsNaN(varargin_1_idx_0)) ||
           muDoubleScalarIsNaN(X))) {
        X = varargin_1_idx_0;
      }
      if (ist + 1 > last) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, last, &b_emlrtBCI, &b_st);
      }
      X *= d;
      if (X < 1.0) {
        c_st.site = &s_emlrtRSI;
        d_st.site = &v_emlrtRSI;
        e_st.site = &t_emlrtRSI;
        if ((X < 0.0) && (!muDoubleScalarIsNaN((N - 1.0) + 1.0)) &&
            (muDoubleScalarFloor((N - 1.0) + 1.0) != (N - 1.0) + 1.0)) {
          emlrtErrorWithMessageIdR2018a(&e_st, &c_emlrtRTEI,
                                        "Coder:toolbox:power_domainError",
                                        "Coder:toolbox:power_domainError", 0);
        }
        X = X / (1.0 - X) - muDoubleScalarPower(X, (N - 1.0) + 1.0) / (1.0 - X);
      } else {
        X = N - 1.0;
      }
      if (ist + 1 > last) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, last, &emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      R += (d - Lmax) * X;
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  emxFree_real_T(sp, &r);
  st.site = &d_emlrtRSI;
  b_st.site = &v_emlrtRSI;
  c_st.site = &t_emlrtRSI;
  X = R * R - 4.0 * Z * Lmax * N;
  if (X < 0.0) {
    X = 0.0;
  }
  st.site = &e_emlrtRSI;
  X = 2.0 * N / (R + muDoubleScalarSqrt(X));
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return X;
}

/* End of code generation (pfqn_xzgsbup.c) */
