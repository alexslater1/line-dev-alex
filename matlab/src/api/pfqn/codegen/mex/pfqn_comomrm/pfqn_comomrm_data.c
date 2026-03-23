/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_comomrm_data.c
 *
 * Code generation for function 'pfqn_comomrm_data'
 *
 */

/* Include files */
#include "pfqn_comomrm_data.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;

const volatile char_T *emlrtBreakCheckR2012bFlagVar = NULL;

emlrtContext emlrtContextGlobal = {
    true,                                                 /* bFirstTime */
    false,                                                /* bInitialized */
    131674U,                                              /* fVersionInfo */
    NULL,                                                 /* fErrorFunction */
    "pfqn_comomrm",                                       /* fFunctionName */
    NULL,                                                 /* fRTCallStack */
    false,                                                /* bDebugMode */
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, /* fSigWrd */
    NULL                                                  /* fSigMem */
};

emlrtRSInfo rb_emlrtRSI = {
    138,                                                           /* lineNo */
    "eml_find",                                                    /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pathName
                                                                    */
};

emlrtRSInfo sb_emlrtRSI = {
    376,                                                           /* lineNo */
    "find_first_indices",                                          /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pathName
                                                                    */
};

emlrtRSInfo tb_emlrtRSI = {
    20,                               /* lineNo */
    "eml_int_forloop_overflow_check", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/eml/"
    "eml_int_forloop_overflow_check.m" /* pathName */
};

emlrtRSInfo ub_emlrtRSI = {
    20,    /* lineNo */
    "sum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sum.m" /* pathName
                                                                     */
};

emlrtRSInfo vb_emlrtRSI =
    {
        99,        /* lineNo */
        "sumprod", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
        "sumprod.m" /* pathName */
};

emlrtRSInfo wb_emlrtRSI = {
    86,                      /* lineNo */
    "combineVectorElements", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "combineVectorElements.m" /* pathName */
};

emlrtRSInfo xb_emlrtRSI = {
    107,                /* lineNo */
    "blockedSummation", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

emlrtRSInfo yb_emlrtRSI = {
    22,                    /* lineNo */
    "sumMatrixIncludeNaN", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

emlrtRSInfo ac_emlrtRSI = {
    41,                 /* lineNo */
    "sumMatrixColumns", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

emlrtRSInfo bc_emlrtRSI = {
    42,                 /* lineNo */
    "sumMatrixColumns", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

emlrtRSInfo dc_emlrtRSI = {
    57,                 /* lineNo */
    "sumMatrixColumns", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pathName */
};

emlrtRSInfo vc_emlrtRSI = {
    44,                           /* lineNo */
    "applyScalarFunctionInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunctionInPlace.m" /* pathName */
};

emlrtRSInfo wc_emlrtRSI =
    {
        69,                  /* lineNo */
        "eml_mtimes_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pathName */
};

emlrtRSInfo od_emlrtRSI = {
    105,       /* lineNo */
    "sortIdx", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

emlrtRSInfo pd_emlrtRSI = {
    301,                /* lineNo */
    "block_merge_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

emlrtRSInfo qd_emlrtRSI = {
    309,                /* lineNo */
    "block_merge_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

emlrtRSInfo rd_emlrtRSI = {
    310,                /* lineNo */
    "block_merge_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

emlrtRSInfo sd_emlrtRSI = {
    318,                /* lineNo */
    "block_merge_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

emlrtRSInfo td_emlrtRSI = {
    326,                /* lineNo */
    "block_merge_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

emlrtRSInfo ud_emlrtRSI = {
    381,                      /* lineNo */
    "initialize_vector_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

emlrtRSInfo vd_emlrtRSI = {
    409,                      /* lineNo */
    "initialize_vector_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

emlrtRSInfo wd_emlrtRSI = {
    416,                      /* lineNo */
    "initialize_vector_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

emlrtRSInfo xd_emlrtRSI = {
    576,                /* lineNo */
    "merge_pow2_block", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

emlrtRSInfo yd_emlrtRSI = {
    578,                /* lineNo */
    "merge_pow2_block", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

emlrtRSInfo ae_emlrtRSI = {
    606,                /* lineNo */
    "merge_pow2_block", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

omp_lock_t emlrtLockGlobal;

omp_nest_lock_t pfqn_comomrm_nestLockGlobal;

emlrtRTEInfo emlrtRTEI =
    {
        133,                   /* lineNo */
        23,                    /* colNo */
        "dynamic_size_checks", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

emlrtRTEInfo b_emlrtRTEI =
    {
        138,                   /* lineNo */
        23,                    /* colNo */
        "dynamic_size_checks", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

emlrtRTEInfo f_emlrtRTEI = {
    14,                                                           /* lineNo */
    9,                                                            /* colNo */
    "log",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/log.m" /* pName */
};

emlrtRTEInfo g_emlrtRTEI = {
    386,                                                           /* lineNo */
    1,                                                             /* colNo */
    "find_first_indices",                                          /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pName */
};

emlrtRTEInfo j_emlrtRTEI = {
    58,                   /* lineNo */
    23,                   /* colNo */
    "assertValidSizeArg", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "assertValidSizeArg.m" /* pName */
};

emlrtRTEInfo cc_emlrtRTEI = {
    35,                    /* lineNo */
    20,                    /* colNo */
    "sumMatrixIncludeNaN", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "sumMatrixIncludeNaN.m" /* pName */
};

emlrtRTEInfo dc_emlrtRTEI = {
    358,                                                           /* lineNo */
    24,                                                            /* colNo */
    "find",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pName */
};

emlrtRTEInfo pe_emlrtRTEI = {
    296,       /* lineNo */
    1,         /* colNo */
    "sortIdx", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pName
                                                                           */
};

emlrtRTEInfo qe_emlrtRTEI = {
    298,       /* lineNo */
    24,        /* colNo */
    "sortIdx", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pName
                                                                           */
};

emlrtRTEInfo re_emlrtRTEI = {
    298,       /* lineNo */
    1,         /* colNo */
    "sortIdx", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pName
                                                                           */
};

/* End of code generation (pfqn_comomrm_data.c) */
