/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pprod.c
 *
 * Code generation for function 'pprod'
 *
 */

/* Include files */
#include "pprod.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_mvams_data.h"
#include "pfqn_mvams_emxutil.h"
#include "pfqn_mvams_types.h"
#include "rt_nonfinite.h"
#include "omp.h"

/* Variable Definitions */
static emlrtRSInfo lf_emlrtRSI = {
    21,                                                           /* lineNo */
    "pprod",                                                      /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/pprod.m" /* pathName */
};

static emlrtRSInfo mf_emlrtRSI = {
    15,    /* lineNo */
    "sum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sum.m" /* pathName
                                                                     */
};

static emlrtECInfo s_emlrtECI = {
    2,                                                            /* nDims */
    21,                                                           /* lineNo */
    12,                                                           /* colNo */
    "pprod",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/pprod.m" /* pName */
};

static emlrtBCInfo ys_emlrtBCI = {
    -1,                                                            /* iFirst */
    -1,                                                            /* iLast */
    27,                                                            /* lineNo */
    20,                                                            /* colNo */
    "n",                                                           /* aName */
    "pprod",                                                       /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/pprod.m", /* pName */
    0 /* checkKind */
};

static emlrtBCInfo at_emlrtBCI = {
    -1,                                                            /* iFirst */
    -1,                                                            /* iLast */
    27,                                                            /* lineNo */
    26,                                                            /* colNo */
    "N",                                                           /* aName */
    "pprod",                                                       /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/pprod.m", /* pName */
    0 /* checkKind */
};

static emlrtBCInfo bt_emlrtBCI = {
    -1,                                                            /* iFirst */
    -1,                                                            /* iLast */
    28,                                                            /* lineNo */
    11,                                                            /* colNo */
    "n",                                                           /* aName */
    "pprod",                                                       /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/pprod.m", /* pName */
    0 /* checkKind */
};

static emlrtBCInfo ct_emlrtBCI = {
    -1,                                                            /* iFirst */
    -1,                                                            /* iLast */
    35,                                                            /* lineNo */
    12,                                                            /* colNo */
    "n",                                                           /* aName */
    "pprod",                                                       /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/pprod.m", /* pName */
    0 /* checkKind */
};

static emlrtRTEInfo ei_emlrtRTEI = {
    21,                                                           /* lineNo */
    12,                                                           /* colNo */
    "pprod",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/pprod.m" /* pName */
};

static emlrtRTEInfo fi_emlrtRTEI = {
    22,                                                           /* lineNo */
    9,                                                            /* colNo */
    "pprod",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/pprod.m" /* pName */
};

/* Function Declarations */
static void eq(const emlrtStack *sp, emxArray_boolean_T *in1,
               const emxArray_real_T *in2, const emxArray_real_T *in3);

/* Function Definitions */
static void eq(const emlrtStack *sp, emxArray_boolean_T *in1,
               const emxArray_real_T *in2, const emxArray_real_T *in3)
{
  jmp_buf *volatile emlrtJBStack;
  const real_T *in2_data;
  const real_T *in3_data;
  int32_T eq_numThreads;
  int32_T i;
  int32_T loop_ub;
  int32_T stride_0_1;
  int32_T stride_1_1;
  boolean_T *in1_data;
  in3_data = in3->data;
  in2_data = in2->data;
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[0] = 1;
  emxEnsureCapacity_boolean_T(sp, in1, stride_0_1, &ei_emlrtRTEI);
  if (in3->size[1] == 1) {
    loop_ub = in2->size[1];
  } else {
    loop_ub = in3->size[1];
  }
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[1] = loop_ub;
  emxEnsureCapacity_boolean_T(sp, in1, stride_0_1, &ei_emlrtRTEI);
  in1_data = in1->data;
  stride_0_1 = (in2->size[1] != 1);
  stride_1_1 = (in3->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      in1_data[i] = (in2_data[i * stride_0_1] == in3_data[i * stride_1_1]);
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    eq_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(eq_numThreads)

    for (i = 0; i < loop_ub; i++) {
      in1_data[i] = (in2_data[i * stride_0_1] == in3_data[i * stride_1_1]);
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
}

void pprod(const emlrtStack *sp, emxArray_real_T *n, const emxArray_real_T *N)
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack st;
  emxArray_boolean_T *x;
  const real_T *N_data;
  real_T *n_data;
  int32_T i;
  int32_T i1;
  int32_T k;
  int32_T nz;
  int32_T pprod_numThreads;
  int32_T vlen;
  boolean_T *x_data;
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
  N_data = N->data;
  n_data = n->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /*  n=PPROD(N) % init */
  /*  n=PPROD(n,N) % next state */
  /*  Return a sequence of non-negative vectors less than a given vector */
  /*  */
  /*  n=pprod(N); */
  /*  while n>=0 */
  /*    ... */
  /*    n=pprod(n,N); */
  /*  end */
  /*  */
  /*  Copyright (c) 2012-2026, Imperial College London */
  /*  All rights reserved. */
  nz = n->size[1];
  i = N->size[1];
  if ((nz != N->size[1]) && ((nz != 1) && (N->size[1] != 1))) {
    emlrtDimSizeImpxCheckR2021b(nz, N->size[1], &s_emlrtECI, (emlrtConstCTX)sp);
  }
  st.site = &lf_emlrtRSI;
  emxInit_boolean_T(&st, &x, &ei_emlrtRTEI);
  if (n->size[1] == N->size[1]) {
    vlen = x->size[0] * x->size[1];
    x->size[0] = 1;
    x->size[1] = nz;
    emxEnsureCapacity_boolean_T(&st, x, vlen, &ei_emlrtRTEI);
    x_data = x->data;
    if (nz < 1600) {
      for (i1 = 0; i1 < nz; i1++) {
        x_data[i1] = (n_data[i1] == N_data[i1]);
      }
    } else {
      emlrtEnterParallelRegion(&st, omp_in_parallel());
      emlrtPushJmpBuf(&st, &emlrtJBStack);
      pprod_numThreads =
          emlrtAllocRegionTLSs(st.tls, omp_in_parallel(), omp_get_max_threads(),
                               omp_get_num_procs());
#pragma omp parallel for num_threads(pprod_numThreads)

      for (i1 = 0; i1 < nz; i1++) {
        x_data[i1] = (n_data[i1] == N_data[i1]);
      }
      emlrtPopJmpBuf(&st, &emlrtJBStack);
      emlrtExitParallelRegion(&st, omp_in_parallel());
    }
  } else {
    b_st.site = &lf_emlrtRSI;
    eq(&b_st, x, n, N);
    x_data = x->data;
  }
  b_st.site = &mf_emlrtRSI;
  c_st.site = &u_emlrtRSI;
  vlen = x->size[1];
  if (x->size[1] == 0) {
    nz = 0;
  } else {
    d_st.site = &bc_emlrtRSI;
    nz = x_data[0];
    e_st.site = &cc_emlrtRSI;
    if (x->size[1] > 2147483646) {
      f_st.site = &gb_emlrtRSI;
      check_forloop_overflow_error(&f_st);
    }
    for (k = 2; k <= vlen; k++) {
      nz += x_data[k - 1];
    }
  }
  emxFree_boolean_T(&c_st, &x);
  if (nz == N->size[1]) {
    vlen = n->size[0] * n->size[1];
    n->size[0] = 1;
    n->size[1] = 1;
    emxEnsureCapacity_real_T(sp, n, vlen, &fi_emlrtRTEI);
    n_data = n->data;
    n_data[0] = -1.0;
  } else {
    boolean_T exitg1;
    nz = N->size[1];
    exitg1 = false;
    while ((!exitg1) && (nz > 0)) {
      vlen = n->size[1];
      if (nz > vlen) {
        emlrtDynamicBoundsCheckR2012b(nz, 1, vlen, &ys_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (nz > i) {
        emlrtDynamicBoundsCheckR2012b(nz, 1, i, &at_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (n_data[nz - 1] == N_data[nz - 1]) {
        if (nz > vlen) {
          emlrtDynamicBoundsCheckR2012b(nz, 1, vlen, &bt_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        n_data[nz - 1] = 0.0;
        nz--;
      } else {
        exitg1 = true;
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    if (nz != 0) {
      vlen = n->size[1];
      if (nz > vlen) {
        emlrtDynamicBoundsCheckR2012b(nz, 1, vlen, &ct_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      n_data[nz - 1]++;
    } else {
      /* n=-1*ones(1,R);         */
    }
  }
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (pprod.c) */
