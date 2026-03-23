/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mva.c
 *
 * Code generation for function 'pfqn_mva'
 *
 */

/* Include files */
#include "pfqn_mva.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_mvamx_data.h"
#include "pfqn_mvamx_emxutil.h"
#include "pfqn_mvamx_mexutil.h"
#include "pfqn_mvamx_types.h"
#include "prod.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>

/* Variable Definitions */
static emlrtRSInfo
    y_emlrtRSI =
        {
            35,         /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    ab_emlrtRSI =
        {
            51,         /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    bb_emlrtRSI =
        {
            57,         /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    cb_emlrtRSI =
        {
            72,         /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    db_emlrtRSI =
        {
            80,         /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    eb_emlrtRSI =
        {
            87,         /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    fb_emlrtRSI =
        {
            123,        /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    gb_emlrtRSI =
        {
            124,        /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    hb_emlrtRSI =
        {
            125,        /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo ib_emlrtRSI = {
    16,                                                            /* lineNo */
    "ceil",                                                        /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/ceil.m" /* pathName
                                                                    */
};

static emlrtRSInfo jb_emlrtRSI = {
    44,                           /* lineNo */
    "applyScalarFunctionInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunctionInPlace.m" /* pathName */
};

static emlrtRSInfo kb_emlrtRSI = {
    13,                                                         /* lineNo */
    "any",                                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/any.m" /* pathName */
};

static emlrtRSInfo lb_emlrtRSI = {
    143,        /* lineNo */
    "allOrAny", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/allOrAny.m" /* pathName
                                                                            */
};

static emlrtRTEInfo b_emlrtRTEI = {
    14,                                                           /* lineNo */
    9,                                                            /* colNo */
    "log",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/log.m" /* pName */
};

static emlrtRTEInfo c_emlrtRTEI =
    {
        13,               /* lineNo */
        13,               /* colNo */
        "toLogicalCheck", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "toLogicalCheck.m" /* pName */
};

static emlrtRTEInfo
    e_emlrtRTEI =
        {
            154,                  /* lineNo */
            9,                    /* colNo */
            "last_nonzero_index", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtBCInfo ac_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        125,        /* lineNo */
        23,         /* colNo */
        "XN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo bc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        124,        /* lineNo */
        91,         /* colNo */
        "n",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo cc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        124,        /* lineNo */
        79,         /* colNo */
        "n",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo dc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        124,        /* lineNo */
        56,         /* colNo */
        "N",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo ec_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        124,        /* lineNo */
        54,         /* colNo */
        "N",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo fc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        124,        /* lineNo */
        32,         /* colNo */
        "n",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo gc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        124,        /* lineNo */
        30,         /* colNo */
        "n",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo hc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        104,        /* lineNo */
        21,         /* colNo */
        "L",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo ic_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        104,        /* lineNo */
        19,         /* colNo */
        "L",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtDCInfo emlrtDCI =
    {
        82,         /* lineNo */
        9,          /* colNo */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        1             /* checkKind */
};

static emlrtDCInfo b_emlrtDCI =
    {
        82,         /* lineNo */
        9,          /* colNo */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        4             /* checkKind */
};

static emlrtECInfo
    d_emlrtECI =
        {
            2,          /* nDims */
            72,         /* lineNo */
            23,         /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtBCInfo jc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        72,         /* lineNo */
        49,         /* colNo */
        "N",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo kc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        72,         /* lineNo */
        45,         /* colNo */
        "N",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtDCInfo c_emlrtDCI =
    {
        72,         /* lineNo */
        30,         /* colNo */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        4             /* checkKind */
};

static emlrtDCInfo d_emlrtDCI =
    {
        70,         /* lineNo */
        15,         /* colNo */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        4             /* checkKind */
};

static emlrtBCInfo lc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        76,         /* lineNo */
        10,         /* colNo */
        "N",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo mc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        72,         /* lineNo */
        13,         /* colNo */
        "prods",    /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo nc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        86,         /* lineNo */
        5,          /* colNo */
        "n",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo oc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        91,         /* lineNo */
        14,         /* colNo */
        "n",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo pc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        142,        /* lineNo */
        20,         /* colNo */
        "XN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo qc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        142,        /* lineNo */
        25,         /* colNo */
        "L",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo rc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        142,        /* lineNo */
        27,         /* colNo */
        "L",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo sc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        142,        /* lineNo */
        12,         /* colNo */
        "UN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo tc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        142,        /* lineNo */
        14,         /* colNo */
        "UN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo uc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        119,        /* lineNo */
        23,         /* colNo */
        "n",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo vc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        119,        /* lineNo */
        29,         /* colNo */
        "N",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo wc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        92,         /* lineNo */
        22,         /* colNo */
        "n",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo xc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        93,         /* lineNo */
        25,         /* colNo */
        "n",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo yc_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        109,        /* lineNo */
        17,         /* colNo */
        "n",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo ad_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        109,        /* lineNo */
        23,         /* colNo */
        "Z",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo bd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        109,        /* lineNo */
        12,         /* colNo */
        "XN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo cd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        105,        /* lineNo */
        29,         /* colNo */
        "mi",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo dd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        105,        /* lineNo */
        34,         /* colNo */
        "Q",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtDCInfo e_emlrtDCI =
    {
        105,        /* lineNo */
        34,         /* colNo */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        1             /* checkKind */
};

static emlrtBCInfo ed_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        105,        /* lineNo */
        45,         /* colNo */
        "Q",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo fd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        105,        /* lineNo */
        16,         /* colNo */
        "CN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo gd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        105,        /* lineNo */
        18,         /* colNo */
        "CN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo hd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        106,        /* lineNo */
        28,         /* colNo */
        "CN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo id_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        106,        /* lineNo */
        30,         /* colNo */
        "CN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo jd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        99,         /* lineNo */
        22,         /* colNo */
        "n",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo kd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        112,        /* lineNo */
        24,         /* colNo */
        "XN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo ld_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        112,        /* lineNo */
        30,         /* colNo */
        "CN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo md_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        112,        /* lineNo */
        32,         /* colNo */
        "CN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo nd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        112,        /* lineNo */
        16,         /* colNo */
        "QN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo od_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        112,        /* lineNo */
        18,         /* colNo */
        "QN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo pd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        96,         /* lineNo */
        41,         /* colNo */
        "n",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo qd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        96,         /* lineNo */
        50,         /* colNo */
        "prods",    /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo rd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        113,        /* lineNo */
        31,         /* colNo */
        "Q",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo sd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        113,        /* lineNo */
        42,         /* colNo */
        "Q",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo td_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        113,        /* lineNo */
        48,         /* colNo */
        "QN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo ud_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        113,        /* lineNo */
        50,         /* colNo */
        "QN",       /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo vd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        113,        /* lineNo */
        15,         /* colNo */
        "Q",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo wd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        113,        /* lineNo */
        26,         /* colNo */
        "Q",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo xd_emlrtBCI =
    {
        -1,                   /* iFirst */
        -1,                   /* iLast */
        155,                  /* lineNo */
        10,                   /* colNo */
        "v",                  /* aName */
        "last_nonzero_index", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo yd_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        131,        /* lineNo */
        12,         /* colNo */
        "n",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtBCInfo ae_emlrtBCI =
    {
        -1,         /* iFirst */
        -1,         /* iLast */
        134,        /* lineNo */
        11,         /* colNo */
        "n",        /* aName */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        0             /* checkKind */
};

static emlrtRTEInfo
    jb_emlrtRTEI =
        {
            37,         /* lineNo */
            3,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    kb_emlrtRTEI =
        {
            37,         /* lineNo */
            1,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    lb_emlrtRTEI =
        {
            42,         /* lineNo */
            5,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    mb_emlrtRTEI =
        {
            51,         /* lineNo */
            10,         /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    nb_emlrtRTEI =
        {
            60,         /* lineNo */
            1,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    ob_emlrtRTEI =
        {
            61,         /* lineNo */
            1,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    pb_emlrtRTEI =
        {
            62,         /* lineNo */
            4,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    qb_emlrtRTEI =
        {
            63,         /* lineNo */
            1,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    rb_emlrtRTEI =
        {
            65,         /* lineNo */
            7,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    sb_emlrtRTEI =
        {
            65,         /* lineNo */
            5,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    tb_emlrtRTEI =
        {
            70,         /* lineNo */
            1,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    ub_emlrtRTEI =
        {
            80,         /* lineNo */
            13,         /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    vb_emlrtRTEI =
        {
            72,         /* lineNo */
            23,         /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    wb_emlrtRTEI =
        {
            82,         /* lineNo */
            1,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    xb_emlrtRTEI =
        {
            85,         /* lineNo */
            1,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    yb_emlrtRTEI =
        {
            124,        /* lineNo */
            28,         /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    ac_emlrtRTEI =
        {
            124,        /* lineNo */
            52,         /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    bc_emlrtRTEI =
        {
            124,        /* lineNo */
            76,         /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

/* Function Declarations */
static void binary_expand_op_1(const emlrtStack *sp, emxArray_real_T *in1,
                               int32_T in2, const emlrtRSInfo in3, int32_T in4,
                               const emxArray_real_T *in5, int32_T in6,
                               int32_T in7);

/* Function Definitions */
static void binary_expand_op_1(const emlrtStack *sp, emxArray_real_T *in1,
                               int32_T in2, const emlrtRSInfo in3, int32_T in4,
                               const emxArray_real_T *in5, int32_T in6,
                               int32_T in7)
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack st;
  emxArray_real_T *r;
  const real_T *in5_data;
  real_T *in1_data;
  real_T *r1;
  int32_T binary_expand_op_1_numThreads;
  int32_T i;
  int32_T i1;
  int32_T loop_ub;
  int32_T stride_1_1;
  st.prev = sp;
  st.tls = sp->tls;
  in5_data = in5->data;
  in1_data = in1->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &r, 2, &vb_emlrtRTEI);
  stride_1_1 = r->size[0] * r->size[1];
  r->size[0] = 1;
  i = in7 - in6;
  if (i == 1) {
    loop_ub = (in4 - in2) - 1;
  } else {
    loop_ub = i;
  }
  r->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, r, stride_1_1, &vb_emlrtRTEI);
  r1 = r->data;
  stride_1_1 = (i != 1);
  if (loop_ub < 1600) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      r1[i1] = in5_data[in6 + i1 * stride_1_1] + 1.0;
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    binary_expand_op_1_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(binary_expand_op_1_numThreads)

    for (i1 = 0; i1 < loop_ub; i1++) {
      r1[i1] = in5_data[in6 + i1 * stride_1_1] + 1.0;
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  stride_1_1 = in1->size[1];
  if ((in2 + 1 < 1) || (in2 + 1 > stride_1_1)) {
    emlrtDynamicBoundsCheckR2012b(in2 + 1, 1, stride_1_1, &mc_emlrtBCI,
                                  (emlrtConstCTX)sp);
  }
  st.site = (emlrtRSInfo *)&in3;
  in1_data[in2] = prod(&st, r);
  emxFree_real_T(sp, &r);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

real_T pfqn_mva(const emlrtStack *sp, const emxArray_real_T *L,
                emxArray_real_T *N, emxArray_real_T *Z,
                const emxArray_real_T *mi, emxArray_real_T *XN,
                emxArray_real_T *QN, emxArray_real_T *UN, emxArray_real_T *CN)
{
  static const int32_T iv[2] = {1, 6};
  static const int32_T iv1[2] = {1, 8};
  static const int32_T iv2[2] = {1, 68};
  static const char_T varargin_3[68] = {
      'd', 'e', 'm', 'a', 'n', 'd', ' ', 'm', 'a', 't', 'r', 'i', 'x', ' ',
      'a', 'n', 'd', ' ', 'p', 'o', 'p', 'u', 'l', 'a', 't', 'i', 'o', 'n',
      ' ', 'v', 'e', 'c', 't', 'o', 'r', ' ', 'h', 'a', 'v', 'e', ' ', 'd',
      'i', 'f', 'f', 'e', 'r', 'e', 'n', 't', ' ', 'n', 'u', 'm', 'b', 'e',
      'r', ' ', 'o', 'f', ' ', 'c', 'l', 'a', 's', 's', 'e', 's'};
  static const char_T varargin_2[8] = {'p', 'f', 'q', 'n', '_', 'm', 'v', 'a'};
  static const char_T varargin_1[6] = {'%', 's', ':', ' ', '%', 's'};
  jmp_buf *volatile emlrtJBStack;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack st;
  emxArray_boolean_T *x;
  emxArray_real_T *b_N;
  emxArray_real_T *n;
  emxArray_real_T *prods;
  const mxArray *b_m;
  const mxArray *b_y;
  const mxArray *c_y;
  const mxArray *d_y;
  const real_T *L_data;
  const real_T *mi_data;
  real_T lGN;
  real_T *CN_data;
  real_T *N_data;
  real_T *QN_data;
  real_T *Q_data;
  real_T *UN_data;
  real_T *XN_data;
  real_T *Z_data;
  real_T *b_N_data;
  real_T *n_data;
  real_T *prods_data;
  int32_T M_original;
  int32_T R;
  int32_T i;
  int32_T ix;
  int32_T k;
  int32_T loop_ub;
  int32_T m;
  int32_T nx;
  int32_T pfqn_mva_numThreads;
  int32_T w;
  boolean_T exitg1;
  boolean_T y;
  boolean_T *x_data;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  mi_data = mi->data;
  Z_data = Z->data;
  N_data = N->data;
  L_data = L->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /* { */
  /*  % @file pfqn_mva.m */
  /*  % @brief Exact Mean Value Analysis (MVA) for product-form queueing
   * networks. */
  /* } */
  /* { */
  /*  % @brief Exact Mean Value Analysis (MVA) for product-form queueing
   * networks. */
  /*  % @fn pfqn_mva(L, N, Z, mi) */
  /*  % @param L Service demand matrix (M x R). */
  /*  % @param N Population vector (1 x R). */
  /*  % @param Z Think time vector (1 x R). */
  /*  % @param mi (Optional) Server multiplicity vector (1 x M). Default: single
   * servers. */
  /*  % @return XN System throughput (1 x R). */
  /*  % @return QN Mean queue length (M x R). */
  /*  % @return UN Utilization (M x R). */
  /*  % @return CN Residence time (M x R). */
  /*  % @return lGN Logarithm of the normalizing constant. */
  /* } */
  /*  [XN,QN,UN,CN,LGN] = PFQN_MVA(L,N,Z,MI) */
  /*  [XN,QN,UN,CN] = pfqn_mva(L,N,Z,mi) */
  XN->size[0] = 0;
  XN->size[1] = 0;
  QN->size[0] = 0;
  QN->size[1] = 0;
  UN->size[0] = 0;
  UN->size[1] = 0;
  CN->size[0] = 0;
  CN->size[1] = 0;
  lGN = 0.0;
  st.site = &y_emlrtRSI;
  b_st.site = &ib_emlrtRSI;
  nx = N->size[1];
  c_st.site = &jb_emlrtRSI;
  if (N->size[1] > 2147483646) {
    d_st.site = &s_emlrtRSI;
    check_forloop_overflow_error(&d_st);
  }
  loop_ub = (N->size[1] < 1600);
  if (loop_ub) {
    for (k = 0; k < nx; k++) {
      N_data[k] = muDoubleScalarCeil(N_data[k]);
    }
  } else {
    emlrtEnterParallelRegion(&b_st, omp_in_parallel());
    emlrtPushJmpBuf(&b_st, &emlrtJBStack);
    pfqn_mva_numThreads =
        emlrtAllocRegionTLSs(b_st.tls, omp_in_parallel(), omp_get_max_threads(),
                             omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mva_numThreads)

    for (k = 0; k < nx; k++) {
      N_data[k] = muDoubleScalarCeil(N_data[k]);
    }
    emlrtPopJmpBuf(&b_st, &emlrtJBStack);
    emlrtExitParallelRegion(&b_st, omp_in_parallel());
  }
  R = L->size[1];
  M_original = L->size[0];
  /*  M stations, R classes */
  emxInit_real_T(sp, &b_N, 2, &jb_emlrtRTEI);
  ix = b_N->size[0] * b_N->size[1];
  b_N->size[0] = 1;
  b_N->size[1] = nx;
  emxEnsureCapacity_real_T(sp, b_N, ix, &jb_emlrtRTEI);
  b_N_data = b_N->data;
  for (m = 0; m < nx; m++) {
    b_N_data[m] = N_data[m];
  }
  ix = N->size[0] * N->size[1];
  N->size[0] = 1;
  N->size[1] = nx;
  emxEnsureCapacity_real_T(sp, N, ix, &kb_emlrtRTEI);
  N_data = N->data;
  for (m = 0; m < nx; m++) {
    N_data[m] = b_N_data[m];
  }
  if (Z->size[1] == 0) {
    ix = Z->size[0] * Z->size[1];
    Z->size[0] = 1;
    Z->size[1] = L->size[1];
    emxEnsureCapacity_real_T(sp, Z, ix, &lb_emlrtRTEI);
    Z_data = Z->data;
    for (m = 0; m < R; m++) {
      Z_data[m] = 0.0;
    }
  }
  /*  Station consolidation disabled: pfqn_unique merges stations with identical
   */
  /*  demand rows, but this is incorrect for tandem (serial) networks where
   * distinct */
  /*  stations happen to have the same service demand. The consolidation treats
   * them */
  /*  as replicated (parallel) copies, producing wrong queue lengths and
   * response times. */
  st.site = &ab_emlrtRSI;
  emxInit_boolean_T(&st, &x, &mb_emlrtRTEI);
  ix = x->size[0] * x->size[1];
  x->size[0] = 1;
  x->size[1] = nx;
  emxEnsureCapacity_boolean_T(&st, x, ix, &mb_emlrtRTEI);
  x_data = x->data;
  if (loop_ub) {
    for (i = 0; i < nx; i++) {
      x_data[i] = (N_data[i] > 0.0);
    }
  } else {
    emlrtEnterParallelRegion(&st, omp_in_parallel());
    emlrtPushJmpBuf(&st, &emlrtJBStack);
    pfqn_mva_numThreads = emlrtAllocRegionTLSs(
        st.tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mva_numThreads)

    for (i = 0; i < nx; i++) {
      x_data[i] = (N_data[i] > 0.0);
    }
    emlrtPopJmpBuf(&st, &emlrtJBStack);
    emlrtExitParallelRegion(&st, omp_in_parallel());
  }
  b_st.site = &kb_emlrtRSI;
  y = false;
  c_st.site = &lb_emlrtRSI;
  ix = 1;
  exitg1 = false;
  while ((!exitg1) && (ix <= x->size[1])) {
    if (x_data[ix - 1]) {
      y = true;
      exitg1 = true;
    } else {
      ix++;
    }
  }
  emxFree_boolean_T(&b_st, &x);
  if (y) {
    __m128d r;
    real_T currentpop;
    real_T totpop;
    int32_T b_Z;
    int32_T b_loop_ub;
    int32_T exitg2;
    int32_T firstnonempty;
    int32_T last_nnz;
    if (L->size[1] != N->size[1]) {
      st.site = &bb_emlrtRSI;
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
      b_st.site = &f_emlrtRSI;
      b_y = NULL;
      b_m = emlrtCreateCharArray(2, &iv[0]);
      emlrtInitCharArrayR2013a(&b_st, 6, b_m, &varargin_1[0]);
      emlrtAssign(&b_y, b_m);
      c_y = NULL;
      b_m = emlrtCreateCharArray(2, &iv1[0]);
      emlrtInitCharArrayR2013a(&b_st, 8, b_m, &varargin_2[0]);
      emlrtAssign(&c_y, b_m);
      d_y = NULL;
      b_m = emlrtCreateCharArray(2, &iv2[0]);
      emlrtInitCharArrayR2013a(&b_st, 68, b_m, &varargin_3[0]);
      emlrtAssign(&d_y, b_m);
      c_st.site = &yb_emlrtRSI;
      b_error(&c_st, b_y, c_y, d_y, &emlrtMCI);
    }
    ix = XN->size[0] * XN->size[1];
    XN->size[0] = 1;
    XN->size[1] = R;
    emxEnsureCapacity_real_T(sp, XN, ix, &nb_emlrtRTEI);
    XN_data = XN->data;
    for (m = 0; m < R; m++) {
      XN_data[m] = 0.0;
    }
    ix = QN->size[0] * QN->size[1];
    QN->size[0] = M_original;
    QN->size[1] = R;
    emxEnsureCapacity_real_T(sp, QN, ix, &ob_emlrtRTEI);
    QN_data = QN->data;
    loop_ub = L->size[0] * L->size[1];
    for (m = 0; m < loop_ub; m++) {
      QN_data[m] = 0.0;
    }
    ix = UN->size[0] * UN->size[1];
    UN->size[0] = M_original;
    UN->size[1] = R;
    emxEnsureCapacity_real_T(sp, UN, ix, &pb_emlrtRTEI);
    UN_data = UN->data;
    ix = CN->size[0] * CN->size[1];
    CN->size[0] = M_original;
    CN->size[1] = R;
    emxEnsureCapacity_real_T(sp, CN, ix, &qb_emlrtRTEI);
    CN_data = CN->data;
    for (m = 0; m < loop_ub; m++) {
      CN_data[m] = 0.0;
    }
    b_Z = Z->size[1];
    ix = b_N->size[0] * b_N->size[1];
    b_N->size[0] = 1;
    b_N->size[1] = Z->size[1];
    emxEnsureCapacity_real_T(sp, b_N, ix, &rb_emlrtRTEI);
    b_N_data = b_N->data;
    for (m = 0; m < b_Z; m++) {
      b_N_data[m] = Z_data[m];
    }
    ix = Z->size[0] * Z->size[1];
    Z->size[0] = 1;
    Z->size[1] = b_Z;
    emxEnsureCapacity_real_T(sp, Z, ix, &sb_emlrtRTEI);
    Z_data = Z->data;
    for (m = 0; m < b_Z; m++) {
      Z_data[m] = b_N_data[m];
    }
    if (L->size[1] - 1 < 0) {
      emlrtNonNegativeCheckR2012b(-1.0, &d_emlrtDCI, (emlrtConstCTX)sp);
    }
    loop_ub = L->size[1] - 1;
    emxInit_real_T(sp, &prods, 2, &tb_emlrtRTEI);
    ix = prods->size[0] * prods->size[1];
    prods->size[0] = 1;
    prods->size[1] = L->size[1] - 1;
    emxEnsureCapacity_real_T(sp, prods, ix, &tb_emlrtRTEI);
    prods_data = prods->data;
    for (m = 0; m < loop_ub; m++) {
      prods_data[m] = 0.0;
    }
    /*  generate population indices */
    for (w = 0; w <= R - 2; w++) {
      if (w + 2 > R) {
        last_nnz = 0;
        ix = 0;
      } else {
        if (w + 2 > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(w + 2, 1, N->size[1], &kc_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        last_nnz = w + 1;
        if (R > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(R, 1, N->size[1], &jc_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        ix = R;
      }
      nx = R - w;
      if (!((real_T)(nx - 2) + 1.0 >= 0.0)) {
        emlrtNonNegativeCheckR2012b((real_T)(nx - 2) + 1.0, &c_emlrtDCI,
                                    (emlrtConstCTX)sp);
      }
      firstnonempty = nx - 1;
      loop_ub = ix - last_nnz;
      if ((nx - 1 != loop_ub) && ((nx - 1 != 1) && (loop_ub != 1))) {
        emlrtDimSizeImpxCheckR2021b(nx - 1, loop_ub, &d_emlrtECI,
                                    (emlrtConstCTX)sp);
      }
      if (nx - 1 == loop_ub) {
        ix = b_N->size[0] * b_N->size[1];
        b_N->size[0] = 1;
        b_N->size[1] = nx - 1;
        emxEnsureCapacity_real_T(sp, b_N, ix, &vb_emlrtRTEI);
        b_N_data = b_N->data;
        ix = ((nx - 1) / 2) << 1;
        loop_ub = ix - 2;
        for (m = 0; m <= loop_ub; m += 2) {
          r = _mm_loadu_pd(&N_data[last_nnz + m]);
          _mm_storeu_pd(&b_N_data[m], _mm_add_pd(_mm_set1_pd(1.0), r));
        }
        for (m = ix; m < firstnonempty; m++) {
          b_N_data[m] = N_data[last_nnz + m] + 1.0;
        }
        if (w + 1 > prods->size[1]) {
          emlrtDynamicBoundsCheckR2012b(w + 1, 1, prods->size[1], &mc_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        st.site = &cb_emlrtRSI;
        prods_data[w] = prod(&st, b_N);
      } else {
        st.site = &cb_emlrtRSI;
        binary_expand_op_1(&st, prods, w, cb_emlrtRSI, R, N, last_nnz, ix);
        prods_data = prods->data;
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    firstnonempty = R;
    do {
      exitg2 = 0;
      b_loop_ub = N->size[1];
      if ((firstnonempty < 1) || (firstnonempty > N->size[1])) {
        emlrtDynamicBoundsCheckR2012b(firstnonempty, 1, N->size[1],
                                      &lc_emlrtBCI, (emlrtConstCTX)sp);
      }
      if (N_data[firstnonempty - 1] == 0.0) {
        firstnonempty--;
      } else {
        exitg2 = 1;
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    } while (exitg2 == 0);
    ix = b_N->size[0] * b_N->size[1];
    b_N->size[0] = 1;
    b_N->size[1] = N->size[1];
    emxEnsureCapacity_real_T(sp, b_N, ix, &ub_emlrtRTEI);
    b_N_data = b_N->data;
    ix = (N->size[1] / 2) << 1;
    loop_ub = ix - 2;
    for (m = 0; m <= loop_ub; m += 2) {
      r = _mm_loadu_pd(&N_data[m]);
      _mm_storeu_pd(&b_N_data[m], _mm_add_pd(r, _mm_set1_pd(1.0)));
    }
    for (m = ix; m < b_loop_ub; m++) {
      b_N_data[m] = N_data[m] + 1.0;
    }
    st.site = &db_emlrtRSI;
    totpop = prod(&st, b_N);
    if (!(totpop >= 0.0)) {
      emlrtNonNegativeCheckR2012b(totpop, &b_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (totpop != (int32_T)muDoubleScalarFloor(totpop)) {
      emlrtIntegerCheckR2012b(totpop, &emlrtDCI, (emlrtConstCTX)sp);
    }
    emxInit_real_T(sp, &Z, 2, &wb_emlrtRTEI);
    ix = Z->size[0] * Z->size[1];
    Z->size[0] = (int32_T)totpop;
    Z->size[1] = M_original;
    emxEnsureCapacity_real_T(sp, Z, ix, &wb_emlrtRTEI);
    Q_data = Z->data;
    ix = (int32_T)totpop * L->size[0];
    for (m = 0; m < ix; m++) {
      Q_data[m] = 0.0;
    }
    currentpop = 2.0;
    emxInit_real_T(sp, &N, 2, &xb_emlrtRTEI);
    ix = N->size[0] * N->size[1];
    N->size[0] = 1;
    N->size[1] = R;
    emxEnsureCapacity_real_T(sp, N, ix, &xb_emlrtRTEI);
    n_data = N->data;
    for (m = 0; m < R; m++) {
      n_data[m] = 0.0;
    }
    if (firstnonempty > R) {
      emlrtDynamicBoundsCheckR2012b(firstnonempty, 1, R, &nc_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    n_data[firstnonempty - 1] = 1.0;
    emxInit_real_T(sp, &n, 2, &yb_emlrtRTEI);
    do {
      exitg2 = 0;
      st.site = &eb_emlrtRSI;
      if (muDoubleScalarIsNaN(totpop)) {
        emlrtErrorWithMessageIdR2018a(&st, &c_emlrtRTEI, "MATLAB:nologicalnan",
                                      "MATLAB:nologicalnan", 0);
      }
      if (totpop != 0.0) {
        uint32_T s;
        /*  for each population */
        s = 1U;
        while (s <= (uint32_T)R) {
          real_T CNtot;
          real_T pos_n_1s;
          uint32_T b_i;
          pos_n_1s = 0.0;
          if ((int32_T)s > N->size[1]) {
            emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, N->size[1],
                                          &oc_emlrtBCI, (emlrtConstCTX)sp);
          }
          CNtot = n_data[(int32_T)s - 1];
          if (CNtot > 0.0) {
            if ((int32_T)s > N->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, N->size[1],
                                            &wc_emlrtBCI, (emlrtConstCTX)sp);
            }
            n_data[(int32_T)s - 1] = CNtot - 1.0;
            ix = N->size[1];
            if (R > N->size[1]) {
              emlrtDynamicBoundsCheckR2012b(R, 1, N->size[1], &xc_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            pos_n_1s = n_data[R - 1];
            loop_ub = 1;
            while (loop_ub <= R - 1) {
              if (loop_ub > ix) {
                emlrtDynamicBoundsCheckR2012b(loop_ub, 1, ix, &pd_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (loop_ub > prods->size[1]) {
                emlrtDynamicBoundsCheckR2012b(loop_ub, 1, prods->size[1],
                                              &qd_emlrtBCI, (emlrtConstCTX)sp);
              }
              pos_n_1s += n_data[loop_ub - 1] * prods_data[loop_ub - 1];
              loop_ub++;
              if (*emlrtBreakCheckR2012bFlagVar != 0) {
                emlrtBreakCheckR2012b((emlrtConstCTX)sp);
              }
            }
            /*  while w <= R-1 */
            if ((int32_T)s > N->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, N->size[1],
                                            &jd_emlrtBCI, (emlrtConstCTX)sp);
            }
            n_data[(int32_T)s - 1]++;
          }
          /*  if */
          CNtot = 0.0;
          b_i = 1U;
          while (b_i <= (uint32_T)M_original) {
            if ((int32_T)b_i > M_original) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, M_original,
                                            &ic_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)s > R) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, R, &hc_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            ix = mi->size[0] * mi->size[1];
            if ((int32_T)b_i > ix) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, ix, &cd_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (pos_n_1s + 1.0 !=
                (int32_T)muDoubleScalarFloor(pos_n_1s + 1.0)) {
              emlrtIntegerCheckR2012b(pos_n_1s + 1.0, &e_emlrtDCI,
                                      (emlrtConstCTX)sp);
            }
            if (((int32_T)(pos_n_1s + 1.0) < 1) ||
                ((int32_T)(pos_n_1s + 1.0) > Z->size[0])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)(pos_n_1s + 1.0), 1,
                                            Z->size[0], &dd_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if ((int32_T)b_i > Z->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, Z->size[1],
                                            &ed_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)b_i > CN->size[0]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, CN->size[0],
                                            &fd_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)s > CN->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, CN->size[1],
                                            &gd_emlrtBCI, (emlrtConstCTX)sp);
            }
            CN_data[((int32_T)b_i + CN->size[0] * ((int32_T)s - 1)) - 1] =
                L_data[((int32_T)b_i + L->size[0] * ((int32_T)s - 1)) - 1] *
                (mi_data[(int32_T)b_i - 1] +
                 Q_data[((int32_T)(pos_n_1s + 1.0) +
                         Z->size[0] * ((int32_T)b_i - 1)) -
                        1]);
            if ((int32_T)b_i > CN->size[0]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, CN->size[0],
                                            &hd_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)s > CN->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, CN->size[1],
                                            &id_emlrtBCI, (emlrtConstCTX)sp);
            }
            CNtot +=
                CN_data[((int32_T)b_i + CN->size[0] * ((int32_T)s - 1)) - 1];
            b_i++;
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b((emlrtConstCTX)sp);
            }
          }
          /*  while i <= M */
          if ((int32_T)s > N->size[1]) {
            emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, N->size[1],
                                          &yc_emlrtBCI, (emlrtConstCTX)sp);
          }
          if ((int32_T)s > b_Z) {
            emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, b_Z, &ad_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if ((int32_T)s > XN->size[1]) {
            emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, XN->size[1],
                                          &bd_emlrtBCI, (emlrtConstCTX)sp);
          }
          XN_data[(int32_T)s - 1] =
              n_data[(int32_T)s - 1] / (Z_data[(int32_T)s - 1] + CNtot);
          b_i = 1U;
          while (b_i <= (uint32_T)M_original) {
            if ((int32_T)s > XN->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, XN->size[1],
                                            &kd_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)b_i > CN->size[0]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, CN->size[0],
                                            &ld_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)s > CN->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, CN->size[1],
                                            &md_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)b_i > QN->size[0]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, QN->size[0],
                                            &nd_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)s > QN->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, QN->size[1],
                                            &od_emlrtBCI, (emlrtConstCTX)sp);
            }
            QN_data[((int32_T)b_i + QN->size[0] * ((int32_T)s - 1)) - 1] =
                XN_data[(int32_T)s - 1] *
                CN_data[((int32_T)b_i + CN->size[0] * ((int32_T)s - 1)) - 1];
            if (((int32_T)currentpop < 1) ||
                ((int32_T)currentpop > Z->size[0])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)currentpop, 1, Z->size[0],
                                            &rd_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)b_i > Z->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, Z->size[1],
                                            &sd_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)b_i > QN->size[0]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, QN->size[0],
                                            &td_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)s > QN->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, QN->size[1],
                                            &ud_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (((int32_T)currentpop < 1) ||
                ((int32_T)currentpop > Z->size[0])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)currentpop, 1, Z->size[0],
                                            &vd_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)b_i > Z->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, Z->size[1],
                                            &wd_emlrtBCI, (emlrtConstCTX)sp);
            }
            Q_data[((int32_T)currentpop + Z->size[0] * ((int32_T)b_i - 1)) -
                   1] +=
                QN_data[((int32_T)b_i + QN->size[0] * ((int32_T)s - 1)) - 1];
            b_i++;
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b((emlrtConstCTX)sp);
            }
          }
          /*  while i <= M */
          s++;
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b((emlrtConstCTX)sp);
          }
        }
        /*  while s <= R */
        s = (uint32_T)R;
        int32_T exitg3;
        boolean_T guard1;
        do {
          exitg3 = 0;
          guard1 = false;
          if ((int32_T)s > 0) {
            if (((int32_T)s < 1) || ((int32_T)s > N->size[1])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, N->size[1],
                                            &uc_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (((int32_T)s < 1) || ((int32_T)s > b_loop_ub)) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, b_loop_ub,
                                            &vc_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (n_data[(int32_T)s - 1] == N_data[(int32_T)s - 1]) {
              s = (uint32_T)((int32_T)s - 1);
            } else {
              guard1 = true;
            }
          } else {
            guard1 = true;
          }
          if (guard1) {
            if ((int32_T)s > firstnonempty) {
              s = (uint32_T)((int32_T)s - 1);
            } else {
              exitg3 = 1;
            }
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b((emlrtConstCTX)sp);
          }
        } while (exitg3 == 0);
        /*  now compute the normalizing constant */
        st.site = &fb_emlrtRSI;
        last_nnz = -1;
        ix = N->size[1];
        emlrtForLoopVectorCheckR2021a(N->size[1], -1.0, 1.0, mxDOUBLE_CLASS,
                                      N->size[1], &e_emlrtRTEI, &st);
        loop_ub = 0;
        exitg1 = false;
        while ((!exitg1) && (loop_ub <= ix - 1)) {
          nx = ix - loop_ub;
          if ((nx < 1) || (nx > ix)) {
            emlrtDynamicBoundsCheckR2012b(nx, 1, ix, &xd_emlrtBCI, &st);
          }
          if (n_data[nx - 1] != 0.0) {
            last_nnz = nx - 1;
            exitg1 = true;
          } else {
            loop_ub++;
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&st);
          }
        }
        if (last_nnz + 1 > 0) {
          if (last_nnz < 1) {
            loop_ub = 0;
            nx = 0;
          } else {
            if (N->size[1] < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, N->size[1], &gc_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (last_nnz > N->size[1]) {
              emlrtDynamicBoundsCheckR2012b(last_nnz, 1, N->size[1],
                                            &fc_emlrtBCI, (emlrtConstCTX)sp);
            }
            loop_ub = last_nnz;
            if (b_loop_ub < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, b_loop_ub, &ec_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (last_nnz > b_loop_ub) {
              emlrtDynamicBoundsCheckR2012b(last_nnz, 1, b_loop_ub,
                                            &dc_emlrtBCI, (emlrtConstCTX)sp);
            }
            nx = last_nnz;
          }
          ix = n->size[0] * n->size[1];
          n->size[0] = 1;
          n->size[1] = loop_ub;
          emxEnsureCapacity_real_T(sp, n, ix, &yb_emlrtRTEI);
          b_N_data = n->data;
          for (m = 0; m < loop_ub; m++) {
            b_N_data[m] = n_data[m];
          }
          ix = b_N->size[0] * b_N->size[1];
          b_N->size[0] = 1;
          b_N->size[1] = nx;
          emxEnsureCapacity_real_T(sp, b_N, ix, &ac_emlrtRTEI);
          b_N_data = b_N->data;
          for (m = 0; m < nx; m++) {
            b_N_data[m] = N_data[m];
          }
          st.site = &gb_emlrtRSI;
          if (sum(&st, n) == sum(&st, b_N)) {
            if ((uint32_T)last_nnz + 2U > (uint32_T)R) {
              nx = 0;
              ix = 0;
            } else {
              if (((int32_T)((uint32_T)last_nnz + 2U) < 1) ||
                  ((int32_T)((uint32_T)last_nnz + 2U) > N->size[1])) {
                emlrtDynamicBoundsCheckR2012b(
                    (int32_T)((uint32_T)last_nnz + 2U), 1, N->size[1],
                    &cc_emlrtBCI, (emlrtConstCTX)sp);
              }
              nx = last_nnz + 1;
              if (R > N->size[1]) {
                emlrtDynamicBoundsCheckR2012b(R, 1, N->size[1], &bc_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              ix = R;
            }
            loop_ub = n->size[0] * n->size[1];
            n->size[0] = 1;
            ix -= nx;
            n->size[1] = ix;
            emxEnsureCapacity_real_T(sp, n, loop_ub, &bc_emlrtRTEI);
            b_N_data = n->data;
            for (m = 0; m < ix; m++) {
              b_N_data[m] = n_data[nx + m];
            }
            st.site = &gb_emlrtRSI;
            if (sum(&st, n) == 0.0) {
              st.site = &hb_emlrtRSI;
              if ((last_nnz < 0) || (last_nnz + 1 > XN->size[1])) {
                emlrtDynamicBoundsCheckR2012b(last_nnz + 1, 1, XN->size[1],
                                              &ac_emlrtBCI, &st);
              }
              if (XN_data[last_nnz] < 0.0) {
                emlrtErrorWithMessageIdR2018a(
                    &st, &b_emlrtRTEI, "Coder:toolbox:ElFunDomainError",
                    "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
              }
              lGN -= muDoubleScalarLog(XN_data[last_nnz]);
            }
          }
        }
        if ((int32_T)s == 0) {
          exitg2 = 1;
        } else {
          if (((int32_T)s < 1) || ((int32_T)s > N->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, N->size[1],
                                          &yd_emlrtBCI, (emlrtConstCTX)sp);
          }
          n_data[(int32_T)s - 1]++;
          s++;
          while (s <= (uint32_T)R) {
            if (((int32_T)s < 1) || ((int32_T)s > N->size[1])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, N->size[1],
                                            &ae_emlrtBCI, (emlrtConstCTX)sp);
            }
            n_data[(int32_T)s - 1] = 0.0;
            s++;
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b((emlrtConstCTX)sp);
            }
          }
          totpop--;
          currentpop++;
        }
      } else {
        exitg2 = 1;
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    } while (exitg2 == 0);
    emxFree_real_T(sp, &n);
    emxFree_real_T(sp, &N);
    emxFree_real_T(sp, &Z);
    emxFree_real_T(sp, &prods);
    for (m = 0; m < M_original; m++) {
      for (w = 0; w < R; w++) {
        if (w + 1 > XN->size[1]) {
          emlrtDynamicBoundsCheckR2012b(w + 1, 1, XN->size[1], &pc_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (m + 1 > M_original) {
          emlrtDynamicBoundsCheckR2012b(m + 1, 1, M_original, &qc_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (w + 1 > R) {
          emlrtDynamicBoundsCheckR2012b(w + 1, 1, R, &rc_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (m + 1 > UN->size[0]) {
          emlrtDynamicBoundsCheckR2012b(m + 1, 1, UN->size[0], &sc_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (w + 1 > UN->size[1]) {
          emlrtDynamicBoundsCheckR2012b(w + 1, 1, UN->size[1], &tc_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        UN_data[m + UN->size[0] * w] = XN_data[w] * L_data[m + L->size[0] * w];
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b((emlrtConstCTX)sp);
        }
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    /*  Expand results back to original dimensions if stations were consolidated
     */
  } else {
    /* line_warning(mfilename,'closed populations are empty'); */
  }
  emxFree_real_T(sp, &b_N);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return lGN;
}

/* End of code generation (pfqn_mva.c) */
