/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * multichoose.c
 *
 * Code generation for function 'multichoose'
 *
 */

/* Include files */
#include "multichoose.h"
#include "pfqn_grnmol_data.h"
#include "pfqn_grnmol_emxutil.h"
#include "pfqn_grnmol_types.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRSInfo y_emlrtRSI = {
    20,            /* lineNo */
    "multichoose", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m" /* pathName
                                                                         */
};

static emlrtRSInfo ab_emlrtRSI = {
    18,            /* lineNo */
    "multichoose", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m" /* pathName
                                                                         */
};

static emlrtRSInfo bb_emlrtRSI = {
    41,    /* lineNo */
    "cat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pathName
                                                                       */
};

static emlrtRSInfo cb_emlrtRSI = {
    65,         /* lineNo */
    "cat_impl", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pathName
                                                                       */
};

static emlrtRTEInfo g_emlrtRTEI = {
    225,                   /* lineNo */
    27,                    /* colNo */
    "check_non_axis_size", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pName
                                                                       */
};

static emlrtBCInfo k_emlrtBCI = {
    -1,            /* iFirst */
    -1,            /* iLast */
    20,            /* lineNo */
    24,            /* colNo */
    "w",           /* aName */
    "multichoose", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m", /* pName
                                                                          */
    0 /* checkKind */
};

static emlrtRTEInfo h_emlrtRTEI = {
    17,            /* lineNo */
    11,            /* colNo */
    "multichoose", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m" /* pName
                                                                         */
};

static emlrtDCInfo d_emlrtDCI = {
    14,            /* lineNo */
    15,            /* colNo */
    "multichoose", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m", /* pName
                                                                          */
    4 /* checkKind */
};

static emlrtRTEInfo eb_emlrtRTEI = {
    11,            /* lineNo */
    5,             /* colNo */
    "multichoose", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m" /* pName
                                                                         */
};

static emlrtRTEInfo fb_emlrtRTEI = {
    14,            /* lineNo */
    5,             /* colNo */
    "multichoose", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m" /* pName
                                                                         */
};

static emlrtRTEInfo gb_emlrtRTEI = {
    20,            /* lineNo */
    19,            /* colNo */
    "multichoose", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m" /* pName
                                                                         */
};

static emlrtRTEInfo hb_emlrtRTEI = {
    245,   /* lineNo */
    14,    /* colNo */
    "cat", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pName
                                                                       */
};

static emlrtRTEInfo ib_emlrtRTEI = {
    20,            /* lineNo */
    13,            /* colNo */
    "multichoose", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m" /* pName
                                                                         */
};

static emlrtRTEInfo jb_emlrtRTEI = {
    18,            /* lineNo */
    9,             /* colNo */
    "multichoose", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m" /* pName
                                                                         */
};

/* Function Definitions */
void multichoose(const emlrtStack *sp, real_T n, real_T k, emxArray_real_T *v)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  emxArray_real_T *b_v;
  emxArray_real_T *varargin_2;
  emxArray_real_T *w;
  real_T *b_v_data;
  real_T *v_data;
  real_T *varargin_2_data;
  real_T *w_data;
  int32_T b_i;
  int32_T c_loop_ub;
  int32_T i1;
  int32_T i3;
  int32_T j;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  v_data = v->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /*  v = MULTICHOOSE(n,k) */
  /*  Chooses k elements out of n with repetition */
  /*  */
  /*  Copyright (c) 2012-2026, Imperial College London */
  /*  All rights reserved. */
  v->size[0] = 0;
  v->size[1] = 0;
  if (n == 1.0) {
    int32_T loop_ub;
    loop_ub = v->size[0] * v->size[1];
    v->size[0] = 1;
    v->size[1] = 1;
    emxEnsureCapacity_real_T(sp, v, loop_ub, &eb_emlrtRTEI);
    v_data = v->data;
    v_data[0] = k;
  } else if (k == 0.0) {
    int32_T b_loop_ub;
    int32_T loop_ub;
    if (!(n >= 0.0)) {
      emlrtNonNegativeCheckR2012b(n, &d_emlrtDCI, (emlrtConstCTX)sp);
    }
    loop_ub = v->size[0] * v->size[1];
    v->size[0] = 1;
    b_loop_ub = (int32_T)n;
    v->size[1] = (int32_T)n;
    emxEnsureCapacity_real_T(sp, v, loop_ub, &fb_emlrtRTEI);
    v_data = v->data;
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      v_data[i1] = 0.0;
    }
  } else {
    int32_T i;
    i = (int32_T)(k + 1.0);
    emlrtForLoopVectorCheckR2021a(0.0, 1.0, k, mxDOUBLE_CLASS,
                                  (int32_T)(k + 1.0), &h_emlrtRTEI,
                                  (emlrtConstCTX)sp);
    emxInit_real_T(sp, &w, 2, &jb_emlrtRTEI);
    emxInit_real_T(sp, &varargin_2, 2, &gb_emlrtRTEI);
    emxInit_real_T(sp, &b_v, 2, &hb_emlrtRTEI);
    for (b_i = 0; b_i < i; b_i++) {
      int32_T i2;
      st.site = &ab_emlrtRSI;
      multichoose(&st, n - 1.0, k - (real_T)b_i, w);
      w_data = w->data;
      i2 = w->size[0];
      if (w->size[0] - 1 >= 0) {
        c_loop_ub = w->size[1];
      }
      for (j = 0; j < i2; j++) {
        int32_T b_loop_ub;
        int32_T loop_ub;
        int32_T sizes_idx_1;
        boolean_T sizes_idx_1_tmp;
        st.site = &y_emlrtRSI;
        if (j + 1 > i2) {
          emlrtDynamicBoundsCheckR2012b(j + 1, 1, i2, &k_emlrtBCI, &st);
        }
        loop_ub = varargin_2->size[0] * varargin_2->size[1];
        varargin_2->size[0] = 1;
        varargin_2->size[1] = w->size[1] + 1;
        emxEnsureCapacity_real_T(&st, varargin_2, loop_ub, &gb_emlrtRTEI);
        varargin_2_data = varargin_2->data;
        varargin_2_data[0] = b_i;
        for (i1 = 0; i1 < c_loop_ub; i1++) {
          varargin_2_data[i1 + 1] = w_data[j + w->size[0] * i1];
        }
        b_st.site = &bb_emlrtRSI;
        sizes_idx_1_tmp = ((v->size[0] != 0) && (v->size[1] != 0));
        if (sizes_idx_1_tmp) {
          sizes_idx_1 = v->size[1];
        } else {
          sizes_idx_1 = varargin_2->size[1];
        }
        c_st.site = &cb_emlrtRSI;
        if ((v->size[1] != sizes_idx_1) &&
            ((v->size[0] != 0) && (v->size[1] != 0))) {
          emlrtErrorWithMessageIdR2018a(
              &c_st, &g_emlrtRTEI, "MATLAB:catenate:matrixDimensionMismatch",
              "MATLAB:catenate:matrixDimensionMismatch", 0);
        }
        if (varargin_2->size[1] != sizes_idx_1) {
          emlrtErrorWithMessageIdR2018a(
              &c_st, &g_emlrtRTEI, "MATLAB:catenate:matrixDimensionMismatch",
              "MATLAB:catenate:matrixDimensionMismatch", 0);
        }
        if (sizes_idx_1_tmp) {
          loop_ub = v->size[0];
        } else {
          loop_ub = 0;
        }
        b_loop_ub = b_v->size[0] * b_v->size[1];
        b_v->size[0] = loop_ub + 1;
        b_v->size[1] = sizes_idx_1;
        emxEnsureCapacity_real_T(&b_st, b_v, b_loop_ub, &hb_emlrtRTEI);
        b_v_data = b_v->data;
        for (i1 = 0; i1 < sizes_idx_1; i1++) {
          for (i3 = 0; i3 < loop_ub; i3++) {
            b_v_data[i3 + b_v->size[0] * i1] = v_data[i3 + loop_ub * i1];
          }
        }
        for (i1 = 0; i1 < sizes_idx_1; i1++) {
          b_v_data[loop_ub + b_v->size[0] * i1] = varargin_2_data[i1];
        }
        loop_ub = v->size[0] * v->size[1];
        v->size[0] = b_v->size[0];
        v->size[1] = sizes_idx_1;
        emxEnsureCapacity_real_T(&b_st, v, loop_ub, &ib_emlrtRTEI);
        v_data = v->data;
        loop_ub = b_v->size[0] * b_v->size[1];
        for (i1 = 0; i1 < loop_ub; i1++) {
          v_data[i1] = b_v_data[i1];
        }
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b((emlrtConstCTX)sp);
        }
      }
      /* for */
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    emxFree_real_T(sp, &b_v);
    emxFree_real_T(sp, &varargin_2);
    emxFree_real_T(sp, &w);
    /* for */

    /* if */
  }
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (multichoose.c) */
