/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mvaldmx.c
 *
 * Code generation for function 'pfqn_mvaldmx'
 *
 */

/* Include files */
#include "pfqn_mvaldmx.h"
#include "any.h"
#include "eml_setop.h"
#include "error.h"
#include "find.h"
#include "ifWhileCond.h"
#include "pfqn_mvaldmx_ec.h"
#include "pfqn_mvams_data.h"
#include "pfqn_mvams_emxutil.h"
#include "pfqn_mvams_types.h"
#include "pprod.h"
#include "prod.h"
#include "repmat.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>

/* Variable Definitions */
static emlrtRSInfo bd_emlrtRSI =
    {
        33,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo cd_emlrtRSI =
    {
        34,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo dd_emlrtRSI =
    {
        36,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo ed_emlrtRSI =
    {
        37,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo fd_emlrtRSI =
    {
        40,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo gd_emlrtRSI =
    {
        41,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo hd_emlrtRSI =
    {
        48,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo id_emlrtRSI =
    {
        49,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo jd_emlrtRSI =
    {
        56,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo kd_emlrtRSI =
    {
        61,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo ld_emlrtRSI =
    {
        62,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo md_emlrtRSI =
    {
        63,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo nd_emlrtRSI =
    {
        69,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo od_emlrtRSI =
    {
        70,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo pd_emlrtRSI =
    {
        71,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo qd_emlrtRSI =
    {
        75,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo rd_emlrtRSI =
    {
        65,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo sd_emlrtRSI =
    {
        85,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo td_emlrtRSI =
    {
        91,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo ud_emlrtRSI =
    {
        96,             /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo vd_emlrtRSI =
    {
        100,            /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo wd_emlrtRSI =
    {
        103,            /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo xd_emlrtRSI =
    {
        104,            /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo yd_emlrtRSI =
    {
        111,            /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo ae_emlrtRSI =
    {
        115,            /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo be_emlrtRSI =
    {
        117,            /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo ce_emlrtRSI =
    {
        120,            /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo de_emlrtRSI =
    {
        133,            /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo ee_emlrtRSI =
    {
        143,            /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo fe_emlrtRSI =
    {
        151,            /* lineNo */
        "pfqn_mvaldmx", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pathName */
};

static emlrtRSInfo pe_emlrtRSI = {
    41,    /* lineNo */
    "cat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pathName
                                                                       */
};

static emlrtRSInfo qe_emlrtRSI = {
    65,         /* lineNo */
    "cat_impl", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pathName
                                                                       */
};

static emlrtECInfo g_emlrtECI =
    {
        2,              /* nDims */
        36,             /* lineNo */
        8,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtBCInfo if_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    48,             /* lineNo */
    16,             /* colNo */
    "mu",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo jf_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    56,             /* lineNo */
    22,             /* colNo */
    "Nc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo kf_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    56,             /* lineNo */
    24,             /* colNo */
    "Nc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo k_emlrtDCI = {
    61,             /* lineNo */
    14,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    4                 /* checkKind */
};

static emlrtDCInfo l_emlrtDCI = {
    61,             /* lineNo */
    14,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    1                 /* checkKind */
};

static emlrtDCInfo m_emlrtDCI = {
    61,             /* lineNo */
    24,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    4                 /* checkKind */
};

static emlrtDCInfo n_emlrtDCI = {
    61,             /* lineNo */
    24,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    1                 /* checkKind */
};

static emlrtDCInfo o_emlrtDCI = {
    62,             /* lineNo */
    13,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    4                 /* checkKind */
};

static emlrtDCInfo p_emlrtDCI = {
    62,             /* lineNo */
    13,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    1                 /* checkKind */
};

static emlrtDCInfo q_emlrtDCI = {
    63,             /* lineNo */
    15,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    4                 /* checkKind */
};

static emlrtDCInfo r_emlrtDCI = {
    63,             /* lineNo */
    15,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    1                 /* checkKind */
};

static emlrtRTEInfo k_emlrtRTEI =
    {
        77,             /* lineNo */
        23,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtBCInfo lf_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    78,             /* lineNo */
    58,             /* colNo */
    "Dc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo mf_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    78,             /* lineNo */
    62,             /* colNo */
    "Dc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo nf_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    65,             /* lineNo */
    13,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo s_emlrtDCI = {
    65,             /* lineNo */
    20,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo of_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    65,             /* lineNo */
    20,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo pf_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    85,             /* lineNo */
    45,             /* colNo */
    "w",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo qf_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    85,             /* lineNo */
    47,             /* colNo */
    "w",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo rf_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    85,             /* lineNo */
    49,             /* colNo */
    "w",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo sf_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    85,             /* lineNo */
    51,             /* colNo */
    "w",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtRTEInfo l_emlrtRTEI =
    {
        88,             /* lineNo */
        15,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtBCInfo tf_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    72,             /* colNo */
    "Dc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo uf_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    76,             /* colNo */
    "Dc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo vf_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    96,             /* lineNo */
    50,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo wf_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    96,             /* lineNo */
    67,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo xf_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    96,             /* lineNo */
    17,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo yf_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    103,            /* lineNo */
    21,             /* colNo */
    "nvec",         /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ag_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    103,            /* lineNo */
    23,             /* colNo */
    "nvec",         /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo bg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    103,            /* lineNo */
    46,             /* colNo */
    "Nc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo cg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    103,            /* lineNo */
    48,             /* colNo */
    "Nc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo dg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    103,            /* lineNo */
    74,             /* colNo */
    "nvec",         /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo eg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    103,            /* lineNo */
    86,             /* colNo */
    "nvec",         /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo fg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    104,            /* lineNo */
    27,             /* colNo */
    "XN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtRTEInfo m_emlrtRTEI =
    {
        120,            /* lineNo */
        15,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtBCInfo gg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    121,            /* lineNo */
    38,             /* colNo */
    "Dc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo hg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    121,            /* lineNo */
    42,             /* colNo */
    "Dc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ig_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    127,            /* lineNo */
    23,             /* colNo */
    "x",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo jg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    127,            /* lineNo */
    25,             /* colNo */
    "x",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo t_emlrtDCI = {
    127,            /* lineNo */
    27,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo kg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    127,            /* lineNo */
    27,             /* colNo */
    "x",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtECInfo h_emlrtECI =
    {
        -1,             /* nDims */
        127,            /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtBCInfo lg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    129,            /* lineNo */
    27,             /* colNo */
    "u",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo mg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    129,            /* lineNo */
    29,             /* colNo */
    "u",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ng_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    129,            /* lineNo */
    31,             /* colNo */
    "u",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo og_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    129,            /* lineNo */
    33,             /* colNo */
    "u",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo pg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    131,            /* lineNo */
    27,             /* colNo */
    "w",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo qg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    131,            /* lineNo */
    29,             /* colNo */
    "w",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo rg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    131,            /* lineNo */
    31,             /* colNo */
    "w",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo sg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    131,            /* lineNo */
    33,             /* colNo */
    "w",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo tg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    131,            /* lineNo */
    35,             /* colNo */
    "w",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtECInfo i_emlrtECI =
    {
        -1,             /* nDims */
        131,            /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtBCInfo ug_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    133,            /* lineNo */
    61,             /* colNo */
    "CN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtECInfo j_emlrtECI =
    {
        1,              /* nDims */
        133,            /* lineNo */
        25,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtECInfo k_emlrtECI =
    {
        2,              /* nDims */
        133,            /* lineNo */
        25,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo n_emlrtRTEI =
    {
        143,            /* lineNo */
        15,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtECInfo l_emlrtECI =
    {
        -1,             /* nDims */
        133,            /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo o_emlrtRTEI =
    {
        151,            /* lineNo */
        15,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtECInfo m_emlrtECI =
    {
        -1,             /* nDims */
        129,            /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtBCInfo vg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    157,            /* lineNo */
    9,              /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo wg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    157,            /* lineNo */
    11,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo xg_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    157,            /* lineNo */
    15,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtRTEInfo p_emlrtRTEI = {
    225,                   /* lineNo */
    27,                    /* colNo */
    "check_non_axis_size", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pName
                                                                       */
};

static emlrtBCInfo yg_emlrtBCI = {
    -1,        /* iFirst */
    -1,        /* iLast */
    17,        /* lineNo */
    31,        /* colNo */
    "n",       /* aName */
    "hashpop", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/hashpop.m", /* pName */
    0 /* checkKind */
};

static emlrtBCInfo ah_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    36,             /* lineNo */
    37,             /* colNo */
    "N",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo bh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    33,             /* lineNo */
    23,             /* colNo */
    "N",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ch_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    36,             /* lineNo */
    8,              /* colNo */
    "N",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo dh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    51,             /* lineNo */
    10,             /* colNo */
    "D",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo eh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    52,             /* lineNo */
    8,              /* colNo */
    "N",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo fh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    53,             /* lineNo */
    8,              /* colNo */
    "Z",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo gh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    56,             /* lineNo */
    11,             /* colNo */
    "prods",        /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo hh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    65,             /* lineNo */
    8,              /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ih_emlrtBCI = {
    -1,        /* iFirst */
    -1,        /* iLast */
    17,        /* lineNo */
    26,        /* colNo */
    "prods",   /* aName */
    "hashpop", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/hashpop.m", /* pName */
    0 /* checkKind */
};

static emlrtBCInfo jh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    74,             /* lineNo */
    21,             /* colNo */
    "nvec",         /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo kh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    127,            /* lineNo */
    4,              /* colNo */
    "XN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo lh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    85,             /* lineNo */
    27,             /* colNo */
    "nvec",         /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo mh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    85,             /* lineNo */
    36,             /* colNo */
    "Zc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo nh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    85,             /* lineNo */
    11,             /* colNo */
    "x",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo oh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    85,             /* lineNo */
    13,             /* colNo */
    "x",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo u_emlrtDCI = {
    85,             /* lineNo */
    13,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo qh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    90,             /* lineNo */
    25,             /* colNo */
    "nvec",         /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo rh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    96,             /* lineNo */
    55,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo v_emlrtDCI = {
    96,             /* lineNo */
    55,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo sh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    96,             /* lineNo */
    12,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo th_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    96,             /* lineNo */
    24,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo w_emlrtDCI = {
    96,             /* lineNo */
    24,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo uh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    129,            /* lineNo */
    8,              /* colNo */
    "UN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo vh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    119,            /* lineNo */
    11,             /* colNo */
    "u",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo wh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    119,            /* lineNo */
    15,             /* colNo */
    "u",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo xh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    78,             /* lineNo */
    40,             /* colNo */
    "w",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo yh_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    78,             /* lineNo */
    44,             /* colNo */
    "w",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ai_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    78,             /* lineNo */
    46,             /* colNo */
    "w",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo bi_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    78,             /* lineNo */
    74,             /* colNo */
    "EC",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ci_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    78,             /* lineNo */
    78,             /* colNo */
    "EC",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo di_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    78,             /* lineNo */
    86,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ei_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    78,             /* lineNo */
    91,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo fi_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    78,             /* lineNo */
    100,            /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo x_emlrtDCI = {
    78,             /* lineNo */
    100,            /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo gi_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    78,             /* lineNo */
    23,             /* colNo */
    "w",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo hi_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    78,             /* lineNo */
    27,             /* colNo */
    "w",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ii_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    78,             /* lineNo */
    29,             /* colNo */
    "w",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo y_emlrtDCI = {
    78,             /* lineNo */
    29,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo ji_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    121,            /* lineNo */
    26,             /* colNo */
    "u",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ki_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    121,            /* lineNo */
    30,             /* colNo */
    "u",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo li_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    121,            /* lineNo */
    49,             /* colNo */
    "x",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo mi_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    121,            /* lineNo */
    51,             /* colNo */
    "x",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo ab_emlrtDCI = {
    121,            /* lineNo */
    51,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo ni_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    121,            /* lineNo */
    67,             /* colNo */
    "Eprime",       /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo oi_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    121,            /* lineNo */
    71,             /* colNo */
    "Eprime",       /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo pi_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    121,            /* lineNo */
    82,             /* colNo */
    "E",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo qi_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    121,            /* lineNo */
    86,             /* colNo */
    "E",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ri_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    121,            /* lineNo */
    98,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo si_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    121,            /* lineNo */
    103,            /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ti_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    121,            /* lineNo */
    110,            /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo bb_emlrtDCI = {
    121,            /* lineNo */
    110,            /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo ui_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    121,            /* lineNo */
    15,             /* colNo */
    "u",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo vi_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    121,            /* lineNo */
    19,             /* colNo */
    "u",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo wi_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    131,            /* lineNo */
    8,              /* colNo */
    "CN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo xi_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    48,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo yi_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    53,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo aj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    60,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo bj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    84,             /* colNo */
    "EC",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo cj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    88,             /* colNo */
    "EC",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo dj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    95,             /* colNo */
    "x",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ej_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    97,             /* colNo */
    "x",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo fj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    109,            /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo gj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    114,            /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo hj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    123,            /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo cb_emlrtDCI = {
    92,             /* lineNo */
    123,            /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo ij_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    24,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo jj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    29,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo kj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    92,             /* lineNo */
    36,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtDCInfo db_emlrtDCI = {
    92,             /* lineNo */
    36,             /* colNo */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    1                 /* checkKind */
};

static emlrtBCInfo lj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    133,            /* lineNo */
    65,             /* colNo */
    "CN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo mj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    133,            /* lineNo */
    8,              /* colNo */
    "QN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo nj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    137,            /* lineNo */
    21,             /* colNo */
    "openClasses",  /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo oj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    139,            /* lineNo */
    20,             /* colNo */
    "lambda",       /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo pj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    139,            /* lineNo */
    8,              /* colNo */
    "XN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo qj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    142,            /* lineNo */
    12,             /* colNo */
    "QN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo rj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    142,            /* lineNo */
    16,             /* colNo */
    "QN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo sj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    147,            /* lineNo */
    24,             /* colNo */
    "QN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo tj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    147,            /* lineNo */
    28,             /* colNo */
    "QN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo uj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    147,            /* lineNo */
    40,             /* colNo */
    "lambda",       /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo vj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    147,            /* lineNo */
    12,             /* colNo */
    "CN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo wj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    147,            /* lineNo */
    16,             /* colNo */
    "CN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo xj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    144,            /* lineNo */
    28,             /* colNo */
    "QN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo yj_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    144,            /* lineNo */
    32,             /* colNo */
    "QN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ak_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    144,            /* lineNo */
    44,             /* colNo */
    "lambda",       /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo bk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    144,            /* lineNo */
    51,             /* colNo */
    "D",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ck_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    144,            /* lineNo */
    55,             /* colNo */
    "D",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo dk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    144,            /* lineNo */
    71,             /* colNo */
    "EC",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ek_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    144,            /* lineNo */
    75,             /* colNo */
    "EC",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo fk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    144,            /* lineNo */
    85,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo gk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    144,            /* lineNo */
    90,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo hk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    144,            /* lineNo */
    95,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ik_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    144,            /* lineNo */
    16,             /* colNo */
    "QN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo jk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    144,            /* lineNo */
    20,             /* colNo */
    "QN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo kk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    150,            /* lineNo */
    12,             /* colNo */
    "UN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo lk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    150,            /* lineNo */
    16,             /* colNo */
    "UN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo mk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    152,            /* lineNo */
    28,             /* colNo */
    "UN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo nk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    152,            /* lineNo */
    32,             /* colNo */
    "UN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo ok_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    152,            /* lineNo */
    44,             /* colNo */
    "lambda",       /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo pk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    152,            /* lineNo */
    56,             /* colNo */
    "Eprime",       /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo qk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    152,            /* lineNo */
    60,             /* colNo */
    "Eprime",       /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo rk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    152,            /* lineNo */
    71,             /* colNo */
    "E",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo sk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    152,            /* lineNo */
    75,             /* colNo */
    "E",            /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo tk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    152,            /* lineNo */
    87,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo uk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    152,            /* lineNo */
    92,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo vk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    152,            /* lineNo */
    97,             /* colNo */
    "Pc",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo wk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    152,            /* lineNo */
    16,             /* colNo */
    "UN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtBCInfo xk_emlrtBCI = {
    -1,             /* iFirst */
    -1,             /* iLast */
    152,            /* lineNo */
    20,             /* colNo */
    "UN",           /* aName */
    "pfqn_mvaldmx", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx.m", /* pName */
    0                 /* checkKind */
};

static emlrtRTEInfo xd_emlrtRTEI =
    {
        33,             /* lineNo */
        23,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo yd_emlrtRTEI =
    {
        33,             /* lineNo */
        21,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo ae_emlrtRTEI =
    {
        36,             /* lineNo */
        8,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo be_emlrtRTEI = {
    14,         /* lineNo */
    1,          /* colNo */
    "isfinite", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/isfinite.m" /* pName
                                                                        */
};

static emlrtRTEInfo ce_emlrtRTEI =
    {
        40,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo de_emlrtRTEI =
    {
        43,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo ee_emlrtRTEI =
    {
        44,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo fe_emlrtRTEI =
    {
        45,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo ge_emlrtRTEI =
    {
        46,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo he_emlrtRTEI =
    {
        48,             /* lineNo */
        11,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo ie_emlrtRTEI = {
    247,   /* lineNo */
    14,    /* colNo */
    "cat", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/cat.m" /* pName
                                                                       */
};

static emlrtRTEInfo je_emlrtRTEI =
    {
        51,             /* lineNo */
        10,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo ke_emlrtRTEI =
    {
        52,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo le_emlrtRTEI =
    {
        53,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo me_emlrtRTEI =
    {
        54,             /* lineNo */
        9,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo ne_emlrtRTEI =
    {
        56,             /* lineNo */
        19,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo oe_emlrtRTEI =
    {
        59,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo pe_emlrtRTEI =
    {
        61,             /* lineNo */
        29,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo qe_emlrtRTEI =
    {
        61,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo re_emlrtRTEI =
    {
        62,             /* lineNo */
        18,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo se_emlrtRTEI =
    {
        62,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo te_emlrtRTEI =
    {
        63,             /* lineNo */
        20,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo ue_emlrtRTEI =
    {
        63,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo ve_emlrtRTEI =
    {
        67,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo we_emlrtRTEI =
    {
        69,             /* lineNo */
        7,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo ye_emlrtRTEI =
    {
        127,            /* lineNo */
        4,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo af_emlrtRTEI =
    {
        85,             /* lineNo */
        43,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo bf_emlrtRTEI =
    {
        100,            /* lineNo */
        25,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo cf_emlrtRTEI =
    {
        103,            /* lineNo */
        16,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo df_emlrtRTEI =
    {
        96,             /* lineNo */
        47,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo ef_emlrtRTEI =
    {
        103,            /* lineNo */
        43,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo ff_emlrtRTEI =
    {
        129,            /* lineNo */
        8,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo gf_emlrtRTEI =
    {
        103,            /* lineNo */
        68,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo hf_emlrtRTEI =
    {
        129,            /* lineNo */
        25,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo if_emlrtRTEI =
    {
        131,            /* lineNo */
        8,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo jf_emlrtRTEI =
    {
        131,            /* lineNo */
        25,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo kf_emlrtRTEI =
    {
        133,            /* lineNo */
        32,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo lf_emlrtRTEI =
    {
        133,            /* lineNo */
        58,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo mf_emlrtRTEI =
    {
        133,            /* lineNo */
        8,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo nf_emlrtRTEI =
    {
        41,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo of_emlrtRTEI =
    {
        54,             /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo pf_emlrtRTEI =
    {
        26,             /* lineNo */
        33,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo qf_emlrtRTEI =
    {
        26,             /* lineNo */
        27,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo rf_emlrtRTEI =
    {
        129,            /* lineNo */
        1,              /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

static emlrtRTEInfo sf_emlrtRTEI = {
    14,         /* lineNo */
    6,          /* colNo */
    "isfinite", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/isfinite.m" /* pName
                                                                        */
};

static emlrtRTEInfo gi_emlrtRTEI =
    {
        133,            /* lineNo */
        25,             /* colNo */
        "pfqn_mvaldmx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvaldmx.m" /* pName */
};

/* Function Declarations */
static void times(const emlrtStack *sp, emxArray_real_T *in1,
                  const emxArray_real_T *in2);

/* Function Definitions */
static void times(const emlrtStack *sp, emxArray_real_T *in1,
                  const emxArray_real_T *in2)
{
  emxArray_real_T *b_in1;
  const real_T *in2_data;
  real_T *b_in1_data;
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
  emxInit_real_T(sp, &b_in1, 2, &gi_emlrtRTEI);
  if (in2->size[0] == 1) {
    loop_ub = in1->size[0];
  } else {
    loop_ub = in2->size[0];
  }
  stride_0_0 = b_in1->size[0] * b_in1->size[1];
  b_in1->size[0] = loop_ub;
  if (in2->size[1] == 1) {
    b_loop_ub = in1->size[1];
  } else {
    b_loop_ub = in2->size[1];
  }
  b_in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(sp, b_in1, stride_0_0, &gi_emlrtRTEI);
  b_in1_data = b_in1->data;
  stride_0_0 = (in1->size[0] != 1);
  stride_0_1 = (in1->size[1] != 1);
  stride_1_0 = (in2->size[0] != 1);
  stride_1_1 = (in2->size[1] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_in1_data[i1 + b_in1->size[0] * i] =
          in1_data[i1 * stride_0_0 + in1->size[0] * aux_0_1] *
          in2_data[i1 * stride_1_0 + in2->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_1;
    aux_0_1 += stride_0_1;
  }
  stride_0_0 = in1->size[0] * in1->size[1];
  in1->size[0] = loop_ub;
  in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_0, &gi_emlrtRTEI);
  in1_data = in1->data;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      in1_data[i1 + in1->size[0] * i] = b_in1_data[i1 + b_in1->size[0] * i];
    }
  }
  emxFree_real_T(sp, &b_in1);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

real_T pfqn_mvaldmx(const emlrtStack *sp, const emxArray_real_T *lambda,
                    const emxArray_real_T *D, const emxArray_real_T *N,
                    const emxArray_real_T *Z, const emxArray_real_T *mu,
                    emxArray_real_T *XN, emxArray_real_T *QN,
                    emxArray_real_T *UN, emxArray_real_T *CN)
{
  __m128d r5;
  __m128d r6;
  jmp_buf emlrtJBEnviron;
  jmp_buf *volatile emlrtJBStack;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack g_st;
  emlrtStack st;
  emxArray_boolean_T *b;
  emxArray_boolean_T *r;
  emxArray_boolean_T *r1;
  emxArray_boolean_T *r3;
  emxArray_int32_T *ia;
  emxArray_int32_T *ii;
  emxArray_real_T *E;
  emxArray_real_T *EC;
  emxArray_real_T *Eprime;
  emxArray_real_T *Nc;
  emxArray_real_T *Pc;
  emxArray_real_T *Zc;
  emxArray_real_T *b_N;
  emxArray_real_T *b_Pc;
  emxArray_real_T *b_mu;
  emxArray_real_T *b_w;
  emxArray_real_T *closedClasses;
  emxArray_real_T *nvec;
  emxArray_real_T *openClasses;
  emxArray_real_T *prods;
  emxArray_real_T *u;
  emxArray_real_T *w;
  emxArray_real_T *x;
  real_T dv[2];
  const real_T *D_data;
  const real_T *N_data;
  const real_T *Z_data;
  const real_T *lambda_data;
  const real_T *mu_data;
  real_T hnvec;
  real_T hnvec_c;
  real_T idx;
  real_T lGN;
  real_T nc;
  real_T *CN_data;
  real_T *EC_data;
  real_T *E_data;
  real_T *Eprime_data;
  real_T *Nc_data;
  real_T *Pc_data;
  real_T *QN_data;
  real_T *UN_data;
  real_T *XN_data;
  real_T *Zc_data;
  real_T *b_N_data;
  real_T *b_mu_data;
  real_T *b_w_data;
  real_T *closedClasses_data;
  real_T *nvec_data;
  real_T *openClasses_data;
  real_T *prods_data;
  real_T *u_data;
  real_T *w_data;
  real_T *x_data;
  int32_T b_result[2];
  int32_T input_sizes[2];
  int32_T C;
  int32_T M;
  int32_T b_i;
  int32_T b_ist;
  int32_T b_loop_ub;
  int32_T b_r;
  int32_T c_loop_ub;
  int32_T d_loop_ub;
  int32_T e_loop_ub;
  int32_T end;
  int32_T exitg1;
  int32_T f_loop_ub;
  int32_T g_loop_ub;
  int32_T i;
  int32_T i1;
  int32_T i10;
  int32_T i11;
  int32_T i2;
  int32_T i3;
  int32_T i4;
  int32_T i5;
  int32_T i6;
  int32_T i7;
  int32_T i8;
  int32_T i9;
  int32_T ii_data;
  int32_T ist;
  int32_T loop_ub;
  int32_T pfqn_mvaldmx_numThreads;
  int32_T result;
  int32_T trueCount;
  int32_T *b_ii_data;
  int32_T *ia_data;
  int8_T sizes_idx_1;
  boolean_T b_b;
  boolean_T emlrtHadParallelError = false;
  boolean_T empty_non_axis_sizes;
  boolean_T *b_data;
  boolean_T *r2;
  boolean_T *r4;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  mu_data = mu->data;
  Z_data = Z->data;
  N_data = N->data;
  D_data = D->data;
  lambda_data = lambda->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /* { */
  /*  % @file pfqn_mvaldmx.m */
  /*  % @brief Load-dependent MVA for mixed open/closed networks with limited
   * load dependence. */
  /* } */
  /* { */
  /*  % @brief Load-dependent MVA for mixed open/closed networks with limited
   * load dependence. */
  /*  % @fn pfqn_mvaldmx(lambda, D, N, Z, mu, S) */
  /*  % @param lambda Arrival rate vector. */
  /*  % @param D Service demand matrix. */
  /*  % @param N Population vector. */
  /*  % @param Z Think time vector. */
  /*  % @param mu Load-dependent rate matrix. */
  /*  % @param S Number of servers per station. */
  /*  % @return XN System throughput. */
  /*  % @return QN Mean queue lengths. */
  /*  % @return UN Utilization. */
  /*  % @return CN Cycle times. */
  /*  % @return lGN Logarithm of normalizing constant. */
  /*  % @return Pc Marginal queue-length probabilities. */
  /* } */
  /*  [XN,QN,UN,CN,lGN,Pc] = PFQN_MVALDMX(LAMBDA,D,N,Z,MU,S) */
  emxInit_boolean_T(sp, &b, &sf_emlrtRTEI);
  loop_ub = b->size[0] * b->size[1];
  b->size[0] = 1;
  ii_data = N->size[1];
  b->size[1] = N->size[1];
  emxEnsureCapacity_boolean_T(sp, b, loop_ub, &vb_emlrtRTEI);
  b_data = b->data;
  loop_ub = N->size[1];
  if (N->size[1] < 1600) {
    for (i = 0; i < ii_data; i++) {
      b_data[i] = muDoubleScalarIsInf(N_data[i]);
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    pfqn_mvaldmx_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mvaldmx_numThreads)

    for (i = 0; i < loop_ub; i++) {
      b_data[i] = muDoubleScalarIsInf(N_data[i]);
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  trueCount = 0;
  for (b_i = 0; b_i < ii_data; b_i++) {
    if ((!b_data[b_i]) && (!muDoubleScalarIsNaN(N_data[b_i]))) {
      trueCount++;
    }
  }
  emxInit_int32_T(sp, &ii, 2, &wc_emlrtRTEI);
  loop_ub = ii->size[0] * ii->size[1];
  ii->size[0] = 1;
  ii->size[1] = trueCount;
  emxEnsureCapacity_int32_T(sp, ii, loop_ub, &xd_emlrtRTEI);
  b_ii_data = ii->data;
  loop_ub = 0;
  for (b_i = 0; b_i < ii_data; b_i++) {
    if ((!b_data[b_i]) && (!muDoubleScalarIsNaN(N_data[b_i]))) {
      b_ii_data[loop_ub] = b_i;
      loop_ub++;
    }
  }
  emxInit_real_T(sp, &b_N, 2, &yd_emlrtRTEI);
  loop_ub = b_N->size[0] * b_N->size[1];
  b_N->size[0] = 1;
  trueCount = ii->size[1];
  b_N->size[1] = ii->size[1];
  emxEnsureCapacity_real_T(sp, b_N, loop_ub, &yd_emlrtRTEI);
  b_N_data = b_N->data;
  for (b_i = 0; b_i < trueCount; b_i++) {
    if (b_ii_data[b_i] > N->size[1] - 1) {
      emlrtDynamicBoundsCheckR2012b(b_ii_data[b_i], 0, N->size[1] - 1,
                                    &bh_emlrtBCI, (emlrtConstCTX)sp);
    }
    b_N_data[b_i] = N_data[b_ii_data[b_i]];
  }
  st.site = &bd_emlrtRSI;
  if (mu->size[1] < b_sum(&st, b_N)) {
    st.site = &cd_emlrtRSI;
    /* LINE_ERROR Display a plain-text error message with file and line info. */
    /*  */
    /*    LINE_ERROR(CALLER, MSG, ...) throws an error with CALLER's name and */
    /*    message, including the source file and line number, in plain text (no
     */
    /*    hyperlink). Extra arguments are passed to sprintf to format MSG. */
    /*    Copyright (c) 2012-2026, Imperial College London */
    /*    All rights reserved. */
    b_st.site = &s_emlrtRSI;
    c_error(&b_st);
  }
  end = lambda->size[1];
  trueCount = 0;
  for (b_i = 0; b_i < end; b_i++) {
    if (lambda_data[b_i] != 0.0) {
      trueCount++;
    }
  }
  emxInit_boolean_T(sp, &r, &ae_emlrtRTEI);
  loop_ub = r->size[0] * r->size[1];
  r->size[0] = 1;
  r->size[1] = trueCount;
  emxEnsureCapacity_boolean_T(sp, r, loop_ub, &ae_emlrtRTEI);
  b_data = r->data;
  loop_ub = 0;
  for (b_i = 0; b_i < end; b_i++) {
    if (lambda_data[b_i] != 0.0) {
      if (b_i > N->size[1] - 1) {
        emlrtDynamicBoundsCheckR2012b(b_i, 0, N->size[1] - 1, &ch_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      b_data[loop_ub] = (N_data[b_i] > 0.0);
      loop_ub++;
    }
  }
  for (b_i = 0; b_i < end; b_i++) {
    if ((lambda_data[b_i] != 0.0) && (b_i > N->size[1] - 1)) {
      emlrtDynamicBoundsCheckR2012b(b_i, 0, N->size[1] - 1, &ah_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
  }
  trueCount = 0;
  for (b_i = 0; b_i < end; b_i++) {
    if (lambda_data[b_i] != 0.0) {
      trueCount++;
    }
  }
  emxInit_boolean_T(sp, &r1, &ae_emlrtRTEI);
  loop_ub = r1->size[0] * r1->size[1];
  r1->size[0] = 1;
  r1->size[1] = trueCount;
  emxEnsureCapacity_boolean_T(sp, r1, loop_ub, &be_emlrtRTEI);
  r2 = r1->data;
  loop_ub = 0;
  for (b_i = 0; b_i < end; b_i++) {
    if (lambda_data[b_i] != 0.0) {
      idx = N_data[b_i];
      r2[loop_ub] =
          ((!muDoubleScalarIsInf(idx)) && (!muDoubleScalarIsNaN(idx)));
      loop_ub++;
    }
  }
  if (r->size[1] != r1->size[1]) {
    emlrtSizeEqCheckNDErrorR2021b(&r->size[0], &r1->size[0], &g_emlrtECI,
                                  (emlrtCTX)sp);
  }
  emxInit_boolean_T(sp, &r3, &ae_emlrtRTEI);
  loop_ub = r3->size[0] * r3->size[1];
  r3->size[0] = 1;
  trueCount = r->size[1];
  r3->size[1] = r->size[1];
  emxEnsureCapacity_boolean_T(sp, r3, loop_ub, &ae_emlrtRTEI);
  r4 = r3->data;
  loop_ub = r->size[1];
  if (r->size[1] < 1600) {
    for (i1 = 0; i1 < trueCount; i1++) {
      r4[i1] = (b_data[i1] && r2[i1]);
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    pfqn_mvaldmx_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mvaldmx_numThreads)

    for (i1 = 0; i1 < loop_ub; i1++) {
      r4[i1] = (b_data[i1] && r2[i1]);
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  emxFree_boolean_T(sp, &r1);
  emxFree_boolean_T(sp, &r);
  st.site = &dd_emlrtRSI;
  if (any(&st, r3)) {
    st.site = &ed_emlrtRSI;
    /* LINE_ERROR Display a plain-text error message with file and line info. */
    /*  */
    /*    LINE_ERROR(CALLER, MSG, ...) throws an error with CALLER's name and */
    /*    message, including the source file and line number, in plain text (no
     */
    /*    hyperlink). Extra arguments are passed to sprintf to format MSG. */
    /*    Copyright (c) 2012-2026, Imperial College London */
    /*    All rights reserved. */
    b_st.site = &s_emlrtRSI;
    d_error(&b_st);
  }
  emxFree_boolean_T(sp, &r3);
  M = D->size[0];
  st.site = &fd_emlrtRSI;
  b_st.site = &ge_emlrtRSI;
  eml_find(&b_st, b, ii);
  b_ii_data = ii->data;
  emxInit_real_T(&st, &openClasses, 2, &ce_emlrtRTEI);
  loop_ub = openClasses->size[0] * openClasses->size[1];
  openClasses->size[0] = 1;
  b_loop_ub = ii->size[1];
  openClasses->size[1] = ii->size[1];
  emxEnsureCapacity_real_T(&st, openClasses, loop_ub, &ce_emlrtRTEI);
  openClasses_data = openClasses->data;
  for (b_i = 0; b_i < b_loop_ub; b_i++) {
    openClasses_data[b_i] = b_ii_data[b_i];
  }
  if (N->size[1] < 1) {
    b_N->size[0] = 1;
    b_N->size[1] = 0;
  } else {
    loop_ub = b_N->size[0] * b_N->size[1];
    b_N->size[0] = 1;
    b_N->size[1] = ii_data;
    emxEnsureCapacity_real_T(sp, b_N, loop_ub, &gb_emlrtRTEI);
    b_N_data = b_N->data;
    loop_ub = N->size[1] - 1;
    trueCount = (N->size[1] / 2) << 1;
    result = trueCount - 2;
    for (b_i = 0; b_i <= result; b_i += 2) {
      dv[0] = b_i;
      dv[1] = b_i + 1;
      r5 = _mm_loadu_pd(&dv[0]);
      _mm_storeu_pd(&b_N_data[b_i], _mm_add_pd(_mm_set1_pd(1.0), r5));
    }
    for (b_i = trueCount; b_i <= loop_ub; b_i++) {
      b_N_data[b_i] = (real_T)b_i + 1.0;
    }
  }
  st.site = &gd_emlrtRSI;
  b_st.site = &je_emlrtRSI;
  emxInit_real_T(&b_st, &closedClasses, 2, &nf_emlrtRTEI);
  emxInit_int32_T(&b_st, &ia, 1, &rf_emlrtRTEI);
  c_st.site = &ke_emlrtRSI;
  do_vectors(&c_st, b_N, openClasses, closedClasses, ia);
  closedClasses_data = closedClasses->data;
  loop_ub = XN->size[0] * XN->size[1];
  XN->size[0] = 1;
  c_loop_ub = D->size[1];
  XN->size[1] = D->size[1];
  emxEnsureCapacity_real_T(sp, XN, loop_ub, &de_emlrtRTEI);
  XN_data = XN->data;
  for (b_i = 0; b_i < c_loop_ub; b_i++) {
    XN_data[b_i] = 0.0;
  }
  loop_ub = UN->size[0] * UN->size[1];
  UN->size[0] = D->size[0];
  UN->size[1] = D->size[1];
  emxEnsureCapacity_real_T(sp, UN, loop_ub, &ee_emlrtRTEI);
  UN_data = UN->data;
  trueCount = D->size[0] * D->size[1];
  for (b_i = 0; b_i < trueCount; b_i++) {
    UN_data[b_i] = 0.0;
  }
  loop_ub = CN->size[0] * CN->size[1];
  CN->size[0] = D->size[0];
  CN->size[1] = D->size[1];
  emxEnsureCapacity_real_T(sp, CN, loop_ub, &fe_emlrtRTEI);
  CN_data = CN->data;
  for (b_i = 0; b_i < trueCount; b_i++) {
    CN_data[b_i] = 0.0;
  }
  loop_ub = QN->size[0] * QN->size[1];
  QN->size[0] = D->size[0];
  QN->size[1] = D->size[1];
  emxEnsureCapacity_real_T(sp, QN, loop_ub, &ge_emlrtRTEI);
  QN_data = QN->data;
  for (b_i = 0; b_i < trueCount; b_i++) {
    QN_data[b_i] = 0.0;
  }
  lGN = 0.0;
  st.site = &hd_emlrtRSI;
  if (mu->size[1] < 1) {
    emlrtDynamicBoundsCheckR2012b(mu->size[1], 1, mu->size[1], &if_emlrtBCI,
                                  &st);
  }
  b_st.site = &pe_emlrtRSI;
  b_b = ((mu->size[0] != 0) && (mu->size[1] != 0));
  if (b_b) {
    result = mu->size[0];
  } else if (mu->size[0] != 0) {
    result = mu->size[0];
  } else {
    result = 0;
  }
  c_st.site = &qe_emlrtRSI;
  if ((mu->size[0] != result) && ((mu->size[0] != 0) && (mu->size[1] != 0))) {
    emlrtErrorWithMessageIdR2018a(&c_st, &p_emlrtRTEI,
                                  "MATLAB:catenate:matrixDimensionMismatch",
                                  "MATLAB:catenate:matrixDimensionMismatch", 0);
  }
  if ((mu->size[0] != result) && (mu->size[0] != 0)) {
    emlrtErrorWithMessageIdR2018a(&c_st, &p_emlrtRTEI,
                                  "MATLAB:catenate:matrixDimensionMismatch",
                                  "MATLAB:catenate:matrixDimensionMismatch", 0);
  }
  empty_non_axis_sizes = (result == 0);
  if (empty_non_axis_sizes || b_b) {
    input_sizes[1] = mu->size[1];
  } else {
    input_sizes[1] = 0;
  }
  if (empty_non_axis_sizes || (mu->size[0] != 0)) {
    sizes_idx_1 = 1;
  } else {
    sizes_idx_1 = 0;
  }
  /*  we need up to sum(N)+1, but there is limited load dep */
  b_result[1] = input_sizes[1];
  emxInit_real_T(sp, &b_mu, 1, &he_emlrtRTEI);
  trueCount = mu->size[0];
  loop_ub = b_mu->size[0];
  b_mu->size[0] = mu->size[0];
  emxEnsureCapacity_real_T(sp, b_mu, loop_ub, &he_emlrtRTEI);
  b_mu_data = b_mu->data;
  for (b_i = 0; b_i < trueCount; b_i++) {
    b_mu_data[b_i] = mu_data[b_i + mu->size[0] * (mu->size[1] - 1)];
  }
  emxInit_real_T(sp, &x, 2, &se_emlrtRTEI);
  loop_ub = x->size[0] * x->size[1];
  x->size[0] = result;
  x->size[1] = b_result[1] + sizes_idx_1;
  emxEnsureCapacity_real_T(sp, x, loop_ub, &ie_emlrtRTEI);
  x_data = x->data;
  loop_ub = b_result[1];
  for (b_i = 0; b_i < loop_ub; b_i++) {
    for (b_r = 0; b_r < result; b_r++) {
      x_data[b_r + x->size[0] * b_i] = mu_data[b_r + result * b_i];
    }
  }
  loop_ub = sizes_idx_1;
  for (b_i = 0; b_i < loop_ub; b_i++) {
    for (b_r = 0; b_r < result; b_r++) {
      x_data[b_r + x->size[0] * b_result[1]] = b_mu_data[b_r];
    }
  }
  emxInit_real_T(sp, &EC, 2, &pf_emlrtRTEI);
  emxInit_real_T(sp, &E, 2, &pf_emlrtRTEI);
  emxInit_real_T(sp, &Eprime, 2, &pf_emlrtRTEI);
  st.site = &id_emlrtRSI;
  pfqn_mvaldmx_ec(&st, lambda, D, x, EC, E, Eprime);
  Eprime_data = Eprime->data;
  E_data = E->data;
  EC_data = EC->data;
  C = closedClasses->size[1];
  /*  number of closed classes */
  loop_ub = b_mu->size[0];
  b_mu->size[0] = closedClasses->size[1];
  emxEnsureCapacity_real_T(sp, b_mu, loop_ub, &je_emlrtRTEI);
  b_mu_data = b_mu->data;
  for (b_i = 0; b_i < C; b_i++) {
    b_mu_data[b_i] = closedClasses_data[b_i];
  }
  for (b_i = 0; b_i < C; b_i++) {
    loop_ub = (int32_T)b_mu_data[b_i];
    if ((loop_ub < 1) || (loop_ub > c_loop_ub)) {
      emlrtDynamicBoundsCheckR2012b(loop_ub, 1, c_loop_ub, &dh_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
  }
  emxInit_real_T(sp, &Nc, 2, &ke_emlrtRTEI);
  loop_ub = Nc->size[0] * Nc->size[1];
  Nc->size[0] = 1;
  Nc->size[1] = closedClasses->size[1];
  emxEnsureCapacity_real_T(sp, Nc, loop_ub, &ke_emlrtRTEI);
  Nc_data = Nc->data;
  for (b_i = 0; b_i < C; b_i++) {
    loop_ub = (int32_T)closedClasses_data[b_i];
    if ((loop_ub < 1) || (loop_ub > ii_data)) {
      emlrtDynamicBoundsCheckR2012b(loop_ub, 1, ii_data, &eh_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    Nc_data[b_i] = N_data[loop_ub - 1];
  }
  emxInit_real_T(sp, &Zc, 2, &le_emlrtRTEI);
  loop_ub = Zc->size[0] * Zc->size[1];
  Zc->size[0] = 1;
  Zc->size[1] = closedClasses->size[1];
  emxEnsureCapacity_real_T(sp, Zc, loop_ub, &le_emlrtRTEI);
  Zc_data = Zc->data;
  for (b_i = 0; b_i < C; b_i++) {
    loop_ub = (int32_T)closedClasses_data[b_i];
    if ((loop_ub < 1) || (loop_ub > Z->size[1])) {
      emlrtDynamicBoundsCheckR2012b(loop_ub, 1, Z->size[1], &fh_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    Zc_data[b_i] = Z_data[loop_ub - 1];
  }
  emxInit_real_T(sp, &prods, 2, &of_emlrtRTEI);
  loop_ub = prods->size[0] * prods->size[1];
  prods->size[0] = 1;
  prods->size[1] = closedClasses->size[1];
  emxEnsureCapacity_real_T(sp, prods, loop_ub, &me_emlrtRTEI);
  prods_data = prods->data;
  /*  needed for fast hashing */
  for (b_r = 0; b_r < C; b_r++) {
    if (b_r < 1) {
      result = 0;
    } else {
      if (C < 1) {
        emlrtDynamicBoundsCheckR2012b(1, 1, C, &jf_emlrtBCI, (emlrtConstCTX)sp);
      }
      if (b_r > C) {
        emlrtDynamicBoundsCheckR2012b(b_r, 1, C, &kf_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      result = b_r;
    }
    loop_ub = b_N->size[0] * b_N->size[1];
    b_N->size[0] = 1;
    b_N->size[1] = result;
    emxEnsureCapacity_real_T(sp, b_N, loop_ub, &ne_emlrtRTEI);
    b_N_data = b_N->data;
    loop_ub = (result / 2) << 1;
    trueCount = loop_ub - 2;
    for (b_i = 0; b_i <= trueCount; b_i += 2) {
      r5 = _mm_loadu_pd(&Nc_data[b_i]);
      _mm_storeu_pd(&b_N_data[b_i], _mm_add_pd(r5, _mm_set1_pd(1.0)));
    }
    for (b_i = loop_ub; b_i < result; b_i++) {
      b_N_data[b_i] = Nc_data[b_i] + 1.0;
    }
    if (b_r + 1 > prods->size[1]) {
      emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, prods->size[1], &gh_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    st.site = &jd_emlrtRSI;
    prods_data[b_r] = prod(&st, b_N);
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  /*  Start at nc=(0,...,0) */
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
  emxInit_real_T(sp, &nvec, 2, &oe_emlrtRTEI);
  loop_ub = nvec->size[0] * nvec->size[1];
  nvec->size[0] = 1;
  nvec->size[1] = closedClasses->size[1];
  emxEnsureCapacity_real_T(sp, nvec, loop_ub, &oe_emlrtRTEI);
  nvec_data = nvec->data;
  for (b_i = 0; b_i < C; b_i++) {
    nvec_data[b_i] = 0.0;
  }
  /*  Initialize Pc */
  st.site = &kd_emlrtRSI;
  idx = 1.0 + b_sum(&st, Nc);
  if (!(idx >= 0.0)) {
    emlrtNonNegativeCheckR2012b(idx, &k_emlrtDCI, (emlrtConstCTX)sp);
  }
  if (idx != (int32_T)muDoubleScalarFloor(idx)) {
    emlrtIntegerCheckR2012b(idx, &l_emlrtDCI, (emlrtConstCTX)sp);
  }
  loop_ub = b_N->size[0] * b_N->size[1];
  b_N->size[0] = 1;
  d_loop_ub = Nc->size[1];
  b_N->size[1] = Nc->size[1];
  emxEnsureCapacity_real_T(sp, b_N, loop_ub, &pe_emlrtRTEI);
  b_N_data = b_N->data;
  trueCount = (Nc->size[1] / 2) << 1;
  loop_ub = trueCount - 2;
  for (b_i = 0; b_i <= loop_ub; b_i += 2) {
    r5 = _mm_loadu_pd(&Nc_data[b_i]);
    _mm_storeu_pd(&b_N_data[b_i], _mm_add_pd(r5, _mm_set1_pd(1.0)));
  }
  for (b_i = trueCount; b_i < d_loop_ub; b_i++) {
    b_N_data[b_i] = Nc_data[b_i] + 1.0;
  }
  st.site = &kd_emlrtRSI;
  hnvec_c = prod(&st, b_N);
  if (!(hnvec_c >= 0.0)) {
    emlrtNonNegativeCheckR2012b(hnvec_c, &m_emlrtDCI, (emlrtConstCTX)sp);
  }
  if (hnvec_c != (int32_T)muDoubleScalarFloor(hnvec_c)) {
    emlrtIntegerCheckR2012b(hnvec_c, &n_emlrtDCI, (emlrtConstCTX)sp);
  }
  emxInit_real_T(sp, &Pc, 3, &qf_emlrtRTEI);
  loop_ub = Pc->size[0] * Pc->size[1] * Pc->size[2];
  Pc->size[0] = D->size[0];
  Pc->size[1] = (int32_T)idx;
  Pc->size[2] = (int32_T)hnvec_c;
  emxEnsureCapacity_real_T(sp, Pc, loop_ub, &qe_emlrtRTEI);
  Pc_data = Pc->data;
  loop_ub = D->size[0] * (int32_T)idx * (int32_T)hnvec_c;
  for (b_i = 0; b_i < loop_ub; b_i++) {
    Pc_data[b_i] = 0.0;
  }
  loop_ub = b_N->size[0] * b_N->size[1];
  b_N->size[0] = 1;
  b_N->size[1] = Nc->size[1];
  emxEnsureCapacity_real_T(sp, b_N, loop_ub, &re_emlrtRTEI);
  b_N_data = b_N->data;
  loop_ub = trueCount - 2;
  for (b_i = 0; b_i <= loop_ub; b_i += 2) {
    r5 = _mm_loadu_pd(&Nc_data[b_i]);
    _mm_storeu_pd(&b_N_data[b_i], _mm_add_pd(r5, _mm_set1_pd(1.0)));
  }
  for (b_i = trueCount; b_i < d_loop_ub; b_i++) {
    b_N_data[b_i] = Nc_data[b_i] + 1.0;
  }
  st.site = &ld_emlrtRSI;
  idx = prod(&st, b_N);
  if (!(idx >= 0.0)) {
    emlrtNonNegativeCheckR2012b(idx, &o_emlrtDCI, (emlrtConstCTX)sp);
  }
  if (idx != (int32_T)muDoubleScalarFloor(idx)) {
    emlrtIntegerCheckR2012b(idx, &p_emlrtDCI, (emlrtConstCTX)sp);
  }
  loop_ub = x->size[0] * x->size[1];
  x->size[0] = closedClasses->size[1];
  x->size[1] = (int32_T)idx;
  emxEnsureCapacity_real_T(sp, x, loop_ub, &se_emlrtRTEI);
  x_data = x->data;
  loop_ub = closedClasses->size[1] * (int32_T)idx;
  for (b_i = 0; b_i < loop_ub; b_i++) {
    x_data[b_i] = 0.0;
  }
  loop_ub = b_N->size[0] * b_N->size[1];
  b_N->size[0] = 1;
  b_N->size[1] = Nc->size[1];
  emxEnsureCapacity_real_T(sp, b_N, loop_ub, &te_emlrtRTEI);
  b_N_data = b_N->data;
  loop_ub = trueCount - 2;
  for (b_i = 0; b_i <= loop_ub; b_i += 2) {
    r5 = _mm_loadu_pd(&Nc_data[b_i]);
    _mm_storeu_pd(&b_N_data[b_i], _mm_add_pd(r5, _mm_set1_pd(1.0)));
  }
  for (b_i = trueCount; b_i < d_loop_ub; b_i++) {
    b_N_data[b_i] = Nc_data[b_i] + 1.0;
  }
  st.site = &md_emlrtRSI;
  idx = prod(&st, b_N);
  if (!(idx >= 0.0)) {
    emlrtNonNegativeCheckR2012b(idx, &q_emlrtDCI, (emlrtConstCTX)sp);
  }
  if (idx != (int32_T)muDoubleScalarFloor(idx)) {
    emlrtIntegerCheckR2012b(idx, &r_emlrtDCI, (emlrtConstCTX)sp);
  }
  emxInit_real_T(sp, &w, 3, &ue_emlrtRTEI);
  loop_ub = w->size[0] * w->size[1] * w->size[2];
  w->size[0] = D->size[0];
  w->size[1] = closedClasses->size[1];
  w->size[2] = (int32_T)idx;
  emxEnsureCapacity_real_T(sp, w, loop_ub, &ue_emlrtRTEI);
  w_data = w->data;
  trueCount = D->size[0] * closedClasses->size[1];
  loop_ub = trueCount * (int32_T)idx;
  for (b_i = 0; b_i < loop_ub; b_i++) {
    w_data[b_i] = 0.0;
  }
  for (b_i = 0; b_i < M; b_i++) {
    if (Pc->size[1] < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, Pc->size[1], &nf_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    st.site = &rd_emlrtRSI;
    /*  idx=HASHPOP(n,N) */
    /*  idx=HASHPOP(n,N,R,prods) where prods(r)=prod(N(1:r-1)+1) (faster) */
    /*  Hash a vector n on an integer lattice defined by vector N */
    /*  */
    /*  Copyright (c) 2012-2026, Imperial College London */
    /*  All rights reserved. */
    idx = 1.0;
    for (b_r = 0; b_r < C; b_r++) {
      if (b_r + 1 > C) {
        emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, C, &yg_emlrtBCI, &st);
      }
      if (b_r + 1 > prods->size[1]) {
        emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, prods->size[1], &ih_emlrtBCI,
                                      &st);
      }
      idx += prods_data[b_r] * 0.0;
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b(&st);
      }
    }
    if (idx != 1.0) {
      emlrtIntegerCheckR2012b(rtNaN, &s_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (Pc->size[2] < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, Pc->size[2], &of_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (b_i + 1 > Pc->size[0]) {
      emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, Pc->size[0], &hh_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    Pc_data[b_i] = 1.0;
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  emxInit_real_T(sp, &u, 2, &ve_emlrtRTEI);
  loop_ub = u->size[0] * u->size[1];
  u->size[0] = D->size[0];
  u->size[1] = closedClasses->size[1];
  emxEnsureCapacity_real_T(sp, u, loop_ub, &ve_emlrtRTEI);
  u_data = u->data;
  for (b_i = 0; b_i < trueCount; b_i++) {
    u_data[b_i] = 0.0;
  }
  /*  Population recursion */
  emxInit_real_T(sp, &b_w, 1, &af_emlrtRTEI);
  emxInit_real_T(sp, &b_Pc, 2, &df_emlrtRTEI);
  do {
    exitg1 = 0;
    loop_ub = b->size[0] * b->size[1];
    b->size[0] = 1;
    e_loop_ub = nvec->size[1];
    b->size[1] = nvec->size[1];
    emxEnsureCapacity_boolean_T(sp, b, loop_ub, &we_emlrtRTEI);
    b_data = b->data;
    for (b_i = 0; b_i < e_loop_ub; b_i++) {
      b_data[b_i] = (nvec_data[b_i] >= 0.0);
    }
    st.site = &nd_emlrtRSI;
    if (ifWhileCond(&st, b)) {
      st.site = &od_emlrtRSI;
      /*  idx=HASHPOP(n,N) */
      /*  idx=HASHPOP(n,N,R,prods) where prods(r)=prod(N(1:r-1)+1) (faster) */
      /*  Hash a vector n on an integer lattice defined by vector N */
      /*  */
      /*  Copyright (c) 2012-2026, Imperial College London */
      /*  All rights reserved. */
      hnvec = 1.0;
      for (b_i = 0; b_i < C; b_i++) {
        if (b_i + 1 > prods->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, prods->size[1],
                                        &ih_emlrtBCI, &st);
        }
        if (b_i + 1 > e_loop_ub) {
          emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, e_loop_ub, &yg_emlrtBCI,
                                        &st);
        }
        hnvec += prods_data[b_i] * nvec_data[b_i];
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b(&st);
        }
      }
      st.site = &pd_emlrtRSI;
      nc = b_sum(&st, nvec);
      for (ist = 0; ist < M; ist++) {
        for (b_r = 0; b_r < C; b_r++) {
          if (b_r + 1 > e_loop_ub) {
            emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, e_loop_ub, &jh_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          idx = nvec_data[b_r];
          if (idx > 0.0) {
            st.site = &qd_emlrtRSI;
            loop_ub = b_N->size[0] * b_N->size[1];
            b_N->size[0] = 1;
            b_N->size[1] = e_loop_ub;
            emxEnsureCapacity_real_T(&st, b_N, loop_ub, &xe_emlrtRTEI);
            b_N_data = b_N->data;
            for (b_i = 0; b_i < e_loop_ub; b_i++) {
              b_N_data[b_i] = nvec_data[b_i];
            }
            /*  N=ONER(N,r) */
            /*  Decrement element in position of r of input vector */
            /*  */
            /*  Copyright (c) 2012-2026, Imperial College London */
            /*  All rights reserved. */
            if (b_r + 1 > e_loop_ub) {
              emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, e_loop_ub, &ph_emlrtBCI,
                                            &st);
            }
            b_N_data[b_r] = idx - 1.0;
            st.site = &qd_emlrtRSI;
            /*  idx=HASHPOP(n,N) */
            /*  idx=HASHPOP(n,N,R,prods) where prods(r)=prod(N(1:r-1)+1)
             * (faster) */
            /*  Hash a vector n on an integer lattice defined by vector N */
            /*  */
            /*  Copyright (c) 2012-2026, Imperial College London */
            /*  All rights reserved. */
            hnvec_c = 1.0;
            for (b_i = 0; b_i < C; b_i++) {
              if (b_i + 1 > prods->size[1]) {
                emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, prods->size[1],
                                              &ih_emlrtBCI, &st);
              }
              if (b_i + 1 > b_N->size[1]) {
                emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, b_N->size[1],
                                              &yg_emlrtBCI, &st);
              }
              hnvec_c += prods_data[b_i] * b_N_data[b_i];
              if (*emlrtBreakCheckR2012bFlagVar != 0) {
                emlrtBreakCheckR2012b(&st);
              }
            }
            /*  Compute mean residence times */
            loop_ub = (int32_T)nc;
            emlrtForLoopVectorCheckR2021a(1.0, 1.0, nc, mxDOUBLE_CLASS,
                                          (int32_T)nc, &k_emlrtRTEI,
                                          (emlrtConstCTX)sp);
            for (b_i = 0; b_i < loop_ub; b_i++) {
              if (ist + 1 > M) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, M, &lf_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (b_r + 1 > C) {
                emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, C, &mf_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (ist + 1 > w->size[0]) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, w->size[0],
                                              &xh_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (b_r + 1 > w->size[1]) {
                emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, w->size[1],
                                              &yh_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (((int32_T)hnvec < 1) || ((int32_T)hnvec > w->size[2])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)hnvec, 1, w->size[2],
                                              &ai_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (ist + 1 > EC->size[0]) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, EC->size[0],
                                              &bi_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (((int32_T)((uint32_T)b_i + 1U) < 1) ||
                  ((int32_T)((uint32_T)b_i + 1U) > EC->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_i + 1U), 1,
                                              EC->size[1], &ci_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (ist + 1 > Pc->size[0]) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, Pc->size[0],
                                              &di_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (((int32_T)((uint32_T)b_i + 1U) < 1) ||
                  ((int32_T)((uint32_T)b_i + 1U) > Pc->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_i + 1U), 1,
                                              Pc->size[1], &ei_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (hnvec_c != (int32_T)muDoubleScalarFloor(hnvec_c)) {
                emlrtIntegerCheckR2012b(hnvec_c, &x_emlrtDCI,
                                        (emlrtConstCTX)sp);
              }
              if (((int32_T)hnvec_c < 1) || ((int32_T)hnvec_c > Pc->size[2])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)hnvec_c, 1, Pc->size[2],
                                              &fi_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (ist + 1 > w->size[0]) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, w->size[0],
                                              &gi_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (b_r + 1 > w->size[1]) {
                emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, w->size[1],
                                              &hi_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (hnvec != (int32_T)muDoubleScalarFloor(hnvec)) {
                emlrtIntegerCheckR2012b(hnvec, &y_emlrtDCI, (emlrtConstCTX)sp);
              }
              if (((int32_T)hnvec < 1) || ((int32_T)hnvec > w->size[2])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)hnvec, 1, w->size[2],
                                              &ii_emlrtBCI, (emlrtConstCTX)sp);
              }
              w_data[(ist + w->size[0] * b_r) +
                     w->size[0] * w->size[1] * ((int32_T)hnvec - 1)] +=
                  D_data[ist + D->size[0] * ((int32_T)b_mu_data[b_r] - 1)] *
                  ((real_T)b_i + 1.0) * EC_data[ist + EC->size[0] * b_i] *
                  Pc_data[(ist + Pc->size[0] * b_i) +
                          Pc->size[0] * Pc->size[1] * ((int32_T)hnvec_c - 1)];
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
      /*  Compute tput */
      if (C - 1 >= 0) {
        if (M < 1) {
          f_loop_ub = 0;
        } else {
          if (w->size[0] < 1) {
            emlrtDynamicBoundsCheckR2012b(1, 1, w->size[0], &pf_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (M > w->size[0]) {
            emlrtDynamicBoundsCheckR2012b(M, 1, w->size[0], &qf_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          f_loop_ub = M;
        }
      }
      for (b_r = 0; b_r < C; b_r++) {
        if (b_r + 1 > w->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, w->size[1], &rf_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (((int32_T)hnvec < 1) || ((int32_T)hnvec > w->size[2])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)hnvec, 1, w->size[2],
                                        &sf_emlrtBCI, (emlrtConstCTX)sp);
        }
        loop_ub = b_w->size[0];
        b_w->size[0] = f_loop_ub;
        emxEnsureCapacity_real_T(sp, b_w, loop_ub, &af_emlrtRTEI);
        b_w_data = b_w->data;
        for (b_i = 0; b_i < f_loop_ub; b_i++) {
          b_w_data[b_i] =
              w_data[(b_i + w->size[0] * b_r) +
                     w->size[0] * w->size[1] * ((int32_T)hnvec - 1)];
        }
        if (b_r + 1 > e_loop_ub) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, e_loop_ub, &lh_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_r + 1 > C) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, C, &mh_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_r + 1 > x->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, x->size[0], &nh_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (hnvec != (int32_T)muDoubleScalarFloor(hnvec)) {
          emlrtIntegerCheckR2012b(hnvec, &u_emlrtDCI, (emlrtConstCTX)sp);
        }
        if (((int32_T)hnvec < 1) || ((int32_T)hnvec > x->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)hnvec, 1, x->size[1],
                                        &oh_emlrtBCI, (emlrtConstCTX)sp);
        }
        st.site = &sd_emlrtRSI;
        x_data[b_r + x->size[0] * ((int32_T)hnvec - 1)] =
            nvec_data[b_r] / (Zc_data[b_r] + c_sum(&st, b_w));
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b((emlrtConstCTX)sp);
        }
      }
      for (b_ist = 0; b_ist < M; b_ist++) {
        trueCount = (int32_T)nc;
        emlrtForLoopVectorCheckR2021a(1.0, 1.0, nc, mxDOUBLE_CLASS, (int32_T)nc,
                                      &l_emlrtRTEI, (emlrtConstCTX)sp);
        for (b_r = 0; b_r < trueCount; b_r++) {
          for (ist = 0; ist < C; ist++) {
            if (ist + 1 > e_loop_ub) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, e_loop_ub, &qh_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            idx = nvec_data[ist];
            if (idx > 0.0) {
              st.site = &td_emlrtRSI;
              loop_ub = b_N->size[0] * b_N->size[1];
              b_N->size[0] = 1;
              b_N->size[1] = e_loop_ub;
              emxEnsureCapacity_real_T(&st, b_N, loop_ub, &xe_emlrtRTEI);
              b_N_data = b_N->data;
              for (b_i = 0; b_i < e_loop_ub; b_i++) {
                b_N_data[b_i] = nvec_data[b_i];
              }
              /*  N=ONER(N,r) */
              /*  Decrement element in position of r of input vector */
              /*  */
              /*  Copyright (c) 2012-2026, Imperial College London */
              /*  All rights reserved. */
              if (ist + 1 > e_loop_ub) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, e_loop_ub,
                                              &ph_emlrtBCI, &st);
              }
              b_N_data[ist] = idx - 1.0;
              st.site = &td_emlrtRSI;
              /*  idx=HASHPOP(n,N) */
              /*  idx=HASHPOP(n,N,R,prods) where prods(r)=prod(N(1:r-1)+1)
               * (faster) */
              /*  Hash a vector n on an integer lattice defined by vector N */
              /*  */
              /*  Copyright (c) 2012-2026, Imperial College London */
              /*  All rights reserved. */
              hnvec_c = 1.0;
              for (b_i = 0; b_i < C; b_i++) {
                if (b_i + 1 > prods->size[1]) {
                  emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, prods->size[1],
                                                &ih_emlrtBCI, &st);
                }
                if (b_i + 1 > b_N->size[1]) {
                  emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, b_N->size[1],
                                                &yg_emlrtBCI, &st);
                }
                hnvec_c += prods_data[b_i] * b_N_data[b_i];
                if (*emlrtBreakCheckR2012bFlagVar != 0) {
                  emlrtBreakCheckR2012b(&st);
                }
              }
              if (b_ist + 1 > M) {
                emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, M, &tf_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (ist + 1 > C) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, C, &uf_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (b_ist + 1 > Pc->size[0]) {
                emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, Pc->size[0],
                                              &xi_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (((int32_T)((uint32_T)b_r + 2U) < 1) ||
                  ((int32_T)((uint32_T)b_r + 2U) > Pc->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_r + 2U), 1,
                                              Pc->size[1], &yi_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (((int32_T)hnvec < 1) || ((int32_T)hnvec > Pc->size[2])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)hnvec, 1, Pc->size[2],
                                              &aj_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (b_ist + 1 > EC->size[0]) {
                emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, EC->size[0],
                                              &bj_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (((int32_T)((uint32_T)b_r + 1U) < 1) ||
                  ((int32_T)((uint32_T)b_r + 1U) > EC->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_r + 1U), 1,
                                              EC->size[1], &cj_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (ist + 1 > x->size[0]) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, x->size[0],
                                              &dj_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (((int32_T)hnvec < 1) || ((int32_T)hnvec > x->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)hnvec, 1, x->size[1],
                                              &ej_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (b_ist + 1 > Pc->size[0]) {
                emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, Pc->size[0],
                                              &fj_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (((int32_T)((uint32_T)b_r + 1U) < 1) ||
                  ((int32_T)((uint32_T)b_r + 1U) > Pc->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_r + 1U), 1,
                                              Pc->size[1], &gj_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (hnvec_c != (int32_T)muDoubleScalarFloor(hnvec_c)) {
                emlrtIntegerCheckR2012b(hnvec_c, &cb_emlrtDCI,
                                        (emlrtConstCTX)sp);
              }
              if (((int32_T)hnvec_c < 1) || ((int32_T)hnvec_c > Pc->size[2])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)hnvec_c, 1, Pc->size[2],
                                              &hj_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (b_ist + 1 > Pc->size[0]) {
                emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, Pc->size[0],
                                              &ij_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (((int32_T)((uint32_T)b_r + 2U) < 1) ||
                  ((int32_T)((uint32_T)b_r + 2U) > Pc->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_r + 2U), 1,
                                              Pc->size[1], &jj_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (hnvec != (int32_T)muDoubleScalarFloor(hnvec)) {
                emlrtIntegerCheckR2012b(hnvec, &db_emlrtDCI, (emlrtConstCTX)sp);
              }
              if (((int32_T)hnvec < 1) || ((int32_T)hnvec > Pc->size[2])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)hnvec, 1, Pc->size[2],
                                              &kj_emlrtBCI, (emlrtConstCTX)sp);
              }
              Pc_data[(b_ist + Pc->size[0] * (b_r + 1)) +
                      Pc->size[0] * Pc->size[1] * ((int32_T)hnvec - 1)] +=
                  D_data[b_ist + D->size[0] * ((int32_T)b_mu_data[ist] - 1)] *
                  EC_data[b_ist + EC->size[0] * b_r] *
                  x_data[ist + x->size[0] * ((int32_T)hnvec - 1)] *
                  Pc_data[(b_ist + Pc->size[0] * b_r) +
                          Pc->size[0] * Pc->size[1] * ((int32_T)hnvec_c - 1)];
            }
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b((emlrtConstCTX)sp);
            }
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b((emlrtConstCTX)sp);
          }
        }
        ii_data = Pc->size[1];
        if (Pc->size[1] < 1) {
          emlrtDynamicBoundsCheckR2012b(1, 1, Pc->size[1], &xf_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (muDoubleScalarIsNaN(nc)) {
          loop_ub = b_N->size[0] * b_N->size[1];
          b_N->size[0] = 1;
          b_N->size[1] = 1;
          emxEnsureCapacity_real_T(sp, b_N, loop_ub, &gb_emlrtRTEI);
          b_N_data = b_N->data;
          b_N_data[0] = rtNaN;
        } else if (nc < 1.0) {
          b_N->size[0] = 1;
          b_N->size[1] = 0;
        } else {
          loop_ub = b_N->size[0] * b_N->size[1];
          b_N->size[0] = 1;
          b_N->size[1] = (int32_T)(nc - 1.0) + 1;
          emxEnsureCapacity_real_T(sp, b_N, loop_ub, &gb_emlrtRTEI);
          b_N_data = b_N->data;
          loop_ub = (int32_T)(nc - 1.0);
          trueCount = (((int32_T)(nc - 1.0) + 1) / 2) << 1;
          result = trueCount - 2;
          for (b_i = 0; b_i <= result; b_i += 2) {
            dv[0] = b_i;
            dv[1] = b_i + 1;
            r5 = _mm_loadu_pd(&dv[0]);
            _mm_storeu_pd(&b_N_data[b_i], _mm_add_pd(_mm_set1_pd(1.0), r5));
          }
          for (b_i = trueCount; b_i <= loop_ub; b_i++) {
            b_N_data[b_i] = (real_T)b_i + 1.0;
          }
        }
        if (b_ist + 1 > Pc->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, Pc->size[0], &vf_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (((int32_T)hnvec < 1) || ((int32_T)hnvec > Pc->size[2])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)hnvec, 1, Pc->size[2],
                                        &wf_emlrtBCI, (emlrtConstCTX)sp);
        }
        loop_ub = b_Pc->size[0] * b_Pc->size[1];
        b_Pc->size[0] = 1;
        trueCount = b_N->size[1];
        b_Pc->size[1] = b_N->size[1];
        emxEnsureCapacity_real_T(sp, b_Pc, loop_ub, &df_emlrtRTEI);
        b_w_data = b_Pc->data;
        for (b_i = 0; b_i < trueCount; b_i++) {
          idx = b_N_data[b_i] + 1.0;
          if (idx != (int32_T)muDoubleScalarFloor(idx)) {
            emlrtIntegerCheckR2012b(idx, &v_emlrtDCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)idx < 1) || ((int32_T)idx > ii_data)) {
            emlrtDynamicBoundsCheckR2012b((int32_T)idx, 1, ii_data,
                                          &rh_emlrtBCI, (emlrtConstCTX)sp);
          }
          b_w_data[b_i] =
              Pc_data[(b_ist + Pc->size[0] * ((int32_T)idx - 1)) +
                      Pc->size[0] * Pc->size[1] * ((int32_T)hnvec - 1)];
        }
        st.site = &ud_emlrtRSI;
        idx = 1.0 - b_sum(&st, b_Pc);
        if (b_ist + 1 > Pc->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, Pc->size[0], &sh_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (hnvec != (int32_T)muDoubleScalarFloor(hnvec)) {
          emlrtIntegerCheckR2012b(hnvec, &w_emlrtDCI, (emlrtConstCTX)sp);
        }
        if (((int32_T)hnvec < 1) || ((int32_T)hnvec > Pc->size[2])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)hnvec, 1, Pc->size[2],
                                        &th_emlrtBCI, (emlrtConstCTX)sp);
        }
        Pc_data[b_ist + Pc->size[0] * Pc->size[1] * ((int32_T)hnvec - 1)] =
            muDoubleScalarMax(2.2204460492503131E-16, idx);
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b((emlrtConstCTX)sp);
        }
      }
      /*  now compute the normalizing constant */
      st.site = &vd_emlrtRSI;
      loop_ub = b->size[0] * b->size[1];
      b->size[0] = 1;
      b->size[1] = nvec->size[1];
      emxEnsureCapacity_boolean_T(&st, b, loop_ub, &bf_emlrtRTEI);
      b_data = b->data;
      for (b_i = 0; b_i < e_loop_ub; b_i++) {
        b_data[b_i] = (nvec_data[b_i] > 0.0);
      }
      b_st.site = &ge_emlrtRSI;
      c_eml_find(&b_st, b, (int32_T *)&ii_data, input_sizes);
      if (input_sizes[1] != 0) {
        if ((real_T)ii_data - 1.0 < 1.0) {
          trueCount = 0;
          result = 0;
        } else {
          if (nvec->size[1] < 1) {
            emlrtDynamicBoundsCheckR2012b(1, 1, nvec->size[1], &yf_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if ((ii_data - 1 < 1) || (ii_data - 1 > nvec->size[1])) {
            emlrtDynamicBoundsCheckR2012b(ii_data - 1, 1, nvec->size[1],
                                          &ag_emlrtBCI, (emlrtConstCTX)sp);
          }
          trueCount = ii_data - 1;
          if (C < 1) {
            emlrtDynamicBoundsCheckR2012b(1, 1, C, &bg_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if ((ii_data - 1 < 1) || (ii_data - 1 > C)) {
            emlrtDynamicBoundsCheckR2012b(ii_data - 1, 1, C, &cg_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          result = ii_data - 1;
        }
        loop_ub = b_Pc->size[0] * b_Pc->size[1];
        b_Pc->size[0] = 1;
        b_Pc->size[1] = trueCount;
        emxEnsureCapacity_real_T(sp, b_Pc, loop_ub, &cf_emlrtRTEI);
        b_w_data = b_Pc->data;
        for (b_i = 0; b_i < trueCount; b_i++) {
          b_w_data[b_i] = nvec_data[b_i];
        }
        loop_ub = b_N->size[0] * b_N->size[1];
        b_N->size[0] = 1;
        b_N->size[1] = result;
        emxEnsureCapacity_real_T(sp, b_N, loop_ub, &ef_emlrtRTEI);
        b_N_data = b_N->data;
        for (b_i = 0; b_i < result; b_i++) {
          b_N_data[b_i] = Nc_data[b_i];
        }
        st.site = &wd_emlrtRSI;
        if (b_sum(&st, b_Pc) == b_sum(&st, b_N)) {
          if ((real_T)ii_data + 1.0 > C) {
            result = 0;
            loop_ub = 0;
          } else {
            if ((ii_data + 1 < 1) || (ii_data + 1 > nvec->size[1])) {
              emlrtDynamicBoundsCheckR2012b(ii_data + 1, 1, nvec->size[1],
                                            &dg_emlrtBCI, (emlrtConstCTX)sp);
            }
            result = ii_data;
            if ((C < 1) || (C > nvec->size[1])) {
              emlrtDynamicBoundsCheckR2012b(C, 1, nvec->size[1], &eg_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            loop_ub = C;
          }
          trueCount = b_Pc->size[0] * b_Pc->size[1];
          b_Pc->size[0] = 1;
          loop_ub -= result;
          b_Pc->size[1] = loop_ub;
          emxEnsureCapacity_real_T(sp, b_Pc, trueCount, &gf_emlrtRTEI);
          b_w_data = b_Pc->data;
          for (b_i = 0; b_i < loop_ub; b_i++) {
            b_w_data[b_i] = nvec_data[result + b_i];
          }
          st.site = &wd_emlrtRSI;
          if (b_sum(&st, b_Pc) == 0.0) {
            st.site = &xd_emlrtRSI;
            if ((ii_data < 1) || (ii_data > c_loop_ub)) {
              emlrtDynamicBoundsCheckR2012b(ii_data, 1, c_loop_ub, &fg_emlrtBCI,
                                            &st);
            }
            lGN = rtInf;
          }
        }
      }
      st.site = &yd_emlrtRSI;
      pprod(&st, nvec, Nc);
      nvec_data = nvec->data;
    } else {
      exitg1 = 1;
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  } while (exitg1 == 0);
  emxFree_real_T(sp, &b_Pc);
  emxFree_real_T(sp, &b_w);
  emxFree_boolean_T(sp, &b);
  emxFree_real_T(sp, &nvec);
  emxFree_real_T(sp, &Zc);
  /*  compute performance indexes at Nc for closed classes */
  st.site = &ae_emlrtRSI;
  /*  idx=HASHPOP(n,N) */
  /*  idx=HASHPOP(n,N,R,prods) where prods(r)=prod(N(1:r-1)+1) (faster) */
  /*  Hash a vector n on an integer lattice defined by vector N */
  /*  */
  /*  Copyright (c) 2012-2026, Imperial College London */
  /*  All rights reserved. */
  hnvec = 1.0;
  for (b_i = 0; b_i < C; b_i++) {
    if (b_i + 1 > prods->size[1]) {
      emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, prods->size[1], &ih_emlrtBCI,
                                    &st);
    }
    if (b_i + 1 > C) {
      emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, C, &yg_emlrtBCI, &st);
    }
    hnvec += prods_data[b_i] * Nc_data[b_i];
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b(&st);
    }
  }
  if (closedClasses->size[1] - 1 >= 0) {
    g_loop_ub = Nc->size[1];
    i2 = closedClasses->size[1];
    i3 = D->size[0];
  }
  for (b_r = 0; b_r < C; b_r++) {
    st.site = &be_emlrtRSI;
    loop_ub = b_N->size[0] * b_N->size[1];
    b_N->size[0] = 1;
    b_N->size[1] = d_loop_ub;
    emxEnsureCapacity_real_T(&st, b_N, loop_ub, &xe_emlrtRTEI);
    b_N_data = b_N->data;
    for (b_i = 0; b_i < g_loop_ub; b_i++) {
      b_N_data[b_i] = Nc_data[b_i];
    }
    /*  N=ONER(N,r) */
    /*  Decrement element in position of r of input vector */
    /*  */
    /*  Copyright (c) 2012-2026, Imperial College London */
    /*  All rights reserved. */
    if (b_r + 1 > C) {
      emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, C, &ph_emlrtBCI, &st);
    }
    b_N_data[b_r] = Nc_data[b_r] - 1.0;
    st.site = &be_emlrtRSI;
    /*  idx=HASHPOP(n,N) */
    /*  idx=HASHPOP(n,N,R,prods) where prods(r)=prod(N(1:r-1)+1) (faster) */
    /*  Hash a vector n on an integer lattice defined by vector N */
    /*  */
    /*  Copyright (c) 2012-2026, Imperial College London */
    /*  All rights reserved. */
    hnvec_c = 1.0;
    for (b_i = 0; b_i < i2; b_i++) {
      if (b_i + 1 > prods->size[1]) {
        emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, prods->size[1], &ih_emlrtBCI,
                                      &st);
      }
      if (b_i + 1 > b_N->size[1]) {
        emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, b_N->size[1], &yg_emlrtBCI,
                                      &st);
      }
      hnvec_c += prods_data[b_i] * b_N_data[b_i];
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b(&st);
      }
    }
    for (b_i = 0; b_i < i3; b_i++) {
      if (b_i + 1 > u->size[0]) {
        emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, u->size[0], &vh_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (b_r + 1 > u->size[1]) {
        emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, u->size[1], &wh_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      u_data[b_i + u->size[0] * b_r] = 0.0;
      st.site = &ce_emlrtRSI;
      idx = b_sum(&st, Nc);
      loop_ub = (int32_T)idx;
      emlrtForLoopVectorCheckR2021a(1.0, 1.0, idx, mxDOUBLE_CLASS, (int32_T)idx,
                                    &m_emlrtRTEI, (emlrtConstCTX)sp);
      for (ist = 0; ist < loop_ub; ist++) {
        /*  closed class utilization */
        if (b_i + 1 > M) {
          emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, M, &gg_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_r + 1 > C) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, C, &hg_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_i + 1 > u->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, u->size[0], &ji_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_r + 1 > u->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, u->size[1], &ki_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_r + 1 > x->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, x->size[0], &li_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (hnvec != (int32_T)muDoubleScalarFloor(hnvec)) {
          emlrtIntegerCheckR2012b(hnvec, &ab_emlrtDCI, (emlrtConstCTX)sp);
        }
        if (((int32_T)hnvec < 1) || ((int32_T)hnvec > x->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)hnvec, 1, x->size[1],
                                        &mi_emlrtBCI, (emlrtConstCTX)sp);
        }
        if (b_i + 1 > Eprime->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, Eprime->size[0],
                                        &ni_emlrtBCI, (emlrtConstCTX)sp);
        }
        if (((int32_T)((uint32_T)ist + 1U) < 1) ||
            ((int32_T)((uint32_T)ist + 1U) > Eprime->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)ist + 1U), 1,
                                        Eprime->size[1], &oi_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_i + 1 > E->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, E->size[0], &pi_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (((int32_T)((uint32_T)ist + 1U) < 1) ||
            ((int32_T)((uint32_T)ist + 1U) > E->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)ist + 1U), 1,
                                        E->size[1], &qi_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_i + 1 > Pc->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, Pc->size[0], &ri_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (((int32_T)((uint32_T)ist + 1U) < 1) ||
            ((int32_T)((uint32_T)ist + 1U) > Pc->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)ist + 1U), 1,
                                        Pc->size[1], &si_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (hnvec_c != (int32_T)muDoubleScalarFloor(hnvec_c)) {
          emlrtIntegerCheckR2012b(hnvec_c, &bb_emlrtDCI, (emlrtConstCTX)sp);
        }
        if (((int32_T)hnvec_c < 1) || ((int32_T)hnvec_c > Pc->size[2])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)hnvec_c, 1, Pc->size[2],
                                        &ti_emlrtBCI, (emlrtConstCTX)sp);
        }
        if (b_i + 1 > u->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, u->size[0], &ui_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_r + 1 > u->size[1]) {
          emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, u->size[1], &vi_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        u_data[b_i + u->size[0] * b_r] +=
            D_data[b_i + D->size[0] * ((int32_T)b_mu_data[b_r] - 1)] *
            x_data[b_r + x->size[0] * ((int32_T)hnvec - 1)] *
            Eprime_data[b_i + Eprime->size[0] * ist] /
            E_data[b_i + E->size[0] * ist] *
            Pc_data[(b_i + Pc->size[0] * ist) +
                    Pc->size[0] * Pc->size[1] * ((int32_T)hnvec_c - 1)];
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b((emlrtConstCTX)sp);
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
  emxFree_real_T(sp, &prods);
  /*  Throughput */
  if (closedClasses->size[1] < 1) {
    result = 0;
  } else {
    if (x->size[0] < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, x->size[0], &ig_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (closedClasses->size[1] > x->size[0]) {
      emlrtDynamicBoundsCheckR2012b(closedClasses->size[1], 1, x->size[0],
                                    &jg_emlrtBCI, (emlrtConstCTX)sp);
    }
    result = closedClasses->size[1];
  }
  loop_ub = ii->size[0] * ii->size[1];
  ii->size[0] = 1;
  ii->size[1] = closedClasses->size[1];
  emxEnsureCapacity_int32_T(sp, ii, loop_ub, &ye_emlrtRTEI);
  b_ii_data = ii->data;
  loop_ub = D->size[1];
  trueCount = closedClasses->size[1];
  f_loop_ub = (closedClasses->size[1] < 1600);
  if (f_loop_ub) {
    for (i4 = 0; i4 < C; i4++) {
      loop_ub = (int32_T)closedClasses_data[i4];
      if ((loop_ub < 1) || (loop_ub > c_loop_ub)) {
        emlrtDynamicBoundsCheckR2012b(loop_ub, 1, c_loop_ub, &kh_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      b_ii_data[i4] = loop_ub;
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    pfqn_mvaldmx_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_mvaldmx_numThreads) private(             \
        d_st, emlrtJBEnviron, i5) firstprivate(emlrtHadParallelError)
    {
      if (setjmp(emlrtJBEnviron) == 0) {
        d_st.prev = sp;
        d_st.tls = emlrtAllocTLS((emlrtCTX)sp, omp_get_thread_num());
        d_st.site = NULL;
        emlrtSetJmpBuf(&d_st, &emlrtJBEnviron);
      } else {
        emlrtHadParallelError = true;
      }
#pragma omp for nowait
      for (i4 = 0; i4 < trueCount; i4++) {
        if (emlrtHadParallelError) {
          continue;
        }
        if (setjmp(emlrtJBEnviron) == 0) {
          i5 = (int32_T)closedClasses_data[i4];
          if ((i5 < 1) || (i5 > loop_ub)) {
            emlrtDynamicBoundsCheckR2012b(i5, 1, loop_ub, &kh_emlrtBCI, &d_st);
          }
          b_ii_data[i4] = i5;
        } else {
          emlrtHadParallelError = true;
        }
      }
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  if (hnvec != (int32_T)muDoubleScalarFloor(hnvec)) {
    emlrtIntegerCheckR2012b(hnvec, &t_emlrtDCI, (emlrtConstCTX)sp);
  }
  g_loop_ub = (int32_T)hnvec;
  if (((int32_T)hnvec < 1) || ((int32_T)hnvec > x->size[1])) {
    emlrtDynamicBoundsCheckR2012b((int32_T)hnvec, 1, x->size[1], &kg_emlrtBCI,
                                  (emlrtConstCTX)sp);
  }
  d_loop_ub = ii->size[1];
  if (ii->size[1] != result) {
    emlrtSubAssignSizeCheck1dR2017a(ii->size[1], result, &h_emlrtECI,
                                    (emlrtConstCTX)sp);
  }
  for (b_i = 0; b_i < d_loop_ub; b_i++) {
    XN_data[b_ii_data[b_i] - 1] =
        x_data[b_i + x->size[0] * ((int32_T)hnvec - 1)];
  }
  /*  Utilization */
  if (D->size[0] < 1) {
    result = 0;
  } else {
    if (u->size[0] < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, u->size[0], &lg_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (M > u->size[0]) {
      emlrtDynamicBoundsCheckR2012b(M, 1, u->size[0], &mg_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    result = M;
  }
  if (closedClasses->size[1] < 1) {
    ii_data = 0;
  } else {
    if (u->size[1] < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, u->size[1], &ng_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (C > u->size[1]) {
      emlrtDynamicBoundsCheckR2012b(C, 1, u->size[1], &og_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    ii_data = C;
  }
  if (D->size[0] < 1) {
    e_loop_ub = 0;
  } else {
    e_loop_ub = M;
  }
  loop_ub = ia->size[0];
  ia->size[0] = C;
  emxEnsureCapacity_int32_T(sp, ia, loop_ub, &ff_emlrtRTEI);
  ia_data = ia->data;
  if (f_loop_ub) {
    for (i6 = 0; i6 < C; i6++) {
      loop_ub = (int32_T)b_mu_data[i6];
      if ((loop_ub < 1) || (loop_ub > c_loop_ub)) {
        emlrtDynamicBoundsCheckR2012b(loop_ub, 1, c_loop_ub, &uh_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      ia_data[i6] = loop_ub - 1;
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    pfqn_mvaldmx_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_mvaldmx_numThreads) private(             \
        emlrtJBEnviron, e_st, i7) firstprivate(emlrtHadParallelError)
    {
      if (setjmp(emlrtJBEnviron) == 0) {
        e_st.prev = sp;
        e_st.tls = emlrtAllocTLS((emlrtCTX)sp, omp_get_thread_num());
        e_st.site = NULL;
        emlrtSetJmpBuf(&e_st, &emlrtJBEnviron);
      } else {
        emlrtHadParallelError = true;
      }
#pragma omp for nowait
      for (i6 = 0; i6 < C; i6++) {
        if (emlrtHadParallelError) {
          continue;
        }
        if (setjmp(emlrtJBEnviron) == 0) {
          i7 = (int32_T)b_mu_data[i6];
          if ((i7 < 1) || (i7 > c_loop_ub)) {
            emlrtDynamicBoundsCheckR2012b(i7, 1, c_loop_ub, &uh_emlrtBCI,
                                          &e_st);
          }
          ia_data[i6] = i7 - 1;
        } else {
          emlrtHadParallelError = true;
        }
      }
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  input_sizes[0] = e_loop_ub;
  trueCount = ia->size[0];
  input_sizes[1] = ia->size[0];
  b_result[0] = result;
  b_result[1] = ii_data;
  emlrtSubAssignSizeCheckR2012b(&input_sizes[0], 2, &b_result[0], 2,
                                &m_emlrtECI, (emlrtCTX)sp);
  for (b_i = 0; b_i < ii_data; b_i++) {
    for (b_r = 0; b_r < result; b_r++) {
      u_data[b_r + result * b_i] = u_data[b_r + u->size[0] * b_i];
    }
  }
  loop_ub = u->size[0] * u->size[1];
  u->size[0] = result;
  u->size[1] = ii_data;
  emxEnsureCapacity_real_T(sp, u, loop_ub, &hf_emlrtRTEI);
  u_data = u->data;
  for (b_i = 0; b_i < trueCount; b_i++) {
    for (b_r = 0; b_r < e_loop_ub; b_r++) {
      UN_data[b_r + UN->size[0] * ia_data[b_i]] = u_data[b_r + e_loop_ub * b_i];
    }
  }
  /*  Response time */
  if (D->size[0] < 1) {
    result = 0;
  } else {
    if (w->size[0] < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, w->size[0], &pg_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (M > w->size[0]) {
      emlrtDynamicBoundsCheckR2012b(M, 1, w->size[0], &qg_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    result = M;
  }
  if (closedClasses->size[1] < 1) {
    ii_data = 0;
  } else {
    if (w->size[1] < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, w->size[1], &rg_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (C > w->size[1]) {
      emlrtDynamicBoundsCheckR2012b(C, 1, w->size[1], &sg_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    ii_data = C;
  }
  if (D->size[0] < 1) {
    e_loop_ub = 0;
  } else {
    e_loop_ub = M;
  }
  loop_ub = ia->size[0];
  ia->size[0] = C;
  emxEnsureCapacity_int32_T(sp, ia, loop_ub, &if_emlrtRTEI);
  ia_data = ia->data;
  if (f_loop_ub) {
    for (i8 = 0; i8 < C; i8++) {
      loop_ub = (int32_T)b_mu_data[i8];
      if ((loop_ub < 1) || (loop_ub > c_loop_ub)) {
        emlrtDynamicBoundsCheckR2012b(loop_ub, 1, c_loop_ub, &wi_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      ia_data[i8] = loop_ub - 1;
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    pfqn_mvaldmx_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_mvaldmx_numThreads) private(             \
        emlrtJBEnviron, f_st, i9) firstprivate(emlrtHadParallelError)
    {
      if (setjmp(emlrtJBEnviron) == 0) {
        f_st.prev = sp;
        f_st.tls = emlrtAllocTLS((emlrtCTX)sp, omp_get_thread_num());
        f_st.site = NULL;
        emlrtSetJmpBuf(&f_st, &emlrtJBEnviron);
      } else {
        emlrtHadParallelError = true;
      }
#pragma omp for nowait
      for (i8 = 0; i8 < C; i8++) {
        if (emlrtHadParallelError) {
          continue;
        }
        if (setjmp(emlrtJBEnviron) == 0) {
          i9 = (int32_T)b_mu_data[i8];
          if ((i9 < 1) || (i9 > c_loop_ub)) {
            emlrtDynamicBoundsCheckR2012b(i9, 1, c_loop_ub, &wi_emlrtBCI,
                                          &f_st);
          }
          ia_data[i8] = i9 - 1;
        } else {
          emlrtHadParallelError = true;
        }
      }
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  if ((g_loop_ub < 1) || (g_loop_ub > w->size[2])) {
    emlrtDynamicBoundsCheckR2012b(g_loop_ub, 1, w->size[2], &tg_emlrtBCI,
                                  (emlrtConstCTX)sp);
  }
  input_sizes[0] = e_loop_ub;
  trueCount = ia->size[0];
  input_sizes[1] = ia->size[0];
  b_result[0] = result;
  b_result[1] = ii_data;
  emlrtSubAssignSizeCheckR2012b(&input_sizes[0], 2, &b_result[0], 2,
                                &i_emlrtECI, (emlrtCTX)sp);
  loop_ub = x->size[0] * x->size[1];
  x->size[0] = result;
  x->size[1] = ii_data;
  emxEnsureCapacity_real_T(sp, x, loop_ub, &jf_emlrtRTEI);
  x_data = x->data;
  for (b_i = 0; b_i < ii_data; b_i++) {
    for (b_r = 0; b_r < result; b_r++) {
      x_data[b_r + x->size[0] * b_i] =
          w_data[(b_r + w->size[0] * b_i) +
                 w->size[0] * w->size[1] * ((int32_T)hnvec - 1)];
    }
  }
  emxFree_real_T(sp, &w);
  for (b_i = 0; b_i < trueCount; b_i++) {
    for (b_r = 0; b_r < e_loop_ub; b_r++) {
      CN_data[b_r + CN->size[0] * ia_data[b_i]] = x_data[b_r + e_loop_ub * b_i];
    }
  }
  /*  Queue-length */
  if (D->size[0] < 1) {
    trueCount = 0;
  } else {
    if (M < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, M, &ug_emlrtBCI, (emlrtConstCTX)sp);
    }
    trueCount = M;
  }
  loop_ub = b_N->size[0] * b_N->size[1];
  b_N->size[0] = 1;
  b_N->size[1] = d_loop_ub;
  emxEnsureCapacity_real_T(sp, b_N, loop_ub, &kf_emlrtRTEI);
  b_N_data = b_N->data;
  for (b_i = 0; b_i < d_loop_ub; b_i++) {
    b_N_data[b_i] = XN_data[b_ii_data[b_i] - 1];
  }
  emxFree_int32_T(sp, &ii);
  st.site = &de_emlrtRSI;
  b_repmat(&st, b_N, D->size[0], x);
  x_data = x->data;
  emxFree_real_T(sp, &b_N);
  loop_ub = u->size[0] * u->size[1];
  u->size[0] = trueCount;
  u->size[1] = C;
  emxEnsureCapacity_real_T(sp, u, loop_ub, &lf_emlrtRTEI);
  u_data = u->data;
  for (b_i = 0; b_i < C; b_i++) {
    for (b_r = 0; b_r < trueCount; b_r++) {
      loop_ub = (int32_T)closedClasses_data[b_i];
      if ((loop_ub < 1) || (loop_ub > c_loop_ub)) {
        emlrtDynamicBoundsCheckR2012b(loop_ub, 1, c_loop_ub, &lj_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      u_data[b_r + u->size[0] * b_i] =
          CN_data[b_r + CN->size[0] * (loop_ub - 1)];
    }
  }
  emxFree_real_T(sp, &closedClasses);
  if ((x->size[0] != trueCount) && ((x->size[0] != 1) && (trueCount != 1))) {
    emlrtDimSizeImpxCheckR2021b(x->size[0], trueCount, &j_emlrtECI,
                                (emlrtConstCTX)sp);
  }
  if ((x->size[1] != C) && ((x->size[1] != 1) && (C != 1))) {
    emlrtDimSizeImpxCheckR2021b(x->size[1], C, &k_emlrtECI, (emlrtConstCTX)sp);
  }
  if (D->size[0] < 1) {
    ii_data = 0;
  } else {
    ii_data = M;
  }
  loop_ub = ia->size[0];
  ia->size[0] = C;
  emxEnsureCapacity_int32_T(sp, ia, loop_ub, &mf_emlrtRTEI);
  ia_data = ia->data;
  if (f_loop_ub) {
    for (i10 = 0; i10 < C; i10++) {
      loop_ub = (int32_T)b_mu_data[i10];
      if ((loop_ub < 1) || (loop_ub > c_loop_ub)) {
        emlrtDynamicBoundsCheckR2012b(loop_ub, 1, c_loop_ub, &mj_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      ia_data[i10] = loop_ub - 1;
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    pfqn_mvaldmx_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_mvaldmx_numThreads) private(             \
        emlrtJBEnviron, g_st, i11) firstprivate(emlrtHadParallelError)
    {
      if (setjmp(emlrtJBEnviron) == 0) {
        g_st.prev = sp;
        g_st.tls = emlrtAllocTLS((emlrtCTX)sp, omp_get_thread_num());
        g_st.site = NULL;
        emlrtSetJmpBuf(&g_st, &emlrtJBEnviron);
      } else {
        emlrtHadParallelError = true;
      }
#pragma omp for nowait
      for (i10 = 0; i10 < C; i10++) {
        if (emlrtHadParallelError) {
          continue;
        }
        if (setjmp(emlrtJBEnviron) == 0) {
          i11 = (int32_T)b_mu_data[i10];
          if ((i11 < 1) || (i11 > c_loop_ub)) {
            emlrtDynamicBoundsCheckR2012b(i11, 1, c_loop_ub, &mj_emlrtBCI,
                                          &g_st);
          }
          ia_data[i10] = i11 - 1;
        } else {
          emlrtHadParallelError = true;
        }
      }
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  emxFree_real_T(sp, &b_mu);
  if ((x->size[0] == u->size[0]) && (x->size[1] == u->size[1])) {
    loop_ub = x->size[0] * x->size[1];
    trueCount = (loop_ub / 2) << 1;
    result = trueCount - 2;
    for (b_i = 0; b_i <= result; b_i += 2) {
      r5 = _mm_loadu_pd(&x_data[b_i]);
      r6 = _mm_loadu_pd(&u_data[b_i]);
      _mm_storeu_pd(&x_data[b_i], _mm_mul_pd(r5, r6));
    }
    for (b_i = trueCount; b_i < loop_ub; b_i++) {
      x_data[b_i] *= u_data[b_i];
    }
  } else {
    st.site = &de_emlrtRSI;
    times(&st, x, u);
    x_data = x->data;
  }
  emxFree_real_T(sp, &u);
  input_sizes[0] = ii_data;
  loop_ub = ia->size[0];
  input_sizes[1] = ia->size[0];
  emlrtSubAssignSizeCheckR2012b(&input_sizes[0], 2, &x->size[0], 2, &l_emlrtECI,
                                (emlrtCTX)sp);
  for (b_i = 0; b_i < loop_ub; b_i++) {
    for (b_r = 0; b_r < ii_data; b_r++) {
      QN_data[b_r + QN->size[0] * ia_data[b_i]] = x_data[b_r + ii_data * b_i];
    }
  }
  emxFree_int32_T(sp, &ia);
  emxFree_real_T(sp, &x);
  /*  Compute performance indexes at Nc for open classes */
  for (b_r = 0; b_r < b_loop_ub; b_r++) {
    if (b_r + 1 > b_loop_ub) {
      emlrtDynamicBoundsCheckR2012b(b_r + 1, 1, b_loop_ub, &nj_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    trueCount = (int32_T)openClasses_data[b_r];
    /*  Throughput */
    idx = openClasses_data[b_r];
    if (((int32_T)idx < 1) || ((int32_T)idx > end)) {
      emlrtDynamicBoundsCheckR2012b((int32_T)idx, 1, end, &oj_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (((int32_T)idx < 1) || ((int32_T)idx > XN->size[1])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)idx, 1, XN->size[1], &pj_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    XN_data[(int32_T)idx - 1] = lambda_data[(int32_T)idx - 1];
    for (ist = 0; ist < M; ist++) {
      /*  Queue-length */
      if (ist + 1 > QN->size[0]) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, QN->size[0], &qj_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if ((trueCount < 1) || (trueCount > QN->size[1])) {
        emlrtDynamicBoundsCheckR2012b(trueCount, 1, QN->size[1], &rj_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      QN_data[ist + QN->size[0] * (trueCount - 1)] = 0.0;
      st.site = &ee_emlrtRSI;
      idx = b_sum(&st, Nc);
      loop_ub = (int32_T)(idx + 1.0);
      emlrtForLoopVectorCheckR2021a(0.0, 1.0, idx, mxDOUBLE_CLASS,
                                    (int32_T)(idx + 1.0), &n_emlrtRTEI,
                                    (emlrtConstCTX)sp);
      for (b_i = 0; b_i < loop_ub; b_i++) {
        if (ist + 1 > QN->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, QN->size[0], &xj_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (trueCount > QN->size[1]) {
          emlrtDynamicBoundsCheckR2012b(trueCount, 1, QN->size[1], &yj_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (trueCount > end) {
          emlrtDynamicBoundsCheckR2012b(trueCount, 1, end, &ak_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (ist + 1 > M) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, M, &bk_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (trueCount > c_loop_ub) {
          emlrtDynamicBoundsCheckR2012b(trueCount, 1, c_loop_ub, &ck_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (ist + 1 > EC->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, EC->size[0], &dk_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (((int32_T)((uint32_T)b_i + 1U) < 1) ||
            ((int32_T)((uint32_T)b_i + 1U) > EC->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_i + 1U), 1,
                                        EC->size[1], &ek_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (ist + 1 > Pc->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, Pc->size[0], &fk_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (((int32_T)((uint32_T)b_i + 1U) < 1) ||
            ((int32_T)((uint32_T)b_i + 1U) > Pc->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_i + 1U), 1,
                                        Pc->size[1], &gk_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (((int32_T)hnvec < 1) || ((int32_T)hnvec > Pc->size[2])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)hnvec, 1, Pc->size[2],
                                        &hk_emlrtBCI, (emlrtConstCTX)sp);
        }
        if (ist + 1 > QN->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, QN->size[0], &ik_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (trueCount > QN->size[1]) {
          emlrtDynamicBoundsCheckR2012b(trueCount, 1, QN->size[1], &jk_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        QN_data[ist + QN->size[0] * (trueCount - 1)] +=
            lambda_data[trueCount - 1] *
            D_data[ist + D->size[0] * (trueCount - 1)] * ((real_T)b_i + 1.0) *
            EC_data[ist + EC->size[0] * b_i] *
            Pc_data[(ist + Pc->size[0] * b_i) +
                    Pc->size[0] * Pc->size[1] * ((int32_T)hnvec - 1)];
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b((emlrtConstCTX)sp);
        }
      }
      /*  Response time */
      if (ist + 1 > QN->size[0]) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, QN->size[0], &sj_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (trueCount > QN->size[1]) {
        emlrtDynamicBoundsCheckR2012b(trueCount, 1, QN->size[1], &tj_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (trueCount > end) {
        emlrtDynamicBoundsCheckR2012b(trueCount, 1, end, &uj_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (ist + 1 > CN->size[0]) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, CN->size[0], &vj_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (trueCount > CN->size[1]) {
        emlrtDynamicBoundsCheckR2012b(trueCount, 1, CN->size[1], &wj_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      idx = lambda_data[trueCount - 1];
      CN_data[ist + CN->size[0] * (trueCount - 1)] =
          QN_data[ist + QN->size[0] * (trueCount - 1)] / idx;
      /*  Utilization - the formula from Bruell-Balbo-Ashfari does not */
      /*  match simulation, this appears to be simly lambda_r*D_{ir} */
      if (ist + 1 > UN->size[0]) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, UN->size[0], &kk_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (trueCount > UN->size[1]) {
        emlrtDynamicBoundsCheckR2012b(trueCount, 1, UN->size[1], &lk_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      UN_data[ist + UN->size[0] * (trueCount - 1)] = 0.0;
      st.site = &fe_emlrtRSI;
      hnvec_c = b_sum(&st, Nc);
      loop_ub = (int32_T)(hnvec_c + 1.0);
      emlrtForLoopVectorCheckR2021a(0.0, 1.0, hnvec_c, mxDOUBLE_CLASS,
                                    (int32_T)(hnvec_c + 1.0), &o_emlrtRTEI,
                                    (emlrtConstCTX)sp);
      for (b_i = 0; b_i < loop_ub; b_i++) {
        if (ist + 1 > UN->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, UN->size[0], &mk_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (trueCount > UN->size[1]) {
          emlrtDynamicBoundsCheckR2012b(trueCount, 1, UN->size[1], &nk_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (trueCount > end) {
          emlrtDynamicBoundsCheckR2012b(trueCount, 1, end, &ok_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (ist + 1 > Eprime->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, Eprime->size[0],
                                        &pk_emlrtBCI, (emlrtConstCTX)sp);
        }
        if (((int32_T)((uint32_T)b_i + 2U) < 1) ||
            ((int32_T)((uint32_T)b_i + 2U) > Eprime->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_i + 2U), 1,
                                        Eprime->size[1], &qk_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (ist + 1 > E->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, E->size[0], &rk_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (((int32_T)((uint32_T)b_i + 2U) < 1) ||
            ((int32_T)((uint32_T)b_i + 2U) > E->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_i + 2U), 1,
                                        E->size[1], &sk_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (ist + 1 > Pc->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, Pc->size[0], &tk_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (((int32_T)((uint32_T)b_i + 1U) < 1) ||
            ((int32_T)((uint32_T)b_i + 1U) > Pc->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_i + 1U), 1,
                                        Pc->size[1], &uk_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (((int32_T)hnvec < 1) || ((int32_T)hnvec > Pc->size[2])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)hnvec, 1, Pc->size[2],
                                        &vk_emlrtBCI, (emlrtConstCTX)sp);
        }
        if (ist + 1 > UN->size[0]) {
          emlrtDynamicBoundsCheckR2012b(ist + 1, 1, UN->size[0], &wk_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (trueCount > UN->size[1]) {
          emlrtDynamicBoundsCheckR2012b(trueCount, 1, UN->size[1], &xk_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        UN_data[ist + UN->size[0] * (trueCount - 1)] +=
            idx * Eprime_data[ist + Eprime->size[0] * (b_i + 1)] /
            E_data[ist + E->size[0] * (b_i + 1)] *
            Pc_data[(ist + Pc->size[0] * b_i) +
                    Pc->size[0] * Pc->size[1] * ((int32_T)hnvec - 1)];
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b((emlrtConstCTX)sp);
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
  emxFree_real_T(sp, &Eprime);
  emxFree_real_T(sp, &E);
  emxFree_real_T(sp, &EC);
  emxFree_real_T(sp, &Nc);
  emxFree_real_T(sp, &openClasses);
  if (D->size[0] >= 1) {
    if (Pc->size[0] < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, Pc->size[0], &vg_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (M > Pc->size[0]) {
      emlrtDynamicBoundsCheckR2012b(M, 1, Pc->size[0], &wg_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
  }
  if (g_loop_ub > Pc->size[2]) {
    emlrtDynamicBoundsCheckR2012b(g_loop_ub, 1, Pc->size[2], &xg_emlrtBCI,
                                  (emlrtConstCTX)sp);
  }
  emxFree_real_T(sp, &Pc);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return lGN;
}

/* End of code generation (pfqn_mvaldmx.c) */
