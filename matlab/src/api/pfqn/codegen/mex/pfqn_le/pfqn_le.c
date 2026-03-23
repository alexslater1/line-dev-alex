/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_le.c
 *
 * Code generation for function 'pfqn_le'
 *
 */

/* Include files */
#include "pfqn_le.h"
#include "abs.h"
#include "applyScalarFunctionInPlace.h"
#include "det.h"
#include "eml_setop.h"
#include "log.h"
#include "mtimes.h"
#include "pfqn_le_data.h"
#include "pfqn_le_emxutil.h"
#include "pfqn_le_types.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>

/* Variable Definitions */
static emlrtRSInfo
    emlrtRSI =
        {
            44,        /* lineNo */
            "pfqn_le", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo
    b_emlrtRSI =
        {
            45,        /* lineNo */
            "pfqn_le", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo
    c_emlrtRSI =
        {
            57,        /* lineNo */
            "pfqn_le", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo
    d_emlrtRSI =
        {
            58,        /* lineNo */
            "pfqn_le", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo
    e_emlrtRSI =
        {
            61,        /* lineNo */
            "pfqn_le", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo
    f_emlrtRSI =
        {
            63,        /* lineNo */
            "pfqn_le", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo
    s_emlrtRSI =
        {
            208,      /* lineNo */
            "factln", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo t_emlrtRSI = {
    10,        /* lineNo */
    "gammaln", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/gammaln.m" /* pathName
                                                                         */
};

static emlrtRSInfo
    eb_emlrtRSI =
        {
            94,             /* lineNo */
            "pfqn_le_fpiZ", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo
    fb_emlrtRSI =
        {
            107,            /* lineNo */
            "pfqn_le_fpiZ", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo
    gb_emlrtRSI =
        {
            112,            /* lineNo */
            "pfqn_le_fpiZ", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo
    hb_emlrtRSI =
        {
            118,            /* lineNo */
            "pfqn_le_fpiZ", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo ib_emlrtRSI =
    {
        69,                  /* lineNo */
        "eml_mtimes_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pathName */
};

static emlrtRSInfo lb_emlrtRSI = {
    41,    /* lineNo */
    "cat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pathName
                                                                       */
};

static emlrtRSInfo mb_emlrtRSI = {
    65,         /* lineNo */
    "cat_impl", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pathName
                                                                       */
};

static emlrtRSInfo
    nb_emlrtRSI =
        {
            152,                /* lineNo */
            "pfqn_le_hessianZ", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo
    ob_emlrtRSI =
        {
            156,                /* lineNo */
            "pfqn_le_hessianZ", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo
    pb_emlrtRSI =
        {
            170,                /* lineNo */
            "pfqn_le_hessianZ", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo
    qb_emlrtRSI =
        {
            176,                /* lineNo */
            "pfqn_le_hessianZ", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo
    rb_emlrtRSI =
        {
            181,                /* lineNo */
            "pfqn_le_hessianZ", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo
    sb_emlrtRSI =
        {
            187,                /* lineNo */
            "pfqn_le_hessianZ", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo tb_emlrtRSI = {
    44,       /* lineNo */
    "mpower", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/matfun/mpower.m" /* pathName
                                                                       */
};

static emlrtRSInfo ub_emlrtRSI = {
    71,                                                           /* lineNo */
    "power",                                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

static emlrtRSInfo
    vb_emlrtRSI =
        {
            196,      /* lineNo */
            "allbut", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pathName */
};

static emlrtRSInfo wb_emlrtRSI = {
    19,        /* lineNo */
    "setdiff", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/setdiff.m" /* pathName
                                                                     */
};

static emlrtRSInfo xb_emlrtRSI =
    {
        97,          /* lineNo */
        "eml_setop", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/private/"
        "eml_setop.m" /* pathName */
};

static emlrtRTEInfo emlrtRTEI =
    {
        133,                   /* lineNo */
        23,                    /* colNo */
        "dynamic_size_checks", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

static emlrtRTEInfo b_emlrtRTEI =
    {
        138,                   /* lineNo */
        23,                    /* colNo */
        "dynamic_size_checks", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

static emlrtECInfo
    emlrtECI =
        {
            2,         /* nDims */
            45,        /* lineNo */
            34,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtBCInfo
    emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            61,        /* lineNo */
            36,        /* colNo */
            "L",       /* aName */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    b_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            107,            /* lineNo */
            82,             /* colNo */
            "L",            /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    c_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            112,            /* lineNo */
            40,             /* colNo */
            "L",            /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtRTEInfo c_emlrtRTEI = {
    225,                   /* lineNo */
    27,                    /* colNo */
    "check_non_axis_size", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pName
                                                                       */
};

static emlrtBCInfo
    d_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            156,                /* lineNo */
            31,                 /* colNo */
            "L",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    e_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            170,                /* lineNo */
            35,                 /* colNo */
            "csi",              /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    f_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            176,                /* lineNo */
            26,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    g_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            178,                /* lineNo */
            8,                  /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    h_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            178,                /* lineNo */
            16,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    i_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            181,                /* lineNo */
            24,                 /* colNo */
            "csi",              /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    j_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            181,                /* lineNo */
            46,                 /* colNo */
            "L",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    k_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            187,                /* lineNo */
            36,                 /* colNo */
            "csi",              /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    l_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            187,                /* lineNo */
            64,                 /* colNo */
            "L",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtRTEInfo e_emlrtRTEI = {
    13,                                                            /* lineNo */
    9,                                                             /* colNo */
    "sqrt",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/sqrt.m" /* pName */
};

static emlrtBCInfo
    m_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            105,            /* lineNo */
            11,             /* colNo */
            "u",            /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    n_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            116,            /* lineNo */
            16,             /* colNo */
            "xi",           /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    o_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            116,            /* lineNo */
            21,             /* colNo */
            "Z",            /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    p_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            112,            /* lineNo */
            17,             /* colNo */
            "N",            /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    q_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            112,            /* lineNo */
            23,             /* colNo */
            "Z",            /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    r_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            112,            /* lineNo */
            12,             /* colNo */
            "xi",           /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    s_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            161,                /* lineNo */
            21,                 /* colNo */
            "Z",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    t_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            161,                /* lineNo */
            28,                 /* colNo */
            "L",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    u_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            161,                /* lineNo */
            30,                 /* colNo */
            "L",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    v_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            161,                /* lineNo */
            14,                 /* colNo */
            "Lhat",             /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    w_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            161,                /* lineNo */
            16,                 /* colNo */
            "Lhat",             /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    x_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            107,            /* lineNo */
            22,             /* colNo */
            "u",            /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    y_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            107,            /* lineNo */
            30,             /* colNo */
            "N",            /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    ab_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            107,            /* lineNo */
            41,             /* colNo */
            "Z",            /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    bb_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            107,            /* lineNo */
            48,             /* colNo */
            "L",            /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    cb_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            107,            /* lineNo */
            52,             /* colNo */
            "L",            /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    db_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            107,            /* lineNo */
            60,             /* colNo */
            "u_1",          /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    eb_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            107,            /* lineNo */
            68,             /* colNo */
            "Z",            /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    fb_emlrtBCI =
        {
            -1,             /* iFirst */
            -1,             /* iLast */
            107,            /* lineNo */
            15,             /* colNo */
            "u",            /* aName */
            "pfqn_le_fpiZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    gb_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            156,                /* lineNo */
            14,                 /* colNo */
            "N",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    hb_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            156,                /* lineNo */
            20,                 /* colNo */
            "Z",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    ib_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            156,                /* lineNo */
            9,                  /* colNo */
            "csi",              /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    jb_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            168,                /* lineNo */
            27,                 /* colNo */
            "u",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    kb_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            168,                /* lineNo */
            32,                 /* colNo */
            "u",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    lb_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            168,                /* lineNo */
            15,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    mb_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            168,                /* lineNo */
            17,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    nb_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            179,                /* lineNo */
            3,                  /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    ob_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            179,                /* lineNo */
            5,                  /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    pb_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            183,                /* lineNo */
            12,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    qb_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            183,                /* lineNo */
            14,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    rb_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            183,                /* lineNo */
            3,                  /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    sb_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            183,                /* lineNo */
            5,                  /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtDCInfo
    emlrtDCI =
        {
            196,      /* lineNo */
            5,        /* colNo */
            "allbut", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            1            /* checkKind */
};

static emlrtBCInfo
    tb_emlrtBCI =
        {
            -1,       /* iFirst */
            -1,       /* iLast */
            196,      /* lineNo */
            5,        /* colNo */
            "y",      /* aName */
            "allbut", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    ub_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            185,                /* lineNo */
            7,                  /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    vb_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            185,                /* lineNo */
            9,                  /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    wb_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            176,                /* lineNo */
            7,                  /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    xb_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            176,                /* lineNo */
            9,                  /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    yb_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            189,                /* lineNo */
            14,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    ac_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            189,                /* lineNo */
            16,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    bc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            189,                /* lineNo */
            7,                  /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    cc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            189,                /* lineNo */
            9,                  /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    dc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            181,                /* lineNo */
            31,                 /* colNo */
            "N",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    ec_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            181,                /* lineNo */
            37,                 /* colNo */
            "Z",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    fc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            170,                /* lineNo */
            26,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    gc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            170,                /* lineNo */
            28,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    hc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            170,                /* lineNo */
            45,                 /* colNo */
            "Lhat",             /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    ic_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            170,                /* lineNo */
            47,                 /* colNo */
            "Lhat",             /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    jc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            170,                /* lineNo */
            55,                 /* colNo */
            "Lhat",             /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    kc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            170,                /* lineNo */
            57,                 /* colNo */
            "Lhat",             /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    lc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            170,                /* lineNo */
            63,                 /* colNo */
            "u",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    mc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            170,                /* lineNo */
            68,                 /* colNo */
            "u",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    nc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            170,                /* lineNo */
            74,                 /* colNo */
            "N",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    oc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            170,                /* lineNo */
            19,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    pc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            170,                /* lineNo */
            21,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    qc_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            61,        /* lineNo */
            18,        /* colNo */
            "Z",       /* aName */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    rc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            181,                /* lineNo */
            14,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    sc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            181,                /* lineNo */
            16,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    tc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            181,                /* lineNo */
            7,                  /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    uc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            181,                /* lineNo */
            9,                  /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    vc_emlrtBCI =
        {
            -1,        /* iFirst */
            -1,        /* iLast */
            61,        /* lineNo */
            9,         /* colNo */
            "N",       /* aName */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    wc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            187,                /* lineNo */
            18,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    xc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            187,                /* lineNo */
            20,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    yc_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            187,                /* lineNo */
            27,                 /* colNo */
            "u",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    ad_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            187,                /* lineNo */
            43,                 /* colNo */
            "N",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    bd_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            187,                /* lineNo */
            52,                 /* colNo */
            "Lhat",             /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    cd_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            187,                /* lineNo */
            54,                 /* colNo */
            "Lhat",             /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    dd_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            187,                /* lineNo */
            72,                 /* colNo */
            "csi",              /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    ed_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            187,                /* lineNo */
            77,                 /* colNo */
            "L",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    fd_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            187,                /* lineNo */
            79,                 /* colNo */
            "L",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    gd_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            187,                /* lineNo */
            11,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtBCInfo
    hd_emlrtBCI =
        {
            -1,                 /* iFirst */
            -1,                 /* iLast */
            187,                /* lineNo */
            13,                 /* colNo */
            "A",                /* aName */
            "pfqn_le_hessianZ", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m", /* pName */
            0            /* checkKind */
};

static emlrtRTEInfo
    o_emlrtRTEI =
        {
            95,        /* lineNo */
            1,         /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    p_emlrtRTEI =
        {
            208,       /* lineNo */
            1,         /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    q_emlrtRTEI =
        {
            97,        /* lineNo */
            1,         /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    r_emlrtRTEI =
        {
            101,       /* lineNo */
            12,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    s_emlrtRTEI =
        {
            45,        /* lineNo */
            34,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    t_emlrtRTEI =
        {
            102,       /* lineNo */
            5,         /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    u_emlrtRTEI =
        {
            57,        /* lineNo */
            5,         /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    v_emlrtRTEI =
        {
            58,        /* lineNo */
            5,         /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    w_emlrtRTEI =
        {
            58,        /* lineNo */
            7,         /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo x_emlrtRTEI =
    {
        74,                  /* lineNo */
        9,                   /* colNo */
        "eml_mtimes_helper", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

static emlrtRTEInfo y_emlrtRTEI =
    {
        76,                  /* lineNo */
        9,                   /* colNo */
        "eml_mtimes_helper", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

static emlrtRTEInfo
    ab_emlrtRTEI =
        {
            118,       /* lineNo */
            15,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    bb_emlrtRTEI =
        {
            118,       /* lineNo */
            11,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    cb_emlrtRTEI =
        {
            112,       /* lineNo */
            36,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    db_emlrtRTEI =
        {
            107,       /* lineNo */
            78,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    eb_emlrtRTEI =
        {
            156,       /* lineNo */
            27,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo fb_emlrtRTEI = {
    28,                                                           /* lineNo */
    9,                                                            /* colNo */
    "colon",                                                      /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/colon.m" /* pName */
};

static emlrtRTEInfo
    gb_emlrtRTEI =
        {
            196,       /* lineNo */
            3,         /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo hb_emlrtRTEI = {
    245,   /* lineNo */
    14,    /* colNo */
    "cat", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pName
                                                                       */
};

static emlrtRTEInfo
    ib_emlrtRTEI =
        {
            118,       /* lineNo */
            5,         /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    jb_emlrtRTEI =
        {
            181,       /* lineNo */
            42,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    kb_emlrtRTEI =
        {
            61,        /* lineNo */
            32,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    lb_emlrtRTEI =
        {
            58,        /* lineNo */
            30,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    mb_emlrtRTEI =
        {
            187,       /* lineNo */
            60,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    nb_emlrtRTEI =
        {
            63,        /* lineNo */
            93,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    ob_emlrtRTEI =
        {
            110,       /* lineNo */
            5,         /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    pb_emlrtRTEI =
        {
            89,        /* lineNo */
            15,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    qb_emlrtRTEI =
        {
            154,       /* lineNo */
            1,         /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    rb_emlrtRTEI =
        {
            158,       /* lineNo */
            1,         /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    sb_emlrtRTEI =
        {
            156,       /* lineNo */
            23,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

static emlrtRTEInfo
    tb_emlrtRTEI =
        {
            19,        /* lineNo */
            19,        /* colNo */
            "pfqn_le", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_le.m" /* pName */
};

/* Function Declarations */
static real_T binary_expand_op(const emlrtStack *sp, const emlrtRSInfo in1,
                               const emxArray_real_T *in2,
                               const emxArray_real_T *in3,
                               const emxArray_real_T *in4);

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
  emxInit_real_T(sp, &b_in3, 2, &s_emlrtRTEI);
  stride_0_1 = b_in3->size[0] * b_in3->size[1];
  b_in3->size[0] = 1;
  if (in4->size[1] == 1) {
    loop_ub = in3->size[1];
  } else {
    loop_ub = in4->size[1];
  }
  b_in3->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, b_in3, stride_0_1, &s_emlrtRTEI);
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

void pfqn_le(const emlrtStack *sp, const emxArray_real_T *L,
             const emxArray_real_T *N, const emxArray_real_T *Z, real_T *Gn,
             real_T *lGn)
{
  __m128d r1;
  __m128d r2;
  jmp_buf emlrtJBEnviron;
  jmp_buf *volatile emlrtJBStack;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack st;
  emxArray_int32_T *ia;
  emxArray_real_T c_L;
  emxArray_real_T *Lhat;
  emxArray_real_T *b_L;
  emxArray_real_T *b_d;
  emxArray_real_T *c_d;
  emxArray_real_T *csi;
  emxArray_real_T *d;
  emxArray_real_T *r;
  emxArray_real_T *u_1;
  emxArray_real_T *umax;
  emxArray_real_T *xi;
  emxArray_real_T *y;
  real_T dv[2];
  const real_T *L_data;
  const real_T *N_data;
  const real_T *Z_data;
  real_T b_vmax;
  real_T eta;
  real_T umax_tmp;
  real_T v_1;
  real_T vmax_tmp;
  real_T *Lhat_data;
  real_T *csi_data;
  real_T *d_data;
  real_T *u_1_data;
  real_T *umax_data;
  real_T *xi_data;
  real_T *y_data;
  int32_T R;
  int32_T b_k;
  int32_T b_loop_ub;
  int32_T b_r;
  int32_T b_scalarLB;
  int32_T c_r;
  int32_T exitg1;
  int32_T i;
  int32_T i1;
  int32_T i2;
  int32_T input_sizes_idx_0;
  int32_T ist;
  int32_T k;
  int32_T loop_ub;
  int32_T pfqn_le_numThreads;
  int32_T scalarLB;
  int32_T sizes_idx_0;
  int32_T vectorUB;
  int8_T b_sizes_idx_0;
  boolean_T b;
  boolean_T emlrtHadParallelError = false;
  boolean_T empty_non_axis_sizes;
  boolean_T guard1;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  f_st.prev = &d_st;
  f_st.tls = d_st.tls;
  Z_data = Z->data;
  N_data = N->data;
  L_data = L->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /* { */
  /*  % @file pfqn_le.m */
  /*  % @brief Logistic expansion (LE) asymptotic approximation for normalizing
   * constant. */
  /* } */
  /* { */
  /*  % @brief Logistic expansion (LE) asymptotic approximation for normalizing
   * constant. */
  /*  % @fn pfqn_le(L, N, Z) */
  /*  % @param L Service demand matrix (MxR). */
  /*  % @param N Population vector (1xR). */
  /*  % @param Z Think time vector (1xR). */
  /*  % @return Gn Estimated normalizing constant. */
  /*  % @return lGn Logarithm of normalizing constant. */
  /* } */
  /*  [GN,LGN]=PFQN_LE(L,N,Z) */
  /*  PFQN_LE Asymptotic solution of closed product-form queueing networks by */
  /*  logistic expansion */
  /*  */
  /*  [Gn,lGn]=pfqn_le(L,N,Z) */
  /*  Input: */
  /*  L : MxR demand matrix. L(i,r) is the demand of class-r at queue i */
  /*  N : 1xR population vector. N(r) is the number of jobs in class r */
  /*  Z : 1xR think time vector. Z(r) is the total think time of class r */
  /*  */
  /*  Output: */
  /*  Gn : estimated normalizing constat */
  /*  lGn: logarithm of Gn. If Gn exceeds the floating-point range, only lGn */
  /*       will be correctly estimated. */
  /*  */
  /*  Reference: */
  /*  G. Casale. Accelerating performance inference over closed systems by */
  /*  asymptotic methods. ACM SIGMETRICS 2017. */
  /*  Availble at: http://dl.acm.org/citation.cfm?id=3084445 */
  emxInit_real_T(sp, &umax, 1, &nb_emlrtRTEI);
  emxInit_real_T(sp, &u_1, 1, &q_emlrtRTEI);
  emxInit_real_T(sp, &xi, 2, &ob_emlrtRTEI);
  emxInit_real_T(sp, &d, 2, &pb_emlrtRTEI);
  d_data = d->data;
  emxInit_real_T(sp, &csi, 2, &qb_emlrtRTEI);
  emxInit_real_T(sp, &Lhat, 2, &rb_emlrtRTEI);
  emxInit_real_T(sp, &y, 2, &sb_emlrtRTEI);
  emxInit_int32_T(sp, &ia, 1, &tb_emlrtRTEI);
  emxInit_real_T(sp, &r, 1, &bb_emlrtRTEI);
  emxInit_real_T(sp, &b_L, 1, &eb_emlrtRTEI);
  emxInit_real_T(sp, &b_d, 2, &gb_emlrtRTEI);
  emxInit_real_T(sp, &c_d, 2, &hb_emlrtRTEI);
  guard1 = false;
  st.site = &emlrtRSI;
  if ((L->size[0] == 0) || (L->size[1] == 0) || (N->size[1] == 0) ||
      (sum(&st, N) == 0.0)) {
    guard1 = true;
  } else {
    i = L->size[0] * L->size[1];
    c_L = *L;
    i1 = i;
    c_L.size = &i1;
    c_L.numDimensions = 1;
    st.site = &emlrtRSI;
    if (b_sum(&st, &c_L) < 0.0001) {
      guard1 = true;
    } else {
      /*  Z>0 */
      st.site = &c_emlrtRSI;
      /*  [U,V,D]=PFQN_LE_FPIZ(L,N,Z) */
      /*  find location of mode of gaussian */
      R = L->size[1];
      b_st.site = &eb_emlrtRSI;
      eta = sum(&b_st, N) + (real_T)L->size[0];
      loop_ub = L->size[0];
      vectorUB = umax->size[0];
      umax->size[0] = L->size[0];
      emxEnsureCapacity_real_T(&st, umax, vectorUB, &o_emlrtRTEI);
      umax_data = umax->data;
      input_sizes_idx_0 = L->size[0];
      vmax_tmp = eta + 1.0;
      b_vmax = eta + 1.0;
      vectorUB = u_1->size[0];
      u_1->size[0] = L->size[0];
      emxEnsureCapacity_real_T(&st, u_1, vectorUB, &q_emlrtRTEI);
      u_1_data = u_1->data;
      if (L->size[0] < 1600) {
        for (i2 = 0; i2 < loop_ub; i2++) {
          umax_data[i2] = 1.0 / (real_T)L->size[0];
          u_1_data[i2] = rtInf;
        }
      } else {
        emlrtEnterParallelRegion(&st, omp_in_parallel());
        emlrtPushJmpBuf(&st, &emlrtJBStack);
        pfqn_le_numThreads =
            emlrtAllocRegionTLSs(st.tls, omp_in_parallel(),
                                 omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_le_numThreads)

        for (i2 = 0; i2 < input_sizes_idx_0; i2++) {
          umax_data[i2] = 1.0 / (real_T)L->size[0];
          u_1_data[i2] = rtInf;
        }
        emlrtPopJmpBuf(&st, &emlrtJBStack);
        emlrtExitParallelRegion(&st, omp_in_parallel());
      }
      d->size[0] = 0;
      d->size[1] = 0;
      do {
        exitg1 = 0;
        b_loop_ub = umax->size[0];
        vectorUB = u_1->size[0];
        u_1->size[0] = umax->size[0];
        emxEnsureCapacity_real_T(&st, u_1, vectorUB, &r_emlrtRTEI);
        u_1_data = u_1->data;
        b_scalarLB = (umax->size[0] / 2) << 1;
        vectorUB = b_scalarLB - 2;
        for (k = 0; k <= vectorUB; k += 2) {
          r1 = _mm_loadu_pd(&umax_data[k]);
          r2 = _mm_loadu_pd(&u_1_data[k]);
          _mm_storeu_pd(&u_1_data[k], _mm_sub_pd(r1, r2));
        }
        for (k = b_scalarLB; k < b_loop_ub; k++) {
          u_1_data[k] = umax_data[k] - u_1_data[k];
        }
        v_1 = 0.0;
        vectorUB = u_1->size[0];
        for (k = 0; k < vectorUB; k++) {
          v_1 += muDoubleScalarAbs(u_1_data[k]);
        }
        if (v_1 > 1.0E-10) {
          vectorUB = u_1->size[0];
          u_1->size[0] = umax->size[0];
          emxEnsureCapacity_real_T(&st, u_1, vectorUB, &t_emlrtRTEI);
          u_1_data = u_1->data;
          for (k = 0; k < b_loop_ub; k++) {
            u_1_data[k] = umax_data[k];
          }
          v_1 = b_vmax;
          scalarLB = b_scalarLB - 2;
          for (ist = 0; ist < loop_ub; ist++) {
            if (ist + 1 > umax->size[0]) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, umax->size[0],
                                            &m_emlrtBCI, &st);
            }
            umax_data[ist] = 1.0 / eta;
            for (b_r = 0; b_r < R; b_r++) {
              vectorUB = y->size[0] * y->size[1];
              y->size[0] = 1;
              y->size[1] = b_loop_ub;
              emxEnsureCapacity_real_T(&st, y, vectorUB, &x_emlrtRTEI);
              y_data = y->data;
              for (k = 0; k <= scalarLB; k += 2) {
                r1 = _mm_loadu_pd(&u_1_data[k]);
                _mm_storeu_pd(&y_data[k], _mm_mul_pd(_mm_set1_pd(b_vmax), r1));
              }
              for (k = b_scalarLB; k < b_loop_ub; k++) {
                y_data[k] = b_vmax * u_1_data[k];
              }
              b_st.site = &fb_emlrtRSI;
              if (b_r + 1 > R) {
                emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, R, &b_emlrtBCI,
                                              &b_st);
              }
              c_st.site = &ib_emlrtRSI;
              if (loop_ub != y->size[1]) {
                if ((y->size[1] == 1) || (loop_ub == 1)) {
                  emlrtErrorWithMessageIdR2018a(
                      &c_st, &emlrtRTEI,
                      "Coder:toolbox:mtimes_noDynamicScalarExpansion",
                      "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
                } else {
                  emlrtErrorWithMessageIdR2018a(&c_st, &b_emlrtRTEI,
                                                "MATLAB:innerdim",
                                                "MATLAB:innerdim", 0);
                }
              }
              vectorUB = b_L->size[0];
              b_L->size[0] = loop_ub;
              emxEnsureCapacity_real_T(&st, b_L, vectorUB, &db_emlrtRTEI);
              y_data = b_L->data;
              for (k = 0; k < loop_ub; k++) {
                y_data[k] = L_data[k + L->size[0] * b_r];
              }
              if (ist + 1 > umax->size[0]) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, umax->size[0],
                                              &x_emlrtBCI, &st);
              }
              if (b_r + 1 > N->size[1]) {
                emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, N->size[1],
                                              &y_emlrtBCI, &st);
              }
              if (b_r + 1 > Z->size[1]) {
                emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, Z->size[1],
                                              &ab_emlrtBCI, &st);
              }
              if (ist + 1 > loop_ub) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, loop_ub, &bb_emlrtBCI,
                                              &st);
              }
              if (b_r + 1 > R) {
                emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, R, &cb_emlrtBCI, &st);
              }
              if (ist + 1 > b_loop_ub) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, b_loop_ub,
                                              &db_emlrtBCI, &st);
              }
              if (b_r + 1 > Z->size[1]) {
                emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, Z->size[1],
                                              &eb_emlrtBCI, &st);
              }
              if (ist + 1 > umax->size[0]) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, umax->size[0],
                                              &fb_emlrtBCI, &st);
              }
              umax_tmp = Z_data[b_r];
              umax_data[ist] +=
                  N_data[b_r] / eta *
                  (umax_tmp + b_vmax * L_data[ist + L->size[0] * b_r]) *
                  u_1_data[ist] / (umax_tmp + mtimes(y, b_L));
              if (*emlrtBreakCheckR2012bFlagVar != 0) {
                emlrtBreakCheckR2012b(&st);
              }
            }
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b(&st);
            }
          }
          vectorUB = xi->size[0] * xi->size[1];
          xi->size[0] = 1;
          xi->size[1] = R;
          emxEnsureCapacity_real_T(&st, xi, vectorUB, &u_emlrtRTEI);
          xi_data = xi->data;
          scalarLB = b_scalarLB - 2;
          for (ist = 0; ist < R; ist++) {
            vectorUB = y->size[0] * y->size[1];
            y->size[0] = 1;
            y->size[1] = b_loop_ub;
            emxEnsureCapacity_real_T(&st, y, vectorUB, &x_emlrtRTEI);
            y_data = y->data;
            for (k = 0; k <= scalarLB; k += 2) {
              r1 = _mm_loadu_pd(&u_1_data[k]);
              _mm_storeu_pd(&y_data[k], _mm_mul_pd(_mm_set1_pd(b_vmax), r1));
            }
            for (k = b_scalarLB; k < b_loop_ub; k++) {
              y_data[k] = b_vmax * u_1_data[k];
            }
            b_st.site = &gb_emlrtRSI;
            if (ist + 1 > R) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, R, &c_emlrtBCI, &b_st);
            }
            c_st.site = &ib_emlrtRSI;
            if (loop_ub != y->size[1]) {
              if ((y->size[1] == 1) || (loop_ub == 1)) {
                emlrtErrorWithMessageIdR2018a(
                    &c_st, &emlrtRTEI,
                    "Coder:toolbox:mtimes_noDynamicScalarExpansion",
                    "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
              } else {
                emlrtErrorWithMessageIdR2018a(&c_st, &b_emlrtRTEI,
                                              "MATLAB:innerdim",
                                              "MATLAB:innerdim", 0);
              }
            }
            vectorUB = b_L->size[0];
            b_L->size[0] = loop_ub;
            emxEnsureCapacity_real_T(&st, b_L, vectorUB, &cb_emlrtRTEI);
            y_data = b_L->data;
            for (k = 0; k < loop_ub; k++) {
              y_data[k] = L_data[k + L->size[0] * ist];
            }
            if (ist + 1 > N->size[1]) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, N->size[1], &p_emlrtBCI,
                                            &st);
            }
            if (ist + 1 > Z->size[1]) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, Z->size[1], &q_emlrtBCI,
                                            &st);
            }
            if (ist + 1 > xi->size[1]) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, xi->size[1],
                                            &r_emlrtBCI, &st);
            }
            xi_data[ist] = N_data[ist] / (Z_data[ist] + mtimes(y, b_L));
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b(&st);
            }
          }
          b_vmax = vmax_tmp;
          for (k = 0; k < R; k++) {
            if (k + 1 > xi->size[1]) {
              emlrtDynamicBoundsCheckR2012b(k + 1, 1, xi->size[1], &n_emlrtBCI,
                                            &st);
            }
            if (k + 1 > Z->size[1]) {
              emlrtDynamicBoundsCheckR2012b(k + 1, 1, Z->size[1], &o_emlrtBCI,
                                            &st);
            }
            b_vmax -= xi_data[k] * Z_data[k];
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b(&st);
            }
          }
          b_st.site = &hb_emlrtRSI;
          v_1 = muDoubleScalarAbs(b_vmax - v_1);
          scalarLB = umax->size[0];
          vectorUB = b_L->size[0];
          b_L->size[0] = umax->size[0];
          emxEnsureCapacity_real_T(&b_st, b_L, vectorUB, &ab_emlrtRTEI);
          y_data = b_L->data;
          vectorUB = (umax->size[0] / 2) << 1;
          input_sizes_idx_0 = vectorUB - 2;
          for (k = 0; k <= input_sizes_idx_0; k += 2) {
            r1 = _mm_loadu_pd(&umax_data[k]);
            r2 = _mm_loadu_pd(&u_1_data[k]);
            _mm_storeu_pd(&y_data[k], _mm_sub_pd(r1, r2));
          }
          for (k = vectorUB; k < scalarLB; k++) {
            y_data[k] = umax_data[k] - u_1_data[k];
          }
          c_st.site = &hb_emlrtRSI;
          b_abs(&c_st, b_L, r);
          u_1_data = r->data;
          vectorUB = y->size[0] * y->size[1];
          y->size[0] = 1;
          scalarLB = r->size[0];
          y->size[1] = r->size[0];
          emxEnsureCapacity_real_T(&b_st, y, vectorUB, &bb_emlrtRTEI);
          y_data = y->data;
          vectorUB = (r->size[0] / 2) << 1;
          input_sizes_idx_0 = vectorUB - 2;
          for (k = 0; k <= input_sizes_idx_0; k += 2) {
            r1 = _mm_loadu_pd(&u_1_data[k]);
            _mm_storeu_pd(&y_data[k], _mm_add_pd(r1, _mm_set1_pd(v_1)));
          }
          for (k = vectorUB; k < scalarLB; k++) {
            y_data[k] = u_1_data[k] + v_1;
          }
          c_st.site = &lb_emlrtRSI;
          b = ((d->size[0] != 0) && (d->size[1] != 0));
          if (b) {
            scalarLB = d->size[1];
          } else if (y->size[1] != 0) {
            scalarLB = y->size[1];
          } else {
            scalarLB = d->size[1];
          }
          d_st.site = &mb_emlrtRSI;
          if ((d->size[1] != scalarLB) &&
              ((d->size[0] != 0) && (d->size[1] != 0))) {
            emlrtErrorWithMessageIdR2018a(
                &d_st, &c_emlrtRTEI, "MATLAB:catenate:matrixDimensionMismatch",
                "MATLAB:catenate:matrixDimensionMismatch", 0);
          }
          if ((y->size[1] != scalarLB) && (y->size[1] != 0)) {
            emlrtErrorWithMessageIdR2018a(
                &d_st, &c_emlrtRTEI, "MATLAB:catenate:matrixDimensionMismatch",
                "MATLAB:catenate:matrixDimensionMismatch", 0);
          }
          empty_non_axis_sizes = (scalarLB == 0);
          if (empty_non_axis_sizes || b) {
            input_sizes_idx_0 = d->size[0];
          } else {
            input_sizes_idx_0 = 0;
          }
          if (empty_non_axis_sizes || (y->size[1] != 0)) {
            b_sizes_idx_0 = 1;
          } else {
            b_sizes_idx_0 = 0;
          }
          sizes_idx_0 = b_sizes_idx_0;
          b_scalarLB = input_sizes_idx_0 + b_sizes_idx_0;
          vectorUB = c_d->size[0] * c_d->size[1];
          c_d->size[0] = b_scalarLB;
          c_d->size[1] = scalarLB;
          emxEnsureCapacity_real_T(&c_st, c_d, vectorUB, &hb_emlrtRTEI);
          u_1_data = c_d->data;
          for (k = 0; k < scalarLB; k++) {
            for (ist = 0; ist < input_sizes_idx_0; ist++) {
              u_1_data[ist + c_d->size[0] * k] =
                  d_data[ist + input_sizes_idx_0 * k];
            }
          }
          for (k = 0; k < scalarLB; k++) {
            for (ist = 0; ist < sizes_idx_0; ist++) {
              u_1_data[input_sizes_idx_0 + c_d->size[0] * k] =
                  y_data[b_sizes_idx_0 * k];
            }
          }
          vectorUB = d->size[0] * d->size[1];
          d->size[0] = b_scalarLB;
          d->size[1] = c_d->size[1];
          emxEnsureCapacity_real_T(&c_st, d, vectorUB, &ib_emlrtRTEI);
          d_data = d->data;
          vectorUB = c_d->size[0] * c_d->size[1];
          for (k = 0; k < vectorUB; k++) {
            d_data[k] = u_1_data[k];
          }
        } else {
          exitg1 = 1;
        }
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b(&st);
        }
      } while (exitg1 == 0);
      st.site = &d_emlrtRSI;
      /*  A=PFQN_LE_HESSIANZ(L,N,Z,U,V) */
      /*  find hessian of gaussian */
      b_st.site = &nb_emlrtRSI;
      v_1 = sum(&b_st, N);
      vectorUB = d->size[0] * d->size[1];
      d->size[0] = loop_ub;
      d->size[1] = loop_ub;
      emxEnsureCapacity_real_T(&st, d, vectorUB, &v_emlrtRTEI);
      d_data = d->data;
      vectorUB = L->size[0] * L->size[0];
      for (k = 0; k < vectorUB; k++) {
        d_data[k] = 0.0;
      }
      vectorUB = csi->size[0] * csi->size[1];
      csi->size[0] = 1;
      csi->size[1] = R;
      emxEnsureCapacity_real_T(&st, csi, vectorUB, &w_emlrtRTEI);
      csi_data = csi->data;
      scalarLB = b_scalarLB - 2;
      for (b_r = 0; b_r < R; b_r++) {
        vectorUB = y->size[0] * y->size[1];
        y->size[0] = 1;
        y->size[1] = b_loop_ub;
        emxEnsureCapacity_real_T(&st, y, vectorUB, &y_emlrtRTEI);
        y_data = y->data;
        for (k = 0; k <= scalarLB; k += 2) {
          r1 = _mm_loadu_pd(&umax_data[k]);
          _mm_storeu_pd(&y_data[k], _mm_mul_pd(_mm_set1_pd(b_vmax), r1));
        }
        for (k = b_scalarLB; k < b_loop_ub; k++) {
          y_data[k] = b_vmax * umax_data[k];
        }
        b_st.site = &ob_emlrtRSI;
        if (b_r + 1 > R) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, R, &d_emlrtBCI, &b_st);
        }
        c_st.site = &ib_emlrtRSI;
        if (L->size[0] != y->size[1]) {
          if ((y->size[1] == 1) || (L->size[0] == 1)) {
            emlrtErrorWithMessageIdR2018a(
                &c_st, &emlrtRTEI,
                "Coder:toolbox:mtimes_noDynamicScalarExpansion",
                "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
          } else {
            emlrtErrorWithMessageIdR2018a(
                &c_st, &b_emlrtRTEI, "MATLAB:innerdim", "MATLAB:innerdim", 0);
          }
        }
        vectorUB = b_L->size[0];
        b_L->size[0] = loop_ub;
        emxEnsureCapacity_real_T(&st, b_L, vectorUB, &eb_emlrtRTEI);
        y_data = b_L->data;
        for (k = 0; k < loop_ub; k++) {
          y_data[k] = L_data[k + L->size[0] * b_r];
        }
        if (b_r + 1 > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, N->size[1], &gb_emlrtBCI,
                                        &st);
        }
        if (b_r + 1 > Z->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, Z->size[1], &hb_emlrtBCI,
                                        &st);
        }
        if (b_r + 1 > csi->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, csi->size[1], &ib_emlrtBCI,
                                        &st);
        }
        csi_data[b_r] = N_data[b_r] / (Z_data[b_r] + mtimes(y, b_L));
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b(&st);
        }
      }
      vectorUB = Lhat->size[0] * Lhat->size[1];
      Lhat->size[0] = loop_ub;
      Lhat->size[1] = R;
      emxEnsureCapacity_real_T(&st, Lhat, vectorUB, &w_emlrtRTEI);
      Lhat_data = Lhat->data;
      if (i < 1600) {
        for (b_k = 0; b_k < loop_ub; b_k++) {
          for (c_r = 0; c_r < R; c_r++) {
            if (b_k + 1 > Lhat->size[0]) {
              emlrtDynamicBoundsCheckR2012b(b_k + 1, 1, Lhat->size[0],
                                            &v_emlrtBCI, &st);
            }
            if (c_r + 1 > Lhat->size[1]) {
              emlrtDynamicBoundsCheckR2012b(c_r + 1, 1, Lhat->size[1],
                                            &w_emlrtBCI, &st);
            }
            if (c_r + 1 > Z->size[1]) {
              emlrtDynamicBoundsCheckR2012b(c_r + 1, 1, Z->size[1], &s_emlrtBCI,
                                            &st);
            }
            if (b_k + 1 > loop_ub) {
              emlrtDynamicBoundsCheckR2012b(b_k + 1, 1, loop_ub, &t_emlrtBCI,
                                            &st);
            }
            if (c_r + 1 > R) {
              emlrtDynamicBoundsCheckR2012b(c_r + 1, 1, R, &u_emlrtBCI, &st);
            }
            Lhat_data[b_k + Lhat->size[0] * c_r] =
                Z_data[c_r] + b_vmax * L_data[b_k + L->size[0] * c_r];
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b(&st);
            }
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&st);
          }
        }
      } else {
        emlrtEnterParallelRegion(&st, omp_in_parallel());
        emlrtPushJmpBuf(&st, &emlrtJBStack);
        pfqn_le_numThreads =
            emlrtAllocRegionTLSs(st.tls, omp_in_parallel(),
                                 omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_le_numThreads) private(                  \
        c_r, e_st, emlrtJBEnviron) firstprivate(st, emlrtHadParallelError)
        {
          if (setjmp(emlrtJBEnviron) == 0) {
            e_st.prev = &st;
            e_st.tls = emlrtAllocTLS(&st, omp_get_thread_num());
            e_st.site = NULL;
            emlrtSetJmpBuf(&e_st, &emlrtJBEnviron);
          } else {
            emlrtHadParallelError = true;
          }
#pragma omp for nowait
          for (b_k = 0; b_k < loop_ub; b_k++) {
            if (emlrtHadParallelError) {
              continue;
            }
            if (setjmp(emlrtJBEnviron) == 0) {
              for (c_r = 0; c_r < R; c_r++) {
                if (c_r + 1 > Z->size[1]) {
                  emlrtDynamicBoundsCheckR2012b(c_r + 1, 1, Z->size[1],
                                                &s_emlrtBCI, &e_st);
                }
                if (b_k + 1 > L->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(b_k + 1, 1, L->size[0],
                                                &t_emlrtBCI, &e_st);
                }
                if (c_r + 1 > L->size[1]) {
                  emlrtDynamicBoundsCheckR2012b(c_r + 1, 1, L->size[1],
                                                &u_emlrtBCI, &e_st);
                }
                if (b_k + 1 > Lhat->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(b_k + 1, 1, Lhat->size[0],
                                                &v_emlrtBCI, &e_st);
                }
                if (c_r + 1 > Lhat->size[1]) {
                  emlrtDynamicBoundsCheckR2012b(c_r + 1, 1, Lhat->size[1],
                                                &w_emlrtBCI, &e_st);
                }
                Lhat_data[b_k + Lhat->size[0] * c_r] =
                    Z_data[c_r] + b_vmax * L_data[b_k + L->size[0] * c_r];
                if (*emlrtBreakCheckR2012bFlagVar != 0) {
                  emlrtBreakCheckR2012b(&e_st);
                }
              }
              if (*emlrtBreakCheckR2012bFlagVar != 0) {
                emlrtBreakCheckR2012b(&e_st);
              }
            } else {
              emlrtHadParallelError = true;
            }
          }
        }
        emlrtPopJmpBuf(&st, &emlrtJBStack);
        emlrtExitParallelRegion(&st, omp_in_parallel());
      }
      v_1 += (real_T)L->size[0];
      for (k = 0; k < loop_ub; k++) {
        for (ist = 0; ist < loop_ub; ist++) {
          if (k != ist) {
            if (k + 1 > b_loop_ub) {
              emlrtDynamicBoundsCheckR2012b(k + 1, 1, b_loop_ub, &jb_emlrtBCI,
                                            &st);
            }
            if (ist + 1 > b_loop_ub) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, b_loop_ub, &kb_emlrtBCI,
                                            &st);
            }
            if (k + 1 > d->size[0]) {
              emlrtDynamicBoundsCheckR2012b(k + 1, 1, d->size[0], &lb_emlrtBCI,
                                            &st);
            }
            if (ist + 1 > d->size[1]) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, d->size[1],
                                            &mb_emlrtBCI, &st);
            }
            eta = umax_data[k];
            umax_tmp = umax_data[ist];
            d_data[k + d->size[0] * ist] = -v_1 * eta * umax_tmp;
            for (b_r = 0; b_r < R; b_r++) {
              b_st.site = &pb_emlrtRSI;
              if (b_r + 1 > csi->size[1]) {
                emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, csi->size[1],
                                              &e_emlrtBCI, &b_st);
              }
              c_st.site = &tb_emlrtRSI;
              d_st.site = &ub_emlrtRSI;
              if (k + 1 > d->size[0]) {
                emlrtDynamicBoundsCheckR2012b(k + 1, 1, d->size[0],
                                              &fc_emlrtBCI, &st);
              }
              if (ist + 1 > d->size[1]) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, d->size[1],
                                              &gc_emlrtBCI, &st);
              }
              if (k + 1 > Lhat->size[0]) {
                emlrtDynamicBoundsCheckR2012b(k + 1, 1, Lhat->size[0],
                                              &hc_emlrtBCI, &st);
              }
              if (b_r + 1 > Lhat->size[1]) {
                emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, Lhat->size[1],
                                              &ic_emlrtBCI, &st);
              }
              if (ist + 1 > Lhat->size[0]) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, Lhat->size[0],
                                              &jc_emlrtBCI, &st);
              }
              if (b_r + 1 > Lhat->size[1]) {
                emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, Lhat->size[1],
                                              &kc_emlrtBCI, &st);
              }
              if (k + 1 > b_loop_ub) {
                emlrtDynamicBoundsCheckR2012b(k + 1, 1, b_loop_ub, &lc_emlrtBCI,
                                              &st);
              }
              if (ist + 1 > b_loop_ub) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, b_loop_ub,
                                              &mc_emlrtBCI, &st);
              }
              if (b_r + 1 > N->size[1]) {
                emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, N->size[1],
                                              &nc_emlrtBCI, &st);
              }
              if (k + 1 > d->size[0]) {
                emlrtDynamicBoundsCheckR2012b(k + 1, 1, d->size[0],
                                              &oc_emlrtBCI, &st);
              }
              if (ist + 1 > d->size[1]) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, d->size[1],
                                              &pc_emlrtBCI, &st);
              }
              d_data[k + d->size[0] * ist] +=
                  csi_data[b_r] * csi_data[b_r] *
                  Lhat_data[k + Lhat->size[0] * b_r] *
                  Lhat_data[ist + Lhat->size[0] * b_r] * (eta * umax_tmp) /
                  N_data[b_r];
              if (*emlrtBreakCheckR2012bFlagVar != 0) {
                emlrtBreakCheckR2012b(&st);
              }
            }
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&st);
          }
        }
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b(&st);
        }
      }
      for (ist = 0; ist < loop_ub; ist++) {
        b_st.site = &qb_emlrtRSI;
        if (ist + 1 > d->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, d->size[0], &f_emlrtBCI,
                                        &b_st);
        }
        /*  Y=ALLBUT(Y,XSET) */
        vectorUB = y->size[0] * y->size[1];
        y->size[0] = 1;
        sizes_idx_0 = d->size[1];
        y->size[1] = d->size[1];
        emxEnsureCapacity_real_T(&b_st, y, vectorUB, &fb_emlrtRTEI);
        y_data = y->data;
        vectorUB = d->size[1] - 1;
        scalarLB = (d->size[1] / 2) << 1;
        input_sizes_idx_0 = scalarLB - 2;
        for (k = 0; k <= input_sizes_idx_0; k += 2) {
          dv[0] = k;
          dv[1] = k + 1;
          r1 = _mm_loadu_pd(&dv[0]);
          _mm_storeu_pd(&y_data[k], _mm_add_pd(_mm_set1_pd(1.0), r1));
        }
        for (k = scalarLB; k <= vectorUB; k++) {
          y_data[k] = (real_T)k + 1.0;
        }
        c_st.site = &vb_emlrtRSI;
        d_st.site = &wb_emlrtRSI;
        f_st.site = &xb_emlrtRSI;
        do_vectors(&f_st, y, (real_T)ist + 1.0, xi, ia);
        xi_data = xi->data;
        scalarLB = xi->size[1];
        for (k = 0; k < scalarLB; k++) {
          if (xi_data[k] != (int32_T)muDoubleScalarFloor(xi_data[k])) {
            emlrtIntegerCheckR2012b(xi_data[k], &emlrtDCI, &b_st);
          }
          vectorUB = (int32_T)xi_data[k];
          if ((vectorUB < 1) || (vectorUB > sizes_idx_0)) {
            emlrtDynamicBoundsCheckR2012b(vectorUB, 1, sizes_idx_0,
                                          &tb_emlrtBCI, &b_st);
          }
        }
        vectorUB = b_d->size[0] * b_d->size[1];
        b_d->size[0] = 1;
        b_d->size[1] = xi->size[1];
        emxEnsureCapacity_real_T(&st, b_d, vectorUB, &gb_emlrtRTEI);
        u_1_data = b_d->data;
        for (k = 0; k < scalarLB; k++) {
          u_1_data[k] = d_data[ist + d->size[0] * ((int32_T)xi_data[k] - 1)];
        }
        if (ist + 1 > d->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, d->size[0], &wb_emlrtBCI,
                                        &st);
        }
        if (ist + 1 > d->size[1]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, d->size[1], &xb_emlrtBCI,
                                        &st);
        }
        b_st.site = &qb_emlrtRSI;
        d_data[ist + d->size[0] * ist] = -sum(&b_st, b_d);
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b(&st);
        }
      }
      if (L->size[0] - 1 < 1) {
        scalarLB = 0;
        input_sizes_idx_0 = 0;
      } else {
        if ((L->size[0] - 1 < 1) || (L->size[0] - 1 > d->size[0])) {
          emlrtDynamicBoundsCheckR2012b(L->size[0] - 1, 1, d->size[0],
                                        &g_emlrtBCI, &st);
        }
        scalarLB = L->size[0] - 1;
        if ((L->size[0] - 1 < 1) || (L->size[0] - 1 > d->size[1])) {
          emlrtDynamicBoundsCheckR2012b(L->size[0] - 1, 1, d->size[1],
                                        &h_emlrtBCI, &st);
        }
        input_sizes_idx_0 = L->size[0] - 1;
      }
      for (k = 0; k < input_sizes_idx_0; k++) {
        for (ist = 0; ist < scalarLB; ist++) {
          d_data[ist + scalarLB * k] = d_data[ist + d->size[0] * k];
        }
      }
      vectorUB = d->size[0] * d->size[1];
      d->size[0] = scalarLB;
      d->size[1] = input_sizes_idx_0;
      emxEnsureCapacity_real_T(&st, d, vectorUB, &v_emlrtRTEI);
      d_data = d->data;
      if (loop_ub > scalarLB) {
        emlrtDynamicBoundsCheckR2012b(loop_ub, 1, scalarLB, &nb_emlrtBCI, &st);
      }
      if (loop_ub > input_sizes_idx_0) {
        emlrtDynamicBoundsCheckR2012b(loop_ub, 1, input_sizes_idx_0,
                                      &ob_emlrtBCI, &st);
      }
      d_data[(loop_ub + d->size[0] * (loop_ub - 1)) - 1] = 1.0;
      scalarLB = b_scalarLB - 2;
      for (ist = 0; ist < R; ist++) {
        b_st.site = &rb_emlrtRSI;
        if (ist + 1 > csi->size[1]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, csi->size[1], &i_emlrtBCI,
                                        &b_st);
        }
        c_st.site = &tb_emlrtRSI;
        d_st.site = &ub_emlrtRSI;
        if (ist + 1 > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, N->size[1], &dc_emlrtBCI,
                                        &st);
        }
        if (ist + 1 > Z->size[1]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, Z->size[1], &ec_emlrtBCI,
                                        &st);
        }
        v_1 = csi_data[ist] * csi_data[ist] / N_data[ist] * Z_data[ist];
        vectorUB = y->size[0] * y->size[1];
        y->size[0] = 1;
        y->size[1] = b_loop_ub;
        emxEnsureCapacity_real_T(&st, y, vectorUB, &y_emlrtRTEI);
        y_data = y->data;
        for (k = 0; k <= scalarLB; k += 2) {
          r1 = _mm_loadu_pd(&umax_data[k]);
          _mm_storeu_pd(&y_data[k], _mm_mul_pd(_mm_set1_pd(v_1), r1));
        }
        for (k = b_scalarLB; k < b_loop_ub; k++) {
          y_data[k] = v_1 * umax_data[k];
        }
        b_st.site = &rb_emlrtRSI;
        if (ist + 1 > R) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, R, &j_emlrtBCI, &b_st);
        }
        c_st.site = &ib_emlrtRSI;
        if (loop_ub != y->size[1]) {
          if ((y->size[1] == 1) || (loop_ub == 1)) {
            emlrtErrorWithMessageIdR2018a(
                &c_st, &emlrtRTEI,
                "Coder:toolbox:mtimes_noDynamicScalarExpansion",
                "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
          } else {
            emlrtErrorWithMessageIdR2018a(
                &c_st, &b_emlrtRTEI, "MATLAB:innerdim", "MATLAB:innerdim", 0);
          }
        }
        vectorUB = b_L->size[0];
        b_L->size[0] = loop_ub;
        emxEnsureCapacity_real_T(&st, b_L, vectorUB, &jb_emlrtRTEI);
        y_data = b_L->data;
        for (k = 0; k < loop_ub; k++) {
          y_data[k] = L_data[k + L->size[0] * ist];
        }
        if (loop_ub > d->size[0]) {
          emlrtDynamicBoundsCheckR2012b(loop_ub, 1, d->size[0], &rc_emlrtBCI,
                                        &st);
        }
        if (loop_ub > d->size[1]) {
          emlrtDynamicBoundsCheckR2012b(loop_ub, 1, d->size[1], &sc_emlrtBCI,
                                        &st);
        }
        if (loop_ub > d->size[0]) {
          emlrtDynamicBoundsCheckR2012b(loop_ub, 1, d->size[0], &tc_emlrtBCI,
                                        &st);
        }
        if (loop_ub > d->size[1]) {
          emlrtDynamicBoundsCheckR2012b(loop_ub, 1, d->size[1], &uc_emlrtBCI,
                                        &st);
        }
        d_data[(loop_ub + d->size[0] * (loop_ub - 1)) - 1] -= mtimes(y, b_L);
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b(&st);
        }
      }
      if (loop_ub > d->size[0]) {
        emlrtDynamicBoundsCheckR2012b(loop_ub, 1, d->size[0], &pb_emlrtBCI,
                                      &st);
      }
      if (loop_ub > d->size[1]) {
        emlrtDynamicBoundsCheckR2012b(loop_ub, 1, d->size[1], &qb_emlrtBCI,
                                      &st);
      }
      if (loop_ub > d->size[0]) {
        emlrtDynamicBoundsCheckR2012b(loop_ub, 1, d->size[0], &rb_emlrtBCI,
                                      &st);
      }
      if (loop_ub > d->size[1]) {
        emlrtDynamicBoundsCheckR2012b(loop_ub, 1, d->size[1], &sb_emlrtBCI,
                                      &st);
      }
      d_data[(loop_ub + d->size[0] * (loop_ub - 1)) - 1] *= b_vmax;
      for (ist = 0; ist <= loop_ub - 2; ist++) {
        if (ist + 1 > d->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, d->size[0], &ub_emlrtBCI,
                                        &st);
        }
        if (loop_ub > d->size[1]) {
          emlrtDynamicBoundsCheckR2012b(loop_ub, 1, d->size[1], &vb_emlrtBCI,
                                        &st);
        }
        d_data[ist + d->size[0] * (loop_ub - 1)] = 0.0;
        for (b_r = 0; b_r < R; b_r++) {
          b_st.site = &sb_emlrtRSI;
          if (b_r + 1 > csi->size[1]) {
            emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, csi->size[1], &k_emlrtBCI,
                                          &b_st);
          }
          c_st.site = &tb_emlrtRSI;
          d_st.site = &ub_emlrtRSI;
          b_st.site = &sb_emlrtRSI;
          if (b_r + 1 > R) {
            emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, R, &l_emlrtBCI, &b_st);
          }
          c_st.site = &ib_emlrtRSI;
          if (b_loop_ub != loop_ub) {
            if ((b_loop_ub == 1) || (loop_ub == 1)) {
              emlrtErrorWithMessageIdR2018a(
                  &c_st, &emlrtRTEI,
                  "Coder:toolbox:mtimes_noDynamicScalarExpansion",
                  "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
            } else {
              emlrtErrorWithMessageIdR2018a(
                  &c_st, &b_emlrtRTEI, "MATLAB:innerdim", "MATLAB:innerdim", 0);
            }
          }
          vectorUB = b_d->size[0] * b_d->size[1];
          b_d->size[0] = 1;
          b_d->size[1] = b_loop_ub;
          emxEnsureCapacity_real_T(&st, b_d, vectorUB, &lb_emlrtRTEI);
          u_1_data = b_d->data;
          vectorUB = b_L->size[0];
          b_L->size[0] = loop_ub;
          emxEnsureCapacity_real_T(&st, b_L, vectorUB, &mb_emlrtRTEI);
          y_data = b_L->data;
          for (k = 0; k < b_loop_ub; k++) {
            u_1_data[k] = umax_data[k];
            y_data[k] = L_data[k + L->size[0] * b_r];
          }
          if (ist + 1 > d->size[0]) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, d->size[0], &wc_emlrtBCI,
                                          &st);
          }
          if (loop_ub > d->size[1]) {
            emlrtDynamicBoundsCheckR2012b(loop_ub, 1, d->size[1], &xc_emlrtBCI,
                                          &st);
          }
          if (ist + 1 > b_loop_ub) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, b_loop_ub, &yc_emlrtBCI,
                                          &st);
          }
          if (b_r + 1 > N->size[1]) {
            emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, N->size[1], &ad_emlrtBCI,
                                          &st);
          }
          if (ist + 1 > Lhat->size[0]) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, Lhat->size[0],
                                          &bd_emlrtBCI, &st);
          }
          if (b_r + 1 > Lhat->size[1]) {
            emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, Lhat->size[1],
                                          &cd_emlrtBCI, &st);
          }
          if (b_r + 1 > csi->size[1]) {
            emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, csi->size[1],
                                          &dd_emlrtBCI, &st);
          }
          if (ist + 1 > loop_ub) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, loop_ub, &ed_emlrtBCI,
                                          &st);
          }
          if (b_r + 1 > R) {
            emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, R, &fd_emlrtBCI, &st);
          }
          if (ist + 1 > d->size[0]) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, d->size[0], &gd_emlrtBCI,
                                          &st);
          }
          if (loop_ub > d->size[1]) {
            emlrtDynamicBoundsCheckR2012b(loop_ub, 1, d->size[1], &hd_emlrtBCI,
                                          &st);
          }
          d_data[ist + d->size[0] * (loop_ub - 1)] +=
              b_vmax * umax_data[ist] *
              (csi_data[b_r] * csi_data[b_r] / N_data[b_r] *
                   Lhat_data[ist + Lhat->size[0] * b_r] * mtimes(b_d, b_L) -
               csi_data[b_r] * L_data[ist + L->size[0] * b_r]);
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&st);
          }
        }
        if (ist + 1 > d->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, d->size[0], &yb_emlrtBCI,
                                        &st);
        }
        if (loop_ub > d->size[1]) {
          emlrtDynamicBoundsCheckR2012b(loop_ub, 1, d->size[1], &ac_emlrtBCI,
                                        &st);
        }
        if (loop_ub > d->size[0]) {
          emlrtDynamicBoundsCheckR2012b(loop_ub, 1, d->size[0], &bc_emlrtBCI,
                                        &st);
        }
        if (ist + 1 > d->size[1]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, d->size[1], &cc_emlrtBCI,
                                        &st);
        }
        d_data[(loop_ub + d->size[0] * ist) - 1] =
            d_data[ist + d->size[0] * (loop_ub - 1)];
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b(&st);
        }
      }
      eta = 0.0;
      scalarLB = b_scalarLB - 2;
      for (ist = 0; ist < R; ist++) {
        vectorUB = y->size[0] * y->size[1];
        y->size[0] = 1;
        y->size[1] = b_loop_ub;
        emxEnsureCapacity_real_T(sp, y, vectorUB, &x_emlrtRTEI);
        y_data = y->data;
        for (k = 0; k <= scalarLB; k += 2) {
          r1 = _mm_loadu_pd(&umax_data[k]);
          _mm_storeu_pd(&y_data[k], _mm_mul_pd(_mm_set1_pd(b_vmax), r1));
        }
        for (k = b_scalarLB; k < b_loop_ub; k++) {
          y_data[k] = b_vmax * umax_data[k];
        }
        st.site = &e_emlrtRSI;
        if (ist + 1 > R) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, R, &emlrtBCI, &st);
        }
        b_st.site = &ib_emlrtRSI;
        if (loop_ub != y->size[1]) {
          if ((y->size[1] == 1) || (loop_ub == 1)) {
            emlrtErrorWithMessageIdR2018a(
                &b_st, &emlrtRTEI,
                "Coder:toolbox:mtimes_noDynamicScalarExpansion",
                "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
          } else {
            emlrtErrorWithMessageIdR2018a(
                &b_st, &b_emlrtRTEI, "MATLAB:innerdim", "MATLAB:innerdim", 0);
          }
        }
        st.site = &e_emlrtRSI;
        vectorUB = b_L->size[0];
        b_L->size[0] = loop_ub;
        emxEnsureCapacity_real_T(&st, b_L, vectorUB, &kb_emlrtRTEI);
        y_data = b_L->data;
        for (k = 0; k < loop_ub; k++) {
          y_data[k] = L_data[k + L->size[0] * ist];
        }
        if (ist + 1 > Z->size[1]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, Z->size[1], &qc_emlrtBCI,
                                        &st);
        }
        v_1 = Z_data[ist] + mtimes(y, b_L);
        if (v_1 < 0.0) {
          emlrtErrorWithMessageIdR2018a(
              &st, &d_emlrtRTEI, "Coder:toolbox:ElFunDomainError",
              "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
        }
        v_1 = muDoubleScalarLog(v_1);
        if (ist + 1 > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, N->size[1], &vc_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        eta += N_data[ist] * v_1;
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b((emlrtConstCTX)sp);
        }
      }
      st.site = &f_emlrtRSI;
      /*  LF=FACTLN(N) */
      b_st.site = &s_emlrtRSI;
      vectorUB = xi->size[0] * xi->size[1];
      xi->size[0] = 1;
      scalarLB = N->size[1];
      xi->size[1] = N->size[1];
      emxEnsureCapacity_real_T(&b_st, xi, vectorUB, &p_emlrtRTEI);
      xi_data = xi->data;
      vectorUB = (N->size[1] / 2) << 1;
      input_sizes_idx_0 = vectorUB - 2;
      for (k = 0; k <= input_sizes_idx_0; k += 2) {
        _mm_storeu_pd(&xi_data[k],
                      _mm_add_pd(_mm_loadu_pd(&N_data[k]), _mm_set1_pd(1.0)));
      }
      for (k = vectorUB; k < scalarLB; k++) {
        xi_data[k] = N_data[k] + 1.0;
      }
      c_st.site = &t_emlrtRSI;
      applyScalarFunctionInPlace(&c_st, xi);
      st.site = &f_emlrtRSI;
      if (b_vmax < 0.0) {
        emlrtErrorWithMessageIdR2018a(
            &st, &d_emlrtRTEI, "Coder:toolbox:ElFunDomainError",
            "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
      }
      st.site = &f_emlrtRSI;
      st.site = &f_emlrtRSI;
      b_st.site = &f_emlrtRSI;
      v_1 = det(&b_st, d);
      if (v_1 < 0.0) {
        emlrtErrorWithMessageIdR2018a(
            &st, &e_emlrtRTEI, "Coder:toolbox:ElFunDomainError",
            "Coder:toolbox:ElFunDomainError", 3, 4, 4, "sqrt");
      }
      st.site = &f_emlrtRSI;
      st.site = &f_emlrtRSI;
      c_log(&st, umax);
      st.site = &f_emlrtRSI;
      *lGn = (((((-sum(&st, xi) - b_vmax) +
                 (real_T)L->size[0] * muDoubleScalarLog(b_vmax)) +
                (real_T)L->size[0] * 0.91893853320467267) -
               muDoubleScalarLog(muDoubleScalarSqrt(v_1))) +
              b_sum(&st, umax)) +
             eta;
      *Gn = muDoubleScalarExp(*lGn);
    }
  }
  if (guard1) {
    st.site = &b_emlrtRSI;
    c_sum(&st, Z, y);
    st.site = &b_emlrtRSI;
    b_log(&st, y);
    y_data = y->data;
    input_sizes_idx_0 = N->size[1];
    if ((N->size[1] != y->size[1]) &&
        ((N->size[1] != 1) && (y->size[1] != 1))) {
      emlrtDimSizeImpxCheckR2021b(N->size[1], y->size[1], &emlrtECI,
                                  (emlrtConstCTX)sp);
    }
    st.site = &b_emlrtRSI;
    /*  LF=FACTLN(N) */
    b_st.site = &s_emlrtRSI;
    vectorUB = xi->size[0] * xi->size[1];
    xi->size[0] = 1;
    xi->size[1] = N->size[1];
    emxEnsureCapacity_real_T(&b_st, xi, vectorUB, &p_emlrtRTEI);
    xi_data = xi->data;
    scalarLB = (N->size[1] / 2) << 1;
    vectorUB = scalarLB - 2;
    for (k = 0; k <= vectorUB; k += 2) {
      _mm_storeu_pd(&xi_data[k],
                    _mm_add_pd(_mm_loadu_pd(&N_data[k]), _mm_set1_pd(1.0)));
    }
    for (k = scalarLB; k < input_sizes_idx_0; k++) {
      xi_data[k] = N_data[k] + 1.0;
    }
    c_st.site = &t_emlrtRSI;
    applyScalarFunctionInPlace(&c_st, xi);
    if (N->size[1] == y->size[1]) {
      vectorUB = b_d->size[0] * b_d->size[1];
      b_d->size[0] = 1;
      b_d->size[1] = N->size[1];
      emxEnsureCapacity_real_T(sp, b_d, vectorUB, &s_emlrtRTEI);
      u_1_data = b_d->data;
      vectorUB = scalarLB - 2;
      for (k = 0; k <= vectorUB; k += 2) {
        r1 = _mm_loadu_pd(&y_data[k]);
        _mm_storeu_pd(&u_1_data[k], _mm_mul_pd(_mm_loadu_pd(&N_data[k]), r1));
      }
      for (k = scalarLB; k < input_sizes_idx_0; k++) {
        u_1_data[k] = N_data[k] * y_data[k];
      }
      st.site = &b_emlrtRSI;
      *lGn = -sum(&st, xi) + sum(&st, b_d);
    } else {
      st.site = &b_emlrtRSI;
      *lGn = binary_expand_op(&st, b_emlrtRSI, xi, N, y);
    }
    *Gn = muDoubleScalarExp(*lGn);
  }
  emxFree_real_T(sp, &c_d);
  emxFree_real_T(sp, &b_d);
  emxFree_real_T(sp, &b_L);
  emxFree_real_T(sp, &r);
  emxFree_int32_T(sp, &ia);
  emxFree_real_T(sp, &y);
  emxFree_real_T(sp, &Lhat);
  emxFree_real_T(sp, &csi);
  emxFree_real_T(sp, &d);
  emxFree_real_T(sp, &xi);
  emxFree_real_T(sp, &u_1);
  emxFree_real_T(sp, &umax);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (pfqn_le.c) */
