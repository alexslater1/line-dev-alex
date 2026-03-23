/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sum.c
 *
 * Code generation for function 'sum'
 *
 */

/* Include files */
#include "sum.h"
#include "pfqn_mmint2_gausslegendre_types.h"
#include "rt_nonfinite.h"
#include "sumMatrixIncludeNaN.h"

/* Variable Definitions */
static emlrtRSInfo g_emlrtRSI = {
    20,    /* lineNo */
    "sum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sum.m" /* pathName
                                                                     */
};

static emlrtRSInfo h_emlrtRSI =
    {
        99,        /* lineNo */
        "sumprod", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
        "sumprod.m" /* pathName */
};

static emlrtRSInfo i_emlrtRSI = {
    86,                      /* lineNo */
    "combineVectorElements", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "combineVectorElements.m" /* pathName */
};

static emlrtRSInfo j_emlrtRSI = {
    99,                 /* lineNo */
    "blockedSummation", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo k_emlrtRSI = {
    22,                    /* lineNo */
    "sumMatrixIncludeNaN", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtRSInfo l_emlrtRSI = {
    42,                 /* lineNo */
    "sumMatrixColumns", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtRSInfo m_emlrtRSI = {
    57,                 /* lineNo */
    "sumMatrixColumns", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtRSInfo hb_emlrtRSI = {
    107,                /* lineNo */
    "blockedSummation", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

/* Function Definitions */
real_T b_sum(const emlrtStack *sp, const emxArray_real_T *x)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack st;
  real_T y;
  int32_T ib;
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
  st.site = &g_emlrtRSI;
  b_st.site = &h_emlrtRSI;
  c_st.site = &i_emlrtRSI;
  if (x->size[0] == 0) {
    y = 0.0;
  } else {
    d_st.site = &hb_emlrtRSI;
    e_st.site = &k_emlrtRSI;
    if (x->size[0] < 4096) {
      f_st.site = &l_emlrtRSI;
      y = sumColumnB(&f_st, x, x->size[0]);
    } else {
      int32_T inb;
      int32_T nfb;
      int32_T nleft;
      nfb = (uint16_T)x->size[0] >> 12;
      inb = nfb << 12;
      nleft = x->size[0] - inb;
      y = sumColumnB4(x, 1);
      for (ib = 2; ib <= nfb; ib++) {
        y += sumColumnB4(x, ((ib - 1) << 12) + 1);
      }
      if (nleft > 0) {
        f_st.site = &m_emlrtRSI;
        y += b_sumColumnB(&f_st, x, nleft, inb + 1);
      }
    }
  }
  return y;
}

real_T sum(const emlrtStack *sp, const emxArray_real_T *x)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack st;
  emxArray_real_T b_x;
  real_T y;
  int32_T c_x;
  int32_T d_x;
  int32_T e_x;
  int32_T f_x;
  int32_T ib;
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
  st.site = &g_emlrtRSI;
  b_st.site = &h_emlrtRSI;
  c_st.site = &i_emlrtRSI;
  if (x->size[1] == 0) {
    y = 0.0;
  } else {
    d_st.site = &j_emlrtRSI;
    e_st.site = &k_emlrtRSI;
    if (x->size[1] < 4096) {
      b_x = *x;
      c_x = x->size[1];
      b_x.size = &c_x;
      b_x.numDimensions = 1;
      f_st.site = &l_emlrtRSI;
      y = sumColumnB(&f_st, &b_x, x->size[1]);
    } else {
      int32_T inb;
      int32_T nfb;
      int32_T nleft;
      nfb = (int32_T)((uint32_T)x->size[1] >> 12);
      inb = nfb << 12;
      nleft = x->size[1] - inb;
      b_x = *x;
      d_x = x->size[1];
      b_x.size = &d_x;
      b_x.numDimensions = 1;
      y = sumColumnB4(&b_x, 1);
      for (ib = 2; ib <= nfb; ib++) {
        b_x = *x;
        e_x = x->size[1];
        b_x.size = &e_x;
        b_x.numDimensions = 1;
        y += sumColumnB4(&b_x, ((ib - 1) << 12) + 1);
      }
      if (nleft > 0) {
        b_x = *x;
        f_x = x->size[1];
        b_x.size = &f_x;
        b_x.numDimensions = 1;
        f_st.site = &m_emlrtRSI;
        y += b_sumColumnB(&f_st, &b_x, nleft, inb + 1);
      }
    }
  }
  return y;
}

/* End of code generation (sum.c) */
