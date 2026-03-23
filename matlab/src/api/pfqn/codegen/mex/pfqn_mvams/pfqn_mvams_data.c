/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mvams_data.c
 *
 * Code generation for function 'pfqn_mvams_data'
 *
 */

/* Include files */
#include "pfqn_mvams_data.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;

const volatile char_T *emlrtBreakCheckR2012bFlagVar = NULL;

emlrtContext emlrtContextGlobal = {
    true,                                                 /* bFirstTime */
    false,                                                /* bInitialized */
    131674U,                                              /* fVersionInfo */
    NULL,                                                 /* fErrorFunction */
    "pfqn_mvams",                                         /* fFunctionName */
    NULL,                                                 /* fRTCallStack */
    false,                                                /* bDebugMode */
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, /* fSigWrd */
    NULL                                                  /* fSigMem */
};

emlrtRSInfo s_emlrtRSI = {
    37,           /* lineNo */
    "line_error", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/io/line_error.m" /* pathName
                                                                          */
};

emlrtRSInfo u_emlrtRSI =
    {
        99,        /* lineNo */
        "sumprod", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
        "sumprod.m" /* pathName */
};

emlrtRSInfo gb_emlrtRSI = {
    20,                               /* lineNo */
    "eml_int_forloop_overflow_check", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/eml/"
    "eml_int_forloop_overflow_check.m" /* pathName */
};

emlrtRSInfo hb_emlrtRSI = {
    34,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

emlrtRSInfo jb_emlrtRSI = {
    85,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

emlrtRSInfo kb_emlrtRSI =
    {
        34,               /* lineNo */
        "rdivide_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "rdivide_helper.m" /* pathName */
};

emlrtRSInfo lb_emlrtRSI = {
    53,    /* lineNo */
    "div", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/div.m" /* pathName
                                                                       */
};

emlrtRSInfo bc_emlrtRSI = {
    149,                     /* lineNo */
    "combineVectorElements", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "combineVectorElements.m" /* pathName */
};

emlrtRSInfo cc_emlrtRSI = {
    209,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "combineVectorElements.m" /* pathName */
};

emlrtRSInfo mc_emlrtRSI = {
    15,    /* lineNo */
    "max", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/max.m" /* pathName
                                                                     */
};

emlrtRSInfo nc_emlrtRSI = {
    73,         /* lineNo */
    "minOrMax", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

emlrtRSInfo oc_emlrtRSI = {
    108,       /* lineNo */
    "maximum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

emlrtRSInfo ge_emlrtRSI = {
    39,                                                            /* lineNo */
    "find",                                                        /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pathName
                                                                    */
};

emlrtRSInfo je_emlrtRSI = {
    19,        /* lineNo */
    "setdiff", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/setdiff.m" /* pathName
                                                                     */
};

emlrtRSInfo ke_emlrtRSI =
    {
        97,          /* lineNo */
        "eml_setop", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/private/"
        "eml_setop.m" /* pathName */
};

emlrtRSInfo nf_emlrtRSI = {
    80,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

emlrtRSInfo of_emlrtRSI = {
    83,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

emlrtMCInfo emlrtMCI = {
    27,                                                            /* lineNo */
    5,                                                             /* colNo */
    "error",                                                       /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/lang/error.m" /* pName */
};

omp_lock_t emlrtLockGlobal;

omp_nest_lock_t pfqn_mvams_nestLockGlobal;

emlrtRTEInfo emlrtRTEI =
    {
        198,             /* lineNo */
        27,              /* colNo */
        "unaryMinOrMax", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pName */
};

emlrtBCInfo dc_emlrtBCI =
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

emlrtBCInfo fc_emlrtBCI =
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

emlrtRTEInfo e_emlrtRTEI = {
    13,                     /* lineNo */
    27,                     /* colNo */
    "assertCompatibleDims", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/shared/coder/coder/lib/+coder/+internal/"
    "assertCompatibleDims.m" /* pName */
};

emlrtRTEInfo f_emlrtRTEI = {
    14,                                                           /* lineNo */
    9,                                                            /* colNo */
    "log",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/log.m" /* pName */
};

emlrtRTEInfo
    h_emlrtRTEI =
        {
            154,                  /* lineNo */
            9,                    /* colNo */
            "last_nonzero_index", /* fName */
            "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
            "pfqn_mva.m" /* pName */
};

emlrtBCInfo ff_emlrtBCI =
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

emlrtBCInfo ph_emlrtBCI = {
    -1,                                                           /* iFirst */
    -1,                                                           /* iLast */
    9,                                                            /* lineNo */
    16,                                                           /* colNo */
    "N",                                                          /* aName */
    "oner",                                                       /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/oner.m", /* pName */
    0 /* checkKind */
};

emlrtRTEInfo gb_emlrtRTEI = {
    28,                                                           /* lineNo */
    9,                                                            /* colNo */
    "colon",                                                      /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/colon.m" /* pName */
};

emlrtRTEInfo vb_emlrtRTEI = {
    16,                                                             /* lineNo */
    9,                                                              /* colNo */
    "isinf",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/isinf.m" /* pName */
};

emlrtRTEInfo ec_emlrtRTEI = {
    54,    /* lineNo */
    9,     /* colNo */
    "div", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/div.m" /* pName
                                                                       */
};

emlrtRTEInfo hc_emlrtRTEI =
    {
        76,           /* lineNo */
        39,           /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

emlrtRTEInfo kc_emlrtRTEI =
    {
        76,           /* lineNo */
        56,           /* colNo */
        "pfqn_mvamx", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvamx.m" /* pName */
};

emlrtRTEInfo wc_emlrtRTEI = {
    31,                                                            /* lineNo */
    6,                                                             /* colNo */
    "find",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pName */
};

emlrtRTEInfo ed_emlrtRTEI = {
    73,       /* lineNo */
    28,       /* colNo */
    "repmat", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pName */
};

emlrtRTEInfo xe_emlrtRTEI = {
    1,                                                           /* lineNo */
    19,                                                          /* colNo */
    "oner",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/oner.m" /* pName */
};

emlrtRTEInfo xh_emlrtRTEI =
    {
        120,          /* lineNo */
        6,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

emlrtRSInfo ng_emlrtRSI = {
    27,                                                            /* lineNo */
    "error",                                                       /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/lang/error.m" /* pathName
                                                                    */
};

/* End of code generation (pfqn_mvams_data.c) */
