/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mvaldmx_ec.c
 *
 * Code generation for function 'pfqn_mvaldmx_ec'
 *
 */

/* Include files */
#include "pfqn_mvaldmx_ec.h"
#include "eml_int_forloop_overflow_check.h"
#include "find.h"
#include "pfqn_mvams_data.h"
#include "pfqn_mvams_emxutil.h"
#include "pfqn_mvams_types.h"
#include "repmat.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "blas.h"
#include "mwmathutil.h"
#include <emmintrin.h>
#include <stddef.h>

/* Variable Definitions */
static emlrtRSInfo re_emlrtRSI = {
    29,                /* lineNo */
    "pfqn_mvaldmx_ec", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pathName */
};

static emlrtRSInfo se_emlrtRSI = {
    34,                /* lineNo */
    "pfqn_mvaldmx_ec", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pathName */
};

static emlrtRSInfo te_emlrtRSI = {
    37,                /* lineNo */
    "pfqn_mvaldmx_ec", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pathName */
};

static emlrtRSInfo ue_emlrtRSI = {
    55,                /* lineNo */
    "pfqn_mvaldmx_ec", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pathName */
};

static emlrtRSInfo ve_emlrtRSI = {
    79,                /* lineNo */
    "pfqn_mvaldmx_ec", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pathName */
};

static emlrtRSInfo we_emlrtRSI = {
    96,                /* lineNo */
    "pfqn_mvaldmx_ec", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pathName */
};

static emlrtRSInfo xe_emlrtRSI = {
    108,               /* lineNo */
    "pfqn_mvaldmx_ec", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pathName */
};

static emlrtRSInfo ye_emlrtRSI =
    {
        69,                  /* lineNo */
        "eml_mtimes_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pathName */
};

static emlrtRSInfo af_emlrtRSI =
    {
        255,             /* lineNo */
        "unaryMinOrMax", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo bf_emlrtRSI =
    {
        966,                    /* lineNo */
        "maxRealVectorOmitNaN", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo cf_emlrtRSI = {
    73,                      /* lineNo */
    "vectorMinOrMaxInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "vectorMinOrMaxInPlace.m" /* pathName */
};

static emlrtRSInfo df_emlrtRSI = {
    65,                      /* lineNo */
    "vectorMinOrMaxInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "vectorMinOrMaxInPlace.m" /* pathName */
};

static emlrtRSInfo ef_emlrtRSI = {
    131,                        /* lineNo */
    "minOrMaxRealVectorKernel", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "vectorMinOrMaxInPlace.m" /* pathName */
};

static emlrtRSInfo ff_emlrtRSI = {
    44,       /* lineNo */
    "mpower", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/matfun/mpower.m" /* pathName
                                                                       */
};

static emlrtRSInfo gf_emlrtRSI = {
    71,                                                           /* lineNo */
    "power",                                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

static emlrtRTEInfo v_emlrtRTEI =
    {
        133,                   /* lineNo */
        23,                    /* colNo */
        "dynamic_size_checks", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

static emlrtRTEInfo w_emlrtRTEI =
    {
        138,                   /* lineNo */
        23,                    /* colNo */
        "dynamic_size_checks", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

static emlrtBCInfo yk_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    108,               /* lineNo */
    52,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo al_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    108,               /* lineNo */
    40,                /* colNo */
    "F2prime",         /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo bl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    101,               /* lineNo */
    33,                /* colNo */
    "F2prime",         /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo cl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    99,                /* lineNo */
    27,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo dl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    96,                /* lineNo */
    41,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo el_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    96,                /* lineNo */
    30,                /* colNo */
    "F3",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo fl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    89,                /* lineNo */
    28,                /* colNo */
    "F3",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo gl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    86,                /* lineNo */
    32,                /* colNo */
    "F3",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo hl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    84,                /* lineNo */
    28,                /* colNo */
    "F3",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo il_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    85,                /* lineNo */
    32,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo jl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    82,                /* lineNo */
    27,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo kl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    79,                /* lineNo */
    41,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo ll_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    79,                /* lineNo */
    30,                /* colNo */
    "F2",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo ml_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    72,                /* lineNo */
    28,                /* colNo */
    "F2",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo nl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    70,                /* lineNo */
    27,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo ol_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    62,                /* lineNo */
    28,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtRTEInfo x_emlrtRTEI = {
    53,                /* lineNo */
    11,                /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtBCInfo pl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    48,                /* lineNo */
    25,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtDCInfo eb_emlrtDCI = {
    45,                /* lineNo */
    16,                /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    1                    /* checkKind */
};

static emlrtDCInfo fb_emlrtDCI = {
    42,                /* lineNo */
    13,                /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    1                    /* checkKind */
};

static emlrtECInfo n_emlrtECI = {
    -1,                /* nDims */
    37,                /* lineNo */
    1,                 /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtBCInfo ql_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    37,                /* lineNo */
    12,                /* colNo */
    "mu",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo rl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    37,                /* lineNo */
    6,                 /* colNo */
    "mu",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtECInfo o_emlrtECI = {
    -1,                /* nDims */
    34,                /* lineNo */
    5,                 /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtBCInfo sl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    37,                /* lineNo */
    40,                /* colNo */
    "mu",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo tl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    34,                /* lineNo */
    37,                /* colNo */
    "mu",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo ul_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    34,                /* lineNo */
    22,                /* colNo */
    "mu",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo vl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    29,                /* lineNo */
    24,                /* colNo */
    "D",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo wl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    29,                /* lineNo */
    8,                 /* colNo */
    "Lo",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo xl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    34,                /* lineNo */
    7,                 /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo yl_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    54,                /* lineNo */
    19,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo am_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    55,                /* lineNo */
    36,                /* colNo */
    "Lo",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo bm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    55,                /* lineNo */
    43,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo cm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    55,                /* lineNo */
    47,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo dm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    55,                /* lineNo */
    49,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo em_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    61,                /* lineNo */
    37,                /* colNo */
    "Lo",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo fm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    61,                /* lineNo */
    44,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo gm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    61,                /* lineNo */
    48,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo hm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    61,                /* lineNo */
    50,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo im_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    66,                /* lineNo */
    37,                /* colNo */
    "Lo",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo jm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    66,                /* lineNo */
    44,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo km_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    66,                /* lineNo */
    48,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo lm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    66,                /* lineNo */
    50,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo mm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    66,                /* lineNo */
    61,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo nm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    66,                /* lineNo */
    65,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo om_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    66,                /* lineNo */
    67,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo pm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    66,                /* lineNo */
    77,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo qm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    66,                /* lineNo */
    81,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo rm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    66,                /* lineNo */
    89,                /* colNo */
    "E1",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo sm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    66,                /* lineNo */
    20,                /* colNo */
    "E1",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo tm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    146,               /* lineNo */
    23,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo um_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    146,               /* lineNo */
    27,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo vm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    146,               /* lineNo */
    34,                /* colNo */
    "E",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo wm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    146,               /* lineNo */
    38,                /* colNo */
    "E",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo xm_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    146,               /* lineNo */
    47,                /* colNo */
    "E",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo ym_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    146,               /* lineNo */
    51,                /* colNo */
    "E",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo an_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    146,               /* lineNo */
    12,                /* colNo */
    "EC",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo bn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    146,               /* lineNo */
    16,                /* colNo */
    "EC",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo cn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    63,                /* lineNo */
    43,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo dn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    63,                /* lineNo */
    47,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo en_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    63,                /* lineNo */
    54,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo fn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    63,                /* lineNo */
    58,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo gn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    63,                /* lineNo */
    60,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo hn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    74,                /* lineNo */
    51,                /* colNo */
    "Lo",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo in_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    74,                /* lineNo */
    60,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo jn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    74,                /* lineNo */
    64,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo kn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    74,                /* lineNo */
    75,                /* colNo */
    "F2",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo ln_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    74,                /* lineNo */
    79,                /* colNo */
    "F2",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo mn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    74,                /* lineNo */
    24,                /* colNo */
    "F2",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo nn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    74,                /* lineNo */
    28,                /* colNo */
    "F2",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo on_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    72,                /* lineNo */
    24,                /* colNo */
    "F2",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo pn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    55,                /* lineNo */
    15,                /* colNo */
    "E",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo qn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    55,                /* lineNo */
    19,                /* colNo */
    "E",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo rn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    57,                /* lineNo */
    33,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo sn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    57,                /* lineNo */
    37,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo tn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    57,                /* lineNo */
    39,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo un_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    57,                /* lineNo */
    47,                /* colNo */
    "E",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo vn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    57,                /* lineNo */
    51,                /* colNo */
    "E",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo wn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    57,                /* lineNo */
    20,                /* colNo */
    "Eprime",          /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo xn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    57,                /* lineNo */
    24,                /* colNo */
    "Eprime",          /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo yn_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    79,                /* lineNo */
    34,                /* colNo */
    "F2",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo ao_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    79,                /* lineNo */
    16,                /* colNo */
    "E2",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo bo_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    91,                /* lineNo */
    51,                /* colNo */
    "Lo",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo co_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    91,                /* lineNo */
    60,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo do_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    91,                /* lineNo */
    64,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo eo_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    91,                /* lineNo */
    66,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo fo_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    91,                /* lineNo */
    77,                /* colNo */
    "F3",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo go_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    91,                /* lineNo */
    81,                /* colNo */
    "F3",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo ho_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    91,                /* lineNo */
    24,                /* colNo */
    "F3",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo io_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    91,                /* lineNo */
    28,                /* colNo */
    "F3",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo jo_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    89,                /* lineNo */
    38,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo ko_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    89,                /* lineNo */
    42,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo lo_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    89,                /* lineNo */
    44,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo mo_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    89,                /* lineNo */
    54,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo no_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    89,                /* lineNo */
    58,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo oo_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    89,                /* lineNo */
    66,                /* colNo */
    "F3",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo po_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    89,                /* lineNo */
    24,                /* colNo */
    "F3",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo qo_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    96,                /* lineNo */
    34,                /* colNo */
    "F3",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo ro_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    96,                /* lineNo */
    16,                /* colNo */
    "E3",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo so_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    86,                /* lineNo */
    57,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo to_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    86,                /* lineNo */
    61,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo uo_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    86,                /* lineNo */
    68,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo vo_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    86,                /* lineNo */
    72,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo wo_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    86,                /* lineNo */
    74,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo xo_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    103,               /* lineNo */
    56,                /* colNo */
    "Lo",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo yo_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    103,               /* lineNo */
    65,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo ap_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    103,               /* lineNo */
    69,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo bp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    103,               /* lineNo */
    87,                /* colNo */
    "F2prime",         /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo cp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    103,               /* lineNo */
    91,                /* colNo */
    "F2prime",         /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo dp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    103,               /* lineNo */
    29,                /* colNo */
    "F2prime",         /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo ep_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    103,               /* lineNo */
    33,                /* colNo */
    "F2prime",         /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo fp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    101,               /* lineNo */
    43,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo gp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    101,               /* lineNo */
    47,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo hp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    101,               /* lineNo */
    29,                /* colNo */
    "F2prime",         /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo ip_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    108,               /* lineNo */
    44,                /* colNo */
    "F2prime",         /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo jp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    108,               /* lineNo */
    21,                /* colNo */
    "E2prime",         /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo kp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    111,               /* lineNo */
    29,                /* colNo */
    "E1",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo lp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    111,               /* lineNo */
    39,                /* colNo */
    "E2",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo mp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    111,               /* lineNo */
    49,                /* colNo */
    "E3",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo np_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    111,               /* lineNo */
    15,                /* colNo */
    "E",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo op_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    111,               /* lineNo */
    19,                /* colNo */
    "E",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo pp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    112,               /* lineNo */
    20,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo qp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    113,               /* lineNo */
    37,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo rp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    113,               /* lineNo */
    41,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo sp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    113,               /* lineNo */
    43,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo tp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    113,               /* lineNo */
    54,                /* colNo */
    "E1",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo up_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    113,               /* lineNo */
    69,                /* colNo */
    "E2prime",         /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo vp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    113,               /* lineNo */
    78,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo wp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    113,               /* lineNo */
    82,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo xp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    113,               /* lineNo */
    84,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo yp_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    113,               /* lineNo */
    95,                /* colNo */
    "E3",              /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo aq_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    113,               /* lineNo */
    24,                /* colNo */
    "Eprime",          /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo bq_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    113,               /* lineNo */
    28,                /* colNo */
    "Eprime",          /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo cq_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    115,               /* lineNo */
    37,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo dq_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    115,               /* lineNo */
    41,                /* colNo */
    "C",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo eq_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    115,               /* lineNo */
    43,                /* colNo */
    "b",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo fq_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    115,               /* lineNo */
    53,                /* colNo */
    "E",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo gq_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    115,               /* lineNo */
    57,                /* colNo */
    "E",               /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo hq_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    115,               /* lineNo */
    24,                /* colNo */
    "Eprime",          /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtBCInfo iq_emlrtBCI = {
    -1,                /* iFirst */
    -1,                /* iLast */
    115,               /* lineNo */
    28,                /* colNo */
    "Eprime",          /* aName */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m", /* pName */
    0                    /* checkKind */
};

static emlrtRTEInfo ag_emlrtRTEI = {
    40,                /* lineNo */
    6,                 /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo bg_emlrtRTEI = {
    29,                /* lineNo */
    22,                /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo cg_emlrtRTEI = {
    34,                /* lineNo */
    19,                /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo dg_emlrtRTEI = {
    37,                /* lineNo */
    35,                /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo eg_emlrtRTEI = {
    38,                /* lineNo */
    1,                 /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo fg_emlrtRTEI = {
    42,                /* lineNo */
    1,                 /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo gg_emlrtRTEI = {
    43,                /* lineNo */
    1,                 /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo hg_emlrtRTEI = {
    45,                /* lineNo */
    5,                 /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo ig_emlrtRTEI = {
    46,                /* lineNo */
    5,                 /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo jg_emlrtRTEI = {
    47,                /* lineNo */
    5,                 /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo kg_emlrtRTEI = {
    48,                /* lineNo */
    5,                 /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo lg_emlrtRTEI = {
    49,                /* lineNo */
    5,                 /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo mg_emlrtRTEI = {
    51,                /* lineNo */
    5,                 /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo ng_emlrtRTEI = {
    52,                /* lineNo */
    5,                 /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo og_emlrtRTEI = {
    79,                /* lineNo */
    27,                /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo pg_emlrtRTEI = {
    96,                /* lineNo */
    27,                /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo qg_emlrtRTEI = {
    108,               /* lineNo */
    32,                /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo rg_emlrtRTEI = {
    32,                /* lineNo */
    1,                 /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo sg_emlrtRTEI = {
    21,                /* lineNo */
    23,                /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

static emlrtRTEInfo tg_emlrtRTEI = {
    79,                /* lineNo */
    37,                /* colNo */
    "pfqn_mvaldmx_ec", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
    "pfqn_mvaldmx_ec.m" /* pName */
};

/* Function Definitions */
void pfqn_mvaldmx_ec(const emlrtStack *sp, const emxArray_real_T *lambda,
                     const emxArray_real_T *D, emxArray_real_T *mu,
                     emxArray_real_T *EC, emxArray_real_T *E,
                     emxArray_real_T *Eprime)
{
  ptrdiff_t incx_t;
  ptrdiff_t incy_t;
  ptrdiff_t n_t;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack g_st;
  emlrtStack h_st;
  emlrtStack i_st;
  emlrtStack st;
  emxArray_boolean_T *b_mu;
  emxArray_int32_T *b;
  emxArray_real_T *C;
  emxArray_real_T *E2;
  emxArray_real_T *E2prime;
  emxArray_real_T *E3;
  emxArray_real_T *F2;
  emxArray_real_T *F2prime;
  emxArray_real_T *F3;
  emxArray_real_T *Lo;
  emxArray_real_T *b_F2;
  emxArray_real_T *d_mu;
  emxArray_real_T *y;
  const real_T *D_data;
  const real_T *lambda_data;
  real_T c_mu;
  real_T *C_data;
  real_T *E1_data;
  real_T *E2_data;
  real_T *E2prime_data;
  real_T *E3_data;
  real_T *EC_data;
  real_T *Eprime_data;
  real_T *F2_data;
  real_T *F2prime_data;
  real_T *F3_data;
  real_T *Lo_data;
  real_T *b_F2_data;
  real_T *mu_data;
  real_T *y_data;
  int32_T ii_size[2];
  int32_T Nt;
  int32_T b_ist;
  int32_T b_loop_ub;
  int32_T ex;
  int32_T i;
  int32_T ii_data;
  int32_T ist;
  int32_T j;
  int32_T last;
  int32_T loop_ub;
  int32_T n;
  int32_T *b_data;
  boolean_T *b_mu_data;
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
  mu_data = mu->data;
  D_data = D->data;
  lambda_data = lambda->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /* { */
  /*  % @file pfqn_mvaldmx_ec.m */
  /*  % @brief Compute effective capacity terms for MVALDMX solver. */
  /* } */
  /* { */
  /*  % @brief Compute effective capacity terms for MVALDMX solver. */
  /*  % @fn pfqn_mvaldmx_ec(lambda, D, mu) */
  /*  % @param lambda Arrival rate vector. */
  /*  % @param D Service demand matrix. */
  /*  % @param mu Load-dependent rate matrix. */
  /*  % @return EC Effective capacity matrix. */
  /*  % @return E E-function values. */
  /*  % @return Eprime E-prime function values. */
  /*  % @return Lo Open class load vector. */
  /* } */
  /*  [EC,E,EPRIME,LO] = PFQN_MVALDMX_EC(LAMBDA,D,MU) */
  /*  Compute the effective capacity terms in MVALDMX */
  /*  Think times are not handled since this assumes limited load-dependence */
  /* Nt = sum(N(isfinite(N))); */
  loop_ub = mu->size[0];
  emxInit_real_T(sp, &Lo, 1, &sg_emlrtRTEI);
  ii_data = Lo->size[0];
  Lo->size[0] = mu->size[0];
  emxEnsureCapacity_real_T(sp, Lo, ii_data, &ag_emlrtRTEI);
  Lo_data = Lo->data;
  emxInit_real_T(sp, &y, 2, &tg_emlrtRTEI);
  y_data = y->data;
  for (ist = 0; ist < loop_ub; ist++) {
    if (ist + 1 > D->size[0]) {
      emlrtDynamicBoundsCheckR2012b(ist + 1, 1, D->size[0], &vl_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    ii_data = y->size[0] * y->size[1];
    y->size[0] = 1;
    b_loop_ub = D->size[1];
    y->size[1] = D->size[1];
    emxEnsureCapacity_real_T(sp, y, ii_data, &bg_emlrtRTEI);
    y_data = y->data;
    for (j = 0; j < b_loop_ub; j++) {
      y_data[j] = D_data[ist + D->size[0] * j];
    }
    st.site = &re_emlrtRSI;
    b_st.site = &ye_emlrtRSI;
    if (lambda->size[1] != D->size[1]) {
      if ((lambda->size[1] == 1) || (D->size[1] == 1)) {
        emlrtErrorWithMessageIdR2018a(
            &b_st, &v_emlrtRTEI,
            "Coder:toolbox:mtimes_noDynamicScalarExpansion",
            "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
      } else {
        emlrtErrorWithMessageIdR2018a(&b_st, &w_emlrtRTEI, "MATLAB:innerdim",
                                      "MATLAB:innerdim", 0);
      }
    }
    if (lambda->size[1] < 1) {
      if (ist + 1 > Lo->size[0]) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, Lo->size[0], &wl_emlrtBCI,
                                      &st);
      }
      Lo_data[ist] = 0.0;
    } else {
      n_t = (ptrdiff_t)lambda->size[1];
      incx_t = (ptrdiff_t)1;
      incy_t = (ptrdiff_t)1;
      if (ist + 1 > Lo->size[0]) {
        emlrtDynamicBoundsCheckR2012b(ist + 1, 1, Lo->size[0], &wl_emlrtBCI,
                                      &st);
      }
      Lo_data[ist] =
          ddot(&n_t, (real_T *)&lambda_data[0], &incx_t, &y_data[0], &incy_t);
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  /*  limited load dependence level */
  emxInit_int32_T(sp, &b, 1, &rg_emlrtRTEI);
  ii_data = b->size[0];
  b->size[0] = mu->size[0];
  emxEnsureCapacity_int32_T(sp, b, ii_data, &ag_emlrtRTEI);
  b_data = b->data;
  emxInit_boolean_T(sp, &b_mu, &cg_emlrtRTEI);
  for (ist = 0; ist < loop_ub; ist++) {
    st.site = &se_emlrtRSI;
    if (ist + 1 > loop_ub) {
      emlrtDynamicBoundsCheckR2012b(ist + 1, 1, loop_ub, &ul_emlrtBCI, &st);
    }
    b_loop_ub = mu->size[1];
    if (mu->size[1] < 1) {
      emlrtDynamicBoundsCheckR2012b(mu->size[1], 1, mu->size[1], &tl_emlrtBCI,
                                    &st);
    }
    c_mu = mu_data[ist + mu->size[0] * (mu->size[1] - 1)];
    ii_data = b_mu->size[0] * b_mu->size[1];
    b_mu->size[0] = 1;
    b_mu->size[1] = mu->size[1];
    emxEnsureCapacity_boolean_T(&st, b_mu, ii_data, &cg_emlrtRTEI);
    b_mu_data = b_mu->data;
    for (j = 0; j < b_loop_ub; j++) {
      b_mu_data[j] = (mu_data[ist + mu->size[0] * j] == c_mu);
    }
    b_st.site = &ge_emlrtRSI;
    b_eml_find(&b_st, b_mu, (int32_T *)&ii_data, ii_size);
    if (ii_size[1] != 1) {
      emlrtSubAssignSizeCheck1dR2017a(1, 0, &o_emlrtECI, (emlrtConstCTX)sp);
    }
    if (ist + 1 > b->size[0]) {
      emlrtDynamicBoundsCheckR2012b(ist + 1, 1, b->size[0], &xl_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    b_data[ist] = ii_data;
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  emxFree_boolean_T(sp, &b_mu);
  Nt = mu->size[1] + 1;
  /*  compute extra elements if present */
  st.site = &te_emlrtRSI;
  b_st.site = &mc_emlrtRSI;
  c_st.site = &nc_emlrtRSI;
  d_st.site = &oc_emlrtRSI;
  if (b->size[0] < 1) {
    emlrtErrorWithMessageIdR2018a(&d_st, &emlrtRTEI,
                                  "Coder:toolbox:eml_min_or_max_varDimZero",
                                  "Coder:toolbox:eml_min_or_max_varDimZero", 0);
  }
  e_st.site = &af_emlrtRSI;
  f_st.site = &bf_emlrtRSI;
  last = b->size[0];
  if (b->size[0] <= 2) {
    if (b->size[0] == 1) {
      ii_data = b_data[0];
    } else if (b_data[0] < b_data[1]) {
      ii_data = b_data[1];
    } else {
      ii_data = b_data[0];
    }
  } else {
    g_st.site = &df_emlrtRSI;
    g_st.site = &cf_emlrtRSI;
    ii_data = b_data[0];
    h_st.site = &ef_emlrtRSI;
    if (b->size[0] > 2147483646) {
      i_st.site = &gb_emlrtRSI;
      check_forloop_overflow_error(&i_st);
    }
    for (ist = 2; ist <= last; ist++) {
      b_loop_ub = b_data[ist - 1];
      if (ii_data < b_loop_ub) {
        ii_data = b_loop_ub;
      }
    }
  }
  c_mu = ((real_T)mu->size[1] + 1.0) + (real_T)ii_data;
  if ((real_T)mu->size[1] + 1.0 > c_mu) {
    i = 0;
    b_loop_ub = 0;
  } else {
    if (((int32_T)((uint32_T)mu->size[1] + 1U) < 1) ||
        ((int32_T)((uint32_T)mu->size[1] + 1U) > mu->size[1])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)mu->size[1] + 1U), 1,
                                    mu->size[1], &rl_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    i = mu->size[1];
    if (((int32_T)c_mu < 1) || ((int32_T)c_mu > mu->size[1])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)c_mu, 1, mu->size[1], &ql_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    b_loop_ub = (int32_T)c_mu;
  }
  st.site = &te_emlrtRSI;
  b_st.site = &mc_emlrtRSI;
  c_st.site = &nc_emlrtRSI;
  d_st.site = &oc_emlrtRSI;
  e_st.site = &af_emlrtRSI;
  f_st.site = &bf_emlrtRSI;
  if (b->size[0] <= 2) {
    if (b->size[0] == 1) {
      ex = b_data[0];
    } else if (b_data[0] < b_data[1]) {
      ex = b_data[1];
    } else {
      ex = b_data[0];
    }
  } else {
    g_st.site = &df_emlrtRSI;
    g_st.site = &cf_emlrtRSI;
    ex = b_data[0];
    h_st.site = &ef_emlrtRSI;
    for (ist = 2; ist <= last; ist++) {
      ii_data = b_data[ist - 1];
      if (ex < ii_data) {
        ex = ii_data;
      }
    }
  }
  if (mu->size[1] < 1) {
    emlrtDynamicBoundsCheckR2012b(mu->size[1], 1, mu->size[1], &sl_emlrtBCI,
                                  (emlrtConstCTX)sp);
  }
  emxInit_real_T(sp, &d_mu, 1, &dg_emlrtRTEI);
  ii_data = d_mu->size[0];
  d_mu->size[0] = mu->size[0];
  emxEnsureCapacity_real_T(sp, d_mu, ii_data, &dg_emlrtRTEI);
  EC_data = d_mu->data;
  for (ist = 0; ist < loop_ub; ist++) {
    EC_data[ist] = mu_data[ist + mu->size[0] * (mu->size[1] - 1)];
  }
  emxInit_real_T(sp, &C, 2, &eg_emlrtRTEI);
  st.site = &te_emlrtRSI;
  repmat(&st, d_mu, (real_T)ex + 1.0, C);
  C_data = C->data;
  emxFree_real_T(sp, &d_mu);
  ii_size[0] = mu->size[0];
  ii_data = b_loop_ub - i;
  ii_size[1] = ii_data;
  emlrtSubAssignSizeCheckR2012b(&ii_size[0], 2, &C->size[0], 2, &n_emlrtECI,
                                (emlrtCTX)sp);
  ii_size[0] = mu->size[0];
  for (ist = 0; ist < ii_data; ist++) {
    for (j = 0; j < loop_ub; j++) {
      mu_data[j + mu->size[0] * (i + ist)] = C_data[j + ii_size[0] * ist];
    }
  }
  ii_data = C->size[0] * C->size[1];
  C->size[0] = mu->size[0];
  C->size[1] = mu->size[1];
  emxEnsureCapacity_real_T(sp, C, ii_data, &eg_emlrtRTEI);
  C_data = C->data;
  ii_data = mu->size[0] * mu->size[1];
  b_loop_ub = (ii_data / 2) << 1;
  ex = b_loop_ub - 2;
  for (ist = 0; ist <= ex; ist += 2) {
    __m128d r;
    r = _mm_loadu_pd(&mu_data[ist]);
    _mm_storeu_pd(&C_data[ist], _mm_div_pd(_mm_set1_pd(1.0), r));
  }
  for (ist = b_loop_ub; ist < ii_data; ist++) {
    C_data[ist] = 1.0 / mu_data[ist];
  }
  ii_data = EC->size[0] * EC->size[1];
  EC->size[0] = mu->size[0];
  EC->size[1] = mu->size[1];
  emxEnsureCapacity_real_T(sp, EC, ii_data, &ag_emlrtRTEI);
  EC_data = EC->data;
  /* Ever = zeros(M,1+Nt); */
  if ((real_T)mu->size[1] + 1.0 != mu->size[1] + 1) {
    emlrtIntegerCheckR2012b((real_T)mu->size[1] + 1.0, &fb_emlrtDCI,
                            (emlrtConstCTX)sp);
  }
  ii_data = E->size[0] * E->size[1];
  E->size[0] = mu->size[0];
  E->size[1] = mu->size[1] + 1;
  emxEnsureCapacity_real_T(sp, E, ii_data, &fg_emlrtRTEI);
  mu_data = E->data;
  b_loop_ub = mu->size[0] * (mu->size[1] + 1);
  for (j = 0; j < b_loop_ub; j++) {
    mu_data[j] = 0.0;
  }
  ii_data = Eprime->size[0] * Eprime->size[1];
  Eprime->size[0] = mu->size[0];
  Eprime->size[1] = mu->size[1] + 1;
  emxEnsureCapacity_real_T(sp, Eprime, ii_data, &gg_emlrtRTEI);
  Eprime_data = Eprime->data;
  for (j = 0; j < b_loop_ub; j++) {
    Eprime_data[j] = 0.0;
  }
  emxInit_real_T(sp, &mu, 2, &hg_emlrtRTEI);
  emxInit_real_T(sp, &E2, 2, &ig_emlrtRTEI);
  emxInit_real_T(sp, &E3, 2, &jg_emlrtRTEI);
  emxInit_real_T(sp, &F2, 2, &kg_emlrtRTEI);
  emxInit_real_T(sp, &F3, 2, &lg_emlrtRTEI);
  emxInit_real_T(sp, &E2prime, 2, &mg_emlrtRTEI);
  emxInit_real_T(sp, &F2prime, 2, &ng_emlrtRTEI);
  emxInit_real_T(sp, &b_F2, 2, &og_emlrtRTEI);
  for (b_ist = 0; b_ist < loop_ub; b_ist++) {
    if ((real_T)(Nt - 1) + 1.0 != Nt) {
      emlrtIntegerCheckR2012b((real_T)(Nt - 1) + 1.0, &eb_emlrtDCI,
                              (emlrtConstCTX)sp);
    }
    ii_data = mu->size[0] * mu->size[1];
    mu->size[0] = Nt;
    mu->size[1] = Nt;
    emxEnsureCapacity_real_T(sp, mu, ii_data, &hg_emlrtRTEI);
    E1_data = mu->data;
    ex = Nt * Nt;
    for (ist = 0; ist < ex; ist++) {
      E1_data[ist] = 0.0;
    }
    ii_data = E2->size[0] * E2->size[1];
    E2->size[0] = Nt;
    E2->size[1] = Nt;
    emxEnsureCapacity_real_T(sp, E2, ii_data, &ig_emlrtRTEI);
    E2_data = E2->data;
    for (ist = 0; ist < ex; ist++) {
      E2_data[ist] = 0.0;
    }
    ii_data = E3->size[0] * E3->size[1];
    E3->size[0] = Nt;
    E3->size[1] = Nt;
    emxEnsureCapacity_real_T(sp, E3, ii_data, &jg_emlrtRTEI);
    E3_data = E3->data;
    for (ist = 0; ist < ex; ist++) {
      E3_data[ist] = 0.0;
    }
    if (b_ist + 1 > last) {
      emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &pl_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    ii_data = F2->size[0] * F2->size[1];
    F2->size[0] = Nt;
    F2->size[1] = b_data[b_ist] - 1;
    emxEnsureCapacity_real_T(sp, F2, ii_data, &kg_emlrtRTEI);
    F2_data = F2->data;
    b_loop_ub = Nt * (b_data[b_ist] - 1);
    for (ist = 0; ist < b_loop_ub; ist++) {
      F2_data[ist] = 0.0;
    }
    ii_data = F3->size[0] * F3->size[1];
    F3->size[0] = Nt;
    F3->size[1] = b_data[b_ist] - 1;
    emxEnsureCapacity_real_T(sp, F3, ii_data, &lg_emlrtRTEI);
    F3_data = F3->data;
    for (ist = 0; ist < b_loop_ub; ist++) {
      F3_data[ist] = 0.0;
    }
    ii_data = E2prime->size[0] * E2prime->size[1];
    E2prime->size[0] = Nt;
    E2prime->size[1] = Nt;
    emxEnsureCapacity_real_T(sp, E2prime, ii_data, &mg_emlrtRTEI);
    E2prime_data = E2prime->data;
    for (ist = 0; ist < ex; ist++) {
      E2prime_data[ist] = 0.0;
    }
    ii_data = F2prime->size[0] * F2prime->size[1];
    F2prime->size[0] = Nt;
    F2prime->size[1] = b_data[b_ist] - 1;
    emxEnsureCapacity_real_T(sp, F2prime, ii_data, &ng_emlrtRTEI);
    F2prime_data = F2prime->data;
    for (ist = 0; ist < b_loop_ub; ist++) {
      F2prime_data[ist] = 0.0;
    }
    emlrtForLoopVectorCheckR2021a(0.0, 1.0, (real_T)Nt - 1.0, mxDOUBLE_CLASS,
                                  Nt, &x_emlrtRTEI, (emlrtConstCTX)sp);
    for (n = 0; n < Nt; n++) {
      if (b_ist + 1 > last) {
        emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &yl_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      i = b_data[b_ist];
      if (n >= i) {
        st.site = &ue_emlrtRSI;
        if (b_ist + 1 > Lo->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, Lo->size[0], &am_emlrtBCI,
                                        &st);
        }
        if (b_ist + 1 > C->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0], &bm_emlrtBCI,
                                        &st);
        }
        if (b_ist + 1 > last) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &dm_emlrtBCI, &st);
        }
        if ((i < 1) || (i > C->size[1])) {
          emlrtDynamicBoundsCheckR2012b(i, 1, C->size[1], &cm_emlrtBCI, &st);
        }
        b_st.site = &ff_emlrtRSI;
        c_st.site = &gf_emlrtRSI;
        if (b_ist + 1 > E->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, E->size[0], &pn_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if ((int32_T)((uint32_T)n + 1U) > E->size[1]) {
          emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)n + 1U), 1,
                                        E->size[1], &qn_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        mu_data[b_ist + E->size[0] * n] =
            1.0 /
            muDoubleScalarPower(
                1.0 - Lo_data[b_ist] *
                          C_data[b_ist + C->size[0] * (b_data[b_ist] - 1)],
                (real_T)n + 1.0);
        /*             Ever(i,1+n) = 1 / (1-Lo(i)*C(i,b(i)))^(n+1); */
        if (b_ist + 1 > C->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0], &rn_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_ist + 1 > last) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &tn_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (i > C->size[1]) {
          emlrtDynamicBoundsCheckR2012b(i, 1, C->size[1], &sn_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_ist + 1 > E->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, E->size[0], &un_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if ((int32_T)((uint32_T)n + 1U) > E->size[1]) {
          emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)n + 1U), 1,
                                        E->size[1], &vn_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_ist + 1 > Eprime->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, Eprime->size[0],
                                        &wn_emlrtBCI, (emlrtConstCTX)sp);
        }
        if ((int32_T)((uint32_T)n + 1U) > Eprime->size[1]) {
          emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)n + 1U), 1,
                                        Eprime->size[1], &xn_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        Eprime_data[b_ist + Eprime->size[0] * n] =
            C_data[b_ist + C->size[0] * (i - 1)] *
            mu_data[b_ist + E->size[0] * n];
      } else {
        real_T E_tmp;
        uint32_T u;
        /*  n <= b(i)-1 */
        /*             %% compute E1 */
        if (n == 0) {
          if (b_ist + 1 > Lo->size[0]) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, Lo->size[0],
                                          &em_emlrtBCI, (emlrtConstCTX)sp);
          }
          ii_data = C->size[0];
          if (b_ist + 1 > C->size[0]) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0],
                                          &fm_emlrtBCI, (emlrtConstCTX)sp);
          }
          b_loop_ub = C->size[1];
          if (b_ist + 1 > last) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &hm_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (i > C->size[1]) {
            emlrtDynamicBoundsCheckR2012b(i, 1, C->size[1], &gm_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          E1_data[0] = 1.0 / (1.0 - Lo_data[b_ist] *
                                        C_data[b_ist + C->size[0] * (i - 1)]);
          if (b_ist + 1 > last) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &ol_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          ex = b_data[b_ist];
          for (ist = 0; ist <= ex - 2; ist++) {
            if (b_ist + 1 > ii_data) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, ii_data, &cn_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (((int32_T)((uint32_T)ist + 1U) < 1) ||
                ((int32_T)((uint32_T)ist + 1U) > b_loop_ub)) {
              emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)ist + 1U), 1,
                                            b_loop_ub, &dn_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (b_ist + 1 > ii_data) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, ii_data, &en_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (b_ist + 1 > last) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &gn_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (i > b_loop_ub) {
              emlrtDynamicBoundsCheckR2012b(i, 1, b_loop_ub, &fn_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            E1_data[0] = E1_data[0] * C_data[b_ist + C->size[0] * ist] /
                         C_data[b_ist + C->size[0] * (i - 1)];
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b((emlrtConstCTX)sp);
            }
          }
        } else {
          /*  n>0 */
          if (b_ist + 1 > Lo->size[0]) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, Lo->size[0],
                                          &im_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (b_ist + 1 > C->size[0]) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0],
                                          &jm_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (b_ist + 1 > last) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &lm_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (i > C->size[1]) {
            emlrtDynamicBoundsCheckR2012b(i, 1, C->size[1], &km_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (b_ist + 1 > C->size[0]) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0],
                                          &mm_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (b_ist + 1 > last) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &om_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (i > C->size[1]) {
            emlrtDynamicBoundsCheckR2012b(i, 1, C->size[1], &nm_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (b_ist + 1 > C->size[0]) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0],
                                          &pm_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (n > C->size[1]) {
            emlrtDynamicBoundsCheckR2012b(n, 1, C->size[1], &qm_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          ii_data = mu->size[0] * mu->size[1];
          if (n > ii_data) {
            emlrtDynamicBoundsCheckR2012b(n, 1, ii_data, &rm_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (n + 1 > ii_data) {
            emlrtDynamicBoundsCheckR2012b(n + 1, 1, ii_data, &sm_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          E1_data[n] =
              1.0 /
              (1.0 - Lo_data[b_ist] * C_data[b_ist + C->size[0] * (i - 1)]) *
              C_data[b_ist + C->size[0] * (i - 1)] /
              C_data[b_ist + C->size[0] * (n - 1)] * E1_data[n - 1];
        }
        /*             %% compute F2 */
        if (b_ist + 1 > last) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &nl_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        ii_data = b_data[b_ist];
        for (ist = 0; ist <= ii_data - 2; ist++) {
          if (ist == 0) {
            if (F2->size[1] < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, F2->size[1], &ml_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (n + 1 > F2->size[0]) {
              emlrtDynamicBoundsCheckR2012b(n + 1, 1, F2->size[0], &on_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            F2_data[n] = 1.0;
          } else {
            if (b_ist + 1 > Lo->size[0]) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, Lo->size[0],
                                            &hn_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (b_ist + 1 > C->size[0]) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0],
                                            &in_emlrtBCI, (emlrtConstCTX)sp);
            }
            u = (uint32_T)n + (uint32_T)ist;
            if (((int32_T)u < 1) || ((int32_T)u > C->size[1])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)u, 1, C->size[1],
                                            &jn_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (n + 1 > F2->size[0]) {
              emlrtDynamicBoundsCheckR2012b(n + 1, 1, F2->size[0], &kn_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (ist > F2->size[1]) {
              emlrtDynamicBoundsCheckR2012b(ist, 1, F2->size[1], &ln_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (n + 1 > F2->size[0]) {
              emlrtDynamicBoundsCheckR2012b(n + 1, 1, F2->size[0], &mn_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if ((int32_T)((uint32_T)ist + 1U) > F2->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)ist + 1U), 1,
                                            F2->size[1], &nn_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            F2_data[n + F2->size[0] * ist] =
                (real_T)u / (real_T)ist * Lo_data[b_ist] *
                C_data[b_ist + C->size[0] * ((int32_T)u - 1)] *
                F2_data[n + F2->size[0] * (ist - 1)];
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b((emlrtConstCTX)sp);
          }
        }
        /*             %% compute E2 */
        if (b_ist + 1 > last) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &kl_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        c_mu = (real_T)b_data[b_ist] - 2.0;
        if (c_mu < 0.0) {
          y->size[0] = 1;
          y->size[1] = 0;
        } else {
          ii_data = y->size[0] * y->size[1];
          y->size[0] = 1;
          y->size[1] = b_data[b_ist] - 1;
          emxEnsureCapacity_real_T(sp, y, ii_data, &gb_emlrtRTEI);
          y_data = y->data;
          ii_data = b_data[b_ist] - 2;
          for (ist = 0; ist <= ii_data; ist++) {
            y_data[ist] = ist;
          }
        }
        if (n + 1 > F2->size[0]) {
          emlrtDynamicBoundsCheckR2012b(n + 1, 1, F2->size[0], &ll_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        ii_data = b_F2->size[0] * b_F2->size[1];
        b_F2->size[0] = 1;
        b_loop_ub = y->size[1];
        b_F2->size[1] = y->size[1];
        emxEnsureCapacity_real_T(sp, b_F2, ii_data, &og_emlrtRTEI);
        b_F2_data = b_F2->data;
        for (ist = 0; ist < b_loop_ub; ist++) {
          ii_data = (int32_T)y_data[ist] + 1;
          if ((ii_data < 1) || (ii_data > F2->size[1])) {
            emlrtDynamicBoundsCheckR2012b(ii_data, 1, F2->size[1], &yn_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          b_F2_data[ist] = F2_data[n + F2->size[0] * (ii_data - 1)];
        }
        ii_data = E2->size[0] * E2->size[1];
        if (n + 1 > ii_data) {
          emlrtDynamicBoundsCheckR2012b(n + 1, 1, ii_data, &ao_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        st.site = &ve_emlrtRSI;
        E2_data[n] = b_sum(&st, b_F2);
        /*             %% compute F3 */
        if (b_ist + 1 > last) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &jl_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        ii_data = b_data[b_ist];
        for (ist = 0; ist <= ii_data - 2; ist++) {
          if ((n == 0) && (ist == 0)) {
            if (F3->size[1] < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, F3->size[1], &hl_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            F3_data[0] = 1.0;
            if (b_ist + 1 > last) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &il_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            b_loop_ub = b_data[b_ist];
            for (j = 0; j <= b_loop_ub - 2; j++) {
              if (F3->size[1] < 1) {
                emlrtDynamicBoundsCheckR2012b(1, 1, F3->size[1], &gl_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (b_ist + 1 > C->size[0]) {
                emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0],
                                              &so_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (((int32_T)((uint32_T)j + 1U) < 1) ||
                  ((int32_T)((uint32_T)j + 1U) > C->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)j + 1U), 1,
                                              C->size[1], &to_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (b_ist + 1 > C->size[0]) {
                emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0],
                                              &uo_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (b_ist + 1 > last) {
                emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &wo_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (i > C->size[1]) {
                emlrtDynamicBoundsCheckR2012b(i, 1, C->size[1], &vo_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              F3_data[0] = F3_data[0] * C_data[b_ist + C->size[0] * j] /
                           C_data[b_ist + C->size[0] * (i - 1)];
              if (*emlrtBreakCheckR2012bFlagVar != 0) {
                emlrtBreakCheckR2012b((emlrtConstCTX)sp);
              }
            }
          } else if ((n > 0) && (ist == 0)) {
            if (F3->size[1] < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, F3->size[1], &fl_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (b_ist + 1 > C->size[0]) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0],
                                            &jo_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (b_ist + 1 > last) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &lo_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (i > C->size[1]) {
              emlrtDynamicBoundsCheckR2012b(i, 1, C->size[1], &ko_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (b_ist + 1 > C->size[0]) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0],
                                            &mo_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (n > C->size[1]) {
              emlrtDynamicBoundsCheckR2012b(n, 1, C->size[1], &no_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (n > F3->size[0]) {
              emlrtDynamicBoundsCheckR2012b(n, 1, F3->size[0], &oo_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (n + 1 > F3->size[0]) {
              emlrtDynamicBoundsCheckR2012b(n + 1, 1, F3->size[0], &po_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            F3_data[n] = C_data[b_ist + C->size[0] * (i - 1)] /
                         C_data[b_ist + C->size[0] * (n - 1)] * F3_data[n - 1];
          } else {
            if (b_ist + 1 > Lo->size[0]) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, Lo->size[0],
                                            &bo_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (b_ist + 1 > C->size[0]) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0],
                                            &co_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (b_ist + 1 > last) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &eo_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (i > C->size[1]) {
              emlrtDynamicBoundsCheckR2012b(i, 1, C->size[1], &do_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (n + 1 > F3->size[0]) {
              emlrtDynamicBoundsCheckR2012b(n + 1, 1, F3->size[0], &fo_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if ((ist < 1) || (ist > F3->size[1])) {
              emlrtDynamicBoundsCheckR2012b(ist, 1, F3->size[1], &go_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (n + 1 > F3->size[0]) {
              emlrtDynamicBoundsCheckR2012b(n + 1, 1, F3->size[0], &ho_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if ((int32_T)((uint32_T)ist + 1U) > F3->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)ist + 1U), 1,
                                            F3->size[1], &io_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            F3_data[n + F3->size[0] * ist] =
                (real_T)((uint32_T)n + (uint32_T)ist) / (real_T)ist *
                Lo_data[b_ist] * C_data[b_ist + C->size[0] * (i - 1)] *
                F3_data[n + F3->size[0] * (ist - 1)];
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b((emlrtConstCTX)sp);
          }
        }
        /*             %% compute E3 */
        if (b_ist + 1 > last) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &dl_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (c_mu < 0.0) {
          y->size[0] = 1;
          y->size[1] = 0;
        } else {
          ii_data = y->size[0] * y->size[1];
          y->size[0] = 1;
          y->size[1] = b_data[b_ist] - 1;
          emxEnsureCapacity_real_T(sp, y, ii_data, &gb_emlrtRTEI);
          y_data = y->data;
          ii_data = b_data[b_ist] - 2;
          for (ist = 0; ist <= ii_data; ist++) {
            y_data[ist] = ist;
          }
        }
        if (n + 1 > F3->size[0]) {
          emlrtDynamicBoundsCheckR2012b(n + 1, 1, F3->size[0], &el_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        ii_data = b_F2->size[0] * b_F2->size[1];
        b_F2->size[0] = 1;
        b_loop_ub = y->size[1];
        b_F2->size[1] = y->size[1];
        emxEnsureCapacity_real_T(sp, b_F2, ii_data, &pg_emlrtRTEI);
        b_F2_data = b_F2->data;
        for (ist = 0; ist < b_loop_ub; ist++) {
          ii_data = (int32_T)y_data[ist] + 1;
          if ((ii_data < 1) || (ii_data > F3->size[1])) {
            emlrtDynamicBoundsCheckR2012b(ii_data, 1, F3->size[1], &qo_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          b_F2_data[ist] = F3_data[n + F3->size[0] * (ii_data - 1)];
        }
        ii_data = E3->size[0] * E3->size[1];
        if (n + 1 > ii_data) {
          emlrtDynamicBoundsCheckR2012b(n + 1, 1, ii_data, &ro_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        st.site = &we_emlrtRSI;
        E3_data[n] = b_sum(&st, b_F2);
        /*             %% compute F2prime */
        if (b_ist + 1 > last) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &cl_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        ii_data = b_data[b_ist];
        for (ist = 0; ist <= ii_data - 2; ist++) {
          if (ist == 0) {
            if (F2prime->size[1] < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, F2prime->size[1],
                                            &bl_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (b_ist + 1 > C->size[0]) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0],
                                            &fp_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (n + 1 > C->size[1]) {
              emlrtDynamicBoundsCheckR2012b(n + 1, 1, C->size[1], &gp_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (n + 1 > F2prime->size[0]) {
              emlrtDynamicBoundsCheckR2012b(n + 1, 1, F2prime->size[0],
                                            &hp_emlrtBCI, (emlrtConstCTX)sp);
            }
            F2prime_data[n] = C_data[b_ist + C->size[0] * n];
          } else {
            if (b_ist + 1 > Lo->size[0]) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, Lo->size[0],
                                            &xo_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (b_ist + 1 > C->size[0]) {
              emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0],
                                            &yo_emlrtBCI, (emlrtConstCTX)sp);
            }
            u = (uint32_T)n + (uint32_T)ist;
            if (((int32_T)(u + 1U) < 1) || ((int32_T)(u + 1U) > C->size[1])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)(u + 1U), 1, C->size[1],
                                            &ap_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (n + 1 > F2prime->size[0]) {
              emlrtDynamicBoundsCheckR2012b(n + 1, 1, F2prime->size[0],
                                            &bp_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (ist > F2prime->size[1]) {
              emlrtDynamicBoundsCheckR2012b(ist, 1, F2prime->size[1],
                                            &cp_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (n + 1 > F2prime->size[0]) {
              emlrtDynamicBoundsCheckR2012b(n + 1, 1, F2prime->size[0],
                                            &dp_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((int32_T)((uint32_T)ist + 1U) > F2prime->size[1]) {
              emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)ist + 1U), 1,
                                            F2prime->size[1], &ep_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            F2prime_data[n + F2prime->size[0] * ist] =
                (real_T)u / (real_T)ist * Lo_data[b_ist] *
                C_data[b_ist + C->size[0] * (int32_T)u] *
                F2prime_data[n + F2prime->size[0] * (ist - 1)];
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b((emlrtConstCTX)sp);
          }
        }
        /*             %% compute E2prime */
        if (b_ist + 1 > last) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &yk_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (c_mu < 0.0) {
          y->size[0] = 1;
          y->size[1] = 0;
        } else {
          ii_data = y->size[0] * y->size[1];
          y->size[0] = 1;
          y->size[1] = b_data[b_ist] - 1;
          emxEnsureCapacity_real_T(sp, y, ii_data, &gb_emlrtRTEI);
          y_data = y->data;
          ii_data = b_data[b_ist] - 2;
          for (ist = 0; ist <= ii_data; ist++) {
            y_data[ist] = ist;
          }
        }
        if (n + 1 > F2prime->size[0]) {
          emlrtDynamicBoundsCheckR2012b(n + 1, 1, F2prime->size[0],
                                        &al_emlrtBCI, (emlrtConstCTX)sp);
        }
        ii_data = b_F2->size[0] * b_F2->size[1];
        b_F2->size[0] = 1;
        b_loop_ub = y->size[1];
        b_F2->size[1] = y->size[1];
        emxEnsureCapacity_real_T(sp, b_F2, ii_data, &qg_emlrtRTEI);
        b_F2_data = b_F2->data;
        for (ist = 0; ist < b_loop_ub; ist++) {
          ii_data = (int32_T)y_data[ist] + 1;
          if ((ii_data < 1) || (ii_data > F2prime->size[1])) {
            emlrtDynamicBoundsCheckR2012b(ii_data, 1, F2prime->size[1],
                                          &ip_emlrtBCI, (emlrtConstCTX)sp);
          }
          b_F2_data[ist] = F2prime_data[n + F2prime->size[0] * (ii_data - 1)];
        }
        ii_data = E2prime->size[0] * E2prime->size[1];
        if (n + 1 > ii_data) {
          emlrtDynamicBoundsCheckR2012b(n + 1, 1, ii_data, &jp_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        st.site = &xe_emlrtRSI;
        E2prime_data[n] = b_sum(&st, b_F2);
        /*  finally, compute E, Eprime, and EC */
        b_loop_ub = mu->size[0] * mu->size[1];
        if (n + 1 > b_loop_ub) {
          emlrtDynamicBoundsCheckR2012b(n + 1, 1, b_loop_ub, &kp_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        ii_data = E2->size[0] * E2->size[1];
        if (n + 1 > ii_data) {
          emlrtDynamicBoundsCheckR2012b(n + 1, 1, ii_data, &lp_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        ex = E3->size[0] * E3->size[1];
        if (n + 1 > ex) {
          emlrtDynamicBoundsCheckR2012b(n + 1, 1, ex, &mp_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (b_ist + 1 > E->size[0]) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, E->size[0], &np_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (n + 1 > E->size[1]) {
          emlrtDynamicBoundsCheckR2012b(n + 1, 1, E->size[1], &op_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        c_mu = E1_data[n];
        E_tmp = E3_data[n];
        mu_data[b_ist + E->size[0] * n] = (c_mu + E2_data[n]) - E_tmp;
        if (b_ist + 1 > last) {
          emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &pp_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (n < i - 1) {
          if (b_ist + 1 > C->size[0]) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0],
                                          &qp_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (b_ist + 1 > last) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &sp_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (i > C->size[1]) {
            emlrtDynamicBoundsCheckR2012b(i, 1, C->size[1], &rp_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (n + 1 > b_loop_ub) {
            emlrtDynamicBoundsCheckR2012b(n + 1, 1, b_loop_ub, &tp_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          ii_data = E2prime->size[0] * E2prime->size[1];
          if (n + 1 > ii_data) {
            emlrtDynamicBoundsCheckR2012b(n + 1, 1, ii_data, &up_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (b_ist + 1 > C->size[0]) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0],
                                          &vp_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (b_ist + 1 > last) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &xp_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (i > C->size[1]) {
            emlrtDynamicBoundsCheckR2012b(i, 1, C->size[1], &wp_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (n + 1 > ex) {
            emlrtDynamicBoundsCheckR2012b(n + 1, 1, ex, &yp_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (b_ist + 1 > Eprime->size[0]) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, Eprime->size[0],
                                          &aq_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (n + 1 > Eprime->size[1]) {
            emlrtDynamicBoundsCheckR2012b(n + 1, 1, Eprime->size[1],
                                          &bq_emlrtBCI, (emlrtConstCTX)sp);
          }
          Eprime_data[b_ist + Eprime->size[0] * n] =
              (C_data[b_ist + C->size[0] * (i - 1)] * c_mu + E2prime_data[n]) -
              C_data[b_ist + C->size[0] * (i - 1)] * E_tmp;
        } else {
          /* n>=b(i)-1 */
          if (b_ist + 1 > C->size[0]) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0],
                                          &cq_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (b_ist + 1 > last) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, last, &eq_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (i > C->size[1]) {
            emlrtDynamicBoundsCheckR2012b(i, 1, C->size[1], &dq_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (b_ist + 1 > E->size[0]) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, E->size[0],
                                          &fq_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (n + 1 > E->size[1]) {
            emlrtDynamicBoundsCheckR2012b(n + 1, 1, E->size[1], &gq_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (b_ist + 1 > Eprime->size[0]) {
            emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, Eprime->size[0],
                                          &hq_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (n + 1 > Eprime->size[1]) {
            emlrtDynamicBoundsCheckR2012b(n + 1, 1, Eprime->size[1],
                                          &iq_emlrtBCI, (emlrtConstCTX)sp);
          }
          Eprime_data[b_ist + Eprime->size[0] * n] =
              C_data[b_ist + C->size[0] * (i - 1)] *
              mu_data[b_ist + E->size[0] * n];
        }
        /*             %% verification of E */
        /*             Ever(i,1+n) = C(i,b(i))^(n+1-b(i)) /
         * (1-Lo(i)*C(i,b(i)))^(n+1) * prod(C(i,(n+1):(b(i)-1))); */
        /*             for n0 = 0:(b(i)-2) */
        /*                 Ever(i,1+n) = Ever(i,1+n) + nchoosek(n+n0,n0) *
         * Lo(i)^n0 *
         * (prod(C(i,(n+1):(n+n0)))-C(i,b(i))^(n0+n+1-b(i))*prod(C(i,(n+1):(b(i)-1))));
         */
        /*             end */
      }
      /*          %% verification2 of E */
      /*          Ever(i,1+n) = 0; */
      /*          for n0=0:1000 */
      /*              if n+n0+1>b(i) */
      /*                  C(i,n+n0+1) = 1/mu(i,b(i)); */
      /*              end */
      /*              Ever(i,1+n) = Ever(i,1+n)  +nchoosek(n+n0,n0) * Lo(i)^n0 *
       * prod(C(i,(n+1):(n+n0))); */
      /*          end */
      /*          %% verification of Eprime */
      /*          Eprimever(i,1+n) = 0; */
      /*          for n0=0:1000 */
      /*              if n+n0+1>b(i) */
      /*                  C(i,n+n0+1) = 1/mu(i,b(i)); */
      /*              end */
      /*              Eprimever(i,1+n) = Eprimever(i,1+n)  +nchoosek(n+n0,n0) *
       * Lo(i)^n0 * prod(C(i,(n+1):(n+n0+1))); */
      /*          end */
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    /*     Eprime = Eprimever; */
    /*  EC not defined for n=0 */
    for (j = 0; j <= Nt - 2; j++) {
      /* if n>=b(i) */
      /*     EC(i,n) = C(i,b(i)) / (1-Lo(i)*C(i,b(i))); */
      /* elseif n>0 */
      if (b_ist + 1 > C->size[0]) {
        emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, C->size[0], &tm_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if ((j + 1 < 1) || (j + 1 > C->size[1])) {
        emlrtDynamicBoundsCheckR2012b(j + 1, 1, C->size[1], &um_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (b_ist + 1 > E->size[0]) {
        emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, E->size[0], &vm_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (((int32_T)((uint32_T)j + 2U) < 1) ||
          ((int32_T)((uint32_T)j + 2U) > E->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)j + 2U), 1,
                                      E->size[1], &wm_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (b_ist + 1 > E->size[0]) {
        emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, E->size[0], &xm_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if ((j + 1 < 1) || (j + 1 > E->size[1])) {
        emlrtDynamicBoundsCheckR2012b(j + 1, 1, E->size[1], &ym_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (b_ist + 1 > EC->size[0]) {
        emlrtDynamicBoundsCheckR2012b(b_ist + 1, 1, EC->size[0], &an_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if ((j + 1 < 1) || (j + 1 > EC->size[1])) {
        emlrtDynamicBoundsCheckR2012b(j + 1, 1, EC->size[1], &bn_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      EC_data[b_ist + EC->size[0] * j] = C_data[b_ist + C->size[0] * j] *
                                         mu_data[b_ist + E->size[0] * (j + 1)] /
                                         mu_data[b_ist + E->size[0] * j];
      /* end */
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  emxFree_real_T(sp, &b_F2);
  emxFree_real_T(sp, &y);
  emxFree_real_T(sp, &Lo);
  emxFree_real_T(sp, &F2prime);
  emxFree_real_T(sp, &E2prime);
  emxFree_real_T(sp, &F3);
  emxFree_real_T(sp, &F2);
  emxFree_real_T(sp, &E3);
  emxFree_real_T(sp, &E2);
  emxFree_real_T(sp, &mu);
  emxFree_real_T(sp, &C);
  emxFree_int32_T(sp, &b);
  /* EC */
  /* E */
  /* Ever */
  /* Eprime */
  /* Eprimever */
  /* Eprime = Eprimever; */
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (pfqn_mvaldmx_ec.c) */
