/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * ctmc_stochcomp.c
 *
 * Code generation for function 'ctmc_stochcomp'
 *
 */

/* Include files */
#include "ctmc_stochcomp.h"
#include "ctmc_stochcomp_data.h"
#include "ctmc_stochcomp_emxutil.h"
#include "ctmc_stochcomp_types.h"
#include "eml_int_forloop_overflow_check.h"
#include "qrsolve.h"
#include "rt_nonfinite.h"
#include "warning.h"
#include "blas.h"
#include "lapacke.h"
#include "mwmathutil.h"
#include <emmintrin.h>
#include <stddef.h>

/* Variable Definitions */
static emlrtRSInfo emlrtRSI =
    {
        25,               /* lineNo */
        "ctmc_stochcomp", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pathName */
};

static emlrtRSInfo b_emlrtRSI =
    {
        26,               /* lineNo */
        "ctmc_stochcomp", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pathName */
};

static emlrtRSInfo c_emlrtRSI = {
    20,         /* lineNo */
    "mldivide", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/mldivide.m" /* pathName
                                                                      */
};

static emlrtRSInfo d_emlrtRSI = {
    42,      /* lineNo */
    "mldiv", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/mldivide.m" /* pathName
                                                                      */
};

static emlrtRSInfo e_emlrtRSI = {
    44,      /* lineNo */
    "mldiv", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/mldivide.m" /* pathName
                                                                      */
};

static emlrtRSInfo f_emlrtRSI = {
    67,        /* lineNo */
    "lusolve", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/lusolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo g_emlrtRSI = {
    109,          /* lineNo */
    "lusolveNxN", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/lusolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo h_emlrtRSI = {
    112,          /* lineNo */
    "lusolveNxN", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/lusolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo i_emlrtRSI = {
    124,          /* lineNo */
    "InvAtimesX", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/lusolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo j_emlrtRSI = {
    19,        /* lineNo */
    "xgetrfs", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xgetrfs.m" /* pathName */
};

static emlrtRSInfo k_emlrtRSI = {
    108,      /* lineNo */
    "cmldiv", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xgetrfs.m" /* pathName */
};

static emlrtRSInfo l_emlrtRSI = {
    70,       /* lineNo */
    "cmldiv", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xgetrfs.m" /* pathName */
};

static emlrtRSInfo o_emlrtRSI = {
    90,              /* lineNo */
    "warn_singular", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/lusolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo p_emlrtRSI = {
    61,        /* lineNo */
    "qrsolve", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo q_emlrtRSI = {
    72,        /* lineNo */
    "qrsolve", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo r_emlrtRSI = {
    85,        /* lineNo */
    "qrsolve", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo s_emlrtRSI =
    {
        63,       /* lineNo */
        "xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo t_emlrtRSI =
    {
        138,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo u_emlrtRSI =
    {
        141,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo v_emlrtRSI =
    {
        143,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo w_emlrtRSI =
    {
        148,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo x_emlrtRSI =
    {
        151,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo y_emlrtRSI =
    {
        154,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo ab_emlrtRSI =
    {
        158,            /* lineNo */
        "ceval_xgeqp3", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pathName */
};

static emlrtRSInfo bb_emlrtRSI = {
    173,          /* lineNo */
    "rankFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo cb_emlrtRSI = {
    172,          /* lineNo */
    "rankFromQR", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/qrsolve.m" /* pathName
                                                                           */
};

static emlrtRSInfo pb_emlrtRSI =
    {
        94,                  /* lineNo */
        "eml_mtimes_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pathName */
};

static emlrtRSInfo qb_emlrtRSI =
    {
        69,                  /* lineNo */
        "eml_mtimes_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pathName */
};

static emlrtRSInfo rb_emlrtRSI =
    {
        142,      /* lineNo */
        "mtimes", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+blas/"
        "mtimes.m" /* pathName */
};

static emlrtRSInfo sb_emlrtRSI =
    {
        177,           /* lineNo */
        "mtimes_blas", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+blas/"
        "mtimes.m" /* pathName */
};

static emlrtMCInfo c_emlrtMCI = {
    53,        /* lineNo */
    19,        /* colNo */
    "flt2str", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/shared/coder/coder/lib/+coder/+internal/"
    "flt2str.m" /* pName */
};

static emlrtRTEInfo c_emlrtRTEI = {
    16,         /* lineNo */
    19,         /* colNo */
    "mldivide", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/mldivide.m" /* pName */
};

static emlrtRTEInfo d_emlrtRTEI =
    {
        133,                   /* lineNo */
        23,                    /* colNo */
        "dynamic_size_checks", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

static emlrtRTEInfo e_emlrtRTEI =
    {
        138,                   /* lineNo */
        23,                    /* colNo */
        "dynamic_size_checks", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

static emlrtECInfo emlrtECI =
    {
        2,                /* nDims */
        27,               /* lineNo */
        5,                /* colNo */
        "ctmc_stochcomp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pName */
};

static emlrtECInfo b_emlrtECI =
    {
        1,                /* nDims */
        27,               /* lineNo */
        5,                /* colNo */
        "ctmc_stochcomp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pName */
};

static emlrtDCInfo emlrtDCI = {
    12,               /* lineNo */
    15,               /* colNo */
    "ctmc_stochcomp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
    "ctmc_stochcomp.m", /* pName */
    4                   /* checkKind */
};

static emlrtBCInfo emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    10,               /* lineNo */
    16,               /* colNo */
    "isSelected",     /* aName */
    "ctmc_stochcomp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
    "ctmc_stochcomp.m", /* pName */
    0                   /* checkKind */
};

static emlrtDCInfo b_emlrtDCI = {
    10,               /* lineNo */
    16,               /* colNo */
    "ctmc_stochcomp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
    "ctmc_stochcomp.m", /* pName */
    1                   /* checkKind */
};

static emlrtBCInfo b_emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    10,               /* lineNo */
    18,               /* colNo */
    "I",              /* aName */
    "ctmc_stochcomp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
    "ctmc_stochcomp.m", /* pName */
    0                   /* checkKind */
};

static emlrtBCInfo c_emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    15,               /* lineNo */
    20,               /* colNo */
    "isSelected",     /* aName */
    "ctmc_stochcomp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
    "ctmc_stochcomp.m", /* pName */
    0                   /* checkKind */
};

static emlrtBCInfo d_emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    20,               /* lineNo */
    9,                /* colNo */
    "Q",              /* aName */
    "ctmc_stochcomp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
    "ctmc_stochcomp.m", /* pName */
    0                   /* checkKind */
};

static emlrtDCInfo c_emlrtDCI = {
    20,               /* lineNo */
    9,                /* colNo */
    "ctmc_stochcomp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
    "ctmc_stochcomp.m", /* pName */
    1                   /* checkKind */
};

static emlrtBCInfo e_emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    20,               /* lineNo */
    11,               /* colNo */
    "Q",              /* aName */
    "ctmc_stochcomp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
    "ctmc_stochcomp.m", /* pName */
    0                   /* checkKind */
};

static emlrtBCInfo f_emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    17,               /* lineNo */
    12,               /* colNo */
    "Ic",             /* aName */
    "ctmc_stochcomp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
    "ctmc_stochcomp.m", /* pName */
    0                   /* checkKind */
};

static emlrtBCInfo g_emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    21,               /* lineNo */
    9,                /* colNo */
    "Q",              /* aName */
    "ctmc_stochcomp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
    "ctmc_stochcomp.m", /* pName */
    0                   /* checkKind */
};

static emlrtBCInfo h_emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    21,               /* lineNo */
    11,               /* colNo */
    "Q",              /* aName */
    "ctmc_stochcomp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
    "ctmc_stochcomp.m", /* pName */
    0                   /* checkKind */
};

static emlrtBCInfo i_emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    22,               /* lineNo */
    9,                /* colNo */
    "Q",              /* aName */
    "ctmc_stochcomp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
    "ctmc_stochcomp.m", /* pName */
    0                   /* checkKind */
};

static emlrtBCInfo j_emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    22,               /* lineNo */
    12,               /* colNo */
    "Q",              /* aName */
    "ctmc_stochcomp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
    "ctmc_stochcomp.m", /* pName */
    0                   /* checkKind */
};

static emlrtBCInfo k_emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    23,               /* lineNo */
    9,                /* colNo */
    "Q",              /* aName */
    "ctmc_stochcomp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
    "ctmc_stochcomp.m", /* pName */
    0                   /* checkKind */
};

static emlrtBCInfo l_emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    23,               /* lineNo */
    12,               /* colNo */
    "Q",              /* aName */
    "ctmc_stochcomp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
    "ctmc_stochcomp.m", /* pName */
    0                   /* checkKind */
};

static emlrtRTEInfo g_emlrtRTEI =
    {
        8,                /* lineNo */
        1,                /* colNo */
        "ctmc_stochcomp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pName */
};

static emlrtRTEInfo h_emlrtRTEI =
    {
        12,               /* lineNo */
        1,                /* colNo */
        "ctmc_stochcomp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pName */
};

static emlrtRTEInfo i_emlrtRTEI =
    {
        20,               /* lineNo */
        1,                /* colNo */
        "ctmc_stochcomp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pName */
};

static emlrtRTEInfo j_emlrtRTEI =
    {
        21,               /* lineNo */
        1,                /* colNo */
        "ctmc_stochcomp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pName */
};

static emlrtRTEInfo k_emlrtRTEI =
    {
        22,               /* lineNo */
        1,                /* colNo */
        "ctmc_stochcomp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pName */
};

static emlrtRTEInfo l_emlrtRTEI =
    {
        23,               /* lineNo */
        1,                /* colNo */
        "ctmc_stochcomp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pName */
};

static emlrtRTEInfo m_emlrtRTEI =
    {
        25,               /* lineNo */
        6,                /* colNo */
        "ctmc_stochcomp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pName */
};

static emlrtRTEInfo n_emlrtRTEI =
    {
        25,               /* lineNo */
        1,                /* colNo */
        "ctmc_stochcomp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pName */
};

static emlrtRTEInfo o_emlrtRTEI =
    {
        61,       /* lineNo */
        9,        /* colNo */
        "xgeqp3", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pName */
};

static emlrtRTEInfo p_emlrtRTEI =
    {
        92,       /* lineNo */
        22,       /* colNo */
        "xgeqp3", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pName */
};

static emlrtRTEInfo q_emlrtRTEI =
    {
        105,      /* lineNo */
        1,        /* colNo */
        "xgeqp3", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
        "xgeqp3.m" /* pName */
};

static emlrtRTEInfo r_emlrtRTEI = {
    70,        /* lineNo */
    23,        /* colNo */
    "xgetrfs", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+lapack/"
    "xgetrfs.m" /* pName */
};

static emlrtRTEInfo s_emlrtRTEI =
    {
        26,               /* lineNo */
        1,                /* colNo */
        "ctmc_stochcomp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pName */
};

static emlrtRTEInfo t_emlrtRTEI =
    {
        177,      /* lineNo */
        65,       /* colNo */
        "mtimes", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+blas/"
        "mtimes.m" /* pName */
};

static emlrtRTEInfo u_emlrtRTEI =
    {
        27,               /* lineNo */
        1,                /* colNo */
        "ctmc_stochcomp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pName */
};

static emlrtRTEInfo v_emlrtRTEI =
    {
        218,      /* lineNo */
        20,       /* colNo */
        "mtimes", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+blas/"
        "mtimes.m" /* pName */
};

static emlrtRTEInfo w_emlrtRTEI =
    {
        25,               /* lineNo */
        14,               /* colNo */
        "ctmc_stochcomp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pName */
};

static emlrtRTEInfo x_emlrtRTEI =
    {
        1,                /* lineNo */
        32,               /* colNo */
        "ctmc_stochcomp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pName */
};

static emlrtRSInfo ub_emlrtRSI = {
    53,        /* lineNo */
    "flt2str", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/shared/coder/coder/lib/+coder/+internal/"
    "flt2str.m" /* pathName */
};

static emlrtRSInfo vb_emlrtRSI =
    {
        27,               /* lineNo */
        "ctmc_stochcomp", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/mc/"
        "ctmc_stochcomp.m" /* pathName */
};

/* Function Declarations */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               char_T y[14]);

static const mxArray *b_sprintf(const emlrtStack *sp, const mxArray *m,
                                const mxArray *m1, emlrtMCInfo *location);

static void emlrt_marshallIn(const emlrtStack *sp,
                             const mxArray *a__output_of_sprintf_,
                             const char_T *identifier, char_T y[14]);

static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, char_T ret[14]);

static void plus(const emlrtStack *sp, emxArray_real_T *in1,
                 const emxArray_real_T *in2, const emxArray_real_T *in3);

/* Function Definitions */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId, char_T y[14])
{
  g_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

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

static void emlrt_marshallIn(const emlrtStack *sp,
                             const mxArray *a__output_of_sprintf_,
                             const char_T *identifier, char_T y[14])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  b_emlrt_marshallIn(sp, emlrtAlias(a__output_of_sprintf_), &thisId, y);
  emlrtDestroyArray(&a__output_of_sprintf_);
}

static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, char_T ret[14])
{
  static const int32_T dims[2] = {1, 14};
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "char", false, 2U,
                          (const void *)&dims[0]);
  emlrtImportCharArrayR2015b((emlrtConstCTX)sp, src, &ret[0], 14);
  emlrtDestroyArray(&src);
}

static void plus(const emlrtStack *sp, emxArray_real_T *in1,
                 const emxArray_real_T *in2, const emxArray_real_T *in3)
{
  const real_T *in2_data;
  const real_T *in3_data;
  real_T *in1_data;
  int32_T aux_0_1;
  int32_T aux_1_1;
  int32_T b_loop_ub;
  int32_T i;
  int32_T i1;
  int32_T loop_ub;
  int32_T stride_0_0;
  int32_T stride_0_1;
  int32_T stride_1_0;
  int32_T stride_1_1;
  in3_data = in3->data;
  in2_data = in2->data;
  if (in3->size[0] == 1) {
    loop_ub = in2->size[0];
  } else {
    loop_ub = in3->size[0];
  }
  stride_0_0 = in1->size[0] * in1->size[1];
  in1->size[0] = loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_0, &u_emlrtRTEI);
  if (in3->size[1] == 1) {
    b_loop_ub = in2->size[1];
  } else {
    b_loop_ub = in3->size[1];
  }
  stride_0_0 = in1->size[0] * in1->size[1];
  in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_0, &u_emlrtRTEI);
  in1_data = in1->data;
  stride_0_0 = (in2->size[0] != 1);
  stride_0_1 = (in2->size[1] != 1);
  stride_1_0 = (in3->size[0] != 1);
  stride_1_1 = (in3->size[1] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      in1_data[i1 + in1->size[0] * i] =
          in2_data[i1 * stride_0_0 + in2->size[0] * aux_0_1] +
          in3_data[i1 * stride_1_0 + in3->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_1;
    aux_0_1 += stride_0_1;
  }
}

void ctmc_stochcomp(const emlrtStack *sp, const emxArray_real_T *Q,
                    const emxArray_real_T *b_I, emxArray_real_T *S,
                    emxArray_real_T *Q11, emxArray_real_T *Q12,
                    emxArray_real_T *Q21, emxArray_real_T *Q22,
                    emxArray_real_T *T)
{
  static const int32_T offsets[4] = {0, 1, 2, 3};
  static const int32_T iv[2] = {1, 6};
  static const char_T b_fname[19] = {'L', 'A', 'P', 'A', 'C', 'K', 'E',
                                     '_', 'd', 'g', 'e', 't', 'r', 'f',
                                     '_', 'w', 'o', 'r', 'k'};
  static const char_T fname[14] = {'L', 'A', 'P', 'A', 'C', 'K', 'E',
                                   '_', 'd', 'g', 'e', 'q', 'p', '3'};
  static const char_T rfmt[6] = {'%', '1', '4', '.', '6', 'e'};
  __m128d r;
  ptrdiff_t k_t;
  ptrdiff_t lda_t;
  ptrdiff_t ldb_t;
  ptrdiff_t ldc_t;
  ptrdiff_t m_t;
  ptrdiff_t n_t;
  ptrdiff_t *jpvt_t_data;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack g_st;
  emlrtStack h_st;
  emlrtStack st;
  emxArray_boolean_T *isSelected;
  emxArray_int32_T *jpvt;
  emxArray_ptrdiff_t *jpvt_t;
  emxArray_real_T *A;
  emxArray_uint32_T *Ic;
  const mxArray *b_y;
  const mxArray *m;
  const mxArray *y;
  const real_T *I_data;
  const real_T *Q_data;
  real_T alpha1;
  real_T beta1;
  real_T *A_data;
  real_T *Q11_data;
  real_T *Q12_data;
  real_T *Q21_data;
  real_T *Q22_data;
  real_T *T_data;
  int32_T i;
  int32_T idx;
  int32_T loop_ub;
  int32_T ma;
  int32_T minmana;
  int32_T nB;
  int32_T na;
  int32_T *jpvt_data;
  uint32_T icPos;
  uint32_T *Ic_data;
  char_T TRANSA1;
  char_T TRANSB1;
  boolean_T *isSelected_data;
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
  I_data = b_I->data;
  Q_data = Q->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /*  [S,Q11,Q12,Q21,Q22,T] = CTMC_STOCHCOMP(Q,I) */
  /*  Copyright (c) 2012-2026, Imperial College London */
  /*  All rights reserved. */
  if ((Q->size[0] == 0) || (Q->size[1] == 0)) {
    minmana = 0;
  } else {
    minmana = muIntScalarMax_sint32(Q->size[0], Q->size[1]);
  }
  emxInit_boolean_T(sp, &isSelected, &g_emlrtRTEI);
  ma = isSelected->size[0] * isSelected->size[1];
  isSelected->size[0] = 1;
  isSelected->size[1] = minmana;
  emxEnsureCapacity_boolean_T(sp, isSelected, ma, &g_emlrtRTEI);
  isSelected_data = isSelected->data;
  for (idx = 0; idx < minmana; idx++) {
    isSelected_data[idx] = false;
  }
  na = b_I->size[1];
  for (idx = 0; idx < na; idx++) {
    if (idx + 1 > na) {
      emlrtDynamicBoundsCheckR2012b(idx + 1, 1, na, &b_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    alpha1 = I_data[idx];
    if (alpha1 != (int32_T)muDoubleScalarFloor(alpha1)) {
      emlrtIntegerCheckR2012b(alpha1, &b_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (((int32_T)alpha1 < 1) || ((int32_T)alpha1 > isSelected->size[1])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)alpha1, 1, isSelected->size[1],
                                    &emlrtBCI, (emlrtConstCTX)sp);
    }
    isSelected_data[(int32_T)alpha1 - 1] = true;
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  nB = minmana - b_I->size[1];
  if (nB < 0) {
    emlrtNonNegativeCheckR2012b(nB, &emlrtDCI, (emlrtConstCTX)sp);
  }
  emxInit_uint32_T(sp, &Ic, &h_emlrtRTEI);
  ma = Ic->size[0] * Ic->size[1];
  Ic->size[0] = 1;
  Ic->size[1] = nB;
  emxEnsureCapacity_uint32_T(sp, Ic, ma, &h_emlrtRTEI);
  Ic_data = Ic->data;
  for (idx = 0; idx < nB; idx++) {
    Ic_data[idx] = 0U;
  }
  icPos = 0U;
  for (idx = 0; idx < minmana; idx++) {
    if (idx + 1 > isSelected->size[1]) {
      emlrtDynamicBoundsCheckR2012b(idx + 1, 1, isSelected->size[1],
                                    &c_emlrtBCI, (emlrtConstCTX)sp);
    }
    if (!isSelected_data[idx]) {
      icPos++;
      if (((int32_T)icPos < 1) || ((int32_T)icPos > Ic->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)icPos, 1, Ic->size[1],
                                      &f_emlrtBCI, (emlrtConstCTX)sp);
      }
      Ic_data[(int32_T)icPos - 1] = (uint32_T)(idx + 1);
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  emxFree_boolean_T(sp, &isSelected);
  ma = Q11->size[0] * Q11->size[1];
  Q11->size[0] = b_I->size[1];
  Q11->size[1] = b_I->size[1];
  emxEnsureCapacity_real_T(sp, Q11, ma, &i_emlrtRTEI);
  Q11_data = Q11->data;
  for (idx = 0; idx < na; idx++) {
    for (i = 0; i < na; i++) {
      if (I_data[i] != (int32_T)muDoubleScalarFloor(I_data[i])) {
        emlrtIntegerCheckR2012b(I_data[i], &c_emlrtDCI, (emlrtConstCTX)sp);
      }
      ma = (int32_T)I_data[i];
      if ((ma < 1) || (ma > Q->size[0])) {
        emlrtDynamicBoundsCheckR2012b(ma, 1, Q->size[0], &d_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      nB = (int32_T)I_data[idx];
      if ((nB < 1) || (nB > Q->size[1])) {
        emlrtDynamicBoundsCheckR2012b(nB, 1, Q->size[1], &e_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      Q11_data[i + Q11->size[0] * idx] =
          Q_data[(ma + Q->size[0] * (nB - 1)) - 1];
    }
  }
  ma = Q12->size[0] * Q12->size[1];
  Q12->size[0] = b_I->size[1];
  loop_ub = Ic->size[1];
  Q12->size[1] = Ic->size[1];
  emxEnsureCapacity_real_T(sp, Q12, ma, &j_emlrtRTEI);
  Q12_data = Q12->data;
  for (idx = 0; idx < loop_ub; idx++) {
    for (i = 0; i < na; i++) {
      ma = (int32_T)I_data[i];
      if ((ma < 1) || (ma > Q->size[0])) {
        emlrtDynamicBoundsCheckR2012b(ma, 1, Q->size[0], &g_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (((int32_T)Ic_data[idx] < 1) || ((int32_T)Ic_data[idx] > Q->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)Ic_data[idx], 1, Q->size[1],
                                      &h_emlrtBCI, (emlrtConstCTX)sp);
      }
      Q12_data[i + Q12->size[0] * idx] =
          Q_data[(ma + Q->size[0] * ((int32_T)Ic_data[idx] - 1)) - 1];
    }
  }
  ma = Q21->size[0] * Q21->size[1];
  Q21->size[0] = Ic->size[1];
  Q21->size[1] = b_I->size[1];
  emxEnsureCapacity_real_T(sp, Q21, ma, &k_emlrtRTEI);
  Q21_data = Q21->data;
  for (idx = 0; idx < na; idx++) {
    for (i = 0; i < loop_ub; i++) {
      if (((int32_T)Ic_data[i] < 1) || ((int32_T)Ic_data[i] > Q->size[0])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)Ic_data[i], 1, Q->size[0],
                                      &i_emlrtBCI, (emlrtConstCTX)sp);
      }
      ma = (int32_T)I_data[idx];
      if ((ma < 1) || (ma > Q->size[1])) {
        emlrtDynamicBoundsCheckR2012b(ma, 1, Q->size[1], &j_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      Q21_data[i + Q21->size[0] * idx] =
          Q_data[((int32_T)Ic_data[i] + Q->size[0] * (ma - 1)) - 1];
    }
  }
  ma = Q22->size[0] * Q22->size[1];
  Q22->size[0] = Ic->size[1];
  Q22->size[1] = Ic->size[1];
  emxEnsureCapacity_real_T(sp, Q22, ma, &l_emlrtRTEI);
  Q22_data = Q22->data;
  for (idx = 0; idx < loop_ub; idx++) {
    for (i = 0; i < loop_ub; i++) {
      if (((int32_T)Ic_data[i] < 1) || ((int32_T)Ic_data[i] > Q->size[0])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)Ic_data[i], 1, Q->size[0],
                                      &k_emlrtBCI, (emlrtConstCTX)sp);
      }
      if (((int32_T)Ic_data[idx] < 1) || ((int32_T)Ic_data[idx] > Q->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)Ic_data[idx], 1, Q->size[1],
                                      &l_emlrtBCI, (emlrtConstCTX)sp);
      }
      Q22_data[i + Q22->size[0] * idx] =
          Q_data[((int32_T)Ic_data[i] +
                  Q->size[0] * ((int32_T)Ic_data[idx] - 1)) -
                 1];
    }
  }
  /* I = eye(size(Q22)); */
  st.site = &emlrtRSI;
  emxInit_real_T(&st, &A, 2, &t_emlrtRTEI);
  ma = A->size[0] * A->size[1];
  A->size[0] = Q22->size[0];
  A->size[1] = Q22->size[1];
  emxEnsureCapacity_real_T(&st, A, ma, &m_emlrtRTEI);
  A_data = A->data;
  ma = Q22->size[0] * Q22->size[1];
  nB = (ma / 2) << 1;
  minmana = nB - 2;
  for (idx = 0; idx <= minmana; idx += 2) {
    r = _mm_loadu_pd(&Q22_data[idx]);
    _mm_storeu_pd(&A_data[idx], _mm_mul_pd(r, _mm_set1_pd(-1.0)));
  }
  for (idx = nB; idx < ma; idx++) {
    A_data[idx] = -Q22_data[idx];
  }
  if (A->size[0] != Ic->size[1]) {
    emlrtErrorWithMessageIdR2018a(&st, &c_emlrtRTEI, "MATLAB:dimagree",
                                  "MATLAB:dimagree", 0);
  }
  b_st.site = &c_emlrtRSI;
  emxInit_int32_T(&b_st, &jpvt, &x_emlrtRTEI);
  emxInit_real_T(&b_st, &Q12, 1, &x_emlrtRTEI);
  emxInit_ptrdiff_t(&b_st, &jpvt_t, &q_emlrtRTEI);
  emxInit_real_T(&b_st, &Q22, 2, &w_emlrtRTEI);
  if ((A->size[0] == 0) || (A->size[1] == 0) ||
      ((Ic->size[1] == 0) || (b_I->size[1] == 0))) {
    ma = T->size[0] * T->size[1];
    T->size[0] = A->size[1];
    T->size[1] = b_I->size[1];
    emxEnsureCapacity_real_T(&b_st, T, ma, &n_emlrtRTEI);
    T_data = T->data;
    ma = A->size[1] * b_I->size[1];
    for (idx = 0; idx < ma; idx++) {
      T_data[idx] = 0.0;
    }
  } else if (A->size[0] == A->size[1]) {
    c_st.site = &d_emlrtRSI;
    d_st.site = &f_emlrtRSI;
    ma = T->size[0] * T->size[1];
    T->size[0] = Q21->size[0];
    T->size[1] = Q21->size[1];
    emxEnsureCapacity_real_T(&d_st, T, ma, &n_emlrtRTEI);
    T_data = T->data;
    ma = Q21->size[0] * Q21->size[1];
    for (idx = 0; idx < ma; idx++) {
      T_data[idx] = Q21_data[idx];
    }
    e_st.site = &g_emlrtRSI;
    f_st.site = &i_emlrtRSI;
    ma = A->size[0];
    nB = A->size[1];
    ma = muIntScalarMin_sint32(ma, nB);
    ma = muIntScalarMin_sint32(loop_ub, ma);
    g_st.site = &j_emlrtRSI;
    h_st.site = &l_emlrtRSI;
    nB = jpvt_t->size[0];
    jpvt_t->size[0] = ma;
    emxEnsureCapacity_ptrdiff_t(&h_st, jpvt_t, nB, &r_emlrtRTEI);
    jpvt_t_data = jpvt_t->data;
    m_t = (ptrdiff_t)ma;
    n_t = (ptrdiff_t)A->size[0];
    k_t = LAPACKE_dgetrf_work(102, m_t, m_t, &A_data[0], n_t, &jpvt_t_data[0]);
    h_st.site = &k_emlrtRSI;
    if ((int32_T)k_t < 0) {
      if ((int32_T)k_t == -1010) {
        emlrtErrorWithMessageIdR2018a(&h_st, &b_emlrtRTEI, "MATLAB:nomem",
                                      "MATLAB:nomem", 0);
      } else {
        emlrtErrorWithMessageIdR2018a(&h_st, &emlrtRTEI,
                                      "Coder:toolbox:LAPACKCallErrorInfo",
                                      "Coder:toolbox:LAPACKCallErrorInfo", 5, 4,
                                      19, &b_fname[0], 12, (int32_T)k_t);
      }
    }
    LAPACKE_dgetrs_work(102, 'N', m_t, (ptrdiff_t)b_I->size[1], &A_data[0], n_t,
                        &jpvt_t_data[0], &T_data[0], (ptrdiff_t)Ic->size[1]);
    if (((A->size[0] != 1) || (A->size[1] != 1)) && ((int32_T)k_t > 0)) {
      e_st.site = &h_emlrtRSI;
      f_st.site = &o_emlrtRSI;
      warning(&f_st);
    }
  } else {
    boolean_T p;
    c_st.site = &e_emlrtRSI;
    d_st.site = &p_emlrtRSI;
    nB = A->size[0];
    na = A->size[1];
    ma = jpvt->size[0] * jpvt->size[1];
    jpvt->size[0] = 1;
    jpvt->size[1] = A->size[1];
    emxEnsureCapacity_int32_T(&d_st, jpvt, ma, &o_emlrtRTEI);
    jpvt_data = jpvt->data;
    for (idx = 0; idx < na; idx++) {
      jpvt_data[idx] = 0;
    }
    e_st.site = &s_emlrtRSI;
    minmana = muIntScalarMin_sint32(nB, na);
    ma = Q12->size[0];
    Q12->size[0] = minmana;
    emxEnsureCapacity_real_T(&e_st, Q12, ma, &p_emlrtRTEI);
    Q22_data = Q12->data;
    ma = jpvt_t->size[0];
    jpvt_t->size[0] = A->size[1];
    emxEnsureCapacity_ptrdiff_t(&e_st, jpvt_t, ma, &q_emlrtRTEI);
    jpvt_t_data = jpvt_t->data;
    for (idx = 0; idx < na; idx++) {
      jpvt_t_data[idx] = (ptrdiff_t)0;
    }
    m_t = LAPACKE_dgeqp3(102, (ptrdiff_t)A->size[0], (ptrdiff_t)A->size[1],
                         &A_data[0], (ptrdiff_t)A->size[0], &jpvt_t_data[0],
                         &Q22_data[0]);
    f_st.site = &t_emlrtRSI;
    if ((int32_T)m_t != 0) {
      p = true;
      if ((int32_T)m_t != -4) {
        if ((int32_T)m_t == -1010) {
          emlrtErrorWithMessageIdR2018a(&f_st, &b_emlrtRTEI, "MATLAB:nomem",
                                        "MATLAB:nomem", 0);
        } else {
          emlrtErrorWithMessageIdR2018a(&f_st, &emlrtRTEI,
                                        "Coder:toolbox:LAPACKCallErrorInfo",
                                        "Coder:toolbox:LAPACKCallErrorInfo", 5,
                                        4, 14, &fname[0], 12, (int32_T)m_t);
        }
      }
    } else {
      p = false;
    }
    if (p) {
      f_st.site = &u_emlrtRSI;
      if (A->size[1] > 2147483646) {
        g_st.site = &n_emlrtRSI;
        check_forloop_overflow_error(&g_st);
      }
      for (idx = 0; idx < na; idx++) {
        f_st.site = &v_emlrtRSI;
        if (nB > 2147483646) {
          g_st.site = &n_emlrtRSI;
          check_forloop_overflow_error(&g_st);
        }
        for (i = 0; i < nB; i++) {
          A_data[idx * nB + i] = rtNaN;
        }
      }
      f_st.site = &w_emlrtRSI;
      if (minmana > 2147483646) {
        g_st.site = &n_emlrtRSI;
        check_forloop_overflow_error(&g_st);
      }
      for (idx = 0; idx < minmana; idx++) {
        Q22_data[idx] = rtNaN;
      }
      ma = minmana + 1;
      f_st.site = &x_emlrtRSI;
      for (idx = ma; idx <= minmana; idx++) {
        Q22_data[idx - 1] = 0.0;
      }
      f_st.site = &y_emlrtRSI;
      ma = (A->size[1] / 4) << 2;
      nB = ma - 4;
      for (idx = 0; idx <= nB; idx += 4) {
        _mm_storeu_si128(
            (__m128i *)&jpvt_data[idx],
            _mm_add_epi32(
                _mm_add_epi32(_mm_set1_epi32(idx),
                              _mm_loadu_si128((const __m128i *)&offsets[0])),
                _mm_set1_epi32(1)));
      }
      for (idx = ma; idx < na; idx++) {
        jpvt_data[idx] = idx + 1;
      }
    } else {
      f_st.site = &ab_emlrtRSI;
      if (A->size[1] > 2147483646) {
        g_st.site = &n_emlrtRSI;
        check_forloop_overflow_error(&g_st);
      }
      for (idx = 0; idx < na; idx++) {
        jpvt_data[idx] = (int32_T)jpvt_t_data[idx];
      }
    }
    d_st.site = &q_emlrtRSI;
    minmana = 0;
    if (A->size[0] < A->size[1]) {
      ma = A->size[0];
      nB = A->size[1];
    } else {
      ma = A->size[1];
      nB = A->size[0];
    }
    alpha1 = muDoubleScalarMin(1.4901161193847656E-8,
                               2.2204460492503131E-15 * (real_T)nB) *
             muDoubleScalarAbs(A_data[0]);
    while ((minmana < ma) &&
           (!(muDoubleScalarAbs(A_data[minmana + A->size[0] * minmana]) <=
              alpha1))) {
      minmana++;
    }
    if (minmana < ma) {
      char_T str[14];
      e_st.site = &bb_emlrtRSI;
      y = NULL;
      m = emlrtCreateCharArray(2, &iv[0]);
      emlrtInitCharArrayR2013a(&e_st, 6, m, &rfmt[0]);
      emlrtAssign(&y, m);
      b_y = NULL;
      m = emlrtCreateDoubleScalar(alpha1);
      emlrtAssign(&b_y, m);
      f_st.site = &ub_emlrtRSI;
      emlrt_marshallIn(&f_st, b_sprintf(&f_st, y, b_y, &c_emlrtMCI),
                       "<output of sprintf>", str);
      e_st.site = &cb_emlrtRSI;
      b_warning(&e_st, minmana, str);
    }
    ma = Q22->size[0] * Q22->size[1];
    Q22->size[0] = Q21->size[0];
    Q22->size[1] = Q21->size[1];
    emxEnsureCapacity_real_T(&c_st, Q22, ma, &w_emlrtRTEI);
    Q22_data = Q22->data;
    ma = Q21->size[0] * Q21->size[1] - 1;
    for (idx = 0; idx <= ma; idx++) {
      Q22_data[idx] = Q21_data[idx];
    }
    d_st.site = &r_emlrtRSI;
    LSQFromQR(&d_st, A, Q12, jpvt, Q22, minmana, T);
    T_data = T->data;
  }
  emxFree_real_T(&b_st, &Q22);
  emxFree_ptrdiff_t(&b_st, &jpvt_t);
  emxFree_real_T(&b_st, &Q12);
  emxFree_int32_T(&b_st, &jpvt);
  st.site = &b_emlrtRSI;
  b_st.site = &qb_emlrtRSI;
  if (T->size[0] != Ic->size[1]) {
    if (((b_I->size[1] == 1) && (Ic->size[1] == 1)) ||
        ((T->size[0] == 1) && (T->size[1] == 1))) {
      emlrtErrorWithMessageIdR2018a(
          &b_st, &d_emlrtRTEI, "Coder:toolbox:mtimes_noDynamicScalarExpansion",
          "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
    } else {
      emlrtErrorWithMessageIdR2018a(&b_st, &e_emlrtRTEI, "MATLAB:innerdim",
                                    "MATLAB:innerdim", 0);
    }
  }
  b_st.site = &pb_emlrtRSI;
  nB = T->size[1];
  if ((b_I->size[1] == 0) || (Ic->size[1] == 0) || (T->size[0] == 0) ||
      (T->size[1] == 0)) {
    ma = T->size[0] * T->size[1];
    T->size[0] = b_I->size[1];
    T->size[1] = nB;
    emxEnsureCapacity_real_T(&b_st, T, ma, &s_emlrtRTEI);
    T_data = T->data;
    ma = b_I->size[1] * nB;
    for (idx = 0; idx < ma; idx++) {
      T_data[idx] = 0.0;
    }
  } else {
    c_st.site = &rb_emlrtRSI;
    d_st.site = &sb_emlrtRSI;
    ma = A->size[0] * A->size[1];
    A->size[0] = T->size[0];
    A->size[1] = T->size[1];
    emxEnsureCapacity_real_T(&d_st, A, ma, &t_emlrtRTEI);
    A_data = A->data;
    ma = T->size[0] * T->size[1];
    for (idx = 0; idx < ma; idx++) {
      A_data[idx] = T_data[idx];
    }
    TRANSB1 = 'N';
    TRANSA1 = 'N';
    alpha1 = 1.0;
    beta1 = 0.0;
    m_t = (ptrdiff_t)b_I->size[1];
    n_t = (ptrdiff_t)T->size[1];
    k_t = (ptrdiff_t)Ic->size[1];
    lda_t = (ptrdiff_t)b_I->size[1];
    ldb_t = (ptrdiff_t)T->size[0];
    ldc_t = (ptrdiff_t)b_I->size[1];
    ma = T->size[0] * T->size[1];
    T->size[0] = b_I->size[1];
    T->size[1] = nB;
    emxEnsureCapacity_real_T(&d_st, T, ma, &v_emlrtRTEI);
    T_data = T->data;
    dgemm(&TRANSA1, &TRANSB1, &m_t, &n_t, &k_t, &alpha1, &Q12_data[0], &lda_t,
          &A_data[0], &ldb_t, &beta1, &T_data[0], &ldc_t);
  }
  emxFree_real_T(&b_st, &A);
  emxFree_uint32_T(&b_st, &Ic);
  if ((T->size[0] != b_I->size[1]) &&
      ((b_I->size[1] != 1) && (T->size[0] != 1))) {
    emlrtDimSizeImpxCheckR2021b(b_I->size[1], T->size[0], &b_emlrtECI,
                                (emlrtConstCTX)sp);
  }
  if ((b_I->size[1] != T->size[1]) &&
      ((b_I->size[1] != 1) && (T->size[1] != 1))) {
    emlrtDimSizeImpxCheckR2021b(b_I->size[1], T->size[1], &emlrtECI,
                                (emlrtConstCTX)sp);
  }
  if ((Q11->size[0] == T->size[0]) && (Q11->size[1] == T->size[1])) {
    ma = S->size[0] * S->size[1];
    S->size[0] = Q11->size[0];
    S->size[1] = Q11->size[1];
    emxEnsureCapacity_real_T(sp, S, ma, &u_emlrtRTEI);
    Q22_data = S->data;
    ma = Q11->size[0] * Q11->size[1];
    nB = (ma / 2) << 1;
    minmana = nB - 2;
    for (idx = 0; idx <= minmana; idx += 2) {
      __m128d r1;
      r = _mm_loadu_pd(&Q11_data[idx]);
      r1 = _mm_loadu_pd(&T_data[idx]);
      _mm_storeu_pd(&Q22_data[idx], _mm_add_pd(r, r1));
    }
    for (idx = nB; idx < ma; idx++) {
      Q22_data[idx] = Q11_data[idx] + T_data[idx];
    }
  } else {
    st.site = &vb_emlrtRSI;
    plus(&st, S, Q11, T);
  }
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (ctmc_stochcomp.c) */
