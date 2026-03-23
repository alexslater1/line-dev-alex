/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_grnmol_data.c
 *
 * Code generation for function 'pfqn_grnmol_data'
 *
 */

/* Include files */
#include "pfqn_grnmol_data.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;

const volatile char_T *emlrtBreakCheckR2012bFlagVar = NULL;

emlrtContext emlrtContextGlobal = {
    true,                                                 /* bFirstTime */
    false,                                                /* bInitialized */
    131674U,                                              /* fVersionInfo */
    NULL,                                                 /* fErrorFunction */
    "pfqn_grnmol",                                        /* fFunctionName */
    NULL,                                                 /* fRTCallStack */
    false,                                                /* bDebugMode */
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, /* fSigWrd */
    NULL                                                  /* fSigMem */
};

emlrtRSInfo h_emlrtRSI =
    {
        99,        /* lineNo */
        "sumprod", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
        "sumprod.m" /* pathName */
};

emlrtRSInfo s_emlrtRSI = {
    20,                               /* lineNo */
    "eml_int_forloop_overflow_check", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/eml/"
    "eml_int_forloop_overflow_check.m" /* pathName */
};

emlrtRSInfo u_emlrtRSI = {
    71,                                                           /* lineNo */
    "power",                                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

omp_lock_t emlrtLockGlobal;

omp_nest_lock_t pfqn_grnmol_nestLockGlobal;

emlrtRTEInfo d_emlrtRTEI = {
    82,                                                           /* lineNo */
    5,                                                            /* colNo */
    "fltpower",                                                   /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pName */
};

/* End of code generation (pfqn_grnmol_data.c) */
