/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * integralCalc.c
 *
 * Code generation for function 'integralCalc'
 *
 */

/* Include files */
#include "integralCalc.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_mmint2.h"
#include "pfqn_mmint2_data.h"
#include "pfqn_mmint2_emxutil.h"
#include "pfqn_mmint2_mexutil.h"
#include "pfqn_mmint2_types.h"
#include "rt_nonfinite.h"
#include "warning.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>

/* Variable Definitions */
static emlrtRSInfo vb_emlrtRSI = {
    180,                    /* lineNo */
    "scalarValuedIntegral", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo wb_emlrtRSI = {
    201,                    /* lineNo */
    "scalarValuedIntegral", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo xb_emlrtRSI = {
    225,                    /* lineNo */
    "scalarValuedIntegral", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo yb_emlrtRSI = {
    227,                    /* lineNo */
    "scalarValuedIntegral", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo ac_emlrtRSI = {
    232,                    /* lineNo */
    "scalarValuedIntegral", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo bc_emlrtRSI = {
    237,                    /* lineNo */
    "scalarValuedIntegral", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo cc_emlrtRSI = {
    244,                    /* lineNo */
    "scalarValuedIntegral", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo dc_emlrtRSI = {
    263,                    /* lineNo */
    "scalarValuedIntegral", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo ec_emlrtRSI = {
    295,                    /* lineNo */
    "scalarValuedIntegral", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo fc_emlrtRSI = {
    310,                    /* lineNo */
    "scalarValuedIntegral", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo gc_emlrtRSI = {
    309,                    /* lineNo */
    "scalarValuedIntegral", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo hc_emlrtRSI = {
    596,               /* lineNo */
    "transFormDomain", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo ic_emlrtRSI = {
    588,               /* lineNo */
    "transFormDomain", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo jc_emlrtRSI = {
    583,               /* lineNo */
    "transFormDomain", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo qc_emlrtRSI = {
    738,                /* lineNo */
    "hasMeshCollapsed", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo rc_emlrtRSI = {
    744,                /* lineNo */
    "hasMeshCollapsed", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo sc_emlrtRSI = {
    743,                /* lineNo */
    "hasMeshCollapsed", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtMCInfo e_emlrtMCI = {
    53,        /* lineNo */
    19,        /* colNo */
    "flt2str", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/shared/coder/coder/lib/+coder/+internal/"
    "flt2str.m" /* pName */
};

static emlrtRTEInfo i_emlrtRTEI = {
    198,                    /* lineNo */
    5,                      /* colNo */
    "scalarValuedIntegral", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo j_emlrtRTEI = {
    228,                    /* lineNo */
    35,                     /* colNo */
    "scalarValuedIntegral", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo nb_emlrtRTEI = {
    175,            /* lineNo */
    26,             /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo ob_emlrtRTEI = {
    176,            /* lineNo */
    26,             /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo pb_emlrtRTEI = {
    177,            /* lineNo */
    26,             /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo qb_emlrtRTEI = {
    199,            /* lineNo */
    24,             /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo rb_emlrtRTEI = {
    576,            /* lineNo */
    20,             /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo sb_emlrtRTEI = {
    577,            /* lineNo */
    20,             /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo tb_emlrtRTEI = {
    230,            /* lineNo */
    13,             /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo ub_emlrtRTEI = {
    237,            /* lineNo */
    17,             /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo vb_emlrtRTEI = {
    175,            /* lineNo */
    1,              /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo wb_emlrtRTEI = {
    176,            /* lineNo */
    1,              /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo xb_emlrtRTEI = {
    177,            /* lineNo */
    1,              /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo yb_emlrtRTEI = {
    199,            /* lineNo */
    5,              /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo ac_emlrtRTEI = {
    212,            /* lineNo */
    13,             /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo bc_emlrtRTEI = {
    147,            /* lineNo */
    30,             /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo cc_emlrtRTEI = {
    230,            /* lineNo */
    18,             /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRSInfo td_emlrtRSI = {
    53,        /* lineNo */
    "flt2str", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/shared/coder/coder/lib/+coder/+internal/"
    "flt2str.m" /* pathName */
};

static emlrtRSInfo ud_emlrtRSI = {
    230,                    /* lineNo */
    "scalarValuedIntegral", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

/* Function Declarations */
static const mxArray *b_sprintf(const emlrtStack *sp, const mxArray *m,
                                const mxArray *m1, emlrtMCInfo *location);

static void c_emlrt_marshallIn(const emlrtStack *sp,
                               const mxArray *a__output_of_sprintf_,
                               const char_T *identifier, char_T y[14]);

static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               char_T y[14]);

static void e_emlrt_marshallIn(const emlrtStack *sp,
                               const mxArray *a__output_of_sprintf_,
                               const char_T *identifier, char_T y[9]);

static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId, char_T y[9]);

static void l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, char_T ret[14]);

static void m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, char_T ret[9]);

static void times(const emlrtStack *sp, emxArray_real_T *in1,
                  const emxArray_real_T *in2);

/* Function Definitions */
static const mxArray *b_sprintf(const emlrtStack *sp, const mxArray *m,
                                const mxArray *m1, emlrtMCInfo *location)
{
  const mxArray *pArrays[2];
  const mxArray *m2;
  pArrays[0] = m;
  pArrays[1] = m1;
  return emlrtCallMATLABR2012b((emlrtConstCTX)sp, 1, &m2, 2, &pArrays[0],
                               "sprintf", true, location);
}

static void c_emlrt_marshallIn(const emlrtStack *sp,
                               const mxArray *a__output_of_sprintf_,
                               const char_T *identifier, char_T y[14])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  d_emlrt_marshallIn(sp, emlrtAlias(a__output_of_sprintf_), &thisId, y);
  emlrtDestroyArray(&a__output_of_sprintf_);
}

static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId, char_T y[14])
{
  l_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static void e_emlrt_marshallIn(const emlrtStack *sp,
                               const mxArray *a__output_of_sprintf_,
                               const char_T *identifier, char_T y[9])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  f_emlrt_marshallIn(sp, emlrtAlias(a__output_of_sprintf_), &thisId, y);
  emlrtDestroyArray(&a__output_of_sprintf_);
}

static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId, char_T y[9])
{
  m_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static void l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, char_T ret[14])
{
  static const int32_T dims[2] = {1, 14};
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "char", false, 2U,
                          (const void *)&dims[0]);
  emlrtImportCharArrayR2015b((emlrtConstCTX)sp, src, &ret[0], 14);
  emlrtDestroyArray(&src);
}

static void m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, char_T ret[9])
{
  static const int32_T dims[2] = {1, 9};
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "char", false, 2U,
                          (const void *)&dims[0]);
  emlrtImportCharArrayR2015b((emlrtConstCTX)sp, src, &ret[0], 9);
  emlrtDestroyArray(&src);
}

static void times(const emlrtStack *sp, emxArray_real_T *in1,
                  const emxArray_real_T *in2)
{
  jmp_buf *volatile emlrtJBStack;
  emxArray_real_T *b_in1;
  const real_T *in2_data;
  real_T *b_in1_data;
  real_T *in1_data;
  int32_T i;
  int32_T i1;
  int32_T loop_ub;
  int32_T stride_0_1;
  int32_T times_numThreads;
  in2_data = in2->data;
  in1_data = in1->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &b_in1, 2, &cc_emlrtRTEI);
  stride_0_1 = b_in1->size[0] * b_in1->size[1];
  b_in1->size[0] = 1;
  loop_ub = in2->size[1];
  b_in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, b_in1, stride_0_1, &cc_emlrtRTEI);
  b_in1_data = b_in1->data;
  stride_0_1 = (in1->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      b_in1_data[i] = in1_data[i * stride_0_1] * in2_data[i];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    times_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(times_numThreads)

    for (i = 0; i < loop_ub; i++) {
      b_in1_data[i] = in1_data[i * stride_0_1] * in2_data[i];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[0] = 1;
  in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &cc_emlrtRTEI);
  in1_data = in1->data;
  for (i1 = 0; i1 < loop_ub; i1++) {
    in1_data[i1] = b_in1_data[i1];
  }
  emxFree_real_T(sp, &b_in1);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

real_T scalarValuedIntegral(const emlrtStack *sp,
                            const emxArray_real_T *fun_workspace_Z,
                            const emxArray_real_T *fun_workspace_nnzClasses,
                            const emxArray_real_T *fun_workspace_L,
                            const emxArray_real_T *fun_workspace_N,
                            const emxArray_real_T *interval, int32_T nt,
                            real_T pathlen, real_T *errbnd, boolean_T *success)
{
  static const real_T dv[15] = {-0.99145537112081261, -0.94910791234275849,
                                -0.8648644233597691,  -0.74153118559939435,
                                -0.58608723546769115, -0.40584515137739718,
                                -0.20778495500789851, 0.0,
                                0.20778495500789851,  0.40584515137739718,
                                0.58608723546769115,  0.74153118559939435,
                                0.8648644233597691,   0.94910791234275849,
                                0.99145537112081261};
  static const real_T dv1[15] = {
      0.022935322010529221, 0.063092092629978544, 0.1047900103222502,
      0.14065325971552589,  0.16900472663926791,  0.19035057806478539,
      0.20443294007529891,  0.20948214108472779,  0.20443294007529891,
      0.19035057806478539,  0.16900472663926791,  0.14065325971552589,
      0.1047900103222502,   0.063092092629978544, 0.022935322010529221};
  static const real_T dv2[15] = {
      0.022935322010529221, -0.066392873538891159, 0.1047900103222502,
      -0.13905213177375081, 0.16900472663926791,   -0.19147947244033353,
      0.20443294007529891,  -0.20847704258874161,  0.20443294007529891,
      -0.19147947244033353, 0.16900472663926791,   -0.13905213177375081,
      0.1047900103222502,   -0.066392873538891159, 0.022935322010529221};
  static const int32_T iv[2] = {1, 6};
  static const int32_T iv1[2] = {1, 5};
  static const char_T rfmt[6] = {'%', '1', '4', '.', '6', 'e'};
  static const char_T b_rfmt[5] = {'%', '9', '.', '1', 'e'};
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  emxArray_real_T *errsubs;
  emxArray_real_T *fx;
  emxArray_real_T *qsubs;
  emxArray_real_T *subs;
  emxArray_real_T *t;
  emxArray_real_T *w;
  emxArray_real_T *x;
  const mxArray *b_y;
  const mxArray *m;
  const mxArray *y;
  const real_T *interval_data;
  real_T err_ok;
  real_T q;
  real_T q_ok;
  real_T *errsubs_data;
  real_T *qsubs_data;
  real_T *subs_data;
  real_T *t_data;
  real_T *w_data;
  real_T *x_data;
  int32_T j;
  int32_T k;
  int32_T nsubs;
  int32_T nx;
  boolean_T firstIteration;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  interval_data = interval->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  *success = true;
  emxInit_real_T(sp, &subs, 2, &vb_emlrtRTEI);
  nx = subs->size[0] * subs->size[1];
  subs->size[0] = 2;
  subs->size[1] = 16383;
  emxEnsureCapacity_real_T(sp, subs, nx, &nb_emlrtRTEI);
  subs_data = subs->data;
  emxInit_real_T(sp, &qsubs, 2, &wb_emlrtRTEI);
  nx = qsubs->size[0] * qsubs->size[1];
  qsubs->size[0] = 1;
  qsubs->size[1] = 16383;
  emxEnsureCapacity_real_T(sp, qsubs, nx, &ob_emlrtRTEI);
  qsubs_data = qsubs->data;
  emxInit_real_T(sp, &errsubs, 2, &xb_emlrtRTEI);
  nx = errsubs->size[0] * errsubs->size[1];
  errsubs->size[0] = 1;
  errsubs->size[1] = 16383;
  emxEnsureCapacity_real_T(sp, errsubs, nx, &pb_emlrtRTEI);
  errsubs_data = errsubs->data;
  nsubs = nt - 1;
  st.site = &vb_emlrtRSI;
  if (nt - 1 > 2147483646) {
    b_st.site = &j_emlrtRSI;
    check_forloop_overflow_error(&b_st);
  }
  for (k = 0; k < nsubs; k++) {
    subs_data[2 * k] = interval_data[k];
    subs_data[2 * k + 1] = interval_data[k + 1];
  }
  q = 0.0;
  *errbnd = 0.0;
  q_ok = 0.0;
  err_ok = 0.0;
  firstIteration = true;
  emxInit_real_T(sp, &x, 2, &yb_emlrtRTEI);
  emxInit_real_T(sp, &fx, 2, &ac_emlrtRTEI);
  emxInit_real_T(sp, &t, 2, &bc_emlrtRTEI);
  emxInit_real_T(sp, &w, 2, &bc_emlrtRTEI);
  int32_T exitg1;
  do {
    real_T absti;
    real_T halfh;
    real_T midpt;
    real_T offset;
    int32_T b;
    int32_T loop_ub;
    boolean_T guard1;
    exitg1 = 0;
    nx = nsubs * 15;
    if (nx > 245745) {
      emlrtErrorWithMessageIdR2018a(sp, &i_emlrtRTEI,
                                    "Coder:builtins:AssertionFailed",
                                    "Coder:builtins:AssertionFailed", 0);
    }
    b = x->size[0] * x->size[1];
    x->size[0] = 1;
    x->size[1] = nx;
    emxEnsureCapacity_real_T(sp, x, b, &qb_emlrtRTEI);
    x_data = x->data;
    nx = -1;
    st.site = &wb_emlrtRSI;
    if (nsubs > 2147483646) {
      b_st.site = &j_emlrtRSI;
      check_forloop_overflow_error(&b_st);
    }
    for (k = 0; k < nsubs; k++) {
      absti = subs_data[2 * k];
      offset = subs_data[2 * k + 1];
      midpt = (absti + offset) / 2.0;
      halfh = (offset - absti) / 2.0;
      for (j = 0; j <= 12; j += 2) {
        _mm_storeu_pd(
            &x_data[(nx + j) + 1],
            _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&dv[j]), _mm_set1_pd(halfh)),
                       _mm_set1_pd(midpt)));
      }
      x_data[nx + 15] = dv[14] * halfh + midpt;
      nx += 15;
    }
    st.site = &xb_emlrtRSI;
    nx = t->size[0] * t->size[1];
    t->size[0] = 1;
    b = x->size[1];
    t->size[1] = x->size[1];
    emxEnsureCapacity_real_T(&st, t, nx, &rb_emlrtRTEI);
    t_data = t->data;
    nx = w->size[0] * w->size[1];
    w->size[0] = 1;
    w->size[1] = x->size[1];
    emxEnsureCapacity_real_T(&st, w, nx, &sb_emlrtRTEI);
    w_data = w->data;
    b_st.site = &jc_emlrtRSI;
    if (x->size[1] > 2147483646) {
      c_st.site = &j_emlrtRSI;
      check_forloop_overflow_error(&c_st);
    }
    for (k = 0; k < b; k++) {
      midpt = x_data[k];
      absti = muDoubleScalarAbs(midpt);
      if (absti > 0.25) {
        b_st.site = &ic_emlrtRSI;
        c_st.site = &v_emlrtRSI;
        offset = 6.90776080947334 * ((absti - 1.0) * (absti - 1.0)) *
                 ((absti - 1.0) + 3.0);
        if (midpt < 0.0) {
          t_data[k] = offset;
        } else {
          t_data[k] = 27.631043237893358 - offset;
        }
        w_data[k] = -20.723282428420021 * (absti - 1.0) * ((absti - 1.0) + 2.0);
      } else {
        b_st.site = &hc_emlrtRSI;
        c_st.site = &v_emlrtRSI;
        absti = midpt * midpt;
        t_data[k] =
            6.90776080947334 * midpt * (3.0 - absti) + 13.815521618946679;
        w_data[k] = 20.723282428420021 * (1.0 - absti);
      }
    }
    guard1 = false;
    if (firstIteration) {
      uint32_T varargin_1[2];
      uint32_T varargin_2[2];
      boolean_T exitg2;
      st.site = &yb_emlrtRSI;
      b_st.site = &r_emlrtRSI;
      pfqn_mmint2_anonFcn1(&b_st, fun_workspace_Z, fun_workspace_nnzClasses,
                           fun_workspace_L, fun_workspace_N, t, fx);
      varargin_1[0] = 1U;
      varargin_2[0] = 1U;
      varargin_1[1] = (uint32_T)fx->size[1];
      varargin_2[1] = (uint32_T)x->size[1];
      firstIteration = true;
      nx = 0;
      exitg2 = false;
      while ((!exitg2) && (nx < 2)) {
        if ((int32_T)varargin_1[nx] != (int32_T)varargin_2[nx]) {
          firstIteration = false;
          exitg2 = true;
        } else {
          nx++;
        }
      }
      if (!firstIteration) {
        emlrtErrorWithMessageIdR2018a(sp, &j_emlrtRTEI,
                                      "MATLAB:integral:FxNotSameSizeAsX",
                                      "MATLAB:integral:FxNotSameSizeAsX", 0);
      }
      firstIteration = false;
      if (fx->size[1] == w->size[1]) {
        loop_ub = fx->size[1] - 1;
        nx = fx->size[0] * fx->size[1];
        fx->size[0] = 1;
        emxEnsureCapacity_real_T(sp, fx, nx, &tb_emlrtRTEI);
        x_data = fx->data;
        nx = (fx->size[1] / 2) << 1;
        b = nx - 2;
        for (k = 0; k <= b; k += 2) {
          __m128d r;
          __m128d r1;
          r = _mm_loadu_pd(&x_data[k]);
          r1 = _mm_loadu_pd(&w_data[k]);
          _mm_storeu_pd(&x_data[k], _mm_mul_pd(r, r1));
        }
        for (k = nx; k <= loop_ub; k++) {
          x_data[k] *= w_data[k];
        }
      } else {
        st.site = &ud_emlrtRSI;
        times(&st, fx, w);
        x_data = fx->data;
      }
      guard1 = true;
    } else {
      boolean_T exitg2;
      boolean_T tooClose;
      st.site = &ac_emlrtRSI;
      absti = muDoubleScalarAbs(t_data[0]);
      tooClose = false;
      b_st.site = &qc_emlrtRSI;
      if (t->size[1] > 2147483646) {
        c_st.site = &j_emlrtRSI;
        check_forloop_overflow_error(&c_st);
      }
      nx = 1;
      exitg2 = false;
      while ((!exitg2) && (nx + 1 <= t->size[1])) {
        offset = absti;
        absti = muDoubleScalarAbs(t_data[nx]);
        midpt = t_data[nx - 1];
        if (muDoubleScalarAbs(t_data[nx] - midpt) <=
            2.2204460492503131E-14 * muDoubleScalarMax(offset, absti)) {
          if (!emlrtSetWarningFlag(&st)) {
            char_T str[14];
            b_st.site = &rc_emlrtRSI;
            y = NULL;
            m = emlrtCreateCharArray(2, &iv[0]);
            emlrtInitCharArrayR2013a(&b_st, 6, m, &rfmt[0]);
            emlrtAssign(&y, m);
            c_st.site = &td_emlrtRSI;
            c_emlrt_marshallIn(
                &c_st,
                b_sprintf(&c_st, y, emlrt_marshallOut(midpt), &e_emlrtMCI),
                "<output of sprintf>", str);
            b_st.site = &sc_emlrtRSI;
            b_warning(&b_st, str);
          }
          tooClose = true;
          exitg2 = true;
        } else {
          nx++;
        }
      }
      if (tooClose) {
        *success = false;
        exitg1 = 1;
      } else {
        st.site = &bc_emlrtRSI;
        b_st.site = &r_emlrtRSI;
        pfqn_mmint2_anonFcn1(&b_st, fun_workspace_Z, fun_workspace_nnzClasses,
                             fun_workspace_L, fun_workspace_N, t, fx);
        if (fx->size[1] == w->size[1]) {
          loop_ub = fx->size[1] - 1;
          nx = fx->size[0] * fx->size[1];
          fx->size[0] = 1;
          emxEnsureCapacity_real_T(sp, fx, nx, &ub_emlrtRTEI);
          x_data = fx->data;
          nx = (fx->size[1] / 2) << 1;
          b = nx - 2;
          for (k = 0; k <= b; k += 2) {
            __m128d r;
            __m128d r1;
            r = _mm_loadu_pd(&x_data[k]);
            r1 = _mm_loadu_pd(&w_data[k]);
            _mm_storeu_pd(&x_data[k], _mm_mul_pd(r, r1));
          }
          for (k = nx; k <= loop_ub; k++) {
            x_data[k] *= w_data[k];
          }
        } else {
          st.site = &bc_emlrtRSI;
          times(&st, fx, w);
          x_data = fx->data;
        }
        guard1 = true;
      }
    }
    if (guard1) {
      int32_T nrefine;
      nx = -1;
      q = 0.0;
      st.site = &cc_emlrtRSI;
      for (k = 0; k < nsubs; k++) {
        offset = 0.0;
        midpt = 0.0;
        for (j = 0; j < 15; j++) {
          absti = x_data[(nx + j) + 1];
          offset += dv1[j] * absti;
          midpt += dv2[j] * absti;
        }
        nx += 15;
        halfh = (subs_data[2 * k + 1] - subs_data[2 * k]) / 2.0;
        absti = offset * halfh;
        qsubs_data[k] = absti;
        errsubs_data[k] = midpt * halfh;
        q += absti;
      }
      q += q_ok;
      absti = muDoubleScalarMax(1.0E-12, muDoubleScalarAbs(q) * 1.0E-6);
      offset = 2.0 * absti / pathlen;
      midpt = 0.0;
      nrefine = 0;
      st.site = &dc_emlrtRSI;
      for (k = 0; k < nsubs; k++) {
        real_T abserrsubsk;
        halfh = errsubs_data[k];
        abserrsubsk = muDoubleScalarAbs(halfh);
        nx = 2 * k + 1;
        if (abserrsubsk < offset * ((subs_data[nx] - subs_data[2 * k]) / 2.0)) {
          err_ok += halfh;
          q_ok += qsubs_data[k];
        } else {
          midpt += abserrsubsk;
          nrefine++;
          b = 2 * (nrefine - 1);
          subs_data[b] = subs_data[2 * k];
          subs_data[b + 1] = subs_data[nx];
        }
      }
      *errbnd = muDoubleScalarAbs(err_ok) + midpt;
      if ((!muDoubleScalarIsInf(q)) && (!muDoubleScalarIsNaN(q)) &&
          ((!muDoubleScalarIsInf(*errbnd)) &&
           (!muDoubleScalarIsNaN(*errbnd)))) {
        if ((*errbnd < absti) || (nrefine == 0)) {
          exitg1 = 1;
        } else {
          nsubs = nrefine << 1;
          if (nsubs >= 16384) {
            if (!emlrtSetWarningFlag((emlrtCTX)sp)) {
              char_T b_str[9];
              st.site = &fc_emlrtRSI;
              b_y = NULL;
              m = emlrtCreateCharArray(2, &iv1[0]);
              emlrtInitCharArrayR2013a(&st, 5, m, &b_rfmt[0]);
              emlrtAssign(&b_y, m);
              b_st.site = &td_emlrtRSI;
              e_emlrt_marshallIn(&b_st,
                                 b_sprintf(&b_st, b_y,
                                           emlrt_marshallOut(*errbnd),
                                           &e_emlrtMCI),
                                 "<output of sprintf>", b_str);
              st.site = &gc_emlrtRSI;
              c_warning(&st, b_str);
            }
            *success = false;
            exitg1 = 1;
          } else {
            for (k = nrefine; k >= 1; k--) {
              nx = (k << 1) - 1;
              b = 2 * (k - 1);
              subs_data[2 * nx + 1] = subs_data[b + 1];
              subs_data[2 * nx] = (subs_data[b] + subs_data[b + 1]) / 2.0;
              loop_ub = 2 * (nx - 1);
              subs_data[loop_ub + 1] = subs_data[2 * nx];
              subs_data[loop_ub] = subs_data[b];
            }
          }
        }
      } else {
        if (emlrtSetWarningFlag((emlrtCTX)sp)) {
          st.site = &ec_emlrtRSI;
          warning(&st);
        }
        *success = false;
        exitg1 = 1;
      }
    }
  } while (exitg1 == 0);
  emxFree_real_T(sp, &w);
  emxFree_real_T(sp, &t);
  emxFree_real_T(sp, &fx);
  emxFree_real_T(sp, &x);
  emxFree_real_T(sp, &errsubs);
  emxFree_real_T(sp, &qsubs);
  emxFree_real_T(sp, &subs);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return q;
}

/* End of code generation (integralCalc.c) */
