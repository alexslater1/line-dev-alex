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
#include "pfqn_comom_data.h"
#include "pfqn_comom_emxutil.h"
#include "pfqn_comom_types.h"
#include "rt_nonfinite.h"
#include "mwmathutil.h"

/* Variable Definitions */
static emlrtRSInfo ib_emlrtRSI = {
    20,            /* lineNo */
    "multichoose", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m" /* pathName
                                                                         */
};

static emlrtRSInfo jb_emlrtRSI = {
    18,            /* lineNo */
    "multichoose", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m" /* pathName
                                                                         */
};

static emlrtRSInfo kb_emlrtRSI = {
    41,    /* lineNo */
    "cat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pathName
                                                                       */
};

static emlrtRSInfo lb_emlrtRSI = {
    65,         /* lineNo */
    "cat_impl", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pathName
                                                                       */
};

static emlrtRTEInfo k_emlrtRTEI = {
    225,                   /* lineNo */
    27,                    /* colNo */
    "check_non_axis_size", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pName
                                                                       */
};

static emlrtBCInfo t_emlrtBCI = {
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

static emlrtRTEInfo l_emlrtRTEI = {
    17,            /* lineNo */
    11,            /* colNo */
    "multichoose", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m" /* pName
                                                                         */
};

static emlrtDCInfo l_emlrtDCI = {
    14,            /* lineNo */
    15,            /* colNo */
    "multichoose", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m", /* pName
                                                                          */
    1 /* checkKind */
};

static emlrtDCInfo m_emlrtDCI = {
    14,            /* lineNo */
    15,            /* colNo */
    "multichoose", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m", /* pName
                                                                          */
    4 /* checkKind */
};

static emlrtRTEInfo kc_emlrtRTEI = {
    11,            /* lineNo */
    5,             /* colNo */
    "multichoose", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m" /* pName
                                                                         */
};

static emlrtRTEInfo lc_emlrtRTEI = {
    14,            /* lineNo */
    5,             /* colNo */
    "multichoose", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m" /* pName
                                                                         */
};

static emlrtRTEInfo mc_emlrtRTEI = {
    20,            /* lineNo */
    19,            /* colNo */
    "multichoose", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m" /* pName
                                                                         */
};

static emlrtRTEInfo nc_emlrtRTEI = {
    245,   /* lineNo */
    14,    /* colNo */
    "cat", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pName
                                                                       */
};

static emlrtRTEInfo oc_emlrtRTEI = {
    20,            /* lineNo */
    13,            /* colNo */
    "multichoose", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoose.m" /* pName
                                                                         */
};

static emlrtRTEInfo pc_emlrtRTEI = {
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
  int32_T i2;
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
    emxEnsureCapacity_real_T(sp, v, loop_ub, &kc_emlrtRTEI);
    v_data = v->data;
    v_data[0] = k;
  } else if (k == 0.0) {
    int32_T b_loop_ub;
    int32_T loop_ub;
    if (!(n >= 0.0)) {
      emlrtNonNegativeCheckR2012b(n, &m_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (n != (int32_T)muDoubleScalarFloor(n)) {
      emlrtIntegerCheckR2012b(n, &l_emlrtDCI, (emlrtConstCTX)sp);
    }
    loop_ub = v->size[0] * v->size[1];
    v->size[0] = 1;
    b_loop_ub = (int32_T)n;
    v->size[1] = (int32_T)n;
    emxEnsureCapacity_real_T(sp, v, loop_ub, &lc_emlrtRTEI);
    v_data = v->data;
    for (i2 = 0; i2 < b_loop_ub; i2++) {
      v_data[i2] = 0.0;
    }
  } else {
    int32_T i;
    i = (int32_T)(k + 1.0);
    emlrtForLoopVectorCheckR2021a(0.0, 1.0, k, mxDOUBLE_CLASS,
                                  (int32_T)(k + 1.0), &l_emlrtRTEI,
                                  (emlrtConstCTX)sp);
    emxInit_real_T(sp, &w, 2, &pc_emlrtRTEI);
    emxInit_real_T(sp, &varargin_2, 2, &mc_emlrtRTEI);
    emxInit_real_T(sp, &b_v, 2, &nc_emlrtRTEI);
    for (b_i = 0; b_i < i; b_i++) {
      int32_T i1;
      st.site = &jb_emlrtRSI;
      multichoose(&st, n - 1.0, k - (real_T)b_i, w);
      w_data = w->data;
      i1 = w->size[0];
      if (w->size[0] - 1 >= 0) {
        c_loop_ub = w->size[1];
      }
      for (j = 0; j < i1; j++) {
        int32_T b_loop_ub;
        int32_T loop_ub;
        int32_T sizes_idx_1;
        boolean_T sizes_idx_1_tmp;
        st.site = &ib_emlrtRSI;
        if (j + 1 > i1) {
          emlrtDynamicBoundsCheckR2012b(j + 1, 1, i1, &t_emlrtBCI, &st);
        }
        loop_ub = varargin_2->size[0] * varargin_2->size[1];
        varargin_2->size[0] = 1;
        varargin_2->size[1] = w->size[1] + 1;
        emxEnsureCapacity_real_T(&st, varargin_2, loop_ub, &mc_emlrtRTEI);
        varargin_2_data = varargin_2->data;
        varargin_2_data[0] = b_i;
        for (i2 = 0; i2 < c_loop_ub; i2++) {
          varargin_2_data[i2 + 1] = w_data[j + w->size[0] * i2];
        }
        b_st.site = &kb_emlrtRSI;
        sizes_idx_1_tmp = ((v->size[0] != 0) && (v->size[1] != 0));
        if (sizes_idx_1_tmp) {
          sizes_idx_1 = v->size[1];
        } else {
          sizes_idx_1 = varargin_2->size[1];
        }
        c_st.site = &lb_emlrtRSI;
        if ((v->size[1] != sizes_idx_1) &&
            ((v->size[0] != 0) && (v->size[1] != 0))) {
          emlrtErrorWithMessageIdR2018a(
              &c_st, &k_emlrtRTEI, "MATLAB:catenate:matrixDimensionMismatch",
              "MATLAB:catenate:matrixDimensionMismatch", 0);
        }
        if (varargin_2->size[1] != sizes_idx_1) {
          emlrtErrorWithMessageIdR2018a(
              &c_st, &k_emlrtRTEI, "MATLAB:catenate:matrixDimensionMismatch",
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
        emxEnsureCapacity_real_T(&b_st, b_v, b_loop_ub, &nc_emlrtRTEI);
        b_v_data = b_v->data;
        for (i2 = 0; i2 < sizes_idx_1; i2++) {
          for (i3 = 0; i3 < loop_ub; i3++) {
            b_v_data[i3 + b_v->size[0] * i2] = v_data[i3 + loop_ub * i2];
          }
        }
        for (i2 = 0; i2 < sizes_idx_1; i2++) {
          b_v_data[loop_ub + b_v->size[0] * i2] = varargin_2_data[i2];
        }
        loop_ub = v->size[0] * v->size[1];
        v->size[0] = b_v->size[0];
        v->size[1] = sizes_idx_1;
        emxEnsureCapacity_real_T(&b_st, v, loop_ub, &oc_emlrtRTEI);
        v_data = v->data;
        loop_ub = b_v->size[0] * b_v->size[1];
        for (i2 = 0; i2 < loop_ub; i2++) {
          v_data[i2] = b_v_data[i2];
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
