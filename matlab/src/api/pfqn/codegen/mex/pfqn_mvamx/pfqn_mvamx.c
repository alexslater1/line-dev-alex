/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mvamx.c
 *
 * Code generation for function 'pfqn_mvamx'
 *
 */

/* Include files */
#include "pfqn_mvamx.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_mva.h"
#include "pfqn_mvamx_data.h"
#include "pfqn_mvamx_emxutil.h"
#include "pfqn_mvamx_mexutil.h"
#include "pfqn_mvamx_types.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>

/* Variable Definitions */
static emlrtRSInfo emlrtRSI =
    {
        34,           /* lineNo */
        "pfqn_mvamx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pathName */
};

static emlrtRSInfo b_emlrtRSI =
    {
        68,           /* lineNo */
        "pfqn_mvamx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pathName */
};

static emlrtRSInfo c_emlrtRSI =
    {
        75,           /* lineNo */
        "pfqn_mvamx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pathName */
};

static emlrtRSInfo d_emlrtRSI =
    {
        76,           /* lineNo */
        "pfqn_mvamx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pathName */
};

static emlrtRSInfo e_emlrtRSI =
    {
        93,           /* lineNo */
        "pfqn_mvamx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pathName */
};

static emlrtRSInfo j_emlrtRSI = {
    112,                /* lineNo */
    "blockedSummation", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo k_emlrtRSI = {
    173,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo l_emlrtRSI = {
    190,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo m_emlrtRSI = {
    192,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo n_emlrtRSI = {
    204,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo o_emlrtRSI = {
    207,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo p_emlrtRSI = {
    225,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo q_emlrtRSI = {
    227,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo r_emlrtRSI = {
    238,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo t_emlrtRSI = {
    34,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

static emlrtRSInfo u_emlrtRSI = {
    78,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

static emlrtRSInfo v_emlrtRSI = {
    85,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

static emlrtRSInfo w_emlrtRSI =
    {
        34,               /* lineNo */
        "rdivide_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "rdivide_helper.m" /* pathName */
};

static emlrtRSInfo x_emlrtRSI = {
    53,    /* lineNo */
    "div", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/div.m" /* pathName
                                                                       */
};

static emlrtRTEInfo emlrtRTEI = {
    13,                     /* lineNo */
    27,                     /* colNo */
    "assertCompatibleDims", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/shared/coder/coder/lib/+coder/+internal/"
    "assertCompatibleDims.m" /* pName */
};

static emlrtECInfo emlrtECI =
    {
        -1,           /* nDims */
        78,           /* lineNo */
        5,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtECInfo b_emlrtECI =
    {
        -1,           /* nDims */
        79,           /* lineNo */
        5,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtBCInfo emlrtBCI =
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

static emlrtBCInfo b_emlrtBCI =
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

static emlrtECInfo c_emlrtECI =
    {
        -1,           /* nDims */
        77,           /* lineNo */
        5,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtBCInfo c_emlrtBCI =
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

static emlrtBCInfo d_emlrtBCI =
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

static emlrtBCInfo e_emlrtBCI =
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

static emlrtBCInfo f_emlrtBCI =
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

static emlrtBCInfo g_emlrtBCI =
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

static emlrtBCInfo h_emlrtBCI =
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

static emlrtBCInfo i_emlrtBCI =
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

static emlrtBCInfo j_emlrtBCI =
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

static emlrtBCInfo k_emlrtBCI =
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

static emlrtBCInfo l_emlrtBCI =
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

static emlrtBCInfo m_emlrtBCI =
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

static emlrtBCInfo n_emlrtBCI =
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

static emlrtBCInfo o_emlrtBCI =
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

static emlrtBCInfo p_emlrtBCI =
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

static emlrtBCInfo q_emlrtBCI =
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

static emlrtBCInfo r_emlrtBCI =
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

static emlrtBCInfo s_emlrtBCI =
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

static emlrtBCInfo t_emlrtBCI =
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

static emlrtBCInfo u_emlrtBCI =
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

static emlrtBCInfo v_emlrtBCI =
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

static emlrtBCInfo w_emlrtBCI =
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

static emlrtBCInfo x_emlrtBCI =
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

static emlrtBCInfo y_emlrtBCI =
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

static emlrtBCInfo ab_emlrtBCI =
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

static emlrtBCInfo bb_emlrtBCI =
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

static emlrtBCInfo cb_emlrtBCI =
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

static emlrtBCInfo db_emlrtBCI =
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

static emlrtBCInfo eb_emlrtBCI =
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

static emlrtBCInfo fb_emlrtBCI =
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

static emlrtBCInfo gb_emlrtBCI =
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

static emlrtBCInfo hb_emlrtBCI =
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

static emlrtBCInfo ib_emlrtBCI =
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

static emlrtBCInfo jb_emlrtBCI =
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

static emlrtBCInfo kb_emlrtBCI =
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

static emlrtBCInfo lb_emlrtBCI =
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

static emlrtBCInfo mb_emlrtBCI =
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

static emlrtBCInfo nb_emlrtBCI =
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

static emlrtBCInfo ob_emlrtBCI =
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

static emlrtBCInfo pb_emlrtBCI =
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

static emlrtBCInfo qb_emlrtBCI =
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

static emlrtBCInfo rb_emlrtBCI =
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

static emlrtBCInfo sb_emlrtBCI =
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

static emlrtBCInfo tb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        76,           /* lineNo */
        41,           /* colNo */
        "N",          /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ub_emlrtBCI =
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

static emlrtBCInfo vb_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        76,           /* lineNo */
        58,           /* colNo */
        "Z",          /* aName */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo wb_emlrtBCI =
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

static emlrtBCInfo xb_emlrtBCI =
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

static emlrtBCInfo yb_emlrtBCI =
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

static emlrtRTEInfo h_emlrtRTEI =
    {
        38,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo i_emlrtRTEI =
    {
        39,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo j_emlrtRTEI =
    {
        51,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo k_emlrtRTEI =
    {
        52,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo l_emlrtRTEI =
    {
        54,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo m_emlrtRTEI =
    {
        55,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo n_emlrtRTEI =
    {
        56,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo o_emlrtRTEI =
    {
        57,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo p_emlrtRTEI =
    {
        68,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo q_emlrtRTEI = {
    146,                /* lineNo */
    24,                 /* colNo */
    "blockedSummation", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pName */
};

static emlrtRTEInfo r_emlrtRTEI = {
    153,                /* lineNo */
    23,                 /* colNo */
    "blockedSummation", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pName */
};

static emlrtRTEInfo s_emlrtRTEI =
    {
        71,           /* lineNo */
        5,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo t_emlrtRTEI =
    {
        75,           /* lineNo */
        10,           /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo u_emlrtRTEI =
    {
        93,           /* lineNo */
        43,           /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo v_emlrtRTEI = {
    73,       /* lineNo */
    28,       /* colNo */
    "repmat", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pName */
};

static emlrtRTEInfo w_emlrtRTEI = {
    54,    /* lineNo */
    9,     /* colNo */
    "div", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/div.m" /* pName
                                                                       */
};

static emlrtRTEInfo x_emlrtRTEI =
    {
        77,           /* lineNo */
        8,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo y_emlrtRTEI =
    {
        76,           /* lineNo */
        39,           /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo ab_emlrtRTEI =
    {
        76,           /* lineNo */
        56,           /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo bb_emlrtRTEI =
    {
        78,           /* lineNo */
        10,           /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo cb_emlrtRTEI =
    {
        79,           /* lineNo */
        10,           /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo db_emlrtRTEI =
    {
        58,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo eb_emlrtRTEI =
    {
        24,           /* lineNo */
        30,           /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo fb_emlrtRTEI =
    {
        77,           /* lineNo */
        5,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo gb_emlrtRTEI =
    {
        78,           /* lineNo */
        5,            /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo hb_emlrtRTEI =
    {
        75,           /* lineNo */
        35,           /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

static emlrtRTEInfo ib_emlrtRTEI = {
    153,                /* lineNo */
    1,                  /* colNo */
    "blockedSummation", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pName */
};

/* Function Declarations */
static real_T
binary_expand_op(const emlrtStack *sp, const emlrtRSInfo in1,
                 const emxArray_real_T *in2, const emxArray_real_T *in3,
                 const emxArray_real_T *in4, const emxArray_uint32_T *in5,
                 const emxArray_real_T *in6, const emxArray_real_T *in7,
                 emxArray_real_T *in8, emxArray_real_T *in9,
                 emxArray_real_T *in10, emxArray_real_T *in11);

/* Function Definitions */
static real_T
binary_expand_op(const emlrtStack *sp, const emlrtRSInfo in1,
                 const emxArray_real_T *in2, const emxArray_real_T *in3,
                 const emxArray_real_T *in4, const emxArray_uint32_T *in5,
                 const emxArray_real_T *in6, const emxArray_real_T *in7,
                 emxArray_real_T *in8, emxArray_real_T *in9,
                 emxArray_real_T *in10, emxArray_real_T *in11)
{
  emlrtStack st;
  emxArray_real_T *b_in2;
  emxArray_real_T *c_in4;
  emxArray_real_T *c_in6;
  const real_T *in2_data;
  const real_T *in3_data;
  const real_T *in4_data;
  const real_T *in6_data;
  real_T out1;
  real_T *b_in2_data;
  int32_T aux_0_1;
  int32_T aux_1_1;
  int32_T b_in4;
  int32_T b_in6;
  int32_T b_loop_ub;
  int32_T i;
  int32_T i1;
  int32_T loop_ub;
  int32_T stride_0_0;
  int32_T stride_0_1;
  int32_T stride_1_0;
  int32_T stride_1_1;
  const uint32_T *in5_data;
  st.prev = sp;
  st.tls = sp->tls;
  in6_data = in6->data;
  in5_data = in5->data;
  in4_data = in4->data;
  in3_data = in3->data;
  in2_data = in2->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  b_in4 = in4->size[1];
  b_in6 = in6->size[1];
  emxInit_real_T(sp, &b_in2, 2, &w_emlrtRTEI);
  if (in3->size[0] == 1) {
    loop_ub = in2->size[0];
  } else {
    loop_ub = in3->size[0];
  }
  stride_0_0 = b_in2->size[0] * b_in2->size[1];
  b_in2->size[0] = loop_ub;
  if (in3->size[1] == 1) {
    b_loop_ub = in2->size[1];
  } else {
    b_loop_ub = in3->size[1];
  }
  b_in2->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(sp, b_in2, stride_0_0, &w_emlrtRTEI);
  b_in2_data = b_in2->data;
  stride_0_0 = (in2->size[0] != 1);
  stride_0_1 = (in2->size[1] != 1);
  stride_1_0 = (in3->size[0] != 1);
  stride_1_1 = (in3->size[1] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_in2_data[i1 + b_in2->size[0] * i] =
          in2_data[i1 * stride_0_0 + in2->size[0] * aux_0_1] /
          in3_data[i1 * stride_1_0 + in3->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_1;
    aux_0_1 += stride_0_1;
  }
  emxInit_real_T(sp, &c_in4, 2, &y_emlrtRTEI);
  stride_0_0 = c_in4->size[0] * c_in4->size[1];
  c_in4->size[0] = 1;
  stride_0_1 = in5->size[1];
  c_in4->size[1] = stride_0_1;
  emxEnsureCapacity_real_T(sp, c_in4, stride_0_0, &y_emlrtRTEI);
  b_in2_data = c_in4->data;
  for (i = 0; i < stride_0_1; i++) {
    if (((int32_T)in5_data[i] < 1) || ((int32_T)in5_data[i] > b_in4)) {
      emlrtDynamicBoundsCheckR2012b((int32_T)in5_data[i], 1, b_in4,
                                    &tb_emlrtBCI, (emlrtConstCTX)sp);
    }
    b_in2_data[i] = in4_data[(int32_T)in5_data[i] - 1];
  }
  emxInit_real_T(sp, &c_in6, 2, &ab_emlrtRTEI);
  stride_0_0 = c_in6->size[0] * c_in6->size[1];
  c_in6->size[0] = 1;
  c_in6->size[1] = stride_0_1;
  emxEnsureCapacity_real_T(sp, c_in6, stride_0_0, &ab_emlrtRTEI);
  b_in2_data = c_in6->data;
  for (i = 0; i < stride_0_1; i++) {
    if (((int32_T)in5_data[i] < 1) || ((int32_T)in5_data[i] > b_in6)) {
      emlrtDynamicBoundsCheckR2012b((int32_T)in5_data[i], 1, b_in6,
                                    &vb_emlrtBCI, (emlrtConstCTX)sp);
    }
    b_in2_data[i] = in6_data[(int32_T)in5_data[i] - 1];
  }
  st.site = (emlrtRSInfo *)&in1;
  out1 = pfqn_mva(&st, b_in2, c_in4, c_in6, in7, in8, in9, in10, in11);
  emxFree_real_T(sp, &c_in6);
  emxFree_real_T(sp, &c_in4);
  emxFree_real_T(sp, &b_in2);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return out1;
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

void pfqn_mvamx(const emlrtStack *sp, const emxArray_real_T *lambda,
                const emxArray_real_T *D, const emxArray_real_T *N,
                emxArray_real_T *Z, const emxArray_real_T *mi,
                emxArray_real_T *XN, emxArray_real_T *QN, emxArray_real_T *UN,
                emxArray_real_T *CN, real_T *lGN)
{
  static const int32_T iv[2] = {1, 6};
  static const int32_T iv1[2] = {1, 10};
  static const int32_T iv2[2] = {1, 51};
  static const char_T varargin_3[51] = {
      'A', 'r', 'r', 'i', 'v', 'a', 'l', ' ', 'r', 'a', 't', 'e', ' ',
      'c', 'a', 'n', 'n', 'o', 't', ' ', 'b', 'e', ' ', 's', 'p', 'e',
      'c', 'i', 'f', 'i', 'e', 'd', ' ', 'o', 'n', ' ', 'c', 'l', 'o',
      's', 'e', 'd', ' ', 'c', 'l', 'a', 's', 's', 'e', 's', '.'};
  static const char_T varargin_2[10] = {'p', 'f', 'q', 'n', '_',
                                        'm', 'v', 'a', 'm', 'x'};
  static const char_T varargin_1[6] = {'%', 's', ':', ' ', '%', 's'};
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
  emlrtStack st;
  emxArray_int32_T *r2;
  emxArray_int32_T *r4;
  emxArray_real_T *CNc;
  emxArray_real_T *QNc;
  emxArray_real_T *UNt;
  emxArray_real_T *XNc;
  emxArray_real_T *a__1;
  emxArray_real_T *b;
  emxArray_real_T *b_N;
  emxArray_real_T *b_Z;
  emxArray_real_T *b_x;
  emxArray_real_T *bsum;
  emxArray_real_T *x;
  emxArray_uint32_T *closedClasses;
  emxArray_uint32_T *openClasses;
  const mxArray *b_y;
  const mxArray *c_y;
  const mxArray *m;
  const mxArray *y;
  const real_T *D_data;
  const real_T *N_data;
  const real_T *lambda_data;
  real_T b_lGN;
  real_T *CN_data;
  real_T *QN_data;
  real_T *QNc_data;
  real_T *UN_data;
  real_T *UNt_data;
  real_T *XN_data;
  real_T *Z_data;
  real_T *bsum_data;
  real_T *x_data;
  int32_T b_D[2];
  int32_T b_loop_ub;
  int32_T bvstride;
  int32_T c_loop_ub;
  int32_T hi;
  int32_T i;
  int32_T i1;
  int32_T i2;
  int32_T i3;
  int32_T i4;
  int32_T ibtile;
  int32_T ist;
  int32_T k;
  int32_T lastBlockLength;
  int32_T loop_ub;
  int32_T nblocks;
  int32_T ntilecols;
  int32_T pfqn_mvamx_numThreads;
  int32_T r;
  int32_T vstride;
  int32_T xblockoffset;
  int32_T xoffset;
  int32_T *r3;
  uint32_T nClosed;
  uint32_T nOpen;
  uint32_T *closedClasses_data;
  uint32_T *openClasses_data;
  boolean_T emlrtHadParallelError = false;
  boolean_T overflow;
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
  D_data = D->data;
  lambda_data = lambda->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
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
  loop_ub = D->size[1];
  for (r = 0; r < loop_ub; r++) {
    if (r + 1 > lambda->size[1]) {
      emlrtDynamicBoundsCheckR2012b(r + 1, 1, lambda->size[1], &j_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (lambda_data[r] > 0.0) {
      if (r + 1 > N->size[1]) {
        emlrtDynamicBoundsCheckR2012b(r + 1, 1, N->size[1], &i_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if ((!muDoubleScalarIsInf(N_data[r])) &&
          (!muDoubleScalarIsNaN(N_data[r]))) {
        if (r + 1 > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, N->size[1], &k_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (N_data[r] > 0.0) {
          st.site = &emlrtRSI;
          /* LINE_ERROR Display a plain-text error message with file and line
           * info. */
          /*  */
          /*    LINE_ERROR(CALLER, MSG, ...) throws an error with CALLER's name
           * and */
          /*    message, including the source file and line number, in plain
           * text (no */
          /*    hyperlink). Extra arguments are passed to sprintf to format MSG.
           */
          /*    Copyright (c) 2012-2026, Imperial College London */
          /*    All rights reserved. */
          b_st.site = &f_emlrtRSI;
          y = NULL;
          m = emlrtCreateCharArray(2, &iv[0]);
          emlrtInitCharArrayR2013a(&b_st, 6, m, &varargin_1[0]);
          emlrtAssign(&y, m);
          b_y = NULL;
          m = emlrtCreateCharArray(2, &iv1[0]);
          emlrtInitCharArrayR2013a(&b_st, 10, m, &varargin_2[0]);
          emlrtAssign(&b_y, m);
          c_y = NULL;
          m = emlrtCreateCharArray(2, &iv2[0]);
          emlrtInitCharArrayR2013a(&b_st, 51, m, &varargin_3[0]);
          emlrtAssign(&c_y, m);
          c_st.site = &yb_emlrtRSI;
          b_error(&c_st, y, b_y, c_y, &emlrtMCI);
        }
      }
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  emxInit_uint32_T(sp, &openClasses, &h_emlrtRTEI);
  ibtile = openClasses->size[0] * openClasses->size[1];
  openClasses->size[0] = 1;
  openClasses->size[1] = D->size[1];
  emxEnsureCapacity_uint32_T(sp, openClasses, ibtile, &h_emlrtRTEI);
  openClasses_data = openClasses->data;
  emxInit_uint32_T(sp, &closedClasses, &i_emlrtRTEI);
  ibtile = closedClasses->size[0] * closedClasses->size[1];
  closedClasses->size[0] = 1;
  closedClasses->size[1] = D->size[1];
  emxEnsureCapacity_uint32_T(sp, closedClasses, ibtile, &i_emlrtRTEI);
  closedClasses_data = closedClasses->data;
  for (r = 0; r < loop_ub; r++) {
    openClasses_data[r] = 0U;
    closedClasses_data[r] = 0U;
  }
  nOpen = 0U;
  nClosed = 0U;
  for (r = 0; r < loop_ub; r++) {
    if (r + 1 > N->size[1]) {
      emlrtDynamicBoundsCheckR2012b(r + 1, 1, N->size[1], &h_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (muDoubleScalarIsInf(N_data[r])) {
      nOpen++;
      if (((int32_T)nOpen < 1) || ((int32_T)nOpen > openClasses->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)nOpen, 1, openClasses->size[1],
                                      &m_emlrtBCI, (emlrtConstCTX)sp);
      }
      openClasses_data[(int32_T)nOpen - 1] = (uint32_T)(r + 1);
    } else {
      nClosed++;
      if (((int32_T)nClosed < 1) ||
          ((int32_T)nClosed > closedClasses->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)nClosed, 1,
                                      closedClasses->size[1], &l_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      closedClasses_data[(int32_T)nClosed - 1] = (uint32_T)(r + 1);
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  if ((int32_T)nOpen < 1) {
    i = 0;
  } else {
    if (openClasses->size[1] < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, openClasses->size[1], &g_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (((int32_T)nOpen < 1) || ((int32_T)nOpen > openClasses->size[1])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)nOpen, 1, openClasses->size[1],
                                    &f_emlrtBCI, (emlrtConstCTX)sp);
    }
    i = (int32_T)nOpen;
  }
  ibtile = openClasses->size[0] * openClasses->size[1];
  openClasses->size[1] = i;
  emxEnsureCapacity_uint32_T(sp, openClasses, ibtile, &j_emlrtRTEI);
  openClasses_data = openClasses->data;
  if ((int32_T)nClosed < 1) {
    b_loop_ub = 0;
  } else {
    if (closedClasses->size[1] < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, closedClasses->size[1], &e_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (((int32_T)nClosed < 1) || ((int32_T)nClosed > closedClasses->size[1])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)nClosed, 1, closedClasses->size[1],
                                    &d_emlrtBCI, (emlrtConstCTX)sp);
    }
    b_loop_ub = (int32_T)nClosed;
  }
  ibtile = closedClasses->size[0] * closedClasses->size[1];
  closedClasses->size[1] = b_loop_ub;
  emxEnsureCapacity_uint32_T(sp, closedClasses, ibtile, &k_emlrtRTEI);
  closedClasses_data = closedClasses->data;
  ibtile = XN->size[0] * XN->size[1];
  XN->size[0] = 1;
  XN->size[1] = D->size[1];
  emxEnsureCapacity_real_T(sp, XN, ibtile, &l_emlrtRTEI);
  XN_data = XN->data;
  for (r = 0; r < loop_ub; r++) {
    XN_data[r] = 0.0;
  }
  c_loop_ub = D->size[0];
  ibtile = UN->size[0] * UN->size[1];
  UN->size[0] = D->size[0];
  UN->size[1] = D->size[1];
  emxEnsureCapacity_real_T(sp, UN, ibtile, &m_emlrtRTEI);
  UN_data = UN->data;
  ntilecols = D->size[0] * D->size[1];
  for (r = 0; r < ntilecols; r++) {
    UN_data[r] = 0.0;
  }
  ibtile = CN->size[0] * CN->size[1];
  CN->size[0] = D->size[0];
  CN->size[1] = D->size[1];
  emxEnsureCapacity_real_T(sp, CN, ibtile, &n_emlrtRTEI);
  CN_data = CN->data;
  for (r = 0; r < ntilecols; r++) {
    CN_data[r] = 0.0;
  }
  ibtile = QN->size[0] * QN->size[1];
  QN->size[0] = D->size[0];
  QN->size[1] = D->size[1];
  emxEnsureCapacity_real_T(sp, QN, ibtile, &o_emlrtRTEI);
  QN_data = QN->data;
  for (r = 0; r < ntilecols; r++) {
    QN_data[r] = 0.0;
  }
  emxInit_real_T(sp, &QNc, 2, &db_emlrtRTEI);
  QNc_data = QNc->data;
  QNc->size[0] = D->size[0];
  QNc->size[1] = 0;
  b_lGN = rtNaN;
  i1 = (int32_T)nOpen;
  for (r = 0; r < i1; r++) {
    if (r + 1 > i) {
      emlrtDynamicBoundsCheckR2012b(r + 1, 1, i, &n_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    ibtile = (int32_T)openClasses_data[r];
    hi = (D->size[0] / 2) << 1;
    ntilecols = hi - 2;
    for (ist = 0; ist <= ntilecols; ist += 2) {
      if (ist + 1 > c_loop_ub) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, c_loop_ub, &xb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if ((ibtile < 1) || (ibtile > loop_ub)) {
        emlrtDynamicBoundsCheckR2012b(ibtile, 1, loop_ub, &xb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (ibtile > lambda->size[1]) {
        emlrtDynamicBoundsCheckR2012b(ibtile, 1, lambda->size[1], &wb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (ist + 1 > UN->size[0]) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, UN->size[0], &yb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (ibtile > UN->size[1]) {
        emlrtDynamicBoundsCheckR2012b(ibtile, 1, UN->size[1], &yb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      _mm_storeu_pd(
          &UN_data[ist + UN->size[0] * (ibtile - 1)],
          _mm_mul_pd(_mm_set1_pd(lambda_data[ibtile - 1]),
                     _mm_loadu_pd(&D_data[ist + D->size[0] * (ibtile - 1)])));
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    for (ist = hi; ist < c_loop_ub; ist++) {
      if ((ibtile < 1) || (ibtile > lambda->size[1])) {
        emlrtDynamicBoundsCheckR2012b(ibtile, 1, lambda->size[1], &q_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (ist + 1 > c_loop_ub) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, c_loop_ub, &r_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (ibtile > loop_ub) {
        emlrtDynamicBoundsCheckR2012b(ibtile, 1, loop_ub, &s_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (ist + 1 > UN->size[0]) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, UN->size[0], &t_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (ibtile > UN->size[1]) {
        emlrtDynamicBoundsCheckR2012b(ibtile, 1, UN->size[1], &u_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      UN_data[ist + UN->size[0] * (ibtile - 1)] =
          lambda_data[ibtile - 1] * D_data[ist + D->size[0] * (ibtile - 1)];
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    nOpen = openClasses_data[r];
    if (((int32_T)nOpen < 1) || ((int32_T)nOpen > lambda->size[1])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)nOpen, 1, lambda->size[1],
                                    &o_emlrtBCI, (emlrtConstCTX)sp);
    }
    if (((int32_T)nOpen < 1) || ((int32_T)nOpen > XN->size[1])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)nOpen, 1, XN->size[1], &p_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    XN_data[(int32_T)nOpen - 1] = lambda_data[(int32_T)nOpen - 1];
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  st.site = &b_emlrtRSI;
  b_st.site = &g_emlrtRSI;
  c_st.site = &h_emlrtRSI;
  d_st.site = &i_emlrtRSI;
  emxInit_real_T(&d_st, &UNt, 1, &p_emlrtRTEI);
  if ((UN->size[0] == 0) || (UN->size[1] == 0)) {
    ibtile = UNt->size[0];
    UNt->size[0] = UN->size[0];
    emxEnsureCapacity_real_T(&d_st, UNt, ibtile, &p_emlrtRTEI);
    UNt_data = UNt->data;
    ibtile = UN->size[0];
    for (r = 0; r < ibtile; r++) {
      UNt_data[r] = 0.0;
    }
  } else {
    e_st.site = &j_emlrtRSI;
    vstride = UN->size[0];
    bvstride = UN->size[0] << 10;
    ibtile = UNt->size[0];
    UNt->size[0] = UN->size[0];
    emxEnsureCapacity_real_T(&e_st, UNt, ibtile, &q_emlrtRTEI);
    UNt_data = UNt->data;
    emxInit_real_T(&e_st, &bsum, 1, &ib_emlrtRTEI);
    ibtile = bsum->size[0];
    bsum->size[0] = UN->size[0];
    emxEnsureCapacity_real_T(&e_st, bsum, ibtile, &r_emlrtRTEI);
    bsum_data = bsum->data;
    if (UN->size[1] <= 1024) {
      ibtile = UN->size[1];
      lastBlockLength = 0;
      nblocks = 1;
    } else {
      ibtile = 1024;
      nblocks = (int32_T)((uint32_T)UN->size[1] >> 10);
      lastBlockLength = UN->size[1] - (nblocks << 10);
      if (lastBlockLength > 0) {
        nblocks++;
      } else {
        lastBlockLength = 1024;
      }
    }
    f_st.site = &k_emlrtRSI;
    if (UN->size[0] > 2147483646) {
      g_st.site = &s_emlrtRSI;
      check_forloop_overflow_error(&g_st);
    }
    for (r = 0; r < vstride; r++) {
      UNt_data[r] = UN_data[r];
      bsum_data[r] = 0.0;
    }
    f_st.site = &l_emlrtRSI;
    for (r = 2; r <= ibtile; r++) {
      xoffset = (r - 1) * vstride;
      f_st.site = &m_emlrtRSI;
      if (vstride > 2147483646) {
        g_st.site = &s_emlrtRSI;
        check_forloop_overflow_error(&g_st);
      }
      hi = (vstride / 2) << 1;
      ntilecols = hi - 2;
      for (ist = 0; ist <= ntilecols; ist += 2) {
        b_r = _mm_loadu_pd(&UNt_data[ist]);
        r1 = _mm_loadu_pd(&UN_data[xoffset + ist]);
        _mm_storeu_pd(&UNt_data[ist], _mm_add_pd(b_r, r1));
      }
      for (ist = hi; ist < vstride; ist++) {
        UNt_data[ist] += UN_data[xoffset + ist];
      }
    }
    f_st.site = &n_emlrtRSI;
    for (k = 2; k <= nblocks; k++) {
      xblockoffset = (k - 1) * bvstride;
      f_st.site = &o_emlrtRSI;
      if (vstride > 2147483646) {
        g_st.site = &s_emlrtRSI;
        check_forloop_overflow_error(&g_st);
      }
      for (r = 0; r < vstride; r++) {
        bsum_data[r] = UN_data[xblockoffset + r];
      }
      if (k == nblocks) {
        hi = lastBlockLength;
      } else {
        hi = 1024;
      }
      f_st.site = &p_emlrtRSI;
      for (r = 2; r <= hi; r++) {
        xoffset = xblockoffset + (r - 1) * vstride;
        f_st.site = &q_emlrtRSI;
        ntilecols = (vstride / 2) << 1;
        ibtile = ntilecols - 2;
        for (ist = 0; ist <= ibtile; ist += 2) {
          b_r = _mm_loadu_pd(&bsum_data[ist]);
          r1 = _mm_loadu_pd(&UN_data[xoffset + ist]);
          _mm_storeu_pd(&bsum_data[ist], _mm_add_pd(b_r, r1));
        }
        for (ist = ntilecols; ist < vstride; ist++) {
          bsum_data[ist] += UN_data[xoffset + ist];
        }
      }
      f_st.site = &r_emlrtRSI;
      hi = (vstride / 2) << 1;
      ntilecols = hi - 2;
      for (r = 0; r <= ntilecols; r += 2) {
        b_r = _mm_loadu_pd(&UNt_data[r]);
        r1 = _mm_loadu_pd(&bsum_data[r]);
        _mm_storeu_pd(&UNt_data[r], _mm_add_pd(b_r, r1));
      }
      for (r = hi; r < vstride; r++) {
        UNt_data[r] += bsum_data[r];
      }
    }
    emxFree_real_T(&e_st, &bsum);
  }
  if (Z->size[1] == 0) {
    ibtile = Z->size[0] * Z->size[1];
    Z->size[0] = 1;
    Z->size[1] = D->size[1];
    emxEnsureCapacity_real_T(sp, Z, ibtile, &s_emlrtRTEI);
    Z_data = Z->data;
    for (r = 0; r < loop_ub; r++) {
      Z_data[r] = 0.0;
    }
  }
  emxInit_real_T(sp, &b_N, 2, &y_emlrtRTEI);
  if ((int32_T)nClosed > 0) {
    st.site = &c_emlrtRSI;
    emxInit_real_T(&st, &x, 2, &t_emlrtRTEI);
    ibtile = x->size[0] * x->size[1];
    x->size[0] = D->size[0];
    x->size[1] = b_loop_ub;
    emxEnsureCapacity_real_T(&st, x, ibtile, &t_emlrtRTEI);
    x_data = x->data;
    for (r = 0; r < b_loop_ub; r++) {
      for (ist = 0; ist < c_loop_ub; ist++) {
        if (((int32_T)closedClasses_data[r] < 1) ||
            ((int32_T)closedClasses_data[r] > loop_ub)) {
          emlrtDynamicBoundsCheckR2012b((int32_T)closedClasses_data[r], 1,
                                        loop_ub, &v_emlrtBCI, &st);
        }
        x_data[ist + x->size[0] * r] =
            D_data[ist + D->size[0] * ((int32_T)closedClasses_data[r] - 1)];
      }
    }
    emxInit_real_T(&st, &b, 2, &hb_emlrtRTEI);
    b_st.site = &c_emlrtRSI;
    c_st.site = &t_emlrtRSI;
    hi = UNt->size[0];
    ibtile = b->size[0] * b->size[1];
    b->size[0] = UNt->size[0];
    b->size[1] = (int32_T)nClosed;
    emxEnsureCapacity_real_T(&b_st, b, ibtile, &v_emlrtRTEI);
    bsum_data = b->data;
    ntilecols = (int32_T)nClosed;
    c_st.site = &u_emlrtRSI;
    if ((int32_T)nClosed > 2147483646) {
      d_st.site = &s_emlrtRSI;
      check_forloop_overflow_error(&d_st);
    }
    overflow = (UNt->size[0] > 2147483646);
    for (r = 0; r < ntilecols; r++) {
      ibtile = r * hi;
      c_st.site = &v_emlrtRSI;
      if (overflow) {
        d_st.site = &s_emlrtRSI;
        check_forloop_overflow_error(&d_st);
      }
      for (k = 0; k < hi; k++) {
        bsum_data[ibtile + k] = UNt_data[k];
      }
    }
    ibtile = b->size[0] * b->size[1];
    ntilecols = (ibtile / 2) << 1;
    hi = ntilecols - 2;
    for (r = 0; r <= hi; r += 2) {
      b_r = _mm_loadu_pd(&bsum_data[r]);
      _mm_storeu_pd(&bsum_data[r], _mm_sub_pd(_mm_set1_pd(1.0), b_r));
    }
    for (r = ntilecols; r < ibtile; r++) {
      bsum_data[r] = 1.0 - bsum_data[r];
    }
    b_st.site = &w_emlrtRSI;
    c_st.site = &x_emlrtRSI;
    if (((D->size[0] != 1) && (b->size[0] != 1) &&
         (D->size[0] != b->size[0])) ||
        ((b_loop_ub != 1) && (b->size[1] != 1) && (b_loop_ub != b->size[1]))) {
      emlrtErrorWithMessageIdR2018a(&c_st, &emlrtRTEI,
                                    "MATLAB:sizeDimensionsMustMatch",
                                    "MATLAB:sizeDimensionsMustMatch", 0);
    }
    emxInit_real_T(sp, &a__1, 2, &eb_emlrtRTEI);
    emxInit_real_T(sp, &XNc, 2, &eb_emlrtRTEI);
    emxInit_real_T(sp, &CNc, 2, &eb_emlrtRTEI);
    if ((x->size[0] == b->size[0]) && (x->size[1] == b->size[1])) {
      emxInit_real_T(sp, &b_x, 2, &w_emlrtRTEI);
      ibtile = b_x->size[0] * b_x->size[1];
      b_x->size[0] = x->size[0];
      b_x->size[1] = x->size[1];
      emxEnsureCapacity_real_T(sp, b_x, ibtile, &w_emlrtRTEI);
      QNc_data = b_x->data;
      ibtile = x->size[0] * x->size[1];
      ntilecols = (ibtile / 2) << 1;
      hi = ntilecols - 2;
      for (r = 0; r <= hi; r += 2) {
        b_r = _mm_loadu_pd(&x_data[r]);
        r1 = _mm_loadu_pd(&bsum_data[r]);
        _mm_storeu_pd(&QNc_data[r], _mm_div_pd(b_r, r1));
      }
      for (r = ntilecols; r < ibtile; r++) {
        QNc_data[r] = x_data[r] / bsum_data[r];
      }
      ibtile = b_N->size[0] * b_N->size[1];
      b_N->size[0] = 1;
      b_N->size[1] = b_loop_ub;
      emxEnsureCapacity_real_T(sp, b_N, ibtile, &y_emlrtRTEI);
      bsum_data = b_N->data;
      for (r = 0; r < b_loop_ub; r++) {
        if (((int32_T)closedClasses_data[r] < 1) ||
            ((int32_T)closedClasses_data[r] > N->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)closedClasses_data[r], 1,
                                        N->size[1], &tb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        bsum_data[r] = N_data[(int32_T)closedClasses_data[r] - 1];
      }
      emxInit_real_T(sp, &b_Z, 2, &ab_emlrtRTEI);
      ibtile = b_Z->size[0] * b_Z->size[1];
      b_Z->size[0] = 1;
      b_Z->size[1] = b_loop_ub;
      emxEnsureCapacity_real_T(sp, b_Z, ibtile, &ab_emlrtRTEI);
      bsum_data = b_Z->data;
      for (r = 0; r < b_loop_ub; r++) {
        if (((int32_T)closedClasses_data[r] < 1) ||
            ((int32_T)closedClasses_data[r] > Z->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)closedClasses_data[r], 1,
                                        Z->size[1], &vb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        bsum_data[r] = Z_data[(int32_T)closedClasses_data[r] - 1];
      }
      st.site = &d_emlrtRSI;
      b_lGN = pfqn_mva(&st, b_x, b_N, b_Z, mi, XNc, QNc, a__1, CNc);
      bsum_data = CNc->data;
      QNc_data = QNc->data;
      x_data = XNc->data;
      emxFree_real_T(sp, &b_Z);
      emxFree_real_T(sp, &b_x);
    } else {
      st.site = &d_emlrtRSI;
      b_lGN = binary_expand_op(&st, d_emlrtRSI, x, b, N, closedClasses, Z, mi,
                               XNc, QNc, a__1, CNc);
      bsum_data = CNc->data;
      QNc_data = QNc->data;
      x_data = XNc->data;
    }
    emxFree_real_T(sp, &b);
    emxFree_real_T(sp, &x);
    emxFree_real_T(sp, &a__1);
    emxInit_int32_T(sp, &r2, 2, &fb_emlrtRTEI);
    ibtile = r2->size[0] * r2->size[1];
    r2->size[0] = 1;
    xoffset = closedClasses->size[1];
    r2->size[1] = closedClasses->size[1];
    emxEnsureCapacity_int32_T(sp, r2, ibtile, &x_emlrtRTEI);
    r3 = r2->data;
    hi = XN->size[1];
    ntilecols = closedClasses->size[1];
    xblockoffset = (closedClasses->size[1] < 1600);
    if (xblockoffset) {
      for (i2 = 0; i2 < xoffset; i2++) {
        if (((int32_T)closedClasses_data[i2] < 1) ||
            ((int32_T)closedClasses_data[i2] > hi)) {
          emlrtDynamicBoundsCheckR2012b((int32_T)closedClasses_data[i2], 1, hi,
                                        &rb_emlrtBCI, (emlrtConstCTX)sp);
        }
        r3[i2] = (int32_T)closedClasses_data[i2];
      }
    } else {
      emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
      emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
      pfqn_mvamx_numThreads =
          emlrtAllocRegionTLSs(sp->tls, omp_in_parallel(),
                               omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_mvamx_numThreads) private(               \
        h_st, emlrtJBEnviron) firstprivate(emlrtHadParallelError)
      {
        if (setjmp(emlrtJBEnviron) == 0) {
          h_st.prev = sp;
          h_st.tls = emlrtAllocTLS((emlrtCTX)sp, omp_get_thread_num());
          h_st.site = NULL;
          emlrtSetJmpBuf(&h_st, &emlrtJBEnviron);
        } else {
          emlrtHadParallelError = true;
        }
#pragma omp for nowait
        for (i2 = 0; i2 < ntilecols; i2++) {
          if (emlrtHadParallelError) {
            continue;
          }
          if (setjmp(emlrtJBEnviron) == 0) {
            if (((int32_T)closedClasses_data[i2] < 1) ||
                ((int32_T)closedClasses_data[i2] > hi)) {
              emlrtDynamicBoundsCheckR2012b((int32_T)closedClasses_data[i2], 1,
                                            hi, &rb_emlrtBCI, &h_st);
            }
            r3[i2] = (int32_T)closedClasses_data[i2];
          } else {
            emlrtHadParallelError = true;
          }
        }
      }
      emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
      emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
    }
    hi = r2->size[1];
    ibtile = XNc->size[0] * XNc->size[1];
    if (r2->size[1] != ibtile) {
      emlrtSubAssignSizeCheck1dR2017a(r2->size[1], ibtile, &c_emlrtECI,
                                      (emlrtConstCTX)sp);
    }
    for (r = 0; r < hi; r++) {
      XN_data[r3[r] - 1] = x_data[r];
    }
    emxFree_int32_T(sp, &r2);
    emxFree_real_T(sp, &XNc);
    emxInit_int32_T(sp, &r4, 1, &gb_emlrtRTEI);
    ibtile = r4->size[0];
    r4->size[0] = xoffset;
    emxEnsureCapacity_int32_T(sp, r4, ibtile, &bb_emlrtRTEI);
    r3 = r4->data;
    if (xblockoffset) {
      for (i3 = 0; i3 < xoffset; i3++) {
        if (((int32_T)closedClasses_data[i3] < 1) ||
            ((int32_T)closedClasses_data[i3] > loop_ub)) {
          emlrtDynamicBoundsCheckR2012b((int32_T)closedClasses_data[i3], 1,
                                        loop_ub, &sb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        r3[i3] = (int32_T)closedClasses_data[i3] - 1;
      }
    } else {
      emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
      emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
      pfqn_mvamx_numThreads =
          emlrtAllocRegionTLSs(sp->tls, omp_in_parallel(),
                               omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_mvamx_numThreads) private(               \
        emlrtJBEnviron, i_st) firstprivate(emlrtHadParallelError)
      {
        if (setjmp(emlrtJBEnviron) == 0) {
          i_st.prev = sp;
          i_st.tls = emlrtAllocTLS((emlrtCTX)sp, omp_get_thread_num());
          i_st.site = NULL;
          emlrtSetJmpBuf(&i_st, &emlrtJBEnviron);
        } else {
          emlrtHadParallelError = true;
        }
#pragma omp for nowait
        for (i3 = 0; i3 < xoffset; i3++) {
          if (emlrtHadParallelError) {
            continue;
          }
          if (setjmp(emlrtJBEnviron) == 0) {
            if (((int32_T)closedClasses_data[i3] < 1) ||
                ((int32_T)closedClasses_data[i3] > loop_ub)) {
              emlrtDynamicBoundsCheckR2012b((int32_T)closedClasses_data[i3], 1,
                                            loop_ub, &sb_emlrtBCI, &i_st);
            }
            r3[i3] = (int32_T)closedClasses_data[i3] - 1;
          } else {
            emlrtHadParallelError = true;
          }
        }
      }
      emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
      emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
    }
    b_D[0] = c_loop_ub;
    hi = r4->size[0];
    b_D[1] = r4->size[0];
    emlrtSubAssignSizeCheckR2012b(&b_D[0], 2, &QNc->size[0], 2, &emlrtECI,
                                  (emlrtCTX)sp);
    for (r = 0; r < hi; r++) {
      for (ist = 0; ist < c_loop_ub; ist++) {
        QN_data[ist + QN->size[0] * r3[r]] = QNc_data[ist + c_loop_ub * r];
      }
    }
    ibtile = r4->size[0];
    r4->size[0] = xoffset;
    emxEnsureCapacity_int32_T(sp, r4, ibtile, &cb_emlrtRTEI);
    r3 = r4->data;
    if (xblockoffset) {
      for (i4 = 0; i4 < xoffset; i4++) {
        if (((int32_T)closedClasses_data[i4] < 1) ||
            ((int32_T)closedClasses_data[i4] > loop_ub)) {
          emlrtDynamicBoundsCheckR2012b((int32_T)closedClasses_data[i4], 1,
                                        loop_ub, &ub_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        r3[i4] = (int32_T)closedClasses_data[i4] - 1;
      }
    } else {
      emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
      emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
      pfqn_mvamx_numThreads =
          emlrtAllocRegionTLSs(sp->tls, omp_in_parallel(),
                               omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_mvamx_numThreads) private(               \
        emlrtJBEnviron, j_st) firstprivate(emlrtHadParallelError)
      {
        if (setjmp(emlrtJBEnviron) == 0) {
          j_st.prev = sp;
          j_st.tls = emlrtAllocTLS((emlrtCTX)sp, omp_get_thread_num());
          j_st.site = NULL;
          emlrtSetJmpBuf(&j_st, &emlrtJBEnviron);
        } else {
          emlrtHadParallelError = true;
        }
#pragma omp for nowait
        for (i4 = 0; i4 < xoffset; i4++) {
          if (emlrtHadParallelError) {
            continue;
          }
          if (setjmp(emlrtJBEnviron) == 0) {
            if (((int32_T)closedClasses_data[i4] < 1) ||
                ((int32_T)closedClasses_data[i4] > loop_ub)) {
              emlrtDynamicBoundsCheckR2012b((int32_T)closedClasses_data[i4], 1,
                                            loop_ub, &ub_emlrtBCI, &j_st);
            }
            r3[i4] = (int32_T)closedClasses_data[i4] - 1;
          } else {
            emlrtHadParallelError = true;
          }
        }
      }
      emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
      emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
    }
    b_D[0] = c_loop_ub;
    ibtile = r4->size[0];
    b_D[1] = r4->size[0];
    emlrtSubAssignSizeCheckR2012b(&b_D[0], 2, &CNc->size[0], 2, &b_emlrtECI,
                                  (emlrtCTX)sp);
    for (r = 0; r < ibtile; r++) {
      for (ist = 0; ist < c_loop_ub; ist++) {
        CN_data[ist + CN->size[0] * r3[r]] = bsum_data[ist + c_loop_ub * r];
      }
    }
    emxFree_int32_T(sp, &r4);
    emxFree_real_T(sp, &CNc);
  }
  for (r = 0; r < c_loop_ub; r++) {
    ibtile = (int32_T)nClosed;
    for (ist = 0; ist < ibtile; ist++) {
      if (ist + 1 > b_loop_ub) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, b_loop_ub, &c_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      nOpen = closedClasses_data[ist];
      if (((int32_T)nOpen < 1) || ((int32_T)nOpen > XN->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)nOpen, 1, XN->size[1],
                                      &w_emlrtBCI, (emlrtConstCTX)sp);
      }
      if (r + 1 > c_loop_ub) {
        emlrtDynamicBoundsCheckR2012b(r + 1, 1, c_loop_ub, &x_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (((int32_T)nOpen < 1) || ((int32_T)nOpen > loop_ub)) {
        emlrtDynamicBoundsCheckR2012b((int32_T)nOpen, 1, loop_ub, &y_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (r + 1 > UN->size[0]) {
        emlrtDynamicBoundsCheckR2012b(r + 1, 1, UN->size[0], &ab_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (((int32_T)nOpen < 1) || ((int32_T)nOpen > UN->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)nOpen, 1, UN->size[1],
                                      &bb_emlrtBCI, (emlrtConstCTX)sp);
      }
      UN_data[r + UN->size[0] * ((int32_T)nOpen - 1)] =
          XN_data[(int32_T)nOpen - 1] *
          D_data[r + D->size[0] * ((int32_T)nOpen - 1)];
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  emxFree_uint32_T(sp, &closedClasses);
  for (ist = 0; ist < c_loop_ub; ist++) {
    for (k = 0; k < i1; k++) {
      if (k + 1 > i) {
        emlrtDynamicBoundsCheckR2012b(k + 1, 1, i, &b_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      ntilecols = QNc->size[1];
      if ((QNc->size[0] == 0) || (QNc->size[1] == 0)) {
        nOpen = openClasses_data[k];
        if (ist + 1 > c_loop_ub) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, c_loop_ub, &cb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (((int32_T)nOpen < 1) || ((int32_T)nOpen > loop_ub)) {
          emlrtDynamicBoundsCheckR2012b((int32_T)nOpen, 1, loop_ub,
                                        &db_emlrtBCI, (emlrtConstCTX)sp);
        }
        if (ist + 1 > UNt->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, UNt->size[0], &eb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (ist + 1 > CN->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, CN->size[0], &fb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (((int32_T)nOpen < 1) || ((int32_T)nOpen > CN->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)nOpen, 1, CN->size[1],
                                        &gb_emlrtBCI, (emlrtConstCTX)sp);
        }
        CN_data[ist + CN->size[0] * ((int32_T)nOpen - 1)] =
            D_data[ist + D->size[0] * ((int32_T)nOpen - 1)] /
            (1.0 - UNt_data[ist]);
      } else {
        if (ist + 1 > QNc->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, QNc->size[0], &emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        ibtile = b_N->size[0] * b_N->size[1];
        b_N->size[0] = 1;
        b_N->size[1] = QNc->size[1];
        emxEnsureCapacity_real_T(sp, b_N, ibtile, &u_emlrtRTEI);
        bsum_data = b_N->data;
        for (r = 0; r < ntilecols; r++) {
          bsum_data[r] = QNc_data[ist + QNc->size[0] * r];
        }
        nOpen = openClasses_data[k];
        if (ist + 1 > c_loop_ub) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, c_loop_ub, &hb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (((int32_T)nOpen < 1) || ((int32_T)nOpen > loop_ub)) {
          emlrtDynamicBoundsCheckR2012b((int32_T)nOpen, 1, loop_ub,
                                        &ib_emlrtBCI, (emlrtConstCTX)sp);
        }
        if (ist + 1 > UNt->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, UNt->size[0], &jb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (ist + 1 > CN->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, CN->size[0], &kb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (((int32_T)nOpen < 1) || ((int32_T)nOpen > CN->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)nOpen, 1, CN->size[1],
                                        &lb_emlrtBCI, (emlrtConstCTX)sp);
        }
        st.site = &e_emlrtRSI;
        CN_data[ist + CN->size[0] * ((int32_T)nOpen - 1)] =
            D_data[ist + D->size[0] * ((int32_T)nOpen - 1)] *
            (1.0 + sum(&st, b_N)) / (1.0 - UNt_data[ist]);
      }
      if (ist + 1 > CN->size[0]) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, CN->size[0], &mb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (((int32_T)nOpen < 1) || ((int32_T)nOpen > CN->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)nOpen, 1, CN->size[1],
                                      &nb_emlrtBCI, (emlrtConstCTX)sp);
      }
      if (((int32_T)nOpen < 1) || ((int32_T)nOpen > XN->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)nOpen, 1, XN->size[1],
                                      &ob_emlrtBCI, (emlrtConstCTX)sp);
      }
      if (ist + 1 > QN->size[0]) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, QN->size[0], &pb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (((int32_T)nOpen < 1) || ((int32_T)nOpen > QN->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)nOpen, 1, QN->size[1],
                                      &qb_emlrtBCI, (emlrtConstCTX)sp);
      }
      QN_data[ist + QN->size[0] * ((int32_T)nOpen - 1)] =
          CN_data[ist + CN->size[0] * ((int32_T)nOpen - 1)] *
          XN_data[(int32_T)nOpen - 1];
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  emxFree_real_T(sp, &b_N);
  emxFree_real_T(sp, &UNt);
  emxFree_real_T(sp, &QNc);
  emxFree_uint32_T(sp, &openClasses);
  *lGN = b_lGN;
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (pfqn_mvamx.c) */
