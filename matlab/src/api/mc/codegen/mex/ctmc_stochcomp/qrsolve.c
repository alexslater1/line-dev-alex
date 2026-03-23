/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * qrsolve.c
 *
 * Code generation for function 'qrsolve'
 *
 */

/* Include files */
#include "qrsolve.h"
#include "ctmc_stochcomp_data.h"
#include "ctmc_stochcomp_emxutil.h"
#include "ctmc_stochcomp_types.h"
#include "eml_int_forloop_overflow_check.h"
#include "rt_nonfinite.h"
#include "lapacke.h"
#include "mwmathutil.h"
#include <emmintrin.h>
#include <stddef.h>

/* Variable Definitions */
static emlrtRSInfo db_emlrtRSI = {
    119,         /* lineNo */
    "LSQFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo eb_emlrtRSI = {
    126,         /* lineNo */
    "LSQFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo fb_emlrtRSI = {
    128,         /* lineNo */
    "LSQFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo gb_emlrtRSI = {
    138,         /* lineNo */
    "LSQFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo hb_emlrtRSI = {
    40,         /* lineNo */
    "xunormqr", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xunormqr.m" /* pathName */
};

static emlrtRSInfo ib_emlrtRSI = {
    106,              /* lineNo */
    "ceval_xunormqr", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xunormqr.m" /* pathName */
};

static emlrtRSInfo jb_emlrtRSI = {
    112,              /* lineNo */
    "ceval_xunormqr", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xunormqr.m" /* pathName */
};

static emlrtRSInfo kb_emlrtRSI = {
    21,          /* lineNo */
    "xzunormqr", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+reflapack/"
    "xzunormqr.m" /* pathName */
};

static emlrtRSInfo lb_emlrtRSI = {
    23,          /* lineNo */
    "xzunormqr", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+reflapack/"
    "xzunormqr.m" /* pathName */
};

static emlrtRSInfo mb_emlrtRSI = {
    38,                    /* lineNo */
    "applyJthHouseHolder", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+reflapack/"
    "xzunormqr.m" /* pathName */
};

static emlrtRSInfo nb_emlrtRSI = {
    40,                    /* lineNo */
    "applyJthHouseHolder", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+reflapack/"
    "xzunormqr.m" /* pathName */
};

static emlrtRSInfo ob_emlrtRSI = {
    46,                    /* lineNo */
    "applyJthHouseHolder", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+reflapack/"
    "xzunormqr.m" /* pathName */
};

static emlrtRTEInfo y_emlrtRTEI = {
    109,       /* lineNo */
    1,         /* colNo */
    "qrsolve", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pName
                                                                           */
};

static emlrtRTEInfo ab_emlrtRTEI = {
    119,       /* lineNo */
    5,         /* colNo */
    "qrsolve", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pName
                                                                           */
};

/* Function Definitions */
void LSQFromQR(const emlrtStack *sp, const emxArray_real_T *A,
               const emxArray_real_T *tau, const emxArray_int32_T *jpvt,
               emxArray_real_T *B, int32_T rankA, emxArray_real_T *Y)
{
  static const char_T fname[14] = {'L', 'A', 'P', 'A', 'C', 'K', 'E',
                                   '_', 'd', 'o', 'r', 'm', 'q', 'r'};
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack st;
  const real_T *A_data;
  const real_T *tau_data;
  real_T *B_data;
  real_T *Y_data;
  const int32_T *jpvt_data;
  int32_T c_i;
  int32_T i;
  int32_T j;
  int32_T k;
  int32_T loop_ub;
  int32_T nb;
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
  B_data = B->data;
  jpvt_data = jpvt->data;
  tau_data = tau->data;
  A_data = A->data;
  nb = B->size[1];
  loop_ub = Y->size[0] * Y->size[1];
  Y->size[0] = A->size[1];
  Y->size[1] = B->size[1];
  emxEnsureCapacity_real_T(sp, Y, loop_ub, &y_emlrtRTEI);
  Y_data = Y->data;
  loop_ub = A->size[1] * B->size[1];
  for (i = 0; i < loop_ub; i++) {
    Y_data[i] = 0.0;
  }
  st.site = &db_emlrtRSI;
  b_st.site = &hb_emlrtRSI;
  if ((A->size[0] != 0) && (A->size[1] != 0) &&
      ((B->size[0] != 0) && (B->size[1] != 0))) {
    ptrdiff_t nrc_t;
    boolean_T overflow;
    nrc_t = (ptrdiff_t)B->size[0];
    nrc_t =
        LAPACKE_dormqr(102, 'L', 'T', nrc_t, (ptrdiff_t)B->size[1],
                       (ptrdiff_t)muIntScalarMin_sint32(A->size[0], A->size[1]),
                       (real_T *)&A_data[0], (ptrdiff_t)A->size[0],
                       (real_T *)&tau_data[0], &B_data[0], nrc_t);
    c_st.site = &ib_emlrtRSI;
    if ((int32_T)nrc_t != 0) {
      boolean_T p;
      overflow = true;
      p = false;
      if ((int32_T)nrc_t == -7) {
        p = true;
      } else if ((int32_T)nrc_t == -9) {
        p = true;
      } else if ((int32_T)nrc_t == -10) {
        p = true;
      }
      if (!p) {
        if ((int32_T)nrc_t == -1010) {
          emlrtErrorWithMessageIdR2018a(&c_st, &b_emlrtRTEI, "MATLAB:nomem",
                                        "MATLAB:nomem", 0);
        } else {
          emlrtErrorWithMessageIdR2018a(&c_st, &emlrtRTEI,
                                        "Coder:toolbox:LAPACKCallErrorInfo",
                                        "Coder:toolbox:LAPACKCallErrorInfo", 5,
                                        4, 14, &fname[0], 12, (int32_T)nrc_t);
        }
      }
    } else {
      overflow = false;
    }
    if (overflow) {
      if (((int32_T)nrc_t == -10) && (B->size[1] > 1)) {
        int32_T m;
        int32_T mn;
        c_st.site = &jb_emlrtRSI;
        mn = muIntScalarMin_sint32(A->size[0], A->size[1]);
        d_st.site = &kb_emlrtRSI;
        if (mn > 2147483646) {
          e_st.site = &n_emlrtRSI;
          check_forloop_overflow_error(&e_st);
        }
        m = A->size[0];
        for (j = 0; j < mn; j++) {
          int32_T b_i;
          d_st.site = &lb_emlrtRSI;
          b_i = B->size[1];
          if (tau_data[j] != 0.0) {
            int32_T a;
            e_st.site = &mb_emlrtRSI;
            if (B->size[1] > 2147483646) {
              f_st.site = &n_emlrtRSI;
              check_forloop_overflow_error(&f_st);
            }
            a = j + 2;
            overflow = ((j + 2 <= m) && (m > 2147483646));
            for (k = 0; k < b_i; k++) {
              real_T wj;
              wj = B_data[j + B->size[0] * k];
              e_st.site = &nb_emlrtRSI;
              if (overflow) {
                f_st.site = &n_emlrtRSI;
                check_forloop_overflow_error(&f_st);
              }
              for (i = a; i <= m; i++) {
                wj += A_data[(i + A->size[0] * j) - 1] *
                      B_data[(i + B->size[0] * k) - 1];
              }
              wj *= tau_data[j];
              if (wj != 0.0) {
                int32_T scalarLB;
                int32_T vectorUB;
                B_data[j + B->size[0] * k] -= wj;
                loop_ub = j + 2;
                e_st.site = &ob_emlrtRSI;
                scalarLB = (((((m - j) - 1) / 2) << 1) + j) + 2;
                vectorUB = scalarLB - 2;
                for (c_i = loop_ub; c_i <= vectorUB; c_i += 2) {
                  __m128d r;
                  r = _mm_loadu_pd(&B_data[(c_i + B->size[0] * k) - 1]);
                  _mm_storeu_pd(
                      &B_data[(c_i + B->size[0] * k) - 1],
                      _mm_sub_pd(
                          r,
                          _mm_mul_pd(
                              _mm_loadu_pd(&A_data[(c_i + A->size[0] * j) - 1]),
                              _mm_set1_pd(wj))));
                }
                for (c_i = scalarLB; c_i <= m; c_i++) {
                  B_data[(c_i + B->size[0] * k) - 1] -=
                      A_data[(c_i + A->size[0] * j) - 1] * wj;
                }
              }
            }
          }
        }
      } else {
        int32_T scalarLB;
        int32_T vectorUB;
        loop_ub = B->size[0];
        scalarLB = B->size[1];
        vectorUB = B->size[0] * B->size[1];
        B->size[0] = loop_ub;
        B->size[1] = scalarLB;
        emxEnsureCapacity_real_T(&b_st, B, vectorUB, &ab_emlrtRTEI);
        B_data = B->data;
        loop_ub *= scalarLB;
        for (i = 0; i < loop_ub; i++) {
          B_data[i] = rtNaN;
        }
      }
    }
  }
  st.site = &eb_emlrtRSI;
  if (nb > 2147483646) {
    b_st.site = &n_emlrtRSI;
    check_forloop_overflow_error(&b_st);
  }
  for (j = 0; j < nb; j++) {
    st.site = &fb_emlrtRSI;
    if (rankA > 2147483646) {
      b_st.site = &n_emlrtRSI;
      check_forloop_overflow_error(&b_st);
    }
    for (i = 0; i < rankA; i++) {
      Y_data[(jpvt_data[i] + Y->size[0] * j) - 1] = B_data[i + B->size[0] * j];
    }
    for (i = rankA; i >= 1; i--) {
      loop_ub = jpvt_data[i - 1];
      Y_data[(loop_ub + Y->size[0] * j) - 1] /=
          A_data[(i + A->size[0] * (i - 1)) - 1];
      st.site = &gb_emlrtRSI;
      for (c_i = 0; c_i <= i - 2; c_i++) {
        Y_data[(jpvt_data[c_i] + Y->size[0] * j) - 1] -=
            Y_data[(loop_ub + Y->size[0] * j) - 1] *
            A_data[c_i + A->size[0] * (i - 1)];
      }
    }
  }
}

/* End of code generation (qrsolve.c) */
