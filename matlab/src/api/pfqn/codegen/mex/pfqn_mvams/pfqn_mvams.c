/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mvams.c
 *
 * Code generation for function 'pfqn_mvams'
 *
 */

/* Include files */
#include "pfqn_mvams.h"
#include "assertCompatibleDims.h"
#include "eml_int_forloop_overflow_check.h"
#include "eml_setop.h"
#include "error.h"
#include "find.h"
#include "ifWhileCond.h"
#include "ixfun.h"
#include "pfqn_mva.h"
#include "pfqn_mvald.h"
#include "pfqn_mvaldmx.h"
#include "pfqn_mvams_data.h"
#include "pfqn_mvams_emxutil.h"
#include "pfqn_mvams_types.h"
#include "pfqn_mvamx.h"
#include "repmat.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>

/* Variable Definitions */
static emlrtRSInfo emlrtRSI =
    {
        54,           /* lineNo */
        "pfqn_mvams", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pathName */
};

static emlrtRSInfo b_emlrtRSI =
    {
        66,           /* lineNo */
        "pfqn_mvams", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pathName */
};

static emlrtRSInfo c_emlrtRSI =
    {
        68,           /* lineNo */
        "pfqn_mvams", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pathName */
};

static emlrtRSInfo d_emlrtRSI =
    {
        72,           /* lineNo */
        "pfqn_mvams", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pathName */
};

static emlrtRSInfo e_emlrtRSI =
    {
        74,           /* lineNo */
        "pfqn_mvams", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pathName */
};

static emlrtRSInfo f_emlrtRSI =
    {
        76,           /* lineNo */
        "pfqn_mvams", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pathName */
};

static emlrtRSInfo g_emlrtRSI =
    {
        79,           /* lineNo */
        "pfqn_mvams", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pathName */
};

static emlrtRSInfo h_emlrtRSI = {
    15,    /* lineNo */
    "min", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/min.m" /* pathName
                                                                     */
};

static emlrtRSInfo i_emlrtRSI = {
    60,         /* lineNo */
    "minOrMax", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

static emlrtRSInfo j_emlrtRSI = {
    96,         /* lineNo */
    "minimum2", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

static emlrtRSInfo k_emlrtRSI =
    {
        139,              /* lineNo */
        "binaryMinOrMax", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "binaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo l_emlrtRSI = {
    65,      /* lineNo */
    "ixfun", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/ixfun.m" /* pathName
                                                                         */
};

static emlrtRSInfo m_emlrtRSI = {
    110,                          /* lineNo */
    "binaryImplicitExpansionFun", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/ixfun.m" /* pathName
                                                                         */
};

static emlrtRSInfo n_emlrtRSI =
    {
        34,           /* lineNo */
        "pfqn_mvamx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pathName */
};

static emlrtRSInfo o_emlrtRSI =
    {
        68,           /* lineNo */
        "pfqn_mvamx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pathName */
};

static emlrtRSInfo p_emlrtRSI =
    {
        75,           /* lineNo */
        "pfqn_mvamx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pathName */
};

static emlrtRSInfo q_emlrtRSI =
    {
        76,           /* lineNo */
        "pfqn_mvamx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pathName */
};

static emlrtRSInfo r_emlrtRSI =
    {
        93,           /* lineNo */
        "pfqn_mvamx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pathName */
};

static emlrtRSInfo pc_emlrtRSI =
    {
        290,             /* lineNo */
        "unaryMinOrMax", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo qc_emlrtRSI =
    {
        383,                     /* lineNo */
        "unaryMinOrMaxDispatch", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo rc_emlrtRSI =
    {
        451,          /* lineNo */
        "minOrMax2D", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo sc_emlrtRSI =
    {
        533,                         /* lineNo */
        "minOrMax2DColumnMajorDim1", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo tc_emlrtRSI =
    {
        531,                         /* lineNo */
        "minOrMax2DColumnMajorDim1", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo wc_emlrtRSI =
    {
        29,             /* lineNo */
        "pfqn_mvaldms", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldms.m" /* pathName */
};

static emlrtRSInfo xc_emlrtRSI =
    {
        37,             /* lineNo */
        "pfqn_mvaldms", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldms.m" /* pathName */
};

static emlrtRSInfo yc_emlrtRSI =
    {
        39,             /* lineNo */
        "pfqn_mvaldms", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldms.m" /* pathName */
};

static emlrtRSInfo ad_emlrtRSI =
    {
        40,             /* lineNo */
        "pfqn_mvaldms", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldms.m" /* pathName */
};

static emlrtECInfo emlrtECI =
    {
        -1,             /* nDims */
        32,             /* lineNo */
        5,              /* colNo */
        "pfqn_mvaldms", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldms.m" /* pName */
};

static emlrtBCInfo emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    32,             /* lineNo */
    8,              /* colNo */
    "mu",           /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo b_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    32,             /* lineNo */
    29,             /* colNo */
    "S",            /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtECInfo b_emlrtECI =
    {
        -1,           /* nDims */
        78,           /* lineNo */
        5,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtECInfo c_emlrtECI =
    {
        -1,           /* nDims */
        79,           /* lineNo */
        5,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtBCInfo c_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        93,           /* lineNo */
        47,           /* colNo */
        "QNc",        /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo d_emlrtBCI =
    {
        -1,            /* iFirst */
        -1,            /* iLast */
        89,            /* lineNo */
        25,            /* colNo */
        "openClasses", /* aName */
        "pfqn_mvamx",  /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo d_emlrtECI =
    {
        -1,           /* nDims */
        77,           /* lineNo */
        5,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtBCInfo e_emlrtBCI =
    {
        -1,              /* iFirst */
        -1,              /* iLast */
        83,              /* lineNo */
        27,              /* colNo */
        "closedClasses", /* aName */
        "pfqn_mvamx",    /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo f_emlrtBCI =
    {
        -1,              /* iFirst */
        -1,              /* iLast */
        52,              /* lineNo */
        33,              /* colNo */
        "closedClasses", /* aName */
        "pfqn_mvamx",    /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo g_emlrtBCI =
    {
        -1,              /* iFirst */
        -1,              /* iLast */
        52,              /* lineNo */
        31,              /* colNo */
        "closedClasses", /* aName */
        "pfqn_mvamx",    /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo h_emlrtBCI =
    {
        -1,            /* iFirst */
        -1,            /* iLast */
        51,            /* lineNo */
        29,            /* colNo */
        "openClasses", /* aName */
        "pfqn_mvamx",  /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo i_emlrtBCI =
    {
        -1,            /* iFirst */
        -1,            /* iLast */
        51,            /* lineNo */
        27,            /* colNo */
        "openClasses", /* aName */
        "pfqn_mvamx",  /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo j_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        43,           /* lineNo */
        16,           /* colNo */
        "N",          /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo k_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        33,           /* lineNo */
        36,           /* colNo */
        "N",          /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtRTEInfo b_emlrtRTEI =
    {
        90,              /* lineNo */
        27,              /* colNo */
        "unaryMinOrMax", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pName */
};

static emlrtECInfo e_emlrtECI =
    {
        -1,           /* nDims */
        54,           /* lineNo */
        5,            /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pName */
};

static emlrtBCInfo l_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        54,           /* lineNo */
        8,            /* colNo */
        "mu",         /* aName */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo m_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        58,           /* lineNo */
        19,           /* colNo */
        "S",          /* aName */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo emlrtDCI =
    {
        54,           /* lineNo */
        42,           /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo n_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        54,           /* lineNo */
        30,           /* colNo */
        "S",          /* aName */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo o_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        34,           /* lineNo */
        16,           /* colNo */
        "N",          /* aName */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo b_emlrtDCI =
    {
        40,           /* lineNo */
        13,           /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m", /* pName */
        1               /* checkKind */
};

static emlrtDCInfo c_emlrtDCI =
    {
        40,           /* lineNo */
        13,           /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m", /* pName */
        4               /* checkKind */
};

static emlrtBCInfo p_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        37,           /* lineNo */
        25,           /* colNo */
        "N",          /* aName */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo q_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        58,           /* lineNo */
        30,           /* colNo */
        "S",          /* aName */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo r_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        80,           /* lineNo */
        15,           /* colNo */
        "lG",         /* aName */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo s_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        33,           /* lineNo */
        15,           /* colNo */
        "lambda",     /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo t_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        33,           /* lineNo */
        45,           /* colNo */
        "N",          /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo u_emlrtBCI =
    {
        -1,              /* iFirst */
        -1,              /* iLast */
        48,              /* lineNo */
        23,              /* colNo */
        "closedClasses", /* aName */
        "pfqn_mvamx",    /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo v_emlrtBCI =
    {
        -1,            /* iFirst */
        -1,            /* iLast */
        45,            /* lineNo */
        21,            /* colNo */
        "openClasses", /* aName */
        "pfqn_mvamx",  /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo w_emlrtBCI =
    {
        -1,            /* iFirst */
        -1,            /* iLast */
        61,            /* lineNo */
        21,            /* colNo */
        "openClasses", /* aName */
        "pfqn_mvamx",  /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo x_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        65,           /* lineNo */
        20,           /* colNo */
        "lambda",     /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo y_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        65,           /* lineNo */
        8,            /* colNo */
        "XN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ab_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        28,           /* colNo */
        "lambda",     /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo bb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        33,           /* colNo */
        "D",          /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo cb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        37,           /* colNo */
        "D",          /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo db_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        12,           /* colNo */
        "UN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo eb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        16,           /* colNo */
        "UN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo fb_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    29,             /* lineNo */
    13,             /* colNo */
    "N",            /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo gb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        75,           /* lineNo */
        14,           /* colNo */
        "D",          /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo d_emlrtDCI = {
    30,             /* lineNo */
    13,             /* colNo */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    1                 /* checkKind */
};

static emlrtDCInfo e_emlrtDCI = {
    30,             /* lineNo */
    13,             /* colNo */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    4                 /* checkKind */
};

static emlrtBCInfo hb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        84,           /* lineNo */
        24,           /* colNo */
        "XN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ib_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        84,           /* lineNo */
        29,           /* colNo */
        "D",          /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo jb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        84,           /* lineNo */
        33,           /* colNo */
        "D",          /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo kb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        84,           /* lineNo */
        12,           /* colNo */
        "UN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo lb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        84,           /* lineNo */
        16,           /* colNo */
        "UN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo mb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        91,           /* lineNo */
        27,           /* colNo */
        "D",          /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo nb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        91,           /* lineNo */
        31,           /* colNo */
        "D",          /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ob_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        91,           /* lineNo */
        43,           /* colNo */
        "UNt",        /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo pb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        91,           /* lineNo */
        16,           /* colNo */
        "CN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo qb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        91,           /* lineNo */
        20,           /* colNo */
        "CN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo rb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        93,           /* lineNo */
        27,           /* colNo */
        "D",          /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo sb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        93,           /* lineNo */
        31,           /* colNo */
        "D",          /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo tb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        93,           /* lineNo */
        65,           /* colNo */
        "UNt",        /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ub_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        93,           /* lineNo */
        16,           /* colNo */
        "CN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo vb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        93,           /* lineNo */
        20,           /* colNo */
        "CN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo wb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        95,           /* lineNo */
        24,           /* colNo */
        "CN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo xb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        95,           /* lineNo */
        28,           /* colNo */
        "CN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo yb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        95,           /* lineNo */
        36,           /* colNo */
        "XN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ac_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        95,           /* lineNo */
        12,           /* colNo */
        "QN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo bc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        95,           /* lineNo */
        16,           /* colNo */
        "QN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo cc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        77,           /* lineNo */
        8,            /* colNo */
        "XN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ec_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        10,           /* colNo */
        "QN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo gc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        79,           /* lineNo */
        10,           /* colNo */
        "CN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo hc_emlrtBCI = {
    -1,              /* iFirst */
    -1,              /* iLast */
    43,              /* lineNo */
    23,              /* colNo */
    "closedClasses", /* aName */
    "pfqn_mvaldms",  /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ic_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    50,             /* lineNo */
    21,             /* colNo */
    "openClasses",  /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo jc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    45,             /* lineNo */
    24,             /* colNo */
    "XN",           /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo kc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    45,             /* lineNo */
    31,             /* colNo */
    "D",            /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo lc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    45,             /* lineNo */
    35,             /* colNo */
    "D",            /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo mc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    45,             /* lineNo */
    40,             /* colNo */
    "S",            /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo nc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    45,             /* lineNo */
    12,             /* colNo */
    "UN",           /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo oc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    45,             /* lineNo */
    16,             /* colNo */
    "UN",           /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo pc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    52,             /* lineNo */
    28,             /* colNo */
    "lambda",       /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo qc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    52,             /* lineNo */
    35,             /* colNo */
    "D",            /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo rc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    52,             /* lineNo */
    39,             /* colNo */
    "D",            /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo sc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    52,             /* lineNo */
    44,             /* colNo */
    "S",            /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo tc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    52,             /* lineNo */
    12,             /* colNo */
    "UN",           /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo uc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    52,             /* lineNo */
    16,             /* colNo */
    "UN",           /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo vc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    52,             /* lineNo */
    21,             /* colNo */
    "lambda",       /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo wc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    52,             /* lineNo */
    21,             /* colNo */
    "D",            /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo xc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    52,             /* lineNo */
    21,             /* colNo */
    "S",            /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo yc_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    52,             /* lineNo */
    9,              /* colNo */
    "UN",           /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ad_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    45,             /* lineNo */
    21,             /* colNo */
    "XN",           /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo bd_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    45,             /* lineNo */
    21,             /* colNo */
    "D",            /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo cd_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    45,             /* lineNo */
    21,             /* colNo */
    "S",            /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo dd_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    45,             /* lineNo */
    9,              /* colNo */
    "UN",           /* aName */
    "pfqn_mvaldms", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldms.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ed_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        21,           /* colNo */
        "lambda",     /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo fd_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        21,           /* colNo */
        "D",          /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo gd_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        9,            /* colNo */
        "UN",         /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtRTEInfo eb_emlrtRTEI =
    {
        40,           /* lineNo */
        6,            /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pName */
};

static emlrtRTEInfo fb_emlrtRTEI =
    {
        48,           /* lineNo */
        5,            /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pName */
};

static emlrtRTEInfo hb_emlrtRTEI =
    {
        68,           /* lineNo */
        39,           /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pName */
};

static emlrtRTEInfo ib_emlrtRTEI =
    {
        68,           /* lineNo */
        41,           /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pName */
};

static emlrtRTEInfo jb_emlrtRTEI =
    {
        54,           /* lineNo */
        17,           /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pName */
};

static emlrtRTEInfo kb_emlrtRTEI =
    {
        54,           /* lineNo */
        35,           /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pName */
};

static emlrtRTEInfo lb_emlrtRTEI =
    {
        523,             /* lineNo */
        21,              /* colNo */
        "unaryMinOrMax", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pName */
};

static emlrtRTEInfo mb_emlrtRTEI =
    {
        38,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo nb_emlrtRTEI =
    {
        39,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo ob_emlrtRTEI =
    {
        72,           /* lineNo */
        12,           /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pName */
};

static emlrtRTEInfo pb_emlrtRTEI =
    {
        51,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo qb_emlrtRTEI =
    {
        52,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo rb_emlrtRTEI =
    {
        54,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo sb_emlrtRTEI =
    {
        55,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo tb_emlrtRTEI =
    {
        56,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo ub_emlrtRTEI =
    {
        57,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo wb_emlrtRTEI =
    {
        71,           /* lineNo */
        5,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo xb_emlrtRTEI =
    {
        75,           /* lineNo */
        10,           /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo yb_emlrtRTEI =
    {
        29,             /* lineNo */
        13,             /* colNo */
        "pfqn_mvaldms", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldms.m" /* pName */
};

static emlrtRTEInfo ac_emlrtRTEI =
    {
        66,           /* lineNo */
        10,           /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pName */
};

static emlrtRTEInfo bc_emlrtRTEI =
    {
        29,             /* lineNo */
        11,             /* colNo */
        "pfqn_mvaldms", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldms.m" /* pName */
};

static emlrtRTEInfo cc_emlrtRTEI =
    {
        93,           /* lineNo */
        43,           /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo dc_emlrtRTEI =
    {
        74,           /* lineNo */
        13,           /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pName */
};

static emlrtRTEInfo fc_emlrtRTEI =
    {
        77,           /* lineNo */
        8,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo gc_emlrtRTEI =
    {
        35,             /* lineNo */
        5,              /* colNo */
        "pfqn_mvaldms", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldms.m" /* pName */
};

static emlrtRTEInfo ic_emlrtRTEI =
    {
        32,             /* lineNo */
        17,             /* colNo */
        "pfqn_mvaldms", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldms.m" /* pName */
};

static emlrtRTEInfo jc_emlrtRTEI =
    {
        39,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldms", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldms.m" /* pName */
};

static emlrtRTEInfo lc_emlrtRTEI =
    {
        78,           /* lineNo */
        10,           /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo mc_emlrtRTEI =
    {
        41,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldms", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldms.m" /* pName */
};

static emlrtRTEInfo nc_emlrtRTEI =
    {
        79,           /* lineNo */
        10,           /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo oc_emlrtRTEI =
    {
        74,           /* lineNo */
        14,           /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pName */
};

static emlrtRTEInfo pc_emlrtRTEI =
    {
        40,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pName */
};

static emlrtRTEInfo qc_emlrtRTEI =
    {
        54,           /* lineNo */
        21,           /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pName */
};

static emlrtRTEInfo rc_emlrtRTEI =
    {
        68,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo sc_emlrtRTEI =
    {
        25,           /* lineNo */
        27,           /* colNo */
        "pfqn_mvams", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvams.m" /* pName */
};

static emlrtRTEInfo tc_emlrtRTEI =
    {
        75,           /* lineNo */
        33,           /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo uc_emlrtRTEI =
    {
        30,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldms", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldms.m" /* pName */
};

static emlrtRTEInfo vc_emlrtRTEI =
    {
        40,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldms", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldms.m" /* pName */
};

static emlrtRTEInfo xc_emlrtRTEI =
    {
        78,           /* lineNo */
        5,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

/* Function Definitions */
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

void pfqn_mvams(const emlrtStack *sp, const emxArray_real_T *lambda,
                const emxArray_real_T *L, const emxArray_real_T *N,
                emxArray_real_T *Z, const emxArray_real_T *mi,
                const emxArray_real_T *S, emxArray_real_T *XN,
                emxArray_real_T *QN, emxArray_real_T *UN, emxArray_real_T *CN,
                real_T *lG)
{
  __m128d b_r;
  __m128d r1;
  jmp_buf emlrtJBEnviron;
  jmp_buf *volatile emlrtJBStack;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack g_st;
  emlrtStack h_st;
  emlrtStack i_st;
  emlrtStack j_st;
  emlrtStack k_st;
  emlrtStack l_st;
  emlrtStack st;
  emxArray_boolean_T *d_lG;
  emxArray_int32_T *ia;
  emxArray_int32_T *ii;
  emxArray_real_T *CNc;
  emxArray_real_T *UNt;
  emxArray_real_T *a__1;
  emxArray_real_T *b_S;
  emxArray_real_T *b_lG;
  emxArray_real_T *b_mu;
  emxArray_real_T *b_y;
  emxArray_real_T *c_mu;
  emxArray_real_T *closedClasses;
  emxArray_real_T *mu;
  emxArray_real_T *openClasses;
  emxArray_real_T *y;
  real_T dv[2];
  const real_T *L_data;
  const real_T *N_data;
  const real_T *S_data;
  const real_T *lambda_data;
  const real_T *mi_data;
  real_T Ntot;
  real_T b;
  real_T c_lG;
  real_T varargin_1;
  real_T varargin_2;
  real_T *CN_data;
  real_T *QN_data;
  real_T *UN_data;
  real_T *UNt_data;
  real_T *XN_data;
  real_T *Z_data;
  real_T *b_S_data;
  real_T *closedClasses_data;
  real_T *lG_data;
  real_T *mu_data;
  real_T *openClasses_data;
  int32_T b_XN[2];
  int32_T XN_tmp;
  int32_T b_loop_ub;
  int32_T c_loop_ub;
  int32_T d_loop_ub;
  int32_T e_loop_ub;
  int32_T i;
  int32_T i1;
  int32_T i2;
  int32_T i3;
  int32_T i4;
  int32_T i5;
  int32_T i6;
  int32_T i7;
  int32_T i8;
  int32_T i9;
  int32_T ist;
  int32_T loop_ub;
  int32_T m;
  int32_T n;
  int32_T pfqn_mvams_numThreads;
  int32_T r;
  int32_T ridx;
  int32_T *ii_data;
  uint32_T nClosed;
  uint32_T nOpen;
  boolean_T emlrtHadParallelError = false;
  boolean_T exitg1;
  boolean_T hasMultiServer;
  boolean_T hasOpenClasses;
  boolean_T *b_lG_data;
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
  i_st.prev = &h_st;
  i_st.tls = h_st.tls;
  S_data = S->data;
  mi_data = mi->data;
  Z_data = Z->data;
  N_data = N->data;
  L_data = L->data;
  lambda_data = lambda->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /* { */
  /*  % @file pfqn_mvams.m */
  /*  % @brief General-purpose MVA for mixed networks with multiserver nodes. */
  /* } */
  /* { */
  /*  % @brief General-purpose MVA for mixed networks with multiserver nodes. */
  /*  % @fn pfqn_mvams(lambda, L, N, Z, mi, S) */
  /*  % @param lambda Arrival rate vector. */
  /*  % @param L Service demand matrix. */
  /*  % @param N Population vector. */
  /*  % @param Z Think time vector. */
  /*  % @param mi Queue replication factors (default: ones). */
  /*  % @param S Number of servers per station (default: ones). */
  /*  % @return XN System throughput. */
  /*  % @return QN Mean queue lengths. */
  /*  % @return UN Utilization. */
  /*  % @return CN Cycle times. */
  /*  % @return lG Logarithm of normalizing constant. */
  /* } */
  /*  [XN,QN,UN,CN,LOGG]=PFQN_MVAMS(LAMBDA,L,N,Z,MI,S) */
  /*  this is a general purpose script to handle mixed qns with multi-server
   * nodes */
  /*  S(i) number of servers in station i */
  /*  get number of queues (M) and classes (R) */
  Ntot = 0.0;
  hasOpenClasses = false;
  loop_ub = L->size[1];
  for (r = 0; r < loop_ub; r++) {
    if (r + 1 > N->size[1]) {
      emlrtDynamicBoundsCheckR2012b(r + 1, 1, N->size[1], &o_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (muDoubleScalarIsInf(N_data[r])) {
      hasOpenClasses = true;
    } else {
      if (r + 1 > N->size[1]) {
        emlrtDynamicBoundsCheckR2012b(r + 1, 1, N->size[1], &p_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      Ntot += N_data[r];
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  b_loop_ub = L->size[0];
  emxInit_real_T(sp, &mu, 2, &pc_emlrtRTEI);
  n = mu->size[0] * mu->size[1];
  mu->size[0] = L->size[0];
  emxEnsureCapacity_real_T(sp, mu, n, &eb_emlrtRTEI);
  if (!(Ntot >= 0.0)) {
    emlrtNonNegativeCheckR2012b(Ntot, &c_emlrtDCI, (emlrtConstCTX)sp);
  }
  b = (int32_T)muDoubleScalarFloor(Ntot);
  if (Ntot != b) {
    emlrtIntegerCheckR2012b(Ntot, &b_emlrtDCI, (emlrtConstCTX)sp);
  }
  n = mu->size[0] * mu->size[1];
  c_loop_ub = (int32_T)Ntot;
  mu->size[1] = (int32_T)Ntot;
  emxEnsureCapacity_real_T(sp, mu, n, &eb_emlrtRTEI);
  mu_data = mu->data;
  if (Z->size[1] == 0) {
    n = Z->size[0] * Z->size[1];
    Z->size[0] = 1;
    Z->size[1] = L->size[1];
    emxEnsureCapacity_real_T(sp, Z, n, &fb_emlrtRTEI);
    Z_data = Z->data;
    for (r = 0; r < loop_ub; r++) {
      Z_data[r] = 0.0;
    }
  }
  emxInit_real_T(sp, &y, 2, &qc_emlrtRTEI);
  XN_data = y->data;
  if (L->size[0] - 1 >= 0) {
    if (Ntot < 1.0) {
      y->size[0] = 1;
      y->size[1] = 0;
    } else {
      XN_tmp = y->size[0] * y->size[1];
      y->size[0] = 1;
      y->size[1] = (int32_T)(Ntot - 1.0) + 1;
      emxEnsureCapacity_real_T(sp, y, XN_tmp, &gb_emlrtRTEI);
      XN_data = y->data;
      XN_tmp = (int32_T)(Ntot - 1.0);
      n = (((int32_T)(Ntot - 1.0) + 1) / 2) << 1;
      m = n - 2;
      for (r = 0; r <= m; r += 2) {
        dv[0] = r;
        dv[1] = r + 1;
        b_r = _mm_loadu_pd(&dv[0]);
        _mm_storeu_pd(&XN_data[r], _mm_add_pd(_mm_set1_pd(1.0), b_r));
      }
      for (r = n; r <= XN_tmp; r++) {
        XN_data[r] = (real_T)r + 1.0;
      }
    }
  }
  emxInit_real_T(sp, &b_lG, 2, &ob_emlrtRTEI);
  emxInit_real_T(sp, &b_S, 2, &kb_emlrtRTEI);
  for (ist = 0; ist < b_loop_ub; ist++) {
    if (ist + 1 > mu->size[0]) {
      emlrtDynamicBoundsCheckR2012b(ist + 1, 1, mu->size[0], &l_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    XN_tmp = S->size[0] * S->size[1];
    if (ist + 1 > XN_tmp) {
      emlrtDynamicBoundsCheckR2012b(ist + 1, 1, XN_tmp, &n_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (Ntot != b) {
      emlrtIntegerCheckR2012b(Ntot, &emlrtDCI, (emlrtConstCTX)sp);
    }
    st.site = &emlrtRSI;
    b_st.site = &h_emlrtRSI;
    c_st.site = &i_emlrtRSI;
    d_st.site = &j_emlrtRSI;
    e_st.site = &k_emlrtRSI;
    f_st.site = &l_emlrtRSI;
    n = y->size[1];
    if (y->size[1] == (int32_T)Ntot) {
      XN_tmp = b_lG->size[0] * b_lG->size[1];
      b_lG->size[0] = 1;
      b_lG->size[1] = y->size[1];
      emxEnsureCapacity_real_T(&f_st, b_lG, XN_tmp, &jb_emlrtRTEI);
      lG_data = b_lG->data;
      for (r = 0; r < n; r++) {
        varargin_1 = XN_data[r];
        varargin_2 = S_data[ist];
        lG_data[r] = muDoubleScalarMin(varargin_1, varargin_2);
      }
    } else {
      XN_tmp = b_S->size[0] * b_S->size[1];
      b_S->size[0] = 1;
      b_S->size[1] = (int32_T)Ntot;
      emxEnsureCapacity_real_T(&f_st, b_S, XN_tmp, &kb_emlrtRTEI);
      b_S_data = b_S->data;
      for (r = 0; r < c_loop_ub; r++) {
        b_S_data[r] = S_data[ist];
      }
      g_st.site = &m_emlrtRSI;
      expand_min(&g_st, y, b_S, b_lG);
      lG_data = b_lG->data;
    }
    b_XN[0] = 1;
    XN_tmp = mu->size[1];
    b_XN[1] = mu->size[1];
    emlrtSubAssignSizeCheckR2012b(&b_XN[0], 2, &b_lG->size[0], 2, &e_emlrtECI,
                                  (emlrtCTX)sp);
    for (r = 0; r < XN_tmp; r++) {
      mu_data[ist + mu->size[0] * r] = lG_data[r];
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  hasMultiServer = false;
  n = 0;
  exitg1 = false;
  while ((!exitg1) && (n <= L->size[0] - 1)) {
    XN_tmp = S->size[0] * S->size[1];
    if (n + 1 > XN_tmp) {
      emlrtDynamicBoundsCheckR2012b(n + 1, 1, XN_tmp, &m_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if ((!muDoubleScalarIsInf(S_data[n])) &&
        (!muDoubleScalarIsNaN(S_data[n]))) {
      if (n + 1 > XN_tmp) {
        emlrtDynamicBoundsCheckR2012b(n + 1, 1, XN_tmp, &q_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (S_data[n] > 1.0) {
        hasMultiServer = true;
        exitg1 = true;
      } else {
        n++;
      }
    } else {
      n++;
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  emxInit_real_T(sp, &b_mu, 2, &uc_emlrtRTEI);
  mu_data = b_mu->data;
  emxInit_real_T(sp, &a__1, 2, &sc_emlrtRTEI);
  emxInit_real_T(sp, &openClasses, 2, &jc_emlrtRTEI);
  emxInit_real_T(sp, &closedClasses, 2, &vc_emlrtRTEI);
  emxInit_int32_T(sp, &ii, 2, &wc_emlrtRTEI);
  emxInit_int32_T(sp, &ia, 1, &xc_emlrtRTEI);
  if (!hasMultiServer) {
    /*  if no multi-server nodes */
    if (hasOpenClasses) {
      /*  open or mixed model */
      st.site = &b_emlrtRSI;
      /* { */
      /*  % @file pfqn_mvamx.m */
      /*  % @brief Exact MVA for mixed open/closed single-server networks. */
      /* } */
      /* { */
      /*  % @brief Exact MVA for mixed open/closed single-server networks. */
      /*  % @fn pfqn_mvamx(lambda, D, N, Z, mi) */
      /*  % @param lambda Arrival rate vector. */
      /*  % @param D Service demand matrix. */
      /*  % @param N Population vector. */
      /*  % @param Z Think time vector. */
      /*  % @param mi Queue replication factors (default: ones). */
      /*  % @return XN System throughput. */
      /*  % @return QN Mean queue lengths. */
      /*  % @return UN Utilization. */
      /*  % @return CN Cycle times. */
      /*  % @return lGN Logarithm of normalizing constant. */
      /* } */
      /*  [XN,QN,UN,CN,LGN] = PFQN_MVAMX(LAMBDA,D,N,Z, MI) */
      for (r = 0; r < loop_ub; r++) {
        if (r + 1 > lambda->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, lambda->size[1], &s_emlrtBCI,
                                        &st);
        }
        if (lambda_data[r] > 0.0) {
          if (r + 1 > N->size[1]) {
            emlrtDynamicBoundsCheckR2012b(r + 1, 1, N->size[1], &k_emlrtBCI,
                                          &st);
          }
          if ((!muDoubleScalarIsInf(N_data[r])) &&
              (!muDoubleScalarIsNaN(N_data[r]))) {
            if (r + 1 > N->size[1]) {
              emlrtDynamicBoundsCheckR2012b(r + 1, 1, N->size[1], &t_emlrtBCI,
                                            &st);
            }
            if (N_data[r] > 0.0) {
              b_st.site = &n_emlrtRSI;
              /* LINE_ERROR Display a plain-text error message with file and
               * line info. */
              /*  */
              /*    LINE_ERROR(CALLER, MSG, ...) throws an error with CALLER's
               * name and */
              /*    message, including the source file and line number, in plain
               * text (no */
              /*    hyperlink). Extra arguments are passed to sprintf to format
               * MSG. */
              /*    Copyright (c) 2012-2026, Imperial College London */
              /*    All rights reserved. */
              c_st.site = &s_emlrtRSI;
              b_error(&c_st);
            }
          }
        }
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b(&st);
        }
      }
      n = openClasses->size[0] * openClasses->size[1];
      openClasses->size[0] = 1;
      openClasses->size[1] = L->size[1];
      emxEnsureCapacity_real_T(&st, openClasses, n, &mb_emlrtRTEI);
      openClasses_data = openClasses->data;
      n = closedClasses->size[0] * closedClasses->size[1];
      closedClasses->size[0] = 1;
      closedClasses->size[1] = L->size[1];
      emxEnsureCapacity_real_T(&st, closedClasses, n, &nb_emlrtRTEI);
      closedClasses_data = closedClasses->data;
      for (r = 0; r < loop_ub; r++) {
        openClasses_data[r] = 0.0;
        closedClasses_data[r] = 0.0;
      }
      nOpen = 0U;
      nClosed = 0U;
      for (r = 0; r < loop_ub; r++) {
        if (r + 1 > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, N->size[1], &j_emlrtBCI, &st);
        }
        if (muDoubleScalarIsInf(N_data[r])) {
          nOpen++;
          if (((int32_T)nOpen < 1) || ((int32_T)nOpen > openClasses->size[1])) {
            emlrtDynamicBoundsCheckR2012b(
                (int32_T)nOpen, 1, openClasses->size[1], &v_emlrtBCI, &st);
          }
          openClasses_data[(int32_T)nOpen - 1] = (real_T)r + 1.0;
        } else {
          nClosed++;
          if (((int32_T)nClosed < 1) ||
              ((int32_T)nClosed > closedClasses->size[1])) {
            emlrtDynamicBoundsCheckR2012b(
                (int32_T)nClosed, 1, closedClasses->size[1], &u_emlrtBCI, &st);
          }
          closedClasses_data[(int32_T)nClosed - 1] = (real_T)r + 1.0;
        }
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b(&st);
        }
      }
      if ((int32_T)nOpen < 1) {
        i = 0;
      } else {
        if (openClasses->size[1] < 1) {
          emlrtDynamicBoundsCheckR2012b(1, 1, openClasses->size[1], &i_emlrtBCI,
                                        &st);
        }
        if (((int32_T)nOpen < 1) || ((int32_T)nOpen > openClasses->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)nOpen, 1, openClasses->size[1],
                                        &h_emlrtBCI, &st);
        }
        i = (int32_T)nOpen;
      }
      n = openClasses->size[0] * openClasses->size[1];
      openClasses->size[1] = i;
      emxEnsureCapacity_real_T(&st, openClasses, n, &pb_emlrtRTEI);
      openClasses_data = openClasses->data;
      if ((int32_T)nClosed < 1) {
        d_loop_ub = 0;
      } else {
        if (closedClasses->size[1] < 1) {
          emlrtDynamicBoundsCheckR2012b(1, 1, closedClasses->size[1],
                                        &g_emlrtBCI, &st);
        }
        if (((int32_T)nClosed < 1) ||
            ((int32_T)nClosed > closedClasses->size[1])) {
          emlrtDynamicBoundsCheckR2012b(
              (int32_T)nClosed, 1, closedClasses->size[1], &f_emlrtBCI, &st);
        }
        d_loop_ub = (int32_T)nClosed;
      }
      XN_tmp = closedClasses->size[0] * closedClasses->size[1];
      closedClasses->size[1] = d_loop_ub;
      emxEnsureCapacity_real_T(&st, closedClasses, XN_tmp, &qb_emlrtRTEI);
      closedClasses_data = closedClasses->data;
      XN_tmp = b_lG->size[0] * b_lG->size[1];
      b_lG->size[0] = 1;
      b_lG->size[1] = L->size[1];
      emxEnsureCapacity_real_T(&st, b_lG, XN_tmp, &rb_emlrtRTEI);
      lG_data = b_lG->data;
      for (r = 0; r < loop_ub; r++) {
        lG_data[r] = 0.0;
      }
      XN_tmp = UN->size[0] * UN->size[1];
      UN->size[0] = L->size[0];
      UN->size[1] = L->size[1];
      emxEnsureCapacity_real_T(&st, UN, XN_tmp, &sb_emlrtRTEI);
      UN_data = UN->data;
      n = L->size[0] * L->size[1];
      for (r = 0; r < n; r++) {
        UN_data[r] = 0.0;
      }
      XN_tmp = CN->size[0] * CN->size[1];
      CN->size[0] = L->size[0];
      CN->size[1] = L->size[1];
      emxEnsureCapacity_real_T(&st, CN, XN_tmp, &tb_emlrtRTEI);
      CN_data = CN->data;
      for (r = 0; r < n; r++) {
        CN_data[r] = 0.0;
      }
      XN_tmp = QN->size[0] * QN->size[1];
      QN->size[0] = L->size[0];
      QN->size[1] = L->size[1];
      emxEnsureCapacity_real_T(&st, QN, XN_tmp, &ub_emlrtRTEI);
      QN_data = QN->data;
      for (r = 0; r < n; r++) {
        QN_data[r] = 0.0;
      }
      b_mu->size[0] = L->size[0];
      b_mu->size[1] = 0;
      c_lG = rtNaN;
      i3 = (int32_T)nOpen;
      for (ist = 0; ist < i3; ist++) {
        if (ist + 1 > i) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, i, &w_emlrtBCI, &st);
        }
        XN_tmp = (int32_T)openClasses_data[ist];
        n = (L->size[0] / 2) << 1;
        m = n - 2;
        for (r = 0; r <= m; r += 2) {
          if (r + 1 > b_loop_ub) {
            emlrtDynamicBoundsCheckR2012b(r + 1, 1, b_loop_ub, &fd_emlrtBCI,
                                          &st);
          }
          if ((XN_tmp < 1) || (XN_tmp > loop_ub)) {
            emlrtDynamicBoundsCheckR2012b(XN_tmp, 1, loop_ub, &fd_emlrtBCI,
                                          &st);
          }
          if (XN_tmp > lambda->size[1]) {
            emlrtDynamicBoundsCheckR2012b(XN_tmp, 1, lambda->size[1],
                                          &ed_emlrtBCI, &st);
          }
          if (r + 1 > UN->size[0]) {
            emlrtDynamicBoundsCheckR2012b(r + 1, 1, UN->size[0], &gd_emlrtBCI,
                                          &st);
          }
          if (XN_tmp > UN->size[1]) {
            emlrtDynamicBoundsCheckR2012b(XN_tmp, 1, UN->size[1], &gd_emlrtBCI,
                                          &st);
          }
          _mm_storeu_pd(
              &UN_data[r + UN->size[0] * (XN_tmp - 1)],
              _mm_mul_pd(_mm_set1_pd(lambda_data[XN_tmp - 1]),
                         _mm_loadu_pd(&L_data[r + L->size[0] * (XN_tmp - 1)])));
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&st);
          }
        }
        for (r = n; r < b_loop_ub; r++) {
          if ((XN_tmp < 1) || (XN_tmp > lambda->size[1])) {
            emlrtDynamicBoundsCheckR2012b(XN_tmp, 1, lambda->size[1],
                                          &ab_emlrtBCI, &st);
          }
          if (r + 1 > b_loop_ub) {
            emlrtDynamicBoundsCheckR2012b(r + 1, 1, b_loop_ub, &bb_emlrtBCI,
                                          &st);
          }
          if (XN_tmp > loop_ub) {
            emlrtDynamicBoundsCheckR2012b(XN_tmp, 1, loop_ub, &cb_emlrtBCI,
                                          &st);
          }
          if (r + 1 > UN->size[0]) {
            emlrtDynamicBoundsCheckR2012b(r + 1, 1, UN->size[0], &db_emlrtBCI,
                                          &st);
          }
          if (XN_tmp > UN->size[1]) {
            emlrtDynamicBoundsCheckR2012b(XN_tmp, 1, UN->size[1], &eb_emlrtBCI,
                                          &st);
          }
          UN_data[r + UN->size[0] * (XN_tmp - 1)] =
              lambda_data[XN_tmp - 1] * L_data[r + L->size[0] * (XN_tmp - 1)];
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&st);
          }
        }
        n = (int32_T)openClasses_data[ist];
        if ((n < 1) || (n > lambda->size[1])) {
          emlrtDynamicBoundsCheckR2012b(n, 1, lambda->size[1], &x_emlrtBCI,
                                        &st);
        }
        if (n > b_lG->size[1]) {
          emlrtDynamicBoundsCheckR2012b(n, 1, b_lG->size[1], &y_emlrtBCI, &st);
        }
        lG_data[n - 1] = lambda_data[n - 1];
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b(&st);
        }
      }
      emxInit_real_T(&st, &UNt, 1, &rc_emlrtRTEI);
      b_st.site = &o_emlrtRSI;
      sum(&b_st, UN, UNt);
      UNt_data = UNt->data;
      if (Z->size[1] == 0) {
        XN_tmp = Z->size[0] * Z->size[1];
        Z->size[0] = 1;
        Z->size[1] = L->size[1];
        emxEnsureCapacity_real_T(&st, Z, XN_tmp, &wb_emlrtRTEI);
        Z_data = Z->data;
        for (r = 0; r < loop_ub; r++) {
          Z_data[r] = 0.0;
        }
      }
      if ((int32_T)nClosed > 0) {
        b_st.site = &p_emlrtRSI;
        XN_tmp = mu->size[0] * mu->size[1];
        mu->size[0] = L->size[0];
        mu->size[1] = d_loop_ub;
        emxEnsureCapacity_real_T(&b_st, mu, XN_tmp, &xb_emlrtRTEI);
        mu_data = mu->data;
        for (r = 0; r < d_loop_ub; r++) {
          for (ist = 0; ist < b_loop_ub; ist++) {
            XN_tmp = (int32_T)closedClasses_data[r];
            if ((XN_tmp < 1) || (XN_tmp > loop_ub)) {
              emlrtDynamicBoundsCheckR2012b(XN_tmp, 1, loop_ub, &gb_emlrtBCI,
                                            &b_st);
            }
            mu_data[ist + mu->size[0] * r] =
                L_data[ist + L->size[0] * (XN_tmp - 1)];
          }
        }
        emxInit_real_T(&b_st, &b_y, 2, &tc_emlrtRTEI);
        c_st.site = &p_emlrtRSI;
        repmat(&c_st, UNt, nClosed, b_y);
        b_S_data = b_y->data;
        XN_tmp = b_y->size[0] * b_y->size[1];
        n = (XN_tmp / 2) << 1;
        m = n - 2;
        for (r = 0; r <= m; r += 2) {
          b_r = _mm_loadu_pd(&b_S_data[r]);
          _mm_storeu_pd(&b_S_data[r], _mm_sub_pd(_mm_set1_pd(1.0), b_r));
        }
        for (r = n; r < XN_tmp; r++) {
          b_S_data[r] = 1.0 - b_S_data[r];
        }
        c_st.site = &kb_emlrtRSI;
        d_st.site = &lb_emlrtRSI;
        assertCompatibleDims(&d_st, mu, b_y);
        emxInit_real_T(&st, &CNc, 2, &sc_emlrtRTEI);
        if ((mu->size[0] == b_y->size[0]) && (mu->size[1] == b_y->size[1])) {
          emxInit_real_T(&st, &c_mu, 2, &ec_emlrtRTEI);
          XN_tmp = c_mu->size[0] * c_mu->size[1];
          c_mu->size[0] = mu->size[0];
          c_mu->size[1] = mu->size[1];
          emxEnsureCapacity_real_T(&st, c_mu, XN_tmp, &ec_emlrtRTEI);
          XN_data = c_mu->data;
          XN_tmp = mu->size[0] * mu->size[1];
          n = (XN_tmp / 2) << 1;
          m = n - 2;
          for (r = 0; r <= m; r += 2) {
            b_r = _mm_loadu_pd(&mu_data[r]);
            r1 = _mm_loadu_pd(&b_S_data[r]);
            _mm_storeu_pd(&XN_data[r], _mm_div_pd(b_r, r1));
          }
          for (r = n; r < XN_tmp; r++) {
            XN_data[r] = mu_data[r] / b_S_data[r];
          }
          XN_tmp = b_S->size[0] * b_S->size[1];
          b_S->size[0] = 1;
          b_S->size[1] = d_loop_ub;
          emxEnsureCapacity_real_T(&st, b_S, XN_tmp, &hc_emlrtRTEI);
          b_S_data = b_S->data;
          for (r = 0; r < d_loop_ub; r++) {
            n = (int32_T)closedClasses_data[r];
            if ((n < 1) || (n > N->size[1])) {
              emlrtDynamicBoundsCheckR2012b(n, 1, N->size[1], &dc_emlrtBCI,
                                            &st);
            }
            b_S_data[r] = N_data[n - 1];
          }
          XN_tmp = y->size[0] * y->size[1];
          y->size[0] = 1;
          y->size[1] = d_loop_ub;
          emxEnsureCapacity_real_T(&st, y, XN_tmp, &kc_emlrtRTEI);
          XN_data = y->data;
          for (r = 0; r < d_loop_ub; r++) {
            n = (int32_T)closedClasses_data[r];
            if ((n < 1) || (n > Z->size[1])) {
              emlrtDynamicBoundsCheckR2012b(n, 1, Z->size[1], &fc_emlrtBCI,
                                            &st);
            }
            XN_data[r] = Z_data[n - 1];
          }
          b_st.site = &q_emlrtRSI;
          c_lG = pfqn_mva(&b_st, c_mu, b_S, y, mi, XN, b_mu, a__1, CNc);
          b_S_data = CNc->data;
          mu_data = b_mu->data;
          XN_data = XN->data;
          emxFree_real_T(&st, &c_mu);
        } else {
          b_st.site = &q_emlrtRSI;
          c_lG = binary_expand_op(&b_st, q_emlrtRSI, mu, b_y, N, closedClasses,
                                  Z, mi, XN, b_mu, a__1, CNc);
          b_S_data = CNc->data;
          mu_data = b_mu->data;
          XN_data = XN->data;
        }
        emxFree_real_T(&st, &b_y);
        n = ii->size[0] * ii->size[1];
        ii->size[0] = 1;
        e_loop_ub = closedClasses->size[1];
        ii->size[1] = closedClasses->size[1];
        emxEnsureCapacity_int32_T(&st, ii, n, &fc_emlrtRTEI);
        ii_data = ii->data;
        m = b_lG->size[1];
        XN_tmp = closedClasses->size[1];
        c_loop_ub = (closedClasses->size[1] < 1600);
        if (c_loop_ub) {
          for (i4 = 0; i4 < e_loop_ub; i4++) {
            XN_tmp = (int32_T)closedClasses_data[i4];
            if ((XN_tmp < 1) || (XN_tmp > m)) {
              emlrtDynamicBoundsCheckR2012b(XN_tmp, 1, m, &cc_emlrtBCI, &st);
            }
            ii_data[i4] = XN_tmp;
          }
        } else {
          emlrtEnterParallelRegion(&st, omp_in_parallel());
          emlrtPushJmpBuf(&st, &emlrtJBStack);
          pfqn_mvams_numThreads =
              emlrtAllocRegionTLSs(st.tls, omp_in_parallel(),
                                   omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_mvams_numThreads) private(               \
        j_st, emlrtJBEnviron, i5) firstprivate(st, emlrtHadParallelError)
          {
            if (setjmp(emlrtJBEnviron) == 0) {
              j_st.prev = &st;
              j_st.tls = emlrtAllocTLS(&st, omp_get_thread_num());
              j_st.site = NULL;
              emlrtSetJmpBuf(&j_st, &emlrtJBEnviron);
            } else {
              emlrtHadParallelError = true;
            }
#pragma omp for nowait
            for (i4 = 0; i4 < XN_tmp; i4++) {
              if (emlrtHadParallelError) {
                continue;
              }
              if (setjmp(emlrtJBEnviron) == 0) {
                i5 = (int32_T)closedClasses_data[i4];
                if ((i5 < 1) || (i5 > m)) {
                  emlrtDynamicBoundsCheckR2012b(i5, 1, m, &cc_emlrtBCI, &j_st);
                }
                ii_data[i4] = i5;
              } else {
                emlrtHadParallelError = true;
              }
            }
          }
          emlrtPopJmpBuf(&st, &emlrtJBStack);
          emlrtExitParallelRegion(&st, omp_in_parallel());
        }
        n = ii->size[1];
        XN_tmp = XN->size[0] * XN->size[1];
        if (ii->size[1] != XN_tmp) {
          emlrtSubAssignSizeCheck1dR2017a(ii->size[1], XN_tmp, &d_emlrtECI,
                                          &st);
        }
        for (r = 0; r < n; r++) {
          lG_data[ii_data[r] - 1] = XN_data[r];
        }
        XN_tmp = ia->size[0];
        ia->size[0] = e_loop_ub;
        emxEnsureCapacity_int32_T(&st, ia, XN_tmp, &lc_emlrtRTEI);
        ii_data = ia->data;
        if (c_loop_ub) {
          for (i6 = 0; i6 < e_loop_ub; i6++) {
            XN_tmp = (int32_T)closedClasses_data[i6];
            if ((XN_tmp < 1) || (XN_tmp > loop_ub)) {
              emlrtDynamicBoundsCheckR2012b(XN_tmp, 1, loop_ub, &ec_emlrtBCI,
                                            &st);
            }
            ii_data[i6] = XN_tmp - 1;
          }
        } else {
          emlrtEnterParallelRegion(&st, omp_in_parallel());
          emlrtPushJmpBuf(&st, &emlrtJBStack);
          pfqn_mvams_numThreads =
              emlrtAllocRegionTLSs(st.tls, omp_in_parallel(),
                                   omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_mvams_numThreads) private(               \
        emlrtJBEnviron, k_st, i7) firstprivate(emlrtHadParallelError, st)
          {
            if (setjmp(emlrtJBEnviron) == 0) {
              k_st.prev = &st;
              k_st.tls = emlrtAllocTLS(&st, omp_get_thread_num());
              k_st.site = NULL;
              emlrtSetJmpBuf(&k_st, &emlrtJBEnviron);
            } else {
              emlrtHadParallelError = true;
            }
#pragma omp for nowait
            for (i6 = 0; i6 < e_loop_ub; i6++) {
              if (emlrtHadParallelError) {
                continue;
              }
              if (setjmp(emlrtJBEnviron) == 0) {
                i7 = (int32_T)closedClasses_data[i6];
                if ((i7 < 1) || (i7 > loop_ub)) {
                  emlrtDynamicBoundsCheckR2012b(i7, 1, loop_ub, &ec_emlrtBCI,
                                                &k_st);
                }
                ii_data[i6] = i7 - 1;
              } else {
                emlrtHadParallelError = true;
              }
            }
          }
          emlrtPopJmpBuf(&st, &emlrtJBStack);
          emlrtExitParallelRegion(&st, omp_in_parallel());
        }
        b_XN[0] = b_loop_ub;
        XN_tmp = ia->size[0];
        b_XN[1] = ia->size[0];
        emlrtSubAssignSizeCheckR2012b(&b_XN[0], 2, &b_mu->size[0], 2,
                                      &b_emlrtECI, &st);
        for (r = 0; r < XN_tmp; r++) {
          for (ist = 0; ist < b_loop_ub; ist++) {
            QN_data[ist + QN->size[0] * ii_data[r]] =
                mu_data[ist + b_loop_ub * r];
          }
        }
        XN_tmp = ia->size[0];
        ia->size[0] = e_loop_ub;
        emxEnsureCapacity_int32_T(&st, ia, XN_tmp, &nc_emlrtRTEI);
        ii_data = ia->data;
        if (c_loop_ub) {
          for (i8 = 0; i8 < e_loop_ub; i8++) {
            XN_tmp = (int32_T)closedClasses_data[i8];
            if ((XN_tmp < 1) || (XN_tmp > loop_ub)) {
              emlrtDynamicBoundsCheckR2012b(XN_tmp, 1, loop_ub, &gc_emlrtBCI,
                                            &st);
            }
            ii_data[i8] = XN_tmp - 1;
          }
        } else {
          emlrtEnterParallelRegion(&st, omp_in_parallel());
          emlrtPushJmpBuf(&st, &emlrtJBStack);
          pfqn_mvams_numThreads =
              emlrtAllocRegionTLSs(st.tls, omp_in_parallel(),
                                   omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_mvams_numThreads) private(               \
        emlrtJBEnviron, l_st, i9) firstprivate(emlrtHadParallelError, st)
          {
            if (setjmp(emlrtJBEnviron) == 0) {
              l_st.prev = &st;
              l_st.tls = emlrtAllocTLS(&st, omp_get_thread_num());
              l_st.site = NULL;
              emlrtSetJmpBuf(&l_st, &emlrtJBEnviron);
            } else {
              emlrtHadParallelError = true;
            }
#pragma omp for nowait
            for (i8 = 0; i8 < e_loop_ub; i8++) {
              if (emlrtHadParallelError) {
                continue;
              }
              if (setjmp(emlrtJBEnviron) == 0) {
                i9 = (int32_T)closedClasses_data[i8];
                if ((i9 < 1) || (i9 > loop_ub)) {
                  emlrtDynamicBoundsCheckR2012b(i9, 1, loop_ub, &gc_emlrtBCI,
                                                &l_st);
                }
                ii_data[i8] = i9 - 1;
              } else {
                emlrtHadParallelError = true;
              }
            }
          }
          emlrtPopJmpBuf(&st, &emlrtJBStack);
          emlrtExitParallelRegion(&st, omp_in_parallel());
        }
        b_XN[0] = b_loop_ub;
        XN_tmp = ia->size[0];
        b_XN[1] = ia->size[0];
        emlrtSubAssignSizeCheckR2012b(&b_XN[0], 2, &CNc->size[0], 2,
                                      &c_emlrtECI, &st);
        for (r = 0; r < XN_tmp; r++) {
          for (ist = 0; ist < b_loop_ub; ist++) {
            CN_data[ist + CN->size[0] * ii_data[r]] =
                b_S_data[ist + b_loop_ub * r];
          }
        }
        emxFree_real_T(&st, &CNc);
      }
      for (r = 0; r < b_loop_ub; r++) {
        XN_tmp = (int32_T)nClosed;
        for (ist = 0; ist < XN_tmp; ist++) {
          if (ist + 1 > d_loop_ub) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, d_loop_ub, &e_emlrtBCI,
                                          &st);
          }
          Ntot = closedClasses_data[ist];
          if (((int32_T)(uint32_T)Ntot < 1) ||
              ((int32_T)(uint32_T)Ntot > b_lG->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)(uint32_T)Ntot, 1,
                                          b_lG->size[1], &hb_emlrtBCI, &st);
          }
          if (r + 1 > b_loop_ub) {
            emlrtDynamicBoundsCheckR2012b(r + 1, 1, b_loop_ub, &ib_emlrtBCI,
                                          &st);
          }
          if (((int32_T)(uint32_T)Ntot < 1) ||
              ((int32_T)(uint32_T)Ntot > loop_ub)) {
            emlrtDynamicBoundsCheckR2012b((int32_T)(uint32_T)Ntot, 1, loop_ub,
                                          &jb_emlrtBCI, &st);
          }
          if (r + 1 > UN->size[0]) {
            emlrtDynamicBoundsCheckR2012b(r + 1, 1, UN->size[0], &kb_emlrtBCI,
                                          &st);
          }
          if (((int32_T)(uint32_T)Ntot < 1) ||
              ((int32_T)(uint32_T)Ntot > UN->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)(uint32_T)Ntot, 1,
                                          UN->size[1], &lb_emlrtBCI, &st);
          }
          UN_data[r + UN->size[0] * ((int32_T)(uint32_T)Ntot - 1)] =
              lG_data[(int32_T)(uint32_T)Ntot - 1] *
              L_data[r + L->size[0] * ((int32_T)(uint32_T)Ntot - 1)];
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&st);
          }
        }
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b(&st);
        }
      }
      for (ist = 0; ist < b_loop_ub; ist++) {
        for (ridx = 0; ridx < i3; ridx++) {
          if (ridx + 1 > i) {
            emlrtDynamicBoundsCheckR2012b(ridx + 1, 1, i, &d_emlrtBCI, &st);
          }
          n = b_mu->size[1];
          if ((b_mu->size[0] == 0) || (b_mu->size[1] == 0)) {
            Ntot = openClasses_data[ridx];
            if (ist + 1 > b_loop_ub) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, b_loop_ub, &mb_emlrtBCI,
                                            &st);
            }
            n = (int32_T)Ntot;
            if ((n < 1) || (n > loop_ub)) {
              emlrtDynamicBoundsCheckR2012b(n, 1, loop_ub, &nb_emlrtBCI, &st);
            }
            if (ist + 1 > UNt->size[0]) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, UNt->size[0],
                                            &ob_emlrtBCI, &st);
            }
            if (ist + 1 > CN->size[0]) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, CN->size[0],
                                            &pb_emlrtBCI, &st);
            }
            if (n > CN->size[1]) {
              emlrtDynamicBoundsCheckR2012b(n, 1, CN->size[1], &qb_emlrtBCI,
                                            &st);
            }
            CN_data[ist + CN->size[0] * (n - 1)] =
                L_data[ist + L->size[0] * (n - 1)] / (1.0 - UNt_data[ist]);
          } else {
            if (ist + 1 > b_mu->size[0]) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, b_mu->size[0],
                                            &c_emlrtBCI, &st);
            }
            XN_tmp = b_S->size[0] * b_S->size[1];
            b_S->size[0] = 1;
            b_S->size[1] = b_mu->size[1];
            emxEnsureCapacity_real_T(&st, b_S, XN_tmp, &cc_emlrtRTEI);
            b_S_data = b_S->data;
            for (r = 0; r < n; r++) {
              b_S_data[r] = mu_data[ist + b_mu->size[0] * r];
            }
            Ntot = openClasses_data[ridx];
            if (ist + 1 > b_loop_ub) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, b_loop_ub, &rb_emlrtBCI,
                                            &st);
            }
            n = (int32_T)Ntot;
            if ((n < 1) || (n > loop_ub)) {
              emlrtDynamicBoundsCheckR2012b(n, 1, loop_ub, &sb_emlrtBCI, &st);
            }
            if (ist + 1 > UNt->size[0]) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, UNt->size[0],
                                            &tb_emlrtBCI, &st);
            }
            if (ist + 1 > CN->size[0]) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, CN->size[0],
                                            &ub_emlrtBCI, &st);
            }
            if (n > CN->size[1]) {
              emlrtDynamicBoundsCheckR2012b(n, 1, CN->size[1], &vb_emlrtBCI,
                                            &st);
            }
            b_st.site = &r_emlrtRSI;
            CN_data[ist + CN->size[0] * (n - 1)] =
                L_data[ist + L->size[0] * (n - 1)] * (1.0 + b_sum(&b_st, b_S)) /
                (1.0 - UNt_data[ist]);
          }
          if (ist + 1 > CN->size[0]) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, CN->size[0], &wb_emlrtBCI,
                                          &st);
          }
          n = (int32_T)Ntot;
          if ((n < 1) || (n > CN->size[1])) {
            emlrtDynamicBoundsCheckR2012b(n, 1, CN->size[1], &xb_emlrtBCI, &st);
          }
          if (n > b_lG->size[1]) {
            emlrtDynamicBoundsCheckR2012b(n, 1, b_lG->size[1], &yb_emlrtBCI,
                                          &st);
          }
          if (ist + 1 > QN->size[0]) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, QN->size[0], &ac_emlrtBCI,
                                          &st);
          }
          if (n > QN->size[1]) {
            emlrtDynamicBoundsCheckR2012b(n, 1, QN->size[1], &bc_emlrtBCI, &st);
          }
          QN_data[ist + QN->size[0] * (n - 1)] =
              CN_data[ist + CN->size[0] * (n - 1)] * lG_data[n - 1];
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&st);
          }
        }
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b(&st);
        }
      }
      emxFree_real_T(&st, &UNt);
      n = XN->size[0] * XN->size[1];
      XN->size[0] = 1;
      XN_tmp = b_lG->size[1];
      XN->size[1] = b_lG->size[1];
      emxEnsureCapacity_real_T(sp, XN, n, &ac_emlrtRTEI);
      XN_data = XN->data;
      for (r = 0; r < XN_tmp; r++) {
        XN_data[r] = lG_data[r];
      }
    } else {
      /*  closed model */
      n = y->size[0] * y->size[1];
      y->size[0] = 1;
      y->size[1] = N->size[1];
      emxEnsureCapacity_real_T(sp, y, n, &hb_emlrtRTEI);
      XN_data = y->data;
      n = N->size[0] * N->size[1] - 1;
      for (r = 0; r <= n; r++) {
        XN_data[r] = N_data[r];
      }
      n = b_S->size[0] * b_S->size[1];
      b_S->size[0] = 1;
      b_S->size[1] = Z->size[1];
      emxEnsureCapacity_real_T(sp, b_S, n, &ib_emlrtRTEI);
      b_S_data = b_S->data;
      n = Z->size[0] * Z->size[1] - 1;
      for (r = 0; r <= n; r++) {
        b_S_data[r] = Z_data[r];
      }
      st.site = &c_emlrtRSI;
      c_lG = pfqn_mva(&st, L, y, b_S, mi, XN, QN, UN, CN);
    }

    /*  if the model has multi-server nodes */
  } else if (hasOpenClasses) {
    /*  open or mixed model */
    st.site = &d_emlrtRSI;
    b_st.site = &mc_emlrtRSI;
    c_st.site = &nc_emlrtRSI;
    d_st.site = &oc_emlrtRSI;
    if (((mi->size[0] != 1) || (mi->size[1] != 1)) && (mi->size[0] == 1)) {
      emlrtErrorWithMessageIdR2018a(&d_st, &b_emlrtRTEI,
                                    "Coder:toolbox:autoDimIncompatibility",
                                    "Coder:toolbox:autoDimIncompatibility", 0);
    }
    if (mi->size[0] < 1) {
      emlrtErrorWithMessageIdR2018a(
          &d_st, &emlrtRTEI, "Coder:toolbox:eml_min_or_max_varDimZero",
          "Coder:toolbox:eml_min_or_max_varDimZero", 0);
    }
    e_st.site = &pc_emlrtRSI;
    f_st.site = &qc_emlrtRSI;
    g_st.site = &rc_emlrtRSI;
    m = mi->size[0];
    n = mi->size[1];
    XN_tmp = b_lG->size[0] * b_lG->size[1];
    b_lG->size[0] = 1;
    b_lG->size[1] = mi->size[1];
    emxEnsureCapacity_real_T(&g_st, b_lG, XN_tmp, &lb_emlrtRTEI);
    lG_data = b_lG->data;
    if (mi->size[1] >= 1) {
      h_st.site = &tc_emlrtRSI;
      if (mi->size[1] > 2147483646) {
        i_st.site = &gb_emlrtRSI;
        check_forloop_overflow_error(&i_st);
      }
      hasOpenClasses = (mi->size[0] > 2147483646);
      for (r = 0; r < n; r++) {
        lG_data[r] = mi_data[mi->size[0] * r];
        h_st.site = &sc_emlrtRSI;
        if (hasOpenClasses) {
          i_st.site = &gb_emlrtRSI;
          check_forloop_overflow_error(&i_st);
        }
        for (ist = 2; ist <= m; ist++) {
          Ntot = lG_data[r];
          b = mi_data[(ist + mi->size[0] * r) - 1];
          if (muDoubleScalarIsNaN(b)) {
            hasMultiServer = false;
          } else if (muDoubleScalarIsNaN(Ntot)) {
            hasMultiServer = true;
          } else {
            hasMultiServer = (Ntot < b);
          }
          if (hasMultiServer) {
            lG_data[r] = b;
          }
        }
      }
    }
    emxInit_boolean_T(sp, &d_lG, &ob_emlrtRTEI);
    XN_tmp = d_lG->size[0] * d_lG->size[1];
    d_lG->size[0] = 1;
    d_lG->size[1] = mi->size[1];
    emxEnsureCapacity_boolean_T(sp, d_lG, XN_tmp, &ob_emlrtRTEI);
    b_lG_data = d_lG->data;
    XN_tmp = mi->size[1];
    if (b_lG->size[1] < 1600) {
      for (i1 = 0; i1 < n; i1++) {
        b_lG_data[i1] = (lG_data[i1] == 1.0);
      }
    } else {
      emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
      emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
      pfqn_mvams_numThreads =
          emlrtAllocRegionTLSs(sp->tls, omp_in_parallel(),
                               omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mvams_numThreads)

      for (i1 = 0; i1 < XN_tmp; i1++) {
        b_lG_data[i1] = (lG_data[i1] == 1.0);
      }
      emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
      emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
    }
    st.site = &d_emlrtRSI;
    if (ifWhileCond(&st, d_lG)) {
      c_lG = rtNaN;
      /*  NC not available in this case */
      st.site = &e_emlrtRSI;
      /* { */
      /*  % @file pfqn_mvaldms.m */
      /*  % @brief Load-dependent MVA for multiserver mixed networks (wrapper
       * for pfqn_mvaldmx). */
      /* } */
      /* { */
      /*  % @brief Load-dependent MVA for multiserver mixed networks (wrapper
       * for pfqn_mvaldmx). */
      /*  % @fn pfqn_mvaldms(lambda, D, N, Z, S) */
      /*  % @param lambda Arrival rate vector. */
      /*  % @param D Service demand matrix. */
      /*  % @param N Population vector. */
      /*  % @param Z Think time vector. */
      /*  % @param S Number of servers per station. */
      /*  % @return XN System throughput. */
      /*  % @return QN Mean queue lengths. */
      /*  % @return UN Utilization (adjusted for multiservers). */
      /*  % @return CN Cycle times. */
      /*  % @return lGN Logarithm of normalizing constant. */
      /* } */
      /*  [XN,QN,UN,CN] = PFQN_MVALDMS(LAMBDA,D,N,Z,S) */
      /*  Wrapper for pfqn_mvaldmx that adjusts utilizations to account for */
      /*  multiservers */
      XN_tmp = d_lG->size[0] * d_lG->size[1];
      d_lG->size[0] = 1;
      c_loop_ub = N->size[1];
      d_lG->size[1] = N->size[1];
      emxEnsureCapacity_boolean_T(&st, d_lG, XN_tmp, &vb_emlrtRTEI);
      b_lG_data = d_lG->data;
      XN_tmp = N->size[1];
      if (N->size[1] < 1600) {
        for (i2 = 0; i2 < c_loop_ub; i2++) {
          b_lG_data[i2] = muDoubleScalarIsInf(N_data[i2]);
        }
      } else {
        emlrtEnterParallelRegion(&st, omp_in_parallel());
        emlrtPushJmpBuf(&st, &emlrtJBStack);
        pfqn_mvams_numThreads =
            emlrtAllocRegionTLSs(st.tls, omp_in_parallel(),
                                 omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mvams_numThreads)

        for (i2 = 0; i2 < XN_tmp; i2++) {
          b_lG_data[i2] = muDoubleScalarIsInf(N_data[i2]);
        }
        emlrtPopJmpBuf(&st, &emlrtJBStack);
        emlrtExitParallelRegion(&st, omp_in_parallel());
      }
      n = 0;
      for (r = 0; r < c_loop_ub; r++) {
        if ((!b_lG_data[r]) && (!muDoubleScalarIsNaN(N_data[r]))) {
          n++;
        }
      }
      XN_tmp = ii->size[0] * ii->size[1];
      ii->size[0] = 1;
      ii->size[1] = n;
      emxEnsureCapacity_int32_T(&st, ii, XN_tmp, &yb_emlrtRTEI);
      ii_data = ii->data;
      XN_tmp = 0;
      for (r = 0; r < c_loop_ub; r++) {
        if ((!b_lG_data[r]) && (!muDoubleScalarIsNaN(N_data[r]))) {
          ii_data[XN_tmp] = r;
          XN_tmp++;
        }
      }
      XN_tmp = b_S->size[0] * b_S->size[1];
      b_S->size[0] = 1;
      n = ii->size[1];
      b_S->size[1] = ii->size[1];
      emxEnsureCapacity_real_T(&st, b_S, XN_tmp, &bc_emlrtRTEI);
      b_S_data = b_S->data;
      for (r = 0; r < n; r++) {
        if (ii_data[r] > N->size[1] - 1) {
          emlrtDynamicBoundsCheckR2012b(ii_data[r], 0, N->size[1] - 1,
                                        &fb_emlrtBCI, &st);
        }
        b_S_data[r] = N_data[ii_data[r]];
      }
      b_st.site = &wc_emlrtRSI;
      Ntot = b_sum(&b_st, b_S);
      XN_tmp = b_mu->size[0] * b_mu->size[1];
      b_mu->size[0] = b_loop_ub;
      emxEnsureCapacity_real_T(&st, b_mu, XN_tmp, &dc_emlrtRTEI);
      if (!(Ntot >= 0.0)) {
        emlrtNonNegativeCheckR2012b(Ntot, &e_emlrtDCI, &st);
      }
      if (Ntot != (int32_T)muDoubleScalarFloor(Ntot)) {
        emlrtIntegerCheckR2012b(Ntot, &d_emlrtDCI, &st);
      }
      XN_tmp = b_mu->size[0] * b_mu->size[1];
      b_mu->size[1] = (int32_T)Ntot;
      emxEnsureCapacity_real_T(&st, b_mu, XN_tmp, &dc_emlrtRTEI);
      mu_data = b_mu->data;
      if (L->size[0] - 1 >= 0) {
        if (Ntot < 1.0) {
          y->size[0] = 1;
          y->size[1] = 0;
        } else {
          n = y->size[0] * y->size[1];
          y->size[0] = 1;
          y->size[1] = (int32_T)(Ntot - 1.0) + 1;
          emxEnsureCapacity_real_T(&st, y, n, &gb_emlrtRTEI);
          XN_data = y->data;
          n = (int32_T)(Ntot - 1.0);
          XN_tmp = (((int32_T)(Ntot - 1.0) + 1) / 2) << 1;
          m = XN_tmp - 2;
          for (r = 0; r <= m; r += 2) {
            dv[0] = r;
            dv[1] = r + 1;
            b_r = _mm_loadu_pd(&dv[0]);
            _mm_storeu_pd(&XN_data[r], _mm_add_pd(_mm_set1_pd(1.0), b_r));
          }
          for (r = XN_tmp; r <= n; r++) {
            XN_data[r] = (real_T)r + 1.0;
          }
        }
        e_loop_ub = y->size[1];
      }
      for (ist = 0; ist < b_loop_ub; ist++) {
        if (ist + 1 > b_mu->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, b_mu->size[0], &emlrtBCI,
                                        &st);
        }
        XN_tmp = S->size[0] * S->size[1];
        if (ist + 1 > XN_tmp) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, XN_tmp, &b_emlrtBCI, &st);
        }
        XN_tmp = b_lG->size[0] * b_lG->size[1];
        b_lG->size[0] = 1;
        b_lG->size[1] = y->size[1];
        emxEnsureCapacity_real_T(&st, b_lG, XN_tmp, &ic_emlrtRTEI);
        lG_data = b_lG->data;
        for (r = 0; r < e_loop_ub; r++) {
          Ntot = XN_data[r];
          b = S_data[ist];
          lG_data[r] = muDoubleScalarMin(Ntot, b);
        }
        b_XN[0] = 1;
        n = b_mu->size[1];
        b_XN[1] = b_mu->size[1];
        emlrtSubAssignSizeCheckR2012b(&b_XN[0], 2, &b_lG->size[0], 2, &emlrtECI,
                                      &st);
        for (r = 0; r < n; r++) {
          mu_data[ist + b_mu->size[0] * r] = lG_data[r];
        }
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b(&st);
        }
      }
      if (Z->size[1] == 0) {
        n = Z->size[0] * Z->size[1];
        Z->size[0] = 1;
        Z->size[1] = loop_ub;
        emxEnsureCapacity_real_T(&st, Z, n, &gc_emlrtRTEI);
        Z_data = Z->data;
        for (r = 0; r < loop_ub; r++) {
          Z_data[r] = 0.0;
        }
      }
      b_st.site = &xc_emlrtRSI;
      pfqn_mvaldmx(&b_st, lambda, L, N, Z, b_mu, b_lG, QN, a__1, CN);
      lG_data = b_lG->data;
      b_st.site = &yc_emlrtRSI;
      c_st.site = &ge_emlrtRSI;
      eml_find(&c_st, d_lG, ii);
      ii_data = ii->data;
      XN_tmp = openClasses->size[0] * openClasses->size[1];
      openClasses->size[0] = 1;
      e_loop_ub = ii->size[1];
      openClasses->size[1] = ii->size[1];
      emxEnsureCapacity_real_T(&b_st, openClasses, XN_tmp, &jc_emlrtRTEI);
      openClasses_data = openClasses->data;
      for (r = 0; r < e_loop_ub; r++) {
        openClasses_data[r] = ii_data[r];
      }
      if (N->size[1] < 1) {
        y->size[0] = 1;
        y->size[1] = 0;
      } else {
        XN_tmp = y->size[0] * y->size[1];
        y->size[0] = 1;
        y->size[1] = c_loop_ub;
        emxEnsureCapacity_real_T(&st, y, XN_tmp, &gb_emlrtRTEI);
        XN_data = y->data;
        n = N->size[1] - 1;
        XN_tmp = (N->size[1] / 2) << 1;
        m = XN_tmp - 2;
        for (r = 0; r <= m; r += 2) {
          dv[0] = r;
          dv[1] = r + 1;
          b_r = _mm_loadu_pd(&dv[0]);
          _mm_storeu_pd(&XN_data[r], _mm_add_pd(_mm_set1_pd(1.0), b_r));
        }
        for (r = XN_tmp; r <= n; r++) {
          XN_data[r] = (real_T)r + 1.0;
        }
      }
      b_st.site = &ad_emlrtRSI;
      c_st.site = &je_emlrtRSI;
      d_st.site = &ke_emlrtRSI;
      do_vectors(&d_st, y, openClasses, closedClasses, ia);
      closedClasses_data = closedClasses->data;
      n = UN->size[0] * UN->size[1];
      UN->size[0] = b_loop_ub;
      UN->size[1] = loop_ub;
      emxEnsureCapacity_real_T(&st, UN, n, &mc_emlrtRTEI);
      UN_data = UN->data;
      XN_tmp = L->size[0] * L->size[1];
      for (r = 0; r < XN_tmp; r++) {
        UN_data[r] = 0.0;
      }
      m = closedClasses->size[1];
      for (r = 0; r < m; r++) {
        if (r + 1 > m) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, m, &hc_emlrtBCI, &st);
        }
        Ntot = closedClasses_data[r];
        c_loop_ub = (b_loop_ub / 2) << 1;
        XN_tmp = c_loop_ub - 2;
        for (ist = 0; ist <= XN_tmp; ist += 2) {
          if (ist + 1 > b_loop_ub) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, b_loop_ub, &bd_emlrtBCI,
                                          &st);
          }
          if (((int32_T)Ntot < 1) || ((int32_T)Ntot > loop_ub)) {
            emlrtDynamicBoundsCheckR2012b((int32_T)Ntot, 1, loop_ub,
                                          &bd_emlrtBCI, &st);
          }
          if (((int32_T)Ntot < 1) || ((int32_T)Ntot > b_lG->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)Ntot, 1, b_lG->size[1],
                                          &ad_emlrtBCI, &st);
          }
          n = S->size[0] * S->size[1];
          if (ist + 1 > n) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, n, &cd_emlrtBCI, &st);
          }
          if (ist + 1 > UN->size[0]) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, UN->size[0], &dd_emlrtBCI,
                                          &st);
          }
          if (((int32_T)Ntot < 1) || ((int32_T)Ntot > UN->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)Ntot, 1, UN->size[1],
                                          &dd_emlrtBCI, &st);
          }
          _mm_storeu_pd(
              &UN_data[ist + UN->size[0] * ((int32_T)Ntot - 1)],
              _mm_div_pd(
                  _mm_mul_pd(
                      _mm_set1_pd(lG_data[(int32_T)Ntot - 1]),
                      _mm_loadu_pd(
                          &L_data[ist + L->size[0] * ((int32_T)Ntot - 1)])),
                  _mm_loadu_pd(&S_data[ist])));
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&st);
          }
        }
        for (ist = c_loop_ub; ist < b_loop_ub; ist++) {
          if (((int32_T)Ntot < 1) || ((int32_T)Ntot > b_lG->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)Ntot, 1, b_lG->size[1],
                                          &jc_emlrtBCI, &st);
          }
          if (ist + 1 > b_loop_ub) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, b_loop_ub, &kc_emlrtBCI,
                                          &st);
          }
          if (((int32_T)Ntot < 1) || ((int32_T)Ntot > loop_ub)) {
            emlrtDynamicBoundsCheckR2012b((int32_T)Ntot, 1, loop_ub,
                                          &lc_emlrtBCI, &st);
          }
          XN_tmp = S->size[0] * S->size[1];
          if (ist + 1 > XN_tmp) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, XN_tmp, &mc_emlrtBCI,
                                          &st);
          }
          if (ist + 1 > UN->size[0]) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, UN->size[0], &nc_emlrtBCI,
                                          &st);
          }
          if (((int32_T)Ntot < 1) || ((int32_T)Ntot > UN->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)Ntot, 1, UN->size[1],
                                          &oc_emlrtBCI, &st);
          }
          UN_data[ist + UN->size[0] * ((int32_T)Ntot - 1)] =
              lG_data[(int32_T)Ntot - 1] *
              L_data[ist + L->size[0] * ((int32_T)Ntot - 1)] / S_data[ist];
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&st);
          }
        }
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b(&st);
        }
      }
      for (r = 0; r < e_loop_ub; r++) {
        if (r + 1 > e_loop_ub) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, e_loop_ub, &ic_emlrtBCI, &st);
        }
        Ntot = openClasses_data[r];
        m = (b_loop_ub / 2) << 1;
        XN_tmp = m - 2;
        for (ist = 0; ist <= XN_tmp; ist += 2) {
          if (ist + 1 > b_loop_ub) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, b_loop_ub, &wc_emlrtBCI,
                                          &st);
          }
          if (((int32_T)Ntot < 1) || ((int32_T)Ntot > loop_ub)) {
            emlrtDynamicBoundsCheckR2012b((int32_T)Ntot, 1, loop_ub,
                                          &wc_emlrtBCI, &st);
          }
          if (((int32_T)Ntot < 1) || ((int32_T)Ntot > lambda->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)Ntot, 1, lambda->size[1],
                                          &vc_emlrtBCI, &st);
          }
          n = S->size[0] * S->size[1];
          if (ist + 1 > n) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, n, &xc_emlrtBCI, &st);
          }
          if (ist + 1 > UN->size[0]) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, UN->size[0], &yc_emlrtBCI,
                                          &st);
          }
          if (((int32_T)Ntot < 1) || ((int32_T)Ntot > UN->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)Ntot, 1, UN->size[1],
                                          &yc_emlrtBCI, &st);
          }
          _mm_storeu_pd(
              &UN_data[ist + UN->size[0] * ((int32_T)Ntot - 1)],
              _mm_div_pd(
                  _mm_mul_pd(
                      _mm_set1_pd(lambda_data[(int32_T)Ntot - 1]),
                      _mm_loadu_pd(
                          &L_data[ist + L->size[0] * ((int32_T)Ntot - 1)])),
                  _mm_loadu_pd(&S_data[ist])));
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&st);
          }
        }
        for (ist = m; ist < b_loop_ub; ist++) {
          if (((int32_T)Ntot < 1) || ((int32_T)Ntot > lambda->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)Ntot, 1, lambda->size[1],
                                          &pc_emlrtBCI, &st);
          }
          if (ist + 1 > b_loop_ub) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, b_loop_ub, &qc_emlrtBCI,
                                          &st);
          }
          if (((int32_T)Ntot < 1) || ((int32_T)Ntot > loop_ub)) {
            emlrtDynamicBoundsCheckR2012b((int32_T)Ntot, 1, loop_ub,
                                          &rc_emlrtBCI, &st);
          }
          n = S->size[0] * S->size[1];
          if (ist + 1 > n) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, n, &sc_emlrtBCI, &st);
          }
          if (ist + 1 > UN->size[0]) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, UN->size[0], &tc_emlrtBCI,
                                          &st);
          }
          if (((int32_T)Ntot < 1) || ((int32_T)Ntot > UN->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)Ntot, 1, UN->size[1],
                                          &uc_emlrtBCI, &st);
          }
          UN_data[ist + UN->size[0] * ((int32_T)Ntot - 1)] =
              lambda_data[(int32_T)Ntot - 1] *
              L_data[ist + L->size[0] * ((int32_T)Ntot - 1)] / S_data[ist];
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&st);
          }
        }
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b(&st);
        }
      }
      n = XN->size[0] * XN->size[1];
      XN->size[0] = 1;
      XN_tmp = b_lG->size[1];
      XN->size[1] = b_lG->size[1];
      emxEnsureCapacity_real_T(sp, XN, n, &oc_emlrtRTEI);
      XN_data = XN->data;
      for (r = 0; r < XN_tmp; r++) {
        XN_data[r] = lG_data[r];
      }
    } else {
      st.site = &f_emlrtRSI;
      /* LINE_ERROR Display a plain-text error message with file and line info.
       */
      /*  */
      /*    LINE_ERROR(CALLER, MSG, ...) throws an error with CALLER's name and
       */
      /*    message, including the source file and line number, in plain text
       * (no */
      /*    hyperlink). Extra arguments are passed to sprintf to format MSG. */
      /*    Copyright (c) 2012-2026, Imperial College London */
      /*    All rights reserved. */
      b_st.site = &s_emlrtRSI;
      e_error(&b_st);
    }
    emxFree_boolean_T(sp, &d_lG);
  } else {
    st.site = &g_emlrtRSI;
    pfqn_mvald(&st, L, N, Z, mu, XN, QN, UN, CN, b_lG);
    lG_data = b_lG->data;
    if (b_lG->size[1] < 1) {
      emlrtDynamicBoundsCheckR2012b(b_lG->size[1], 1, b_lG->size[1],
                                    &r_emlrtBCI, (emlrtConstCTX)sp);
    }
    c_lG = lG_data[b_lG->size[1] - 1];
  }
  emxFree_real_T(sp, &b_S);
  emxFree_int32_T(sp, &ia);
  emxFree_int32_T(sp, &ii);
  emxFree_real_T(sp, &closedClasses);
  emxFree_real_T(sp, &openClasses);
  emxFree_real_T(sp, &a__1);
  emxFree_real_T(sp, &b_mu);
  emxFree_real_T(sp, &y);
  emxFree_real_T(sp, &b_lG);
  emxFree_real_T(sp, &mu);
  *lG = c_lG;
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (pfqn_mvams.c) */
