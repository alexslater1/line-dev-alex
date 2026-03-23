/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_sqni.c
 *
 * Code generation for function 'pfqn_sqni'
 *
 */

/* Include files */
#include "pfqn_sqni.h"
#include "combineVectorElements.h"
#include "div.h"
#include "pfqn_sqni_data.h"
#include "pfqn_sqni_emxutil.h"
#include "pfqn_sqni_types.h"
#include "rt_nonfinite.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>

/* Variable Definitions */
static emlrtRSInfo emlrtRSI =
    {
        23,          /* lineNo */
        "pfqn_sqni", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pathName */
};

static emlrtRSInfo b_emlrtRSI =
    {
        27,          /* lineNo */
        "pfqn_sqni", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pathName */
};

static emlrtRSInfo c_emlrtRSI =
    {
        29,          /* lineNo */
        "pfqn_sqni", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pathName */
};

static emlrtRSInfo d_emlrtRSI =
    {
        42,          /* lineNo */
        "pfqn_sqni", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pathName */
};

static emlrtRSInfo e_emlrtRSI =
    {
        53,          /* lineNo */
        "pfqn_sqni", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pathName */
};

static emlrtRSInfo f_emlrtRSI =
    {
        57,          /* lineNo */
        "pfqn_sqni", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pathName */
};

static emlrtRSInfo g_emlrtRSI = {
    20,    /* lineNo */
    "sum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sum.m" /* pathName
                                                                     */
};

static emlrtRSInfo h_emlrtRSI =
    {
        34,               /* lineNo */
        "rdivide_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "rdivide_helper.m" /* pathName */
};

static emlrtRSInfo i_emlrtRSI = {
    53,    /* lineNo */
    "div", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/div.m" /* pathName
                                                                       */
};

static emlrtRSInfo j_emlrtRSI = {
    44,       /* lineNo */
    "mpower", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/matfun/mpower.m" /* pathName
                                                                       */
};

static emlrtBCInfo emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        37,          /* lineNo */
        16,          /* colNo */
        "N",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo b_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        38,          /* lineNo */
        16,          /* colNo */
        "L",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo c_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        39,          /* lineNo */
        16,          /* colNo */
        "Z",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtECInfo emlrtECI =
    {
        2,           /* nDims */
        42,          /* lineNo */
        18,          /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtECInfo b_emlrtECI =
    {
        2,           /* nDims */
        42,          /* lineNo */
        39,          /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtECInfo c_emlrtECI =
    {
        2,           /* nDims */
        42,          /* lineNo */
        52,          /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtECInfo d_emlrtECI =
    {
        2,           /* nDims */
        42,          /* lineNo */
        14,          /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtRTEInfo emlrtRTEI =
    {
        46,        /* lineNo */
        23,        /* colNo */
        "sumprod", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
        "sumprod.m" /* pName */
};

static emlrtRTEInfo b_emlrtRTEI = {
    13,                     /* lineNo */
    27,                     /* colNo */
    "assertCompatibleDims", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/shared/coder/coder/lib/+coder/+internal/"
    "assertCompatibleDims.m" /* pName */
};

static emlrtBCInfo d_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        31,          /* lineNo */
        18,          /* colNo */
        "N",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo e_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        31,          /* lineNo */
        24,          /* colNo */
        "Z",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo f_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        31,          /* lineNo */
        29,          /* colNo */
        "L",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo g_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        31,          /* lineNo */
        11,          /* colNo */
        "X",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo h_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        32,          /* lineNo */
        27,          /* colNo */
        "X",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo i_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        32,          /* lineNo */
        32,          /* colNo */
        "L",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo j_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        32,          /* lineNo */
        20,          /* colNo */
        "U",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo k_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        33,          /* lineNo */
        27,          /* colNo */
        "X",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo l_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        33,          /* lineNo */
        32,          /* colNo */
        "L",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo m_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        33,          /* lineNo */
        20,          /* colNo */
        "Q",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo n_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        64,          /* lineNo */
        10,          /* colNo */
        "Z",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo o_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        65,          /* lineNo */
        18,          /* colNo */
        "N",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo p_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        65,          /* lineNo */
        24,          /* colNo */
        "L",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo q_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        65,          /* lineNo */
        11,          /* colNo */
        "X",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo r_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        66,          /* lineNo */
        27,          /* colNo */
        "X",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo s_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        66,          /* lineNo */
        32,          /* colNo */
        "L",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo t_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        66,          /* lineNo */
        20,          /* colNo */
        "U",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo u_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        67,          /* lineNo */
        27,          /* colNo */
        "N",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo v_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        67,          /* lineNo */
        32,          /* colNo */
        "X",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo w_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        67,          /* lineNo */
        37,          /* colNo */
        "Z",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo x_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        67,          /* lineNo */
        20,          /* colNo */
        "Q",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo y_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        41,          /* lineNo */
        30,          /* colNo */
        "Nvec_1r",   /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo ab_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        51,          /* lineNo */
        15,          /* colNo */
        "X",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo bb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        46,          /* lineNo */
        36,          /* colNo */
        "Br",        /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo cb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        59,          /* lineNo */
        27,          /* colNo */
        "X",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo db_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        59,          /* lineNo */
        32,          /* colNo */
        "L",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo eb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        59,          /* lineNo */
        20,          /* colNo */
        "U",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo fb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        60,          /* lineNo */
        27,          /* colNo */
        "N",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo gb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        60,          /* lineNo */
        32,          /* colNo */
        "X",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo hb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        60,          /* lineNo */
        37,          /* colNo */
        "Z",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo ib_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        60,          /* lineNo */
        20,          /* colNo */
        "Q",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo jb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        57,          /* lineNo */
        15,          /* colNo */
        "X",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo kb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        31,          /* lineNo */
        22,          /* colNo */
        "Z",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo lb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        31,          /* lineNo */
        22,          /* colNo */
        "L",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo mb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        31,          /* lineNo */
        16,          /* colNo */
        "N",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo nb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        31,          /* lineNo */
        9,           /* colNo */
        "X",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo ob_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        32,          /* lineNo */
        25,          /* colNo */
        "X",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo pb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        32,          /* lineNo */
        25,          /* colNo */
        "L",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo qb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        32,          /* lineNo */
        9,           /* colNo */
        "U",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo rb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        33,          /* lineNo */
        25,          /* colNo */
        "X",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo sb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        33,          /* lineNo */
        25,          /* colNo */
        "L",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtBCInfo tb_emlrtBCI =
    {
        -1,          /* iFirst */
        -1,          /* iLast */
        33,          /* lineNo */
        9,           /* colNo */
        "Q",         /* aName */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m", /* pName */
        0              /* checkKind */
};

static emlrtRTEInfo c_emlrtRTEI =
    {
        24,          /* lineNo */
        1,           /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtRTEInfo d_emlrtRTEI =
    {
        25,          /* lineNo */
        1,           /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtRTEInfo e_emlrtRTEI =
    {
        26,          /* lineNo */
        1,           /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtRTEInfo f_emlrtRTEI =
    {
        24,          /* lineNo */
        5,           /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtRTEInfo g_emlrtRTEI =
    {
        40,          /* lineNo */
        9,           /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtRTEInfo h_emlrtRTEI =
    {
        42,          /* lineNo */
        52,          /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtRTEInfo i_emlrtRTEI =
    {
        42,          /* lineNo */
        56,          /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtRTEInfo j_emlrtRTEI =
    {
        42,          /* lineNo */
        39,          /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtRTEInfo k_emlrtRTEI =
    {
        34,               /* lineNo */
        1,                /* colNo */
        "rdivide_helper", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "rdivide_helper.m" /* pName */
};

static emlrtRTEInfo l_emlrtRTEI =
    {
        42,          /* lineNo */
        22,          /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtRTEInfo m_emlrtRTEI =
    {
        42,          /* lineNo */
        18,          /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtRTEInfo n_emlrtRTEI =
    {
        42,          /* lineNo */
        14,          /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtRTEInfo o_emlrtRTEI =
    {
        42,          /* lineNo */
        9,           /* colNo */
        "pfqn_sqni", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_sqni.m" /* pName */
};

static emlrtRSInfo l_emlrtRSI = {
    54,    /* lineNo */
    "div", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/div.m" /* pathName
                                                                       */
};

/* Function Declarations */
static void b_plus(const emlrtStack *sp, emxArray_real_T *in1,
                   const emxArray_real_T *in2);

static void b_times(const emlrtStack *sp, emxArray_real_T *in1,
                    const emxArray_real_T *in2);

static void c_plus(const emlrtStack *sp, emxArray_real_T *in1,
                   const emxArray_real_T *in2, const emxArray_real_T *in3);

static void plus(const emlrtStack *sp, emxArray_real_T *in1,
                 const emxArray_real_T *in2);

static void times(const emlrtStack *sp, emxArray_real_T *in1,
                  const emxArray_real_T *in2);

/* Function Definitions */
static void b_plus(const emlrtStack *sp, emxArray_real_T *in1,
                   const emxArray_real_T *in2)
{
  jmp_buf *volatile emlrtJBStack;
  emxArray_real_T *b_in2;
  const real_T *in2_data;
  real_T *b_in2_data;
  real_T *in1_data;
  int32_T b_plus_numThreads;
  int32_T i;
  int32_T i1;
  int32_T loop_ub;
  int32_T stride_0_1;
  int32_T stride_1_1;
  in2_data = in2->data;
  in1_data = in1->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &b_in2, &h_emlrtRTEI);
  stride_0_1 = b_in2->size[0] * b_in2->size[1];
  b_in2->size[0] = 1;
  if (in1->size[1] == 1) {
    loop_ub = in2->size[1];
  } else {
    loop_ub = in1->size[1];
  }
  b_in2->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, b_in2, stride_0_1, &h_emlrtRTEI);
  b_in2_data = b_in2->data;
  stride_0_1 = (in2->size[1] != 1);
  stride_1_1 = (in1->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      b_in2_data[i] = in2_data[i * stride_0_1] + in1_data[i * stride_1_1];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    b_plus_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(b_plus_numThreads)

    for (i = 0; i < loop_ub; i++) {
      b_in2_data[i] = in2_data[i * stride_0_1] + in1_data[i * stride_1_1];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[0] = 1;
  in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &h_emlrtRTEI);
  in1_data = in1->data;
  for (i1 = 0; i1 < loop_ub; i1++) {
    in1_data[i1] = b_in2_data[i1];
  }
  emxFree_real_T(sp, &b_in2);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

static void b_times(const emlrtStack *sp, emxArray_real_T *in1,
                    const emxArray_real_T *in2)
{
  jmp_buf *volatile emlrtJBStack;
  emxArray_real_T *b_in2;
  const real_T *in2_data;
  real_T *b_in2_data;
  real_T *in1_data;
  int32_T b_times_numThreads;
  int32_T i;
  int32_T i1;
  int32_T loop_ub;
  int32_T stride_0_1;
  int32_T stride_1_1;
  in2_data = in2->data;
  in1_data = in1->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &b_in2, &j_emlrtRTEI);
  stride_0_1 = b_in2->size[0] * b_in2->size[1];
  b_in2->size[0] = 1;
  if (in1->size[1] == 1) {
    loop_ub = in2->size[1];
  } else {
    loop_ub = in1->size[1];
  }
  b_in2->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, b_in2, stride_0_1, &j_emlrtRTEI);
  b_in2_data = b_in2->data;
  stride_0_1 = (in2->size[1] != 1);
  stride_1_1 = (in1->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      b_in2_data[i] = in2_data[i * stride_0_1] * in1_data[i * stride_1_1];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    b_times_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(b_times_numThreads)

    for (i = 0; i < loop_ub; i++) {
      b_in2_data[i] = in2_data[i * stride_0_1] * in1_data[i * stride_1_1];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[0] = 1;
  in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &j_emlrtRTEI);
  in1_data = in1->data;
  for (i1 = 0; i1 < loop_ub; i1++) {
    in1_data[i1] = b_in2_data[i1];
  }
  emxFree_real_T(sp, &b_in2);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

static void c_plus(const emlrtStack *sp, emxArray_real_T *in1,
                   const emxArray_real_T *in2, const emxArray_real_T *in3)
{
  jmp_buf *volatile emlrtJBStack;
  const real_T *in2_data;
  const real_T *in3_data;
  real_T *in1_data;
  int32_T c_plus_numThreads;
  int32_T i;
  int32_T loop_ub;
  int32_T stride_0_1;
  int32_T stride_1_1;
  in3_data = in3->data;
  in2_data = in2->data;
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[0] = 1;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &h_emlrtRTEI);
  if (in3->size[1] == 1) {
    loop_ub = in2->size[1];
  } else {
    loop_ub = in3->size[1];
  }
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &h_emlrtRTEI);
  in1_data = in1->data;
  stride_0_1 = (in2->size[1] != 1);
  stride_1_1 = (in3->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      in1_data[i] = in2_data[i * stride_0_1] + in3_data[i * stride_1_1];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    c_plus_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(c_plus_numThreads)

    for (i = 0; i < loop_ub; i++) {
      in1_data[i] = in2_data[i * stride_0_1] + in3_data[i * stride_1_1];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
}

static void plus(const emlrtStack *sp, emxArray_real_T *in1,
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
  int32_T plus_numThreads;
  int32_T stride_0_1;
  int32_T stride_1_1;
  in2_data = in2->data;
  in1_data = in1->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &b_in1, &m_emlrtRTEI);
  stride_0_1 = b_in1->size[0] * b_in1->size[1];
  b_in1->size[0] = 1;
  if (in2->size[1] == 1) {
    loop_ub = in1->size[1];
  } else {
    loop_ub = in2->size[1];
  }
  b_in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, b_in1, stride_0_1, &m_emlrtRTEI);
  b_in1_data = b_in1->data;
  stride_0_1 = (in1->size[1] != 1);
  stride_1_1 = (in2->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      b_in1_data[i] = in1_data[i * stride_0_1] + in2_data[i * stride_1_1];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    plus_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(plus_numThreads)

    for (i = 0; i < loop_ub; i++) {
      b_in1_data[i] = in1_data[i * stride_0_1] + in2_data[i * stride_1_1];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[0] = 1;
  in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &m_emlrtRTEI);
  in1_data = in1->data;
  for (i1 = 0; i1 < loop_ub; i1++) {
    in1_data[i1] = b_in1_data[i1];
  }
  emxFree_real_T(sp, &b_in1);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
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
  int32_T stride_1_1;
  int32_T times_numThreads;
  in2_data = in2->data;
  in1_data = in1->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &b_in1, &n_emlrtRTEI);
  stride_0_1 = b_in1->size[0] * b_in1->size[1];
  b_in1->size[0] = 1;
  if (in2->size[1] == 1) {
    loop_ub = in1->size[1];
  } else {
    loop_ub = in2->size[1];
  }
  b_in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, b_in1, stride_0_1, &n_emlrtRTEI);
  b_in1_data = b_in1->data;
  stride_0_1 = (in1->size[1] != 1);
  stride_1_1 = (in2->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      b_in1_data[i] = in1_data[i * stride_0_1] * in2_data[i * stride_1_1];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    times_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(times_numThreads)

    for (i = 0; i < loop_ub; i++) {
      b_in1_data[i] = in1_data[i * stride_0_1] * in2_data[i * stride_1_1];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[0] = 1;
  in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &n_emlrtRTEI);
  in1_data = in1->data;
  for (i1 = 0; i1 < loop_ub; i1++) {
    in1_data[i1] = b_in1_data[i1];
  }
  emxFree_real_T(sp, &b_in1);
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

void pfqn_sqni(const emlrtStack *sp, const emxArray_real_T *N,
               const emxArray_real_T *L, const emxArray_real_T *Z,
               emxArray_real_T *Q, emxArray_real_T *U, emxArray_real_T *X)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  emxArray_real_T *Br;
  emxArray_real_T *Nvec_1r;
  emxArray_real_T *y;
  const real_T *L_data;
  const real_T *N_data;
  const real_T *Z_data;
  real_T Nt;
  real_T b_Nt;
  real_T *Br_data;
  real_T *Nvec_1r_data;
  real_T *Q_data;
  real_T *U_data;
  real_T *X_data;
  real_T *y_data;
  int32_T b_loop_ub;
  int32_T b_r;
  int32_T c_loop_ub;
  int32_T d_loop_ub;
  int32_T i;
  int32_T loop_ub;
  int32_T r;
  int32_T scalarLB;
  boolean_T b;
  boolean_T b1;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  Z_data = Z->data;
  L_data = L->data;
  N_data = N->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /* { */
  /*  % @file pfqn_sqni.m */
  /*  % @brief Single Queue Network Iteration (SQNI) approximate solver. */
  /* } */
  /* { */
  /*  % @brief Single Queue Network Iteration (SQNI) approximate solver. */
  /*  % @fn pfqn_sqni(N, L, Z) */
  /*  % @param N Population vector. */
  /*  % @param L Service demand vector. */
  /*  % @param Z Think time vector. */
  /*  % @return Q Mean queue lengths. */
  /*  % @return U Utilization. */
  /*  % @return X System throughput. */
  /* } */
  st.site = &emlrtRSI;
  b_st.site = &g_emlrtRSI;
  Nt = combineVectorElements(N);
  scalarLB = X->size[0] * X->size[1];
  X->size[0] = 1;
  loop_ub = L->size[1];
  X->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, X, scalarLB, &c_emlrtRTEI);
  X_data = X->data;
  for (r = 0; r < loop_ub; r++) {
    X_data[r] = 0.0;
  }
  scalarLB = Q->size[0] * Q->size[1];
  Q->size[0] = 1;
  Q->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, Q, scalarLB, &d_emlrtRTEI);
  Q_data = Q->data;
  for (r = 0; r < loop_ub; r++) {
    Q_data[r] = 0.0;
  }
  scalarLB = U->size[0] * U->size[1];
  U->size[0] = 1;
  U->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, U, scalarLB, &e_emlrtRTEI);
  U_data = U->data;
  for (r = 0; r < loop_ub; r++) {
    U_data[r] = 0.0;
  }
  st.site = &b_emlrtRSI;
  b_st.site = &g_emlrtRSI;
  if (!(Nt <= 0.0)) {
    real_T Brsum;
    real_T U_tmp;
    real_T b_Br;
    real_T discriminant;
    st.site = &c_emlrtRSI;
    b_st.site = &g_emlrtRSI;
    if (Nt == 1.0) {
      int32_T vectorUB;
      scalarLB = X->size[0] * X->size[1];
      X->size[0] = 1;
      X->size[1] = L->size[1];
      emxEnsureCapacity_real_T(sp, X, scalarLB, &f_emlrtRTEI);
      X_data = X->data;
      scalarLB = (L->size[1] / 2) << 1;
      vectorUB = scalarLB - 2;
      for (r = 0; r <= vectorUB; r += 2) {
        __m128d c_r;
        if (r + 1 > Z->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, Z->size[1], &kb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > loop_ub) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, loop_ub, &lb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, N->size[1], &mb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > X->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, X->size[1], &nb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        _mm_storeu_pd(&X_data[r],
                      _mm_div_pd(_mm_loadu_pd(&N_data[r]),
                                 _mm_add_pd(_mm_loadu_pd(&Z_data[r]),
                                            _mm_loadu_pd(&L_data[r]))));
        if (r + 1 > X->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, X->size[1], &ob_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        c_r = _mm_loadu_pd(&X_data[r]);
        if (r + 1 > loop_ub) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, loop_ub, &pb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > U->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, U->size[1], &qb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        _mm_storeu_pd(&U_data[r], _mm_mul_pd(c_r, _mm_loadu_pd(&L_data[r])));
        if (r + 1 > X->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, X->size[1], &rb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        c_r = _mm_loadu_pd(&X_data[r]);
        if (r + 1 > loop_ub) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, loop_ub, &sb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > Q->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, Q->size[1], &tb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        _mm_storeu_pd(&Q_data[r], _mm_mul_pd(c_r, _mm_loadu_pd(&L_data[r])));
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b((emlrtConstCTX)sp);
        }
      }
      for (r = scalarLB; r < loop_ub; r++) {
        if (r + 1 > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, N->size[1], &d_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > Z->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, Z->size[1], &e_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > loop_ub) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, loop_ub, &f_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > X->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, X->size[1], &g_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        Brsum = L_data[r];
        X_data[r] = N_data[r] / (Z_data[r] + Brsum);
        if (r + 1 > X->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, X->size[1], &h_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > loop_ub) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, loop_ub, &i_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > U->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, U->size[1], &j_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        Brsum *= X_data[r];
        U_data[r] = Brsum;
        if (r + 1 > X->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, X->size[1], &k_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > loop_ub) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, loop_ub, &l_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > Q->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, Q->size[1], &m_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        Q_data[r] = Brsum;
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b((emlrtConstCTX)sp);
        }
      }
    } else {
      scalarLB = X->size[0] * X->size[1];
      X->size[0] = 1;
      X->size[1] = L->size[1];
      emxEnsureCapacity_real_T(sp, X, scalarLB, &f_emlrtRTEI);
      X_data = X->data;
      if (L->size[1] - 1 >= 0) {
        b_loop_ub = N->size[1];
        b = true;
        b_Nt = Nt - 2.0;
        c_loop_ub = L->size[1];
        b1 = true;
        d_loop_ub = L->size[1];
        i = L->size[1];
      }
      emxInit_real_T(sp, &Nvec_1r, &g_emlrtRTEI);
      emxInit_real_T(sp, &Br, &o_emlrtRTEI);
      emxInit_real_T(sp, &y, &h_emlrtRTEI);
      for (b_r = 0; b_r < loop_ub; b_r++) {
        __m128d c_r;
        __m128d r1;
        real_T Nvec_1r_tmp;
        int32_T e_loop_ub;
        int32_T vectorUB;
        if (b_r + 1 > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, N->size[1], &emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_r + 1 > loop_ub) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, loop_ub, &b_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        e_loop_ub = Z->size[1];
        if (b_r + 1 > Z->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, Z->size[1], &c_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        scalarLB = Nvec_1r->size[0] * Nvec_1r->size[1];
        Nvec_1r->size[0] = 1;
        Nvec_1r->size[1] = N->size[1];
        emxEnsureCapacity_real_T(sp, Nvec_1r, scalarLB, &g_emlrtRTEI);
        Nvec_1r_data = Nvec_1r->data;
        for (r = 0; r < b_loop_ub; r++) {
          Nvec_1r_data[r] = N_data[r];
        }
        if (b_r + 1 > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, N->size[1], &y_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        Nvec_1r_tmp = N_data[b_r];
        Nvec_1r_data[b_r] = Nvec_1r_tmp - 1.0;
        if ((Z->size[1] != loop_ub) && ((Z->size[1] != 1) && (loop_ub != 1))) {
          emlrtDimSizeImpxCheckR2021b(Z->size[1], loop_ub, &emlrtECI,
                                      (emlrtConstCTX)sp);
        }
        if ((Z->size[1] != Nvec_1r->size[1]) &&
            ((Z->size[1] != 1) && (Nvec_1r->size[1] != 1))) {
          emlrtDimSizeImpxCheckR2021b(Z->size[1], Nvec_1r->size[1], &b_emlrtECI,
                                      (emlrtConstCTX)sp);
        }
        if ((Z->size[1] != loop_ub) && ((Z->size[1] != 1) && (loop_ub != 1))) {
          emlrtDimSizeImpxCheckR2021b(Z->size[1], loop_ub, &c_emlrtECI,
                                      (emlrtConstCTX)sp);
        }
        if (Z->size[1] == L->size[1]) {
          scalarLB = Br->size[0] * Br->size[1];
          Br->size[0] = 1;
          Br->size[1] = Z->size[1];
          emxEnsureCapacity_real_T(sp, Br, scalarLB, &h_emlrtRTEI);
          Br_data = Br->data;
          scalarLB = (Z->size[1] / 2) << 1;
          vectorUB = scalarLB - 2;
          for (r = 0; r <= vectorUB; r += 2) {
            _mm_storeu_pd(&Br_data[r], _mm_add_pd(_mm_loadu_pd(&Z_data[r]),
                                                  _mm_loadu_pd(&L_data[r])));
          }
          for (r = scalarLB; r < e_loop_ub; r++) {
            Br_data[r] = Z_data[r] + L_data[r];
          }
        } else {
          st.site = &d_emlrtRSI;
          c_plus(&st, Br, Z, L);
          Br_data = Br->data;
        }
        st.site = &d_emlrtRSI;
        b_st.site = &g_emlrtRSI;
        if (!b) {
          emlrtErrorWithMessageIdR2018a(
              &b_st, &emlrtRTEI, "Coder:toolbox:autoDimIncompatibility",
              "Coder:toolbox:autoDimIncompatibility", 0);
        }
        scalarLB = y->size[0] * y->size[1];
        y->size[0] = 1;
        y->size[1] = loop_ub;
        emxEnsureCapacity_real_T(sp, y, scalarLB, &i_emlrtRTEI);
        y_data = y->data;
        scalarLB = (c_loop_ub / 2) << 1;
        vectorUB = scalarLB - 2;
        for (r = 0; r <= vectorUB; r += 2) {
          _mm_storeu_pd(&y_data[r], _mm_mul_pd(_mm_loadu_pd(&L_data[r]),
                                               _mm_set1_pd(b_Nt)));
        }
        for (r = scalarLB; r < c_loop_ub; r++) {
          y_data[r] = L_data[r] * b_Nt;
        }
        if ((Br->size[1] != y->size[1]) &&
            ((Br->size[1] != 1) && (y->size[1] != 1))) {
          emlrtDimSizeImpxCheckR2021b(Br->size[1], y->size[1], &c_emlrtECI,
                                      (emlrtConstCTX)sp);
        }
        st.site = &d_emlrtRSI;
        if (Z->size[1] == Nvec_1r->size[1]) {
          e_loop_ub = Z->size[1] - 1;
          scalarLB = Nvec_1r->size[0] * Nvec_1r->size[1];
          Nvec_1r->size[0] = 1;
          Nvec_1r->size[1] = Z->size[1];
          emxEnsureCapacity_real_T(&st, Nvec_1r, scalarLB, &j_emlrtRTEI);
          Nvec_1r_data = Nvec_1r->data;
          scalarLB = (Z->size[1] / 2) << 1;
          vectorUB = scalarLB - 2;
          for (r = 0; r <= vectorUB; r += 2) {
            c_r = _mm_loadu_pd(&Nvec_1r_data[r]);
            _mm_storeu_pd(&Nvec_1r_data[r],
                          _mm_mul_pd(_mm_loadu_pd(&Z_data[r]), c_r));
          }
          for (r = scalarLB; r <= e_loop_ub; r++) {
            Nvec_1r_data[r] *= Z_data[r];
          }
        } else {
          b_st.site = &d_emlrtRSI;
          b_times(&b_st, Nvec_1r, Z);
        }
        if (Br->size[1] == y->size[1]) {
          e_loop_ub = Br->size[1] - 1;
          scalarLB = y->size[0] * y->size[1];
          y->size[0] = 1;
          y->size[1] = Br->size[1];
          emxEnsureCapacity_real_T(&st, y, scalarLB, &h_emlrtRTEI);
          y_data = y->data;
          scalarLB = (Br->size[1] / 2) << 1;
          vectorUB = scalarLB - 2;
          for (r = 0; r <= vectorUB; r += 2) {
            c_r = _mm_loadu_pd(&Br_data[r]);
            r1 = _mm_loadu_pd(&y_data[r]);
            _mm_storeu_pd(&y_data[r], _mm_add_pd(c_r, r1));
          }
          for (r = scalarLB; r <= e_loop_ub; r++) {
            y_data[r] += Br_data[r];
          }
        } else {
          b_st.site = &d_emlrtRSI;
          b_plus(&b_st, y, Br);
          y_data = y->data;
        }
        b_st.site = &h_emlrtRSI;
        c_st.site = &i_emlrtRSI;
        if ((Nvec_1r->size[1] != 1) && (y->size[1] != 1) &&
            (Nvec_1r->size[1] != y->size[1])) {
          emlrtErrorWithMessageIdR2018a(&c_st, &b_emlrtRTEI,
                                        "MATLAB:sizeDimensionsMustMatch",
                                        "MATLAB:sizeDimensionsMustMatch", 0);
        }
        if (Nvec_1r->size[1] == y->size[1]) {
          e_loop_ub = Nvec_1r->size[1] - 1;
          scalarLB = Nvec_1r->size[0] * Nvec_1r->size[1];
          Nvec_1r->size[0] = 1;
          emxEnsureCapacity_real_T(&b_st, Nvec_1r, scalarLB, &k_emlrtRTEI);
          Nvec_1r_data = Nvec_1r->data;
          scalarLB = (Nvec_1r->size[1] / 2) << 1;
          vectorUB = scalarLB - 2;
          for (r = 0; r <= vectorUB; r += 2) {
            c_r = _mm_loadu_pd(&Nvec_1r_data[r]);
            r1 = _mm_loadu_pd(&y_data[r]);
            _mm_storeu_pd(&Nvec_1r_data[r], _mm_div_pd(c_r, r1));
          }
          for (r = scalarLB; r <= e_loop_ub; r++) {
            Nvec_1r_data[r] /= y_data[r];
          }
        } else {
          c_st.site = &l_emlrtRSI;
          b_rdivide(&c_st, Nvec_1r, y);
        }
        st.site = &d_emlrtRSI;
        b_st.site = &g_emlrtRSI;
        if (!b1) {
          emlrtErrorWithMessageIdR2018a(
              &b_st, &emlrtRTEI, "Coder:toolbox:autoDimIncompatibility",
              "Coder:toolbox:autoDimIncompatibility", 0);
        }
        st.site = &d_emlrtRSI;
        b_st.site = &g_emlrtRSI;
        Brsum = combineVectorElements(Nvec_1r);
        scalarLB = Nvec_1r->size[0] * Nvec_1r->size[1];
        Nvec_1r->size[0] = 1;
        Nvec_1r->size[1] = loop_ub;
        emxEnsureCapacity_real_T(sp, Nvec_1r, scalarLB, &l_emlrtRTEI);
        Nvec_1r_data = Nvec_1r->data;
        Brsum = (Nt - 1.0) - Brsum;
        scalarLB = (d_loop_ub / 2) << 1;
        vectorUB = scalarLB - 2;
        for (r = 0; r <= vectorUB; r += 2) {
          _mm_storeu_pd(&Nvec_1r_data[r], _mm_mul_pd(_mm_loadu_pd(&L_data[r]),
                                                     _mm_set1_pd(Brsum)));
        }
        for (r = scalarLB; r < d_loop_ub; r++) {
          Nvec_1r_data[r] = L_data[r] * Brsum;
        }
        if ((Br->size[1] != Nvec_1r->size[1]) &&
            ((Br->size[1] != 1) && (Nvec_1r->size[1] != 1))) {
          emlrtDimSizeImpxCheckR2021b(Br->size[1], Nvec_1r->size[1], &emlrtECI,
                                      (emlrtConstCTX)sp);
        }
        st.site = &d_emlrtRSI;
        if (Br->size[1] == Nvec_1r->size[1]) {
          e_loop_ub = Br->size[1] - 1;
          scalarLB = Br->size[0] * Br->size[1];
          Br->size[0] = 1;
          emxEnsureCapacity_real_T(&st, Br, scalarLB, &m_emlrtRTEI);
          Br_data = Br->data;
          scalarLB = (Br->size[1] / 2) << 1;
          vectorUB = scalarLB - 2;
          for (r = 0; r <= vectorUB; r += 2) {
            c_r = _mm_loadu_pd(&Br_data[r]);
            r1 = _mm_loadu_pd(&Nvec_1r_data[r]);
            _mm_storeu_pd(&Br_data[r], _mm_add_pd(c_r, r1));
          }
          for (r = scalarLB; r <= e_loop_ub; r++) {
            Br_data[r] += Nvec_1r_data[r];
          }
        } else {
          b_st.site = &d_emlrtRSI;
          plus(&b_st, Br, Nvec_1r);
        }
        b_st.site = &h_emlrtRSI;
        c_st.site = &i_emlrtRSI;
        if ((N->size[1] != 1) && (Br->size[1] != 1) &&
            (N->size[1] != Br->size[1])) {
          emlrtErrorWithMessageIdR2018a(&c_st, &b_emlrtRTEI,
                                        "MATLAB:sizeDimensionsMustMatch",
                                        "MATLAB:sizeDimensionsMustMatch", 0);
        }
        if (N->size[1] == Br->size[1]) {
          e_loop_ub = N->size[1] - 1;
          scalarLB = Br->size[0] * Br->size[1];
          Br->size[0] = 1;
          Br->size[1] = N->size[1];
          emxEnsureCapacity_real_T(&b_st, Br, scalarLB, &n_emlrtRTEI);
          Br_data = Br->data;
          scalarLB = (N->size[1] / 2) << 1;
          vectorUB = scalarLB - 2;
          for (r = 0; r <= vectorUB; r += 2) {
            c_r = _mm_loadu_pd(&Br_data[r]);
            _mm_storeu_pd(&Br_data[r],
                          _mm_div_pd(_mm_loadu_pd(&N_data[r]), c_r));
          }
          for (r = scalarLB; r <= e_loop_ub; r++) {
            Br_data[r] = N_data[r] / Br_data[r];
          }
        } else {
          c_st.site = &l_emlrtRSI;
          rdivide(&c_st, Br, N);
        }
        if ((Br->size[1] != Z->size[1]) &&
            ((Br->size[1] != 1) && (Z->size[1] != 1))) {
          emlrtDimSizeImpxCheckR2021b(Br->size[1], Z->size[1], &d_emlrtECI,
                                      (emlrtConstCTX)sp);
        }
        if (Br->size[1] == Z->size[1]) {
          e_loop_ub = Br->size[1] - 1;
          scalarLB = Br->size[0] * Br->size[1];
          Br->size[0] = 1;
          emxEnsureCapacity_real_T(sp, Br, scalarLB, &o_emlrtRTEI);
          Br_data = Br->data;
          scalarLB = (Br->size[1] / 2) << 1;
          vectorUB = scalarLB - 2;
          for (r = 0; r <= vectorUB; r += 2) {
            c_r = _mm_loadu_pd(&Br_data[r]);
            _mm_storeu_pd(&Br_data[r],
                          _mm_mul_pd(c_r, _mm_loadu_pd(&Z_data[r])));
          }
          for (r = scalarLB; r <= e_loop_ub; r++) {
            Br_data[r] *= Z_data[r];
          }
        } else {
          st.site = &d_emlrtRSI;
          times(&st, Br, Z);
          Br_data = Br->data;
        }
        Brsum = 0.0;
        for (r = 0; r < i; r++) {
          if (r != b_r) {
            if (r + 1 > Br->size[1]) {
              emlrtDynamicBoundsCheckR2012b(r + 1, 1, Br->size[1], &bb_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            Brsum += Br_data[r];
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b((emlrtConstCTX)sp);
          }
        }
        b_Br = L_data[b_r] * Brsum;
        if (L_data[b_r] == 0.0) {
          if (b_r + 1 > X->size[1]) {
            emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, X->size[1], &ab_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          X_data[b_r] = N_data[b_r] / Z_data[b_r];
        } else {
          st.site = &e_emlrtRSI;
          b_st.site = &j_emlrtRSI;
          st.site = &e_emlrtRSI;
          b_st.site = &j_emlrtRSI;
          st.site = &e_emlrtRSI;
          b_st.site = &j_emlrtRSI;
          st.site = &e_emlrtRSI;
          b_st.site = &j_emlrtRSI;
          discriminant = 2.0 * b_Br;
          U_tmp = 2.0 * L_data[b_r];
          discriminant = (((((b_Br * b_Br - discriminant * L_data[b_r] * Nt) -
                             discriminant * Z_data[b_r]) +
                            L_data[b_r] * L_data[b_r] * (Nt * Nt)) +
                           U_tmp * Nt * Z_data[b_r]) -
                          4.0 * N_data[b_r] * L_data[b_r] * Z_data[b_r]) +
                         Z_data[b_r] * Z_data[b_r];
          if (discriminant < 0.0) {
            discriminant = 0.0;
          }
          st.site = &f_emlrtRSI;
          if (b_r + 1 > X->size[1]) {
            emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, X->size[1], &jb_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          X_data[b_r] =
              (((Z_data[b_r] - muDoubleScalarSqrt(discriminant)) - b_Br) +
               L_data[b_r] * Nt) /
              (U_tmp * Z_data[b_r]);
        }
        if (b_r + 1 > X->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, X->size[1], &cb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_r + 1 > loop_ub) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, loop_ub, &db_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_r + 1 > U->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, U->size[1], &eb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        discriminant = X_data[b_r];
        U_data[b_r] = discriminant * L_data[b_r];
        if (b_r + 1 > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, N->size[1], &fb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_r + 1 > X->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, X->size[1], &gb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_r + 1 > Z->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, Z->size[1], &hb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_r + 1 > Q->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, Q->size[1], &ib_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        Q_data[b_r] = Nvec_1r_tmp - discriminant * Z_data[b_r];
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b((emlrtConstCTX)sp);
        }
      }
      emxFree_real_T(sp, &y);
      emxFree_real_T(sp, &Br);
      emxFree_real_T(sp, &Nvec_1r);
    }
    for (r = 0; r < loop_ub; r++) {
      if (r + 1 > Z->size[1]) {
        emlrtDynamicBoundsCheckR2012b(r + 1, 1, Z->size[1], &n_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      Brsum = Z_data[r];
      if (Brsum == 0.0) {
        if (r + 1 > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, N->size[1], &o_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > loop_ub) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, loop_ub, &p_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > X->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, X->size[1], &q_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        discriminant = L_data[r];
        b_Br = N_data[r];
        X_data[r] = b_Br / (discriminant * (combineVectorElements(Q) + 1.0));
        if (r + 1 > X->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, X->size[1], &r_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > loop_ub) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, loop_ub, &s_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > U->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, U->size[1], &t_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        U_tmp = X_data[r];
        U_data[r] = U_tmp * discriminant;
        if (r + 1 > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, N->size[1], &u_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > X->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, X->size[1], &v_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > Z->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, Z->size[1], &w_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (r + 1 > Q->size[1]) {
          emlrtDynamicBoundsCheckR2012b(r + 1, 1, Q->size[1], &x_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        Q_data[r] = b_Br - U_tmp * Brsum;
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
  }
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (pfqn_sqni.c) */
