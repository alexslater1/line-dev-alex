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
#include "any.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_mvams_data.h"
#include "pfqn_mvams_emxutil.h"
#include "pfqn_mvams_mexutil.h"
#include "pfqn_mvams_types.h"
#include "prod.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>

/* Variable Definitions */
static emlrtRSInfo
    mb_emlrtRSI =
        {
            35,         /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    nb_emlrtRSI =
        {
            51,         /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    ob_emlrtRSI =
        {
            57,         /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    pb_emlrtRSI =
        {
            72,         /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    qb_emlrtRSI =
        {
            80,         /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    rb_emlrtRSI =
        {
            87,         /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    sb_emlrtRSI =
        {
            123,        /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    tb_emlrtRSI =
        {
            124,        /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo
    ub_emlrtRSI =
        {
            125,        /* lineNo */
            "pfqn_mva", /* fcnName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pathName */
};

static emlrtRSInfo vb_emlrtRSI = {
    16,                                                            /* lineNo */
    "ceil",                                                        /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/ceil.m" /* pathName
                                                                    */
};

static emlrtRSInfo wb_emlrtRSI = {
    44,                           /* lineNo */
    "applyScalarFunctionInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunctionInPlace.m" /* pathName */
};

static emlrtRTEInfo g_emlrtRTEI =
    {
        13,               /* lineNo */
        13,               /* colNo */
        "toLogicalCheck", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "toLogicalCheck.m" /* pName */
};

static emlrtBCInfo hd_emlrtBCI =
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

static emlrtBCInfo id_emlrtBCI =
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

static emlrtBCInfo jd_emlrtBCI =
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

static emlrtBCInfo kd_emlrtBCI =
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

static emlrtBCInfo ld_emlrtBCI =
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

static emlrtBCInfo md_emlrtBCI =
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

static emlrtBCInfo nd_emlrtBCI =
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

static emlrtBCInfo od_emlrtBCI =
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

static emlrtBCInfo pd_emlrtBCI =
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

static emlrtDCInfo f_emlrtDCI =
    {
        82,         /* lineNo */
        9,          /* colNo */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        1             /* checkKind */
};

static emlrtDCInfo g_emlrtDCI =
    {
        82,         /* lineNo */
        9,          /* colNo */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        4             /* checkKind */
};

static emlrtECInfo
    f_emlrtECI =
        {
            2,          /* nDims */
            72,         /* lineNo */
            23,         /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtBCInfo qd_emlrtBCI =
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

static emlrtBCInfo rd_emlrtBCI =
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

static emlrtDCInfo h_emlrtDCI =
    {
        72,         /* lineNo */
        30,         /* colNo */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        4             /* checkKind */
};

static emlrtDCInfo i_emlrtDCI =
    {
        70,         /* lineNo */
        15,         /* colNo */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        4             /* checkKind */
};

static emlrtBCInfo sd_emlrtBCI =
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

static emlrtBCInfo td_emlrtBCI =
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

static emlrtBCInfo ud_emlrtBCI =
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

static emlrtBCInfo vd_emlrtBCI =
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

static emlrtBCInfo wd_emlrtBCI =
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

static emlrtBCInfo xd_emlrtBCI =
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

static emlrtBCInfo yd_emlrtBCI =
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

static emlrtBCInfo ae_emlrtBCI =
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

static emlrtBCInfo be_emlrtBCI =
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

static emlrtBCInfo ce_emlrtBCI =
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

static emlrtBCInfo de_emlrtBCI =
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

static emlrtBCInfo ee_emlrtBCI =
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

static emlrtBCInfo fe_emlrtBCI =
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

static emlrtBCInfo ge_emlrtBCI =
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

static emlrtBCInfo he_emlrtBCI =
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

static emlrtBCInfo ie_emlrtBCI =
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

static emlrtBCInfo je_emlrtBCI =
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

static emlrtBCInfo ke_emlrtBCI =
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

static emlrtDCInfo j_emlrtDCI =
    {
        105,        /* lineNo */
        34,         /* colNo */
        "pfqn_mva", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mva.m", /* pName */
        1             /* checkKind */
};

static emlrtBCInfo le_emlrtBCI =
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

static emlrtBCInfo me_emlrtBCI =
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

static emlrtBCInfo ne_emlrtBCI =
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

static emlrtBCInfo oe_emlrtBCI =
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

static emlrtBCInfo pe_emlrtBCI =
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

static emlrtBCInfo qe_emlrtBCI =
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

static emlrtBCInfo re_emlrtBCI =
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

static emlrtBCInfo se_emlrtBCI =
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

static emlrtBCInfo te_emlrtBCI =
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

static emlrtBCInfo ue_emlrtBCI =
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

static emlrtBCInfo ve_emlrtBCI =
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

static emlrtBCInfo we_emlrtBCI =
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

static emlrtBCInfo xe_emlrtBCI =
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

static emlrtBCInfo ye_emlrtBCI =
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

static emlrtBCInfo af_emlrtBCI =
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

static emlrtBCInfo bf_emlrtBCI =
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

static emlrtBCInfo cf_emlrtBCI =
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

static emlrtBCInfo df_emlrtBCI =
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

static emlrtBCInfo ef_emlrtBCI =
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

static emlrtBCInfo gf_emlrtBCI =
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

static emlrtBCInfo hf_emlrtBCI =
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
    fd_emlrtRTEI =
        {
            37,         /* lineNo */
            3,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    gd_emlrtRTEI =
        {
            37,         /* lineNo */
            1,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    hd_emlrtRTEI =
        {
            42,         /* lineNo */
            5,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    id_emlrtRTEI =
        {
            51,         /* lineNo */
            10,         /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    jd_emlrtRTEI =
        {
            60,         /* lineNo */
            1,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    kd_emlrtRTEI =
        {
            61,         /* lineNo */
            1,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    ld_emlrtRTEI =
        {
            62,         /* lineNo */
            4,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    md_emlrtRTEI =
        {
            63,         /* lineNo */
            1,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    nd_emlrtRTEI =
        {
            65,         /* lineNo */
            7,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    od_emlrtRTEI =
        {
            65,         /* lineNo */
            5,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    pd_emlrtRTEI =
        {
            70,         /* lineNo */
            1,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    qd_emlrtRTEI =
        {
            80,         /* lineNo */
            13,         /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    rd_emlrtRTEI =
        {
            72,         /* lineNo */
            23,         /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    sd_emlrtRTEI =
        {
            82,         /* lineNo */
            1,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    td_emlrtRTEI =
        {
            85,         /* lineNo */
            1,          /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    ud_emlrtRTEI =
        {
            124,        /* lineNo */
            28,         /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    vd_emlrtRTEI =
        {
            124,        /* lineNo */
            52,         /* colNo */
            "pfqn_mva", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

static emlrtRTEInfo
    wd_emlrtRTEI =
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
  emxInit_real_T(sp, &r, 2, &rd_emlrtRTEI);
  stride_1_1 = r->size[0] * r->size[1];
  r->size[0] = 1;
  i = in7 - in6;
  if (i == 1) {
    loop_ub = (in4 - in2) - 1;
  } else {
    loop_ub = i;
  }
  r->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, r, stride_1_1, &rd_emlrtRTEI);
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
    emlrtDynamicBoundsCheckR2012b(in2 + 1, 1, stride_1_1, &td_emlrtBCI,
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
  emxArray_boolean_T *c_N;
  emxArray_real_T *b_N;
  emxArray_real_T *n;
  emxArray_real_T *prods;
  const mxArray *b_m;
  const mxArray *b_y;
  const mxArray *c_y;
  const mxArray *y;
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
  int32_T k;
  int32_T loop_ub;
  int32_T m;
  int32_T nx;
  int32_T pfqn_mva_numThreads;
  int32_T vectorUB;
  int32_T w;
  boolean_T *c_N_data;
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
  st.site = &mb_emlrtRSI;
  b_st.site = &vb_emlrtRSI;
  nx = N->size[1];
  c_st.site = &wb_emlrtRSI;
  if (N->size[1] > 2147483646) {
    d_st.site = &gb_emlrtRSI;
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
  emxInit_real_T(sp, &b_N, 2, &fd_emlrtRTEI);
  vectorUB = b_N->size[0] * b_N->size[1];
  b_N->size[0] = 1;
  b_N->size[1] = nx;
  emxEnsureCapacity_real_T(sp, b_N, vectorUB, &fd_emlrtRTEI);
  b_N_data = b_N->data;
  for (m = 0; m < nx; m++) {
    b_N_data[m] = N_data[m];
  }
  vectorUB = N->size[0] * N->size[1];
  N->size[0] = 1;
  N->size[1] = nx;
  emxEnsureCapacity_real_T(sp, N, vectorUB, &gd_emlrtRTEI);
  N_data = N->data;
  for (m = 0; m < nx; m++) {
    N_data[m] = b_N_data[m];
  }
  if (Z->size[1] == 0) {
    vectorUB = Z->size[0] * Z->size[1];
    Z->size[0] = 1;
    Z->size[1] = L->size[1];
    emxEnsureCapacity_real_T(sp, Z, vectorUB, &hd_emlrtRTEI);
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
  emxInit_boolean_T(sp, &c_N, &id_emlrtRTEI);
  vectorUB = c_N->size[0] * c_N->size[1];
  c_N->size[0] = 1;
  c_N->size[1] = nx;
  emxEnsureCapacity_boolean_T(sp, c_N, vectorUB, &id_emlrtRTEI);
  c_N_data = c_N->data;
  if (loop_ub) {
    for (i = 0; i < nx; i++) {
      c_N_data[i] = (N_data[i] > 0.0);
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    pfqn_mva_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mva_numThreads)

    for (i = 0; i < nx; i++) {
      c_N_data[i] = (N_data[i] > 0.0);
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  st.site = &nb_emlrtRSI;
  if (any(&st, c_N)) {
    __m128d r;
    real_T currentpop;
    real_T totpop;
    int32_T b_Z;
    int32_T b_loop_ub;
    int32_T exitg1;
    int32_T firstnonempty;
    int32_T last_nnz;
    if (L->size[1] != N->size[1]) {
      st.site = &ob_emlrtRSI;
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
      y = NULL;
      b_m = emlrtCreateCharArray(2, &iv[0]);
      emlrtInitCharArrayR2013a(&b_st, 6, b_m, &varargin_1[0]);
      emlrtAssign(&y, b_m);
      b_y = NULL;
      b_m = emlrtCreateCharArray(2, &iv1[0]);
      emlrtInitCharArrayR2013a(&b_st, 8, b_m, &varargin_2[0]);
      emlrtAssign(&b_y, b_m);
      c_y = NULL;
      b_m = emlrtCreateCharArray(2, &iv2[0]);
      emlrtInitCharArrayR2013a(&b_st, 68, b_m, &varargin_3[0]);
      emlrtAssign(&c_y, b_m);
      c_st.site = &ng_emlrtRSI;
      f_error(&c_st, y, b_y, c_y, &emlrtMCI);
    }
    vectorUB = XN->size[0] * XN->size[1];
    XN->size[0] = 1;
    XN->size[1] = R;
    emxEnsureCapacity_real_T(sp, XN, vectorUB, &jd_emlrtRTEI);
    XN_data = XN->data;
    for (m = 0; m < R; m++) {
      XN_data[m] = 0.0;
    }
    vectorUB = QN->size[0] * QN->size[1];
    QN->size[0] = M_original;
    QN->size[1] = R;
    emxEnsureCapacity_real_T(sp, QN, vectorUB, &kd_emlrtRTEI);
    QN_data = QN->data;
    loop_ub = L->size[0] * L->size[1];
    for (m = 0; m < loop_ub; m++) {
      QN_data[m] = 0.0;
    }
    vectorUB = UN->size[0] * UN->size[1];
    UN->size[0] = M_original;
    UN->size[1] = R;
    emxEnsureCapacity_real_T(sp, UN, vectorUB, &ld_emlrtRTEI);
    UN_data = UN->data;
    vectorUB = CN->size[0] * CN->size[1];
    CN->size[0] = M_original;
    CN->size[1] = R;
    emxEnsureCapacity_real_T(sp, CN, vectorUB, &md_emlrtRTEI);
    CN_data = CN->data;
    for (m = 0; m < loop_ub; m++) {
      CN_data[m] = 0.0;
    }
    b_Z = Z->size[1];
    vectorUB = b_N->size[0] * b_N->size[1];
    b_N->size[0] = 1;
    b_N->size[1] = Z->size[1];
    emxEnsureCapacity_real_T(sp, b_N, vectorUB, &nd_emlrtRTEI);
    b_N_data = b_N->data;
    for (m = 0; m < b_Z; m++) {
      b_N_data[m] = Z_data[m];
    }
    vectorUB = Z->size[0] * Z->size[1];
    Z->size[0] = 1;
    Z->size[1] = b_Z;
    emxEnsureCapacity_real_T(sp, Z, vectorUB, &od_emlrtRTEI);
    Z_data = Z->data;
    for (m = 0; m < b_Z; m++) {
      Z_data[m] = b_N_data[m];
    }
    if (L->size[1] - 1 < 0) {
      emlrtNonNegativeCheckR2012b(-1.0, &i_emlrtDCI, (emlrtConstCTX)sp);
    }
    loop_ub = L->size[1] - 1;
    emxInit_real_T(sp, &prods, 2, &pd_emlrtRTEI);
    vectorUB = prods->size[0] * prods->size[1];
    prods->size[0] = 1;
    prods->size[1] = L->size[1] - 1;
    emxEnsureCapacity_real_T(sp, prods, vectorUB, &pd_emlrtRTEI);
    prods_data = prods->data;
    for (m = 0; m < loop_ub; m++) {
      prods_data[m] = 0.0;
    }
    /*  generate population indices */
    for (w = 0; w <= R - 2; w++) {
      if (w + 2 > R) {
        last_nnz = 0;
        loop_ub = 0;
      } else {
        if (w + 2 > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(w + 2, 1, N->size[1], &rd_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        last_nnz = w + 1;
        if (R > N->size[1]) {
          emlrtDynamicBoundsCheckR2012b(R, 1, N->size[1], &qd_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        loop_ub = R;
      }
      nx = R - w;
      if (!((real_T)(nx - 2) + 1.0 >= 0.0)) {
        emlrtNonNegativeCheckR2012b((real_T)(nx - 2) + 1.0, &h_emlrtDCI,
                                    (emlrtConstCTX)sp);
      }
      firstnonempty = nx - 1;
      vectorUB = loop_ub - last_nnz;
      if ((nx - 1 != vectorUB) && ((nx - 1 != 1) && (vectorUB != 1))) {
        emlrtDimSizeImpxCheckR2021b(nx - 1, vectorUB, &f_emlrtECI,
                                    (emlrtConstCTX)sp);
      }
      if (nx - 1 == vectorUB) {
        loop_ub = b_N->size[0] * b_N->size[1];
        b_N->size[0] = 1;
        b_N->size[1] = nx - 1;
        emxEnsureCapacity_real_T(sp, b_N, loop_ub, &rd_emlrtRTEI);
        b_N_data = b_N->data;
        loop_ub = ((nx - 1) / 2) << 1;
        vectorUB = loop_ub - 2;
        for (m = 0; m <= vectorUB; m += 2) {
          r = _mm_loadu_pd(&N_data[last_nnz + m]);
          _mm_storeu_pd(&b_N_data[m], _mm_add_pd(_mm_set1_pd(1.0), r));
        }
        for (m = loop_ub; m < firstnonempty; m++) {
          b_N_data[m] = N_data[last_nnz + m] + 1.0;
        }
        if (w + 1 > prods->size[1]) {
          emlrtDynamicBoundsCheckR2012b(w + 1, 1, prods->size[1], &td_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        st.site = &pb_emlrtRSI;
        prods_data[w] = prod(&st, b_N);
      } else {
        st.site = &pb_emlrtRSI;
        binary_expand_op_1(&st, prods, w, pb_emlrtRSI, R, N, last_nnz, loop_ub);
        prods_data = prods->data;
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    firstnonempty = R;
    do {
      exitg1 = 0;
      b_loop_ub = N->size[1];
      if ((firstnonempty < 1) || (firstnonempty > N->size[1])) {
        emlrtDynamicBoundsCheckR2012b(firstnonempty, 1, N->size[1],
                                      &sd_emlrtBCI, (emlrtConstCTX)sp);
      }
      if (N_data[firstnonempty - 1] == 0.0) {
        firstnonempty--;
      } else {
        exitg1 = 1;
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    } while (exitg1 == 0);
    vectorUB = b_N->size[0] * b_N->size[1];
    b_N->size[0] = 1;
    b_N->size[1] = N->size[1];
    emxEnsureCapacity_real_T(sp, b_N, vectorUB, &qd_emlrtRTEI);
    b_N_data = b_N->data;
    loop_ub = (N->size[1] / 2) << 1;
    vectorUB = loop_ub - 2;
    for (m = 0; m <= vectorUB; m += 2) {
      r = _mm_loadu_pd(&N_data[m]);
      _mm_storeu_pd(&b_N_data[m], _mm_add_pd(r, _mm_set1_pd(1.0)));
    }
    for (m = loop_ub; m < b_loop_ub; m++) {
      b_N_data[m] = N_data[m] + 1.0;
    }
    st.site = &qb_emlrtRSI;
    totpop = prod(&st, b_N);
    if (!(totpop >= 0.0)) {
      emlrtNonNegativeCheckR2012b(totpop, &g_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (totpop != (int32_T)muDoubleScalarFloor(totpop)) {
      emlrtIntegerCheckR2012b(totpop, &f_emlrtDCI, (emlrtConstCTX)sp);
    }
    emxInit_real_T(sp, &Z, 2, &sd_emlrtRTEI);
    vectorUB = Z->size[0] * Z->size[1];
    Z->size[0] = (int32_T)totpop;
    Z->size[1] = M_original;
    emxEnsureCapacity_real_T(sp, Z, vectorUB, &sd_emlrtRTEI);
    Q_data = Z->data;
    vectorUB = (int32_T)totpop * L->size[0];
    for (m = 0; m < vectorUB; m++) {
      Q_data[m] = 0.0;
    }
    currentpop = 2.0;
    emxInit_real_T(sp, &N, 2, &td_emlrtRTEI);
    vectorUB = N->size[0] * N->size[1];
    N->size[0] = 1;
    N->size[1] = R;
    emxEnsureCapacity_real_T(sp, N, vectorUB, &td_emlrtRTEI);
    n_data = N->data;
    for (m = 0; m < R; m++) {
      n_data[m] = 0.0;
    }
    if (firstnonempty > R) {
      emlrtDynamicBoundsCheckR2012b(firstnonempty, 1, R, &ud_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    n_data[firstnonempty - 1] = 1.0;
    emxInit_real_T(sp, &n, 2, &ud_emlrtRTEI);
    do {
      exitg1 = 0;
      st.site = &rb_emlrtRSI;
      if (muDoubleScalarIsNaN(totpop)) {
        emlrtErrorWithMessageIdR2018a(&st, &g_emlrtRTEI, "MATLAB:nologicalnan",
                                      "MATLAB:nologicalnan", 0);
      }
      if (totpop != 0.0) {
        uint32_T s;
        boolean_T exitg3;
        /*  for each population */
        s = 1U;
        while (s <= (uint32_T)R) {
          real_T CNtot;
          real_T pos_n_1s;
          uint32_T b_i;
          pos_n_1s = 0.0;
          if ((int32_T)s > N->size[1]) {
            emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, N->size[1],
                                          &vd_emlrtBCI, (emlrtConstCTX)sp);
          }
          CNtot = n_data[(int32_T)s - 1];
          if (CNtot > 0.0) {
            if ((int32_T)s > N->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, N->size[1],
                                            &ee_emlrtBCI, (emlrtConstCTX)sp);
            }
            n_data[(int32_T)s - 1] = CNtot - 1.0;
            loop_ub = N->size[1];
            if (R > N->size[1]) {
              emlrtDynamicBoundsCheckR2012b(R, 1, N->size[1], &fe_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            pos_n_1s = n_data[R - 1];
            vectorUB = 1;
            while (vectorUB <= R - 1) {
              if (vectorUB > loop_ub) {
                emlrtDynamicBoundsCheckR2012b(vectorUB, 1, loop_ub,
                                              &we_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (vectorUB > prods->size[1]) {
                emlrtDynamicBoundsCheckR2012b(vectorUB, 1, prods->size[1],
                                              &xe_emlrtBCI, (emlrtConstCTX)sp);
              }
              pos_n_1s += n_data[vectorUB - 1] * prods_data[vectorUB - 1];
              vectorUB++;
              if (*emlrtBreakCheckR2012bFlagVar != 0) {
                emlrtBreakCheckR2012b((emlrtConstCTX)sp);
              }
            }
            /*  while w <= R-1 */
            if ((int32_T)s > N->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, N->size[1],
                                            &qe_emlrtBCI, (emlrtConstCTX)sp);
            }
            n_data[(int32_T)s - 1]++;
          }
          /*  if */
          CNtot = 0.0;
          b_i = 1U;
          while (b_i <= (uint32_T)M_original) {
            if ((int32_T)b_i > M_original) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, M_original,
                                            &pd_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)s > R) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, R, &od_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            vectorUB = mi->size[0] * mi->size[1];
            if ((int32_T)b_i > vectorUB) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, vectorUB,
                                            &je_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (pos_n_1s + 1.0 !=
                (int32_T)muDoubleScalarFloor(pos_n_1s + 1.0)) {
              emlrtIntegerCheckR2012b(pos_n_1s + 1.0, &j_emlrtDCI,
                                      (emlrtConstCTX)sp);
            }
            if (((int32_T)(pos_n_1s + 1.0) < 1) ||
                ((int32_T)(pos_n_1s + 1.0) > Z->size[0])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)(pos_n_1s + 1.0), 1,
                                            Z->size[0], &ke_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if ((int32_T)b_i > Z->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, Z->size[1],
                                            &le_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)b_i > CN->size[0]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, CN->size[0],
                                            &me_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)s > CN->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, CN->size[1],
                                            &ne_emlrtBCI, (emlrtConstCTX)sp);
            }
            CN_data[((int32_T)b_i + CN->size[0] * ((int32_T)s - 1)) - 1] =
                L_data[((int32_T)b_i + L->size[0] * ((int32_T)s - 1)) - 1] *
                (mi_data[(int32_T)b_i - 1] +
                 Q_data[((int32_T)(pos_n_1s + 1.0) +
                         Z->size[0] * ((int32_T)b_i - 1)) -
                        1]);
            if ((int32_T)b_i > CN->size[0]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, CN->size[0],
                                            &oe_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)s > CN->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, CN->size[1],
                                            &pe_emlrtBCI, (emlrtConstCTX)sp);
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
                                          &ge_emlrtBCI, (emlrtConstCTX)sp);
          }
          if ((int32_T)s > b_Z) {
            emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, b_Z, &he_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if ((int32_T)s > XN->size[1]) {
            emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, XN->size[1],
                                          &ie_emlrtBCI, (emlrtConstCTX)sp);
          }
          XN_data[(int32_T)s - 1] =
              n_data[(int32_T)s - 1] / (Z_data[(int32_T)s - 1] + CNtot);
          b_i = 1U;
          while (b_i <= (uint32_T)M_original) {
            if ((int32_T)s > XN->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, XN->size[1],
                                            &re_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)b_i > CN->size[0]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, CN->size[0],
                                            &se_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)s > CN->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, CN->size[1],
                                            &te_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)b_i > QN->size[0]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, QN->size[0],
                                            &ue_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)s > QN->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, QN->size[1],
                                            &ve_emlrtBCI, (emlrtConstCTX)sp);
            }
            QN_data[((int32_T)b_i + QN->size[0] * ((int32_T)s - 1)) - 1] =
                XN_data[(int32_T)s - 1] *
                CN_data[((int32_T)b_i + CN->size[0] * ((int32_T)s - 1)) - 1];
            if (((int32_T)currentpop < 1) ||
                ((int32_T)currentpop > Z->size[0])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)currentpop, 1, Z->size[0],
                                            &ye_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)b_i > Z->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, Z->size[1],
                                            &af_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)b_i > QN->size[0]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, QN->size[0],
                                            &bf_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)s > QN->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, QN->size[1],
                                            &cf_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (((int32_T)currentpop < 1) ||
                ((int32_T)currentpop > Z->size[0])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)currentpop, 1, Z->size[0],
                                            &df_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)b_i > Z->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)b_i, 1, Z->size[1],
                                            &ef_emlrtBCI, (emlrtConstCTX)sp);
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
        int32_T exitg2;
        boolean_T guard1;
        do {
          exitg2 = 0;
          guard1 = false;
          if ((int32_T)s > 0) {
            if (((int32_T)s < 1) || ((int32_T)s > N->size[1])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, N->size[1],
                                            &ce_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (((int32_T)s < 1) || ((int32_T)s > b_loop_ub)) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, b_loop_ub,
                                            &de_emlrtBCI, (emlrtConstCTX)sp);
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
              exitg2 = 1;
            }
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b((emlrtConstCTX)sp);
          }
        } while (exitg2 == 0);
        /*  now compute the normalizing constant */
        st.site = &sb_emlrtRSI;
        last_nnz = -1;
        loop_ub = N->size[1];
        emlrtForLoopVectorCheckR2021a(N->size[1], -1.0, 1.0, mxDOUBLE_CLASS,
                                      N->size[1], &h_emlrtRTEI, &st);
        vectorUB = 0;
        exitg3 = false;
        while ((!exitg3) && (vectorUB <= loop_ub - 1)) {
          nx = loop_ub - vectorUB;
          if ((nx < 1) || (nx > loop_ub)) {
            emlrtDynamicBoundsCheckR2012b(nx, 1, loop_ub, &ff_emlrtBCI, &st);
          }
          if (n_data[nx - 1] != 0.0) {
            last_nnz = nx - 1;
            exitg3 = true;
          } else {
            vectorUB++;
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&st);
          }
        }
        if (last_nnz + 1 > 0) {
          if (last_nnz < 1) {
            vectorUB = 0;
            nx = 0;
          } else {
            if (N->size[1] < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, N->size[1], &nd_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (last_nnz > N->size[1]) {
              emlrtDynamicBoundsCheckR2012b(last_nnz, 1, N->size[1],
                                            &md_emlrtBCI, (emlrtConstCTX)sp);
            }
            vectorUB = last_nnz;
            if (b_loop_ub < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, b_loop_ub, &ld_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (last_nnz > b_loop_ub) {
              emlrtDynamicBoundsCheckR2012b(last_nnz, 1, b_loop_ub,
                                            &kd_emlrtBCI, (emlrtConstCTX)sp);
            }
            nx = last_nnz;
          }
          loop_ub = n->size[0] * n->size[1];
          n->size[0] = 1;
          n->size[1] = vectorUB;
          emxEnsureCapacity_real_T(sp, n, loop_ub, &ud_emlrtRTEI);
          b_N_data = n->data;
          for (m = 0; m < vectorUB; m++) {
            b_N_data[m] = n_data[m];
          }
          loop_ub = b_N->size[0] * b_N->size[1];
          b_N->size[0] = 1;
          b_N->size[1] = nx;
          emxEnsureCapacity_real_T(sp, b_N, loop_ub, &vd_emlrtRTEI);
          b_N_data = b_N->data;
          for (m = 0; m < nx; m++) {
            b_N_data[m] = N_data[m];
          }
          st.site = &tb_emlrtRSI;
          if (b_sum(&st, n) == b_sum(&st, b_N)) {
            if ((uint32_T)last_nnz + 2U > (uint32_T)R) {
              nx = 0;
              loop_ub = 0;
            } else {
              if (((int32_T)((uint32_T)last_nnz + 2U) < 1) ||
                  ((int32_T)((uint32_T)last_nnz + 2U) > N->size[1])) {
                emlrtDynamicBoundsCheckR2012b(
                    (int32_T)((uint32_T)last_nnz + 2U), 1, N->size[1],
                    &jd_emlrtBCI, (emlrtConstCTX)sp);
              }
              nx = last_nnz + 1;
              if (R > N->size[1]) {
                emlrtDynamicBoundsCheckR2012b(R, 1, N->size[1], &id_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              loop_ub = R;
            }
            vectorUB = n->size[0] * n->size[1];
            n->size[0] = 1;
            loop_ub -= nx;
            n->size[1] = loop_ub;
            emxEnsureCapacity_real_T(sp, n, vectorUB, &wd_emlrtRTEI);
            b_N_data = n->data;
            for (m = 0; m < loop_ub; m++) {
              b_N_data[m] = n_data[nx + m];
            }
            st.site = &tb_emlrtRSI;
            if (b_sum(&st, n) == 0.0) {
              st.site = &ub_emlrtRSI;
              if ((last_nnz < 0) || (last_nnz + 1 > XN->size[1])) {
                emlrtDynamicBoundsCheckR2012b(last_nnz + 1, 1, XN->size[1],
                                              &hd_emlrtBCI, &st);
              }
              if (XN_data[last_nnz] < 0.0) {
                emlrtErrorWithMessageIdR2018a(
                    &st, &f_emlrtRTEI, "Coder:toolbox:ElFunDomainError",
                    "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
              }
              lGN -= muDoubleScalarLog(XN_data[last_nnz]);
            }
          }
        }
        if ((int32_T)s == 0) {
          exitg1 = 1;
        } else {
          if (((int32_T)s < 1) || ((int32_T)s > N->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, N->size[1],
                                          &gf_emlrtBCI, (emlrtConstCTX)sp);
          }
          n_data[(int32_T)s - 1]++;
          s++;
          while (s <= (uint32_T)R) {
            if (((int32_T)s < 1) || ((int32_T)s > N->size[1])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)s, 1, N->size[1],
                                            &hf_emlrtBCI, (emlrtConstCTX)sp);
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
        exitg1 = 1;
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    } while (exitg1 == 0);
    emxFree_real_T(sp, &n);
    emxFree_real_T(sp, &N);
    emxFree_real_T(sp, &Z);
    emxFree_real_T(sp, &prods);
    for (m = 0; m < M_original; m++) {
      for (w = 0; w < R; w++) {
        if (w + 1 > XN->size[1]) {
          emlrtDynamicBoundsCheckR2012b(w + 1, 1, XN->size[1], &wd_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (m + 1 > M_original) {
          emlrtDynamicBoundsCheckR2012b(m + 1, 1, M_original, &xd_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (w + 1 > R) {
          emlrtDynamicBoundsCheckR2012b(w + 1, 1, R, &yd_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (m + 1 > UN->size[0]) {
          emlrtDynamicBoundsCheckR2012b(m + 1, 1, UN->size[0], &ae_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (w + 1 > UN->size[1]) {
          emlrtDynamicBoundsCheckR2012b(w + 1, 1, UN->size[1], &be_emlrtBCI,
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
  emxFree_boolean_T(sp, &c_N);
  emxFree_real_T(sp, &b_N);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return lGN;
}

/* End of code generation (pfqn_mva.c) */
