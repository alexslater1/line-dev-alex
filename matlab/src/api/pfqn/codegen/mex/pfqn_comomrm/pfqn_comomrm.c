/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_comomrm.c
 *
 * Code generation for function 'pfqn_comomrm'
 *
 */

/* Include files */
#include "pfqn_comomrm.h"
#include "abs.h"
#include "applyScalarFunctionInPlace.h"
#include "error.h"
#include "exp.h"
#include "eye.h"
#include "gammaln.h"
#include "log.h"
#include "mtimes.h"
#include "pfqn_comomrm_data.h"
#include "pfqn_comomrm_emxutil.h"
#include "pfqn_comomrm_types.h"
#include "pfqn_nc_sanitize.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "sum.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>

/* Variable Definitions */
static emlrtRSInfo emlrtRSI =
    {
        125,            /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo b_emlrtRSI =
    {
        124,            /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo c_emlrtRSI =
    {
        119,            /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo d_emlrtRSI =
    {
        118,            /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo e_emlrtRSI =
    {
        117,            /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo f_emlrtRSI =
    {
        116,            /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo g_emlrtRSI =
    {
        113,            /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo h_emlrtRSI =
    {
        107,            /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo i_emlrtRSI =
    {
        105,            /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo j_emlrtRSI =
    {
        76,             /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo k_emlrtRSI =
    {
        75,             /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo l_emlrtRSI =
    {
        72,             /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo m_emlrtRSI =
    {
        71,             /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo n_emlrtRSI =
    {
        69,             /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo o_emlrtRSI =
    {
        63,             /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo p_emlrtRSI =
    {
        62,             /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo q_emlrtRSI =
    {
        58,             /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo r_emlrtRSI =
    {
        55,             /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo s_emlrtRSI =
    {
        54,             /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo t_emlrtRSI =
    {
        50,             /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo u_emlrtRSI =
    {
        31,             /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo v_emlrtRSI =
    {
        25,             /* lineNo */
        "pfqn_comomrm", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pathName */
};

static emlrtRSInfo w_emlrtRSI = {
    37,           /* lineNo */
    "line_error", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/io/line_error.m" /* pathName
                                                                          */
};

static emlrtRSInfo ne_emlrtRSI = {
    8,                                                             /* lineNo */
    "factln",                                                      /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/factln.m" /* pathName
                                                                    */
};

static emlrtRSInfo oe_emlrtRSI = {
    10,        /* lineNo */
    "gammaln", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/gammaln.m" /* pathName
                                                                         */
};

static emlrtRSInfo pe_emlrtRSI = {
    17,                           /* lineNo */
    "applyScalarFunctionInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunctionInPlace.m" /* pathName */
};

static emlrtRSInfo bf_emlrtRSI = {
    41,    /* lineNo */
    "cat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pathName
                                                                       */
};

static emlrtRSInfo cf_emlrtRSI = {
    65,         /* lineNo */
    "cat_impl", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pathName
                                                                       */
};

static emlrtRSInfo df_emlrtRSI =
    {
        94,                  /* lineNo */
        "eml_mtimes_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pathName */
};

static emlrtRSInfo gf_emlrtRSI = {
    42,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sort.m" /* pathName
                                                                      */
};

static emlrtECInfo emlrtECI =
    {
        2,              /* nDims */
        116,            /* lineNo */
        18,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtECInfo b_emlrtECI =
    {
        1,              /* nDims */
        116,            /* lineNo */
        18,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtDCInfo emlrtDCI = {
    111,            /* lineNo */
    29,             /* colNo */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    105,            /* lineNo */
    43,             /* colNo */
    "Z",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo b_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    105,            /* lineNo */
    28,             /* colNo */
    "L",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo c_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    34,             /* colNo */
    "h_1",          /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtECInfo c_emlrtECI =
    {
        -1,             /* nDims */
        88,             /* lineNo */
        21,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtBCInfo d_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    88,             /* lineNo */
    31,             /* colNo */
    "hr",           /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo e_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    88,             /* lineNo */
    25,             /* colNo */
    "hr",           /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo f_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    87,             /* lineNo */
    27,             /* colNo */
    "hr",           /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo g_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    88,             /* lineNo */
    53,             /* colNo */
    "h",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo h_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    88,             /* lineNo */
    44,             /* colNo */
    "h",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo i_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    87,             /* lineNo */
    40,             /* colNo */
    "h",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo j_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    87,             /* lineNo */
    37,             /* colNo */
    "h",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtRTEInfo c_emlrtRTEI =
    {
        82,             /* lineNo */
        16,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtBCInfo k_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    82,             /* lineNo */
    20,             /* colNo */
    "N",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo b_emlrtDCI = {
    81,             /* lineNo */
    21,             /* colNo */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    1                 /* checkKind */
};

static emlrtDCInfo c_emlrtDCI = {
    80,             /* lineNo */
    21,             /* colNo */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    1                 /* checkKind */
};

static emlrtRTEInfo d_emlrtRTEI =
    {
        79,             /* lineNo */
        11,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtDCInfo d_emlrtDCI = {
    75,             /* lineNo */
    20,             /* colNo */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    1                 /* checkKind */
};

static emlrtDCInfo e_emlrtDCI = {
    75,             /* lineNo */
    20,             /* colNo */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    4                 /* checkKind */
};

static emlrtBCInfo l_emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    61,               /* lineNo */
    28,               /* colNo */
    "zerothinktimes", /* aName */
    "pfqn_comomrm",   /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo m_emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    53,               /* lineNo */
    28,               /* colNo */
    "zerothinktimes", /* aName */
    "pfqn_comomrm",   /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo n_emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    44,               /* lineNo */
    30,               /* colNo */
    "zerothinktimes", /* aName */
    "pfqn_comomrm",   /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtRTEInfo e_emlrtRTEI = {
    225,                   /* lineNo */
    27,                    /* colNo */
    "check_non_axis_size", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pName
                                                                       */
};

static emlrtBCInfo o_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    35,             /* lineNo */
    10,             /* colNo */
    "Z",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo p_emlrtBCI = {
    -1,               /* iFirst */
    -1,               /* iLast */
    37,               /* lineNo */
    24,               /* colNo */
    "zerothinktimes", /* aName */
    "pfqn_comomrm",   /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo f_emlrtDCI = {
    47,             /* lineNo */
    5,              /* colNo */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo q_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    45,             /* lineNo */
    23,             /* colNo */
    "N",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo r_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    45,             /* lineNo */
    14,             /* colNo */
    "nvec",         /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo s_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    72,             /* lineNo */
    22,             /* colNo */
    "h",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo t_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    124,            /* lineNo */
    22,             /* colNo */
    "h",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo u_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    83,             /* lineNo */
    26,             /* colNo */
    "nvec",         /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo g_emlrtDCI = {
    86,             /* lineNo */
    21,             /* colNo */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo v_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    116,            /* lineNo */
    31,             /* colNo */
    "nvec",         /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo w_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    101,            /* lineNo */
    36,             /* colNo */
    "N",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo x_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    101,            /* lineNo */
    25,             /* colNo */
    "A12",          /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo y_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    102,            /* lineNo */
    39,             /* colNo */
    "Z",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ab_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    102,            /* lineNo */
    25,             /* colNo */
    "A12",          /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo bb_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    102,            /* lineNo */
    29,             /* colNo */
    "A12",          /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo cb_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    105,            /* lineNo */
    30,             /* colNo */
    "L",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo db_emlrtBCI = {
    -1,                                                           /* iFirst */
    -1,                                                           /* iLast */
    9,                                                            /* lineNo */
    16,                                                           /* colNo */
    "N",                                                          /* aName */
    "oner",                                                       /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/oner.m", /* pName */
    0 /* checkKind */
};

static emlrtBCInfo eb_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    43,             /* colNo */
    "scale",        /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo h_emlrtDCI = {
    92,             /* lineNo */
    43,             /* colNo */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo fb_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    27,             /* colNo */
    "h",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo gb_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    93,             /* lineNo */
    36,             /* colNo */
    "h_1",          /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo hb_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    93,             /* lineNo */
    51,             /* colNo */
    "scale",        /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ib_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    93,             /* lineNo */
    27,             /* colNo */
    "h",            /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo jb_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    118,            /* lineNo */
    19,             /* colNo */
    "scale",        /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo i_emlrtDCI = {
    118,            /* lineNo */
    19,             /* colNo */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo kb_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    119,            /* lineNo */
    30,             /* colNo */
    "scale",        /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo lb_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    58,             /* lineNo */
    8,              /* colNo */
    "lh",           /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo mb_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    55,             /* lineNo */
    12,             /* colNo */
    "lh",           /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo nb_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    63,             /* lineNo */
    12,             /* colNo */
    "lh",           /* aName */
    "pfqn_comomrm", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_comomrm.m", /* pName */
    0                 /* checkKind */
};

static emlrtRTEInfo u_emlrtRTEI =
    {
        31,             /* lineNo */
        2,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo v_emlrtRTEI =
    {
        31,             /* lineNo */
        4,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo w_emlrtRTEI =
    {
        31,             /* lineNo */
        6,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo x_emlrtRTEI =
    {
        31,             /* lineNo */
        8,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo y_emlrtRTEI =
    {
        32,             /* lineNo */
        1,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo ab_emlrtRTEI =
    {
        41,             /* lineNo */
        1,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo bb_emlrtRTEI =
    {
        67,             /* lineNo */
        5,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo cb_emlrtRTEI =
    {
        47,             /* lineNo */
        5,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo db_emlrtRTEI =
    {
        71,             /* lineNo */
        5,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo eb_emlrtRTEI =
    {
        75,             /* lineNo */
        5,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo fb_emlrtRTEI =
    {
        77,             /* lineNo */
        5,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo gb_emlrtRTEI =
    {
        80,             /* lineNo */
        9,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo hb_emlrtRTEI =
    {
        124,            /* lineNo */
        40,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo ib_emlrtRTEI =
    {
        81,             /* lineNo */
        9,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo jb_emlrtRTEI = {
    8,                                                             /* lineNo */
    1,                                                             /* colNo */
    "factln",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/factln.m" /* pName */
};

static emlrtRTEInfo kb_emlrtRTEI =
    {
        125,            /* lineNo */
        5,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo lb_emlrtRTEI =
    {
        115,            /* lineNo */
        13,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo mb_emlrtRTEI =
    {
        86,             /* lineNo */
        21,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo nb_emlrtRTEI =
    {
        54,             /* lineNo */
        9,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo ob_emlrtRTEI =
    {
        97,             /* lineNo */
        17,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo pb_emlrtRTEI =
    {
        116,            /* lineNo */
        22,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo qb_emlrtRTEI =
    {
        116,            /* lineNo */
        18,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo rb_emlrtRTEI =
    {
        115,            /* lineNo */
        19,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo sb_emlrtRTEI =
    {
        89,             /* lineNo */
        21,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo tb_emlrtRTEI = {
    42,     /* lineNo */
    5,      /* colNo */
    "sort", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sort.m" /* pName */
};

static emlrtRTEInfo ub_emlrtRTEI =
    {
        62,             /* lineNo */
        9,              /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo vb_emlrtRTEI =
    {
        105,            /* lineNo */
        17,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo wb_emlrtRTEI =
    {
        119,            /* lineNo */
        13,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo xb_emlrtRTEI =
    {
        111,            /* lineNo */
        17,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo yb_emlrtRTEI =
    {
        113,            /* lineNo */
        17,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo ac_emlrtRTEI =
    {
        107,            /* lineNo */
        17,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

static emlrtRTEInfo bc_emlrtRTEI =
    {
        21,             /* lineNo */
        23,             /* colNo */
        "pfqn_comomrm", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comomrm.m" /* pName */
};

/* Function Declarations */
static void binary_expand_op(const emlrtStack *sp, emxArray_real_T *in1,
                             const emxArray_int8_T *in2);

/* Function Definitions */
static void binary_expand_op(const emlrtStack *sp, emxArray_real_T *in1,
                             const emxArray_int8_T *in2)
{
  emxArray_real_T *b_in2;
  real_T *b_in2_data;
  real_T *in1_data;
  int32_T aux_0_1;
  int32_T b_loop_ub;
  int32_T i;
  int32_T i1;
  int32_T loop_ub;
  int32_T stride_0_0;
  int32_T stride_0_1;
  const int8_T *in2_data;
  in2_data = in2->data;
  in1_data = in1->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &b_in2, 2, &qb_emlrtRTEI);
  loop_ub = in2->size[0];
  stride_0_0 = b_in2->size[0] * b_in2->size[1];
  b_in2->size[0] = loop_ub;
  b_loop_ub = in2->size[1];
  b_in2->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(sp, b_in2, stride_0_0, &qb_emlrtRTEI);
  b_in2_data = b_in2->data;
  stride_0_0 = (in1->size[0] != 1);
  stride_0_1 = (in1->size[1] != 1);
  aux_0_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_in2_data[i1 + b_in2->size[0] * i] =
          (real_T)in2_data[i1 + in2->size[0] * i] +
          in1_data[i1 * stride_0_0 + in1->size[0] * aux_0_1];
    }
    aux_0_1 += stride_0_1;
  }
  stride_0_0 = in1->size[0] * in1->size[1];
  in1->size[0] = loop_ub;
  in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_0, &qb_emlrtRTEI);
  in1_data = in1->data;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      in1_data[i1 + in1->size[0] * i] = b_in2_data[i1 + b_in2->size[0] * i];
    }
  }
  emxFree_real_T(sp, &b_in2);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
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

void pfqn_comomrm(const emlrtStack *sp, const emxArray_real_T *L,
                  const emxArray_real_T *N, const emxArray_real_T *Z, real_T m,
                  real_T b_atol, real_T *lG, emxArray_real_T *lGbasis)
{
  __m128d d_r;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack st;
  emxArray_int8_T *F1r;
  emxArray_real_T *A12;
  emxArray_real_T *B2r;
  emxArray_real_T *F2r;
  emxArray_real_T *b_L;
  emxArray_real_T *b_N;
  emxArray_real_T *b_Z;
  emxArray_real_T *h_1;
  emxArray_real_T *hr;
  emxArray_real_T *iC;
  emxArray_real_T *lh;
  emxArray_real_T *nvec;
  emxArray_real_T *nvec_s;
  emxArray_real_T *zerothinktimes;
  const real_T *L_data;
  const real_T *N_data;
  const real_T *Z_data;
  real_T lG0;
  real_T n;
  real_T *F2r_data;
  real_T *b_L_data;
  real_T *b_N_data;
  real_T *b_Z_data;
  real_T *h_1_data;
  real_T *iC_data;
  real_T *lh_data;
  real_T *nvec_data;
  real_T *nvec_s_data;
  real_T *zerothinktimes_data;
  int32_T Nr;
  int32_T b_loop_ub;
  int32_T b_r;
  int32_T i;
  int32_T loop_ub;
  int32_T r;
  int32_T scalarLB;
  int32_T sizes_idx_0;
  int32_T vectorUB;
  int32_T z;
  uint32_T numZeroThinkTimes;
  int8_T *F1r_data;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  Z_data = Z->data;
  N_data = N->data;
  L_data = L->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /* { */
  /*  % @file pfqn_comomrm.m */
  /*  % @brief CoMoM (Convolution Method of Moments) for finite repairman model.
   */
  /* } */
  /* { */
  /*  % @brief CoMoM (Convolution Method of Moments) for finite repairman model.
   */
  /*  % @fn pfqn_comomrm(L, N, Z, m, atol) */
  /*  % @param L Service demand matrix. */
  /*  % @param N Population vector. */
  /*  % @param Z Think time vector. */
  /*  % @param m Replication factor (default: 1). */
  /*  % @param atol Absolute tolerance for numerical computations. */
  /*  % @return lG Logarithm of normalizing constant. */
  /*  % @return lGbasis Logarithm of basis functions. */
  /* } */
  /*  comom for a finite repairment model */
  if (L->size[0] != 1) {
    st.site = &v_emlrtRSI;
    /* LINE_ERROR Display a plain-text error message with file and line info. */
    /*  */
    /*    LINE_ERROR(CALLER, MSG, ...) throws an error with CALLER's name and */
    /*    message, including the source file and line number, in plain text (no
     */
    /*    hyperlink). Extra arguments are passed to sprintf to format MSG. */
    /*    Copyright (c) 2012-2026, Imperial College London */
    /*    All rights reserved. */
    b_st.site = &w_emlrtRSI;
    b_error(&b_st);
  }
  emxInit_real_T(sp, &nvec_s, 2, &nb_emlrtRTEI);
  scalarLB = nvec_s->size[0] * nvec_s->size[1];
  nvec_s->size[0] = 1;
  sizes_idx_0 = N->size[1];
  nvec_s->size[1] = N->size[1];
  emxEnsureCapacity_real_T(sp, nvec_s, scalarLB, &u_emlrtRTEI);
  nvec_s_data = nvec_s->data;
  scalarLB = (N->size[1] / 2) << 1;
  vectorUB = scalarLB - 2;
  for (r = 0; r <= vectorUB; r += 2) {
    _mm_storeu_pd(&nvec_s_data[r],
                  _mm_mul_pd(_mm_set1_pd(0.0), _mm_loadu_pd(&N_data[r])));
  }
  for (r = scalarLB; r < sizes_idx_0; r++) {
    nvec_s_data[r] = 0.0 * N_data[r];
  }
  emxInit_real_T(sp, &b_L, 2, &bc_emlrtRTEI);
  scalarLB = b_L->size[0] * b_L->size[1];
  b_L->size[0] = 1;
  loop_ub = L->size[1];
  b_L->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, b_L, scalarLB, &v_emlrtRTEI);
  b_L_data = b_L->data;
  for (r = 0; r < loop_ub; r++) {
    b_L_data[r] = L_data[r];
  }
  emxInit_real_T(sp, &b_N, 2, &bc_emlrtRTEI);
  scalarLB = b_N->size[0] * b_N->size[1];
  b_N->size[0] = 1;
  b_N->size[1] = N->size[1];
  emxEnsureCapacity_real_T(sp, b_N, scalarLB, &w_emlrtRTEI);
  b_N_data = b_N->data;
  for (r = 0; r < sizes_idx_0; r++) {
    b_N_data[r] = N_data[r];
  }
  emxInit_real_T(sp, &b_Z, 2, &bc_emlrtRTEI);
  scalarLB = b_Z->size[0] * b_Z->size[1];
  b_Z->size[0] = 1;
  vectorUB = Z->size[1];
  b_Z->size[1] = Z->size[1];
  emxEnsureCapacity_real_T(sp, b_Z, scalarLB, &x_emlrtRTEI);
  b_Z_data = b_Z->data;
  for (r = 0; r < vectorUB; r++) {
    b_Z_data[r] = Z_data[r];
  }
  st.site = &u_emlrtRSI;
  lG0 = pfqn_nc_sanitize(&st, nvec_s, b_L, b_N, b_Z, b_atol);
  b_Z_data = b_Z->data;
  b_N_data = b_N->data;
  b_L_data = b_L->data;
  emxInit_real_T(sp, &zerothinktimes, 2, &y_emlrtRTEI);
  scalarLB = zerothinktimes->size[0] * zerothinktimes->size[1];
  zerothinktimes->size[0] = 1;
  zerothinktimes->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, zerothinktimes, scalarLB, &y_emlrtRTEI);
  zerothinktimes_data = zerothinktimes->data;
  for (r = 0; r < loop_ub; r++) {
    zerothinktimes_data[r] = 0.0;
  }
  numZeroThinkTimes = 0U;
  for (r = 0; r < loop_ub; r++) {
    if (r + 1 > b_Z->size[1]) {
      emlrtDynamicBoundsCheckR2012b(r + 1, 1, b_Z->size[1], &o_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (b_Z_data[r] < 1.0E-6) {
      numZeroThinkTimes++;
      if (((int32_T)numZeroThinkTimes < 1) ||
          ((int32_T)numZeroThinkTimes > zerothinktimes->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)numZeroThinkTimes, 1,
                                      zerothinktimes->size[1], &p_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      zerothinktimes_data[(int32_T)numZeroThinkTimes - 1] = (real_T)r + 1.0;
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  /*  initialize */
  emxInit_real_T(sp, &nvec, 2, &ab_emlrtRTEI);
  scalarLB = nvec->size[0] * nvec->size[1];
  nvec->size[0] = 1;
  nvec->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, nvec, scalarLB, &ab_emlrtRTEI);
  nvec_data = nvec->data;
  for (r = 0; r < loop_ub; r++) {
    nvec_data[r] = 0.0;
  }
  emxInit_real_T(sp, &lh, 1, &cb_emlrtRTEI);
  if ((int32_T)numZeroThinkTimes > 0) {
    i = (int32_T)numZeroThinkTimes;
    for (r = 0; r < i; r++) {
      if (r + 1 > zerothinktimes->size[1]) {
        emlrtDynamicBoundsCheckR2012b(r + 1, 1, zerothinktimes->size[1],
                                      &n_emlrtBCI, (emlrtConstCTX)sp);
      }
      scalarLB = (int32_T)zerothinktimes_data[r];
      if ((scalarLB < 1) || (scalarLB > b_N->size[1])) {
        emlrtDynamicBoundsCheckR2012b(scalarLB, 1, b_N->size[1], &q_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (scalarLB > nvec->size[1]) {
        emlrtDynamicBoundsCheckR2012b(scalarLB, 1, nvec->size[1], &r_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      nvec_data[scalarLB - 1] = b_N_data[scalarLB - 1];
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    n = 2.0 * (real_T)numZeroThinkTimes + 2.0;
    if (n != (int32_T)n) {
      emlrtIntegerCheckR2012b(n, &f_emlrtDCI, (emlrtConstCTX)sp);
    }
    vectorUB = (int32_T)n;
    scalarLB = lh->size[0];
    lh->size[0] = (int32_T)n;
    emxEnsureCapacity_real_T(sp, lh, scalarLB, &cb_emlrtRTEI);
    lh_data = lh->data;
    if (n != (int32_T)n) {
      emlrtIntegerCheckR2012b(n, &f_emlrtDCI, (emlrtConstCTX)sp);
    }
    for (r = 0; r < vectorUB; r++) {
      lh_data[r] = 0.0;
    }
    /*  these are trivial models with a single queueing station with demands all
     * equal to one and think time 0 */
    st.site = &t_emlrtRSI;
    b_st.site = &t_emlrtRSI;
    n = ((b_sum(&b_st, nvec) + m) + 1.0) - 1.0;
    /*  lf=FACTLN(n) */
    /*  Compure the logarithm of n!        */
    /*  */
    /*  Copyright (c) 2012-2026, Imperial College London */
    /*  All rights reserved.   */
    b_st.site = &ne_emlrtRSI;
    c_st.site = &oe_emlrtRSI;
    n++;
    d_st.site = &pe_emlrtRSI;
    scalar_gammaln(&d_st, &n);
    st.site = &t_emlrtRSI;
    /*  lf=FACTLN(n) */
    /*  Compure the logarithm of n!        */
    /*  */
    /*  Copyright (c) 2012-2026, Imperial College London */
    /*  All rights reserved.   */
    b_st.site = &ne_emlrtRSI;
    scalarLB = nvec_s->size[0] * nvec_s->size[1];
    nvec_s->size[0] = 1;
    b_loop_ub = nvec->size[1];
    nvec_s->size[1] = nvec->size[1];
    emxEnsureCapacity_real_T(&b_st, nvec_s, scalarLB, &jb_emlrtRTEI);
    nvec_s_data = nvec_s->data;
    loop_ub = (nvec->size[1] / 2) << 1;
    scalarLB = loop_ub - 2;
    for (r = 0; r <= scalarLB; r += 2) {
      d_r = _mm_loadu_pd(&nvec_data[r]);
      _mm_storeu_pd(&nvec_s_data[r], _mm_add_pd(d_r, _mm_set1_pd(1.0)));
    }
    for (r = loop_ub; r < b_loop_ub; r++) {
      nvec_s_data[r] = nvec_data[r] + 1.0;
    }
    c_st.site = &oe_emlrtRSI;
    applyScalarFunctionInPlace(&c_st, nvec_s);
    st.site = &t_emlrtRSI;
    lh_data[0] = n - b_sum(&st, nvec_s);
    for (z = 0; z < i; z++) {
      if (z + 1 > zerothinktimes->size[1]) {
        emlrtDynamicBoundsCheckR2012b(z + 1, 1, zerothinktimes->size[1],
                                      &m_emlrtBCI, (emlrtConstCTX)sp);
      }
      st.site = &s_emlrtRSI;
      scalarLB = nvec_s->size[0] * nvec_s->size[1];
      nvec_s->size[0] = 1;
      nvec_s->size[1] = b_loop_ub;
      emxEnsureCapacity_real_T(&st, nvec_s, scalarLB, &nb_emlrtRTEI);
      nvec_s_data = nvec_s->data;
      for (r = 0; r < b_loop_ub; r++) {
        nvec_s_data[r] = nvec_data[r];
      }
      /*  N=ONER(N,r) */
      /*  Decrement element in position of r of input vector */
      /*  */
      /*  Copyright (c) 2012-2026, Imperial College London */
      /*  All rights reserved. */
      scalarLB = (int32_T)zerothinktimes_data[z];
      if (scalarLB != 0) {
        if ((scalarLB < 1) || (scalarLB > b_loop_ub)) {
          emlrtDynamicBoundsCheckR2012b(scalarLB, 1, b_loop_ub, &db_emlrtBCI,
                                        &st);
        }
        nvec_s_data[scalarLB - 1] = nvec_data[scalarLB - 1] - 1.0;
      }
      st.site = &r_emlrtRSI;
      b_st.site = &r_emlrtRSI;
      n = ((b_sum(&b_st, nvec_s) + m) + 1.0) - 1.0;
      /*  lf=FACTLN(n) */
      /*  Compure the logarithm of n!        */
      /*  */
      /*  Copyright (c) 2012-2026, Imperial College London */
      /*  All rights reserved.   */
      b_st.site = &ne_emlrtRSI;
      c_st.site = &oe_emlrtRSI;
      n++;
      d_st.site = &pe_emlrtRSI;
      scalar_gammaln(&d_st, &n);
      st.site = &r_emlrtRSI;
      /*  lf=FACTLN(n) */
      /*  Compure the logarithm of n!        */
      /*  */
      /*  Copyright (c) 2012-2026, Imperial College London */
      /*  All rights reserved.   */
      b_st.site = &ne_emlrtRSI;
      scalarLB = nvec_s->size[0] * nvec_s->size[1];
      nvec_s->size[0] = 1;
      emxEnsureCapacity_real_T(&b_st, nvec_s, scalarLB, &jb_emlrtRTEI);
      nvec_s_data = nvec_s->data;
      scalarLB = nvec_s->size[1] - 1;
      vectorUB = (nvec_s->size[1] / 2) << 1;
      sizes_idx_0 = vectorUB - 2;
      for (r = 0; r <= sizes_idx_0; r += 2) {
        d_r = _mm_loadu_pd(&nvec_s_data[r]);
        _mm_storeu_pd(&nvec_s_data[r], _mm_add_pd(d_r, _mm_set1_pd(1.0)));
      }
      for (r = vectorUB; r <= scalarLB; r++) {
        nvec_s_data[r]++;
      }
      c_st.site = &oe_emlrtRSI;
      applyScalarFunctionInPlace(&c_st, nvec_s);
      if (((int32_T)((uint32_T)z + 2U) < 1) ||
          ((int32_T)((uint32_T)z + 2U) > lh->size[0])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)z + 2U), 1,
                                      lh->size[0], &mb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      st.site = &r_emlrtRSI;
      lh_data[z + 1] = n - b_sum(&st, nvec_s);
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    st.site = &q_emlrtRSI;
    b_st.site = &q_emlrtRSI;
    n = (b_sum(&b_st, nvec) + m) - 1.0;
    /*  lf=FACTLN(n) */
    /*  Compure the logarithm of n!        */
    /*  */
    /*  Copyright (c) 2012-2026, Imperial College London */
    /*  All rights reserved.   */
    b_st.site = &ne_emlrtRSI;
    c_st.site = &oe_emlrtRSI;
    n++;
    d_st.site = &pe_emlrtRSI;
    scalar_gammaln(&d_st, &n);
    st.site = &q_emlrtRSI;
    /*  lf=FACTLN(n) */
    /*  Compure the logarithm of n!        */
    /*  */
    /*  Copyright (c) 2012-2026, Imperial College London */
    /*  All rights reserved.   */
    b_st.site = &ne_emlrtRSI;
    scalarLB = nvec_s->size[0] * nvec_s->size[1];
    nvec_s->size[0] = 1;
    nvec_s->size[1] = nvec->size[1];
    emxEnsureCapacity_real_T(&b_st, nvec_s, scalarLB, &jb_emlrtRTEI);
    nvec_s_data = nvec_s->data;
    scalarLB = loop_ub - 2;
    for (r = 0; r <= scalarLB; r += 2) {
      d_r = _mm_loadu_pd(&nvec_data[r]);
      _mm_storeu_pd(&nvec_s_data[r], _mm_add_pd(d_r, _mm_set1_pd(1.0)));
    }
    for (r = loop_ub; r < b_loop_ub; r++) {
      nvec_s_data[r] = nvec_data[r] + 1.0;
    }
    c_st.site = &oe_emlrtRSI;
    applyScalarFunctionInPlace(&c_st, nvec_s);
    if (((int32_T)(numZeroThinkTimes + 2U) < 1) ||
        ((int32_T)(numZeroThinkTimes + 2U) > lh->size[0])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)(numZeroThinkTimes + 2U), 1,
                                    lh->size[0], &lb_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    st.site = &q_emlrtRSI;
    lh_data[(int32_T)numZeroThinkTimes + 1] = n - b_sum(&st, nvec_s);
    for (z = 0; z < i; z++) {
      if (z + 1 > zerothinktimes->size[1]) {
        emlrtDynamicBoundsCheckR2012b(z + 1, 1, zerothinktimes->size[1],
                                      &l_emlrtBCI, (emlrtConstCTX)sp);
      }
      st.site = &p_emlrtRSI;
      scalarLB = nvec_s->size[0] * nvec_s->size[1];
      nvec_s->size[0] = 1;
      nvec_s->size[1] = b_loop_ub;
      emxEnsureCapacity_real_T(&st, nvec_s, scalarLB, &ub_emlrtRTEI);
      nvec_s_data = nvec_s->data;
      for (r = 0; r < b_loop_ub; r++) {
        nvec_s_data[r] = nvec_data[r];
      }
      /*  N=ONER(N,r) */
      /*  Decrement element in position of r of input vector */
      /*  */
      /*  Copyright (c) 2012-2026, Imperial College London */
      /*  All rights reserved. */
      scalarLB = (int32_T)zerothinktimes_data[z];
      if (scalarLB != 0) {
        if ((scalarLB < 1) || (scalarLB > b_loop_ub)) {
          emlrtDynamicBoundsCheckR2012b(scalarLB, 1, b_loop_ub, &db_emlrtBCI,
                                        &st);
        }
        nvec_s_data[scalarLB - 1] = nvec_data[scalarLB - 1] - 1.0;
      }
      st.site = &o_emlrtRSI;
      b_st.site = &o_emlrtRSI;
      n = (b_sum(&b_st, nvec_s) + m) - 1.0;
      /*  lf=FACTLN(n) */
      /*  Compure the logarithm of n!        */
      /*  */
      /*  Copyright (c) 2012-2026, Imperial College London */
      /*  All rights reserved.   */
      b_st.site = &ne_emlrtRSI;
      c_st.site = &oe_emlrtRSI;
      n++;
      d_st.site = &pe_emlrtRSI;
      scalar_gammaln(&d_st, &n);
      st.site = &o_emlrtRSI;
      /*  lf=FACTLN(n) */
      /*  Compure the logarithm of n!        */
      /*  */
      /*  Copyright (c) 2012-2026, Imperial College London */
      /*  All rights reserved.   */
      b_st.site = &ne_emlrtRSI;
      scalarLB = nvec_s->size[0] * nvec_s->size[1];
      nvec_s->size[0] = 1;
      emxEnsureCapacity_real_T(&b_st, nvec_s, scalarLB, &jb_emlrtRTEI);
      nvec_s_data = nvec_s->data;
      scalarLB = nvec_s->size[1] - 1;
      vectorUB = (nvec_s->size[1] / 2) << 1;
      sizes_idx_0 = vectorUB - 2;
      for (r = 0; r <= sizes_idx_0; r += 2) {
        d_r = _mm_loadu_pd(&nvec_s_data[r]);
        _mm_storeu_pd(&nvec_s_data[r], _mm_add_pd(d_r, _mm_set1_pd(1.0)));
      }
      for (r = vectorUB; r <= scalarLB; r++) {
        nvec_s_data[r]++;
      }
      c_st.site = &oe_emlrtRSI;
      applyScalarFunctionInPlace(&c_st, nvec_s);
      scalarLB = (int32_T)((numZeroThinkTimes + (uint32_T)z) + 3U);
      if ((scalarLB < 1) || (scalarLB > lh->size[0])) {
        emlrtDynamicBoundsCheckR2012b(scalarLB, 1, lh->size[0], &nb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      st.site = &o_emlrtRSI;
      lh_data[scalarLB - 1] = n - b_sum(&st, nvec_s);
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
  } else {
    scalarLB = lh->size[0];
    lh->size[0] = 2;
    emxEnsureCapacity_real_T(sp, lh, scalarLB, &bb_emlrtRTEI);
    lh_data = lh->data;
    lh_data[0] = 0.0;
    lh_data[1] = 0.0;
  }
  st.site = &n_emlrtRSI;
  b_exp(&st, lh);
  lh_data = lh->data;
  if ((int32_T)numZeroThinkTimes == L->size[1]) {
    vectorUB = lh->size[0];
    scalarLB = lGbasis->size[0];
    lGbasis->size[0] = lh->size[0];
    emxEnsureCapacity_real_T(sp, lGbasis, scalarLB, &db_emlrtRTEI);
    nvec_s_data = lGbasis->data;
    for (r = 0; r < vectorUB; r++) {
      nvec_s_data[r] = lh_data[r];
    }
    st.site = &m_emlrtRSI;
    c_log(&st, lGbasis);
    st.site = &l_emlrtRSI;
    scalarLB = lh->size[0] - L->size[1];
    if ((scalarLB < 1) || (scalarLB > lh->size[0])) {
      emlrtDynamicBoundsCheckR2012b(scalarLB, 1, lh->size[0], &s_emlrtBCI, &st);
    }
    n = lh_data[scalarLB - 1];
    if (n < 0.0) {
      emlrtErrorWithMessageIdR2018a(
          &st, &f_emlrtRTEI, "Coder:toolbox:ElFunDomainError",
          "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
    }
    n = muDoubleScalarLog(n);
    n += lG0;
  } else {
    real_T nt;
    int32_T i1;
    st.site = &k_emlrtRSI;
    n = b_sum(&st, b_N);
    if (!(n >= 0.0)) {
      emlrtNonNegativeCheckR2012b(n, &e_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (n != (int32_T)muDoubleScalarFloor(n)) {
      emlrtIntegerCheckR2012b(n, &d_emlrtDCI, (emlrtConstCTX)sp);
    }
    scalarLB = zerothinktimes->size[0] * zerothinktimes->size[1];
    zerothinktimes->size[0] = 1;
    vectorUB = (int32_T)n;
    zerothinktimes->size[1] = (int32_T)n;
    emxEnsureCapacity_real_T(sp, zerothinktimes, scalarLB, &eb_emlrtRTEI);
    zerothinktimes_data = zerothinktimes->data;
    for (r = 0; r < vectorUB; r++) {
      zerothinktimes_data[r] = 1.0;
    }
    st.site = &j_emlrtRSI;
    nt = b_sum(&st, nvec);
    emxInit_real_T(sp, &h_1, 1, &fb_emlrtRTEI);
    vectorUB = lh->size[0];
    scalarLB = h_1->size[0];
    h_1->size[0] = lh->size[0];
    emxEnsureCapacity_real_T(sp, h_1, scalarLB, &fb_emlrtRTEI);
    h_1_data = h_1->data;
    for (r = 0; r < vectorUB; r++) {
      h_1_data[r] = lh_data[r];
    }
    /* iterate */
    i1 = L->size[1] - (int32_T)numZeroThinkTimes;
    emlrtForLoopVectorCheckR2021a((real_T)numZeroThinkTimes + 1.0, 1.0,
                                  L->size[1], mxDOUBLE_CLASS, i1, &d_emlrtRTEI,
                                  (emlrtConstCTX)sp);
    emxInit_int8_T(sp, &F1r, &gb_emlrtRTEI);
    emxInit_real_T(sp, &F2r, 2, &ib_emlrtRTEI);
    emxInit_real_T(sp, &hr, 1, &mb_emlrtRTEI);
    emxInit_real_T(sp, &A12, 2, &ob_emlrtRTEI);
    emxInit_real_T(sp, &B2r, 2, &vb_emlrtRTEI);
    emxInit_real_T(sp, &iC, 2, &ac_emlrtRTEI);
    for (b_r = 0; b_r < i1; b_r++) {
      real_T d;
      int32_T c_loop_ub;
      int32_T i2;
      int32_T loop_ub_tmp;
      uint32_T c_r;
      c_r = (numZeroThinkTimes + (uint32_T)b_r) + 1U;
      d = 2.0 * (real_T)c_r;
      if (d != (int32_T)d) {
        emlrtIntegerCheckR2012b(d, &c_emlrtDCI, (emlrtConstCTX)sp);
      }
      loop_ub_tmp = (int32_T)d;
      scalarLB = F1r->size[0] * F1r->size[1];
      F1r->size[0] = (int32_T)d;
      F1r->size[1] = (int32_T)d;
      emxEnsureCapacity_int8_T(sp, F1r, scalarLB, &gb_emlrtRTEI);
      F1r_data = F1r->data;
      vectorUB = (int32_T)d * (int32_T)d;
      for (r = 0; r < vectorUB; r++) {
        F1r_data[r] = 0;
      }
      if (d != (int32_T)d) {
        emlrtIntegerCheckR2012b(d, &b_emlrtDCI, (emlrtConstCTX)sp);
      }
      scalarLB = F2r->size[0] * F2r->size[1];
      F2r->size[0] = (int32_T)d;
      F2r->size[1] = (int32_T)d;
      emxEnsureCapacity_real_T(sp, F2r, scalarLB, &ib_emlrtRTEI);
      F2r_data = F2r->data;
      for (r = 0; r < vectorUB; r++) {
        F2r_data[r] = 0.0;
      }
      i = b_N->size[1];
      c_loop_ub = (int32_T)c_r;
      if (((int32_T)c_r < 1) || ((int32_T)c_r > b_N->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)c_r, 1, b_N->size[1],
                                      &k_emlrtBCI, (emlrtConstCTX)sp);
      }
      n = b_N_data[(int32_T)c_r - 1];
      i2 = (int32_T)n;
      emlrtForLoopVectorCheckR2021a(1.0, 1.0, n, mxDOUBLE_CLASS, (int32_T)n,
                                    &c_emlrtRTEI, (emlrtConstCTX)sp);
      for (Nr = 0; Nr < i2; Nr++) {
        if (((int32_T)c_r < 1) || ((int32_T)c_r > nvec->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)c_r, 1, nvec->size[1],
                                        &u_emlrtBCI, (emlrtConstCTX)sp);
        }
        nvec_data[(int32_T)c_r - 1]++;
        if (Nr == 0) {
          uint32_T u;
          boolean_T empty_non_axis_sizes;
          if (c_r > numZeroThinkTimes + 1U) {
            if (d != (int32_T)d) {
              emlrtIntegerCheckR2012b(d, &g_emlrtDCI, (emlrtConstCTX)sp);
            }
            scalarLB = hr->size[0];
            hr->size[0] = (int32_T)d;
            emxEnsureCapacity_real_T(sp, hr, scalarLB, &mb_emlrtRTEI);
            nvec_s_data = hr->data;
            if (d != (int32_T)d) {
              emlrtIntegerCheckR2012b(d, &g_emlrtDCI, (emlrtConstCTX)sp);
            }
            for (r = 0; r < loop_ub_tmp; r++) {
              nvec_s_data[r] = 0.0;
            }
            if (lh->size[0] < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, lh->size[0], &j_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (((int32_T)((real_T)c_r - 1.0) < 1) ||
                ((int32_T)((real_T)c_r - 1.0) > lh->size[0])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)((real_T)c_r - 1.0), 1,
                                            lh->size[0], &i_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            scalarLB = (int32_T)c_r << 1;
            if (((int32_T)((real_T)c_r - 1.0) < 1) ||
                ((int32_T)((real_T)c_r - 1.0) > scalarLB)) {
              emlrtDynamicBoundsCheckR2012b((int32_T)((real_T)c_r - 1.0), 1,
                                            scalarLB, &f_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            for (r = 0; r <= c_loop_ub - 2; r++) {
              nvec_s_data[r] = lh_data[r];
            }
            u = (uint32_T)((int32_T)c_r - 1) << 1;
            if (c_r > u) {
              sizes_idx_0 = 1;
              vectorUB = 0;
            } else {
              if (((int32_T)c_r < 1) || ((int32_T)c_r > lh->size[0])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)c_r, 1, lh->size[0],
                                              &h_emlrtBCI, (emlrtConstCTX)sp);
              }
              sizes_idx_0 = (int32_T)c_r;
              if (((int32_T)u < 1) || ((int32_T)u > lh->size[0])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)u, 1, lh->size[0],
                                              &g_emlrtBCI, (emlrtConstCTX)sp);
              }
              vectorUB = (int32_T)u;
            }
            u = (c_r << 1) - 1U;
            if (c_r + 1U > u) {
              loop_ub = 0;
              scalarLB = 0;
            } else {
              if (((int32_T)(c_r + 1U) < 1) ||
                  ((int32_T)(c_r + 1U) > hr->size[0])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)(c_r + 1U), 1,
                                              hr->size[0], &e_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              loop_ub = (int32_T)c_r;
              if (((int32_T)u < 1) || ((int32_T)u > hr->size[0])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)u, 1, hr->size[0],
                                              &d_emlrtBCI, (emlrtConstCTX)sp);
              }
              scalarLB = (int32_T)u;
            }
            scalarLB -= loop_ub;
            vectorUB -= sizes_idx_0;
            if (scalarLB != vectorUB + 1) {
              emlrtSubAssignSizeCheck1dR2017a(scalarLB, vectorUB + 1,
                                              &c_emlrtECI, (emlrtConstCTX)sp);
            }
            for (r = 0; r <= vectorUB; r++) {
              nvec_s_data[loop_ub + r] = lh_data[(sizes_idx_0 + r) - 1];
            }
            vectorUB = hr->size[0];
            scalarLB = lh->size[0];
            lh->size[0] = hr->size[0];
            emxEnsureCapacity_real_T(sp, lh, scalarLB, &sb_emlrtRTEI);
            lh_data = lh->data;
            for (r = 0; r < vectorUB; r++) {
              lh_data[r] = nvec_s_data[r];
            }
            /*  update scalings */
            if (nt > 0.0) {
              if (h_1->size[0] < 1) {
                emlrtDynamicBoundsCheckR2012b(1, 1, h_1->size[0], &c_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (nt != (int32_T)muDoubleScalarFloor(nt)) {
                emlrtIntegerCheckR2012b(nt, &h_emlrtDCI, (emlrtConstCTX)sp);
              }
              if (((int32_T)nt < 1) ||
                  ((int32_T)nt > zerothinktimes->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)nt, 1,
                                              zerothinktimes->size[1],
                                              &eb_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (((int32_T)c_r < 1) || ((int32_T)c_r > hr->size[0])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)c_r, 1, hr->size[0],
                                              &fb_emlrtBCI, (emlrtConstCTX)sp);
              }
              lh_data[(int32_T)c_r - 1] =
                  h_1_data[0] / zerothinktimes_data[(int32_T)nt - 1];
              if (((int32_T)c_r < 1) || ((int32_T)c_r > h_1->size[0])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)c_r, 1, h_1->size[0],
                                              &gb_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (((int32_T)nt < 1) ||
                  ((int32_T)nt > zerothinktimes->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)nt, 1,
                                              zerothinktimes->size[1],
                                              &hb_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (lh->size[0] < 1) {
                emlrtDynamicBoundsCheckR2012b(lh->size[0], 1, lh->size[0],
                                              &ib_emlrtBCI, (emlrtConstCTX)sp);
              }
              lh_data[lh->size[0] - 1] = h_1_data[(int32_T)c_r - 1] /
                                         zerothinktimes_data[(int32_T)nt - 1];
            }
          }
          /*  CE for G+ */
          scalarLB = A12->size[0] * A12->size[1];
          A12->size[0] = (int32_T)c_r;
          A12->size[1] = (int32_T)c_r;
          emxEnsureCapacity_real_T(sp, A12, scalarLB, &ob_emlrtRTEI);
          nvec_s_data = A12->data;
          scalarLB = (int32_T)c_r * (int32_T)c_r;
          for (r = 0; r < scalarLB; r++) {
            nvec_s_data[r] = 0.0;
          }
          nvec_s_data[0] = -1.0;
          /*  Class-1..(R-1) PCs for G */
          for (r = 0; r <= c_loop_ub - 2; r++) {
            if ((r + 1 < 1) || (r + 1 > i)) {
              emlrtDynamicBoundsCheckR2012b(r + 1, 1, i, &w_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if ((r + 2 < 1) || (r + 2 > A12->size[0])) {
              emlrtDynamicBoundsCheckR2012b(r + 2, 1, A12->size[0], &x_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            nvec_s_data[r + 1] = b_N_data[r];
            if ((r + 1 < 1) || (r + 1 > b_Z->size[1])) {
              emlrtDynamicBoundsCheckR2012b(r + 1, 1, b_Z->size[1], &y_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if ((r + 2 < 1) || (r + 2 > A12->size[0])) {
              emlrtDynamicBoundsCheckR2012b(r + 2, 1, A12->size[0],
                                            &ab_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((r + 2 < 1) || (r + 2 > A12->size[1])) {
              emlrtDynamicBoundsCheckR2012b(r + 2, 1, A12->size[1],
                                            &bb_emlrtBCI, (emlrtConstCTX)sp);
            }
            nvec_s_data[(r + A12->size[0] * (r + 1)) + 1] = -b_Z_data[r];
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b((emlrtConstCTX)sp);
            }
          }
          /*  Class-R PCs */
          if (b_L->size[0] < 1) {
            emlrtDynamicBoundsCheckR2012b(1, 1, b_L->size[0], &b_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (((int32_T)c_r < 1) || ((int32_T)c_r > b_L->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)c_r, 1, b_L->size[1],
                                          &cb_emlrtBCI, (emlrtConstCTX)sp);
          }
          n = m * b_L_data[b_L->size[0] * ((int32_T)c_r - 1)];
          st.site = &i_emlrtRSI;
          eye(&st, c_r, iC);
          iC_data = iC->data;
          scalarLB = iC->size[0] * iC->size[1];
          vectorUB = (scalarLB / 2) << 1;
          sizes_idx_0 = vectorUB - 2;
          for (r = 0; r <= sizes_idx_0; r += 2) {
            d_r = _mm_loadu_pd(&iC_data[r]);
            _mm_storeu_pd(&iC_data[r], _mm_mul_pd(_mm_set1_pd(n), d_r));
          }
          for (r = vectorUB; r < scalarLB; r++) {
            iC_data[r] *= n;
          }
          if (((int32_T)c_r < 1) || ((int32_T)c_r > b_Z->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)c_r, 1, b_Z->size[1],
                                          &emlrtBCI, (emlrtConstCTX)sp);
          }
          st.site = &i_emlrtRSI;
          eye(&st, c_r, F2r);
          F2r_data = F2r->data;
          scalarLB = F2r->size[0] * F2r->size[1];
          n = b_Z_data[(int32_T)c_r - 1];
          vectorUB = (scalarLB / 2) << 1;
          sizes_idx_0 = vectorUB - 2;
          for (r = 0; r <= sizes_idx_0; r += 2) {
            d_r = _mm_loadu_pd(&F2r_data[r]);
            _mm_storeu_pd(&F2r_data[r], _mm_mul_pd(_mm_set1_pd(n), d_r));
          }
          for (r = vectorUB; r < scalarLB; r++) {
            F2r_data[r] *= n;
          }
          st.site = &i_emlrtRSI;
          b_st.site = &bf_emlrtRSI;
          c_st.site = &cf_emlrtRSI;
          if (F2r->size[0] != iC->size[0]) {
            emlrtErrorWithMessageIdR2018a(
                &c_st, &e_emlrtRTEI, "MATLAB:catenate:matrixDimensionMismatch",
                "MATLAB:catenate:matrixDimensionMismatch", 0);
          }
          loop_ub = iC->size[0];
          sizes_idx_0 = iC->size[0];
          vectorUB = F2r->size[1];
          scalarLB = B2r->size[0] * B2r->size[1];
          B2r->size[0] = iC->size[0];
          b_loop_ub = iC->size[1] + F2r->size[1];
          B2r->size[1] = b_loop_ub;
          emxEnsureCapacity_real_T(&b_st, B2r, scalarLB, &vb_emlrtRTEI);
          nvec_s_data = B2r->data;
          scalarLB = iC->size[1];
          for (r = 0; r < scalarLB; r++) {
            for (z = 0; z < loop_ub; z++) {
              nvec_s_data[z + B2r->size[0] * r] = iC_data[z + iC->size[0] * r];
            }
          }
          for (r = 0; r < vectorUB; r++) {
            for (z = 0; z < loop_ub; z++) {
              nvec_s_data[z + B2r->size[0] * (r + iC->size[1])] =
                  F2r_data[z + sizes_idx_0 * r];
            }
          }
          /*  explicit formula for inv(C) */
          st.site = &h_emlrtRSI;
          eye(&st, c_r, iC);
          iC_data = iC->data;
          scalarLB = iC->size[0] * iC->size[1];
          vectorUB = (scalarLB / 2) << 1;
          sizes_idx_0 = vectorUB - 2;
          for (r = 0; r <= sizes_idx_0; r += 2) {
            d_r = _mm_loadu_pd(&iC_data[r]);
            _mm_storeu_pd(
                &iC_data[r],
                _mm_div_pd(_mm_mul_pd(d_r, _mm_set1_pd(-1.0)), _mm_set1_pd(m)));
          }
          for (r = vectorUB; r < scalarLB; r++) {
            iC_data[r] = -iC_data[r] / m;
          }
          scalarLB = iC->size[1];
          for (r = 0; r < scalarLB; r++) {
            iC_data[iC->size[0] * r] = -1.0 / m;
          }
          sizes_idx_0 = iC->size[0] * iC->size[1];
          iC_data[0] = 1.0;
          /*  explicit formula for F1r */
          u = c_r << 1;
          if (d != (int32_T)d) {
            emlrtIntegerCheckR2012b(d, &emlrtDCI, (emlrtConstCTX)sp);
          }
          scalarLB = F1r->size[0] * F1r->size[1];
          F1r->size[0] = (int32_T)u;
          F1r->size[1] = (int32_T)u;
          emxEnsureCapacity_int8_T(sp, F1r, scalarLB, &xb_emlrtRTEI);
          F1r_data = F1r->data;
          scalarLB = (int32_T)u * (int32_T)u;
          for (r = 0; r < scalarLB; r++) {
            F1r_data[r] = 0;
          }
          F1r_data[0] = 1;
          /*  F2r by the definition */
          st.site = &g_emlrtRSI;
          scalarLB = (sizes_idx_0 / 2) << 1;
          vectorUB = scalarLB - 2;
          for (r = 0; r <= vectorUB; r += 2) {
            d_r = _mm_loadu_pd(&iC_data[r]);
            _mm_storeu_pd(&iC_data[r], _mm_mul_pd(d_r, _mm_set1_pd(-1.0)));
          }
          for (r = scalarLB; r < sizes_idx_0; r++) {
            iC_data[r] = -iC_data[r];
          }
          b_st.site = &wc_emlrtRSI;
          if (A12->size[0] != iC->size[1]) {
            if (((iC->size[0] == 1) && (iC->size[1] == 1)) ||
                ((A12->size[0] == 1) && (A12->size[1] == 1))) {
              emlrtErrorWithMessageIdR2018a(
                  &b_st, &emlrtRTEI,
                  "Coder:toolbox:mtimes_noDynamicScalarExpansion",
                  "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
            } else {
              emlrtErrorWithMessageIdR2018a(
                  &b_st, &b_emlrtRTEI, "MATLAB:innerdim", "MATLAB:innerdim", 0);
            }
          }
          b_st.site = &df_emlrtRSI;
          mtimes(&b_st, iC, A12, F2r);
          st.site = &g_emlrtRSI;
          b_st.site = &wc_emlrtRSI;
          if (B2r->size[0] != F2r->size[1]) {
            if (((F2r->size[0] == 1) && (F2r->size[1] == 1)) ||
                ((B2r->size[0] == 1) && (B2r->size[1] == 1))) {
              emlrtErrorWithMessageIdR2018a(
                  &b_st, &emlrtRTEI,
                  "Coder:toolbox:mtimes_noDynamicScalarExpansion",
                  "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
            } else {
              emlrtErrorWithMessageIdR2018a(
                  &b_st, &b_emlrtRTEI, "MATLAB:innerdim", "MATLAB:innerdim", 0);
            }
          }
          b_st.site = &df_emlrtRSI;
          mtimes(&b_st, F2r, B2r, iC);
          iC_data = iC->data;
          st.site = &g_emlrtRSI;
          b_st.site = &bf_emlrtRSI;
          if (iC->size[1] != 0) {
            b_loop_ub = iC->size[1];
          } else if (B2r->size[1] == 0) {
            b_loop_ub = 0;
          }
          c_st.site = &cf_emlrtRSI;
          if ((iC->size[1] != b_loop_ub) && (iC->size[1] != 0)) {
            emlrtErrorWithMessageIdR2018a(
                &c_st, &e_emlrtRTEI, "MATLAB:catenate:matrixDimensionMismatch",
                "MATLAB:catenate:matrixDimensionMismatch", 0);
          }
          if ((B2r->size[1] != b_loop_ub) && (B2r->size[1] != 0)) {
            emlrtErrorWithMessageIdR2018a(
                &c_st, &e_emlrtRTEI, "MATLAB:catenate:matrixDimensionMismatch",
                "MATLAB:catenate:matrixDimensionMismatch", 0);
          }
          empty_non_axis_sizes = (b_loop_ub == 0);
          if (empty_non_axis_sizes || (iC->size[1] != 0)) {
            vectorUB = iC->size[0];
          } else {
            vectorUB = 0;
          }
          if (empty_non_axis_sizes || (B2r->size[1] != 0)) {
            sizes_idx_0 = loop_ub;
          } else {
            sizes_idx_0 = 0;
          }
          scalarLB = F2r->size[0] * F2r->size[1];
          F2r->size[0] = vectorUB + sizes_idx_0;
          F2r->size[1] = b_loop_ub;
          emxEnsureCapacity_real_T(&b_st, F2r, scalarLB, &yb_emlrtRTEI);
          F2r_data = F2r->data;
          for (r = 0; r < b_loop_ub; r++) {
            for (z = 0; z < vectorUB; z++) {
              F2r_data[z + F2r->size[0] * r] = iC_data[z + vectorUB * r];
            }
          }
          for (r = 0; r < b_loop_ub; r++) {
            for (z = 0; z < sizes_idx_0; z++) {
              F2r_data[(z + vectorUB) + F2r->size[0] * r] =
                  nvec_s_data[z + sizes_idx_0 * r];
            }
          }
        }
        vectorUB = lh->size[0];
        scalarLB = h_1->size[0];
        h_1->size[0] = lh->size[0];
        emxEnsureCapacity_real_T(sp, h_1, scalarLB, &lb_emlrtRTEI);
        h_1_data = h_1->data;
        for (r = 0; r < vectorUB; r++) {
          h_1_data[r] = lh_data[r];
        }
        scalarLB = iC->size[0] * iC->size[1];
        iC->size[0] = F2r->size[0];
        iC->size[1] = F2r->size[1];
        emxEnsureCapacity_real_T(sp, iC, scalarLB, &pb_emlrtRTEI);
        iC_data = iC->data;
        if (((int32_T)c_r < 1) || ((int32_T)c_r > nvec->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)c_r, 1, nvec->size[1],
                                        &v_emlrtBCI, (emlrtConstCTX)sp);
        }
        n = nvec_data[(int32_T)c_r - 1];
        scalarLB = F2r->size[0] * F2r->size[1];
        vectorUB = (scalarLB / 2) << 1;
        sizes_idx_0 = vectorUB - 2;
        for (r = 0; r <= sizes_idx_0; r += 2) {
          d_r = _mm_loadu_pd(&F2r_data[r]);
          _mm_storeu_pd(&iC_data[r], _mm_div_pd(d_r, _mm_set1_pd(n)));
        }
        for (r = vectorUB; r < scalarLB; r++) {
          iC_data[r] = F2r_data[r] / n;
        }
        if ((F1r->size[0] != iC->size[0]) &&
            ((F1r->size[0] != 1) && (iC->size[0] != 1))) {
          emlrtDimSizeImpxCheckR2021b(F1r->size[0], iC->size[0], &b_emlrtECI,
                                      (emlrtConstCTX)sp);
        }
        if ((F1r->size[1] != iC->size[1]) &&
            ((F1r->size[1] != 1) && (iC->size[1] != 1))) {
          emlrtDimSizeImpxCheckR2021b(F1r->size[1], iC->size[1], &emlrtECI,
                                      (emlrtConstCTX)sp);
        }
        st.site = &f_emlrtRSI;
        if ((F1r->size[0] == iC->size[0]) && (F1r->size[1] == iC->size[1])) {
          vectorUB = F1r->size[0] * F1r->size[1];
          scalarLB = iC->size[0] * iC->size[1];
          iC->size[0] = F1r->size[0];
          iC->size[1] = F1r->size[1];
          emxEnsureCapacity_real_T(&st, iC, scalarLB, &qb_emlrtRTEI);
          iC_data = iC->data;
          for (r = 0; r < vectorUB; r++) {
            iC_data[r] += (real_T)F1r_data[r];
          }
        } else {
          b_st.site = &f_emlrtRSI;
          binary_expand_op(&b_st, iC, F1r);
        }
        b_st.site = &wc_emlrtRSI;
        if (lh->size[0] != iC->size[1]) {
          if (((iC->size[0] == 1) && (iC->size[1] == 1)) ||
              (lh->size[0] == 1)) {
            emlrtErrorWithMessageIdR2018a(
                &b_st, &emlrtRTEI,
                "Coder:toolbox:mtimes_noDynamicScalarExpansion",
                "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
          } else {
            emlrtErrorWithMessageIdR2018a(
                &b_st, &b_emlrtRTEI, "MATLAB:innerdim", "MATLAB:innerdim", 0);
          }
        }
        scalarLB = hr->size[0];
        hr->size[0] = lh->size[0];
        emxEnsureCapacity_real_T(&st, hr, scalarLB, &rb_emlrtRTEI);
        nvec_s_data = hr->data;
        scalarLB = lh->size[0] - 1;
        for (r = 0; r <= scalarLB; r++) {
          nvec_s_data[r] = lh_data[r];
        }
        b_st.site = &df_emlrtRSI;
        b_mtimes(&b_st, iC, hr, lh);
        lh_data = lh->data;
        st.site = &e_emlrtRSI;
        nt = b_sum(&st, nvec);
        st.site = &d_emlrtRSI;
        vectorUB = lh->size[0];
        scalarLB = hr->size[0];
        hr->size[0] = lh->size[0];
        emxEnsureCapacity_real_T(&st, hr, scalarLB, &tb_emlrtRTEI);
        nvec_s_data = hr->data;
        for (r = 0; r < vectorUB; r++) {
          nvec_s_data[r] = lh_data[r];
        }
        b_st.site = &gf_emlrtRSI;
        b_sort(&b_st, hr);
        st.site = &d_emlrtRSI;
        n = c_sum(&st, hr);
        if (nt != (int32_T)muDoubleScalarFloor(nt)) {
          emlrtIntegerCheckR2012b(nt, &i_emlrtDCI, (emlrtConstCTX)sp);
        }
        if (((int32_T)nt < 1) || ((int32_T)nt > zerothinktimes->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)nt, 1, zerothinktimes->size[1],
                                        &jb_emlrtBCI, (emlrtConstCTX)sp);
        }
        zerothinktimes_data[(int32_T)nt - 1] = muDoubleScalarAbs(n);
        st.site = &c_emlrtRSI;
        b_abs(&st, lh, hr);
        nvec_s_data = hr->data;
        if (((int32_T)nt < 1) || ((int32_T)nt > zerothinktimes->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)nt, 1, zerothinktimes->size[1],
                                        &kb_emlrtBCI, (emlrtConstCTX)sp);
        }
        n = zerothinktimes_data[(int32_T)nt - 1];
        sizes_idx_0 = hr->size[0];
        scalarLB = lh->size[0];
        lh->size[0] = hr->size[0];
        emxEnsureCapacity_real_T(sp, lh, scalarLB, &wb_emlrtRTEI);
        lh_data = lh->data;
        scalarLB = (hr->size[0] / 2) << 1;
        vectorUB = scalarLB - 2;
        for (r = 0; r <= vectorUB; r += 2) {
          d_r = _mm_loadu_pd(&nvec_s_data[r]);
          _mm_storeu_pd(&lh_data[r], _mm_div_pd(d_r, _mm_set1_pd(n)));
        }
        for (r = scalarLB; r < sizes_idx_0; r++) {
          lh_data[r] = nvec_s_data[r] / n;
        }
        /*  rescale so that |h|=1 */
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b((emlrtConstCTX)sp);
        }
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    emxFree_real_T(sp, &iC);
    emxFree_real_T(sp, &B2r);
    emxFree_real_T(sp, &A12);
    emxFree_real_T(sp, &hr);
    emxFree_real_T(sp, &F2r);
    emxFree_int8_T(sp, &F1r);
    emxFree_real_T(sp, &h_1);
    /*  unscale and return the log of the normalizing constant */
    st.site = &b_emlrtRSI;
    scalarLB = (lh->size[0] - L->size[1]) + 1;
    if ((scalarLB < 1) || (scalarLB > lh->size[0])) {
      emlrtDynamicBoundsCheckR2012b(scalarLB, 1, lh->size[0], &t_emlrtBCI, &st);
    }
    n = lh_data[scalarLB - 1];
    if (n < 0.0) {
      emlrtErrorWithMessageIdR2018a(
          &st, &f_emlrtRTEI, "Coder:toolbox:ElFunDomainError",
          "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
    }
    n = muDoubleScalarLog(n);
    scalarLB = nvec_s->size[0] * nvec_s->size[1];
    nvec_s->size[0] = 1;
    vectorUB = zerothinktimes->size[1];
    nvec_s->size[1] = zerothinktimes->size[1];
    emxEnsureCapacity_real_T(sp, nvec_s, scalarLB, &hb_emlrtRTEI);
    nvec_s_data = nvec_s->data;
    for (r = 0; r < vectorUB; r++) {
      nvec_s_data[r] = zerothinktimes_data[r];
    }
    st.site = &b_emlrtRSI;
    b_log(&st, nvec_s);
    st.site = &b_emlrtRSI;
    n = (lG0 + n) + b_sum(&st, nvec_s);
    st.site = &emlrtRSI;
    c_log(&st, lh);
    lh_data = lh->data;
    st.site = &emlrtRSI;
    b_log(&st, zerothinktimes);
    st.site = &emlrtRSI;
    nt = b_sum(&st, zerothinktimes);
    sizes_idx_0 = lh->size[0];
    scalarLB = lGbasis->size[0];
    lGbasis->size[0] = lh->size[0];
    emxEnsureCapacity_real_T(sp, lGbasis, scalarLB, &kb_emlrtRTEI);
    nvec_s_data = lGbasis->data;
    scalarLB = (lh->size[0] / 2) << 1;
    vectorUB = scalarLB - 2;
    for (r = 0; r <= vectorUB; r += 2) {
      d_r = _mm_loadu_pd(&lh_data[r]);
      _mm_storeu_pd(&nvec_s_data[r], _mm_add_pd(d_r, _mm_set1_pd(nt)));
    }
    for (r = scalarLB; r < sizes_idx_0; r++) {
      nvec_s_data[r] = lh_data[r] + nt;
    }
  }
  emxFree_real_T(sp, &b_Z);
  emxFree_real_T(sp, &b_N);
  emxFree_real_T(sp, &b_L);
  emxFree_real_T(sp, &nvec_s);
  emxFree_real_T(sp, &lh);
  emxFree_real_T(sp, &nvec);
  emxFree_real_T(sp, &zerothinktimes);
  *lG = n;
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (pfqn_comomrm.c) */
