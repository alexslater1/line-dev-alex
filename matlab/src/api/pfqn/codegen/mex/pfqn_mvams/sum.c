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
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_mvams_data.h"
#include "pfqn_mvams_emxutil.h"
#include "pfqn_mvams_types.h"
#include "rt_nonfinite.h"
#include "sumMatrixIncludeNaN.h"
#include <emmintrin.h>

/* Variable Definitions */
static emlrtRSInfo t_emlrtRSI = {
    20,    /* lineNo */
    "sum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sum.m" /* pathName
                                                                     */
};

static emlrtRSInfo v_emlrtRSI = {
    86,                      /* lineNo */
    "combineVectorElements", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "combineVectorElements.m" /* pathName */
};

static emlrtRSInfo w_emlrtRSI = {
    112,                /* lineNo */
    "blockedSummation", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo x_emlrtRSI = {
    173,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo y_emlrtRSI = {
    190,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo ab_emlrtRSI = {
    192,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo bb_emlrtRSI = {
    204,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo cb_emlrtRSI = {
    207,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo db_emlrtRSI = {
    225,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo eb_emlrtRSI = {
    227,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo fb_emlrtRSI = {
    238,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo dc_emlrtRSI = {
    99,                 /* lineNo */
    "blockedSummation", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo ec_emlrtRSI = {
    22,                    /* lineNo */
    "sumMatrixIncludeNaN", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtRSInfo fc_emlrtRSI = {
    42,                 /* lineNo */
    "sumMatrixColumns", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtRSInfo hc_emlrtRSI = {
    57,                 /* lineNo */
    "sumMatrixColumns", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtRSInfo hf_emlrtRSI = {
    107,                /* lineNo */
    "blockedSummation", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRTEInfo ad_emlrtRTEI = {
    20,                                                             /* lineNo */
    1,                                                              /* colNo */
    "sum",                                                          /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sum.m" /* pName */
};

static emlrtRTEInfo bd_emlrtRTEI = {
    146,                /* lineNo */
    24,                 /* colNo */
    "blockedSummation", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pName */
};

static emlrtRTEInfo cd_emlrtRTEI = {
    153,                /* lineNo */
    23,                 /* colNo */
    "blockedSummation", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pName */
};

static emlrtRTEInfo dd_emlrtRTEI = {
    153,                /* lineNo */
    1,                  /* colNo */
    "blockedSummation", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pName */
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
  st.site = &t_emlrtRSI;
  b_st.site = &u_emlrtRSI;
  c_st.site = &v_emlrtRSI;
  if (x->size[1] == 0) {
    y = 0.0;
  } else {
    d_st.site = &dc_emlrtRSI;
    e_st.site = &ec_emlrtRSI;
    if (x->size[1] < 4096) {
      b_x = *x;
      c_x = x->size[1];
      b_x.size = &c_x;
      b_x.numDimensions = 1;
      f_st.site = &fc_emlrtRSI;
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
        f_st.site = &hc_emlrtRSI;
        y += b_sumColumnB(&f_st, &b_x, nleft, inb + 1);
      }
    }
  }
  return y;
}

real_T c_sum(const emlrtStack *sp, const emxArray_real_T *x)
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
  st.site = &t_emlrtRSI;
  b_st.site = &u_emlrtRSI;
  c_st.site = &v_emlrtRSI;
  if (x->size[0] == 0) {
    y = 0.0;
  } else {
    d_st.site = &hf_emlrtRSI;
    e_st.site = &ec_emlrtRSI;
    if (x->size[0] < 4096) {
      f_st.site = &fc_emlrtRSI;
      y = sumColumnB(&f_st, x, x->size[0]);
    } else {
      int32_T inb;
      int32_T nfb;
      int32_T nleft;
      nfb = (int32_T)((uint32_T)x->size[0] >> 12);
      inb = nfb << 12;
      nleft = x->size[0] - inb;
      y = sumColumnB4(x, 1);
      for (ib = 2; ib <= nfb; ib++) {
        y += sumColumnB4(x, ((ib - 1) << 12) + 1);
      }
      if (nleft > 0) {
        f_st.site = &hc_emlrtRSI;
        y += b_sumColumnB(&f_st, x, nleft, inb + 1);
      }
    }
  }
  return y;
}

void sum(const emlrtStack *sp, const emxArray_real_T *x, emxArray_real_T *y)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack st;
  const real_T *x_data;
  real_T *bsum_data;
  real_T *y_data;
  int32_T b_xj;
  int32_T ib;
  int32_T xj;
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
  x_data = x->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  st.site = &t_emlrtRSI;
  b_st.site = &u_emlrtRSI;
  c_st.site = &v_emlrtRSI;
  if ((x->size[0] == 0) || (x->size[1] == 0)) {
    int32_T hi;
    hi = y->size[0];
    y->size[0] = x->size[0];
    emxEnsureCapacity_real_T(&c_st, y, hi, &ad_emlrtRTEI);
    y_data = y->data;
    hi = x->size[0];
    for (xj = 0; xj < hi; xj++) {
      y_data[xj] = 0.0;
    }
  } else {
    __m128d r;
    int32_T bvstride;
    int32_T firstBlockLength;
    int32_T hi;
    int32_T lastBlockLength;
    int32_T nblocks;
    int32_T vectorUB;
    int32_T vstride;
    int32_T xoffset;
    d_st.site = &w_emlrtRSI;
    vstride = x->size[0];
    bvstride = x->size[0] << 10;
    hi = y->size[0];
    y->size[0] = x->size[0];
    emxEnsureCapacity_real_T(&d_st, y, hi, &bd_emlrtRTEI);
    y_data = y->data;
    emxInit_real_T(&d_st, &y, 1, &dd_emlrtRTEI);
    hi = y->size[0];
    y->size[0] = x->size[0];
    emxEnsureCapacity_real_T(&d_st, y, hi, &cd_emlrtRTEI);
    bsum_data = y->data;
    if (x->size[1] <= 1024) {
      firstBlockLength = x->size[1];
      lastBlockLength = 0;
      nblocks = 1;
    } else {
      firstBlockLength = 1024;
      nblocks = (int32_T)((uint32_T)x->size[1] >> 10);
      lastBlockLength = x->size[1] - (nblocks << 10);
      if (lastBlockLength > 0) {
        nblocks++;
      } else {
        lastBlockLength = 1024;
      }
    }
    e_st.site = &x_emlrtRSI;
    if (x->size[0] > 2147483646) {
      f_st.site = &gb_emlrtRSI;
      check_forloop_overflow_error(&f_st);
    }
    for (xj = 0; xj < vstride; xj++) {
      y_data[xj] = x_data[xj];
      bsum_data[xj] = 0.0;
    }
    e_st.site = &y_emlrtRSI;
    for (xj = 2; xj <= firstBlockLength; xj++) {
      xoffset = (xj - 1) * vstride;
      e_st.site = &ab_emlrtRSI;
      if (vstride > 2147483646) {
        f_st.site = &gb_emlrtRSI;
        check_forloop_overflow_error(&f_st);
      }
      hi = (vstride / 2) << 1;
      vectorUB = hi - 2;
      for (b_xj = 0; b_xj <= vectorUB; b_xj += 2) {
        r = _mm_loadu_pd(&y_data[b_xj]);
        _mm_storeu_pd(&y_data[b_xj],
                      _mm_add_pd(r, _mm_loadu_pd(&x_data[xoffset + b_xj])));
      }
      for (b_xj = hi; b_xj < vstride; b_xj++) {
        y_data[b_xj] += x_data[xoffset + b_xj];
      }
    }
    e_st.site = &bb_emlrtRSI;
    for (ib = 2; ib <= nblocks; ib++) {
      int32_T xblockoffset;
      xblockoffset = (ib - 1) * bvstride;
      e_st.site = &cb_emlrtRSI;
      if (vstride > 2147483646) {
        f_st.site = &gb_emlrtRSI;
        check_forloop_overflow_error(&f_st);
      }
      for (xj = 0; xj < vstride; xj++) {
        bsum_data[xj] = x_data[xblockoffset + xj];
      }
      if (ib == nblocks) {
        hi = lastBlockLength;
      } else {
        hi = 1024;
      }
      e_st.site = &db_emlrtRSI;
      for (xj = 2; xj <= hi; xj++) {
        xoffset = xblockoffset + (xj - 1) * vstride;
        e_st.site = &eb_emlrtRSI;
        vectorUB = (vstride / 2) << 1;
        firstBlockLength = vectorUB - 2;
        for (b_xj = 0; b_xj <= firstBlockLength; b_xj += 2) {
          r = _mm_loadu_pd(&bsum_data[b_xj]);
          _mm_storeu_pd(&bsum_data[b_xj],
                        _mm_add_pd(r, _mm_loadu_pd(&x_data[xoffset + b_xj])));
        }
        for (b_xj = vectorUB; b_xj < vstride; b_xj++) {
          bsum_data[b_xj] += x_data[xoffset + b_xj];
        }
      }
      e_st.site = &fb_emlrtRSI;
      hi = (vstride / 2) << 1;
      vectorUB = hi - 2;
      for (xj = 0; xj <= vectorUB; xj += 2) {
        __m128d r1;
        r = _mm_loadu_pd(&y_data[xj]);
        r1 = _mm_loadu_pd(&bsum_data[xj]);
        _mm_storeu_pd(&y_data[xj], _mm_add_pd(r, r1));
      }
      for (xj = hi; xj < vstride; xj++) {
        y_data[xj] += bsum_data[xj];
      }
    }
    emxFree_real_T(&d_st, &y);
  }
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (sum.c) */
