/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_ca.c
 *
 * Code generation for function 'pfqn_ca'
 *
 */

/* Include files */
#include "pfqn_ca.h"
#include "applyScalarFunctionInPlace.h"
#include "eml_int_forloop_overflow_check.h"
#include "gammaln.h"
#include "pfqn_ca_data.h"
#include "pfqn_ca_emxutil.h"
#include "pfqn_ca_types.h"
#include "prod.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "sumMatrixIncludeNaN.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>

/* Variable Definitions */
static emlrtRSInfo
    emlrtRSI =
        {
            25,        /* lineNo */
            "pfqn_ca", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pathName */
};

static emlrtRSInfo
    b_emlrtRSI =
        {
            30,        /* lineNo */
            "pfqn_ca", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pathName */
};

static emlrtRSInfo
    c_emlrtRSI =
        {
            36,        /* lineNo */
            "pfqn_ca", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pathName */
};

static emlrtRSInfo
    d_emlrtRSI =
        {
            43,        /* lineNo */
            "pfqn_ca", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pathName */
};

static emlrtRSInfo
    e_emlrtRSI =
        {
            45,        /* lineNo */
            "pfqn_ca", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pathName */
};

static emlrtRSInfo
    f_emlrtRSI =
        {
            46,        /* lineNo */
            "pfqn_ca", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pathName */
};

static emlrtRSInfo
    g_emlrtRSI =
        {
            47,        /* lineNo */
            "pfqn_ca", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pathName */
};

static emlrtRSInfo
    h_emlrtRSI =
        {
            53,        /* lineNo */
            "pfqn_ca", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pathName */
};

static emlrtRSInfo
    i_emlrtRSI =
        {
            59,        /* lineNo */
            "pfqn_ca", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pathName */
};

static emlrtRSInfo
    j_emlrtRSI =
        {
            62,        /* lineNo */
            "pfqn_ca", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pathName */
};

static emlrtRSInfo k_emlrtRSI = {
    8,                                                             /* lineNo */
    "factln",                                                      /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/factln.m" /* pathName
                                                                    */
};

static emlrtRSInfo l_emlrtRSI = {
    10,        /* lineNo */
    "gammaln", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/gammaln.m" /* pathName
                                                                         */
};

static emlrtRSInfo gb_emlrtRSI = {
    107,                /* lineNo */
    "blockedSummation", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo hb_emlrtRSI = {
    41,                 /* lineNo */
    "sumMatrixColumns", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtRSInfo ib_emlrtRSI = {
    17,                                                           /* lineNo */
    "log",                                                        /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/log.m" /* pathName */
};

static emlrtRSInfo jb_emlrtRSI = {
    44,                           /* lineNo */
    "applyScalarFunctionInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunctionInPlace.m" /* pathName */
};

static emlrtRSInfo kb_emlrtRSI = {
    15,    /* lineNo */
    "min", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/min.m" /* pathName
                                                                     */
};

static emlrtRSInfo lb_emlrtRSI = {
    75,         /* lineNo */
    "minOrMax", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

static emlrtRSInfo mb_emlrtRSI = {
    121,       /* lineNo */
    "minimum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

static emlrtRSInfo nb_emlrtRSI =
    {
        273,             /* lineNo */
        "unaryMinOrMax", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo ob_emlrtRSI =
    {
        962,                    /* lineNo */
        "minRealVectorOmitNaN", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo pb_emlrtRSI = {
    73,                      /* lineNo */
    "vectorMinOrMaxInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "vectorMinOrMaxInPlace.m" /* pathName */
};

static emlrtRSInfo qb_emlrtRSI = {
    65,                      /* lineNo */
    "vectorMinOrMaxInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "vectorMinOrMaxInPlace.m" /* pathName */
};

static emlrtRSInfo rb_emlrtRSI = {
    114,         /* lineNo */
    "findFirst", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "vectorMinOrMaxInPlace.m" /* pathName */
};

static emlrtRSInfo sb_emlrtRSI = {
    131,                        /* lineNo */
    "minOrMaxRealVectorKernel", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "vectorMinOrMaxInPlace.m" /* pathName */
};

static emlrtRSInfo
    wb_emlrtRSI =
        {
            73,        /* lineNo */
            "hashpop", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pathName */
};

static emlrtRSInfo
    xb_emlrtRSI =
        {
            118,  /* lineNo */
            "Fz", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pathName */
};

static emlrtRSInfo
    yb_emlrtRSI =
        {
            125,  /* lineNo */
            "Fz", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pathName */
};

static emlrtRSInfo
    ac_emlrtRSI =
        {
            126,  /* lineNo */
            "Fz", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pathName */
};

static emlrtRSInfo bc_emlrtRSI = {
    17,                           /* lineNo */
    "applyScalarFunctionInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunctionInPlace.m" /* pathName */
};

static emlrtRSInfo
    cc_emlrtRSI =
        {
            96,      /* lineNo */
            "pprod", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pathName */
};

static emlrtRSInfo dc_emlrtRSI = {
    15,    /* lineNo */
    "sum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sum.m" /* pathName
                                                                     */
};

static emlrtBCInfo
    emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            73,        /* lineNo */
            29,        /* colNo */
            "N",       /* aName */
            "hashpop", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtRTEInfo emlrtRTEI = {
    14,                                                           /* lineNo */
    9,                                                            /* colNo */
    "log",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/log.m" /* pName */
};

static emlrtECInfo
    emlrtECI =
        {
            2,       /* nDims */
            96,      /* lineNo */
            8,       /* colNo */
            "pprod", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pName */
};

static emlrtBCInfo
    b_emlrtBCI =
        {
            -1,   /* iFirst */
            -1,   /* iLast */
            126,  /* lineNo */
            25,   /* colNo */
            "n",  /* aName */
            "Fz", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    c_emlrtBCI =
        {
            -1,   /* iFirst */
            -1,   /* iLast */
            125,  /* lineNo */
            19,   /* colNo */
            "Z",  /* aName */
            "Fz", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtRTEInfo c_emlrtRTEI =
    {
        198,             /* lineNo */
        27,              /* colNo */
        "unaryMinOrMax", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pName */
};

static emlrtRTEInfo
    e_emlrtRTEI =
        {
            48,        /* lineNo */
            11,        /* colNo */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pName */
};

static emlrtDCInfo
    emlrtDCI =
        {
            43,        /* lineNo */
            14,        /* colNo */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            1            /* checkKind */
};

static emlrtDCInfo
    b_emlrtDCI =
        {
            43,        /* lineNo */
            14,        /* colNo */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            4            /* checkKind */
};

static emlrtDCInfo
    c_emlrtDCI =
        {
            43,        /* lineNo */
            10,        /* colNo */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            1            /* checkKind */
};

static emlrtECInfo
    b_emlrtECI =
        {
            2,         /* nDims */
            25,        /* lineNo */
            34,        /* colNo */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pName */
};

static emlrtBCInfo
    d_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            61,        /* lineNo */
            6,         /* colNo */
            "G",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    e_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            61,        /* lineNo */
            10,        /* colNo */
            "G",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    f_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            47,        /* lineNo */
            9,         /* colNo */
            "G",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtDCInfo
    d_emlrtDCI =
        {
            47,        /* lineNo */
            9,         /* colNo */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            1            /* checkKind */
};

static emlrtBCInfo
    g_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            73,        /* lineNo */
            39,        /* colNo */
            "n",       /* aName */
            "hashpop", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    h_emlrtBCI =
        {
            -1,   /* iFirst */
            -1,   /* iLast */
            124,  /* lineNo */
            10,   /* colNo */
            "Z",  /* aName */
            "Fz", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    i_emlrtBCI =
        {
            -1,   /* iFirst */
            -1,   /* iLast */
            127,  /* lineNo */
            14,   /* colNo */
            "n",  /* aName */
            "Fz", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    j_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            49,        /* lineNo */
            23,        /* colNo */
            "G",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    k_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            49,        /* lineNo */
            27,        /* colNo */
            "G",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    l_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            49,        /* lineNo */
            11,        /* colNo */
            "G",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    m_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            49,        /* lineNo */
            13,        /* colNo */
            "G",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    n_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            51,        /* lineNo */
            18,        /* colNo */
            "n",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    o_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            52,        /* lineNo */
            26,        /* colNo */
            "n",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    p_emlrtBCI =
        {
            -1,   /* iFirst */
            -1,   /* iLast */
            125,  /* lineNo */
            25,   /* colNo */
            "n",  /* aName */
            "Fz", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    q_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            54,        /* lineNo */
            26,        /* colNo */
            "n",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    r_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            55,        /* lineNo */
            31,        /* colNo */
            "G",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    s_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            55,        /* lineNo */
            33,        /* colNo */
            "G",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    t_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            55,        /* lineNo */
            43,        /* colNo */
            "L",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    u_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            55,        /* lineNo */
            47,        /* colNo */
            "L",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    v_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            55,        /* lineNo */
            52,        /* colNo */
            "G",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    w_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            55,        /* lineNo */
            54,        /* colNo */
            "G",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtDCInfo
    e_emlrtDCI =
        {
            55,        /* lineNo */
            54,        /* colNo */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            1            /* checkKind */
};

static emlrtBCInfo
    x_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            55,        /* lineNo */
            19,        /* colNo */
            "G",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    y_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            55,        /* lineNo */
            21,        /* colNo */
            "G",       /* aName */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    ab_emlrtBCI =
        {
            -1,      /* iFirst */
            -1,      /* iLast */
            102,     /* lineNo */
            16,      /* colNo */
            "n",     /* aName */
            "pprod", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    bb_emlrtBCI =
        {
            -1,      /* iFirst */
            -1,      /* iLast */
            102,     /* lineNo */
            22,      /* colNo */
            "N",     /* aName */
            "pprod", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    cb_emlrtBCI =
        {
            -1,      /* iFirst */
            -1,      /* iLast */
            103,     /* lineNo */
            7,       /* colNo */
            "n",     /* aName */
            "pprod", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    db_emlrtBCI =
        {
            -1,      /* iFirst */
            -1,      /* iLast */
            110,     /* lineNo */
            8,       /* colNo */
            "n",     /* aName */
            "pprod", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m", /* pName */
            0            /* checkKind */
};

static emlrtRTEInfo
    h_emlrtRTEI =
        {
            22,        /* lineNo */
            5,         /* colNo */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pName */
};

static emlrtRTEInfo i_emlrtRTEI = {
    35,                    /* lineNo */
    20,                    /* colNo */
    "sumMatrixIncludeNaN", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pName */
};

static emlrtRTEInfo
    j_emlrtRTEI =
        {
            43,        /* lineNo */
            19,        /* colNo */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pName */
};

static emlrtRTEInfo
    k_emlrtRTEI =
        {
            43,        /* lineNo */
            1,         /* colNo */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pName */
};

static emlrtRTEInfo l_emlrtRTEI = {
    8,                                                             /* lineNo */
    1,                                                             /* colNo */
    "factln",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/factln.m" /* pName */
};

static emlrtRTEInfo
    m_emlrtRTEI =
        {
            44,        /* lineNo */
            1,         /* colNo */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pName */
};

static emlrtRTEInfo
    n_emlrtRTEI =
        {
            25,        /* lineNo */
            34,        /* colNo */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pName */
};

static emlrtRTEInfo
    o_emlrtRTEI =
        {
            73,        /* lineNo */
            25,        /* colNo */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pName */
};

static emlrtRTEInfo
    p_emlrtRTEI =
        {
            96,        /* lineNo */
            8,         /* colNo */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pName */
};

static emlrtRTEInfo
    q_emlrtRTEI =
        {
            59,        /* lineNo */
            5,         /* colNo */
            "pfqn_ca", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_ca.m" /* pName */
};

static emlrtRTEInfo r_emlrtRTEI = {
    8,                                                             /* lineNo */
    14,                                                            /* colNo */
    "factln",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/factln.m" /* pName */
};

/* Function Declarations */
static real_T binary_expand_op(const emlrtStack *sp, const emlrtRSInfo in1,
                               const emxArray_real_T *in2,
                               const emxArray_real_T *in3,
                               const emxArray_real_T *in4);

static void eq(const emlrtStack *sp, emxArray_boolean_T *in1,
               const emxArray_real_T *in2, const emxArray_real_T *in3);

/* Function Definitions */
static real_T binary_expand_op(const emlrtStack *sp, const emlrtRSInfo in1,
                               const emxArray_real_T *in2,
                               const emxArray_real_T *in3,
                               const emxArray_real_T *in4)
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack st;
  emxArray_real_T *b_in3;
  const real_T *in3_data;
  const real_T *in4_data;
  real_T out1;
  real_T *b_in3_data;
  int32_T binary_expand_op_numThreads;
  int32_T i;
  int32_T loop_ub;
  int32_T stride_0_1;
  int32_T stride_1_1;
  st.prev = sp;
  st.tls = sp->tls;
  in4_data = in4->data;
  in3_data = in3->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &b_in3, &n_emlrtRTEI);
  stride_0_1 = b_in3->size[0] * b_in3->size[1];
  b_in3->size[0] = 1;
  if (in4->size[1] == 1) {
    loop_ub = in3->size[1];
  } else {
    loop_ub = in4->size[1];
  }
  b_in3->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, b_in3, stride_0_1, &n_emlrtRTEI);
  b_in3_data = b_in3->data;
  stride_0_1 = (in3->size[1] != 1);
  stride_1_1 = (in4->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      b_in3_data[i] = in3_data[i * stride_0_1] * in4_data[i * stride_1_1];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    binary_expand_op_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(binary_expand_op_numThreads)

    for (i = 0; i < loop_ub; i++) {
      b_in3_data[i] = in3_data[i * stride_0_1] * in4_data[i * stride_1_1];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  st.site = (emlrtRSInfo *)&in1;
  out1 = -sum(&st, in2) + sum(&st, b_in3);
  emxFree_real_T(sp, &b_in3);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return out1;
}

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
  emxEnsureCapacity_boolean_T(sp, in1, stride_0_1, &p_emlrtRTEI);
  if (in3->size[1] == 1) {
    loop_ub = in2->size[1];
  } else {
    loop_ub = in3->size[1];
  }
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[1] = loop_ub;
  emxEnsureCapacity_boolean_T(sp, in1, stride_0_1, &p_emlrtRTEI);
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

void pfqn_ca(const emlrtStack *sp, const emxArray_real_T *L,
             const emxArray_real_T *N, emxArray_real_T *Z, real_T *Gn,
             real_T *lGn)
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack g_st;
  emlrtStack h_st;
  emlrtStack i_st;
  emlrtStack st;
  emxArray_boolean_T *x;
  emxArray_real_T *G;
  emxArray_real_T *b_N;
  emxArray_real_T *b_x;
  emxArray_real_T *n;
  const real_T *L_data;
  const real_T *N_data;
  real_T c_x;
  real_T *G_data;
  real_T *Z_data;
  real_T *b_N_data;
  real_T *n_data;
  int32_T M;
  int32_T b_k;
  int32_T b_r;
  int32_T c_r;
  int32_T col;
  int32_T d_r;
  int32_T k;
  int32_T nx;
  int32_T nz;
  int32_T pfqn_ca_numThreads;
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
  g_st.prev = &f_st;
  g_st.tls = f_st.tls;
  h_st.prev = &g_st;
  h_st.tls = g_st.tls;
  i_st.prev = &h_st;
  i_st.tls = h_st.tls;
  Z_data = Z->data;
  N_data = N->data;
  L_data = L->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /* { */
  /*  % @file pfqn_ca.m */
  /*  % @brief Convolution Algorithm for exact normalizing constant computation.
   */
  /* } */
  /* { */
  /*  % @brief Convolution Algorithm for exact normalizing constant computation.
   */
  /*  % @fn pfqn_ca(L, N, Z) */
  /*  % @param L Service demand matrix. */
  /*  % @param N Population vector. */
  /*  % @param Z Think time vector. */
  /*  % @return Gn Normalizing constant. */
  /*  % @return lGn Logarithm of the normalizing constant. */
  /* } */
  M = L->size[0];
  if (Z->size[1] == 0) {
    nx = Z->size[0] * Z->size[1];
    Z->size[0] = 1;
    nz = L->size[1];
    Z->size[1] = L->size[1];
    emxEnsureCapacity_real_T(sp, Z, nx, &h_emlrtRTEI);
    Z_data = Z->data;
    for (k = 0; k < nz; k++) {
      Z_data[k] = 0.0;
    }
  }
  emxInit_real_T(sp, &G, &k_emlrtRTEI);
  emxInit_real_T(sp, &n, &m_emlrtRTEI);
  n_data = n->data;
  emxInit_boolean_T(sp, &x, &p_emlrtRTEI);
  emxInit_real_T(sp, &b_x, &r_emlrtRTEI);
  emxInit_real_T(sp, &b_N, &j_emlrtRTEI);
  if (L->size[0] == 0) {
    int32_T loop_ub;
    boolean_T p;
    st.site = &emlrtRSI;
    b_st.site = &t_emlrtRSI;
    c_st.site = &u_emlrtRSI;
    d_st.site = &v_emlrtRSI;
    if (Z->size[1] == 0) {
      n->size[0] = 1;
      n->size[1] = 0;
    } else {
      e_st.site = &gb_emlrtRSI;
      f_st.site = &x_emlrtRSI;
      nx = n->size[0] * n->size[1];
      n->size[0] = 1;
      nz = Z->size[1];
      n->size[1] = Z->size[1];
      emxEnsureCapacity_real_T(&f_st, n, nx, &i_emlrtRTEI);
      n_data = n->data;
      nx = Z->size[1];
      g_st.site = &hb_emlrtRSI;
      if (Z->size[1] > 2147483646) {
        h_st.site = &o_emlrtRSI;
        check_forloop_overflow_error(&h_st);
      }
      if (Z->size[1] < 1600) {
        for (col = 0; col < nz; col++) {
          n_data[col] = c_sumColumnB(Z, col + 1);
        }
      } else {
        emlrtEnterParallelRegion(&f_st, omp_in_parallel());
        emlrtPushJmpBuf(&f_st, &emlrtJBStack);
        pfqn_ca_numThreads =
            emlrtAllocRegionTLSs(f_st.tls, omp_in_parallel(),
                                 omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_ca_numThreads)

        for (col = 0; col < nx; col++) {
          n_data[col] = c_sumColumnB(Z, col + 1);
        }
        emlrtPopJmpBuf(&f_st, &emlrtJBStack);
        emlrtExitParallelRegion(&f_st, omp_in_parallel());
      }
    }
    st.site = &emlrtRSI;
    p = false;
    nz = n->size[1];
    for (k = 0; k < nz; k++) {
      if (p || (n_data[k] < 0.0)) {
        p = true;
      }
    }
    if (p) {
      emlrtErrorWithMessageIdR2018a(
          &st, &emlrtRTEI, "Coder:toolbox:ElFunDomainError",
          "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
    }
    b_st.site = &ib_emlrtRSI;
    nx = n->size[1];
    c_st.site = &jb_emlrtRSI;
    if (n->size[1] > 2147483646) {
      d_st.site = &o_emlrtRSI;
      check_forloop_overflow_error(&d_st);
    }
    if (n->size[1] < 1600) {
      for (b_k = 0; b_k < nz; b_k++) {
        n_data[b_k] = muDoubleScalarLog(n_data[b_k]);
      }
    } else {
      emlrtEnterParallelRegion(&b_st, omp_in_parallel());
      emlrtPushJmpBuf(&b_st, &emlrtJBStack);
      pfqn_ca_numThreads =
          emlrtAllocRegionTLSs(b_st.tls, omp_in_parallel(),
                               omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_ca_numThreads)

      for (b_k = 0; b_k < nx; b_k++) {
        n_data[b_k] = muDoubleScalarLog(n_data[b_k]);
      }
      emlrtPopJmpBuf(&b_st, &emlrtJBStack);
      emlrtExitParallelRegion(&b_st, omp_in_parallel());
    }
    loop_ub = N->size[1];
    if ((N->size[1] != nz) && ((N->size[1] != 1) && (nz != 1))) {
      emlrtDimSizeImpxCheckR2021b(N->size[1], nz, &b_emlrtECI,
                                  (emlrtConstCTX)sp);
    }
    st.site = &emlrtRSI;
    /*  lf=FACTLN(n) */
    /*  Compure the logarithm of n!        */
    /*  */
    /*  Copyright (c) 2012-2026, Imperial College London */
    /*  All rights reserved.   */
    b_st.site = &k_emlrtRSI;
    nx = b_x->size[0] * b_x->size[1];
    b_x->size[0] = 1;
    b_x->size[1] = N->size[1];
    emxEnsureCapacity_real_T(&b_st, b_x, nx, &l_emlrtRTEI);
    Z_data = b_x->data;
    nz = (N->size[1] / 2) << 1;
    nx = nz - 2;
    for (k = 0; k <= nx; k += 2) {
      _mm_storeu_pd(&Z_data[k],
                    _mm_add_pd(_mm_loadu_pd(&N_data[k]), _mm_set1_pd(1.0)));
    }
    for (k = nz; k < loop_ub; k++) {
      Z_data[k] = N_data[k] + 1.0;
    }
    c_st.site = &l_emlrtRSI;
    applyScalarFunctionInPlace(&c_st, b_x);
    if (N->size[1] == n->size[1]) {
      nx = b_N->size[0] * b_N->size[1];
      b_N->size[0] = 1;
      b_N->size[1] = N->size[1];
      emxEnsureCapacity_real_T(sp, b_N, nx, &n_emlrtRTEI);
      b_N_data = b_N->data;
      nx = nz - 2;
      for (k = 0; k <= nx; k += 2) {
        __m128d r;
        r = _mm_loadu_pd(&n_data[k]);
        _mm_storeu_pd(&b_N_data[k], _mm_mul_pd(_mm_loadu_pd(&N_data[k]), r));
      }
      for (k = nz; k < loop_ub; k++) {
        b_N_data[k] = N_data[k] * n_data[k];
      }
      st.site = &emlrtRSI;
      *lGn = -sum(&st, b_x) + sum(&st, b_N);
    } else {
      st.site = &emlrtRSI;
      *lGn = binary_expand_op(&st, emlrtRSI, b_x, N, n);
    }
    *Gn = muDoubleScalarExp(*lGn);
  } else {
    real_T idxn_1r;
    int32_T last;
    boolean_T exitg1;
    st.site = &b_emlrtRSI;
    b_st.site = &kb_emlrtRSI;
    c_st.site = &lb_emlrtRSI;
    d_st.site = &mb_emlrtRSI;
    if (N->size[1] < 1) {
      emlrtErrorWithMessageIdR2018a(
          &d_st, &c_emlrtRTEI, "Coder:toolbox:eml_min_or_max_varDimZero",
          "Coder:toolbox:eml_min_or_max_varDimZero", 0);
    }
    e_st.site = &nb_emlrtRSI;
    f_st.site = &ob_emlrtRSI;
    last = N->size[1];
    if (N->size[1] <= 2) {
      if (N->size[1] == 1) {
        idxn_1r = N_data[0];
      } else if ((N_data[0] > N_data[1]) ||
                 (muDoubleScalarIsNaN(N_data[0]) &&
                  (!muDoubleScalarIsNaN(N_data[1])))) {
        idxn_1r = N_data[1];
      } else {
        idxn_1r = N_data[0];
      }
    } else {
      g_st.site = &qb_emlrtRSI;
      if (!muDoubleScalarIsNaN(N_data[0])) {
        nz = 1;
      } else {
        nz = 0;
        h_st.site = &rb_emlrtRSI;
        if (N->size[1] > 2147483646) {
          i_st.site = &o_emlrtRSI;
          check_forloop_overflow_error(&i_st);
        }
        nx = 2;
        exitg1 = false;
        while ((!exitg1) && (nx <= last)) {
          if (!muDoubleScalarIsNaN(N_data[nx - 1])) {
            nz = nx;
            exitg1 = true;
          } else {
            nx++;
          }
        }
      }
      if (nz == 0) {
        idxn_1r = N_data[0];
      } else {
        g_st.site = &pb_emlrtRSI;
        idxn_1r = N_data[nz - 1];
        nx = nz + 1;
        h_st.site = &sb_emlrtRSI;
        if ((nz + 1 <= N->size[1]) && (N->size[1] > 2147483646)) {
          i_st.site = &o_emlrtRSI;
          check_forloop_overflow_error(&i_st);
        }
        for (k = nx; k <= last; k++) {
          c_x = N_data[k - 1];
          if (idxn_1r > c_x) {
            idxn_1r = c_x;
          }
        }
      }
    }
    if (idxn_1r < 0.0) {
      *Gn = 0.0;
      *lGn = rtMinusInf;
    } else {
      st.site = &c_emlrtRSI;
      if (sum(&st, N) == 0.0) {
        *Gn = 1.0;
        *lGn = 0.0;
      } else {
        if ((real_T)L->size[0] + 1.0 != L->size[0] + 1) {
          emlrtIntegerCheckR2012b((real_T)L->size[0] + 1.0, &c_emlrtDCI,
                                  (emlrtConstCTX)sp);
        }
        nx = b_N->size[0] * b_N->size[1];
        b_N->size[0] = 1;
        b_N->size[1] = N->size[1];
        emxEnsureCapacity_real_T(sp, b_N, nx, &j_emlrtRTEI);
        b_N_data = b_N->data;
        nx = (N->size[1] / 2) << 1;
        nz = nx - 2;
        for (k = 0; k <= nz; k += 2) {
          _mm_storeu_pd(&b_N_data[k],
                        _mm_add_pd(_mm_loadu_pd(&N_data[k]), _mm_set1_pd(1.0)));
        }
        for (k = nx; k < last; k++) {
          b_N_data[k] = N_data[k] + 1.0;
        }
        st.site = &d_emlrtRSI;
        idxn_1r = prod(&st, b_N);
        if (!(idxn_1r >= 0.0)) {
          emlrtNonNegativeCheckR2012b(idxn_1r, &b_emlrtDCI, (emlrtConstCTX)sp);
        }
        if (idxn_1r != (int32_T)muDoubleScalarFloor(idxn_1r)) {
          emlrtIntegerCheckR2012b(idxn_1r, &emlrtDCI, (emlrtConstCTX)sp);
        }
        nx = G->size[0] * G->size[1];
        G->size[0] = L->size[0] + 1;
        G->size[1] = (int32_T)idxn_1r;
        emxEnsureCapacity_real_T(sp, G, nx, &k_emlrtRTEI);
        G_data = G->data;
        nx = (L->size[0] + 1) * (int32_T)idxn_1r;
        for (k = 0; k < nx; k++) {
          G_data[k] = 1.0;
        }
        /*  stores G across recursion */
        /*  [N]=PPROD(N,N) */
        /*  sequentially generate all vectors n: 0<=n<=N */
        /*  n=pprod(N) - init */
        /*  n=pprod(n,N) - next state */
        nx = n->size[0] * n->size[1];
        n->size[0] = 1;
        n->size[1] = N->size[1];
        emxEnsureCapacity_real_T(sp, n, nx, &m_emlrtRTEI);
        n_data = n->data;
        nx = N->size[1];
        for (k = 0; k < nx; k++) {
          n_data[k] = 0.0;
        }
        int32_T exitg11;
        do {
          exitg11 = 0;
          st.site = &e_emlrtRSI;
          if (sum(&st, n) != -1.0) {
            real_T idxn;
            int32_T loop_ub;
            st.site = &f_emlrtRSI;
            /*  IDX=HASHPOP(N,N,R,PRODS) */
            /*  hash a population vector in n: 0<=n<=N */
            idxn = 1.0;
            for (b_r = 0; b_r < last; b_r++) {
              if (b_r < 1) {
                loop_ub = 0;
              } else {
                if (b_r > last) {
                  emlrtDynamicBoundsCheckR2012b(b_r, 1, last, &emlrtBCI, &st);
                }
                loop_ub = b_r;
              }
              nx = b_N->size[0] * b_N->size[1];
              b_N->size[0] = 1;
              b_N->size[1] = loop_ub;
              emxEnsureCapacity_real_T(&st, b_N, nx, &o_emlrtRTEI);
              b_N_data = b_N->data;
              nx = (loop_ub / 2) << 1;
              nz = nx - 2;
              for (k = 0; k <= nz; k += 2) {
                _mm_storeu_pd(&b_N_data[k], _mm_add_pd(_mm_loadu_pd(&N_data[k]),
                                                       _mm_set1_pd(1.0)));
              }
              for (k = nx; k < loop_ub; k++) {
                b_N_data[k] = N_data[k] + 1.0;
              }
              if (b_r + 1 > n->size[1]) {
                emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, n->size[1],
                                              &g_emlrtBCI, &st);
              }
              b_st.site = &wb_emlrtRSI;
              idxn += prod(&b_st, b_N) * n_data[b_r];
              if (*emlrtBreakCheckR2012bFlagVar != 0) {
                emlrtBreakCheckR2012b(&st);
              }
            }
            st.site = &g_emlrtRSI;
            /*  F=FZ(Z,N) */
            b_st.site = &xb_emlrtRSI;
            if (sum(&b_st, n) == 0.0) {
              if (idxn != (int32_T)muDoubleScalarFloor(idxn)) {
                emlrtIntegerCheckR2012b(idxn, &d_emlrtDCI, &st);
              }
              if (((int32_T)idxn < 1) || ((int32_T)idxn > G->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)idxn, 1, G->size[1],
                                              &f_emlrtBCI, &st);
              }
              G_data[G->size[0] * ((int32_T)idxn - 1)] = 1.0;
            } else {
              idxn_1r = 0.0;
              nx = 0;
              int32_T exitg2;
              do {
                exitg2 = 0;
                if (nx <= n->size[1] - 1) {
                  if (nx + 1 > Z->size[1]) {
                    emlrtDynamicBoundsCheckR2012b(nx + 1, 1, Z->size[1],
                                                  &h_emlrtBCI, &st);
                  }
                  if (Z_data[nx] > 0.0) {
                    b_st.site = &yb_emlrtRSI;
                    if (nx + 1 > Z->size[1]) {
                      emlrtDynamicBoundsCheckR2012b(nx + 1, 1, Z->size[1],
                                                    &c_emlrtBCI, &b_st);
                    }
                    if (Z_data[nx] < 0.0) {
                      emlrtErrorWithMessageIdR2018a(
                          &b_st, &emlrtRTEI, "Coder:toolbox:ElFunDomainError",
                          "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
                    }
                    if (nx + 1 > n->size[1]) {
                      emlrtDynamicBoundsCheckR2012b(nx + 1, 1, n->size[1],
                                                    &p_emlrtBCI, &st);
                    }
                    idxn_1r += muDoubleScalarLog(Z_data[nx]) * n_data[nx];
                    b_st.site = &ac_emlrtRSI;
                    if (nx + 1 > n->size[1]) {
                      emlrtDynamicBoundsCheckR2012b(nx + 1, 1, n->size[1],
                                                    &b_emlrtBCI, &b_st);
                    }
                    c_st.site = &l_emlrtRSI;
                    c_x = n_data[nx] + 1.0;
                    d_st.site = &bc_emlrtRSI;
                    scalar_gammaln(&d_st, &c_x);
                    idxn_1r -= c_x;
                    nx++;
                  } else {
                    if (nx + 1 > n->size[1]) {
                      emlrtDynamicBoundsCheckR2012b(nx + 1, 1, n->size[1],
                                                    &i_emlrtBCI, &st);
                    }
                    if (n_data[nx] > 0.0) {
                      if (idxn != (int32_T)muDoubleScalarFloor(idxn)) {
                        emlrtIntegerCheckR2012b(idxn, &d_emlrtDCI, &st);
                      }
                      if (((int32_T)idxn < 1) || ((int32_T)idxn > G->size[1])) {
                        emlrtDynamicBoundsCheckR2012b(
                            (int32_T)idxn, 1, G->size[1], &f_emlrtBCI, &st);
                      }
                      G_data[G->size[0] * ((int32_T)idxn - 1)] = 0.0;
                      exitg2 = 1;
                    } else {
                      nx++;
                    }
                  }
                } else {
                  if (idxn != (int32_T)muDoubleScalarFloor(idxn)) {
                    emlrtIntegerCheckR2012b(idxn, &d_emlrtDCI, &st);
                  }
                  if (((int32_T)idxn < 1) || ((int32_T)idxn > G->size[1])) {
                    emlrtDynamicBoundsCheckR2012b((int32_T)idxn, 1, G->size[1],
                                                  &f_emlrtBCI, &st);
                  }
                  G_data[G->size[0] * ((int32_T)idxn - 1)] =
                      muDoubleScalarExp(idxn_1r);
                  exitg2 = 1;
                }
                if (*emlrtBreakCheckR2012bFlagVar != 0) {
                  emlrtBreakCheckR2012b(&st);
                }
              } while (exitg2 == 0);
            }
            emlrtForLoopVectorCheckR2021a(2.0, 1.0, (real_T)M + 1.0,
                                          mxDOUBLE_CLASS, M, &e_emlrtRTEI,
                                          (emlrtConstCTX)sp);
            loop_ub = L->size[1];
            for (b_r = 0; b_r < M; b_r++) {
              if ((int32_T)((uint32_T)b_r + 1U) > G->size[0]) {
                emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_r + 1U), 1,
                                              G->size[0], &j_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (((int32_T)idxn < 1) || ((int32_T)idxn > G->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)idxn, 1, G->size[1],
                                              &k_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (((int32_T)((uint32_T)b_r + 2U) < 1) ||
                  ((int32_T)((uint32_T)b_r + 2U) > G->size[0])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_r + 2U), 1,
                                              G->size[0], &l_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (((int32_T)idxn < 1) || ((int32_T)idxn > G->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)idxn, 1, G->size[1],
                                              &m_emlrtBCI, (emlrtConstCTX)sp);
              }
              G_data[(b_r + G->size[0] * ((int32_T)idxn - 1)) + 1] =
                  G_data[b_r + G->size[0] * ((int32_T)idxn - 1)];
              /*  norm constant with m-1 queues */
              for (c_r = 0; c_r < loop_ub; c_r++) {
                if (c_r + 1 > n->size[1]) {
                  emlrtDynamicBoundsCheckR2012b(c_r + 1, 1, n->size[1],
                                                &n_emlrtBCI, (emlrtConstCTX)sp);
                }
                idxn_1r = n_data[c_r];
                if (idxn_1r >= 1.0) {
                  if (c_r + 1 > n->size[1]) {
                    emlrtDynamicBoundsCheckR2012b(
                        c_r + 1, 1, n->size[1], &o_emlrtBCI, (emlrtConstCTX)sp);
                  }
                  n_data[c_r] = idxn_1r - 1.0;
                  st.site = &h_emlrtRSI;
                  /*  IDX=HASHPOP(N,N,R,PRODS) */
                  /*  hash a population vector in n: 0<=n<=N */
                  idxn_1r = 1.0;
                  for (d_r = 0; d_r < last; d_r++) {
                    int32_T b_loop_ub;
                    if (d_r < 1) {
                      b_loop_ub = 0;
                    } else {
                      if (d_r > last) {
                        emlrtDynamicBoundsCheckR2012b(d_r, 1, last, &emlrtBCI,
                                                      &st);
                      }
                      b_loop_ub = d_r;
                    }
                    nx = b_N->size[0] * b_N->size[1];
                    b_N->size[0] = 1;
                    b_N->size[1] = b_loop_ub;
                    emxEnsureCapacity_real_T(&st, b_N, nx, &o_emlrtRTEI);
                    b_N_data = b_N->data;
                    nx = (b_loop_ub / 2) << 1;
                    nz = nx - 2;
                    for (k = 0; k <= nz; k += 2) {
                      _mm_storeu_pd(&b_N_data[k],
                                    _mm_add_pd(_mm_loadu_pd(&N_data[k]),
                                               _mm_set1_pd(1.0)));
                    }
                    for (k = nx; k < b_loop_ub; k++) {
                      b_N_data[k] = N_data[k] + 1.0;
                    }
                    if (d_r + 1 > n->size[1]) {
                      emlrtDynamicBoundsCheckR2012b(d_r + 1, 1, n->size[1],
                                                    &g_emlrtBCI, &st);
                    }
                    b_st.site = &wb_emlrtRSI;
                    idxn_1r += prod(&b_st, b_N) * n_data[d_r];
                    if (*emlrtBreakCheckR2012bFlagVar != 0) {
                      emlrtBreakCheckR2012b(&st);
                    }
                  }
                  if (c_r + 1 > n->size[1]) {
                    emlrtDynamicBoundsCheckR2012b(
                        c_r + 1, 1, n->size[1], &q_emlrtBCI, (emlrtConstCTX)sp);
                  }
                  n_data[c_r]++;
                  if (((int32_T)((uint32_T)b_r + 2U) < 1) ||
                      ((int32_T)((uint32_T)b_r + 2U) > G->size[0])) {
                    emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_r + 2U),
                                                  1, G->size[0], &r_emlrtBCI,
                                                  (emlrtConstCTX)sp);
                  }
                  if (((int32_T)idxn < 1) || ((int32_T)idxn > G->size[1])) {
                    emlrtDynamicBoundsCheckR2012b((int32_T)idxn, 1, G->size[1],
                                                  &s_emlrtBCI,
                                                  (emlrtConstCTX)sp);
                  }
                  if ((int32_T)((uint32_T)b_r + 1U) > M) {
                    emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_r + 1U),
                                                  1, M, &t_emlrtBCI,
                                                  (emlrtConstCTX)sp);
                  }
                  if (c_r + 1 > loop_ub) {
                    emlrtDynamicBoundsCheckR2012b(
                        c_r + 1, 1, loop_ub, &u_emlrtBCI, (emlrtConstCTX)sp);
                  }
                  if (((int32_T)((uint32_T)b_r + 2U) < 1) ||
                      ((int32_T)((uint32_T)b_r + 2U) > G->size[0])) {
                    emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_r + 2U),
                                                  1, G->size[0], &v_emlrtBCI,
                                                  (emlrtConstCTX)sp);
                  }
                  if (idxn_1r != (int32_T)muDoubleScalarFloor(idxn_1r)) {
                    emlrtIntegerCheckR2012b(idxn_1r, &e_emlrtDCI,
                                            (emlrtConstCTX)sp);
                  }
                  if (((int32_T)idxn_1r < 1) ||
                      ((int32_T)idxn_1r > G->size[1])) {
                    emlrtDynamicBoundsCheckR2012b((int32_T)idxn_1r, 1,
                                                  G->size[1], &w_emlrtBCI,
                                                  (emlrtConstCTX)sp);
                  }
                  if (((int32_T)((uint32_T)b_r + 2U) < 1) ||
                      ((int32_T)((uint32_T)b_r + 2U) > G->size[0])) {
                    emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_r + 2U),
                                                  1, G->size[0], &x_emlrtBCI,
                                                  (emlrtConstCTX)sp);
                  }
                  if (((int32_T)idxn < 1) || ((int32_T)idxn > G->size[1])) {
                    emlrtDynamicBoundsCheckR2012b((int32_T)idxn, 1, G->size[1],
                                                  &y_emlrtBCI,
                                                  (emlrtConstCTX)sp);
                  }
                  G_data[(b_r + G->size[0] * ((int32_T)idxn - 1)) + 1] +=
                      L_data[b_r + L->size[0] * c_r] *
                      G_data[(b_r + G->size[0] * ((int32_T)idxn_1r - 1)) + 1];
                }
                if (*emlrtBreakCheckR2012bFlagVar != 0) {
                  emlrtBreakCheckR2012b((emlrtConstCTX)sp);
                }
              }
              if (*emlrtBreakCheckR2012bFlagVar != 0) {
                emlrtBreakCheckR2012b((emlrtConstCTX)sp);
              }
            }
            st.site = &i_emlrtRSI;
            /*  [N]=PPROD(N,N) */
            /*  sequentially generate all vectors n: 0<=n<=N */
            /*  n=pprod(N) - init */
            /*  n=pprod(n,N) - next state */
            nz = n->size[1];
            if ((n->size[1] != last) && ((n->size[1] != 1) && (last != 1))) {
              emlrtDimSizeImpxCheckR2021b(n->size[1], last, &emlrtECI, &st);
            }
            b_st.site = &cc_emlrtRSI;
            if (n->size[1] == N->size[1]) {
              nx = x->size[0] * x->size[1];
              x->size[0] = 1;
              x->size[1] = n->size[1];
              emxEnsureCapacity_boolean_T(&b_st, x, nx, &p_emlrtRTEI);
              x_data = x->data;
              for (k = 0; k < nz; k++) {
                x_data[k] = (n_data[k] == N_data[k]);
              }
            } else {
              c_st.site = &cc_emlrtRSI;
              eq(&c_st, x, n, N);
              x_data = x->data;
            }
            c_st.site = &dc_emlrtRSI;
            d_st.site = &u_emlrtRSI;
            nx = x->size[1];
            if (x->size[1] == 0) {
              nz = 0;
            } else {
              e_st.site = &ub_emlrtRSI;
              nz = x_data[0];
              f_st.site = &vb_emlrtRSI;
              if (x->size[1] > 2147483646) {
                g_st.site = &o_emlrtRSI;
                check_forloop_overflow_error(&g_st);
              }
              for (k = 2; k <= nx; k++) {
                nz += x_data[k - 1];
              }
            }
            if (nz == N->size[1]) {
              nx = n->size[0] * n->size[1];
              n->size[0] = 1;
              n->size[1] = 1;
              emxEnsureCapacity_real_T(&st, n, nx, &q_emlrtRTEI);
              n_data = n->data;
              n_data[0] = -1.0;
            } else {
              nx = N->size[1];
              exitg1 = false;
              while ((!exitg1) && (nx > 0)) {
                if (nx > n->size[1]) {
                  emlrtDynamicBoundsCheckR2012b(nx, 1, n->size[1], &ab_emlrtBCI,
                                                &st);
                }
                if (nx > last) {
                  emlrtDynamicBoundsCheckR2012b(nx, 1, last, &bb_emlrtBCI, &st);
                }
                if (n_data[nx - 1] == N_data[nx - 1]) {
                  if (nx > n->size[1]) {
                    emlrtDynamicBoundsCheckR2012b(nx, 1, n->size[1],
                                                  &cb_emlrtBCI, &st);
                  }
                  n_data[nx - 1] = 0.0;
                  nx--;
                } else {
                  exitg1 = true;
                }
                if (*emlrtBreakCheckR2012bFlagVar != 0) {
                  emlrtBreakCheckR2012b(&st);
                }
              }
              if (nx != 0) {
                if (nx > n->size[1]) {
                  emlrtDynamicBoundsCheckR2012b(nx, 1, n->size[1], &db_emlrtBCI,
                                                &st);
                }
                n_data[nx - 1]++;
              } else {
                /* n=-1*ones(1,R); */
              }
            }
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b((emlrtConstCTX)sp);
            }
          } else {
            exitg11 = 1;
          }
        } while (exitg11 == 0);
        if (((int32_T)((uint32_T)L->size[0] + 1U) < 1) ||
            ((int32_T)((uint32_T)L->size[0] + 1U) > G->size[0])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)L->size[0] + 1U), 1,
                                        G->size[0], &d_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (G->size[1] < 1) {
          emlrtDynamicBoundsCheckR2012b(G->size[1], 1, G->size[1], &e_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        *Gn = G_data[L->size[0] + G->size[0] * (G->size[1] - 1)];
        st.site = &j_emlrtRSI;
        if (*Gn < 0.0) {
          emlrtErrorWithMessageIdR2018a(
              &st, &emlrtRTEI, "Coder:toolbox:ElFunDomainError",
              "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
        }
        *lGn = muDoubleScalarLog(*Gn);
      }
    }
  }
  emxFree_real_T(sp, &b_N);
  emxFree_real_T(sp, &b_x);
  emxFree_boolean_T(sp, &x);
  emxFree_real_T(sp, &n);
  emxFree_real_T(sp, &G);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (pfqn_ca.c) */
