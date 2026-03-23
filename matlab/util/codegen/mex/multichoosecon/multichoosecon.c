/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * multichoosecon.c
 *
 * Code generation for function 'multichoosecon'
 *
 */

/* Include files */
#include "multichoosecon.h"
#include "cat.h"
#include "multichoosecon_data.h"
#include "multichoosecon_emxutil.h"
#include "multichoosecon_types.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRSInfo b_emlrtRSI = {
    25,               /* lineNo */
    "multichoosecon", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoosecon.m" /* pathName
                                                                            */
};

static emlrtECInfo emlrtECI = {
    2,                /* nDims */
    28,               /* lineNo */
    17,               /* colNo */
    "multichoosecon", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoosecon.m" /* pName
                                                                            */
};

static emlrtBCInfo
    emlrtBCI =
        {
            -1,               /* iFirst */
            -1,               /* iLast */
            27,               /* lineNo */
            13,               /* colNo */
            "y",              /* aName */
            "multichoosecon", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/"
            "multichoosecon.m", /* pName */
            0                   /* checkKind */
};

static emlrtBCInfo
    b_emlrtBCI =
        {
            -1,               /* iFirst */
            -1,               /* iLast */
            14,               /* lineNo */
            24,               /* colNo */
            "v",              /* aName */
            "multichoosecon", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/"
            "multichoosecon.m", /* pName */
            0                   /* checkKind */
};

static emlrtBCInfo
    c_emlrtBCI =
        {
            -1,               /* iFirst */
            -1,               /* iLast */
            14,               /* lineNo */
            15,               /* colNo */
            "v",              /* aName */
            "multichoosecon", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/"
            "multichoosecon.m", /* pName */
            0                   /* checkKind */
};

static emlrtBCInfo
    d_emlrtBCI =
        {
            -1,               /* iFirst */
            -1,               /* iLast */
            22,               /* lineNo */
            10,               /* colNo */
            "n",              /* aName */
            "multichoosecon", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/"
            "multichoosecon.m", /* pName */
            0                   /* checkKind */
};

static emlrtBCInfo
    e_emlrtBCI =
        {
            -1,               /* iFirst */
            -1,               /* iLast */
            12,               /* lineNo */
            14,               /* colNo */
            "n",              /* aName */
            "multichoosecon", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/"
            "multichoosecon.m", /* pName */
            0                   /* checkKind */
};

static emlrtBCInfo
    f_emlrtBCI =
        {
            -1,               /* iFirst */
            -1,               /* iLast */
            24,               /* lineNo */
            22,               /* colNo */
            "n_1",            /* aName */
            "multichoosecon", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/"
            "multichoosecon.m", /* pName */
            0                   /* checkKind */
};

static emlrtRTEInfo emlrtRTEI = {
    23,               /* lineNo */
    9,                /* colNo */
    "multichoosecon", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/multichoosecon.m" /* pName
                                                                            */
};

/* Function Definitions */
void multichoosecon(const emlrtStack *sp, const emxArray_real_T *n, real_T S,
                    int32_T v_size[2])
{
  emlrtStack st;
  emxArray_real_T *n_1;
  const real_T *n_data;
  real_T *n_1_data;
  int32_T unusedExpr[2];
  int32_T b_i;
  int32_T c_i;
  st.prev = sp;
  st.tls = sp->tls;
  n_data = n->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /*  v = MULTICHOOSECON(n,S) */
  /*  Pick vectors of S elements from the available units in vector n */
  /*  */
  /*  Copyright (c) 2012-2026, Imperial College London */
  /*  All rights reserved. */
  v_size[0] = 0;
  v_size[1] = 0;
  if (S == 1.0) {
    int32_T i;
    i = n->size[1];
    for (b_i = 0; b_i < i; b_i++) {
      if (b_i + 1 > i) {
        emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, i, &e_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (n_data[b_i] > 0.0) {
        if (n->size[1] >= 1) {
          emlrtDynamicBoundsCheckR2012b(1, 1, 0, &b_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        emlrtDynamicBoundsCheckR2012b(1, 1, 0, &c_emlrtBCI, (emlrtConstCTX)sp);
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
  } else {
    int32_T loop_ub;
    loop_ub = n->size[1];
    emxInit_real_T(sp, &n_1, &emlrtRTEI);
    for (c_i = 0; c_i < loop_ub; c_i++) {
      real_T d;
      if (c_i + 1 > loop_ub) {
        emlrtDynamicBoundsCheckR2012b(c_i + 1, 1, loop_ub, &d_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      d = n_data[c_i];
      if (d > 0.0) {
        int32_T i;
        i = n_1->size[0] * n_1->size[1];
        n_1->size[0] = 1;
        n_1->size[1] = loop_ub;
        emxEnsureCapacity_real_T(sp, n_1, i, &emlrtRTEI);
        n_1_data = n_1->data;
        for (b_i = 0; b_i < loop_ub; b_i++) {
          n_1_data[b_i] = n_data[b_i];
        }
        if (c_i + 1 > loop_ub) {
          emlrtDynamicBoundsCheckR2012b(c_i + 1, 1, loop_ub, &f_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        n_1_data[c_i] = d - 1.0;
        st.site = &b_emlrtRSI;
        multichoosecon(&st, n_1, S - 1.0, unusedExpr);
        if (c_i + 1 > loop_ub) {
          emlrtDynamicBoundsCheckR2012b(c_i + 1, 1, loop_ub, &emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if ((loop_ub != 0) && (loop_ub != 1)) {
          emlrtDimSizeImpxCheckR2021b(loop_ub, 0, &emlrtECI, (emlrtConstCTX)sp);
        }
        if (n->size[1] == 0) {
          v_size[0] = 0;
          v_size[1] = 0;
        } else {
          binary_expand_op(v_size);
        }
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    emxFree_real_T(sp, &n_1);
    /* v= sortrows(unique(v,'rows')); */
  }
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (multichoosecon.c) */
