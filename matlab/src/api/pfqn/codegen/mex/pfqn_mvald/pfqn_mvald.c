/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mvald.c
 *
 * Code generation for function 'pfqn_mvald'
 *
 */

/* Include files */
#include "pfqn_mvald.h"
#include "any.h"
#include "div.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_mvald_data.h"
#include "pfqn_mvald_emxutil.h"
#include "pfqn_mvald_types.h"
#include "pprod.h"
#include "prod.h"
#include "repmat.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "sumMatrixIncludeNaN.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>

/* Variable Definitions */
static emlrtRSInfo emlrtRSI =
    {
        120,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo b_emlrtRSI =
    {
        117,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo c_emlrtRSI =
    {
        116,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo d_emlrtRSI =
    {
        114,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo e_emlrtRSI =
    {
        111,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo f_emlrtRSI =
    {
        106,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo g_emlrtRSI =
    {
        105,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo h_emlrtRSI =
    {
        104,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo i_emlrtRSI =
    {
        100,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo j_emlrtRSI =
    {
        97,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo k_emlrtRSI =
    {
        95,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo l_emlrtRSI =
    {
        94,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo m_emlrtRSI =
    {
        86,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo n_emlrtRSI =
    {
        78,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo o_emlrtRSI =
    {
        66,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo p_emlrtRSI =
    {
        73,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo q_emlrtRSI =
    {
        71,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo r_emlrtRSI =
    {
        63,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo s_emlrtRSI =
    {
        62,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo t_emlrtRSI =
    {
        56,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo u_emlrtRSI =
    {
        53,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo v_emlrtRSI =
    {
        51,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo w_emlrtRSI =
    {
        50,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo x_emlrtRSI =
    {
        39,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo rb_emlrtRSI =
    {
        18,            /* lineNo */
        "ifWhileCond", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "ifWhileCond.m" /* pathName */
};

static emlrtRSInfo sb_emlrtRSI =
    {
        31,            /* lineNo */
        "checkNoNaNs", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "ifWhileCond.m" /* pathName */
};

static emlrtRSInfo tb_emlrtRSI =
    {
        128,       /* lineNo */
        "hashpop", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo ub_emlrtRSI = {
    107,                /* lineNo */
    "blockedSummation", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo vb_emlrtRSI = {
    21,                                                           /* lineNo */
    "pprod",                                                      /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/pprod.m" /* pathName */
};

static emlrtRSInfo wb_emlrtRSI = {
    15,    /* lineNo */
    "sum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sum.m" /* pathName
                                                                     */
};

static emlrtRSInfo cc_emlrtRSI =
    {
        34,               /* lineNo */
        "rdivide_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "rdivide_helper.m" /* pathName */
};

static emlrtRSInfo dc_emlrtRSI = {
    53,    /* lineNo */
    "div", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/div.m" /* pathName
                                                                       */
};

static emlrtRTEInfo emlrtRTEI = {
    14,                                                           /* lineNo */
    9,                                                            /* colNo */
    "log",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/log.m" /* pName */
};

static emlrtRTEInfo b_emlrtRTEI =
    {
        13,               /* lineNo */
        13,               /* colNo */
        "toLogicalCheck", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "toLogicalCheck.m" /* pName */
};

static emlrtRTEInfo c_emlrtRTEI = {
    13,                     /* lineNo */
    27,                     /* colNo */
    "assertCompatibleDims", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/shared/coder/coder/lib/+coder/+internal/"
    "assertCompatibleDims.m" /* pName */
};

static emlrtECInfo emlrtECI = {
    2,                                                            /* nDims */
    21,                                                           /* lineNo */
    12,                                                           /* colNo */
    "pprod",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/pprod.m" /* pName */
};

static emlrtRTEInfo e_emlrtRTEI =
    {
        134,                  /* lineNo */
        9,                    /* colNo */
        "last_nonzero_index", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtDCInfo emlrtDCI =
    {
        50,           /* lineNo */
        12,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        4               /* checkKind */
};

static emlrtDCInfo b_emlrtDCI =
    {
        50,           /* lineNo */
        12,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtDCInfo c_emlrtDCI =
    {
        51,           /* lineNo */
        11,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        4               /* checkKind */
};

static emlrtDCInfo d_emlrtDCI =
    {
        51,           /* lineNo */
        11,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtDCInfo e_emlrtDCI =
    {
        51,           /* lineNo */
        20,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        4               /* checkKind */
};

static emlrtDCInfo f_emlrtDCI =
    {
        51,           /* lineNo */
        20,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtDCInfo g_emlrtDCI =
    {
        54,           /* lineNo */
        16,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        4               /* checkKind */
};

static emlrtDCInfo h_emlrtDCI =
    {
        54,           /* lineNo */
        16,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtRTEInfo f_emlrtRTEI =
    {
        62,           /* lineNo */
        23,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtBCInfo emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        66,           /* lineNo */
        52,           /* colNo */
        "WN",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtRTEInfo g_emlrtRTEI =
    {
        71,           /* lineNo */
        11,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtBCInfo b_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        86,           /* lineNo */
        23,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo i_emlrtDCI =
    {
        86,           /* lineNo */
        40,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo c_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        86,           /* lineNo */
        40,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo d_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        95,           /* lineNo */
        24,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo e_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        97,           /* lineNo */
        24,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo f_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        100,          /* lineNo */
        20,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo g_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        105,          /* lineNo */
        30,           /* colNo */
        "n",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo h_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        105,          /* lineNo */
        32,           /* colNo */
        "n",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo i_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        105,          /* lineNo */
        54,           /* colNo */
        "N",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo j_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        105,          /* lineNo */
        56,           /* colNo */
        "N",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo k_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        105,          /* lineNo */
        79,           /* colNo */
        "n",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo l_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        105,          /* lineNo */
        91,           /* colNo */
        "n",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo m_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        113,          /* lineNo */
        11,           /* colNo */
        "lGN",        /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo n_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        113,          /* lineNo */
        13,           /* colNo */
        "lGN",        /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo j_emlrtDCI =
    {
        114,          /* lineNo */
        8,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo o_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        114,          /* lineNo */
        8,            /* colNo */
        "Xs",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo k_emlrtDCI =
    {
        116,          /* lineNo */
        11,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo p_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        116,          /* lineNo */
        11,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo b_emlrtECI =
    {
        1,            /* nDims */
        117,          /* lineNo */
        6,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtECInfo c_emlrtECI =
    {
        2,            /* nDims */
        117,          /* lineNo */
        6,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtBCInfo q_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        119,          /* lineNo */
        13,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo d_emlrtECI =
    {
        2,            /* nDims */
        120,          /* lineNo */
        6,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtBCInfo r_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        59,           /* lineNo */
        14,           /* colNo */
        "n",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo s_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        61,           /* lineNo */
        20,           /* colNo */
        "WN",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo t_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        61,           /* lineNo */
        24,           /* colNo */
        "WN",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo u_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        73,           /* lineNo */
        16,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo v_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        73,           /* lineNo */
        20,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo w_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        73,           /* lineNo */
        26,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo l_emlrtDCI =
    {
        73,           /* lineNo */
        26,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo x_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        76,           /* lineNo */
        18,           /* colNo */
        "n",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo y_emlrtBCI =
    {
        -1,                   /* iFirst */
        -1,                   /* iLast */
        135,                  /* lineNo */
        10,                   /* colNo */
        "v",                  /* aName */
        "last_nonzero_index", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ab_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        86,           /* lineNo */
        27,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo m_emlrtDCI =
    {
        86,           /* lineNo */
        27,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo bb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        100,          /* lineNo */
        16,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo cb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        100,          /* lineNo */
        26,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo n_emlrtDCI =
    {
        100,          /* lineNo */
        26,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
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

static emlrtBCInfo eb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        66,           /* lineNo */
        34,           /* colNo */
        "n",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo fb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        66,           /* lineNo */
        40,           /* colNo */
        "Z",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo gb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        66,           /* lineNo */
        16,           /* colNo */
        "Xs",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo hb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        66,           /* lineNo */
        18,           /* colNo */
        "Xs",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo o_emlrtDCI =
    {
        66,           /* lineNo */
        18,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo ib_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        34,           /* colNo */
        "WN",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo jb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        38,           /* colNo */
        "WN",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo kb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        44,           /* colNo */
        "L",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo lb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        48,           /* colNo */
        "L",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo mb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        54,           /* colNo */
        "mu",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo nb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        58,           /* colNo */
        "mu",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ob_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        67,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo pb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        71,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo qb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        79,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo p_emlrtDCI =
    {
        63,           /* lineNo */
        79,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo rb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        24,           /* colNo */
        "WN",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo sb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        28,           /* colNo */
        "WN",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo tb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        97,           /* lineNo */
        20,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ub_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        97,           /* lineNo */
        30,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo q_emlrtDCI =
    {
        97,           /* lineNo */
        30,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo vb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        95,           /* lineNo */
        20,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo wb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        95,           /* lineNo */
        30,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo r_emlrtDCI =
    {
        95,           /* lineNo */
        30,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo xb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        53,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo yb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        57,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ac_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        63,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo s_emlrtDCI =
    {
        78,           /* lineNo */
        63,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo bc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        82,           /* colNo */
        "L",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo cc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        86,           /* colNo */
        "L",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo dc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        92,           /* colNo */
        "mu",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ec_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        96,           /* colNo */
        "mu",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo fc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        103,          /* colNo */
        "Xs",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo gc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        105,          /* colNo */
        "Xs",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo t_emlrtDCI =
    {
        78,           /* lineNo */
        105,          /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo hc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        122,          /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ic_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        126,          /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo jc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        134,          /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo u_emlrtDCI =
    {
        78,           /* lineNo */
        134,          /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo kc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        24,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo lc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        28,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo mc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        34,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo v_emlrtDCI =
    {
        78,           /* lineNo */
        34,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo nc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        106,          /* lineNo */
        23,           /* colNo */
        "Xs",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo oc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        106,          /* lineNo */
        32,           /* colNo */
        "Xs",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo w_emlrtDCI =
    {
        106,          /* lineNo */
        32,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo pc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        108,          /* lineNo */
        26,           /* colNo */
        "lGN",        /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo qc_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        108,          /* lineNo */
        13,           /* colNo */
        "lGN",        /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo rc_emlrtBCI = {
    -1,                                                            /* iFirst */
    -1,                                                            /* iLast */
    27,                                                            /* lineNo */
    20,                                                            /* colNo */
    "n",                                                           /* aName */
    "pprod",                                                       /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/pprod.m", /* pName */
    0 /* checkKind */
};

static emlrtBCInfo sc_emlrtBCI = {
    -1,                                                            /* iFirst */
    -1,                                                            /* iLast */
    27,                                                            /* lineNo */
    26,                                                            /* colNo */
    "N",                                                           /* aName */
    "pprod",                                                       /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/pprod.m", /* pName */
    0 /* checkKind */
};

static emlrtBCInfo tc_emlrtBCI = {
    -1,                                                            /* iFirst */
    -1,                                                            /* iLast */
    28,                                                            /* lineNo */
    11,                                                            /* colNo */
    "n",                                                           /* aName */
    "pprod",                                                       /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/pprod.m", /* pName */
    0 /* checkKind */
};

static emlrtBCInfo uc_emlrtBCI = {
    -1,                                                            /* iFirst */
    -1,                                                            /* iLast */
    35,                                                            /* lineNo */
    12,                                                            /* colNo */
    "n",                                                           /* aName */
    "pprod",                                                       /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/pprod.m", /* pName */
    0 /* checkKind */
};

static emlrtBCInfo vc_emlrtBCI =
    {
        -1,        /* iFirst */
        -1,        /* iLast */
        128,       /* lineNo */
        16,        /* colNo */
        "N",       /* aName */
        "hashpop", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo wc_emlrtBCI =
    {
        -1,        /* iFirst */
        -1,        /* iLast */
        128,       /* lineNo */
        18,        /* colNo */
        "N",       /* aName */
        "hashpop", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo xc_emlrtBCI =
    {
        -1,        /* iFirst */
        -1,        /* iLast */
        128,       /* lineNo */
        28,        /* colNo */
        "n",       /* aName */
        "hashpop", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtRTEInfo k_emlrtRTEI =
    {
        31,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo l_emlrtRTEI =
    {
        32,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo m_emlrtRTEI =
    {
        33,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo n_emlrtRTEI =
    {
        34,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo o_emlrtRTEI =
    {
        35,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo p_emlrtRTEI =
    {
        36,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo q_emlrtRTEI =
    {
        37,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo r_emlrtRTEI =
    {
        39,           /* lineNo */
        8,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo s_emlrtRTEI =
    {
        50,           /* lineNo */
        17,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo t_emlrtRTEI =
    {
        50,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo u_emlrtRTEI =
    {
        51,           /* lineNo */
        25,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo v_emlrtRTEI =
    {
        51,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo w_emlrtRTEI =
    {
        52,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo x_emlrtRTEI =
    {
        54,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo y_emlrtRTEI =
    {
        56,           /* lineNo */
        7,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo ab_emlrtRTEI =
    {
        113,          /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo bb_emlrtRTEI =
    {
        114,          /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo cb_emlrtRTEI =
    {
        115,          /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo db_emlrtRTEI = {
    28,                                                           /* lineNo */
    9,                                                            /* colNo */
    "colon",                                                      /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/colon.m" /* pName */
};

static emlrtRTEInfo eb_emlrtRTEI =
    {
        116,          /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo fb_emlrtRTEI = {
    1,                                                           /* lineNo */
    19,                                                          /* colNo */
    "oner",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/oner.m" /* pName */
};

static emlrtRTEInfo gb_emlrtRTEI =
    {
        66,           /* lineNo */
        47,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo hb_emlrtRTEI =
    {
        115,          /* lineNo */
        4,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo ib_emlrtRTEI =
    {
        105,          /* lineNo */
        28,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo jb_emlrtRTEI =
    {
        86,           /* lineNo */
        20,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo kb_emlrtRTEI =
    {
        105,          /* lineNo */
        52,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo lb_emlrtRTEI =
    {
        117,          /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo mb_emlrtRTEI =
    {
        119,          /* lineNo */
        6,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo nb_emlrtRTEI =
    {
        105,          /* lineNo */
        76,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo pb_emlrtRTEI =
    {
        119,          /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo qb_emlrtRTEI =
    {
        111,          /* lineNo */
        5,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo sb_emlrtRTEI =
    {
        120,          /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo tb_emlrtRTEI =
    {
        26,           /* lineNo */
        39,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo ub_emlrtRTEI =
    {
        63,           /* lineNo */
        87,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo vb_emlrtRTEI =
    {
        128,          /* lineNo */
        14,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo yb_emlrtRTEI =
    {
        117,          /* lineNo */
        6,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRSInfo ec_emlrtRSI = {
    54,    /* lineNo */
    "div", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/div.m" /* pathName
                                                                       */
};

/* Function Declarations */
static void binary_expand_op(const emlrtStack *sp, emxArray_real_T *in1,
                             const emxArray_real_T *in2,
                             const emxArray_real_T *in3);

static real_T hashpop(const emlrtStack *sp, const emxArray_real_T *n,
                      const emxArray_real_T *N);

static void times(const emlrtStack *sp, emxArray_real_T *in1,
                  const emxArray_real_T *in2);

/* Function Definitions */
static void binary_expand_op(const emlrtStack *sp, emxArray_real_T *in1,
                             const emxArray_real_T *in2,
                             const emxArray_real_T *in3)
{
  jmp_buf *volatile emlrtJBStack;
  const real_T *in2_data;
  const real_T *in3_data;
  real_T *in1_data;
  int32_T binary_expand_op_numThreads;
  int32_T i;
  int32_T loop_ub;
  int32_T stride_0_1;
  int32_T stride_1_1;
  in3_data = in3->data;
  in2_data = in2->data;
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[0] = 1;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &sb_emlrtRTEI);
  if (in3->size[1] == 1) {
    loop_ub = in2->size[1];
  } else {
    loop_ub = in3->size[1];
  }
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &sb_emlrtRTEI);
  in1_data = in1->data;
  stride_0_1 = (in2->size[1] != 1);
  stride_1_1 = (in3->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      in1_data[in1->size[0] * i] =
          in2_data[i * stride_0_1] - in3_data[i * stride_1_1];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    binary_expand_op_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(binary_expand_op_numThreads)

    for (i = 0; i < loop_ub; i++) {
      in1_data[in1->size[0] * i] =
          in2_data[i * stride_0_1] - in3_data[i * stride_1_1];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
}

static real_T hashpop(const emlrtStack *sp, const emxArray_real_T *n,
                      const emxArray_real_T *N)
{
  emlrtStack st;
  emxArray_real_T *b_N;
  const real_T *N_data;
  const real_T *n_data;
  real_T i;
  real_T *b_N_data;
  int32_T b_i;
  int32_T i1;
  int32_T r;
  st.prev = sp;
  st.tls = sp->tls;
  N_data = N->data;
  n_data = n->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /*  I=HASHPOP(N,N) */
  i = 1.0;
  /*  index of the empty population */
  b_i = N->size[1];
  emxInit_real_T(sp, &b_N, 2, &vb_emlrtRTEI);
  for (r = 0; r < b_i; r++) {
    int32_T loop_ub;
    int32_T scalarLB;
    int32_T vectorUB;
    if (r < 1) {
      loop_ub = 0;
    } else {
      if (b_i < 1) {
        emlrtDynamicBoundsCheckR2012b(1, 1, b_i, &vc_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (r > b_i) {
        emlrtDynamicBoundsCheckR2012b(r, 1, b_i, &wc_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      loop_ub = r;
    }
    scalarLB = b_N->size[0] * b_N->size[1];
    b_N->size[0] = 1;
    b_N->size[1] = loop_ub;
    emxEnsureCapacity_real_T(sp, b_N, scalarLB, &vb_emlrtRTEI);
    b_N_data = b_N->data;
    scalarLB = (loop_ub / 2) << 1;
    vectorUB = scalarLB - 2;
    for (i1 = 0; i1 <= vectorUB; i1 += 2) {
      _mm_storeu_pd(&b_N_data[i1],
                    _mm_add_pd(_mm_loadu_pd(&N_data[i1]), _mm_set1_pd(1.0)));
    }
    for (i1 = scalarLB; i1 < loop_ub; i1++) {
      b_N_data[i1] = N_data[i1] + 1.0;
    }
    if (r + 1 > n->size[1]) {
      emlrtDynamicBoundsCheckR2012b(r + 1, 1, n->size[1], &xc_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    st.site = &tb_emlrtRSI;
    i += prod(&st, b_N) * n_data[r];
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  emxFree_real_T(sp, &b_N);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return i;
}

static void times(const emlrtStack *sp, emxArray_real_T *in1,
                  const emxArray_real_T *in2)
{
  emxArray_real_T *b_in2;
  const real_T *in2_data;
  real_T *b_in2_data;
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
  in2_data = in2->data;
  in1_data = in1->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &b_in2, 2, &yb_emlrtRTEI);
  if (in1->size[0] == 1) {
    loop_ub = in2->size[0];
  } else {
    loop_ub = in1->size[0];
  }
  stride_0_0 = b_in2->size[0] * b_in2->size[1];
  b_in2->size[0] = loop_ub;
  if (in1->size[1] == 1) {
    b_loop_ub = in2->size[1];
  } else {
    b_loop_ub = in1->size[1];
  }
  b_in2->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(sp, b_in2, stride_0_0, &yb_emlrtRTEI);
  b_in2_data = b_in2->data;
  stride_0_0 = (in2->size[0] != 1);
  stride_0_1 = (in2->size[1] != 1);
  stride_1_0 = (in1->size[0] != 1);
  stride_1_1 = (in1->size[1] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_in2_data[i1 + b_in2->size[0] * i] =
          in2_data[i1 * stride_0_0 + in2->size[0] * aux_0_1] *
          in1_data[i1 * stride_1_0 + in1->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_1;
    aux_0_1 += stride_0_1;
  }
  stride_0_0 = in1->size[0] * in1->size[1];
  in1->size[0] = loop_ub;
  in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_0, &yb_emlrtRTEI);
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

void pfqn_mvald(const emlrtStack *sp, const emxArray_real_T *L,
                const emxArray_real_T *N, const emxArray_real_T *Z,
                const emxArray_real_T *mu, real_T stabilize,
                emxArray_real_T *XN, emxArray_real_T *QN, emxArray_real_T *UN,
                emxArray_real_T *CN, emxArray_real_T *lGN,
                boolean_T *isNumStable, emxArray_real_T *pi)
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack g_st;
  emlrtStack st;
  emxArray_boolean_T *b_N;
  emxArray_real_T *WN;
  emxArray_real_T *X;
  emxArray_real_T *Xs;
  emxArray_real_T *b_n;
  emxArray_real_T *b_pi;
  emxArray_real_T *c_N;
  emxArray_real_T *d_N;
  emxArray_real_T *n;
  real_T dv[2];
  const real_T *L_data;
  const real_T *N_data;
  const real_T *Z_data;
  const real_T *mu_data;
  real_T *WN_data;
  real_T *XN_data;
  real_T *X_data;
  real_T *b_pi_data;
  real_T *c_N_data;
  real_T *lGN_data;
  real_T *n_data;
  real_T *pi_data;
  int32_T M;
  int32_T R;
  int32_T b_ist;
  int32_T c_loop_ub;
  int32_T d_loop_ub;
  int32_T i;
  int32_T ist;
  int32_T k;
  int32_T loop_ub;
  int32_T nz;
  int32_T pfqn_mvald_numThreads;
  int32_T s;
  int32_T vlen;
  boolean_T b_isNumStable;
  boolean_T *b_N_data;
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
  mu_data = mu->data;
  Z_data = Z->data;
  N_data = N->data;
  L_data = L->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /* { */
  /*  % @file pfqn_mvald.m */
  /*  % @brief Exact MVA for load-dependent closed queueing networks. */
  /* } */
  /* { */
  /*  % @brief Exact MVA for load-dependent closed queueing networks. */
  /*  % @fn pfqn_mvald(L, N, Z, mu, stabilize) */
  /*  % @param L Service demand matrix. */
  /*  % @param N Population vector. */
  /*  % @param Z Think time vector. */
  /*  % @param mu Load-dependent rate matrix (MxNt). */
  /*  % @param stabilize Force non-negative probabilities (default: true). */
  /*  % @return XN System throughput. */
  /*  % @return QN Mean queue lengths. */
  /*  % @return UN Utilization. */
  /*  % @return CN Cycle times. */
  /*  % @return lGN Logarithm of normalizing constant evolution. */
  /*  % @return isNumStable Numerical stability flag. */
  /*  % @return pi Marginal queue-length probabilities. */
  /* } */
  /*  [XN,QN,UN,CN,LGN]=PFQN_MVALD(L,N,Z,MU) */
  R = L->size[1];
  M = L->size[0];
  /*  get number of queues (M) and classes (R) */
  nz = XN->size[0] * XN->size[1];
  XN->size[0] = L->size[0];
  XN->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, XN, nz, &k_emlrtRTEI);
  XN_data = XN->data;
  vlen = L->size[0] * L->size[1];
  for (ist = 0; ist < vlen; ist++) {
    XN_data[ist] = 0.0;
  }
  nz = QN->size[0] * QN->size[1];
  QN->size[0] = L->size[0];
  QN->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, QN, nz, &l_emlrtRTEI);
  XN_data = QN->data;
  for (ist = 0; ist < vlen; ist++) {
    XN_data[ist] = 0.0;
  }
  nz = UN->size[0] * UN->size[1];
  UN->size[0] = L->size[0];
  UN->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, UN, nz, &m_emlrtRTEI);
  XN_data = UN->data;
  for (ist = 0; ist < vlen; ist++) {
    XN_data[ist] = 0.0;
  }
  nz = CN->size[0] * CN->size[1];
  CN->size[0] = L->size[0];
  CN->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, CN, nz, &n_emlrtRTEI);
  XN_data = CN->data;
  for (ist = 0; ist < vlen; ist++) {
    XN_data[ist] = 0.0;
  }
  emxInit_real_T(sp, &WN, 2, &o_emlrtRTEI);
  nz = WN->size[0] * WN->size[1];
  WN->size[0] = L->size[0];
  WN->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, WN, nz, &o_emlrtRTEI);
  WN_data = WN->data;
  for (ist = 0; ist < vlen; ist++) {
    WN_data[ist] = 0.0;
  }
  nz = lGN->size[0] * lGN->size[1];
  lGN->size[0] = 1;
  lGN->size[1] = 1;
  emxEnsureCapacity_real_T(sp, lGN, nz, &p_emlrtRTEI);
  lGN_data = lGN->data;
  lGN_data[0] = rtMinusInf;
  nz = pi->size[0] * pi->size[1];
  pi->size[0] = L->size[0];
  pi->size[1] = 1;
  emxEnsureCapacity_real_T(sp, pi, nz, &q_emlrtRTEI);
  pi_data = pi->data;
  for (ist = 0; ist < M; ist++) {
    pi_data[ist] = 0.0;
  }
  emxInit_boolean_T(sp, &b_N, &r_emlrtRTEI);
  nz = b_N->size[0] * b_N->size[1];
  b_N->size[0] = 1;
  loop_ub = N->size[1];
  b_N->size[1] = N->size[1];
  emxEnsureCapacity_boolean_T(sp, b_N, nz, &r_emlrtRTEI);
  b_N_data = b_N->data;
  vlen = N->size[1];
  if (N->size[1] < 1600) {
    for (i = 0; i < loop_ub; i++) {
      b_N_data[i] = (N_data[i] < 0.0);
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    pfqn_mvald_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mvald_numThreads)

    for (i = 0; i < vlen; i++) {
      b_N_data[i] = (N_data[i] < 0.0);
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  st.site = &x_emlrtRSI;
  if (any(&st, b_N)) {
    b_isNumStable = true;
  } else {
    __m128d r;
    real_T numLGTerms;
    real_T p0;
    int32_T b_vectorUB;
    int32_T last_nnz;
    int32_T scalarLB;
    int32_T vectorUB;
    uint32_T lgIdx;
    boolean_T warn;
    /*  stabilize ensures that probabilities do not become negative */
    warn = true;
    b_isNumStable = true;
    emxInit_real_T(sp, &c_N, 2, &s_emlrtRTEI);
    nz = c_N->size[0] * c_N->size[1];
    c_N->size[0] = 1;
    c_N->size[1] = loop_ub;
    emxEnsureCapacity_real_T(sp, c_N, nz, &s_emlrtRTEI);
    XN_data = c_N->data;
    scalarLB = (N->size[1] / 2) << 1;
    vlen = scalarLB - 2;
    for (ist = 0; ist <= vlen; ist += 2) {
      _mm_storeu_pd(&XN_data[ist],
                    _mm_add_pd(_mm_loadu_pd(&N_data[ist]), _mm_set1_pd(1.0)));
    }
    for (ist = scalarLB; ist < loop_ub; ist++) {
      XN_data[ist] = N_data[ist] + 1.0;
    }
    st.site = &w_emlrtRSI;
    numLGTerms = prod(&st, c_N);
    if (!(numLGTerms >= 0.0)) {
      emlrtNonNegativeCheckR2012b(numLGTerms, &emlrtDCI, (emlrtConstCTX)sp);
    }
    if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
      emlrtIntegerCheckR2012b(numLGTerms, &b_emlrtDCI, (emlrtConstCTX)sp);
    }
    emxInit_real_T(sp, &Xs, 2, &t_emlrtRTEI);
    nz = Xs->size[0] * Xs->size[1];
    Xs->size[0] = R;
    Xs->size[1] = (int32_T)numLGTerms;
    emxEnsureCapacity_real_T(sp, Xs, nz, &t_emlrtRTEI);
    pi_data = Xs->data;
    vlen = L->size[1] * (int32_T)numLGTerms;
    for (ist = 0; ist < vlen; ist++) {
      pi_data[ist] = 0.0;
    }
    /*  throughput for a model with station i less */
    st.site = &v_emlrtRSI;
    numLGTerms = sum(&st, N) + 1.0;
    if (!(numLGTerms >= 0.0)) {
      emlrtNonNegativeCheckR2012b(numLGTerms, &c_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
      emlrtIntegerCheckR2012b(numLGTerms, &d_emlrtDCI, (emlrtConstCTX)sp);
    }
    nz = c_N->size[0] * c_N->size[1];
    c_N->size[0] = 1;
    c_N->size[1] = loop_ub;
    emxEnsureCapacity_real_T(sp, c_N, nz, &u_emlrtRTEI);
    XN_data = c_N->data;
    vlen = scalarLB - 2;
    for (ist = 0; ist <= vlen; ist += 2) {
      _mm_storeu_pd(&XN_data[ist],
                    _mm_add_pd(_mm_loadu_pd(&N_data[ist]), _mm_set1_pd(1.0)));
    }
    for (ist = scalarLB; ist < loop_ub; ist++) {
      XN_data[ist] = N_data[ist] + 1.0;
    }
    st.site = &v_emlrtRSI;
    p0 = prod(&st, c_N);
    if (!(p0 >= 0.0)) {
      emlrtNonNegativeCheckR2012b(p0, &e_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (p0 != (int32_T)muDoubleScalarFloor(p0)) {
      emlrtIntegerCheckR2012b(p0, &f_emlrtDCI, (emlrtConstCTX)sp);
    }
    emxInit_real_T(sp, &b_pi, 3, &tb_emlrtRTEI);
    nz = b_pi->size[0] * b_pi->size[1] * b_pi->size[2];
    b_pi->size[0] = M;
    b_pi->size[1] = (int32_T)numLGTerms;
    b_pi->size[2] = (int32_T)p0;
    emxEnsureCapacity_real_T(sp, b_pi, nz, &v_emlrtRTEI);
    b_pi_data = b_pi->data;
    vlen = L->size[0] * (int32_T)numLGTerms * (int32_T)p0;
    for (ist = 0; ist < vlen; ist++) {
      b_pi_data[ist] = 1.0;
    }
    /*  marginal queue-length probabilities pi(k) */
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
    emxInit_real_T(sp, &n, 2, &w_emlrtRTEI);
    nz = n->size[0] * n->size[1];
    n->size[0] = 1;
    n->size[1] = N->size[1];
    emxEnsureCapacity_real_T(sp, n, nz, &w_emlrtRTEI);
    n_data = n->data;
    vlen = N->size[1];
    for (ist = 0; ist < vlen; ist++) {
      n_data[ist] = 0.0;
    }
    /*  initialize the current population */
    st.site = &u_emlrtRSI;
    numLGTerms = sum(&st, N) + 1.0;
    if (!(numLGTerms >= 0.0)) {
      emlrtNonNegativeCheckR2012b(numLGTerms, &g_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
      emlrtIntegerCheckR2012b(numLGTerms, &h_emlrtDCI, (emlrtConstCTX)sp);
    }
    vlen = lGN->size[0] * lGN->size[1];
    lGN->size[0] = 1;
    nz = (int32_T)numLGTerms;
    lGN->size[1] = (int32_T)numLGTerms;
    emxEnsureCapacity_real_T(sp, lGN, vlen, &x_emlrtRTEI);
    lGN_data = lGN->data;
    for (ist = 0; ist < nz; ist++) {
      lGN_data[ist] = 0.0;
    }
    lgIdx = 1U;
    emxInit_real_T(sp, &X, 1, &bb_emlrtRTEI);
    emxInit_real_T(sp, &d_N, 2, &ub_emlrtRTEI);
    c_N_data = d_N->data;
    emxInit_real_T(sp, &b_n, 2, &ib_emlrtRTEI);
    int32_T b_loop_ub;
    int32_T exitg1;
    do {
      boolean_T exitg2;
      boolean_T y;
      exitg1 = 0;
      st.site = &t_emlrtRSI;
      vlen = b_N->size[0] * b_N->size[1];
      b_N->size[0] = 1;
      b_loop_ub = n->size[1];
      b_N->size[1] = n->size[1];
      emxEnsureCapacity_boolean_T(&st, b_N, vlen, &y_emlrtRTEI);
      b_N_data = b_N->data;
      for (ist = 0; ist < b_loop_ub; ist++) {
        b_N_data[ist] = (n_data[ist] != -1.0);
      }
      y = (b_N->size[1] != 0);
      if (y) {
        b_st.site = &rb_emlrtRSI;
        c_st.site = &sb_emlrtRSI;
        if (b_N->size[1] > 2147483646) {
          d_st.site = &bb_emlrtRSI;
          check_forloop_overflow_error(&d_st);
        }
        vlen = 0;
        exitg2 = false;
        while ((!exitg2) && (vlen <= b_N->size[1] - 1)) {
          if (!b_N_data[vlen]) {
            y = false;
            exitg2 = true;
          } else {
            vlen++;
          }
        }
      }
      if (y) {
        real_T d;
        vlen = WN->size[0] * WN->size[1];
        nz = (vlen / 2) << 1;
        vectorUB = nz - 2;
        for (b_ist = 0; b_ist <= vectorUB; b_ist += 2) {
          r = _mm_loadu_pd(&WN_data[b_ist]);
          _mm_storeu_pd(&WN_data[b_ist], _mm_mul_pd(_mm_set1_pd(0.0), r));
        }
        for (b_ist = nz; b_ist < vlen; b_ist++) {
          WN_data[b_ist] *= 0.0;
        }
        for (s = 0; s < R; s++) {
          if (s + 1 > b_loop_ub) {
            emlrtDynamicBoundsCheckR2012b(s + 1, 1, b_loop_ub, &r_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          d = n_data[s];
          if (d > 0.0) {
            for (b_ist = 0; b_ist < M; b_ist++) {
              if (b_ist + 1 > WN->size[0]) {
                emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, WN->size[0],
                                              &s_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (s + 1 > WN->size[1]) {
                emlrtDynamicBoundsCheckR2012b(s + 1, 1, WN->size[1],
                                              &t_emlrtBCI, (emlrtConstCTX)sp);
              }
              WN_data[b_ist + WN->size[0] * s] = 0.0;
              st.site = &s_emlrtRSI;
              numLGTerms = sum(&st, n);
              nz = (int32_T)numLGTerms;
              emlrtForLoopVectorCheckR2021a(1.0, 1.0, numLGTerms,
                                            mxDOUBLE_CLASS, (int32_T)numLGTerms,
                                            &f_emlrtRTEI, (emlrtConstCTX)sp);
              if ((int32_T)numLGTerms - 1 >= 0) {
                d_loop_ub = b_loop_ub;
              }
              for (k = 0; k < nz; k++) {
                st.site = &r_emlrtRSI;
                vlen = d_N->size[0] * d_N->size[1];
                d_N->size[0] = 1;
                d_N->size[1] = b_loop_ub;
                emxEnsureCapacity_real_T(&st, d_N, vlen, &fb_emlrtRTEI);
                c_N_data = d_N->data;
                for (ist = 0; ist < d_loop_ub; ist++) {
                  c_N_data[ist] = n_data[ist];
                }
                /*  N=ONER(N,r) */
                /*  Decrement element in position of r of input vector */
                /*  */
                /*  Copyright (c) 2012-2026, Imperial College London */
                /*  All rights reserved. */
                if (s + 1 > b_loop_ub) {
                  emlrtDynamicBoundsCheckR2012b(s + 1, 1, b_loop_ub,
                                                &db_emlrtBCI, &st);
                }
                c_N_data[s] = d - 1.0;
                if (b_ist + 1 > WN->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, WN->size[0],
                                                &ib_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (s + 1 > WN->size[1]) {
                  emlrtDynamicBoundsCheckR2012b(
                      s + 1, 1, WN->size[1], &jb_emlrtBCI, (emlrtConstCTX)sp);
                }
                if (b_ist + 1 > M) {
                  emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, M, &kb_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (s + 1 > R) {
                  emlrtDynamicBoundsCheckR2012b(s + 1, 1, R, &lb_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (b_ist + 1 > mu->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, mu->size[0],
                                                &mb_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (((int32_T)((uint32_T)k + 1U) < 1) ||
                    ((int32_T)((uint32_T)k + 1U) > mu->size[1])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)k + 1U), 1,
                                                mu->size[1], &nb_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (b_ist + 1 > b_pi->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, b_pi->size[0],
                                                &ob_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (((int32_T)((uint32_T)k + 1U) < 1) ||
                    ((int32_T)((uint32_T)k + 1U) > b_pi->size[1])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)k + 1U), 1,
                                                b_pi->size[1], &pb_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                st.site = &r_emlrtRSI;
                numLGTerms = hashpop(&st, d_N, N);
                if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
                  emlrtIntegerCheckR2012b(numLGTerms, &p_emlrtDCI,
                                          (emlrtConstCTX)sp);
                }
                if (((int32_T)numLGTerms < 1) ||
                    ((int32_T)numLGTerms > b_pi->size[2])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1,
                                                b_pi->size[2], &qb_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (b_ist + 1 > WN->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, WN->size[0],
                                                &rb_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (s + 1 > WN->size[1]) {
                  emlrtDynamicBoundsCheckR2012b(
                      s + 1, 1, WN->size[1], &sb_emlrtBCI, (emlrtConstCTX)sp);
                }
                WN_data[b_ist + WN->size[0] * s] +=
                    L_data[b_ist + L->size[0] * s] /
                    mu_data[b_ist + mu->size[0] * k] * ((real_T)k + 1.0) *
                    b_pi_data[(b_ist + b_pi->size[0] * k) +
                              b_pi->size[0] * b_pi->size[1] *
                                  ((int32_T)numLGTerms - 1)];
                if (*emlrtBreakCheckR2012bFlagVar != 0) {
                  emlrtBreakCheckR2012b((emlrtConstCTX)sp);
                }
              }
              if (*emlrtBreakCheckR2012bFlagVar != 0) {
                emlrtBreakCheckR2012b((emlrtConstCTX)sp);
              }
            }
            st.site = &o_emlrtRSI;
            if (s + 1 > WN->size[1]) {
              emlrtDynamicBoundsCheckR2012b(s + 1, 1, WN->size[1], &emlrtBCI,
                                            &st);
            }
            b_st.site = &gb_emlrtRSI;
            vlen = WN->size[0];
            c_st.site = &db_emlrtRSI;
            d_st.site = &hb_emlrtRSI;
            if (WN->size[0] == 0) {
              numLGTerms = 0.0;
            } else {
              e_st.site = &ub_emlrtRSI;
              vectorUB = X->size[0];
              X->size[0] = WN->size[0];
              emxEnsureCapacity_real_T(&e_st, X, vectorUB, &gb_emlrtRTEI);
              X_data = X->data;
              for (b_ist = 0; b_ist < vlen; b_ist++) {
                X_data[b_ist] = WN_data[b_ist + WN->size[0] * s];
              }
              f_st.site = &jb_emlrtRSI;
              numLGTerms = sumMatrixColumns(&f_st, X, WN->size[0]);
            }
            if (s + 1 > b_loop_ub) {
              emlrtDynamicBoundsCheckR2012b(s + 1, 1, b_loop_ub, &eb_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (s + 1 > Z->size[1]) {
              emlrtDynamicBoundsCheckR2012b(s + 1, 1, Z->size[1], &fb_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (s + 1 > Xs->size[0]) {
              emlrtDynamicBoundsCheckR2012b(s + 1, 1, Xs->size[0], &gb_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            st.site = &o_emlrtRSI;
            p0 = hashpop(&st, n, N);
            if (p0 != (int32_T)muDoubleScalarFloor(p0)) {
              emlrtIntegerCheckR2012b(p0, &o_emlrtDCI, (emlrtConstCTX)sp);
            }
            if (((int32_T)p0 < 1) || ((int32_T)p0 > Xs->size[1])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)p0, 1, Xs->size[1],
                                            &hb_emlrtBCI, (emlrtConstCTX)sp);
            }
            pi_data[s + Xs->size[0] * ((int32_T)p0 - 1)] =
                d / (Z_data[s] + numLGTerms);
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b((emlrtConstCTX)sp);
          }
        }
        /*  compute pi(k|n) */
        st.site = &q_emlrtRSI;
        numLGTerms = sum(&st, n);
        nz = (int32_T)numLGTerms;
        emlrtForLoopVectorCheckR2021a(1.0, 1.0, numLGTerms, mxDOUBLE_CLASS,
                                      (int32_T)numLGTerms, &g_emlrtRTEI,
                                      (emlrtConstCTX)sp);
        for (s = 0; s < nz; s++) {
          for (ist = 0; ist < M; ist++) {
            if (ist + 1 > b_pi->size[0]) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, b_pi->size[0],
                                            &u_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (((int32_T)((uint32_T)s + 2U) < 1) ||
                ((int32_T)((uint32_T)s + 2U) > b_pi->size[1])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)s + 2U), 1,
                                            b_pi->size[1], &v_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            st.site = &p_emlrtRSI;
            numLGTerms = hashpop(&st, n, N);
            if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
              emlrtIntegerCheckR2012b(numLGTerms, &l_emlrtDCI,
                                      (emlrtConstCTX)sp);
            }
            if (((int32_T)numLGTerms < 1) ||
                ((int32_T)numLGTerms > b_pi->size[2])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1,
                                            b_pi->size[2], &w_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            b_pi_data[(ist + b_pi->size[0] * (s + 1)) +
                      b_pi->size[0] * b_pi->size[1] *
                          ((int32_T)numLGTerms - 1)] = 0.0;
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b((emlrtConstCTX)sp);
            }
          }
          for (b_ist = 0; b_ist < R; b_ist++) {
            if (b_ist + 1 > b_loop_ub) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, b_loop_ub,
                                            &x_emlrtBCI, (emlrtConstCTX)sp);
            }
            numLGTerms = n_data[b_ist];
            if (numLGTerms > 0.0) {
              if (M - 1 >= 0) {
                c_loop_ub = b_loop_ub;
              }
              for (k = 0; k < M; k++) {
                real_T d1;
                real_T d2;
                st.site = &n_emlrtRSI;
                vlen = d_N->size[0] * d_N->size[1];
                d_N->size[0] = 1;
                d_N->size[1] = b_loop_ub;
                emxEnsureCapacity_real_T(&st, d_N, vlen, &fb_emlrtRTEI);
                c_N_data = d_N->data;
                for (ist = 0; ist < c_loop_ub; ist++) {
                  c_N_data[ist] = n_data[ist];
                }
                /*  N=ONER(N,r) */
                /*  Decrement element in position of r of input vector */
                /*  */
                /*  Copyright (c) 2012-2026, Imperial College London */
                /*  All rights reserved. */
                if (b_ist + 1 > b_loop_ub) {
                  emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, b_loop_ub,
                                                &db_emlrtBCI, &st);
                }
                c_N_data[b_ist] = numLGTerms - 1.0;
                if (k + 1 > b_pi->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(
                      k + 1, 1, b_pi->size[0], &xb_emlrtBCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)((uint32_T)s + 2U) < 1) ||
                    ((int32_T)((uint32_T)s + 2U) > b_pi->size[1])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)s + 2U), 1,
                                                b_pi->size[1], &yb_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                st.site = &n_emlrtRSI;
                p0 = hashpop(&st, n, N);
                if (p0 != (int32_T)muDoubleScalarFloor(p0)) {
                  emlrtIntegerCheckR2012b(p0, &s_emlrtDCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)p0 < 1) || ((int32_T)p0 > b_pi->size[2])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)p0, 1, b_pi->size[2],
                                                &ac_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (k + 1 > M) {
                  emlrtDynamicBoundsCheckR2012b(k + 1, 1, M, &bc_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (b_ist + 1 > R) {
                  emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, R, &cc_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (k + 1 > mu->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(
                      k + 1, 1, mu->size[0], &dc_emlrtBCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)((uint32_T)s + 1U) < 1) ||
                    ((int32_T)((uint32_T)s + 1U) > mu->size[1])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)s + 1U), 1,
                                                mu->size[1], &ec_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (b_ist + 1 > Xs->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, Xs->size[0],
                                                &fc_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                st.site = &n_emlrtRSI;
                d = hashpop(&st, n, N);
                if (d != (int32_T)muDoubleScalarFloor(d)) {
                  emlrtIntegerCheckR2012b(d, &t_emlrtDCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)d < 1) || ((int32_T)d > Xs->size[1])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)d, 1, Xs->size[1],
                                                &gc_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (k + 1 > b_pi->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(
                      k + 1, 1, b_pi->size[0], &hc_emlrtBCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)((uint32_T)s + 1U) < 1) ||
                    ((int32_T)((uint32_T)s + 1U) > b_pi->size[1])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)s + 1U), 1,
                                                b_pi->size[1], &ic_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                st.site = &n_emlrtRSI;
                d1 = hashpop(&st, d_N, N);
                if (d1 != (int32_T)muDoubleScalarFloor(d1)) {
                  emlrtIntegerCheckR2012b(d1, &u_emlrtDCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)d1 < 1) || ((int32_T)d1 > b_pi->size[2])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)d1, 1, b_pi->size[2],
                                                &jc_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (k + 1 > b_pi->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(
                      k + 1, 1, b_pi->size[0], &kc_emlrtBCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)((uint32_T)s + 2U) < 1) ||
                    ((int32_T)((uint32_T)s + 2U) > b_pi->size[1])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)s + 2U), 1,
                                                b_pi->size[1], &lc_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                st.site = &n_emlrtRSI;
                d2 = hashpop(&st, n, N);
                if (d2 != (int32_T)muDoubleScalarFloor(d2)) {
                  emlrtIntegerCheckR2012b(d2, &v_emlrtDCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)d2 < 1) || ((int32_T)d2 > b_pi->size[2])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)d2, 1, b_pi->size[2],
                                                &mc_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                b_pi_data[(k + b_pi->size[0] * (s + 1)) +
                          b_pi->size[0] * b_pi->size[1] * ((int32_T)d2 - 1)] =
                    b_pi_data[(k + b_pi->size[0] * (s + 1)) +
                              b_pi->size[0] * b_pi->size[1] *
                                  ((int32_T)p0 - 1)] +
                    L_data[k + L->size[0] * b_ist] /
                        mu_data[k + mu->size[0] * s] *
                        pi_data[b_ist + Xs->size[0] * ((int32_T)d - 1)] *
                        b_pi_data[(k + b_pi->size[0] * s) +
                                  b_pi->size[0] * b_pi->size[1] *
                                      ((int32_T)d1 - 1)];
                if (*emlrtBreakCheckR2012bFlagVar != 0) {
                  emlrtBreakCheckR2012b((emlrtConstCTX)sp);
                }
              }
            }
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b((emlrtConstCTX)sp);
            }
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b((emlrtConstCTX)sp);
          }
        }
        /*  compute pi(0|n) */
        for (k = 0; k < M; k++) {
          st.site = &m_emlrtRSI;
          numLGTerms = sum(&st, n);
          if (muDoubleScalarIsNaN(numLGTerms)) {
            vlen = d_N->size[0] * d_N->size[1];
            d_N->size[0] = 1;
            d_N->size[1] = 1;
            emxEnsureCapacity_real_T(sp, d_N, vlen, &db_emlrtRTEI);
            c_N_data = d_N->data;
            c_N_data[0] = rtNaN;
          } else if (numLGTerms < 1.0) {
            d_N->size[0] = 1;
            d_N->size[1] = 0;
          } else {
            vlen = d_N->size[0] * d_N->size[1];
            d_N->size[0] = 1;
            d_N->size[1] = (int32_T)(numLGTerms - 1.0) + 1;
            emxEnsureCapacity_real_T(sp, d_N, vlen, &db_emlrtRTEI);
            c_N_data = d_N->data;
            vlen = (int32_T)(numLGTerms - 1.0);
            nz = (((int32_T)(numLGTerms - 1.0) + 1) / 2) << 1;
            b_vectorUB = nz - 2;
            for (ist = 0; ist <= b_vectorUB; ist += 2) {
              dv[0] = ist;
              dv[1] = ist + 1;
              r = _mm_loadu_pd(&dv[0]);
              _mm_storeu_pd(&c_N_data[ist], _mm_add_pd(_mm_set1_pd(1.0), r));
            }
            for (ist = nz; ist <= vlen; ist++) {
              c_N_data[ist] = (real_T)ist + 1.0;
            }
          }
          if (k + 1 > b_pi->size[0]) {
            emlrtDynamicBoundsCheckR2012b(k + 1, 1, b_pi->size[0], &b_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          st.site = &m_emlrtRSI;
          numLGTerms = hashpop(&st, n, N);
          if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
            emlrtIntegerCheckR2012b(numLGTerms, &i_emlrtDCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)numLGTerms < 1) ||
              ((int32_T)numLGTerms > b_pi->size[2])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1, b_pi->size[2],
                                          &c_emlrtBCI, (emlrtConstCTX)sp);
          }
          vectorUB = c_N->size[0] * c_N->size[1];
          c_N->size[0] = 1;
          vlen = d_N->size[1];
          c_N->size[1] = d_N->size[1];
          emxEnsureCapacity_real_T(sp, c_N, vectorUB, &jb_emlrtRTEI);
          XN_data = c_N->data;
          for (b_ist = 0; b_ist < vlen; b_ist++) {
            p0 = c_N_data[b_ist] + 1.0;
            if (p0 != (int32_T)muDoubleScalarFloor(p0)) {
              emlrtIntegerCheckR2012b(p0, &m_emlrtDCI, (emlrtConstCTX)sp);
            }
            if (((int32_T)p0 < 1) || ((int32_T)p0 > b_pi->size[1])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)p0, 1, b_pi->size[1],
                                            &ab_emlrtBCI, (emlrtConstCTX)sp);
            }
            XN_data[b_ist] = b_pi_data[(k + b_pi->size[0] * ((int32_T)p0 - 1)) +
                                       b_pi->size[0] * b_pi->size[1] *
                                           ((int32_T)numLGTerms - 1)];
          }
          st.site = &m_emlrtRSI;
          p0 = 1.0 - sum(&st, c_N);
          if (p0 < 0.0) {
            if (warn) {
              /*                 N */
              warn = false;
              b_isNumStable = false;
            }
            st.site = &l_emlrtRSI;
            if (muDoubleScalarIsNaN(stabilize)) {
              emlrtErrorWithMessageIdR2018a(&st, &b_emlrtRTEI,
                                            "MATLAB:nologicalnan",
                                            "MATLAB:nologicalnan", 0);
            }
            if (stabilize != 0.0) {
              if (b_pi->size[1] < 1) {
                emlrtDynamicBoundsCheckR2012b(1, 1, b_pi->size[1], &d_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (k + 1 > b_pi->size[0]) {
                emlrtDynamicBoundsCheckR2012b(k + 1, 1, b_pi->size[0],
                                              &vb_emlrtBCI, (emlrtConstCTX)sp);
              }
              st.site = &k_emlrtRSI;
              numLGTerms = hashpop(&st, n, N);
              if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
                emlrtIntegerCheckR2012b(numLGTerms, &r_emlrtDCI,
                                        (emlrtConstCTX)sp);
              }
              if (((int32_T)numLGTerms < 1) ||
                  ((int32_T)numLGTerms > b_pi->size[2])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1,
                                              b_pi->size[2], &wb_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              b_pi_data[k + b_pi->size[0] * b_pi->size[1] *
                                ((int32_T)numLGTerms - 1)] =
                  2.2204460492503131E-16;
            } else {
              if (b_pi->size[1] < 1) {
                emlrtDynamicBoundsCheckR2012b(1, 1, b_pi->size[1], &e_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (k + 1 > b_pi->size[0]) {
                emlrtDynamicBoundsCheckR2012b(k + 1, 1, b_pi->size[0],
                                              &tb_emlrtBCI, (emlrtConstCTX)sp);
              }
              st.site = &j_emlrtRSI;
              numLGTerms = hashpop(&st, n, N);
              if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
                emlrtIntegerCheckR2012b(numLGTerms, &q_emlrtDCI,
                                        (emlrtConstCTX)sp);
              }
              if (((int32_T)numLGTerms < 1) ||
                  ((int32_T)numLGTerms > b_pi->size[2])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1,
                                              b_pi->size[2], &ub_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              b_pi_data[k + b_pi->size[0] * b_pi->size[1] *
                                ((int32_T)numLGTerms - 1)] = p0;
            }
          } else {
            if (b_pi->size[1] < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, b_pi->size[1], &f_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (k + 1 > b_pi->size[0]) {
              emlrtDynamicBoundsCheckR2012b(k + 1, 1, b_pi->size[0],
                                            &bb_emlrtBCI, (emlrtConstCTX)sp);
            }
            st.site = &i_emlrtRSI;
            numLGTerms = hashpop(&st, n, N);
            if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
              emlrtIntegerCheckR2012b(numLGTerms, &n_emlrtDCI,
                                      (emlrtConstCTX)sp);
            }
            if (((int32_T)numLGTerms < 1) ||
                ((int32_T)numLGTerms > b_pi->size[2])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1,
                                            b_pi->size[2], &cb_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            b_pi_data[k + b_pi->size[0] * b_pi->size[1] *
                              ((int32_T)numLGTerms - 1)] = p0;
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b((emlrtConstCTX)sp);
          }
        }
        st.site = &h_emlrtRSI;
        last_nnz = 0;
        emlrtForLoopVectorCheckR2021a(n->size[1], -1.0, 1.0, mxDOUBLE_CLASS,
                                      n->size[1], &e_emlrtRTEI, &st);
        vectorUB = 0;
        exitg2 = false;
        while ((!exitg2) && (vectorUB <= b_loop_ub - 1)) {
          b_vectorUB = b_loop_ub - vectorUB;
          if ((b_vectorUB < 1) || (b_vectorUB > b_loop_ub)) {
            emlrtDynamicBoundsCheckR2012b(b_vectorUB, 1, b_loop_ub, &y_emlrtBCI,
                                          &st);
          }
          if (n_data[b_vectorUB - 1] != 0.0) {
            last_nnz = b_vectorUB;
            exitg2 = true;
          } else {
            vectorUB++;
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&st);
          }
        }
        if (last_nnz > 0) {
          if (last_nnz - 1 < 1) {
            nz = 0;
            vectorUB = 0;
          } else {
            if (n->size[1] < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, n->size[1], &g_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if ((last_nnz - 1 < 1) || (last_nnz - 1 > n->size[1])) {
              emlrtDynamicBoundsCheckR2012b(last_nnz - 1, 1, n->size[1],
                                            &h_emlrtBCI, (emlrtConstCTX)sp);
            }
            nz = last_nnz - 1;
            if (loop_ub < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, loop_ub, &i_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if ((last_nnz - 1 < 1) || (last_nnz - 1 > loop_ub)) {
              emlrtDynamicBoundsCheckR2012b(last_nnz - 1, 1, loop_ub,
                                            &j_emlrtBCI, (emlrtConstCTX)sp);
            }
            vectorUB = last_nnz - 1;
          }
          vlen = b_n->size[0] * b_n->size[1];
          b_n->size[0] = 1;
          b_n->size[1] = nz;
          emxEnsureCapacity_real_T(sp, b_n, vlen, &ib_emlrtRTEI);
          XN_data = b_n->data;
          for (ist = 0; ist < nz; ist++) {
            XN_data[ist] = n_data[ist];
          }
          vlen = c_N->size[0] * c_N->size[1];
          c_N->size[0] = 1;
          c_N->size[1] = vectorUB;
          emxEnsureCapacity_real_T(sp, c_N, vlen, &kb_emlrtRTEI);
          XN_data = c_N->data;
          for (b_ist = 0; b_ist < vectorUB; b_ist++) {
            XN_data[b_ist] = N_data[b_ist];
          }
          st.site = &g_emlrtRSI;
          if (sum(&st, b_n) == sum(&st, c_N)) {
            if ((uint32_T)last_nnz + 1U > (uint32_T)R) {
              vectorUB = 0;
              vlen = 0;
            } else {
              if (((int32_T)((uint32_T)last_nnz + 1U) < 1) ||
                  ((int32_T)((uint32_T)last_nnz + 1U) > n->size[1])) {
                emlrtDynamicBoundsCheckR2012b(
                    (int32_T)((uint32_T)last_nnz + 1U), 1, n->size[1],
                    &k_emlrtBCI, (emlrtConstCTX)sp);
              }
              vectorUB = last_nnz;
              if ((R < 1) || (R > n->size[1])) {
                emlrtDynamicBoundsCheckR2012b(R, 1, n->size[1], &l_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              vlen = R;
            }
            nz = b_n->size[0] * b_n->size[1];
            b_n->size[0] = 1;
            vlen -= vectorUB;
            b_n->size[1] = vlen;
            emxEnsureCapacity_real_T(sp, b_n, nz, &nb_emlrtRTEI);
            XN_data = b_n->data;
            for (ist = 0; ist < vlen; ist++) {
              XN_data[ist] = n_data[vectorUB + ist];
            }
            st.site = &g_emlrtRSI;
            if (sum(&st, b_n) == 0.0) {
              st.site = &f_emlrtRSI;
              if (last_nnz > Xs->size[0]) {
                emlrtDynamicBoundsCheckR2012b(last_nnz, 1, Xs->size[0],
                                              &nc_emlrtBCI, &st);
              }
              b_st.site = &f_emlrtRSI;
              numLGTerms = hashpop(&b_st, n, N);
              if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
                emlrtIntegerCheckR2012b(numLGTerms, &w_emlrtDCI, &st);
              }
              if (((int32_T)numLGTerms < 1) ||
                  ((int32_T)numLGTerms > Xs->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1,
                                              Xs->size[1], &oc_emlrtBCI, &st);
              }
              numLGTerms =
                  pi_data[(last_nnz + Xs->size[0] * ((int32_T)numLGTerms - 1)) -
                          1];
              if (numLGTerms < 0.0) {
                emlrtErrorWithMessageIdR2018a(
                    &st, &emlrtRTEI, "Coder:toolbox:ElFunDomainError",
                    "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
              }
              numLGTerms = muDoubleScalarLog(numLGTerms);
              lgIdx++;
              if (((int32_T)lgIdx - 1 < 1) ||
                  ((int32_T)lgIdx - 1 > lGN->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)lgIdx - 1, 1,
                                              lGN->size[1], &pc_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (((int32_T)lgIdx < 1) || ((int32_T)lgIdx > lGN->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)lgIdx, 1, lGN->size[1],
                                              &qc_emlrtBCI, (emlrtConstCTX)sp);
              }
              lGN_data[(int32_T)lgIdx - 1] =
                  lGN_data[(int32_T)lgIdx - 2] - numLGTerms;
            }
          }
        }
        st.site = &e_emlrtRSI;
        /*  n=PPROD(N) % init */
        /*  n=PPROD(n,N) % next state */
        /*  Return a sequence of non-negative vectors less than a given vector
         */
        /*  */
        /*  n=pprod(N); */
        /*  while n>=0 */
        /*    ... */
        /*    n=pprod(n,N); */
        /*  end */
        /*  */
        /*  Copyright (c) 2012-2026, Imperial College London */
        /*  All rights reserved. */
        if ((n->size[1] != loop_ub) && ((n->size[1] != 1) && (loop_ub != 1))) {
          emlrtDimSizeImpxCheckR2021b(n->size[1], loop_ub, &emlrtECI, &st);
        }
        b_st.site = &vb_emlrtRSI;
        if (n->size[1] == N->size[1]) {
          vlen = b_N->size[0] * b_N->size[1];
          b_N->size[0] = 1;
          b_N->size[1] = n->size[1];
          emxEnsureCapacity_boolean_T(&b_st, b_N, vlen, &ob_emlrtRTEI);
          b_N_data = b_N->data;
          for (ist = 0; ist < b_loop_ub; ist++) {
            b_N_data[ist] = (n_data[ist] == N_data[ist]);
          }
        } else {
          c_st.site = &vb_emlrtRSI;
          eq(&c_st, b_N, n, N);
          b_N_data = b_N->data;
        }
        c_st.site = &wb_emlrtRSI;
        d_st.site = &db_emlrtRSI;
        vlen = b_N->size[1];
        if (b_N->size[1] == 0) {
          nz = 0;
        } else {
          e_st.site = &eb_emlrtRSI;
          nz = b_N_data[0];
          f_st.site = &fb_emlrtRSI;
          if (b_N->size[1] > 2147483646) {
            g_st.site = &bb_emlrtRSI;
            check_forloop_overflow_error(&g_st);
          }
          for (ist = 2; ist <= vlen; ist++) {
            nz += b_N_data[ist - 1];
          }
        }
        if (nz == N->size[1]) {
          nz = n->size[0] * n->size[1];
          n->size[0] = 1;
          n->size[1] = 1;
          emxEnsureCapacity_real_T(&st, n, nz, &qb_emlrtRTEI);
          n_data = n->data;
          n_data[0] = -1.0;
        } else {
          nz = N->size[1];
          exitg2 = false;
          while ((!exitg2) && (nz > 0)) {
            if (nz > n->size[1]) {
              emlrtDynamicBoundsCheckR2012b(nz, 1, n->size[1], &rc_emlrtBCI,
                                            &st);
            }
            if (nz > loop_ub) {
              emlrtDynamicBoundsCheckR2012b(nz, 1, loop_ub, &sc_emlrtBCI, &st);
            }
            if (n_data[nz - 1] == N_data[nz - 1]) {
              if (nz > n->size[1]) {
                emlrtDynamicBoundsCheckR2012b(nz, 1, n->size[1], &tc_emlrtBCI,
                                              &st);
              }
              n_data[nz - 1] = 0.0;
              nz--;
            } else {
              exitg2 = true;
            }
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b(&st);
            }
          }
          if (nz != 0) {
            if (nz > n->size[1]) {
              emlrtDynamicBoundsCheckR2012b(nz, 1, n->size[1], &uc_emlrtBCI,
                                            &st);
            }
            n_data[nz - 1]++;
          } else {
            /* n=-1*ones(1,R);         */
          }
        }
        /*  get the next population */
      } else {
        exitg1 = 1;
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    } while (exitg1 == 0);
    emxFree_real_T(sp, &b_n);
    emxFree_real_T(sp, &n);
    if (lGN->size[1] < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, lGN->size[1], &m_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (((int32_T)lgIdx < 1) || ((int32_T)lgIdx > lGN->size[1])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)lgIdx, 1, lGN->size[1],
                                    &n_emlrtBCI, (emlrtConstCTX)sp);
    }
    nz = lGN->size[0] * lGN->size[1];
    lGN->size[1] = (int32_T)lgIdx;
    emxEnsureCapacity_real_T(sp, lGN, nz, &ab_emlrtRTEI);
    st.site = &d_emlrtRSI;
    numLGTerms = hashpop(&st, N, N);
    if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
      emlrtIntegerCheckR2012b(numLGTerms, &j_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (((int32_T)numLGTerms < 1) || ((int32_T)numLGTerms > Xs->size[1])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1, Xs->size[1],
                                    &o_emlrtBCI, (emlrtConstCTX)sp);
    }
    vectorUB = Xs->size[0];
    nz = X->size[0];
    X->size[0] = Xs->size[0];
    emxEnsureCapacity_real_T(sp, X, nz, &bb_emlrtRTEI);
    X_data = X->data;
    for (ist = 0; ist < vectorUB; ist++) {
      X_data[ist] = pi_data[ist + Xs->size[0] * ((int32_T)numLGTerms - 1)];
    }
    nz = XN->size[0] * XN->size[1];
    XN->size[0] = 1;
    XN->size[1] = Xs->size[0];
    emxEnsureCapacity_real_T(sp, XN, nz, &cb_emlrtRTEI);
    XN_data = XN->data;
    for (ist = 0; ist < vectorUB; ist++) {
      XN_data[XN->size[0] * ist] = X_data[ist];
    }
    st.site = &c_emlrtRSI;
    numLGTerms = hashpop(&st, N, N);
    if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
      emlrtIntegerCheckR2012b(numLGTerms, &k_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (((int32_T)numLGTerms < 1) || ((int32_T)numLGTerms > b_pi->size[2])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1, b_pi->size[2],
                                    &p_emlrtBCI, (emlrtConstCTX)sp);
    }
    last_nnz = b_pi->size[0];
    nz = pi->size[0] * pi->size[1];
    pi->size[0] = b_pi->size[0];
    vlen = b_pi->size[1];
    pi->size[1] = b_pi->size[1];
    emxEnsureCapacity_real_T(sp, pi, nz, &eb_emlrtRTEI);
    pi_data = pi->data;
    for (ist = 0; ist < vlen; ist++) {
      for (b_ist = 0; b_ist < last_nnz; b_ist++) {
        pi_data[b_ist + pi->size[0] * ist] =
            b_pi_data[(b_ist + b_pi->size[0] * ist) +
                      b_pi->size[0] * b_pi->size[1] *
                          ((int32_T)numLGTerms - 1)];
      }
    }
    nz = c_N->size[0] * c_N->size[1];
    c_N->size[0] = 1;
    c_N->size[1] = Xs->size[0];
    emxFree_real_T(sp, &Xs);
    emxEnsureCapacity_real_T(sp, c_N, nz, &hb_emlrtRTEI);
    XN_data = c_N->data;
    for (ist = 0; ist < vectorUB; ist++) {
      XN_data[ist] = X_data[ist];
    }
    st.site = &b_emlrtRSI;
    repmat(&st, c_N, L->size[0], QN);
    emxFree_real_T(sp, &c_N);
    if ((WN->size[0] != QN->size[0]) &&
        ((WN->size[0] != 1) && (QN->size[0] != 1))) {
      emlrtDimSizeImpxCheckR2021b(WN->size[0], QN->size[0], &b_emlrtECI,
                                  (emlrtConstCTX)sp);
    }
    if ((WN->size[1] != QN->size[1]) &&
        ((WN->size[1] != 1) && (QN->size[1] != 1))) {
      emlrtDimSizeImpxCheckR2021b(WN->size[1], QN->size[1], &c_emlrtECI,
                                  (emlrtConstCTX)sp);
    }
    if ((WN->size[0] == QN->size[0]) && (WN->size[1] == QN->size[1])) {
      b_vectorUB = WN->size[0] * WN->size[1];
      nz = QN->size[0] * QN->size[1];
      QN->size[0] = WN->size[0];
      QN->size[1] = WN->size[1];
      emxEnsureCapacity_real_T(sp, QN, nz, &lb_emlrtRTEI);
      XN_data = QN->data;
      vectorUB = (b_vectorUB / 2) << 1;
      vlen = vectorUB - 2;
      for (ist = 0; ist <= vlen; ist += 2) {
        __m128d r1;
        r = _mm_loadu_pd(&WN_data[ist]);
        r1 = _mm_loadu_pd(&XN_data[ist]);
        _mm_storeu_pd(&XN_data[ist], _mm_mul_pd(r, r1));
      }
      for (ist = vectorUB; ist < b_vectorUB; ist++) {
        XN_data[ist] *= WN_data[ist];
      }
    } else {
      st.site = &b_emlrtRSI;
      times(&st, QN, WN);
    }
    /* UN = repmat(XN,M,1) .* L; */
    if (b_pi->size[1] < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, b_pi->size[1], &q_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    emxInit_real_T(sp, &XN, 1, &mb_emlrtRTEI);
    nz = XN->size[0];
    XN->size[0] = b_pi->size[0];
    emxEnsureCapacity_real_T(sp, XN, nz, &mb_emlrtRTEI);
    lGN_data = XN->data;
    nz = (pi->size[0] / 2) << 1;
    vectorUB = nz - 2;
    for (ist = 0; ist <= vectorUB; ist += 2) {
      r = _mm_loadu_pd(&pi_data[ist]);
      _mm_storeu_pd(&lGN_data[ist], _mm_sub_pd(_mm_set1_pd(1.0), r));
    }
    for (ist = nz; ist < last_nnz; ist++) {
      lGN_data[ist] = 1.0 - pi_data[ist];
    }
    nz = UN->size[0] * UN->size[1];
    UN->size[0] = b_pi->size[0];
    emxFree_real_T(sp, &b_pi);
    UN->size[1] = 1;
    emxEnsureCapacity_real_T(sp, UN, nz, &pb_emlrtRTEI);
    XN_data = UN->data;
    for (ist = 0; ist < last_nnz; ist++) {
      XN_data[ist] = lGN_data[ist];
    }
    emxFree_real_T(sp, &XN);
    st.site = &emlrtRSI;
    b_st.site = &cc_emlrtRSI;
    c_st.site = &dc_emlrtRSI;
    if ((N->size[1] != 1) && (X->size[0] != 1) && (X->size[0] != N->size[1])) {
      emlrtErrorWithMessageIdR2018a(&c_st, &c_emlrtRTEI,
                                    "MATLAB:sizeDimensionsMustMatch",
                                    "MATLAB:sizeDimensionsMustMatch", 0);
    }
    if (X->size[0] == N->size[1]) {
      nz = d_N->size[0] * d_N->size[1];
      d_N->size[0] = 1;
      d_N->size[1] = loop_ub;
      emxEnsureCapacity_real_T(&b_st, d_N, nz, &rb_emlrtRTEI);
      c_N_data = d_N->data;
      nz = scalarLB - 2;
      for (ist = 0; ist <= nz; ist += 2) {
        r = _mm_loadu_pd(&X_data[ist]);
        _mm_storeu_pd(&c_N_data[ist],
                      _mm_div_pd(_mm_loadu_pd(&N_data[ist]), r));
      }
      for (ist = scalarLB; ist < loop_ub; ist++) {
        c_N_data[ist] = N_data[ist] / X_data[ist];
      }
    } else {
      c_st.site = &ec_emlrtRSI;
      binary_expand_op_1(&c_st, d_N, N, X);
      c_N_data = d_N->data;
    }
    emxFree_real_T(&b_st, &X);
    vlen = d_N->size[1];
    if ((d_N->size[1] != Z->size[1]) &&
        ((d_N->size[1] != 1) && (Z->size[1] != 1))) {
      emlrtDimSizeImpxCheckR2021b(d_N->size[1], Z->size[1], &d_emlrtECI,
                                  (emlrtConstCTX)sp);
    }
    if (d_N->size[1] == Z->size[1]) {
      nz = CN->size[0] * CN->size[1];
      CN->size[0] = 1;
      CN->size[1] = d_N->size[1];
      emxEnsureCapacity_real_T(sp, CN, nz, &sb_emlrtRTEI);
      XN_data = CN->data;
      nz = (d_N->size[1] / 2) << 1;
      vectorUB = nz - 2;
      for (ist = 0; ist <= vectorUB; ist += 2) {
        r = _mm_loadu_pd(&c_N_data[ist]);
        _mm_storeu_pd(&XN_data[ist], _mm_sub_pd(r, _mm_loadu_pd(&Z_data[ist])));
      }
      for (ist = nz; ist < vlen; ist++) {
        XN_data[ist] = c_N_data[ist] - Z_data[ist];
      }
    } else {
      st.site = &emlrtRSI;
      binary_expand_op(&st, CN, d_N, Z);
    }
    emxFree_real_T(sp, &d_N);
    /*  cycle time exclusive of think time */
  }
  emxFree_boolean_T(sp, &b_N);
  emxFree_real_T(sp, &WN);
  *isNumStable = b_isNumStable;
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (pfqn_mvald.c) */
