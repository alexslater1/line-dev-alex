/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mvamx_data.c
 *
 * Code generation for function 'pfqn_mvamx_data'
 *
 */

/* Include files */
#include "pfqn_mvamx_data.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;

const volatile char_T *emlrtBreakCheckR2012bFlagVar = NULL;

emlrtContext emlrtContextGlobal = {
    true,                                                 /* bFirstTime */
    false,                                                /* bInitialized */
    131674U,                                              /* fVersionInfo */
    NULL,                                                 /* fErrorFunction */
    "pfqn_mvamx",                                         /* fFunctionName */
    NULL,                                                 /* fRTCallStack */
    false,                                                /* bDebugMode */
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, /* fSigWrd */
    NULL                                                  /* fSigMem */
};

emlrtRSInfo f_emlrtRSI = {
    37,           /* lineNo */
    "line_error", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/io/line_error.m" /* pathName
                                                                          */
};

emlrtRSInfo g_emlrtRSI = {
    20,    /* lineNo */
    "sum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sum.m" /* pathName
                                                                     */
};

emlrtRSInfo h_emlrtRSI =
    {
        99,        /* lineNo */
        "sumprod", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
        "sumprod.m" /* pathName */
};

emlrtRSInfo i_emlrtRSI = {
    86,                      /* lineNo */
    "combineVectorElements", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "combineVectorElements.m" /* pathName */
};

emlrtRSInfo s_emlrtRSI = {
    20,                               /* lineNo */
    "eml_int_forloop_overflow_check", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/eml/"
    "eml_int_forloop_overflow_check.m" /* pathName */
};

emlrtMCInfo emlrtMCI = {
    27,                                                            /* lineNo */
    5,                                                             /* colNo */
    "error",                                                       /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/lang/error.m" /* pName */
};

omp_lock_t emlrtLockGlobal;

omp_nest_lock_t pfqn_mvamx_nestLockGlobal;

emlrtRSInfo yb_emlrtRSI = {
    27,                                                            /* lineNo */
    "error",                                                       /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/lang/error.m" /* pathName
                                                                    */
};

/* End of code generation (pfqn_mvamx_data.c) */
