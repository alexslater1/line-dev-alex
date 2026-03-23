/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mmint2_data.c
 *
 * Code generation for function 'pfqn_mmint2_data'
 *
 */

/* Include files */
#include "pfqn_mmint2_data.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;

emlrtContext emlrtContextGlobal = {
    true,                                                 /* bFirstTime */
    false,                                                /* bInitialized */
    131674U,                                              /* fVersionInfo */
    NULL,                                                 /* fErrorFunction */
    "pfqn_mmint2",                                        /* fFunctionName */
    NULL,                                                 /* fRTCallStack */
    false,                                                /* bDebugMode */
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, /* fSigWrd */
    NULL                                                  /* fSigMem */
};

emlrtRSInfo j_emlrtRSI = {
    20,                               /* lineNo */
    "eml_int_forloop_overflow_check", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/eml/"
    "eml_int_forloop_overflow_check.m" /* pathName */
};

emlrtRSInfo r_emlrtRSI = {
    63,                               /* lineNo */
    "function_handle/parenReference", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "function_handle.m" /* pathName */
};

emlrtRSInfo v_emlrtRSI = {
    71,                                                           /* lineNo */
    "power",                                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

emlrtRSInfo w_emlrtRSI = {
    81,                                                           /* lineNo */
    "fltpower",                                                   /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

emlrtRSInfo x_emlrtRSI = {
    94,                                                           /* lineNo */
    "fltpower",                                                   /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

emlrtRSInfo y_emlrtRSI = {
    106,                                                          /* lineNo */
    "fltpower_domain_error",                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

emlrtRSInfo ab_emlrtRSI = {
    113,                                                          /* lineNo */
    "fltpower_domain_error",                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

emlrtRSInfo bb_emlrtRSI = {
    123,                                                          /* lineNo */
    "fltpower_domain_error",                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

emlrtRSInfo
    cb_emlrtRSI =
        {
            45,          /* lineNo */
            "vAllOrAny", /* fcnName */
            "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
            "vAllOrAny.m" /* pathName */
};

emlrtRSInfo
    db_emlrtRSI =
        {
            121,                  /* lineNo */
            "flatVectorAllOrAny", /* fcnName */
            "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
            "vAllOrAny.m" /* pathName */
};

emlrtRSInfo eb_emlrtRSI = {
    65,      /* lineNo */
    "ixfun", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/ixfun.m" /* pathName
                                                                         */
};

emlrtRSInfo fb_emlrtRSI = {
    110,                          /* lineNo */
    "binaryImplicitExpansionFun", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/ixfun.m" /* pathName
                                                                         */
};

emlrtRSInfo gb_emlrtRSI = {
    16,                                                         /* lineNo */
    "any",                                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/any.m" /* pathName */
};

emlrtRSInfo hb_emlrtRSI = {
    57,         /* lineNo */
    "allOrAny", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/allOrAny.m" /* pathName
                                                                            */
};

emlrtRSInfo ib_emlrtRSI = {
    106,                   /* lineNo */
    "applyToMultipleDims", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyToMultipleDims.m" /* pathName */
};

emlrtRSInfo lb_emlrtRSI = {
    11,     /* lineNo */
    "prod", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/prod.m" /* pathName
                                                                      */
};

emlrtRSInfo mb_emlrtRSI =
    {
        99,        /* lineNo */
        "sumprod", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
        "sumprod.m" /* pathName */
};

emlrtRSInfo nb_emlrtRSI = {
    149,                     /* lineNo */
    "combineVectorElements", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "combineVectorElements.m" /* pathName */
};

emlrtRSInfo ob_emlrtRSI = {
    209,                /* lineNo */
    "colMajorFlatIter", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "combineVectorElements.m" /* pathName */
};

omp_lock_t emlrtLockGlobal;

omp_nest_lock_t pfqn_mmint2_nestLockGlobal;

emlrtRTEInfo e_emlrtRTEI = {
    82,                                                           /* lineNo */
    5,                                                            /* colNo */
    "fltpower",                                                   /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pName */
};

emlrtRTEInfo bb_emlrtRTEI = {
    71,                                                           /* lineNo */
    5,                                                            /* colNo */
    "power",                                                      /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pName */
};

emlrtRTEInfo cb_emlrtRTEI = {
    65,      /* lineNo */
    5,       /* colNo */
    "ixfun", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/ixfun.m" /* pName
                                                                         */
};

emlrtRTEInfo db_emlrtRTEI = {
    123,                                                          /* lineNo */
    13,                                                           /* colNo */
    "power",                                                      /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pName */
};

/* End of code generation (pfqn_mmint2_data.c) */
