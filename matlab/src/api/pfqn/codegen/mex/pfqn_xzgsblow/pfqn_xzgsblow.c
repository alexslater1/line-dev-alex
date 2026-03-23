/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_xzgsblow.c
 *
 * Code generation for function 'pfqn_xzgsblow'
 *
 */

/* Include files */
#include "pfqn_xzgsblow.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_xzgsblow_data.h"
#include "pfqn_xzgsblow_types.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "mwmathutil.h"

/* Variable Definitions */
static emlrtRSInfo emlrtRSI = {
    20,              /* lineNo */
    "pfqn_xzgsblow", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_xzgsblow.m" /* pathName */
};

static emlrtRSInfo b_emlrtRSI = {
    21,              /* lineNo */
    "pfqn_xzgsblow", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_xzgsblow.m" /* pathName */
};

static emlrtRSInfo c_emlrtRSI = {
    24,              /* lineNo */
    "pfqn_xzgsblow", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_xzgsblow.m" /* pathName */
};

static emlrtRSInfo d_emlrtRSI = {
    27,              /* lineNo */
    "pfqn_xzgsblow", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_xzgsblow.m" /* pathName */
};

static emlrtRSInfo e_emlrtRSI = {
    31,              /* lineNo */
    "pfqn_xzgsblow", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_xzgsblow.m" /* pathName */
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
        20,             /* lineNo */
        "pfqn_qzgblow", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_qzgblow.m" /* pathName */
};

static emlrtRSInfo r_emlrtRSI =
    {
        21,             /* lineNo */
        "pfqn_qzgblow", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_qzgblow.m" /* pathName */
};

static emlrtRSInfo s_emlrtRSI = {
    44,       /* lineNo */
    "mpower", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/matfun/mpower.m" /* pathName
                                                                       */
};

static emlrtRSInfo t_emlrtRSI = {
    71,                                                           /* lineNo */
    "power",                                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

static emlrtBCInfo emlrtBCI = {
    -1,              /* iFirst */
    -1,              /* iLast */
    24,              /* lineNo */
    16,              /* colNo */
    "L",             /* aName */
    "pfqn_xzgsblow", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_xzgsblow.m", /* pName */
    0                  /* checkKind */
};

static emlrtBCInfo b_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    20,             /* lineNo */
    8,              /* colNo */
    "L",            /* aName */
    "pfqn_qzgblow", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_qzgblow.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo c_emlrtBCI = {
    -1,              /* iFirst */
    -1,              /* iLast */
    23,              /* lineNo */
    10,              /* colNo */
    "L",             /* aName */
    "pfqn_xzgsblow", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_xzgsblow.m", /* pName */
    0                  /* checkKind */
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

/* Function Definitions */
real_T pfqn_xzgsblow(const emlrtStack *sp, const emxArray_real_T *L, real_T N,
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
  emlrtStack st;
  const real_T *L_data;
  real_T Lmax;
  real_T R;
  real_T X;
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
  L_data = L->data;
  /* { */
  /*  % @file pfqn_xzgsblow.m */
  /*  % @brief Lower asymptotic bound on throughput
   * (Zahorjan-Gittelsohn-Schweitzer-Bryant). */
  /* } */
  /* { */
  /*  % @brief Lower asymptotic bound on throughput
   * (Zahorjan-Gittelsohn-Schweitzer-Bryant). */
  /*  % @fn pfqn_xzgsblow(L, N, Z) */
  /*  % @param L Service demand vector. */
  /*  % @param N Population. */
  /*  % @param Z Think time. */
  /*  % @return X Lower bound on throughput. */
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
  for (ist = 0; ist < last; ist++) {
    real_T d;
    if (ist + 1 > last) {
      emlrtDynamicBoundsCheckR2012b(ist + 1, 1, last, &c_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    d = L_data[ist];
    if (d < Lmax) {
      st.site = &c_emlrtRSI;
      /* { */
      /*  % @file pfqn_qzgblow.m */
      /*  % @brief Lower asymptotic bound on queue length
       * (Zahorjan-Gittelsohn-Bryant). */
      /* } */
      /* { */
      /*  % @brief Lower asymptotic bound on queue length
       * (Zahorjan-Gittelsohn-Bryant). */
      /*  % @fn pfqn_qzgblow(L, N, Z, i) */
      /*  % @param L Service demand vector. */
      /*  % @param N Population. */
      /*  % @param Z Think time. */
      /*  % @param i Station index. */
      /*  % @return Qgb Lower bound on mean queue length at station i. */
      /* } */
      b_st.site = &q_emlrtRSI;
      c_st.site = &f_emlrtRSI;
      d_st.site = &g_emlrtRSI;
      e_st.site = &h_emlrtRSI;
      f_st.site = &i_emlrtRSI;
      g_st.site = &j_emlrtRSI;
      if (L->size[1] <= 2) {
        if (L->size[1] == 1) {
          X = L_data[0];
        } else if ((L_data[0] < L_data[1]) ||
                   (muDoubleScalarIsNaN(L_data[0]) &&
                    (!muDoubleScalarIsNaN(L_data[1])))) {
          X = L_data[1];
        } else {
          X = L_data[0];
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
          X = L_data[0];
        } else {
          h_st.site = &k_emlrtRSI;
          X = L_data[idx - 1];
          a = idx + 1;
          i_st.site = &o_emlrtRSI;
          if ((idx + 1 <= L->size[1]) && (L->size[1] > 2147483646)) {
            j_st.site = &n_emlrtRSI;
            check_forloop_overflow_error(&j_st);
          }
          for (k = a; k <= last; k++) {
            real_T d1;
            d1 = L_data[k - 1];
            if (X < d1) {
              X = d1;
            }
          }
        }
      }
      if (ist + 1 > last) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, last, &b_emlrtBCI, &st);
      }
      b_st.site = &q_emlrtRSI;
      X = (N - 1.0) * d / ((Z + sum(L)) + X * (N - 1.0));
      b_st.site = &r_emlrtRSI;
      c_st.site = &s_emlrtRSI;
      d_st.site = &t_emlrtRSI;
      if ((X < 0.0) && (!muDoubleScalarIsNaN((N - 1.0) + 1.0)) &&
          (muDoubleScalarFloor((N - 1.0) + 1.0) != (N - 1.0) + 1.0)) {
        emlrtErrorWithMessageIdR2018a(&d_st, &c_emlrtRTEI,
                                      "Coder:toolbox:power_domainError",
                                      "Coder:toolbox:power_domainError", 0);
      }
      if (ist + 1 > last) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, last, &emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      R += (d - Lmax) * (X / (1.0 - X) -
                         muDoubleScalarPower(X, (N - 1.0) + 1.0) / (1.0 - X));
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  st.site = &d_emlrtRSI;
  b_st.site = &s_emlrtRSI;
  c_st.site = &t_emlrtRSI;
  X = R * R - 4.0 * Z * Lmax * (N - 1.0);
  if (X < 0.0) {
    X = 0.0;
  }
  st.site = &e_emlrtRSI;
  return 2.0 * N * (1.0 / (R + muDoubleScalarSqrt(X)));
}

/* End of code generation (pfqn_xzgsblow.c) */
