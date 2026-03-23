/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mmint2.c
 *
 * Code generation for function 'pfqn_mmint2'
 *
 */

/* Include files */
#include "pfqn_mmint2.h"
#include "allOrAny.h"
#include "applyScalarFunctionInPlace.h"
#include "eml_int_forloop_overflow_check.h"
#include "handle.h"
#include "indexShapeCheck.h"
#include "integralCalc.h"
#include "ixfun.h"
#include "pfqn_mmint2_data.h"
#include "pfqn_mmint2_emxutil.h"
#include "pfqn_mmint2_internal_types.h"
#include "pfqn_mmint2_mexutil.h"
#include "pfqn_mmint2_types.h"
#include "power.h"
#include "prod.h"
#include "rt_nonfinite.h"
#include "sumMatrixIncludeNaN.h"
#include "vAllOrAny.h"
#include "warning.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>

/* Variable Definitions */
static emlrtRSInfo emlrtRSI =
    {
        22,            /* lineNo */
        "pfqn_mmint2", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pathName */
};

static emlrtRSInfo b_emlrtRSI =
    {
        30,            /* lineNo */
        "pfqn_mmint2", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pathName */
};

static emlrtRSInfo c_emlrtRSI =
    {
        31,            /* lineNo */
        "pfqn_mmint2", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pathName */
};

static emlrtRSInfo d_emlrtRSI =
    {
        33,            /* lineNo */
        "pfqn_mmint2", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pathName */
};

static emlrtRSInfo e_emlrtRSI =
    {
        34,            /* lineNo */
        "pfqn_mmint2", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pathName */
};

static emlrtRSInfo f_emlrtRSI =
    {
        29,            /* lineNo */
        "pfqn_mmint2", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pathName */
};

static emlrtRSInfo g_emlrtRSI = {
    39,                                                            /* lineNo */
    "find",                                                        /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pathName
                                                                    */
};

static emlrtRSInfo h_emlrtRSI = {
    138,                                                           /* lineNo */
    "eml_find",                                                    /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pathName
                                                                    */
};

static emlrtRSInfo i_emlrtRSI = {
    376,                                                           /* lineNo */
    "find_first_indices",                                          /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pathName
                                                                    */
};

static emlrtRSInfo k_emlrtRSI = {
    51,        /* lineNo */
    "warning", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/lang/warning.m" /* pathName
                                                                      */
};

static emlrtRSInfo l_emlrtRSI =
    {
        71,                         /* lineNo */
        "WarningState/callWarning", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "WarningState.m" /* pathName */
};

static emlrtRSInfo m_emlrtRSI =
    {
        54,                          /* lineNo */
        "WarningState/WarningState", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "WarningState.m" /* pathName */
};

static emlrtRSInfo n_emlrtRSI = {
    53,        /* lineNo */
    "warning", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/lang/warning.m" /* pathName
                                                                      */
};

static emlrtRSInfo o_emlrtRSI = {
    9,          /* lineNo */
    "integral", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/integral.m" /* pathName
                                                                         */
};

static emlrtRSInfo p_emlrtRSI = {
    10,         /* lineNo */
    "integral", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/integral.m" /* pathName
                                                                         */
};

static emlrtRSInfo q_emlrtRSI = {
    20,          /* lineNo */
    "parseArgs", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/integral.m" /* pathName
                                                                         */
};

static emlrtRSInfo s_emlrtRSI =
    {
        25, /* lineNo */
        "@(u)(exp(-u\').*prod((Z(nnzClasses)+L(nnzClasses).*repmat(u(:),1,"
        "length(nnzClasses))).^N(nnzClasses),2))\'", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pathName */
};

static emlrtRSInfo u_emlrtRSI = {
    34,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

static emlrtRSInfo pb_emlrtRSI = {
    126,            /* lineNo */
    "integralCalc", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo qb_emlrtRSI = {
    136,            /* lineNo */
    "integralCalc", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo rb_emlrtRSI = {
    140,            /* lineNo */
    "integralCalc", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo sb_emlrtRSI = {
    683,     /* lineNo */
    "split", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo tb_emlrtRSI = {
    720,        /* lineNo */
    "midpArea", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo ub_emlrtRSI = {
    723,        /* lineNo */
    "midpArea", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pathName */
};

static emlrtRSInfo kc_emlrtRSI = {
    10,                                                           /* lineNo */
    "exp",                                                        /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/exp.m" /* pathName */
};

static emlrtRSInfo lc_emlrtRSI = {
    44,                           /* lineNo */
    "applyScalarFunctionInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunctionInPlace.m" /* pathName */
};

static emlrtRSInfo mc_emlrtRSI = {
    78,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

static emlrtRSInfo nc_emlrtRSI = {
    85,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

static emlrtRSInfo oc_emlrtRSI = {
    193,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "combineVectorElements.m" /* pathName */
};

static emlrtRSInfo pc_emlrtRSI = {
    211,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "combineVectorElements.m" /* pathName */
};

static emlrtRSInfo tc_emlrtRSI = {
    8,                                                             /* lineNo */
    "factln",                                                      /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/factln.m" /* pathName
                                                                    */
};

static emlrtRSInfo uc_emlrtRSI = {
    10,        /* lineNo */
    "gammaln", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/gammaln.m" /* pathName
                                                                         */
};

static emlrtRSInfo cd_emlrtRSI = {
    20,    /* lineNo */
    "sum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sum.m" /* pathName
                                                                     */
};

static emlrtRSInfo dd_emlrtRSI = {
    86,                      /* lineNo */
    "combineVectorElements", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "combineVectorElements.m" /* pathName */
};

static emlrtRSInfo ed_emlrtRSI = {
    99,                 /* lineNo */
    "blockedSummation", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo fd_emlrtRSI = {
    22,                    /* lineNo */
    "sumMatrixIncludeNaN", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtRSInfo gd_emlrtRSI = {
    42,                 /* lineNo */
    "sumMatrixColumns", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtRSInfo id_emlrtRSI = {
    57,                 /* lineNo */
    "sumMatrixColumns", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtMCInfo emlrtMCI =
    {
        70,                         /* lineNo */
        27,                         /* colNo */
        "WarningState/callWarning", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "WarningState.m" /* pName */
};

static emlrtMCInfo b_emlrtMCI =
    {
        84,                         /* lineNo */
        21,                         /* colNo */
        "WarningState/callWarning", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "WarningState.m" /* pName */
};

static emlrtMCInfo f_emlrtMCI =
    {
        34,                     /* lineNo */
        21,                     /* colNo */
        "WarningState/warning", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "WarningState.m" /* pName */
};

static emlrtRTEInfo emlrtRTEI = {
    386,                                                           /* lineNo */
    1,                                                             /* colNo */
    "find_first_indices",                                          /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pName */
};

static emlrtRTEInfo b_emlrtRTEI = {
    14,                                                           /* lineNo */
    9,                                                            /* colNo */
    "log",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/log.m" /* pName */
};

static emlrtECInfo emlrtECI =
    {
        2,  /* nDims */
        25, /* lineNo */
        26, /* colNo */
        "@(u)(exp(-u\').*prod((Z(nnzClasses)+L(nnzClasses).*repmat(u(:),1,"
        "length(nnzClasses))).^N(nnzClasses),2))\'", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pName */
};

static emlrtBCInfo emlrtBCI =
    {
        -1,  /* iFirst */
        -1,  /* iLast */
        25,  /* lineNo */
        42,  /* colNo */
        "L", /* aName */
        "@(u)(exp(-u\').*prod((Z(nnzClasses)+L(nnzClasses).*repmat(u(:),1,"
        "length(nnzClasses))).^N(nnzClasses),2))\'", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m", /* pName */
        0                /* checkKind */
};

static emlrtBCInfo b_emlrtBCI =
    {
        -1,  /* iFirst */
        -1,  /* iLast */
        25,  /* lineNo */
        28,  /* colNo */
        "Z", /* aName */
        "@(u)(exp(-u\').*prod((Z(nnzClasses)+L(nnzClasses).*repmat(u(:),1,"
        "length(nnzClasses))).^N(nnzClasses),2))\'", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m", /* pName */
        0                /* checkKind */
};

static emlrtBCInfo c_emlrtBCI =
    {
        -1,  /* iFirst */
        -1,  /* iLast */
        25,  /* lineNo */
        93,  /* colNo */
        "N", /* aName */
        "@(u)(exp(-u\').*prod((Z(nnzClasses)+L(nnzClasses).*repmat(u(:),1,"
        "length(nnzClasses))).^N(nnzClasses),2))\'", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m", /* pName */
        0                /* checkKind */
};

static emlrtECInfo b_emlrtECI =
    {
        1,  /* nDims */
        25, /* lineNo */
        10, /* colNo */
        "@(u)(exp(-u\').*prod((Z(nnzClasses)+L(nnzClasses).*repmat(u(:),1,"
        "length(nnzClasses))).^N(nnzClasses),2))\'", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pName */
};

static emlrtRTEInfo l_emlrtRTEI = {
    358,                                                           /* lineNo */
    24,                                                            /* colNo */
    "find",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pName */
};

static emlrtRTEInfo m_emlrtRTEI = {
    39,                                                            /* lineNo */
    5,                                                             /* colNo */
    "find",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pName */
};

static emlrtRTEInfo n_emlrtRTEI =
    {
        25,            /* lineNo */
        1,             /* colNo */
        "pfqn_mmint2", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pName */
};

static emlrtRTEInfo o_emlrtRTEI = {
    8,                                                             /* lineNo */
    1,                                                             /* colNo */
    "factln",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/factln.m" /* pName */
};

static emlrtRTEInfo p_emlrtRTEI =
    {
        25,            /* lineNo */
        40,            /* colNo */
        "pfqn_mmint2", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pName */
};

static emlrtRTEInfo q_emlrtRTEI =
    {
        25,            /* lineNo */
        26,            /* colNo */
        "pfqn_mmint2", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pName */
};

static emlrtRTEInfo r_emlrtRTEI = {
    15,             /* lineNo */
    29,             /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo s_emlrtRTEI =
    {
        25,            /* lineNo */
        91,            /* colNo */
        "pfqn_mmint2", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pName */
};

static emlrtRTEInfo t_emlrtRTEI = {
    94,             /* lineNo */
    9,              /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo u_emlrtRTEI =
    {
        19,            /* lineNo */
        18,            /* colNo */
        "pfqn_mmint2", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pName */
};

static emlrtRTEInfo v_emlrtRTEI = {
    696,            /* lineNo */
    9,              /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo w_emlrtRTEI = {
    693,            /* lineNo */
    9,              /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo x_emlrtRTEI = {
    31,                                                            /* lineNo */
    6,                                                             /* colNo */
    "find",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pName */
};

static emlrtRTEInfo y_emlrtRTEI = {
    8,                                                             /* lineNo */
    14,                                                            /* colNo */
    "factln",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/factln.m" /* pName */
};

static emlrtRTEInfo ab_emlrtRTEI = {
    15,             /* lineNo */
    3,              /* colNo */
    "integralCalc", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/funfun/private/"
    "integralCalc.m" /* pName */
};

static emlrtRTEInfo fb_emlrtRTEI = {
    73,       /* lineNo */
    28,       /* colNo */
    "repmat", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pName */
};

static emlrtRTEInfo gb_emlrtRTEI =
    {
        25,            /* lineNo */
        14,            /* colNo */
        "pfqn_mmint2", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pName */
};

static emlrtRTEInfo hb_emlrtRTEI =
    {
        25,            /* lineNo */
        20,            /* colNo */
        "pfqn_mmint2", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pName */
};

static emlrtRTEInfo ib_emlrtRTEI = {
    181,                     /* lineNo */
    24,                      /* colNo */
    "combineVectorElements", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "combineVectorElements.m" /* pName */
};

static emlrtRTEInfo jb_emlrtRTEI =
    {
        25,            /* lineNo */
        4,             /* colNo */
        "pfqn_mmint2", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pName */
};

static emlrtRTEInfo kb_emlrtRTEI =
    {
        25,            /* lineNo */
        10,            /* colNo */
        "pfqn_mmint2", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pName */
};

static emlrtRTEInfo lb_emlrtRTEI =
    {
        25,            /* lineNo */
        25,            /* colNo */
        "pfqn_mmint2", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mmint2.m" /* pName */
};

static emlrtRSInfo od_emlrtRSI =
    {
        70,                         /* lineNo */
        "WarningState/callWarning", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "WarningState.m" /* pathName */
};

static emlrtRSInfo rd_emlrtRSI =
    {
        84,                         /* lineNo */
        "WarningState/callWarning", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "WarningState.m" /* pathName */
};

static emlrtRSInfo sd_emlrtRSI =
    {
        34,                     /* lineNo */
        "WarningState/warning", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "WarningState.m" /* pathName */
};

/* Function Declarations */
static int32_T b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                  const emlrtMsgIdentifier *parentId);

static void binary_expand_op(const emlrtStack *sp, emxArray_real_T *in1,
                             const emlrtRSInfo in2, const emxArray_real_T *in3,
                             const emxArray_real_T *in4,
                             const emxArray_real_T *in5);

static void binary_expand_op_4(const emlrtStack *sp, emxArray_real_T *in1,
                               const emxArray_real_T *in2,
                               const emxArray_real_T *in3);

static void binary_expand_op_5(const emlrtStack *sp, emxArray_real_T *in1,
                               const emxArray_real_T *in2,
                               const emxArray_real_T *in3,
                               const emxArray_real_T *in4);

static void binary_expand_op_6(const emlrtStack *sp, emxArray_real_T *in1,
                               const emxArray_real_T *in2,
                               const emxArray_real_T *in3,
                               const emxArray_real_T *in4);

static int32_T emlrt_marshallIn(const emlrtStack *sp,
                                const mxArray *a__output_of_feval_,
                                const char_T *identifier);

static const mxArray *feval(const emlrtStack *sp, const mxArray *m,
                            emlrtMCInfo *location);

static int32_T k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                  const emlrtMsgIdentifier *msgId);

/* Function Definitions */
static int32_T b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                  const emlrtMsgIdentifier *parentId)
{
  int32_T y;
  y = k_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

static void binary_expand_op(const emlrtStack *sp, emxArray_real_T *in1,
                             const emlrtRSInfo in2, const emxArray_real_T *in3,
                             const emxArray_real_T *in4,
                             const emxArray_real_T *in5)
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack st;
  emxArray_real_T *b_in3;
  emxArray_real_T *c_in5;
  const real_T *in3_data;
  const real_T *in4_data;
  const real_T *in5_data;
  real_T *b_in3_data;
  real_T *in1_data;
  int32_T b_in5;
  int32_T binary_expand_op_numThreads;
  int32_T i;
  int32_T i1;
  int32_T loop_ub;
  int32_T stride_0_1;
  int32_T stride_1_1;
  st.prev = sp;
  st.tls = sp->tls;
  in5_data = in5->data;
  in4_data = in4->data;
  in3_data = in3->data;
  in1_data = in1->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  b_in5 = in5->size[1];
  emxInit_real_T(sp, &b_in3, 2, &q_emlrtRTEI);
  stride_0_1 = b_in3->size[0] * b_in3->size[1];
  b_in3->size[0] = 1;
  if (in1->size[1] == 1) {
    loop_ub = in4->size[1];
  } else {
    loop_ub = in1->size[1];
  }
  b_in3->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, b_in3, stride_0_1, &q_emlrtRTEI);
  b_in3_data = b_in3->data;
  stride_0_1 = (in4->size[1] != 1);
  stride_1_1 = (in1->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      b_in3_data[i] = in3_data[(int32_T)in4_data[i * stride_0_1] - 1] +
                      in1_data[i * stride_1_1];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    binary_expand_op_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(binary_expand_op_numThreads)

    for (i = 0; i < loop_ub; i++) {
      b_in3_data[i] = in3_data[(int32_T)in4_data[i * stride_0_1] - 1] +
                      in1_data[i * stride_1_1];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  emxInit_real_T(sp, &c_in5, 2, &s_emlrtRTEI);
  stride_0_1 = c_in5->size[0] * c_in5->size[1];
  c_in5->size[0] = 1;
  stride_1_1 = in4->size[1];
  c_in5->size[1] = stride_1_1;
  emxEnsureCapacity_real_T(sp, c_in5, stride_0_1, &s_emlrtRTEI);
  in1_data = c_in5->data;
  for (i1 = 0; i1 < stride_1_1; i1++) {
    stride_0_1 = (int32_T)in4_data[i1];
    if ((stride_0_1 < 1) || (stride_0_1 > b_in5)) {
      emlrtDynamicBoundsCheckR2012b(stride_0_1, 1, b_in5, &c_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    in1_data[i1] = in5_data[stride_0_1 - 1];
  }
  st.site = (emlrtRSInfo *)&in2;
  power(&st, b_in3, c_in5, in1);
  emxFree_real_T(sp, &c_in5);
  emxFree_real_T(sp, &b_in3);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

static void binary_expand_op_4(const emlrtStack *sp, emxArray_real_T *in1,
                               const emxArray_real_T *in2,
                               const emxArray_real_T *in3)
{
  jmp_buf *volatile emlrtJBStack;
  const real_T *in2_data;
  const real_T *in3_data;
  real_T *in1_data;
  int32_T binary_expand_op_4_numThreads;
  int32_T i;
  int32_T loop_ub;
  int32_T stride_0_1;
  in3_data = in3->data;
  in2_data = in2->data;
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[0] = 1;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &jb_emlrtRTEI);
  loop_ub = in2->size[0];
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &jb_emlrtRTEI);
  in1_data = in1->data;
  stride_0_1 = (in3->size[0] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      in1_data[i] = in2_data[i] * in3_data[i * stride_0_1];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    binary_expand_op_4_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(binary_expand_op_4_numThreads)

    for (i = 0; i < loop_ub; i++) {
      in1_data[i] = in2_data[i] * in3_data[i * stride_0_1];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
}

static void binary_expand_op_5(const emlrtStack *sp, emxArray_real_T *in1,
                               const emxArray_real_T *in2,
                               const emxArray_real_T *in3,
                               const emxArray_real_T *in4)
{
  const real_T *in2_data;
  const real_T *in3_data;
  const real_T *in4_data;
  real_T *in1_data;
  int32_T aux_0_1;
  int32_T aux_1_1;
  int32_T b_loop_ub;
  int32_T i;
  int32_T i1;
  int32_T loop_ub;
  int32_T stride_0_1;
  int32_T stride_1_1;
  in4_data = in4->data;
  in3_data = in3->data;
  in2_data = in2->data;
  loop_ub = in4->size[0];
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[0] = loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &q_emlrtRTEI);
  if (in4->size[1] == 1) {
    b_loop_ub = in3->size[1];
  } else {
    b_loop_ub = in4->size[1];
  }
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &q_emlrtRTEI);
  in1_data = in1->data;
  stride_0_1 = (in3->size[1] != 1);
  stride_1_1 = (in4->size[1] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    int32_T scalarLB;
    int32_T vectorUB;
    scalarLB = (loop_ub / 2) << 1;
    vectorUB = scalarLB - 2;
    for (i1 = 0; i1 <= vectorUB; i1 += 2) {
      __m128d r;
      r = _mm_loadu_pd(&in4_data[i1 + in4->size[0] * aux_1_1]);
      _mm_storeu_pd(
          &in1_data[i1 + in1->size[0] * i],
          _mm_add_pd(_mm_set1_pd(in2_data[(int32_T)in3_data[aux_0_1] - 1]), r));
    }
    for (i1 = scalarLB; i1 < loop_ub; i1++) {
      in1_data[i1 + in1->size[0] * i] =
          in2_data[(int32_T)in3_data[aux_0_1] - 1] +
          in4_data[i1 + in4->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_1;
    aux_0_1 += stride_0_1;
  }
}

static void binary_expand_op_6(const emlrtStack *sp, emxArray_real_T *in1,
                               const emxArray_real_T *in2,
                               const emxArray_real_T *in3,
                               const emxArray_real_T *in4)
{
  const real_T *in2_data;
  const real_T *in3_data;
  const real_T *in4_data;
  real_T *in1_data;
  int32_T aux_0_1;
  int32_T aux_1_1;
  int32_T b_loop_ub;
  int32_T i;
  int32_T i1;
  int32_T loop_ub;
  int32_T stride_0_1;
  int32_T stride_1_1;
  in4_data = in4->data;
  in3_data = in3->data;
  in2_data = in2->data;
  loop_ub = in4->size[0];
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[0] = loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &p_emlrtRTEI);
  if (in4->size[1] == 1) {
    b_loop_ub = in3->size[1];
  } else {
    b_loop_ub = in4->size[1];
  }
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &p_emlrtRTEI);
  in1_data = in1->data;
  stride_0_1 = (in3->size[1] != 1);
  stride_1_1 = (in4->size[1] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    int32_T scalarLB;
    int32_T vectorUB;
    scalarLB = (loop_ub / 2) << 1;
    vectorUB = scalarLB - 2;
    for (i1 = 0; i1 <= vectorUB; i1 += 2) {
      __m128d r;
      r = _mm_loadu_pd(&in4_data[i1 + in4->size[0] * aux_1_1]);
      _mm_storeu_pd(
          &in1_data[i1 + in1->size[0] * i],
          _mm_mul_pd(_mm_set1_pd(in2_data[(int32_T)in3_data[aux_0_1] - 1]), r));
    }
    for (i1 = scalarLB; i1 < loop_ub; i1++) {
      in1_data[i1 + in1->size[0] * i] =
          in2_data[(int32_T)in3_data[aux_0_1] - 1] *
          in4_data[i1 + in4->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_1;
    aux_0_1 += stride_0_1;
  }
}

static int32_T emlrt_marshallIn(const emlrtStack *sp,
                                const mxArray *a__output_of_feval_,
                                const char_T *identifier)
{
  emlrtMsgIdentifier thisId;
  int32_T y;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(a__output_of_feval_), &thisId);
  emlrtDestroyArray(&a__output_of_feval_);
  return y;
}

static const mxArray *feval(const emlrtStack *sp, const mxArray *m,
                            emlrtMCInfo *location)
{
  const mxArray *m1;
  return emlrtCallMATLABR2012b((emlrtConstCTX)sp, 1, &m1, 1, &m, "feval", true,
                               location);
}

static int32_T k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                  const emlrtMsgIdentifier *msgId)
{
  static const int32_T dims = 0;
  int32_T ret;
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "int32", false, 0U,
                          (const void *)&dims);
  ret = *(int32_T *)emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

emlrtCTX emlrtGetRootTLSGlobal(void)
{
  return emlrtRootTLSGlobal;
}

void emlrtLockerFunction(EmlrtLockeeFunction aLockee, emlrtConstCTX aTLS,
                         void *aData)
{
  omp_set_lock(&emlrtLockGlobal);
  emlrtCallLockeeFunction(aLockee, aTLS, aData);
  omp_unset_lock(&emlrtLockGlobal);
}

void pfqn_mmint2(const emlrtStack *sp, const emxArray_real_T *L,
                 const emxArray_real_T *N, const emxArray_real_T *Z, real_T *G,
                 real_T *lG)
{
  static const int32_T iv[2] = {1, 50};
  static const int32_T iv1[2] = {1, 7};
  static const int32_T iv2[2] = {1, 3};
  static const int32_T iv3[2] = {1, 51};
  static const char_T c_u[51] = {
      'c', 'o', 'd', 'e', 'r', '.', 'i', 'n', 't', 'e', 'r', 'n', 'a',
      'l', '.', 'E', 'x', 't', 'r', 'i', 'n', 's', 'i', 'c', 'W', 'a',
      'r', 'n', 'i', 'n', 'g', 'M', 'a', 'n', 'a', 'g', 'e', 'r', '.',
      'r', 'e', 's', 't', 'o', 'r', 'e', 'S', 't', 'a', 't', 'e'};
  static const char_T u[50] = {
      'c', 'o', 'd', 'e', 'r', '.', 'i', 'n', 't', 'e', 'r', 'n', 'a',
      'l', '.', 'E', 'x', 't', 'r', 'i', 'n', 's', 'i', 'c', 'W', 'a',
      'r', 'n', 'i', 'n', 'g', 'M', 'a', 'n', 'a', 'g', 'e', 'r', '.',
      'c', 'a', 'l', 'l', 'W', 'a', 'r', 'n', 'i', 'n', 'g'};
  static const char_T b_u[7] = {'w', 'a', 'r', 'n', 'i', 'n', 'g'};
  static const char_T varargin_1[3] = {'o', 'f', 'f'};
  jmp_buf *volatile emlrtJBStack;
  coder_internal_WarningState lobj_0[2];
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack g_st;
  emlrtStack st;
  emxArray_int32_T *ii;
  emxArray_real_T b_x;
  emxArray_real_T *b_N;
  emxArray_real_T *b_Z;
  emxArray_real_T *f_workspace_nnzClasses;
  emxArray_real_T *interval;
  emxArray_real_T *x;
  const mxArray *b_y;
  const mxArray *c_y;
  const mxArray *d_y;
  const mxArray *e_y;
  const mxArray *m;
  const mxArray *y;
  const real_T *L_data;
  const real_T *N_data;
  const real_T *Z_data;
  real_T delta;
  real_T pathlen;
  real_T s;
  real_T *b_Z_data;
  real_T *f_workspace_nnzClasses_data;
  real_T *interval_data;
  int32_T b_L;
  int32_T b_nx;
  int32_T c_x;
  int32_T d_x;
  int32_T e_x;
  int32_T f_x;
  int32_T i;
  int32_T i1;
  int32_T i2;
  int32_T i3;
  int32_T idx;
  int32_T j;
  int32_T lidx;
  int32_T loop_ub;
  int32_T nx;
  int32_T pfqn_mmint2_numThreads;
  int32_T *ii_data;
  boolean_T exitg1;
  boolean_T success;
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
  Z_data = Z->data;
  N_data = N->data;
  L_data = L->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &f_workspace_nnzClasses, 2, &n_emlrtRTEI);
  emxInit_int32_T(sp, &ii, &x_emlrtRTEI);
  emxInit_real_T(sp, &x, 2, &y_emlrtRTEI);
  emxInit_real_T(sp, &interval, 2, &ab_emlrtRTEI);
  emxInit_real_T(sp, &b_Z, 2, &q_emlrtRTEI);
  emxInit_real_T(sp, &b_N, 2, &s_emlrtRTEI);
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emlrtPushHeapReferenceStackR2021a((emlrtCTX)sp, true, &lobj_0[0],
                                    (void *)&handle_matlabCodegenDestructor,
                                    NULL, NULL, NULL);
  emlrtPushHeapReferenceStackR2021a((emlrtCTX)sp, true, &lobj_0[1],
                                    (void *)&handle_matlabCodegenDestructor,
                                    NULL, NULL, NULL);
  lobj_0[0].matlabCodegenIsDeleted = true;
  lobj_0[1].matlabCodegenIsDeleted = true;
  /* { */
  /*  % @file pfqn_mmint2.m */
  /*  % @brief McKenna-Mitra integral form for repairman models using MATLAB
   * integral. */
  /* } */
  /* { */
  /*  % @brief McKenna-Mitra integral form for repairman models using MATLAB
   * integral. */
  /*  % @fn pfqn_mmint2(L, N, Z) */
  /*  % @param L Service demand vector. */
  /*  % @param N Population vector. */
  /*  % @param Z Think time vector. */
  /*  % @return G Normalizing constant. */
  /*  % @return lG Logarithm of normalizing constant. */
  /* } */
  /*  [G,LOGG] = PFQN_MMINT2(L,N,Z) */
  st.site = &emlrtRSI;
  b_st.site = &g_emlrtRSI;
  nx = N->size[1];
  c_st.site = &h_emlrtRSI;
  idx = 0;
  b_nx = ii->size[0] * ii->size[1];
  ii->size[0] = 1;
  ii->size[1] = N->size[1];
  emxEnsureCapacity_int32_T(&c_st, ii, b_nx, &l_emlrtRTEI);
  ii_data = ii->data;
  d_st.site = &i_emlrtRSI;
  if (N->size[1] > 2147483646) {
    e_st.site = &j_emlrtRSI;
    check_forloop_overflow_error(&e_st);
  }
  b_nx = 0;
  exitg1 = false;
  while ((!exitg1) && (b_nx <= nx - 1)) {
    if (N_data[b_nx] != 0.0) {
      idx++;
      ii_data[idx - 1] = b_nx + 1;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        b_nx++;
      }
    } else {
      b_nx++;
    }
  }
  if (idx > N->size[1]) {
    emlrtErrorWithMessageIdR2018a(&c_st, &emlrtRTEI,
                                  "Coder:builtins:AssertionFailed",
                                  "Coder:builtins:AssertionFailed", 0);
  }
  if (N->size[1] == 1) {
    if (idx == 0) {
      ii->size[0] = 1;
      ii->size[1] = 0;
    }
  } else {
    b_nx = ii->size[0] * ii->size[1];
    if (idx < 1) {
      ii->size[1] = 0;
    } else {
      ii->size[1] = idx;
    }
    emxEnsureCapacity_int32_T(&c_st, ii, b_nx, &m_emlrtRTEI);
    ii_data = ii->data;
  }
  b_nx = f_workspace_nnzClasses->size[0] * f_workspace_nnzClasses->size[1];
  f_workspace_nnzClasses->size[0] = 1;
  loop_ub = ii->size[1];
  f_workspace_nnzClasses->size[1] = ii->size[1];
  emxEnsureCapacity_real_T(&st, f_workspace_nnzClasses, b_nx, &n_emlrtRTEI);
  f_workspace_nnzClasses_data = f_workspace_nnzClasses->data;
  for (j = 0; j < loop_ub; j++) {
    f_workspace_nnzClasses_data[j] = ii_data[j];
  }
  /*  repairmen integration */
  /*  cutoff for exponential term */
  st.site = &f_emlrtRSI;
  b_st.site = &k_emlrtRSI;
  y = NULL;
  m = emlrtCreateCharArray(2, &iv[0]);
  emlrtInitCharArrayR2013a(&b_st, 50, m, &u[0]);
  emlrtAssign(&y, m);
  c_st.site = &od_emlrtRSI;
  b_nx =
      emlrt_marshallIn(&c_st, feval(&c_st, y, &emlrtMCI), "<output of feval>");
  c_st.site = &l_emlrtRSI;
  d_st.site = &m_emlrtRSI;
  lobj_0[1].idx = b_nx;
  lobj_0[1].matlabCodegenIsDeleted = false;
  st.site = &b_emlrtRSI;
  b_st.site = &n_emlrtRSI;
  b_y = NULL;
  m = emlrtCreateCharArray(2, &iv1[0]);
  emlrtInitCharArrayR2013a(&b_st, 7, m, &b_u[0]);
  emlrtAssign(&b_y, m);
  c_y = NULL;
  m = emlrtCreateCharArray(2, &iv2[0]);
  emlrtInitCharArrayR2013a(&b_st, 3, m, &varargin_1[0]);
  emlrtAssign(&c_y, m);
  c_st.site = &rd_emlrtRSI;
  b_feval(&c_st, b_y, c_y, &b_emlrtMCI);
  st.site = &c_emlrtRSI;
  /*  lf=FACTLN(n) */
  /*  Compure the logarithm of n!        */
  /*  */
  /*  Copyright (c) 2012-2026, Imperial College London */
  /*  All rights reserved.   */
  b_st.site = &tc_emlrtRSI;
  b_nx = x->size[0] * x->size[1];
  x->size[0] = 1;
  x->size[1] = N->size[1];
  emxEnsureCapacity_real_T(&b_st, x, b_nx, &o_emlrtRTEI);
  interval_data = x->data;
  b_nx = (N->size[1] / 2) << 1;
  lidx = b_nx - 2;
  for (j = 0; j <= lidx; j += 2) {
    _mm_storeu_pd(&interval_data[j],
                  _mm_add_pd(_mm_loadu_pd(&N_data[j]), _mm_set1_pd(1.0)));
  }
  for (j = b_nx; j < nx; j++) {
    interval_data[j] = N_data[j] + 1.0;
  }
  c_st.site = &uc_emlrtRSI;
  applyScalarFunctionInPlace(&c_st, x);
  st.site = &c_emlrtRSI;
  b_st.site = &o_emlrtRSI;
  c_st.site = &q_emlrtRSI;
  d_st.site = &r_emlrtRSI;
  e_st.site = &s_emlrtRSI;
  indexShapeCheck(&e_st, L->size, f_workspace_nnzClasses->size);
  b_L = L->size[0] * L->size[1];
  for (j = 0; j < loop_ub; j++) {
    b_nx = (int32_T)f_workspace_nnzClasses_data[j];
    if (b_nx > b_L) {
      emlrtDynamicBoundsCheckR2012b(b_nx, 1, b_L, &emlrtBCI, &d_st);
    }
  }
  e_st.site = &s_emlrtRSI;
  f_st.site = &u_emlrtRSI;
  for (j = 0; j < loop_ub; j++) {
    b_nx = (int32_T)f_workspace_nnzClasses_data[j];
    if (b_nx > Z->size[1]) {
      emlrtDynamicBoundsCheckR2012b(b_nx, 1, Z->size[1], &b_emlrtBCI, &d_st);
    }
  }
  b_nx = interval->size[0] * interval->size[1];
  interval->size[0] = 1;
  interval->size[1] = ii->size[1];
  emxEnsureCapacity_real_T(&d_st, interval, b_nx, &p_emlrtRTEI);
  interval_data = interval->data;
  b_nx = ii->size[1];
  idx = (f_workspace_nnzClasses->size[1] < 1600);
  if (idx) {
    for (i = 0; i < loop_ub; i++) {
      interval_data[i] =
          L_data[(int32_T)f_workspace_nnzClasses_data[i] - 1] * 0.0;
    }
  } else {
    emlrtEnterParallelRegion(&d_st, omp_in_parallel());
    emlrtPushJmpBuf(&d_st, &emlrtJBStack);
    pfqn_mmint2_numThreads =
        emlrtAllocRegionTLSs(d_st.tls, omp_in_parallel(), omp_get_max_threads(),
                             omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mmint2_numThreads)

    for (i = 0; i < b_nx; i++) {
      interval_data[i] =
          L_data[(int32_T)f_workspace_nnzClasses_data[i] - 1] * 0.0;
    }
    emlrtPopJmpBuf(&d_st, &emlrtJBStack);
    emlrtExitParallelRegion(&d_st, omp_in_parallel());
  }
  if (f_workspace_nnzClasses->size[1] == interval->size[1]) {
    b_nx = b_Z->size[0] * b_Z->size[1];
    b_Z->size[0] = 1;
    b_Z->size[1] = loop_ub;
    emxEnsureCapacity_real_T(&d_st, b_Z, b_nx, &q_emlrtRTEI);
    b_Z_data = b_Z->data;
    if (idx) {
      for (i1 = 0; i1 < loop_ub; i1++) {
        b_Z_data[i1] = Z_data[(int32_T)f_workspace_nnzClasses_data[i1] - 1] +
                       interval_data[i1];
      }
    } else {
      emlrtEnterParallelRegion(&d_st, omp_in_parallel());
      emlrtPushJmpBuf(&d_st, &emlrtJBStack);
      pfqn_mmint2_numThreads =
          emlrtAllocRegionTLSs(d_st.tls, omp_in_parallel(),
                               omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mmint2_numThreads)

      for (i1 = 0; i1 < loop_ub; i1++) {
        b_Z_data[i1] = Z_data[(int32_T)f_workspace_nnzClasses_data[i1] - 1] +
                       interval_data[i1];
      }
      emlrtPopJmpBuf(&d_st, &emlrtJBStack);
      emlrtExitParallelRegion(&d_st, omp_in_parallel());
    }
    b_nx = b_N->size[0] * b_N->size[1];
    b_N->size[0] = 1;
    b_N->size[1] = loop_ub;
    emxEnsureCapacity_real_T(&d_st, b_N, b_nx, &s_emlrtRTEI);
    interval_data = b_N->data;
    for (j = 0; j < loop_ub; j++) {
      b_nx = (int32_T)f_workspace_nnzClasses_data[j];
      if ((b_nx < 1) || (b_nx > nx)) {
        emlrtDynamicBoundsCheckR2012b(b_nx, 1, nx, &c_emlrtBCI, &d_st);
      }
      interval_data[j] = N_data[b_nx - 1];
    }
    e_st.site = &s_emlrtRSI;
    power(&e_st, b_Z, b_N, interval);
  } else {
    e_st.site = &s_emlrtRSI;
    binary_expand_op(&e_st, interval, s_emlrtRSI, Z, f_workspace_nnzClasses, N);
  }
  e_st.site = &s_emlrtRSI;
  prod(&e_st, interval);
  b_st.site = &p_emlrtRSI;
  b_nx = interval->size[0] * interval->size[1];
  interval->size[0] = 1;
  interval->size[1] = 2;
  emxEnsureCapacity_real_T(&b_st, interval, b_nx, &r_emlrtRTEI);
  interval_data = interval->data;
  interval_data[0] = -1.0;
  interval_data[1] = 1.0;
  b_nx = interval->size[0] * interval->size[1];
  interval->size[0] = 1;
  interval->size[1] = 2;
  emxEnsureCapacity_real_T(&b_st, interval, b_nx, &t_emlrtRTEI);
  interval_data = interval->data;
  c_st.site = &pb_emlrtRSI;
  b_nx = 2;
  pathlen = interval_data[1] - interval_data[0];
  if (pathlen > 0.0) {
    b_nx = (int32_T)(muDoubleScalarCeil(pathlen * (10.0 / pathlen)) - 1.0);
    loop_ub = muIntScalarMax_sint32(0, b_nx) + 2;
    idx = loop_ub - 2;
    lidx = interval->size[0] * interval->size[1];
    interval->size[1] = loop_ub;
    emxEnsureCapacity_real_T(&c_st, interval, lidx, &u_emlrtRTEI);
    interval_data = interval->data;
    for (j = 0; j < idx; j++) {
      interval_data[j + 2] = 0.0;
    }
    if (loop_ub > 2) {
      interval_data[loop_ub - 1] = interval_data[1];
      delta = (interval_data[1] - interval_data[0]) / (real_T)(b_nx + 1);
      for (j = b_nx; j >= 1; j--) {
        interval_data[((loop_ub + j) - b_nx) - 2] =
            interval_data[0] + (real_T)j * delta;
      }
    }
    b_nx = loop_ub;
  } else {
    loop_ub = 2;
  }
  lidx = 0;
  d_st.site = &sb_emlrtRSI;
  for (j = 2; j <= b_nx; j++) {
    delta = interval_data[j - 1];
    if (muDoubleScalarAbs(delta - interval_data[lidx]) > 0.0) {
      lidx++;
      interval_data[lidx] = delta;
    } else {
      loop_ub--;
    }
  }
  if (loop_ub < 2) {
    delta = interval_data[0];
    s = interval_data[0];
    b_nx = interval->size[0] * interval->size[1];
    interval->size[0] = 1;
    interval->size[1] = 2;
    emxEnsureCapacity_real_T(&c_st, interval, b_nx, &w_emlrtRTEI);
    interval_data = interval->data;
    interval_data[0] = delta;
    interval_data[1] = s;
    loop_ub = 2;
  } else {
    b_nx = interval->size[0] * interval->size[1];
    interval->size[1] = loop_ub;
    emxEnsureCapacity_real_T(&c_st, interval, b_nx, &v_emlrtRTEI);
  }
  if (!(pathlen > 0.0)) {
    c_st.site = &qb_emlrtRSI;
    d_st.site = &tb_emlrtRSI;
    e_st.site = &r_emlrtRSI;
    f_st.site = &s_emlrtRSI;
    indexShapeCheck(&f_st, L->size, f_workspace_nnzClasses->size);
    lidx = f_workspace_nnzClasses->size[1];
    for (j = 0; j < lidx; j++) {
      b_nx = (int32_T)f_workspace_nnzClasses_data[j];
      if ((b_nx < 1) || (b_nx > b_L)) {
        emlrtDynamicBoundsCheckR2012b(b_nx, 1, b_L, &emlrtBCI, &e_st);
      }
    }
    f_st.site = &s_emlrtRSI;
    for (j = 0; j < lidx; j++) {
      b_nx = (int32_T)f_workspace_nnzClasses_data[j];
      if ((b_nx < 1) || (b_nx > Z->size[1])) {
        emlrtDynamicBoundsCheckR2012b(b_nx, 1, Z->size[1], &b_emlrtBCI, &e_st);
      }
    }
    b_nx = interval->size[0] * interval->size[1];
    interval->size[0] = 1;
    interval->size[1] = f_workspace_nnzClasses->size[1];
    emxEnsureCapacity_real_T(&e_st, interval, b_nx, &p_emlrtRTEI);
    interval_data = interval->data;
    b_nx = f_workspace_nnzClasses->size[1];
    idx = (f_workspace_nnzClasses->size[1] < 1600);
    if (idx) {
      for (i2 = 0; i2 < lidx; i2++) {
        interval_data[i2] =
            L_data[(int32_T)f_workspace_nnzClasses_data[i2] - 1] *
            13.815521618946679;
      }
    } else {
      emlrtEnterParallelRegion(&e_st, omp_in_parallel());
      emlrtPushJmpBuf(&e_st, &emlrtJBStack);
      pfqn_mmint2_numThreads =
          emlrtAllocRegionTLSs(e_st.tls, omp_in_parallel(),
                               omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mmint2_numThreads)

      for (i2 = 0; i2 < b_nx; i2++) {
        interval_data[i2] =
            L_data[(int32_T)f_workspace_nnzClasses_data[i2] - 1] *
            13.815521618946679;
      }
      emlrtPopJmpBuf(&e_st, &emlrtJBStack);
      emlrtExitParallelRegion(&e_st, omp_in_parallel());
    }
    if (f_workspace_nnzClasses->size[1] == interval->size[1]) {
      b_nx = b_Z->size[0] * b_Z->size[1];
      b_Z->size[0] = 1;
      b_Z->size[1] = lidx;
      emxEnsureCapacity_real_T(&e_st, b_Z, b_nx, &q_emlrtRTEI);
      b_Z_data = b_Z->data;
      if (idx) {
        for (i3 = 0; i3 < lidx; i3++) {
          b_Z_data[i3] = Z_data[(int32_T)f_workspace_nnzClasses_data[i3] - 1] +
                         interval_data[i3];
        }
      } else {
        emlrtEnterParallelRegion(&e_st, omp_in_parallel());
        emlrtPushJmpBuf(&e_st, &emlrtJBStack);
        pfqn_mmint2_numThreads =
            emlrtAllocRegionTLSs(e_st.tls, omp_in_parallel(),
                                 omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mmint2_numThreads)

        for (i3 = 0; i3 < lidx; i3++) {
          b_Z_data[i3] = Z_data[(int32_T)f_workspace_nnzClasses_data[i3] - 1] +
                         interval_data[i3];
        }
        emlrtPopJmpBuf(&e_st, &emlrtJBStack);
        emlrtExitParallelRegion(&e_st, omp_in_parallel());
      }
      b_nx = b_N->size[0] * b_N->size[1];
      b_N->size[0] = 1;
      b_N->size[1] = lidx;
      emxEnsureCapacity_real_T(&e_st, b_N, b_nx, &s_emlrtRTEI);
      interval_data = b_N->data;
      for (j = 0; j < lidx; j++) {
        b_nx = (int32_T)f_workspace_nnzClasses_data[j];
        if ((b_nx < 1) || (b_nx > nx)) {
          emlrtDynamicBoundsCheckR2012b(b_nx, 1, nx, &c_emlrtBCI, &e_st);
        }
        interval_data[j] = N_data[b_nx - 1];
      }
      f_st.site = &s_emlrtRSI;
      power(&f_st, b_Z, b_N, interval);
    } else {
      f_st.site = &s_emlrtRSI;
      binary_expand_op(&f_st, interval, s_emlrtRSI, Z, f_workspace_nnzClasses,
                       N);
    }
    f_st.site = &s_emlrtRSI;
    delta = 9.9998893907876746E-7 * prod(&f_st, interval);
    if ((muDoubleScalarIsInf(delta) || muDoubleScalarIsNaN(delta)) &&
        (!emlrtSetWarningFlag(&c_st))) {
      d_st.site = &ub_emlrtRSI;
      warning(&d_st);
    }
    delta *= 27.631043237893358;
  } else {
    c_st.site = &rb_emlrtRSI;
    delta = scalarValuedIntegral(&c_st, Z, f_workspace_nnzClasses, L, N,
                                 interval, loop_ub, pathlen, &delta, &success);
  }
  st.site = &c_emlrtRSI;
  if (delta < 0.0) {
    emlrtErrorWithMessageIdR2018a(
        &st, &b_emlrtRTEI, "Coder:toolbox:ElFunDomainError",
        "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
  }
  delta = muDoubleScalarLog(delta);
  st.site = &c_emlrtRSI;
  b_st.site = &cd_emlrtRSI;
  c_st.site = &mb_emlrtRSI;
  d_st.site = &dd_emlrtRSI;
  if (x->size[1] == 0) {
    s = 0.0;
  } else {
    e_st.site = &ed_emlrtRSI;
    f_st.site = &fd_emlrtRSI;
    if (x->size[1] < 4096) {
      b_x = *x;
      c_x = x->size[1];
      b_x.size = &c_x;
      b_x.numDimensions = 1;
      g_st.site = &gd_emlrtRSI;
      s = sumColumnB(&g_st, &b_x, x->size[1]);
    } else {
      b_nx = (int32_T)((uint32_T)x->size[1] >> 12);
      idx = b_nx << 12;
      lidx = x->size[1] - idx;
      b_x = *x;
      d_x = x->size[1];
      b_x.size = &d_x;
      b_x.numDimensions = 1;
      s = sumColumnB4(&b_x, 1);
      for (j = 2; j <= b_nx; j++) {
        b_x = *x;
        e_x = x->size[1];
        b_x.size = &e_x;
        b_x.numDimensions = 1;
        s += sumColumnB4(&b_x, ((j - 1) << 12) + 1);
      }
      if (lidx > 0) {
        b_x = *x;
        f_x = x->size[1];
        b_x.size = &f_x;
        b_x.numDimensions = 1;
        g_st.site = &id_emlrtRSI;
        s += b_sumColumnB(&g_st, &b_x, lidx, idx + 1);
      }
    }
  }
  *lG = delta - s;
  *G = muDoubleScalarExp(*lG);
  st.site = &d_emlrtRSI;
  if (lobj_0[1].idx > 0) {
    d_y = NULL;
    m = emlrtCreateCharArray(2, &iv3[0]);
    emlrtInitCharArrayR2013a(&st, 51, m, &c_u[0]);
    emlrtAssign(&d_y, m);
    b_nx = lobj_0[1].idx;
    e_y = NULL;
    m = emlrtCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    *(int32_T *)emlrtMxGetData(m) = b_nx;
    emlrtAssign(&e_y, m);
    b_st.site = &sd_emlrtRSI;
    b_feval(&b_st, d_y, e_y, &f_emlrtMCI);
  }
  st.site = &e_emlrtRSI;
  handle_matlabCodegenDestructor(&st, &lobj_0[0]);
  st.site = &e_emlrtRSI;
  handle_matlabCodegenDestructor(&st, &lobj_0[1]);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  emxFree_real_T(sp, &b_N);
  emxFree_real_T(sp, &b_Z);
  emxFree_real_T(sp, &interval);
  emxFree_real_T(sp, &x);
  emxFree_int32_T(sp, &ii);
  emxFree_real_T(sp, &f_workspace_nnzClasses);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

void pfqn_mmint2_anonFcn1(const emlrtStack *sp, const emxArray_real_T *Z,
                          const emxArray_real_T *nnzClasses,
                          const emxArray_real_T *L, const emxArray_real_T *N,
                          const emxArray_real_T *u,
                          emxArray_real_T *varargout_1)
{
  __m128d r3;
  __m128d r5;
  jmp_buf *volatile emlrtJBStack;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack g_st;
  emlrtStack h_st;
  emlrtStack st;
  emxArray_boolean_T b_c;
  emxArray_boolean_T *c;
  emxArray_real_T *a;
  emxArray_real_T *b;
  emxArray_real_T *r;
  emxArray_real_T *r1;
  emxArray_real_T *r4;
  emxArray_real_T *y;
  const real_T *L_data;
  const real_T *N_data;
  const real_T *Z_data;
  const real_T *nnzClasses_data;
  const real_T *u_data;
  real_T b_varargin_1;
  real_T b_varargin_2;
  real_T c_varargin_1;
  real_T c_varargin_2;
  real_T d_varargin_2;
  real_T varargin_1;
  real_T *a_data;
  real_T *b_data;
  real_T *r2;
  real_T *y_data;
  int32_T b_k;
  int32_T c_c;
  int32_T i;
  int32_T i1;
  int32_T ibtile;
  int32_T jtilecol;
  int32_T k;
  int32_T loop_ub;
  int32_T nx;
  int32_T pfqn_mmint2_anonFcn1_numThreads;
  int32_T vectorUB;
  int32_T vlen;
  boolean_T b_varargout_1;
  boolean_T p;
  boolean_T *c_data;
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
  u_data = u->data;
  N_data = N->data;
  L_data = L->data;
  nnzClasses_data = nnzClasses->data;
  Z_data = Z->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  st.site = &s_emlrtRSI;
  indexShapeCheck(&st, L->size, nnzClasses->size);
  ibtile = L->size[0] * L->size[1];
  vlen = nnzClasses->size[1];
  for (jtilecol = 0; jtilecol < vlen; jtilecol++) {
    vectorUB = (int32_T)nnzClasses_data[jtilecol];
    if ((vectorUB < 1) || (vectorUB > ibtile)) {
      emlrtDynamicBoundsCheckR2012b(vectorUB, 1, ibtile, &emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
  }
  st.site = &s_emlrtRSI;
  b_st.site = &u_emlrtRSI;
  emxInit_real_T(&st, &y, 2, &lb_emlrtRTEI);
  loop_ub = u->size[1];
  ibtile = y->size[0] * y->size[1];
  y->size[0] = u->size[1];
  y->size[1] = nnzClasses->size[1];
  emxEnsureCapacity_real_T(&st, y, ibtile, &fb_emlrtRTEI);
  y_data = y->data;
  b_st.site = &mc_emlrtRSI;
  if (nnzClasses->size[1] > 2147483646) {
    c_st.site = &j_emlrtRSI;
    check_forloop_overflow_error(&c_st);
  }
  for (jtilecol = 0; jtilecol < vlen; jtilecol++) {
    ibtile = jtilecol * loop_ub;
    b_st.site = &nc_emlrtRSI;
    if (loop_ub > 2147483646) {
      c_st.site = &j_emlrtRSI;
      check_forloop_overflow_error(&c_st);
    }
    for (k = 0; k < loop_ub; k++) {
      y_data[ibtile + k] = u_data[k];
    }
  }
  for (jtilecol = 0; jtilecol < vlen; jtilecol++) {
    ibtile = (int32_T)nnzClasses_data[jtilecol];
    if ((ibtile < 1) || (ibtile > Z->size[1])) {
      emlrtDynamicBoundsCheckR2012b(ibtile, 1, Z->size[1], &b_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
  }
  emxInit_real_T(sp, &r, 2, &q_emlrtRTEI);
  if (nnzClasses->size[1] == y->size[1]) {
    ibtile = r->size[0] * r->size[1];
    r->size[0] = u->size[1];
    r->size[1] = nnzClasses->size[1];
    emxEnsureCapacity_real_T(sp, r, ibtile, &p_emlrtRTEI);
    b_data = r->data;
    for (jtilecol = 0; jtilecol < vlen; jtilecol++) {
      ibtile = (y->size[0] / 2) << 1;
      vectorUB = ibtile - 2;
      for (k = 0; k <= vectorUB; k += 2) {
        r3 = _mm_loadu_pd(&y_data[k + y->size[0] * jtilecol]);
        _mm_storeu_pd(
            &b_data[k + r->size[0] * jtilecol],
            _mm_mul_pd(
                _mm_set1_pd(L_data[(int32_T)nnzClasses_data[jtilecol] - 1]),
                r3));
      }
      for (k = ibtile; k < loop_ub; k++) {
        b_data[k + r->size[0] * jtilecol] =
            L_data[(int32_T)nnzClasses_data[jtilecol] - 1] *
            y_data[k + y->size[0] * jtilecol];
      }
    }
  } else {
    st.site = &s_emlrtRSI;
    binary_expand_op_6(&st, r, L, nnzClasses, y);
    b_data = r->data;
  }
  if ((nnzClasses->size[1] != r->size[1]) &&
      ((nnzClasses->size[1] != 1) && (r->size[1] != 1))) {
    emlrtDimSizeImpxCheckR2021b(nnzClasses->size[1], r->size[1], &emlrtECI,
                                (emlrtConstCTX)sp);
  }
  st.site = &s_emlrtRSI;
  emxInit_real_T(&st, &r1, 1, &kb_emlrtRTEI);
  ibtile = r1->size[0];
  r1->size[0] = u->size[1];
  emxEnsureCapacity_real_T(&st, r1, ibtile, &gb_emlrtRTEI);
  r2 = r1->data;
  vectorUB = (u->size[1] / 2) << 1;
  ibtile = vectorUB - 2;
  for (jtilecol = 0; jtilecol <= ibtile; jtilecol += 2) {
    _mm_storeu_pd(&r2[jtilecol], _mm_mul_pd(_mm_loadu_pd(&u_data[jtilecol]),
                                            _mm_set1_pd(-1.0)));
  }
  for (jtilecol = vectorUB; jtilecol < loop_ub; jtilecol++) {
    r2[jtilecol] = -u_data[jtilecol];
  }
  b_st.site = &kc_emlrtRSI;
  nx = r1->size[0];
  c_st.site = &lc_emlrtRSI;
  if (r1->size[0] > 2147483646) {
    d_st.site = &j_emlrtRSI;
    check_forloop_overflow_error(&d_st);
  }
  if (r1->size[0] < 1600) {
    for (b_k = 0; b_k < nx; b_k++) {
      r2[b_k] = muDoubleScalarExp(r2[b_k]);
    }
  } else {
    emlrtEnterParallelRegion(&b_st, omp_in_parallel());
    emlrtPushJmpBuf(&b_st, &emlrtJBStack);
    pfqn_mmint2_anonFcn1_numThreads =
        emlrtAllocRegionTLSs(b_st.tls, omp_in_parallel(), omp_get_max_threads(),
                             omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mmint2_anonFcn1_numThreads)

    for (b_k = 0; b_k < nx; b_k++) {
      r2[b_k] = muDoubleScalarExp(r2[b_k]);
    }
    emlrtPopJmpBuf(&b_st, &emlrtJBStack);
    emlrtExitParallelRegion(&b_st, omp_in_parallel());
  }
  st.site = &s_emlrtRSI;
  emxInit_real_T(&st, &a, 2, &q_emlrtRTEI);
  if (nnzClasses->size[1] == r->size[1]) {
    loop_ub = r->size[0];
    ibtile = a->size[0] * a->size[1];
    a->size[0] = r->size[0];
    a->size[1] = vlen;
    emxEnsureCapacity_real_T(&st, a, ibtile, &q_emlrtRTEI);
    a_data = a->data;
    for (jtilecol = 0; jtilecol < vlen; jtilecol++) {
      ibtile = (r->size[0] / 2) << 1;
      vectorUB = ibtile - 2;
      for (k = 0; k <= vectorUB; k += 2) {
        r3 = _mm_loadu_pd(&b_data[k + r->size[0] * jtilecol]);
        _mm_storeu_pd(
            &a_data[k + a->size[0] * jtilecol],
            _mm_add_pd(
                _mm_set1_pd(Z_data[(int32_T)nnzClasses_data[jtilecol] - 1]),
                r3));
      }
      for (k = ibtile; k < loop_ub; k++) {
        a_data[k + a->size[0] * jtilecol] =
            Z_data[(int32_T)nnzClasses_data[jtilecol] - 1] +
            b_data[k + r->size[0] * jtilecol];
      }
    }
  } else {
    b_st.site = &s_emlrtRSI;
    binary_expand_op_5(&b_st, a, Z, nnzClasses, r);
    a_data = a->data;
  }
  emxFree_real_T(&st, &r);
  emxInit_real_T(&st, &b, 2, &s_emlrtRTEI);
  ibtile = b->size[0] * b->size[1];
  b->size[0] = 1;
  b->size[1] = vlen;
  emxEnsureCapacity_real_T(&st, b, ibtile, &s_emlrtRTEI);
  b_data = b->data;
  for (jtilecol = 0; jtilecol < vlen; jtilecol++) {
    ibtile = (int32_T)nnzClasses_data[jtilecol];
    if ((ibtile < 1) || (ibtile > N->size[1])) {
      emlrtDynamicBoundsCheckR2012b(ibtile, 1, N->size[1], &c_emlrtBCI, &st);
    }
    b_data[jtilecol] = N_data[ibtile - 1];
  }
  b_st.site = &v_emlrtRSI;
  c_st.site = &w_emlrtRSI;
  if ((a->size[0] == 1) && (a->size[1] == 1)) {
    if (a_data[0] < 0.0) {
      d_st.site = &y_emlrtRSI;
      p = vAllOrAny(&d_st, b);
    } else {
      p = false;
    }
  } else if (nnzClasses->size[1] == 1) {
    varargin_1 = N_data[(int32_T)nnzClasses_data[0] - 1];
    if ((!muDoubleScalarIsNaN(varargin_1)) &&
        (muDoubleScalarFloor(varargin_1) != varargin_1)) {
      d_st.site = &ab_emlrtRSI;
      e_st.site = &cb_emlrtRSI;
      ibtile = a->size[0] * a->size[1];
      p = false;
      f_st.site = &db_emlrtRSI;
      if (ibtile > 2147483646) {
        g_st.site = &j_emlrtRSI;
        check_forloop_overflow_error(&g_st);
      }
      for (jtilecol = 0; jtilecol < ibtile; jtilecol++) {
        if (p || (a_data[jtilecol] < 0.0)) {
          p = true;
        }
      }
    } else {
      p = false;
    }
  } else {
    uint32_T sz[2];
    uint32_T varargin_2[2];
    boolean_T exitg1;
    sz[0] = (uint32_T)a->size[0];
    sz[1] = (uint32_T)a->size[1];
    varargin_2[0] = 1U;
    varargin_2[1] = (uint32_T)nnzClasses->size[1];
    p = true;
    ibtile = 0;
    exitg1 = false;
    while ((!exitg1) && (ibtile < 2)) {
      if ((int32_T)sz[ibtile] != (int32_T)varargin_2[ibtile]) {
        p = false;
        exitg1 = true;
      } else {
        ibtile++;
      }
    }
    if (p) {
      p = false;
      ibtile = a->size[0] * a->size[1];
      for (jtilecol = 0; jtilecol < ibtile; jtilecol++) {
        if (p) {
          p = true;
        } else if (a_data[jtilecol] < 0.0) {
          varargin_1 = b_data[jtilecol];
          if ((!muDoubleScalarIsNaN(varargin_1)) &&
              (muDoubleScalarFloor(varargin_1) != varargin_1)) {
            p = true;
          }
        }
      }
    } else {
      emxInit_boolean_T(&c_st, &c, &db_emlrtRTEI);
      d_st.site = &bb_emlrtRSI;
      e_st.site = &eb_emlrtRSI;
      if ((a->size[0] == 1) && (a->size[1] == nnzClasses->size[1])) {
        ibtile = c->size[0] * c->size[1];
        c->size[0] = 1;
        vectorUB = a->size[1];
        c->size[1] = a->size[1];
        emxEnsureCapacity_boolean_T(&e_st, c, ibtile, &cb_emlrtRTEI);
        c_data = c->data;
        ibtile = a->size[1];
        if (a->size[1] < 1600) {
          for (i1 = 0; i1 < vectorUB; i1++) {
            varargin_1 = a_data[i1];
            b_varargin_2 = b_data[i1];
            if ((varargin_1 < 0.0) && (!muDoubleScalarIsNaN(b_varargin_2)) &&
                (muDoubleScalarFloor(b_varargin_2) != b_varargin_2)) {
              p = true;
            } else {
              p = false;
            }
            c_data[i1] = p;
          }
        } else {
          emlrtEnterParallelRegion(&e_st, omp_in_parallel());
          emlrtPushJmpBuf(&e_st, &emlrtJBStack);
          pfqn_mmint2_anonFcn1_numThreads =
              emlrtAllocRegionTLSs(e_st.tls, omp_in_parallel(),
                                   omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mmint2_anonFcn1_numThreads) private( \
        c_varargin_1, d_varargin_2, b_varargout_1)

          for (i1 = 0; i1 < ibtile; i1++) {
            c_varargin_1 = a_data[i1];
            d_varargin_2 = b_data[i1];
            if ((c_varargin_1 < 0.0) && (!muDoubleScalarIsNaN(d_varargin_2)) &&
                (muDoubleScalarFloor(d_varargin_2) != d_varargin_2)) {
              b_varargout_1 = true;
            } else {
              b_varargout_1 = false;
            }
            c_data[i1] = b_varargout_1;
          }
          emlrtPopJmpBuf(&e_st, &emlrtJBStack);
          emlrtExitParallelRegion(&e_st, omp_in_parallel());
        }
      } else {
        f_st.site = &fb_emlrtRSI;
        b_expand_fltpower_domain_error(&f_st, a, b, c);
      }
      d_st.site = &bb_emlrtRSI;
      e_st.site = &gb_emlrtRSI;
      f_st.site = &hb_emlrtRSI;
      g_st.site = &ib_emlrtRSI;
      b_c = *c;
      c_c = c->size[0] * c->size[1];
      b_c.size = &c_c;
      b_c.numDimensions = 1;
      h_st.site = &r_emlrtRSI;
      p = allOrAny_anonFcn1(&h_st, &b_c);
      emxFree_boolean_T(&g_st, &c);
    }
  }
  if (p) {
    emlrtErrorWithMessageIdR2018a(&b_st, &e_emlrtRTEI,
                                  "Coder:toolbox:power_domainError",
                                  "Coder:toolbox:power_domainError", 0);
  }
  c_st.site = &x_emlrtRSI;
  d_st.site = &eb_emlrtRSI;
  if ((a->size[0] == 1) && (a->size[1] == nnzClasses->size[1])) {
    ibtile = y->size[0] * y->size[1];
    y->size[0] = 1;
    vectorUB = a->size[1];
    y->size[1] = a->size[1];
    emxEnsureCapacity_real_T(&d_st, y, ibtile, &bb_emlrtRTEI);
    y_data = y->data;
    ibtile = a->size[1];
    if (a->size[1] < 1600) {
      for (i = 0; i < vectorUB; i++) {
        varargin_1 = a_data[i];
        b_varargin_2 = b_data[i];
        y_data[i] = muDoubleScalarPower(varargin_1, b_varargin_2);
      }
    } else {
      emlrtEnterParallelRegion(&d_st, omp_in_parallel());
      emlrtPushJmpBuf(&d_st, &emlrtJBStack);
      pfqn_mmint2_anonFcn1_numThreads =
          emlrtAllocRegionTLSs(d_st.tls, omp_in_parallel(),
                               omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mmint2_anonFcn1_numThreads) private( \
        b_varargin_1, c_varargin_2)

      for (i = 0; i < ibtile; i++) {
        b_varargin_1 = a_data[i];
        c_varargin_2 = b_data[i];
        y_data[i] = muDoubleScalarPower(b_varargin_1, c_varargin_2);
      }
      emlrtPopJmpBuf(&d_st, &emlrtJBStack);
      emlrtExitParallelRegion(&d_st, omp_in_parallel());
    }
  } else {
    e_st.site = &fb_emlrtRSI;
    b_expand_power(&e_st, a, b, y);
    y_data = y->data;
  }
  emxFree_real_T(&d_st, &b);
  emxFree_real_T(&d_st, &a);
  st.site = &s_emlrtRSI;
  b_st.site = &lb_emlrtRSI;
  c_st.site = &mb_emlrtRSI;
  vlen = y->size[1];
  emxInit_real_T(&c_st, &r4, 1, &kb_emlrtRTEI);
  if ((y->size[0] == 0) || (y->size[1] == 0)) {
    ibtile = r4->size[0];
    r4->size[0] = y->size[0];
    emxEnsureCapacity_real_T(&c_st, r4, ibtile, &hb_emlrtRTEI);
    a_data = r4->data;
    ibtile = y->size[0];
    for (jtilecol = 0; jtilecol < ibtile; jtilecol++) {
      a_data[jtilecol] = 1.0;
    }
  } else {
    int32_T vstride;
    d_st.site = &nb_emlrtRSI;
    vstride = y->size[0];
    ibtile = r4->size[0];
    r4->size[0] = y->size[0];
    emxEnsureCapacity_real_T(&d_st, r4, ibtile, &ib_emlrtRTEI);
    a_data = r4->data;
    e_st.site = &oc_emlrtRSI;
    if (y->size[0] > 2147483646) {
      f_st.site = &j_emlrtRSI;
      check_forloop_overflow_error(&f_st);
    }
    for (jtilecol = 0; jtilecol < vstride; jtilecol++) {
      a_data[jtilecol] = y_data[jtilecol];
    }
    e_st.site = &ob_emlrtRSI;
    if (y->size[1] > 2147483646) {
      f_st.site = &j_emlrtRSI;
      check_forloop_overflow_error(&f_st);
    }
    for (jtilecol = 2; jtilecol <= vlen; jtilecol++) {
      ibtile = (jtilecol - 1) * vstride;
      e_st.site = &pc_emlrtRSI;
      if (vstride > 2147483646) {
        f_st.site = &j_emlrtRSI;
        check_forloop_overflow_error(&f_st);
      }
      vectorUB = (vstride / 2) << 1;
      loop_ub = vectorUB - 2;
      for (k = 0; k <= loop_ub; k += 2) {
        r3 = _mm_loadu_pd(&a_data[k]);
        r5 = _mm_loadu_pd(&y_data[ibtile + k]);
        _mm_storeu_pd(&a_data[k], _mm_mul_pd(r3, r5));
      }
      for (k = vectorUB; k < vstride; k++) {
        a_data[k] *= y_data[ibtile + k];
      }
    }
  }
  emxFree_real_T(&c_st, &y);
  if ((nx != r4->size[0]) && ((nx != 1) && (r4->size[0] != 1))) {
    emlrtDimSizeImpxCheckR2021b(nx, r4->size[0], &b_emlrtECI,
                                (emlrtConstCTX)sp);
  }
  if (r1->size[0] == r4->size[0]) {
    ibtile = varargout_1->size[0] * varargout_1->size[1];
    varargout_1->size[0] = 1;
    varargout_1->size[1] = nx;
    emxEnsureCapacity_real_T(sp, varargout_1, ibtile, &jb_emlrtRTEI);
    b_data = varargout_1->data;
    ibtile = (r1->size[0] / 2) << 1;
    vectorUB = ibtile - 2;
    for (jtilecol = 0; jtilecol <= vectorUB; jtilecol += 2) {
      r3 = _mm_loadu_pd(&r2[jtilecol]);
      r5 = _mm_loadu_pd(&a_data[jtilecol]);
      _mm_storeu_pd(&b_data[jtilecol], _mm_mul_pd(r3, r5));
    }
    for (jtilecol = ibtile; jtilecol < nx; jtilecol++) {
      b_data[jtilecol] = r2[jtilecol] * a_data[jtilecol];
    }
  } else {
    st.site = &s_emlrtRSI;
    binary_expand_op_4(&st, varargout_1, r1, r4);
  }
  emxFree_real_T(sp, &r4);
  emxFree_real_T(sp, &r1);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (pfqn_mmint2.c) */
