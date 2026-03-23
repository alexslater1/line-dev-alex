/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_nc_sanitize.c
 *
 * Code generation for function 'pfqn_nc_sanitize'
 *
 */

/* Include files */
#include "pfqn_nc_sanitize.h"
#include "div.h"
#include "eml_int_forloop_overflow_check.h"
#include "eml_setop.h"
#include "find.h"
#include "indexShapeCheck.h"
#include "log.h"
#include "nullAssignment.h"
#include "pfqn_comomrm_data.h"
#include "pfqn_comomrm_emxutil.h"
#include "pfqn_comomrm_types.h"
#include "repmat.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "sum.h"
#include "sumMatrixIncludeNaN.h"
#include "blas.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>
#include <stddef.h>

/* Variable Definitions */
static emlrtRSInfo x_emlrtRSI = {
    28,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo y_emlrtRSI = {
    34,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo ab_emlrtRSI = {
    35,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo bb_emlrtRSI = {
    37,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo cb_emlrtRSI = {
    42,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo db_emlrtRSI = {
    36,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo eb_emlrtRSI = {
    44,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo fb_emlrtRSI = {
    45,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo gb_emlrtRSI = {
    46,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo hb_emlrtRSI = {
    47,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo ib_emlrtRSI = {
    50,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo jb_emlrtRSI = {
    54,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo kb_emlrtRSI = {
    55,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo lb_emlrtRSI = {
    56,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo mb_emlrtRSI = {
    59,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo nb_emlrtRSI = {
    67,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo ob_emlrtRSI = {
    68,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo pb_emlrtRSI = {
    38,                 /* lineNo */
    "pfqn_nc_sanitize", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pathName */
};

static emlrtRSInfo qb_emlrtRSI = {
    39,                                                            /* lineNo */
    "find",                                                        /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pathName
                                                                    */
};

static emlrtRSInfo cc_emlrtRSI = {
    50,                 /* lineNo */
    "sumMatrixColumns", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

static emlrtRSInfo yc_emlrtRSI = {
    15,    /* lineNo */
    "max", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/max.m" /* pathName
                                                                     */
};

static emlrtRSInfo ad_emlrtRSI = {
    73,         /* lineNo */
    "minOrMax", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

static emlrtRSInfo bd_emlrtRSI = {
    108,       /* lineNo */
    "maximum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

static emlrtRSInfo cd_emlrtRSI =
    {
        290,             /* lineNo */
        "unaryMinOrMax", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo dd_emlrtRSI =
    {
        383,                     /* lineNo */
        "unaryMinOrMaxDispatch", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo ed_emlrtRSI =
    {
        451,          /* lineNo */
        "minOrMax2D", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo fd_emlrtRSI =
    {
        533,                         /* lineNo */
        "minOrMax2DColumnMajorDim1", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo gd_emlrtRSI =
    {
        531,                         /* lineNo */
        "minOrMax2DColumnMajorDim1", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo kd_emlrtRSI =
    {
        34,               /* lineNo */
        "rdivide_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "rdivide_helper.m" /* pathName */
};

static emlrtRSInfo ld_emlrtRSI = {
    53,    /* lineNo */
    "div", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/div.m" /* pathName
                                                                       */
};

static emlrtRSInfo md_emlrtRSI = {
    37,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sort.m" /* pathName
                                                                      */
};

static emlrtRSInfo he_emlrtRSI = {
    19,        /* lineNo */
    "setdiff", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/setdiff.m" /* pathName
                                                                     */
};

static emlrtRSInfo ie_emlrtRSI =
    {
        97,          /* lineNo */
        "eml_setop", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/private/"
        "eml_setop.m" /* pathName */
};

static emlrtRTEInfo p_emlrtRTEI = {
    13,                     /* lineNo */
    27,                     /* colNo */
    "assertCompatibleDims", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/shared/coder/coder/lib/+coder/+internal/"
    "assertCompatibleDims.m" /* pName */
};

static emlrtRTEInfo q_emlrtRTEI =
    {
        198,             /* lineNo */
        27,              /* colNo */
        "unaryMinOrMax", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pName */
};

static emlrtBCInfo ob_emlrtBCI = {
    -1,                 /* iFirst */
    -1,                 /* iLast */
    26,                 /* lineNo */
    1,                  /* colNo */
    "L",                /* aName */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    0                     /* checkKind */
};

static emlrtBCInfo pb_emlrtBCI = {
    -1,                 /* iFirst */
    -1,                 /* iLast */
    27,                 /* lineNo */
    1,                  /* colNo */
    "Z",                /* aName */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    0                     /* checkKind */
};

static emlrtBCInfo qb_emlrtBCI = {
    -1,                 /* iFirst */
    -1,                 /* iLast */
    29,                 /* lineNo */
    7,                  /* colNo */
    "L",                /* aName */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    0                     /* checkKind */
};

static emlrtBCInfo rb_emlrtBCI = {
    -1,                 /* iFirst */
    -1,                 /* iLast */
    30,                 /* lineNo */
    7,                  /* colNo */
    "N",                /* aName */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    0                     /* checkKind */
};

static emlrtBCInfo sb_emlrtBCI = {
    -1,                 /* iFirst */
    -1,                 /* iLast */
    31,                 /* lineNo */
    7,                  /* colNo */
    "Z",                /* aName */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    0                     /* checkKind */
};

static emlrtBCInfo tb_emlrtBCI = {
    -1,                 /* iFirst */
    -1,                 /* iLast */
    32,                 /* lineNo */
    17,                 /* colNo */
    "lambda",           /* aName */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    0                     /* checkKind */
};

static emlrtBCInfo ub_emlrtBCI = {
    -1,                 /* iFirst */
    -1,                 /* iLast */
    44,                 /* lineNo */
    31,                 /* colNo */
    "N",                /* aName */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    0                     /* checkKind */
};

static emlrtBCInfo vb_emlrtBCI = {
    -1,                 /* iFirst */
    -1,                 /* iLast */
    44,                 /* lineNo */
    52,                 /* colNo */
    "Z",                /* aName */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    0                     /* checkKind */
};

static emlrtBCInfo wb_emlrtBCI = {
    -1,                 /* iFirst */
    -1,                 /* iLast */
    44,                 /* lineNo */
    79,                 /* colNo */
    "N",                /* aName */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    0                     /* checkKind */
};

static emlrtBCInfo xb_emlrtBCI = {
    -1,                 /* iFirst */
    -1,                 /* iLast */
    63,                 /* lineNo */
    11,                 /* colNo */
    "Z",                /* aName */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    0                     /* checkKind */
};

static emlrtBCInfo yb_emlrtBCI = {
    -1,                 /* iFirst */
    -1,                 /* iLast */
    61,                 /* lineNo */
    15,                 /* colNo */
    "L",                /* aName */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    0                     /* checkKind */
};

static emlrtBCInfo ac_emlrtBCI = {
    -1,                 /* iFirst */
    -1,                 /* iLast */
    64,                 /* lineNo */
    11,                 /* colNo */
    "N",                /* aName */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    0                     /* checkKind */
};

static emlrtDCInfo j_emlrtDCI = {
    69,                 /* lineNo */
    7,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    1                     /* checkKind */
};

static emlrtBCInfo bc_emlrtBCI = {
    -1,                 /* iFirst */
    -1,                 /* iLast */
    69,                 /* lineNo */
    7,                  /* colNo */
    "L",                /* aName */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    0                     /* checkKind */
};

static emlrtDCInfo k_emlrtDCI = {
    70,                 /* lineNo */
    7,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    1                     /* checkKind */
};

static emlrtBCInfo cc_emlrtBCI = {
    -1,                 /* iFirst */
    -1,                 /* iLast */
    70,                 /* lineNo */
    7,                  /* colNo */
    "N",                /* aName */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    0                     /* checkKind */
};

static emlrtDCInfo l_emlrtDCI = {
    71,                 /* lineNo */
    7,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    1                     /* checkKind */
};

static emlrtBCInfo dc_emlrtBCI = {
    -1,                 /* iFirst */
    -1,                 /* iLast */
    71,                 /* lineNo */
    7,                  /* colNo */
    "Z",                /* aName */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m", /* pName */
    0                     /* checkKind */
};

static emlrtRTEInfo qc_emlrtRTEI = {
    39,                                                            /* lineNo */
    5,                                                             /* colNo */
    "find",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pName */
};

static emlrtRTEInfo rc_emlrtRTEI = {
    29,                 /* lineNo */
    7,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo sc_emlrtRTEI = {
    29,                 /* lineNo */
    3,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo tc_emlrtRTEI = {
    29,                 /* lineNo */
    1,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo uc_emlrtRTEI = {
    30,                 /* lineNo */
    3,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo vc_emlrtRTEI = {
    30,                 /* lineNo */
    1,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo wc_emlrtRTEI = {
    31,                 /* lineNo */
    3,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo xc_emlrtRTEI = {
    31,                 /* lineNo */
    1,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo yc_emlrtRTEI = {
    32,                 /* lineNo */
    8,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo ad_emlrtRTEI = {
    32,                 /* lineNo */
    1,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo bd_emlrtRTEI = {
    34,                 /* lineNo */
    19,                 /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo cd_emlrtRTEI = {
    34,                 /* lineNo */
    18,                 /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo dd_emlrtRTEI = {
    42,                 /* lineNo */
    18,                 /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo ed_emlrtRTEI = {
    42,                 /* lineNo */
    1,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo fd_emlrtRTEI = {
    44,                 /* lineNo */
    29,                 /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo gd_emlrtRTEI = {
    44,                 /* lineNo */
    46,                 /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo hd_emlrtRTEI =
    {
        523,             /* lineNo */
        21,              /* colNo */
        "unaryMinOrMax", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pName */
};

static emlrtRTEInfo id_emlrtRTEI = {
    44,                 /* lineNo */
    73,                 /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo jd_emlrtRTEI = {
    45,                 /* lineNo */
    9,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo kd_emlrtRTEI = {
    52,                 /* lineNo */
    5,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo ld_emlrtRTEI = {
    46,                 /* lineNo */
    9,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo md_emlrtRTEI = {
    47,                 /* lineNo */
    9,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo nd_emlrtRTEI = {
    55,                 /* lineNo */
    1,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo od_emlrtRTEI = {
    67,                 /* lineNo */
    21,                 /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo pd_emlrtRTEI = {
    38,     /* lineNo */
    5,      /* colNo */
    "sort", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sort.m" /* pName */
};

static emlrtRTEInfo qd_emlrtRTEI = {
    63,                 /* lineNo */
    7,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo rd_emlrtRTEI = {
    61,                 /* lineNo */
    11,                 /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo sd_emlrtRTEI = {
    67,                 /* lineNo */
    1,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo td_emlrtRTEI = {
    63,                 /* lineNo */
    5,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo ud_emlrtRTEI = {
    61,                 /* lineNo */
    9,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo vd_emlrtRTEI = {
    28,                                                           /* lineNo */
    9,                                                            /* colNo */
    "colon",                                                      /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/colon.m" /* pName */
};

static emlrtRTEInfo wd_emlrtRTEI = {
    64,                 /* lineNo */
    7,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo xd_emlrtRTEI = {
    64,                 /* lineNo */
    5,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo yd_emlrtRTEI = {
    69,                 /* lineNo */
    7,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo ae_emlrtRTEI = {
    69,                 /* lineNo */
    3,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo be_emlrtRTEI = {
    69,                 /* lineNo */
    1,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo ce_emlrtRTEI = {
    70,                 /* lineNo */
    7,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo de_emlrtRTEI = {
    70,                 /* lineNo */
    3,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo ee_emlrtRTEI = {
    70,                 /* lineNo */
    1,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo fe_emlrtRTEI = {
    71,                 /* lineNo */
    7,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo ge_emlrtRTEI = {
    71,                 /* lineNo */
    3,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo he_emlrtRTEI = {
    71,                 /* lineNo */
    1,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo ie_emlrtRTEI = {
    50,                 /* lineNo */
    1,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo je_emlrtRTEI = {
    68,                 /* lineNo */
    1,                  /* colNo */
    "pfqn_nc_sanitize", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_nc_sanitize.m" /* pName */
};

static emlrtRTEInfo ke_emlrtRTEI = {
    31,                                                            /* lineNo */
    6,                                                             /* colNo */
    "find",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pName */
};

static emlrtRSInfo sf_emlrtRSI = {
    54,    /* lineNo */
    "div", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/div.m" /* pathName
                                                                       */
};

/* Function Definitions */
real_T pfqn_nc_sanitize(const emlrtStack *sp, emxArray_real_T *lambda,
                        emxArray_real_T *L, emxArray_real_T *N,
                        emxArray_real_T *Z, real_T b_atol)
{
  __m128d r;
  __m128d r1;
  jmp_buf emlrtJBEnviron;
  ptrdiff_t incx_t;
  ptrdiff_t incy_t;
  ptrdiff_t n_t;
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
  emlrtStack m_st;
  emlrtStack n_st;
  emlrtStack o_st;
  emlrtStack st;
  emxArray_boolean_T *b_Lmax;
  emxArray_boolean_T *c_L;
  emxArray_int32_T *b_ii;
  emxArray_int32_T *ii;
  emxArray_real_T *Lmax;
  emxArray_real_T *b_L;
  emxArray_real_T *b_N;
  emxArray_real_T *nonzerothinktimes;
  emxArray_real_T *zerodemands;
  real_T dv[2];
  real_T b;
  real_T lGremaind;
  real_T s;
  real_T *L_data;
  real_T *Lmax_data;
  real_T *N_data;
  real_T *Z_data;
  real_T *b_L_data;
  real_T *lambda_data;
  real_T *zerodemands_data;
  int32_T b_i;
  int32_T b_loop_ub;
  int32_T c_i;
  int32_T c_loop_ub;
  int32_T d_loop_ub;
  int32_T i;
  int32_T i1;
  int32_T i10;
  int32_T i11;
  int32_T i12;
  int32_T i13;
  int32_T i14;
  int32_T i15;
  int32_T i2;
  int32_T i3;
  int32_T i4;
  int32_T i5;
  int32_T i6;
  int32_T i7;
  int32_T i8;
  int32_T i9;
  int32_T idx;
  int32_T loop_ub;
  int32_T n;
  int32_T nfb;
  int32_T nleft;
  int32_T nx;
  int32_T pfqn_nc_sanitize_numThreads;
  int32_T *ii_data;
  uint32_T sz_idx_1;
  boolean_T emlrtHadParallelError = false;
  boolean_T exitg1;
  boolean_T overflow;
  boolean_T p;
  boolean_T *b_Lmax_data;
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
  lambda_data = lambda->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /* { */
  /*  % @file pfqn_nc_sanitize.m */
  /*  % @brief Sanitize and preprocess network parameters for NC solvers. */
  /* } */
  /* { */
  /*  % @brief Sanitize and preprocess network parameters for NC solvers. */
  /*  % @fn pfqn_nc_sanitize(lambda, L, N, Z, atol) */
  /*  % @param lambda Arrival rate vector. */
  /*  % @param L Service demand matrix. */
  /*  % @param N Population vector. */
  /*  % @param Z Think time vector. */
  /*  % @param atol Absolute tolerance. */
  /*  % @return lambda Sanitized arrival rates. */
  /*  % @return L Sanitized service demands (rescaled). */
  /*  % @return N Sanitized populations. */
  /*  % @return Z Sanitized think times (rescaled). */
  /*  % @return lGremaind Log normalization factor from removed classes. */
  /* } */
  /*  erase empty classes */
  nfb = L->size[0] * L->size[1];
  for (i = 0; i < nfb; i++) {
    idx = L->size[0] * L->size[1];
    if (muDoubleScalarIsNaN(L_data[i])) {
      if (i > idx - 1) {
        emlrtDynamicBoundsCheckR2012b(i, 0, idx - 1, &ob_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      L_data[i] = 0.0;
    }
  }
  nfb = Z->size[1];
  for (i = 0; i < nfb; i++) {
    if (muDoubleScalarIsNaN(Z_data[i])) {
      idx = Z->size[1] - 1;
      if (i > idx) {
        emlrtDynamicBoundsCheckR2012b(i, 0, idx, &pb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      Z_data[i] = 0.0;
    }
  }
  st.site = &x_emlrtRSI;
  b_st.site = &qb_emlrtRSI;
  nx = N->size[1];
  c_st.site = &rb_emlrtRSI;
  idx = 0;
  emxInit_int32_T(&c_st, &ii, 2, &ke_emlrtRTEI);
  nfb = ii->size[0] * ii->size[1];
  ii->size[0] = 1;
  ii->size[1] = nx;
  emxEnsureCapacity_int32_T(&c_st, ii, nfb, &dc_emlrtRTEI);
  ii_data = ii->data;
  d_st.site = &sb_emlrtRSI;
  if (N->size[1] > 2147483646) {
    e_st.site = &tb_emlrtRSI;
    check_forloop_overflow_error(&e_st);
  }
  nfb = 0;
  exitg1 = false;
  while ((!exitg1) && (nfb <= nx - 1)) {
    if (N_data[nfb] != 0.0) {
      idx++;
      ii_data[idx - 1] = nfb + 1;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        nfb++;
      }
    } else {
      nfb++;
    }
  }
  if (idx > N->size[1]) {
    emlrtErrorWithMessageIdR2018a(&c_st, &g_emlrtRTEI,
                                  "Coder:builtins:AssertionFailed",
                                  "Coder:builtins:AssertionFailed", 0);
  }
  if (N->size[1] == 1) {
    if (idx == 0) {
      ii->size[0] = 1;
      ii->size[1] = 0;
    }
  } else {
    nfb = ii->size[0] * ii->size[1];
    if (idx < 1) {
      ii->size[1] = 0;
    } else {
      ii->size[1] = idx;
    }
    emxEnsureCapacity_int32_T(&c_st, ii, nfb, &qc_emlrtRTEI);
    ii_data = ii->data;
  }
  emxInit_real_T(sp, &zerodemands, 1, &ed_emlrtRTEI);
  loop_ub = ii->size[1];
  nfb = zerodemands->size[0];
  zerodemands->size[0] = ii->size[1];
  emxEnsureCapacity_real_T(sp, zerodemands, nfb, &rc_emlrtRTEI);
  zerodemands_data = zerodemands->data;
  for (i = 0; i < loop_ub; i++) {
    zerodemands_data[i] = ii_data[i];
  }
  nleft = L->size[0];
  n = L->size[1];
  emxInit_real_T(sp, &b_L, 2, &sc_emlrtRTEI);
  nfb = b_L->size[0] * b_L->size[1];
  b_L->size[0] = nleft;
  b_L->size[1] = ii->size[1];
  emxEnsureCapacity_real_T(sp, b_L, nfb, &sc_emlrtRTEI);
  b_L_data = b_L->data;
  for (i = 0; i < loop_ub; i++) {
    for (b_i = 0; b_i < nleft; b_i++) {
      idx = (int32_T)zerodemands_data[i];
      if (idx > n) {
        emlrtDynamicBoundsCheckR2012b(idx, 1, n, &qb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      b_L_data[b_i + b_L->size[0] * i] = L_data[b_i + L->size[0] * (idx - 1)];
    }
  }
  idx = b_L->size[0];
  nfb = L->size[0] * L->size[1];
  L->size[0] = b_L->size[0];
  emxEnsureCapacity_real_T(sp, L, nfb, &tc_emlrtRTEI);
  n = b_L->size[1];
  nfb = L->size[0] * L->size[1];
  L->size[1] = b_L->size[1];
  emxEnsureCapacity_real_T(sp, L, nfb, &tc_emlrtRTEI);
  L_data = L->data;
  for (i = 0; i < n; i++) {
    for (b_i = 0; b_i < idx; b_i++) {
      L_data[b_i + L->size[0] * i] = b_L_data[b_i + b_L->size[0] * i];
    }
  }
  emxInit_real_T(sp, &b_N, 2, &uc_emlrtRTEI);
  nfb = b_N->size[0] * b_N->size[1];
  b_N->size[0] = 1;
  b_N->size[1] = ii->size[1];
  emxEnsureCapacity_real_T(sp, b_N, nfb, &uc_emlrtRTEI);
  b_L_data = b_N->data;
  for (i = 0; i < loop_ub; i++) {
    nfb = (int32_T)zerodemands_data[i];
    if (nfb > nx) {
      emlrtDynamicBoundsCheckR2012b(nfb, 1, nx, &rb_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    b_L_data[i] = N_data[nfb - 1];
  }
  nfb = N->size[0] * N->size[1];
  N->size[0] = 1;
  emxEnsureCapacity_real_T(sp, N, nfb, &vc_emlrtRTEI);
  nleft = b_N->size[1];
  nfb = N->size[0] * N->size[1];
  N->size[1] = b_N->size[1];
  emxEnsureCapacity_real_T(sp, N, nfb, &vc_emlrtRTEI);
  N_data = N->data;
  for (i = 0; i < nleft; i++) {
    N_data[i] = b_L_data[i];
  }
  idx = Z->size[1];
  nfb = b_N->size[0] * b_N->size[1];
  b_N->size[0] = 1;
  b_N->size[1] = ii->size[1];
  emxEnsureCapacity_real_T(sp, b_N, nfb, &wc_emlrtRTEI);
  b_L_data = b_N->data;
  for (i = 0; i < loop_ub; i++) {
    nfb = (int32_T)zerodemands_data[i];
    if (nfb > idx) {
      emlrtDynamicBoundsCheckR2012b(nfb, 1, idx, &sb_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    b_L_data[i] = Z_data[nfb - 1];
  }
  nfb = Z->size[0] * Z->size[1];
  Z->size[0] = 1;
  emxEnsureCapacity_real_T(sp, Z, nfb, &xc_emlrtRTEI);
  idx = b_N->size[1];
  nfb = Z->size[0] * Z->size[1];
  Z->size[1] = b_N->size[1];
  emxEnsureCapacity_real_T(sp, Z, nfb, &xc_emlrtRTEI);
  Z_data = Z->data;
  for (i = 0; i < idx; i++) {
    Z_data[i] = b_L_data[i];
  }
  idx = lambda->size[1];
  nfb = b_N->size[0] * b_N->size[1];
  b_N->size[0] = 1;
  b_N->size[1] = ii->size[1];
  emxEnsureCapacity_real_T(sp, b_N, nfb, &yc_emlrtRTEI);
  b_L_data = b_N->data;
  for (i = 0; i < loop_ub; i++) {
    nfb = (int32_T)zerodemands_data[i];
    if (nfb > idx) {
      emlrtDynamicBoundsCheckR2012b(nfb, 1, idx, &tb_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    b_L_data[i] = lambda_data[nfb - 1];
  }
  nfb = lambda->size[0] * lambda->size[1];
  lambda->size[0] = 1;
  emxEnsureCapacity_real_T(sp, lambda, nfb, &ad_emlrtRTEI);
  idx = b_N->size[1];
  nfb = lambda->size[0] * lambda->size[1];
  lambda->size[1] = b_N->size[1];
  emxEnsureCapacity_real_T(sp, lambda, nfb, &ad_emlrtRTEI);
  lambda_data = lambda->data;
  for (i = 0; i < idx; i++) {
    lambda_data[i] = b_L_data[i];
  }
  /*  erase ill-defined classes */
  st.site = &y_emlrtRSI;
  b_st.site = &ub_emlrtRSI;
  c_st.site = &vb_emlrtRSI;
  d_st.site = &wb_emlrtRSI;
  emxInit_real_T(&d_st, &Lmax, 2, &ie_emlrtRTEI);
  if ((L->size[0] == 0) || (L->size[1] == 0) || (L->size[0] == 0)) {
    sz_idx_1 = (uint32_T)L->size[1];
    nfb = Lmax->size[0] * Lmax->size[1];
    Lmax->size[0] = 1;
    Lmax->size[1] = (int32_T)sz_idx_1;
    emxEnsureCapacity_real_T(&d_st, Lmax, nfb, &bd_emlrtRTEI);
    Lmax_data = Lmax->data;
    nfb = (int32_T)sz_idx_1;
    for (i = 0; i < nfb; i++) {
      Lmax_data[i] = 0.0;
    }
  } else {
    e_st.site = &xb_emlrtRSI;
    f_st.site = &yb_emlrtRSI;
    nfb = Lmax->size[0] * Lmax->size[1];
    Lmax->size[0] = 1;
    Lmax->size[1] = b_L->size[1];
    emxEnsureCapacity_real_T(&f_st, Lmax, nfb, &cc_emlrtRTEI);
    Lmax_data = Lmax->data;
    if (L->size[0] < 4096) {
      g_st.site = &ac_emlrtRSI;
      if (L->size[1] > 2147483646) {
        h_st.site = &tb_emlrtRSI;
        check_forloop_overflow_error(&h_st);
      }
      for (i = 0; i < n; i++) {
        g_st.site = &bc_emlrtRSI;
        Lmax_data[i] = sumColumnB(&g_st, L, i + 1, L->size[0]);
      }
    } else {
      nfb = (int32_T)((uint32_T)L->size[0] >> 12);
      idx = nfb << 12;
      nleft = L->size[0] - idx;
      g_st.site = &cc_emlrtRSI;
      if (L->size[1] > 2147483646) {
        h_st.site = &tb_emlrtRSI;
        check_forloop_overflow_error(&h_st);
      }
      for (i = 0; i < n; i++) {
        s = sumColumnB4(L, i + 1, 1);
        for (b_i = 2; b_i <= nfb; b_i++) {
          s += sumColumnB4(L, i + 1, ((b_i - 1) << 12) + 1);
        }
        if (nleft > 0) {
          g_st.site = &dc_emlrtRSI;
          s += b_sumColumnB(&g_st, L, i + 1, nleft, idx + 1);
        }
        Lmax_data[i] = s;
      }
    }
  }
  st.site = &y_emlrtRSI;
  sum(&st, Z, b_N);
  b_L_data = b_N->data;
  st.site = &y_emlrtRSI;
  emxInit_boolean_T(&st, &b_Lmax, &cd_emlrtRTEI);
  nfb = b_Lmax->size[0] * b_Lmax->size[1];
  b_Lmax->size[0] = 1;
  idx = Lmax->size[1];
  b_Lmax->size[1] = Lmax->size[1];
  emxEnsureCapacity_boolean_T(&st, b_Lmax, nfb, &cd_emlrtRTEI);
  b_Lmax_data = b_Lmax->data;
  nfb = Lmax->size[1];
  if (Lmax->size[1] < 1600) {
    for (c_i = 0; c_i < idx; c_i++) {
      b_Lmax_data[c_i] = (Lmax_data[c_i] + b_L_data[c_i] < b_atol);
    }
  } else {
    emlrtEnterParallelRegion(&st, omp_in_parallel());
    emlrtPushJmpBuf(&st, &emlrtJBStack);
    pfqn_nc_sanitize_numThreads = emlrtAllocRegionTLSs(
        st.tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_nc_sanitize_numThreads)

    for (c_i = 0; c_i < nfb; c_i++) {
      b_Lmax_data[c_i] = (Lmax_data[c_i] + b_L_data[c_i] < b_atol);
    }
    emlrtPopJmpBuf(&st, &emlrtJBStack);
    emlrtExitParallelRegion(&st, omp_in_parallel());
  }
  b_st.site = &qb_emlrtRSI;
  eml_find(&b_st, b_Lmax, ii);
  st.site = &ab_emlrtRSI;
  nullAssignment(&st, L, ii);
  L_data = L->data;
  st.site = &db_emlrtRSI;
  b_nullAssignment(&st, N, ii);
  N_data = N->data;
  st.site = &bb_emlrtRSI;
  b_nullAssignment(&st, Z, ii);
  Z_data = Z->data;
  st.site = &pb_emlrtRSI;
  b_nullAssignment(&st, lambda, ii);
  /*  */
  lGremaind = 0.0;
  /*  find zero demand classes */
  st.site = &cb_emlrtRSI;
  emxInit_boolean_T(&st, &c_L, &dd_emlrtRTEI);
  nfb = c_L->size[0] * c_L->size[1];
  c_L->size[0] = L->size[0];
  c_L->size[1] = L->size[1];
  emxEnsureCapacity_boolean_T(&st, c_L, nfb, &dd_emlrtRTEI);
  b_Lmax_data = c_L->data;
  nfb = L->size[0] * L->size[1];
  if (nfb < 1600) {
    for (i1 = 0; i1 < nfb; i1++) {
      b_Lmax_data[i1] = (L_data[i1] < b_atol);
    }
  } else {
    emlrtEnterParallelRegion(&st, omp_in_parallel());
    emlrtPushJmpBuf(&st, &emlrtJBStack);
    pfqn_nc_sanitize_numThreads = emlrtAllocRegionTLSs(
        st.tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_nc_sanitize_numThreads)

    for (i1 = 0; i1 < nfb; i1++) {
      b_Lmax_data[i1] = (L_data[i1] < b_atol);
    }
    emlrtPopJmpBuf(&st, &emlrtJBStack);
    emlrtExitParallelRegion(&st, omp_in_parallel());
  }
  emxInit_int32_T(&st, &b_ii, 1, &ke_emlrtRTEI);
  b_st.site = &qb_emlrtRSI;
  b_eml_find(&b_st, c_L, b_ii);
  ii_data = b_ii->data;
  emxFree_boolean_T(&st, &c_L);
  n = b_ii->size[0];
  nfb = zerodemands->size[0];
  zerodemands->size[0] = b_ii->size[0];
  emxEnsureCapacity_real_T(&st, zerodemands, nfb, &ed_emlrtRTEI);
  zerodemands_data = zerodemands->data;
  for (i = 0; i < n; i++) {
    zerodemands_data[i] = ii_data[i];
  }
  if (zerodemands->size[0] != 0) {
    st.site = &eb_emlrtRSI;
    b_indexShapeCheck(&st, N->size, zerodemands->size[0]);
    st.site = &eb_emlrtRSI;
    b_indexShapeCheck(&st, Z->size, zerodemands->size[0]);
    nleft = N->size[1];
    emxInit_real_T(sp, &lambda, 1, &fd_emlrtRTEI);
    nfb = lambda->size[0];
    lambda->size[0] = b_ii->size[0];
    emxEnsureCapacity_real_T(sp, lambda, nfb, &fd_emlrtRTEI);
    lambda_data = lambda->data;
    for (i = 0; i < n; i++) {
      nfb = (int32_T)zerodemands_data[i];
      if ((nfb < 1) || (nfb > nleft)) {
        emlrtDynamicBoundsCheckR2012b(nfb, 1, nleft, &ub_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      lambda_data[i] = N_data[nfb - 1];
    }
    idx = Z->size[1];
    nfb = Lmax->size[0] * Lmax->size[1];
    Lmax->size[0] = 1;
    Lmax->size[1] = b_ii->size[0];
    emxEnsureCapacity_real_T(sp, Lmax, nfb, &gd_emlrtRTEI);
    Lmax_data = Lmax->data;
    for (i = 0; i < n; i++) {
      nfb = (int32_T)zerodemands_data[i];
      if ((nfb < 1) || (nfb > idx)) {
        emlrtDynamicBoundsCheckR2012b(nfb, 1, idx, &vb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      Lmax_data[i] = Z_data[nfb - 1];
    }
    st.site = &eb_emlrtRSI;
    b_log(&st, Lmax);
    Lmax_data = Lmax->data;
    st.site = &eb_emlrtRSI;
    b_indexShapeCheck(&st, N->size, zerodemands->size[0]);
    st.site = &eb_emlrtRSI;
    b_st.site = &wc_emlrtRSI;
    if (zerodemands->size[0] != Lmax->size[1]) {
      if ((zerodemands->size[0] == 1) || (Lmax->size[1] == 1)) {
        emlrtErrorWithMessageIdR2018a(
            &b_st, &emlrtRTEI, "Coder:toolbox:mtimes_noDynamicScalarExpansion",
            "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
      } else {
        emlrtErrorWithMessageIdR2018a(&b_st, &b_emlrtRTEI, "MATLAB:innerdim",
                                      "MATLAB:innerdim", 0);
      }
    }
    n_t = (ptrdiff_t)zerodemands->size[0];
    incx_t = (ptrdiff_t)1;
    incy_t = (ptrdiff_t)1;
    s = ddot(&n_t, &lambda_data[0], &incx_t, &Lmax_data[0], &incy_t);
    emxFree_real_T(&st, &lambda);
    nfb = Lmax->size[0] * Lmax->size[1];
    Lmax->size[0] = 1;
    Lmax->size[1] = b_ii->size[0];
    emxEnsureCapacity_real_T(sp, Lmax, nfb, &id_emlrtRTEI);
    Lmax_data = Lmax->data;
    for (i = 0; i < n; i++) {
      nfb = (int32_T)zerodemands_data[i];
      if ((nfb < 1) || (nfb > nleft)) {
        emlrtDynamicBoundsCheckR2012b(nfb, 1, nleft, &wb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      Lmax_data[i] = N_data[nfb - 1];
    }
    st.site = &eb_emlrtRSI;
    b_log(&st, Lmax);
    st.site = &eb_emlrtRSI;
    lGremaind = s - b_sum(&st, Lmax);
    nfb = b_ii->size[0];
    b_ii->size[0] = n;
    emxEnsureCapacity_int32_T(sp, b_ii, nfb, &jd_emlrtRTEI);
    ii_data = b_ii->data;
    for (i = 0; i < n; i++) {
      ii_data[i] = (int32_T)zerodemands_data[i];
    }
    st.site = &fb_emlrtRSI;
    c_nullAssignment(&st, L, b_ii);
    L_data = L->data;
    nfb = b_ii->size[0];
    b_ii->size[0] = n;
    emxEnsureCapacity_int32_T(sp, b_ii, nfb, &ld_emlrtRTEI);
    ii_data = b_ii->data;
    for (i = 0; i < n; i++) {
      ii_data[i] = (int32_T)zerodemands_data[i];
    }
    st.site = &gb_emlrtRSI;
    d_nullAssignment(&st, Z, b_ii);
    nfb = b_ii->size[0];
    b_ii->size[0] = n;
    emxEnsureCapacity_int32_T(sp, b_ii, nfb, &md_emlrtRTEI);
    ii_data = b_ii->data;
    for (i = 0; i < n; i++) {
      ii_data[i] = (int32_T)zerodemands_data[i];
    }
    st.site = &hb_emlrtRSI;
    d_nullAssignment(&st, N, b_ii);
    N_data = N->data;
  }
  emxFree_real_T(sp, &zerodemands);
  /*  rescale demands */
  st.site = &ib_emlrtRSI;
  b_st.site = &yc_emlrtRSI;
  c_st.site = &ad_emlrtRSI;
  d_st.site = &bd_emlrtRSI;
  if (L->size[0] < 1) {
    emlrtErrorWithMessageIdR2018a(&d_st, &q_emlrtRTEI,
                                  "Coder:toolbox:eml_min_or_max_varDimZero",
                                  "Coder:toolbox:eml_min_or_max_varDimZero", 0);
  }
  e_st.site = &cd_emlrtRSI;
  f_st.site = &dd_emlrtRSI;
  g_st.site = &ed_emlrtRSI;
  idx = L->size[0];
  n = L->size[1];
  nfb = Lmax->size[0] * Lmax->size[1];
  Lmax->size[0] = 1;
  Lmax->size[1] = n;
  emxEnsureCapacity_real_T(&g_st, Lmax, nfb, &hd_emlrtRTEI);
  Lmax_data = Lmax->data;
  if (L->size[1] >= 1) {
    h_st.site = &gd_emlrtRSI;
    if (L->size[1] > 2147483646) {
      i_st.site = &tb_emlrtRSI;
      check_forloop_overflow_error(&i_st);
    }
    overflow = (idx > 2147483646);
    for (i = 0; i < n; i++) {
      Lmax_data[i] = L_data[L->size[0] * i];
      h_st.site = &fd_emlrtRSI;
      if (overflow) {
        i_st.site = &tb_emlrtRSI;
        check_forloop_overflow_error(&i_st);
      }
      for (b_i = 2; b_i <= idx; b_i++) {
        s = Lmax_data[i];
        b = L_data[(b_i + L->size[0] * i) - 1];
        if (muDoubleScalarIsNaN(b)) {
          p = false;
        } else if (muDoubleScalarIsNaN(s)) {
          p = true;
        } else {
          p = (s < b);
        }
        if (p) {
          Lmax_data[i] = b;
        }
      }
    }
  }
  /*  use L, which has been santized to always be ~=0 */
  if (Lmax->size[1] == 0) {
    nfb = Lmax->size[0] * Lmax->size[1];
    Lmax->size[0] = 1;
    idx = Z->size[1];
    Lmax->size[1] = idx;
    emxEnsureCapacity_real_T(sp, Lmax, nfb, &kd_emlrtRTEI);
    Lmax_data = Lmax->data;
    for (i = 0; i < idx; i++) {
      Lmax_data[i] = 1.0;
    }
  }
  st.site = &jb_emlrtRSI;
  b_st.site = &jb_emlrtRSI;
  repmat(&b_st, Lmax, L->size[0], b_L);
  b_L_data = b_L->data;
  b_st.site = &kd_emlrtRSI;
  c_st.site = &ld_emlrtRSI;
  if (((L->size[0] != 1) && (b_L->size[0] != 1) &&
       (L->size[0] != b_L->size[0])) ||
      ((L->size[1] != 1) && (b_L->size[1] != 1) &&
       (L->size[1] != b_L->size[1]))) {
    emlrtErrorWithMessageIdR2018a(&c_st, &p_emlrtRTEI,
                                  "MATLAB:sizeDimensionsMustMatch",
                                  "MATLAB:sizeDimensionsMustMatch", 0);
  }
  if ((L->size[0] == b_L->size[0]) && (L->size[1] == b_L->size[1])) {
    for (i = 0; i < n; i++) {
      nfb = L->size[0];
      idx = (nfb / 2) << 1;
      nleft = idx - 2;
      for (b_i = 0; b_i <= nleft; b_i += 2) {
        r = _mm_loadu_pd(&L_data[b_i + L->size[0] * i]);
        r1 = _mm_loadu_pd(&b_L_data[b_i + b_L->size[0] * i]);
        _mm_storeu_pd(&L_data[b_i + L->size[0] * i], _mm_div_pd(r, r1));
      }
      for (b_i = idx; b_i < nfb; b_i++) {
        L_data[b_i + L->size[0] * i] /= b_L_data[b_i + b_L->size[0] * i];
      }
    }
  } else {
    c_st.site = &sf_emlrtRSI;
    b_rdivide(&c_st, L, b_L);
    L_data = L->data;
  }
  st.site = &kb_emlrtRSI;
  b_st.site = &kd_emlrtRSI;
  c_st.site = &ld_emlrtRSI;
  if ((Z->size[1] != 1) && (Lmax->size[1] != 1) &&
      (Z->size[1] != Lmax->size[1])) {
    emlrtErrorWithMessageIdR2018a(&c_st, &p_emlrtRTEI,
                                  "MATLAB:sizeDimensionsMustMatch",
                                  "MATLAB:sizeDimensionsMustMatch", 0);
  }
  if (Z->size[1] == Lmax->size[1]) {
    nfb = Z->size[0] * Z->size[1];
    Z->size[0] = 1;
    emxEnsureCapacity_real_T(&b_st, Z, nfb, &nd_emlrtRTEI);
    Z_data = Z->data;
    nfb = Z->size[1];
    idx = (nfb / 2) << 1;
    nleft = idx - 2;
    for (i = 0; i <= nleft; i += 2) {
      r = _mm_loadu_pd(&Z_data[i]);
      r1 = _mm_loadu_pd(&Lmax_data[i]);
      _mm_storeu_pd(&Z_data[i], _mm_div_pd(r, r1));
    }
    for (i = idx; i < nfb; i++) {
      Z_data[i] /= Lmax_data[i];
    }
  } else {
    c_st.site = &sf_emlrtRSI;
    rdivide(&c_st, Z, Lmax);
    Z_data = Z->data;
  }
  st.site = &lb_emlrtRSI;
  b_log(&st, Lmax);
  Lmax_data = Lmax->data;
  st.site = &lb_emlrtRSI;
  b_st.site = &wc_emlrtRSI;
  if (N->size[1] != Lmax->size[1]) {
    if ((N->size[1] == 1) || (Lmax->size[1] == 1)) {
      emlrtErrorWithMessageIdR2018a(
          &b_st, &emlrtRTEI, "Coder:toolbox:mtimes_noDynamicScalarExpansion",
          "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
    } else {
      emlrtErrorWithMessageIdR2018a(&b_st, &b_emlrtRTEI, "MATLAB:innerdim",
                                    "MATLAB:innerdim", 0);
    }
  }
  if (N->size[1] < 1) {
    s = 0.0;
  } else {
    n_t = (ptrdiff_t)N->size[1];
    incx_t = (ptrdiff_t)1;
    incy_t = (ptrdiff_t)1;
    s = ddot(&n_t, &N_data[0], &incx_t, &Lmax_data[0], &incy_t);
  }
  lGremaind += s;
  /*  sort from smallest to largest think time */
  if (Z->size[1] != 0) {
    st.site = &mb_emlrtRSI;
    b_st.site = &mb_emlrtRSI;
    sum(&b_st, Z, Lmax);
    b_st.site = &md_emlrtRSI;
    sort(&b_st, Lmax, ii);
    ii_data = ii->data;
    nfb = Lmax->size[0] * Lmax->size[1];
    Lmax->size[0] = 1;
    n = ii->size[1];
    Lmax->size[1] = ii->size[1];
    emxEnsureCapacity_real_T(&st, Lmax, nfb, &pd_emlrtRTEI);
    Lmax_data = Lmax->data;
    for (i = 0; i < n; i++) {
      Lmax_data[i] = ii_data[i];
    }
    if ((L->size[0] != 0) && (L->size[1] != 0)) {
      idx = L->size[0];
      nleft = L->size[1];
      nfb = b_L->size[0] * b_L->size[1];
      b_L->size[0] = idx;
      b_L->size[1] = ii->size[1];
      emxEnsureCapacity_real_T(sp, b_L, nfb, &rd_emlrtRTEI);
      b_L_data = b_L->data;
      for (i = 0; i < n; i++) {
        for (b_i = 0; b_i < idx; b_i++) {
          nfb = (int32_T)Lmax_data[i];
          if ((nfb < 1) || (nfb > nleft)) {
            emlrtDynamicBoundsCheckR2012b(nfb, 1, nleft, &yb_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          b_L_data[b_i + b_L->size[0] * i] =
              L_data[b_i + L->size[0] * (nfb - 1)];
        }
      }
      nleft = b_L->size[0];
      nfb = L->size[0] * L->size[1];
      L->size[0] = b_L->size[0];
      emxEnsureCapacity_real_T(sp, L, nfb, &ud_emlrtRTEI);
      idx = b_L->size[1];
      nfb = L->size[0] * L->size[1];
      L->size[1] = b_L->size[1];
      emxEnsureCapacity_real_T(sp, L, nfb, &ud_emlrtRTEI);
      L_data = L->data;
      for (i = 0; i < idx; i++) {
        for (b_i = 0; b_i < nleft; b_i++) {
          L_data[b_i + L->size[0] * i] = b_L_data[b_i + b_L->size[0] * i];
        }
      }
    }
    idx = Z->size[1];
    nfb = b_N->size[0] * b_N->size[1];
    b_N->size[0] = 1;
    b_N->size[1] = ii->size[1];
    emxEnsureCapacity_real_T(sp, b_N, nfb, &qd_emlrtRTEI);
    b_L_data = b_N->data;
    for (i = 0; i < n; i++) {
      nfb = (int32_T)Lmax_data[i];
      if ((nfb < 1) || (nfb > idx)) {
        emlrtDynamicBoundsCheckR2012b(nfb, 1, idx, &xb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      b_L_data[i] = Z_data[nfb - 1];
    }
    nfb = Z->size[0] * Z->size[1];
    Z->size[0] = 1;
    emxEnsureCapacity_real_T(sp, Z, nfb, &td_emlrtRTEI);
    idx = b_N->size[1];
    nfb = Z->size[0] * Z->size[1];
    Z->size[1] = b_N->size[1];
    emxEnsureCapacity_real_T(sp, Z, nfb, &td_emlrtRTEI);
    Z_data = Z->data;
    for (i = 0; i < idx; i++) {
      Z_data[i] = b_L_data[i];
    }
    idx = N->size[1];
    nfb = b_N->size[0] * b_N->size[1];
    b_N->size[0] = 1;
    b_N->size[1] = ii->size[1];
    emxEnsureCapacity_real_T(sp, b_N, nfb, &wd_emlrtRTEI);
    b_L_data = b_N->data;
    for (i = 0; i < n; i++) {
      nfb = (int32_T)Lmax_data[i];
      if ((nfb < 1) || (nfb > idx)) {
        emlrtDynamicBoundsCheckR2012b(nfb, 1, idx, &ac_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      b_L_data[i] = N_data[nfb - 1];
    }
    nfb = N->size[0] * N->size[1];
    N->size[0] = 1;
    emxEnsureCapacity_real_T(sp, N, nfb, &xd_emlrtRTEI);
    idx = b_N->size[1];
    nfb = N->size[0] * N->size[1];
    N->size[1] = b_N->size[1];
    emxEnsureCapacity_real_T(sp, N, nfb, &xd_emlrtRTEI);
    N_data = N->data;
    for (i = 0; i < idx; i++) {
      N_data[i] = b_L_data[i];
    }
  }
  /*  ensure zero think time classes are anyway frist */
  st.site = &nb_emlrtRSI;
  nfb = b_Lmax->size[0] * b_Lmax->size[1];
  b_Lmax->size[0] = 1;
  b_loop_ub = Z->size[1];
  b_Lmax->size[1] = b_loop_ub;
  emxEnsureCapacity_boolean_T(&st, b_Lmax, nfb, &od_emlrtRTEI);
  b_Lmax_data = b_Lmax->data;
  if (b_loop_ub < 1600) {
    for (i2 = 0; i2 < b_loop_ub; i2++) {
      b_Lmax_data[i2] = (Z_data[i2] < b_atol);
    }
  } else {
    emlrtEnterParallelRegion(&st, omp_in_parallel());
    emlrtPushJmpBuf(&st, &emlrtJBStack);
    pfqn_nc_sanitize_numThreads = emlrtAllocRegionTLSs(
        st.tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_nc_sanitize_numThreads)

    for (i2 = 0; i2 < b_loop_ub; i2++) {
      b_Lmax_data[i2] = (Z_data[i2] < b_atol);
    }
    emlrtPopJmpBuf(&st, &emlrtJBStack);
    emlrtExitParallelRegion(&st, omp_in_parallel());
  }
  b_st.site = &qb_emlrtRSI;
  eml_find(&b_st, b_Lmax, ii);
  ii_data = ii->data;
  emxFree_boolean_T(&st, &b_Lmax);
  nfb = Lmax->size[0] * Lmax->size[1];
  Lmax->size[0] = 1;
  c_loop_ub = ii->size[1];
  Lmax->size[1] = ii->size[1];
  emxEnsureCapacity_real_T(&st, Lmax, nfb, &sd_emlrtRTEI);
  Lmax_data = Lmax->data;
  for (i = 0; i < c_loop_ub; i++) {
    Lmax_data[i] = ii_data[i];
  }
  if (L->size[1] < 1) {
    b_N->size[0] = 1;
    b_N->size[1] = 0;
  } else {
    idx = L->size[1];
    nfb = b_N->size[0] * b_N->size[1];
    b_N->size[0] = 1;
    b_N->size[1] = idx;
    emxEnsureCapacity_real_T(sp, b_N, nfb, &vd_emlrtRTEI);
    b_L_data = b_N->data;
    nleft = idx - 1;
    nfb = (idx / 2) << 1;
    idx = nfb - 2;
    for (i = 0; i <= idx; i += 2) {
      dv[0] = i;
      dv[1] = i + 1;
      r = _mm_loadu_pd(&dv[0]);
      _mm_storeu_pd(&b_L_data[i], _mm_add_pd(_mm_set1_pd(1.0), r));
    }
    for (i = nfb; i <= nleft; i++) {
      b_L_data[i] = (real_T)i + 1.0;
    }
  }
  st.site = &ob_emlrtRSI;
  b_st.site = &he_emlrtRSI;
  emxInit_real_T(&b_st, &nonzerothinktimes, 2, &je_emlrtRTEI);
  c_st.site = &ie_emlrtRSI;
  do_vectors(&c_st, b_N, Lmax, nonzerothinktimes, b_ii);
  lambda_data = nonzerothinktimes->data;
  n = L->size[0];
  nleft = L->size[1];
  i3 = Lmax->size[1] + nonzerothinktimes->size[1];
  nfb = b_ii->size[0];
  b_ii->size[0] = i3;
  emxEnsureCapacity_int32_T(sp, b_ii, nfb, &yd_emlrtRTEI);
  ii_data = b_ii->data;
  idx = ii->size[1];
  emxFree_int32_T(sp, &ii);
  loop_ub = (Lmax->size[1] < 1600);
  if (loop_ub) {
    for (i4 = 0; i4 < c_loop_ub; i4++) {
      if (Lmax_data[i4] != Lmax_data[i4]) {
        emlrtIntegerCheckR2012b(Lmax_data[i4], &j_emlrtDCI, (emlrtConstCTX)sp);
      }
      idx = (int32_T)Lmax_data[i4];
      if ((idx < 1) || (idx > nleft)) {
        emlrtDynamicBoundsCheckR2012b(idx, 1, nleft, &bc_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      ii_data[i4] = idx - 1;
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    pfqn_nc_sanitize_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_nc_sanitize_numThreads) private(         \
        j_st, emlrtJBEnviron, i6) firstprivate(emlrtHadParallelError)
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
      for (i4 = 0; i4 < idx; i4++) {
        if (emlrtHadParallelError) {
          continue;
        }
        if (setjmp(emlrtJBEnviron) == 0) {
          if (Lmax_data[i4] != Lmax_data[i4]) {
            emlrtIntegerCheckR2012b(Lmax_data[i4], &j_emlrtDCI, &j_st);
          }
          i6 = (int32_T)Lmax_data[i4];
          if ((i6 < 1) || (i6 > nleft)) {
            emlrtDynamicBoundsCheckR2012b(i6, 1, nleft, &bc_emlrtBCI, &j_st);
          }
          ii_data[i4] = i6 - 1;
        } else {
          emlrtHadParallelError = true;
        }
      }
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  d_loop_ub = nonzerothinktimes->size[1];
  nx = (nonzerothinktimes->size[1] < 1600);
  if (nx) {
    for (i5 = 0; i5 < d_loop_ub; i5++) {
      if (lambda_data[i5] != (int32_T)muDoubleScalarFloor(lambda_data[i5])) {
        emlrtIntegerCheckR2012b(lambda_data[i5], &j_emlrtDCI,
                                (emlrtConstCTX)sp);
      }
      idx = (int32_T)lambda_data[i5];
      if ((idx < 1) || (idx > nleft)) {
        emlrtDynamicBoundsCheckR2012b(idx, 1, nleft, &bc_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      ii_data[i5 + Lmax->size[1]] = idx - 1;
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    pfqn_nc_sanitize_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_nc_sanitize_numThreads) private(         \
        emlrtJBEnviron, k_st, i7) firstprivate(emlrtHadParallelError)
    {
      if (setjmp(emlrtJBEnviron) == 0) {
        k_st.prev = sp;
        k_st.tls = emlrtAllocTLS((emlrtCTX)sp, omp_get_thread_num());
        k_st.site = NULL;
        emlrtSetJmpBuf(&k_st, &emlrtJBEnviron);
      } else {
        emlrtHadParallelError = true;
      }
#pragma omp for nowait
      for (i5 = 0; i5 < d_loop_ub; i5++) {
        if (emlrtHadParallelError) {
          continue;
        }
        if (setjmp(emlrtJBEnviron) == 0) {
          if (lambda_data[i5] !=
              (int32_T)muDoubleScalarFloor(lambda_data[i5])) {
            emlrtIntegerCheckR2012b(lambda_data[i5], &j_emlrtDCI, &k_st);
          }
          i7 = (int32_T)lambda_data[i5];
          if ((i7 < 1) || (i7 > nleft)) {
            emlrtDynamicBoundsCheckR2012b(i7, 1, nleft, &bc_emlrtBCI, &k_st);
          }
          ii_data[i5 + Lmax->size[1]] = i7 - 1;
        } else {
          emlrtHadParallelError = true;
        }
      }
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  idx = b_L->size[0] * b_L->size[1];
  b_L->size[0] = n;
  nfb = b_ii->size[0];
  b_L->size[1] = b_ii->size[0];
  emxEnsureCapacity_real_T(sp, b_L, idx, &ae_emlrtRTEI);
  b_L_data = b_L->data;
  for (i = 0; i < nfb; i++) {
    for (b_i = 0; b_i < n; b_i++) {
      b_L_data[b_i + b_L->size[0] * i] = L_data[b_i + L->size[0] * ii_data[i]];
    }
  }
  idx = L->size[0] * L->size[1];
  L->size[0] = n;
  L->size[1] = b_ii->size[0];
  emxEnsureCapacity_real_T(sp, L, idx, &be_emlrtRTEI);
  L_data = L->data;
  for (i = 0; i < nfb; i++) {
    for (b_i = 0; b_i < n; b_i++) {
      L_data[b_i + L->size[0] * i] = b_L_data[b_i + b_L->size[0] * i];
    }
  }
  emxFree_real_T(sp, &b_L);
  nfb = N->size[1];
  idx = b_ii->size[0];
  b_ii->size[0] = i3;
  emxEnsureCapacity_int32_T(sp, b_ii, idx, &ce_emlrtRTEI);
  ii_data = b_ii->data;
  if (loop_ub) {
    for (i8 = 0; i8 < c_loop_ub; i8++) {
      if (Lmax_data[i8] != Lmax_data[i8]) {
        emlrtIntegerCheckR2012b(Lmax_data[i8], &k_emlrtDCI, (emlrtConstCTX)sp);
      }
      idx = (int32_T)Lmax_data[i8];
      if ((idx < 1) || (idx > nfb)) {
        emlrtDynamicBoundsCheckR2012b(idx, 1, nfb, &cc_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      ii_data[i8] = idx - 1;
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    pfqn_nc_sanitize_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_nc_sanitize_numThreads) private(         \
        emlrtJBEnviron, l_st, i10) firstprivate(emlrtHadParallelError)
    {
      if (setjmp(emlrtJBEnviron) == 0) {
        l_st.prev = sp;
        l_st.tls = emlrtAllocTLS((emlrtCTX)sp, omp_get_thread_num());
        l_st.site = NULL;
        emlrtSetJmpBuf(&l_st, &emlrtJBEnviron);
      } else {
        emlrtHadParallelError = true;
      }
#pragma omp for nowait
      for (i8 = 0; i8 < c_loop_ub; i8++) {
        if (emlrtHadParallelError) {
          continue;
        }
        if (setjmp(emlrtJBEnviron) == 0) {
          if (Lmax_data[i8] != Lmax_data[i8]) {
            emlrtIntegerCheckR2012b(Lmax_data[i8], &k_emlrtDCI, &l_st);
          }
          i10 = (int32_T)Lmax_data[i8];
          if ((i10 < 1) || (i10 > nfb)) {
            emlrtDynamicBoundsCheckR2012b(i10, 1, nfb, &cc_emlrtBCI, &l_st);
          }
          ii_data[i8] = i10 - 1;
        } else {
          emlrtHadParallelError = true;
        }
      }
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  if (nx) {
    for (i9 = 0; i9 < d_loop_ub; i9++) {
      if (lambda_data[i9] != (int32_T)muDoubleScalarFloor(lambda_data[i9])) {
        emlrtIntegerCheckR2012b(lambda_data[i9], &k_emlrtDCI,
                                (emlrtConstCTX)sp);
      }
      idx = (int32_T)lambda_data[i9];
      if ((idx < 1) || (idx > nfb)) {
        emlrtDynamicBoundsCheckR2012b(idx, 1, nfb, &cc_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      ii_data[i9 + Lmax->size[1]] = idx - 1;
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    pfqn_nc_sanitize_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_nc_sanitize_numThreads) private(         \
        emlrtJBEnviron, m_st, i11) firstprivate(emlrtHadParallelError)
    {
      if (setjmp(emlrtJBEnviron) == 0) {
        m_st.prev = sp;
        m_st.tls = emlrtAllocTLS((emlrtCTX)sp, omp_get_thread_num());
        m_st.site = NULL;
        emlrtSetJmpBuf(&m_st, &emlrtJBEnviron);
      } else {
        emlrtHadParallelError = true;
      }
#pragma omp for nowait
      for (i9 = 0; i9 < d_loop_ub; i9++) {
        if (emlrtHadParallelError) {
          continue;
        }
        if (setjmp(emlrtJBEnviron) == 0) {
          if (lambda_data[i9] !=
              (int32_T)muDoubleScalarFloor(lambda_data[i9])) {
            emlrtIntegerCheckR2012b(lambda_data[i9], &k_emlrtDCI, &m_st);
          }
          i11 = (int32_T)lambda_data[i9];
          if ((i11 < 1) || (i11 > nfb)) {
            emlrtDynamicBoundsCheckR2012b(i11, 1, nfb, &cc_emlrtBCI, &m_st);
          }
          ii_data[i9 + Lmax->size[1]] = i11 - 1;
        } else {
          emlrtHadParallelError = true;
        }
      }
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  idx = b_N->size[0] * b_N->size[1];
  b_N->size[0] = 1;
  nfb = b_ii->size[0];
  b_N->size[1] = b_ii->size[0];
  emxEnsureCapacity_real_T(sp, b_N, idx, &de_emlrtRTEI);
  b_L_data = b_N->data;
  for (i = 0; i < nfb; i++) {
    b_L_data[i] = N_data[ii_data[i]];
  }
  idx = N->size[0] * N->size[1];
  N->size[0] = 1;
  N->size[1] = b_ii->size[0];
  emxEnsureCapacity_real_T(sp, N, idx, &ee_emlrtRTEI);
  N_data = N->data;
  for (i = 0; i < nfb; i++) {
    N_data[i] = b_L_data[i];
  }
  idx = b_ii->size[0];
  b_ii->size[0] = i3;
  emxEnsureCapacity_int32_T(sp, b_ii, idx, &fe_emlrtRTEI);
  ii_data = b_ii->data;
  if (loop_ub) {
    for (i12 = 0; i12 < c_loop_ub; i12++) {
      if (Lmax_data[i12] != Lmax_data[i12]) {
        emlrtIntegerCheckR2012b(Lmax_data[i12], &l_emlrtDCI, (emlrtConstCTX)sp);
      }
      nfb = (int32_T)Lmax_data[i12];
      if ((nfb < 1) || (nfb > b_loop_ub)) {
        emlrtDynamicBoundsCheckR2012b(nfb, 1, b_loop_ub, &dc_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      ii_data[i12] = nfb - 1;
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    pfqn_nc_sanitize_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_nc_sanitize_numThreads) private(         \
        emlrtJBEnviron, n_st, i14) firstprivate(emlrtHadParallelError)
    {
      if (setjmp(emlrtJBEnviron) == 0) {
        n_st.prev = sp;
        n_st.tls = emlrtAllocTLS((emlrtCTX)sp, omp_get_thread_num());
        n_st.site = NULL;
        emlrtSetJmpBuf(&n_st, &emlrtJBEnviron);
      } else {
        emlrtHadParallelError = true;
      }
#pragma omp for nowait
      for (i12 = 0; i12 < c_loop_ub; i12++) {
        if (emlrtHadParallelError) {
          continue;
        }
        if (setjmp(emlrtJBEnviron) == 0) {
          if (Lmax_data[i12] != Lmax_data[i12]) {
            emlrtIntegerCheckR2012b(Lmax_data[i12], &l_emlrtDCI, &n_st);
          }
          i14 = (int32_T)Lmax_data[i12];
          if ((i14 < 1) || (i14 > b_loop_ub)) {
            emlrtDynamicBoundsCheckR2012b(i14, 1, b_loop_ub, &dc_emlrtBCI,
                                          &n_st);
          }
          ii_data[i12] = i14 - 1;
        } else {
          emlrtHadParallelError = true;
        }
      }
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  if (nx) {
    for (i13 = 0; i13 < d_loop_ub; i13++) {
      if (lambda_data[i13] != (int32_T)muDoubleScalarFloor(lambda_data[i13])) {
        emlrtIntegerCheckR2012b(lambda_data[i13], &l_emlrtDCI,
                                (emlrtConstCTX)sp);
      }
      nfb = (int32_T)lambda_data[i13];
      if ((nfb < 1) || (nfb > b_loop_ub)) {
        emlrtDynamicBoundsCheckR2012b(nfb, 1, b_loop_ub, &dc_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      ii_data[i13 + Lmax->size[1]] = nfb - 1;
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    pfqn_nc_sanitize_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_nc_sanitize_numThreads) private(         \
        emlrtJBEnviron, o_st, i15) firstprivate(emlrtHadParallelError)
    {
      if (setjmp(emlrtJBEnviron) == 0) {
        o_st.prev = sp;
        o_st.tls = emlrtAllocTLS((emlrtCTX)sp, omp_get_thread_num());
        o_st.site = NULL;
        emlrtSetJmpBuf(&o_st, &emlrtJBEnviron);
      } else {
        emlrtHadParallelError = true;
      }
#pragma omp for nowait
      for (i13 = 0; i13 < d_loop_ub; i13++) {
        if (emlrtHadParallelError) {
          continue;
        }
        if (setjmp(emlrtJBEnviron) == 0) {
          if (lambda_data[i13] !=
              (int32_T)muDoubleScalarFloor(lambda_data[i13])) {
            emlrtIntegerCheckR2012b(lambda_data[i13], &l_emlrtDCI, &o_st);
          }
          i15 = (int32_T)lambda_data[i13];
          if ((i15 < 1) || (i15 > b_loop_ub)) {
            emlrtDynamicBoundsCheckR2012b(i15, 1, b_loop_ub, &dc_emlrtBCI,
                                          &o_st);
          }
          ii_data[i13 + Lmax->size[1]] = i15 - 1;
        } else {
          emlrtHadParallelError = true;
        }
      }
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  emxFree_real_T(sp, &nonzerothinktimes);
  emxFree_real_T(sp, &Lmax);
  nfb = b_N->size[0] * b_N->size[1];
  b_N->size[0] = 1;
  idx = b_ii->size[0];
  b_N->size[1] = b_ii->size[0];
  emxEnsureCapacity_real_T(sp, b_N, nfb, &ge_emlrtRTEI);
  b_L_data = b_N->data;
  for (i = 0; i < idx; i++) {
    b_L_data[i] = Z_data[ii_data[i]];
  }
  nfb = Z->size[0] * Z->size[1];
  Z->size[0] = 1;
  Z->size[1] = b_ii->size[0];
  emxFree_int32_T(sp, &b_ii);
  emxEnsureCapacity_real_T(sp, Z, nfb, &he_emlrtRTEI);
  Z_data = Z->data;
  for (i = 0; i < idx; i++) {
    Z_data[i] = b_L_data[i];
  }
  emxFree_real_T(sp, &b_N);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return lGremaind;
}

/* End of code generation (pfqn_nc_sanitize.c) */
