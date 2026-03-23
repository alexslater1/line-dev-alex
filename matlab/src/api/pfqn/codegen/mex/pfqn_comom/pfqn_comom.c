/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_comom.c
 *
 * Code generation for function 'pfqn_comom'
 *
 */

/* Include files */
#include "pfqn_comom.h"
#include "all.h"
#include "applyScalarFunctionInPlace.h"
#include "assertCompatibleDims.h"
#include "bsxfun.h"
#include "div.h"
#include "eml_int_forloop_overflow_check.h"
#include "exp.h"
#include "find.h"
#include "gammaln.h"
#include "log.h"
#include "mldivide.h"
#include "mtimes.h"
#include "multichoose.h"
#include "nchoosek.h"
#include "pfqn_comom_data.h"
#include "pfqn_comom_emxutil.h"
#include "pfqn_comom_types.h"
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

/* Type Definitions */
#ifndef typedef_captured_var
#define typedef_captured_var
typedef struct {
  real_T contents;
} captured_var;
#endif /* typedef_captured_var */

/* Variable Definitions */
static emlrtRSInfo emlrtRSI =
    {
        20,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo b_emlrtRSI =
    {
        27,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo c_emlrtRSI =
    {
        28,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo d_emlrtRSI =
    {
        29,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo e_emlrtRSI =
    {
        35,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo f_emlrtRSI =
    {
        37,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo g_emlrtRSI =
    {
        40,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo h_emlrtRSI =
    {
        41,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo i_emlrtRSI =
    {
        42,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo j_emlrtRSI =
    {
        43,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo k_emlrtRSI =
    {
        44,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo l_emlrtRSI =
    {
        53,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo m_emlrtRSI =
    {
        57,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo n_emlrtRSI =
    {
        58,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo o_emlrtRSI =
    {
        59,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo p_emlrtRSI =
    {
        60,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo q_emlrtRSI =
    {
        61,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo r_emlrtRSI =
    {
        65,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo s_emlrtRSI = {
    7,         /* lineNo */
    "ref/ref", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/ref.m" /* pathName
                                                                       */
};

static emlrtRSInfo t_emlrtRSI = {
    15,    /* lineNo */
    "max", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/max.m" /* pathName
                                                                     */
};

static emlrtRSInfo u_emlrtRSI = {
    73,         /* lineNo */
    "minOrMax", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

static emlrtRSInfo v_emlrtRSI = {
    108,       /* lineNo */
    "maximum", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/minOrMax.m" /* pathName
                                                                            */
};

static emlrtRSInfo w_emlrtRSI =
    {
        290,             /* lineNo */
        "unaryMinOrMax", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo x_emlrtRSI =
    {
        383,                     /* lineNo */
        "unaryMinOrMaxDispatch", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo y_emlrtRSI =
    {
        451,          /* lineNo */
        "minOrMax2D", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo ab_emlrtRSI =
    {
        533,                         /* lineNo */
        "minOrMax2DColumnMajorDim1", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo bb_emlrtRSI =
    {
        531,                         /* lineNo */
        "minOrMax2DColumnMajorDim1", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pathName */
};

static emlrtRSInfo gb_emlrtRSI =
    {
        34,               /* lineNo */
        "rdivide_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "rdivide_helper.m" /* pathName */
};

static emlrtRSInfo hb_emlrtRSI = {
    53,    /* lineNo */
    "div", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/div.m" /* pathName
                                                                       */
};

static emlrtRSInfo mb_emlrtRSI =
    {
        168,                       /* lineNo */
        "pfqn_comom/sortbynnzpos", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo nb_emlrtRSI =
    {
        158,                /* lineNo */
        "pfqn_comom/ginit", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo ob_emlrtRSI =
    {
        148,               /* lineNo */
        "pfqn_comom/hash", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo pb_emlrtRSI =
    {
        150,               /* lineNo */
        "pfqn_comom/hash", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo qb_emlrtRSI = {
    13,         /* lineNo */
    "matchrow", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/matchrow.m" /* pathName
                                                                      */
};

static emlrtRSInfo rb_emlrtRSI = {
    16,         /* lineNo */
    "matchrow", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/matchrow.m" /* pathName
                                                                      */
};

static emlrtRSInfo wb_emlrtRSI = {
    39,                                                            /* lineNo */
    "find",                                                        /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/find.m" /* pathName
                                                                    */
};

static emlrtRSInfo oc_emlrtRSI = {
    8,                                                             /* lineNo */
    "factln",                                                      /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/factln.m" /* pathName
                                                                    */
};

static emlrtRSInfo pc_emlrtRSI = {
    10,        /* lineNo */
    "gammaln", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/gammaln.m" /* pathName
                                                                         */
};

static emlrtRSInfo qc_emlrtRSI = {
    17,                           /* lineNo */
    "applyScalarFunctionInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunctionInPlace.m" /* pathName */
};

static emlrtRSInfo ed_emlrtRSI =
    {
        131,                    /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo fd_emlrtRSI =
    {
        129,                    /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo gd_emlrtRSI =
    {
        106,                    /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo hd_emlrtRSI =
    {
        128,                    /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo id_emlrtRSI =
    {
        127,                    /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo jd_emlrtRSI =
    {
        115,                    /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo kd_emlrtRSI =
    {
        113,                    /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo ld_emlrtRSI =
    {
        112,                    /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo md_emlrtRSI =
    {
        104,                    /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo nd_emlrtRSI =
    {
        102,                    /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo od_emlrtRSI =
    {
        101,                    /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo pd_emlrtRSI =
    {
        124,                    /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo qd_emlrtRSI =
    {
        97,                     /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo rd_emlrtRSI =
    {
        92,                     /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo sd_emlrtRSI =
    {
        88,                     /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo td_emlrtRSI =
    {
        87,                     /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo ud_emlrtRSI =
    {
        85,                     /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo vd_emlrtRSI =
    {
        81,                     /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo wd_emlrtRSI =
    {
        75,                     /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo xd_emlrtRSI =
    {
        71,                     /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo yd_emlrtRSI =
    {
        70,                     /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo ae_emlrtRSI =
    {
        69,                     /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo ee_emlrtRSI =
    {
        94,                  /* lineNo */
        "eml_mtimes_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pathName */
};

static emlrtRSInfo fe_emlrtRSI =
    {
        69,                  /* lineNo */
        "eml_mtimes_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pathName */
};

static emlrtRSInfo ge_emlrtRSI =
    {
        142,      /* lineNo */
        "mtimes", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+blas/"
        "mtimes.m" /* pathName */
};

static emlrtRSInfo he_emlrtRSI =
    {
        177,           /* lineNo */
        "mtimes_blas", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+blas/"
        "mtimes.m" /* pathName */
};

static emlrtRSInfo mf_emlrtRSI = {
    42,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sort.m" /* pathName
                                                                      */
};

static emlrtRSInfo ng_emlrtRSI = {
    107,                /* lineNo */
    "blockedSummation", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/private/"
    "blockedSummation.m" /* pathName */
};

static emlrtRSInfo og_emlrtRSI = {
    19,                                                           /* lineNo */
    "abs",                                                        /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/abs.m" /* pathName */
};

static emlrtRSInfo pg_emlrtRSI = {
    82,                    /* lineNo */
    "applyScalarFunction", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunction.m" /* pathName */
};

static emlrtRTEInfo emlrtRTEI =
    {
        133,                   /* lineNo */
        23,                    /* colNo */
        "dynamic_size_checks", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

static emlrtRTEInfo b_emlrtRTEI =
    {
        138,                   /* lineNo */
        23,                    /* colNo */
        "dynamic_size_checks", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

static emlrtBCInfo emlrtBCI =
    {
        -1,                  /* iFirst */
        -1,                  /* iLast */
        189,                 /* lineNo */
        37,                  /* colNo */
        "i2",                /* aName */
        "pfqn_comom/nnzcmp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo b_emlrtBCI =
    {
        -1,                  /* iFirst */
        -1,                  /* iLast */
        189,                 /* lineNo */
        27,                  /* colNo */
        "i1",                /* aName */
        "pfqn_comom/nnzcmp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo c_emlrtBCI =
    {
        -1,                  /* iFirst */
        -1,                  /* iLast */
        186,                 /* lineNo */
        34,                  /* colNo */
        "i2",                /* aName */
        "pfqn_comom/nnzcmp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo d_emlrtBCI =
    {
        -1,                  /* iFirst */
        -1,                  /* iLast */
        186,                 /* lineNo */
        23,                  /* colNo */
        "i1",                /* aName */
        "pfqn_comom/nnzcmp", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtRTEInfo e_emlrtRTEI =
    {
        157,                /* lineNo */
        15,                 /* colNo */
        "pfqn_comom/ginit", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtECInfo emlrtECI =
    {
        -1,                        /* nDims */
        171,                       /* lineNo */
        21,                        /* colNo */
        "pfqn_comom/sortbynnzpos", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtECInfo b_emlrtECI =
    {
        -1,                        /* nDims */
        170,                       /* lineNo */
        21,                        /* colNo */
        "pfqn_comom/sortbynnzpos", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtBCInfo e_emlrtBCI =
    {
        -1,                        /* iFirst */
        -1,                        /* iLast */
        170,                       /* lineNo */
        23,                        /* colNo */
        "I",                       /* aName */
        "pfqn_comom/sortbynnzpos", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo f_emlrtBCI =
    {
        -1,                        /* iFirst */
        -1,                        /* iLast */
        170,                       /* lineNo */
        30,                        /* colNo */
        "I",                       /* aName */
        "pfqn_comom/sortbynnzpos", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo g_emlrtBCI =
    {
        -1,                        /* iFirst */
        -1,                        /* iLast */
        169,                       /* lineNo */
        25,                        /* colNo */
        "I",                       /* aName */
        "pfqn_comom/sortbynnzpos", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo h_emlrtBCI =
    {
        -1,                        /* iFirst */
        -1,                        /* iLast */
        168,                       /* lineNo */
        36,                        /* colNo */
        "I",                       /* aName */
        "pfqn_comom/sortbynnzpos", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo i_emlrtBCI =
    {
        -1,                        /* iFirst */
        -1,                        /* iLast */
        168,                       /* lineNo */
        29,                        /* colNo */
        "I",                       /* aName */
        "pfqn_comom/sortbynnzpos", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtRTEInfo f_emlrtRTEI =
    {
        198,             /* lineNo */
        27,              /* colNo */
        "unaryMinOrMax", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pName */
};

static emlrtBCInfo j_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        57,           /* lineNo */
        22,           /* colNo */
        "nvec",       /* aName */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo c_emlrtECI =
    {
        2,            /* nDims */
        55,           /* lineNo */
        15,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtECInfo d_emlrtECI =
    {
        1,            /* nDims */
        55,           /* lineNo */
        15,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo g_emlrtRTEI =
    {
        50,           /* lineNo */
        12,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtBCInfo k_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        50,           /* lineNo */
        16,           /* colNo */
        "N",          /* aName */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo emlrtDCI =
    {
        47,           /* lineNo */
        12,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtDCInfo b_emlrtDCI =
    {
        46,           /* lineNo */
        11,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtDCInfo c_emlrtDCI =
    {
        45,           /* lineNo */
        11,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtDCInfo d_emlrtDCI =
    {
        45,           /* lineNo */
        11,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        4               /* checkKind */
};

static emlrtDCInfo e_emlrtDCI =
    {
        43,           /* lineNo */
        15,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtDCInfo f_emlrtDCI =
    {
        43,           /* lineNo */
        15,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        4               /* checkKind */
};

static emlrtBCInfo l_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        36,           /* lineNo */
        6,            /* colNo */
        "Dn",         /* aName */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo m_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        26,           /* lineNo */
        19,           /* colNo */
        "Z",          /* aName */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo n_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        26,           /* lineNo */
        1,            /* colNo */
        "Lmax",       /* aName */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo g_emlrtDCI =
    {
        40,           /* lineNo */
        1,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtDCInfo h_emlrtDCI =
    {
        158,                /* lineNo */
        15,                 /* colNo */
        "pfqn_comom/ginit", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo o_emlrtBCI =
    {
        -1,                 /* iFirst */
        -1,                 /* iLast */
        158,                /* lineNo */
        15,                 /* colNo */
        "g",                /* aName */
        "pfqn_comom/ginit", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo p_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        51,           /* lineNo */
        22,           /* colNo */
        "nvec",       /* aName */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo q_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        65,           /* lineNo */
        10,           /* colNo */
        "h",          /* aName */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo i_emlrtDCI =
    {
        65,           /* lineNo */
        10,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo r_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        60,           /* lineNo */
        15,           /* colNo */
        "scale",      /* aName */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo j_emlrtDCI =
    {
        60,           /* lineNo */
        15,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo s_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        61,           /* lineNo */
        26,           /* colNo */
        "scale",      /* aName */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo e_emlrtECI =
    {
        2,                 /* nDims */
        148,               /* lineNo */
        42,                /* colNo */
        "pfqn_comom/hash", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtECInfo f_emlrtECI =
    {
        2,                 /* nDims */
        150,               /* lineNo */
        30,                /* colNo */
        "pfqn_comom/hash", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtBCInfo u_emlrtBCI = {
    -1,         /* iFirst */
    -1,         /* iLast */
    13,         /* lineNo */
    15,         /* colNo */
    "matrix",   /* aName */
    "matchrow", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/matchrow.m", /* pName
                                                                       */
    0 /* checkKind */
};

static emlrtECInfo g_emlrtECI = {
    2,          /* nDims */
    13,         /* lineNo */
    8,          /* colNo */
    "matchrow", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/matchrow.m" /* pName */
};

static emlrtRTEInfo v_emlrtRTEI =
    {
        139,                  /* lineNo */
        15,                   /* colNo */
        "pfqn_comom/hashnnz", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtDCInfo n_emlrtDCI =
    {
        69,                     /* lineNo */
        17,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        4               /* checkKind */
};

static emlrtDCInfo o_emlrtDCI =
    {
        69,                     /* lineNo */
        17,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtDCInfo p_emlrtDCI =
    {
        70,                     /* lineNo */
        18,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        4               /* checkKind */
};

static emlrtDCInfo q_emlrtDCI =
    {
        70,                     /* lineNo */
        18,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtDCInfo r_emlrtDCI =
    {
        71,                     /* lineNo */
        17,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        4               /* checkKind */
};

static emlrtDCInfo s_emlrtDCI =
    {
        71,                     /* lineNo */
        17,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo v_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        75,                     /* lineNo */
        29,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo w_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        81,                     /* lineNo */
        23,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo x_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        81,                     /* lineNo */
        26,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo t_emlrtDCI =
    {
        81,                     /* lineNo */
        29,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo y_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        81,                     /* lineNo */
        29,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtRTEInfo w_emlrtRTEI =
    {
        83,                     /* lineNo */
        23,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtBCInfo ab_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        85,                     /* lineNo */
        39,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo h_emlrtECI =
    {
        2,                      /* nDims */
        85,                     /* lineNo */
        34,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtBCInfo bb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        87,                     /* lineNo */
        31,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo cb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        87,                     /* lineNo */
        34,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo u_emlrtDCI =
    {
        87,                     /* lineNo */
        39,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo db_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        87,                     /* lineNo */
        39,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo eb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        88,                     /* lineNo */
        43,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo i_emlrtECI =
    {
        2,                      /* nDims */
        88,                     /* lineNo */
        38,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtDCInfo v_emlrtDCI =
    {
        91,                     /* lineNo */
        36,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        4               /* checkKind */
};

static emlrtDCInfo w_emlrtDCI =
    {
        91,                     /* lineNo */
        36,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo fb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        92,                     /* lineNo */
        43,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo j_emlrtECI =
    {
        2,                      /* nDims */
        92,                     /* lineNo */
        38,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtBCInfo gb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        97,                     /* lineNo */
        27,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo hb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        97,                     /* lineNo */
        29,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ib_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        97,                     /* lineNo */
        31,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtRTEInfo x_emlrtRTEI =
    {
        98,                     /* lineNo */
        27,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtBCInfo jb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        111,                    /* lineNo */
        32,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo k_emlrtECI =
    {
        2,                      /* nDims */
        111,                    /* lineNo */
        27,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo y_emlrtRTEI =
    {
        114,                    /* lineNo */
        31,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtBCInfo kb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        124,                    /* lineNo */
        23,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo lb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        124,                    /* lineNo */
        26,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo x_emlrtDCI =
    {
        124,                    /* lineNo */
        29,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo mb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        124,                    /* lineNo */
        29,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo nb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        126,                    /* lineNo */
        24,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo l_emlrtECI =
    {
        2,                      /* nDims */
        126,                    /* lineNo */
        19,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo ab_emlrtRTEI =
    {
        130,                    /* lineNo */
        23,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtBCInfo ob_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        86,                     /* lineNo */
        23,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo pb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        101,                    /* lineNo */
        27,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo qb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        101,                    /* lineNo */
        43,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo m_emlrtECI =
    {
        2,                      /* nDims */
        101,                    /* lineNo */
        38,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtBCInfo rb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        102,                    /* lineNo */
        27,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo sb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        102,                    /* lineNo */
        43,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo n_emlrtECI =
    {
        2,                      /* nDims */
        102,                    /* lineNo */
        38,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtBCInfo tb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        104,                    /* lineNo */
        31,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ub_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        104,                    /* lineNo */
        52,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo o_emlrtECI =
    {
        2,                      /* nDims */
        104,                    /* lineNo */
        47,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtBCInfo vb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        112,                    /* lineNo */
        27,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo wb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        113,                    /* lineNo */
        27,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo xb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        115,                    /* lineNo */
        31,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo yb_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        127,                    /* lineNo */
        19,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ac_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        128,                    /* lineNo */
        20,                     /* colNo */
        "DA",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo bc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        89,                     /* lineNo */
        27,                     /* colNo */
        "B",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo cc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        93,                     /* lineNo */
        27,                     /* colNo */
        "B",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo dc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        106,                    /* lineNo */
        27,                     /* colNo */
        "B",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ec_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        106,                    /* lineNo */
        43,                     /* colNo */
        "Dn",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo p_emlrtECI =
    {
        2,                      /* nDims */
        106,                    /* lineNo */
        38,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtBCInfo fc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        117,                    /* lineNo */
        27,                     /* colNo */
        "B",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo gc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        129,                    /* lineNo */
        19,                     /* colNo */
        "B",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo hc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        131,                    /* lineNo */
        23,                     /* colNo */
        "B",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ic_emlrtBCI =
    {
        -1,                   /* iFirst */
        -1,                   /* iLast */
        140,                  /* lineNo */
        19,                   /* colNo */
        "dn",                 /* aName */
        "pfqn_comom/hashnnz", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo y_emlrtDCI =
    {
        86,                     /* lineNo */
        27,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo jc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        86,                     /* lineNo */
        27,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo ab_emlrtDCI =
    {
        127,                    /* lineNo */
        23,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo kc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        127,                    /* lineNo */
        23,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo bb_emlrtDCI =
    {
        101,                    /* lineNo */
        31,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo lc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        101,                    /* lineNo */
        31,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo mc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        127,                    /* lineNo */
        17,                     /* colNo */
        "n",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo cb_emlrtDCI =
    {
        128,                    /* lineNo */
        24,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo nc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        128,                    /* lineNo */
        24,                     /* colNo */
        "DA",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo db_emlrtDCI =
    {
        112,                    /* lineNo */
        31,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo oc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        112,                    /* lineNo */
        31,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo pc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        112,                    /* lineNo */
        25,                     /* colNo */
        "n",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo eb_emlrtDCI =
    {
        129,                    /* lineNo */
        23,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo qc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        129,                    /* lineNo */
        23,                     /* colNo */
        "B",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo fb_emlrtDCI =
    {
        102,                    /* lineNo */
        31,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo rc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        102,                    /* lineNo */
        31,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo sc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        129,                    /* lineNo */
        17,                     /* colNo */
        "Z",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo gb_emlrtDCI =
    {
        89,                     /* lineNo */
        31,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo tc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        89,                     /* lineNo */
        31,                     /* colNo */
        "B",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo uc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        91,                     /* lineNo */
        43,                     /* colNo */
        "er",                   /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo hb_emlrtDCI =
    {
        131,                    /* lineNo */
        27,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo vc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        131,                    /* lineNo */
        27,                     /* colNo */
        "B",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo wc_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        131,                    /* lineNo */
        21,                     /* colNo */
        "L",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo xc_emlrtBCI = {
    -1,                                                           /* iFirst */
    -1,                                                           /* iLast */
    9,                                                            /* lineNo */
    16,                                                           /* colNo */
    "N",                                                          /* aName */
    "oner",                                                       /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/oner.m", /* pName */
    0 /* checkKind */
};

static emlrtBCInfo yc_emlrtBCI = {
    -1,                                                           /* iFirst */
    -1,                                                           /* iLast */
    9,                                                            /* lineNo */
    11,                                                           /* colNo */
    "N",                                                          /* aName */
    "oner",                                                       /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/oner.m", /* pName */
    0 /* checkKind */
};

static emlrtDCInfo ib_emlrtDCI =
    {
        106,                    /* lineNo */
        31,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo ad_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        106,                    /* lineNo */
        31,                     /* colNo */
        "B",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo bd_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        106,                    /* lineNo */
        25,                     /* colNo */
        "L",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo jb_emlrtDCI =
    {
        93,                     /* lineNo */
        31,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo cd_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        93,                     /* lineNo */
        31,                     /* colNo */
        "B",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo kb_emlrtDCI =
    {
        113,                    /* lineNo */
        31,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo dd_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        113,                    /* lineNo */
        31,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ed_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        113,                    /* lineNo */
        25,                     /* colNo */
        "Z",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo lb_emlrtDCI =
    {
        104,                    /* lineNo */
        35,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo fd_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        104,                    /* lineNo */
        35,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo gd_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        104,                    /* lineNo */
        29,                     /* colNo */
        "L",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo mb_emlrtDCI =
    {
        115,                    /* lineNo */
        35,                     /* colNo */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo hd_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        115,                    /* lineNo */
        35,                     /* colNo */
        "A",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo id_emlrtBCI =
    {
        -1,                     /* iFirst */
        -1,                     /* iLast */
        115,                    /* lineNo */
        29,                     /* colNo */
        "L",                    /* aName */
        "pfqn_comom/genmatrix", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m", /* pName */
        0               /* checkKind */
};

static emlrtRTEInfo gb_emlrtRTEI =
    {
        25,           /* lineNo */
        1,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo hb_emlrtRTEI =
    {
        26,           /* lineNo */
        6,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo ib_emlrtRTEI =
    {
        523,             /* lineNo */
        21,              /* colNo */
        "unaryMinOrMax", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "unaryMinOrMax.m" /* pName */
};

static emlrtRTEInfo kb_emlrtRTEI =
    {
        29,           /* lineNo */
        1,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo lb_emlrtRTEI =
    {
        35,           /* lineNo */
        1,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo mb_emlrtRTEI =
    {
        37,           /* lineNo */
        17,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo nb_emlrtRTEI =
    {
        37,           /* lineNo */
        1,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo ob_emlrtRTEI =
    {
        39,           /* lineNo */
        1,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo pb_emlrtRTEI =
    {
        168,          /* lineNo */
        27,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo qb_emlrtRTEI =
    {
        168,          /* lineNo */
        34,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo rb_emlrtRTEI =
    {
        40,           /* lineNo */
        1,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo sb_emlrtRTEI =
    {
        169,          /* lineNo */
        21,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo tb_emlrtRTEI =
    {
        170,          /* lineNo */
        21,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo ub_emlrtRTEI = {
    8,                                                             /* lineNo */
    1,                                                             /* colNo */
    "factln",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/factln.m" /* pName */
};

static emlrtRTEInfo vb_emlrtRTEI =
    {
        43,           /* lineNo */
        1,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo wb_emlrtRTEI =
    {
        45,           /* lineNo */
        1,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo xb_emlrtRTEI =
    {
        46,           /* lineNo */
        1,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo yb_emlrtRTEI =
    {
        47,           /* lineNo */
        1,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo ac_emlrtRTEI =
    {
        94,                  /* lineNo */
        5,                   /* colNo */
        "eml_mtimes_helper", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

static emlrtRTEInfo bc_emlrtRTEI =
    {
        218,      /* lineNo */
        20,       /* colNo */
        "mtimes", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+blas/"
        "mtimes.m" /* pName */
};

static emlrtRTEInfo cc_emlrtRTEI = {
    42,     /* lineNo */
    5,      /* colNo */
    "sort", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/datafun/sort.m" /* pName */
};

static emlrtRTEInfo dc_emlrtRTEI = {
    30,                    /* lineNo */
    21,                    /* colNo */
    "applyScalarFunction", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunction.m" /* pName */
};

static emlrtRTEInfo ec_emlrtRTEI =
    {
        61,           /* lineNo */
        9,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo fc_emlrtRTEI =
    {
        41,           /* lineNo */
        1,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo gc_emlrtRTEI =
    {
        57,           /* lineNo */
        9,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo hc_emlrtRTEI =
    {
        8,            /* lineNo */
        28,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo ic_emlrtRTEI =
    {
        26,           /* lineNo */
        1,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo qc_emlrtRTEI =
    {
        150,          /* lineNo */
        30,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo rc_emlrtRTEI =
    {
        148,          /* lineNo */
        42,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo sc_emlrtRTEI = {
    13,         /* lineNo */
    8,          /* colNo */
    "matchrow", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/matchrow.m" /* pName */
};

static emlrtRTEInfo tc_emlrtRTEI = {
    16,         /* lineNo */
    16,         /* colNo */
    "matchrow", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/matchrow.m" /* pName */
};

static emlrtRTEInfo uc_emlrtRTEI = {
    16,         /* lineNo */
    20,         /* colNo */
    "matchrow", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/matchrow.m" /* pName */
};

static emlrtRTEInfo xc_emlrtRTEI =
    {
        69,           /* lineNo */
        9,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo yc_emlrtRTEI =
    {
        70,           /* lineNo */
        9,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo ad_emlrtRTEI =
    {
        71,           /* lineNo */
        9,            /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo bd_emlrtRTEI =
    {
        75,           /* lineNo */
        26,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo cd_emlrtRTEI =
    {
        81,           /* lineNo */
        20,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo dd_emlrtRTEI =
    {
        124,          /* lineNo */
        20,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo ed_emlrtRTEI =
    {
        97,           /* lineNo */
        24,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo fd_emlrtRTEI =
    {
        126,          /* lineNo */
        17,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo gd_emlrtRTEI =
    {
        85,           /* lineNo */
        34,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo hd_emlrtRTEI =
    {
        101,          /* lineNo */
        38,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo id_emlrtRTEI =
    {
        87,           /* lineNo */
        28,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo jd_emlrtRTEI =
    {
        111,          /* lineNo */
        25,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo kd_emlrtRTEI =
    {
        91,           /* lineNo */
        25,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo ld_emlrtRTEI =
    {
        88,           /* lineNo */
        38,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo md_emlrtRTEI =
    {
        102,          /* lineNo */
        38,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo nd_emlrtRTEI = {
    1,                                                           /* lineNo */
    19,                                                          /* colNo */
    "oner",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/oner.m" /* pName */
};

static emlrtRTEInfo od_emlrtRTEI =
    {
        92,           /* lineNo */
        38,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo pd_emlrtRTEI =
    {
        106,          /* lineNo */
        38,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo qd_emlrtRTEI =
    {
        104,          /* lineNo */
        47,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRTEInfo ke_emlrtRTEI =
    {
        55,           /* lineNo */
        15,           /* colNo */
        "pfqn_comom", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pName */
};

static emlrtRSInfo sg_emlrtRSI =
    {
        55,           /* lineNo */
        "pfqn_comom", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo tg_emlrtRSI = {
    54,    /* lineNo */
    "div", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/div.m" /* pathName
                                                                       */
};

static emlrtRSInfo ug_emlrtRSI =
    {
        126,                    /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

static emlrtRSInfo vg_emlrtRSI =
    {
        111,                    /* lineNo */
        "pfqn_comom/genmatrix", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_comom.m" /* pathName */
};

/* Function Declarations */
static int32_T b_hash(const emlrtStack *sp, const b_captured_var *Dn,
                      const captured_var *M, const emxArray_real_T *N,
                      const emxArray_real_T *n, real_T col_data[]);

static int32_T
binary_expand_op(const emlrtStack *sp, real_T in1_data[], const emlrtRSInfo in2,
                 const b_captured_var *in3, const captured_var *in4,
                 const emxArray_real_T *in5, int32_T in6, int32_T in7);

static int32_T binary_expand_op_1(const emlrtStack *sp, real_T in1_data[],
                                  const emlrtRSInfo in2,
                                  const b_captured_var *in3,
                                  const captured_var *in4,
                                  const emxArray_real_T *in5,
                                  const emxArray_real_T *in6,
                                  const emxArray_real_T *in7, int32_T in8);

static void binary_expand_op_2(const emlrtStack *sp, emxArray_real_T *in1,
                               const emxArray_real_T *in2,
                               const b_captured_var *in3, int32_T in4);

static int32_T binary_expand_op_5(const emlrtStack *sp, real_T in1_data[],
                                  const emlrtRSInfo in2,
                                  const b_captured_var *in3,
                                  const captured_var *in4,
                                  const emxArray_real_T *in5, int32_T in6,
                                  int32_T in7);

static int32_T binary_expand_op_6(const emlrtStack *sp, real_T in1_data[],
                                  const emlrtRSInfo in2,
                                  const b_captured_var *in3,
                                  const captured_var *in4,
                                  const emxArray_real_T *in5, int32_T in6);

static void genmatrix(const emlrtStack *sp, const b_captured_var *Dn,
                      captured_var *M, captured_var *R,
                      const emxArray_real_T *L, const emxArray_real_T *N,
                      const emxArray_real_T *Z, real_T r, emxArray_real_T *A,
                      emxArray_real_T *B, emxArray_real_T *DA);

static int32_T hash(const emlrtStack *sp, const b_captured_var *Dn,
                    const captured_var *M, const emxArray_real_T *N,
                    const emxArray_real_T *n, real_T i, real_T col_data[]);

static void minus(const emlrtStack *sp, emxArray_real_T *in1,
                  const emxArray_real_T *in2, const emxArray_real_T *in3);

static void plus(const emlrtStack *sp, emxArray_real_T *in1,
                 const emxArray_real_T *in2);

/* Function Definitions */
static int32_T b_hash(const emlrtStack *sp, const b_captured_var *Dn,
                      const captured_var *M, const emxArray_real_T *N,
                      const emxArray_real_T *n, real_T col_data[])
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  emxArray_boolean_T *b_Dn;
  emxArray_boolean_T *r;
  emxArray_boolean_T *x;
  emxArray_real_T *row;
  const real_T *N_data;
  const real_T *n_data;
  real_T c_Dn;
  real_T *row_data;
  int32_T b_hash_numThreads;
  int32_T col_size;
  int32_T i;
  int32_T i1;
  int32_T ii_data;
  int32_T loop_ub;
  int32_T vectorUB;
  boolean_T *Dn_data;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  n_data = n->data;
  N_data = N->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  loop_ub = N->size[1];
  if ((N->size[1] != n->size[1]) && ((N->size[1] != 1) && (n->size[1] != 1))) {
    emlrtDimSizeImpxCheckR2021b(N->size[1], n->size[1], &e_emlrtECI,
                                (emlrtConstCTX)sp);
  }
  st.site = &ob_emlrtRSI;
  emxInit_real_T(&st, &row, 2, &rc_emlrtRTEI);
  if (N->size[1] == n->size[1]) {
    col_size = row->size[0] * row->size[1];
    row->size[0] = 1;
    row->size[1] = N->size[1];
    emxEnsureCapacity_real_T(&st, row, col_size, &rc_emlrtRTEI);
    row_data = row->data;
    col_size = (N->size[1] / 2) << 1;
    vectorUB = col_size - 2;
    for (i = 0; i <= vectorUB; i += 2) {
      _mm_storeu_pd(&row_data[i], _mm_sub_pd(_mm_loadu_pd(&N_data[i]),
                                             _mm_loadu_pd(&n_data[i])));
    }
    for (i = col_size; i < loop_ub; i++) {
      row_data[i] = N_data[i] - n_data[i];
    }
  } else {
    b_st.site = &ob_emlrtRSI;
    minus(&b_st, row, N, n);
    row_data = row->data;
  }
  /*  pos = matchrow(M, r) */
  /*  Return position of row r in matrix M if unique, -1 otherwise */
  /*  */
  /*  Copyright (c) 2012-2026, Imperial College London */
  /*  All rights reserved. */
  if (Dn->contents->size[1] != row->size[1]) {
    /* Incompatible matrix and row sizes */
    ii_data = -2;
  } else {
    ii_data = Dn->contents->size[0];
    if (ii_data < 1) {
      emlrtDynamicBoundsCheckR2012b(ii_data, 1, ii_data, &u_emlrtBCI, &st);
    }
    col_size = Dn->contents->size[1];
    if ((col_size != row->size[1]) &&
        ((col_size != 1) && (row->size[1] != 1))) {
      emlrtDimSizeImpxCheckR2021b(col_size, row->size[1], &g_emlrtECI, &st);
    }
    emxInit_boolean_T(&st, &b_Dn, 2, &sc_emlrtRTEI);
    col_size = b_Dn->size[0] * b_Dn->size[1];
    b_Dn->size[0] = 1;
    if (row->size[1] == 1) {
      loop_ub = Dn->contents->size[1];
    } else {
      loop_ub = row->size[1];
    }
    b_Dn->size[1] = loop_ub;
    emxEnsureCapacity_boolean_T(&st, b_Dn, col_size, &sc_emlrtRTEI);
    Dn_data = b_Dn->data;
    col_size = (Dn->contents->size[1] != 1);
    vectorUB = (row->size[1] != 1);
    if (loop_ub < 1600) {
      for (i1 = 0; i1 < loop_ub; i1++) {
        Dn_data[i1] =
            (Dn->contents
                 ->data[(ii_data + Dn->contents->size[0] * (i1 * col_size)) -
                        1] == row_data[i1 * vectorUB]);
      }
    } else {
      emlrtEnterParallelRegion(&st, omp_in_parallel());
      emlrtPushJmpBuf(&st, &emlrtJBStack);
      b_hash_numThreads =
          emlrtAllocRegionTLSs(st.tls, omp_in_parallel(), omp_get_max_threads(),
                               omp_get_num_procs());
#pragma omp parallel for num_threads(b_hash_numThreads)

      for (i1 = 0; i1 < loop_ub; i1++) {
        Dn_data[i1] =
            (Dn->contents
                 ->data[(ii_data + Dn->contents->size[0] * (i1 * col_size)) -
                        1] == row_data[i1 * vectorUB]);
      }
      emlrtPopJmpBuf(&st, &emlrtJBStack);
      emlrtExitParallelRegion(&st, omp_in_parallel());
    }
    b_st.site = &qb_emlrtRSI;
    if (!all(&b_st, b_Dn)) {
      b_st.site = &rb_emlrtRSI;
      emxInit_boolean_T(&b_st, &r, 2, &uc_emlrtRTEI);
      c_st.site = &rb_emlrtRSI;
      bsxfun(&c_st, Dn->contents, row, r);
      emxInit_boolean_T(&b_st, &x, 1, &tc_emlrtRTEI);
      c_st.site = &rb_emlrtRSI;
      b_all(&c_st, r, x);
      emxFree_boolean_T(&b_st, &r);
      c_st.site = &wb_emlrtRSI;
      col_size = eml_find(&c_st, x, (int32_T *)&ii_data);
      emxFree_boolean_T(&b_st, &x);
      if (col_size == 0) {
        ii_data = -1;
      }
    }
    emxFree_boolean_T(&st, &b_Dn);
  }
  emxFree_real_T(&st, &row);
  c_Dn = (real_T)Dn->contents->size[0] * M->contents;
  col_size = 1;
  col_data[0] = c_Dn + (real_T)ii_data;
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return col_size;
}

static int32_T
binary_expand_op(const emlrtStack *sp, real_T in1_data[], const emlrtRSInfo in2,
                 const b_captured_var *in3, const captured_var *in4,
                 const emxArray_real_T *in5, int32_T in6, int32_T in7)
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack st;
  emxArray_real_T *b_in5;
  const real_T *in5_data;
  real_T *b_in5_data;
  int32_T binary_expand_op_numThreads;
  int32_T i;
  int32_T in1_size;
  int32_T loop_ub;
  int32_T stride_1_1;
  st.prev = sp;
  st.tls = sp->tls;
  in5_data = in5->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &b_in5, 2, &ld_emlrtRTEI);
  in1_size = b_in5->size[0] * b_in5->size[1];
  b_in5->size[0] = 1;
  if (in3->contents->size[1] == 1) {
    loop_ub = in5->size[1];
  } else {
    loop_ub = in3->contents->size[1];
  }
  b_in5->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, b_in5, in1_size, &ld_emlrtRTEI);
  b_in5_data = b_in5->data;
  in1_size = (in5->size[1] != 1);
  stride_1_1 = (in3->contents->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      b_in5_data[i] =
          in5_data[i * in1_size] -
          in3->contents->data[in6 + in3->contents->size[0] * (i * stride_1_1)];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    binary_expand_op_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(binary_expand_op_numThreads)

    for (i = 0; i < loop_ub; i++) {
      b_in5_data[i] =
          in5_data[i * in1_size] -
          in3->contents->data[in6 + in3->contents->size[0] * (i * stride_1_1)];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  st.site = (emlrtRSInfo *)&in2;
  in1_size = hash(&st, in3, in4, in5, b_in5, (real_T)in7 + 1.0, in1_data);
  emxFree_real_T(sp, &b_in5);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return in1_size;
}

static int32_T binary_expand_op_1(const emlrtStack *sp, real_T in1_data[],
                                  const emlrtRSInfo in2,
                                  const b_captured_var *in3,
                                  const captured_var *in4,
                                  const emxArray_real_T *in5,
                                  const emxArray_real_T *in6,
                                  const emxArray_real_T *in7, int32_T in8)
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack st;
  emxArray_real_T *b_in6;
  const real_T *in6_data;
  const real_T *in7_data;
  real_T *b_in6_data;
  int32_T binary_expand_op_1_numThreads;
  int32_T i;
  int32_T in1_size;
  int32_T loop_ub;
  int32_T stride_1_1;
  st.prev = sp;
  st.tls = sp->tls;
  in7_data = in7->data;
  in6_data = in6->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &b_in6, 2, &od_emlrtRTEI);
  in1_size = b_in6->size[0] * b_in6->size[1];
  b_in6->size[0] = 1;
  if (in7->size[1] == 1) {
    loop_ub = in6->size[1];
  } else {
    loop_ub = in7->size[1];
  }
  b_in6->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, b_in6, in1_size, &od_emlrtRTEI);
  b_in6_data = b_in6->data;
  in1_size = (in6->size[1] != 1);
  stride_1_1 = (in7->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      b_in6_data[i] = in6_data[i * in1_size] + in7_data[i * stride_1_1];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    binary_expand_op_1_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(binary_expand_op_1_numThreads)

    for (i = 0; i < loop_ub; i++) {
      b_in6_data[i] = in6_data[i * in1_size] + in7_data[i * stride_1_1];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  st.site = (emlrtRSInfo *)&in2;
  in1_size = hash(&st, in3, in4, in5, b_in6, (real_T)in8 + 1.0, in1_data);
  emxFree_real_T(sp, &b_in6);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return in1_size;
}

static void binary_expand_op_2(const emlrtStack *sp, emxArray_real_T *in1,
                               const emxArray_real_T *in2,
                               const b_captured_var *in3, int32_T in4)
{
  jmp_buf *volatile emlrtJBStack;
  const real_T *in2_data;
  real_T *in1_data;
  int32_T binary_expand_op_2_numThreads;
  int32_T i;
  int32_T loop_ub;
  int32_T stride_0_1;
  int32_T stride_1_1;
  in2_data = in2->data;
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[0] = 1;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &od_emlrtRTEI);
  if (in3->contents->size[1] == 1) {
    loop_ub = in2->size[1];
  } else {
    loop_ub = in3->contents->size[1];
  }
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &od_emlrtRTEI);
  in1_data = in1->data;
  stride_0_1 = (in2->size[1] != 1);
  stride_1_1 = (in3->contents->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      in1_data[i] =
          in2_data[i * stride_0_1] -
          in3->contents->data[in4 + in3->contents->size[0] * (i * stride_1_1)];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    binary_expand_op_2_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(binary_expand_op_2_numThreads)

    for (i = 0; i < loop_ub; i++) {
      in1_data[i] =
          in2_data[i * stride_0_1] -
          in3->contents->data[in4 + in3->contents->size[0] * (i * stride_1_1)];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
}

static int32_T binary_expand_op_5(const emlrtStack *sp, real_T in1_data[],
                                  const emlrtRSInfo in2,
                                  const b_captured_var *in3,
                                  const captured_var *in4,
                                  const emxArray_real_T *in5, int32_T in6,
                                  int32_T in7)
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack st;
  emxArray_real_T *b_in5;
  const real_T *in5_data;
  real_T *b_in5_data;
  int32_T binary_expand_op_5_numThreads;
  int32_T i;
  int32_T in1_size;
  int32_T loop_ub;
  int32_T stride_1_1;
  st.prev = sp;
  st.tls = sp->tls;
  in5_data = in5->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &b_in5, 2, &pd_emlrtRTEI);
  in1_size = b_in5->size[0] * b_in5->size[1];
  b_in5->size[0] = 1;
  if (in3->contents->size[1] == 1) {
    loop_ub = in5->size[1];
  } else {
    loop_ub = in3->contents->size[1];
  }
  b_in5->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, b_in5, in1_size, &pd_emlrtRTEI);
  b_in5_data = b_in5->data;
  in1_size = (in5->size[1] != 1);
  stride_1_1 = (in3->contents->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      b_in5_data[i] =
          in5_data[i * in1_size] -
          in3->contents->data[in6 + in3->contents->size[0] * (i * stride_1_1)];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    binary_expand_op_5_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(binary_expand_op_5_numThreads)

    for (i = 0; i < loop_ub; i++) {
      b_in5_data[i] =
          in5_data[i * in1_size] -
          in3->contents->data[in6 + in3->contents->size[0] * (i * stride_1_1)];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  st.site = (emlrtRSInfo *)&in2;
  in1_size =
      hash(&st, in3, in4, in5, b_in5, ((real_T)in7 + 1.0) + 1.0, in1_data);
  emxFree_real_T(sp, &b_in5);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return in1_size;
}

static int32_T binary_expand_op_6(const emlrtStack *sp, real_T in1_data[],
                                  const emlrtRSInfo in2,
                                  const b_captured_var *in3,
                                  const captured_var *in4,
                                  const emxArray_real_T *in5, int32_T in6)
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack st;
  emxArray_real_T *b_in5;
  const real_T *in5_data;
  real_T *b_in5_data;
  int32_T binary_expand_op_6_numThreads;
  int32_T i;
  int32_T in1_size;
  int32_T loop_ub;
  int32_T stride_1_1;
  st.prev = sp;
  st.tls = sp->tls;
  in5_data = in5->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &b_in5, 2, &md_emlrtRTEI);
  in1_size = b_in5->size[0] * b_in5->size[1];
  b_in5->size[0] = 1;
  if (in3->contents->size[1] == 1) {
    loop_ub = in5->size[1];
  } else {
    loop_ub = in3->contents->size[1];
  }
  b_in5->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, b_in5, in1_size, &md_emlrtRTEI);
  b_in5_data = b_in5->data;
  in1_size = (in5->size[1] != 1);
  stride_1_1 = (in3->contents->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      b_in5_data[i] =
          in5_data[i * in1_size] -
          in3->contents->data[in6 + in3->contents->size[0] * (i * stride_1_1)];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    binary_expand_op_6_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(binary_expand_op_6_numThreads)

    for (i = 0; i < loop_ub; i++) {
      b_in5_data[i] =
          in5_data[i * in1_size] -
          in3->contents->data[in6 + in3->contents->size[0] * (i * stride_1_1)];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  st.site = (emlrtRSInfo *)&in2;
  in1_size = b_hash(&st, in3, in4, in5, b_in5, in1_data);
  emxFree_real_T(sp, &b_in5);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return in1_size;
}

static void genmatrix(const emlrtStack *sp, const b_captured_var *Dn,
                      captured_var *M, captured_var *R,
                      const emxArray_real_T *L, const emxArray_real_T *N,
                      const emxArray_real_T *Z, real_T r, emxArray_real_T *A,
                      emxArray_real_T *B, emxArray_real_T *DA)
{
  emlrtStack b_st;
  emlrtStack st;
  emxArray_real_T *b_n;
  emxArray_real_T *c_n;
  emxArray_real_T *er;
  const real_T *L_data;
  const real_T *N_data;
  const real_T *Z_data;
  real_T A_tmp;
  real_T col_data;
  real_T d;
  real_T d1 = 0.0;
  real_T *A_data;
  real_T *B_data;
  real_T *DA_data;
  real_T *b_n_data;
  real_T *er_data;
  real_T *n_data;
  int32_T b_R;
  int32_T b_d;
  int32_T b_loop_ub;
  int32_T c_d;
  int32_T c_loop_ub;
  int32_T d_loop_ub;
  int32_T e_loop_ub;
  int32_T g_loop_ub;
  int32_T h_loop_ub = 0;
  int32_T i;
  int32_T i1;
  int32_T i2;
  int32_T i3;
  int32_T i4;
  int32_T i5;
  int32_T i6 = 0;
  int32_T i7 = 0;
  int32_T i_loop_ub;
  int32_T k;
  int32_T loop_ub;
  int32_T n;
  int32_T t;
  int32_T varargin_1;
  int32_T varargin_2;
  uint32_T row;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  Z_data = Z->data;
  N_data = N->data;
  L_data = L->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  b_R = L->size[1];
  R->contents = L->size[1];
  d = L->size[0];
  M->contents = d;
  A_tmp = (M->contents + R->contents) - 1.0;
  st.site = &ae_emlrtRSI;
  col_data = nchoosek(&st, A_tmp, M->contents) * (M->contents + 1.0);
  if (!(col_data >= 0.0)) {
    emlrtNonNegativeCheckR2012b(col_data, &n_emlrtDCI, (emlrtConstCTX)sp);
  }
  if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
    emlrtIntegerCheckR2012b(col_data, &o_emlrtDCI, (emlrtConstCTX)sp);
  }
  loop_ub = A->size[0] * A->size[1];
  A->size[0] = (int32_T)col_data;
  A->size[1] = (int32_T)col_data;
  emxEnsureCapacity_real_T(sp, A, loop_ub, &xc_emlrtRTEI);
  A_data = A->data;
  b_loop_ub = (int32_T)col_data * (int32_T)col_data;
  for (t = 0; t < b_loop_ub; t++) {
    A_data[t] = 0.0;
  }
  st.site = &yd_emlrtRSI;
  col_data = nchoosek(&st, A_tmp, M->contents) * (M->contents + 1.0);
  if (!(col_data >= 0.0)) {
    emlrtNonNegativeCheckR2012b(col_data, &p_emlrtDCI, (emlrtConstCTX)sp);
  }
  if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
    emlrtIntegerCheckR2012b(col_data, &q_emlrtDCI, (emlrtConstCTX)sp);
  }
  b_loop_ub = DA->size[0] * DA->size[1];
  DA->size[0] = (int32_T)col_data;
  DA->size[1] = (int32_T)col_data;
  emxEnsureCapacity_real_T(sp, DA, b_loop_ub, &yc_emlrtRTEI);
  DA_data = DA->data;
  loop_ub = (int32_T)col_data * (int32_T)col_data;
  for (t = 0; t < loop_ub; t++) {
    DA_data[t] = 0.0;
  }
  st.site = &xd_emlrtRSI;
  col_data = nchoosek(&st, A_tmp, M->contents) * (M->contents + 1.0);
  if (!(col_data >= 0.0)) {
    emlrtNonNegativeCheckR2012b(col_data, &r_emlrtDCI, (emlrtConstCTX)sp);
  }
  if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
    emlrtIntegerCheckR2012b(col_data, &s_emlrtDCI, (emlrtConstCTX)sp);
  }
  b_loop_ub = B->size[0] * B->size[1];
  B->size[0] = (int32_T)col_data;
  B->size[1] = (int32_T)col_data;
  emxEnsureCapacity_real_T(sp, B, b_loop_ub, &ad_emlrtRTEI);
  B_data = B->data;
  c_loop_ub = (int32_T)col_data * (int32_T)col_data;
  for (t = 0; t < c_loop_ub; t++) {
    B_data[t] = 0.0;
  }
  row = 0U;
  varargin_1 = Dn->contents->size[0];
  varargin_2 = Dn->contents->size[1];
  if ((varargin_1 == 0) || (varargin_2 == 0)) {
    loop_ub = 0;
  } else {
    loop_ub = muIntScalarMax_sint32(varargin_1, varargin_2);
  }
  emxInit_real_T(sp, &er, 2, &kd_emlrtRTEI);
  for (b_d = 0; b_d < loop_ub; b_d++) {
    st.site = &wd_emlrtRSI;
    if (b_d + 1 > varargin_1) {
      emlrtDynamicBoundsCheckR2012b(b_d + 1, 1, varargin_1, &v_emlrtBCI, &st);
    }
    b_loop_ub = er->size[0] * er->size[1];
    er->size[0] = 1;
    er->size[1] = varargin_2;
    emxEnsureCapacity_real_T(&st, er, b_loop_ub, &bd_emlrtRTEI);
    er_data = er->data;
    for (t = 0; t < varargin_2; t++) {
      er_data[t] = Dn->contents->data[b_d + Dn->contents->size[0] * t];
    }
    emlrtForLoopVectorCheckR2021a(1.0, 1.0, b_R, mxDOUBLE_CLASS, b_R,
                                  &v_emlrtRTEI, &st);
    for (t = 0; t < b_R; t++) {
      if (((int32_T)((uint32_T)t + 1U) < 1) ||
          ((int32_T)((uint32_T)t + 1U) > varargin_2)) {
        emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)t + 1U), 1,
                                      varargin_2, &ic_emlrtBCI, &st);
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b(&st);
      }
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  if ((varargin_1 == 0) || (varargin_2 == 0)) {
    n = 0;
  } else {
    n = muIntScalarMax_sint32(varargin_1, varargin_2);
    col_data = R->contents - 1.0;
    if (r > col_data) {
      i = 0;
      i1 = 0;
    } else {
      if (((int32_T)r < 1) || ((int32_T)r > varargin_2)) {
        emlrtDynamicBoundsCheckR2012b((int32_T)r, 1, varargin_2, &x_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      i = (int32_T)r - 1;
      if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
        emlrtIntegerCheckR2012b(col_data, &t_emlrtDCI, (emlrtConstCTX)sp);
      }
      if (((int32_T)col_data < 1) || ((int32_T)col_data > varargin_2)) {
        emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, varargin_2,
                                      &y_emlrtBCI, (emlrtConstCTX)sp);
      }
      i1 = (int32_T)col_data;
    }
    d_loop_ub = i1 - i;
  }
  emxInit_real_T(sp, &b_n, 2, &jd_emlrtRTEI);
  emxInit_real_T(sp, &c_n, 2, &od_emlrtRTEI);
  for (c_d = 0; c_d < n; c_d++) {
    if (c_d + 1 > varargin_1) {
      emlrtDynamicBoundsCheckR2012b(c_d + 1, 1, varargin_1, &w_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    b_loop_ub = er->size[0] * er->size[1];
    er->size[0] = 1;
    er->size[1] = i1 - i;
    emxEnsureCapacity_real_T(sp, er, b_loop_ub, &cd_emlrtRTEI);
    er_data = er->data;
    for (b_d = 0; b_d < d_loop_ub; b_d++) {
      er_data[b_d] =
          Dn->contents->data[c_d + Dn->contents->size[0] * (i + b_d)];
    }
    st.site = &vd_emlrtRSI;
    if (sum(&st, er) > 0.0) {
      int32_T f_loop_ub;
      /*  dummy rows for unused norm consts */
      f_loop_ub = (int32_T)(d + 1.0);
      emlrtForLoopVectorCheckR2021a(0.0, 1.0, d, mxDOUBLE_CLASS,
                                    (int32_T)(d + 1.0), &w_emlrtRTEI,
                                    (emlrtConstCTX)sp);
      if ((int32_T)(d + 1.0) - 1 >= 0) {
        col_data = R->contents - 1.0;
        if (r + 1.0 > col_data) {
          i4 = 0;
          i5 = 0;
        } else {
          if (((int32_T)(r + 1.0) < 1) || ((int32_T)(r + 1.0) > varargin_2)) {
            emlrtDynamicBoundsCheckR2012b((int32_T)(r + 1.0), 1, varargin_2,
                                          &cb_emlrtBCI, (emlrtConstCTX)sp);
          }
          i4 = (int32_T)(r + 1.0) - 1;
          if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
            emlrtIntegerCheckR2012b(col_data, &u_emlrtDCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)col_data < 1) || ((int32_T)col_data > varargin_2)) {
            emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, varargin_2,
                                          &db_emlrtBCI, (emlrtConstCTX)sp);
          }
          i5 = (int32_T)col_data;
        }
        g_loop_ub = i5 - i4;
      }
      for (b_d = 0; b_d < f_loop_ub; b_d++) {
        row++;
        if (c_d + 1 > varargin_1) {
          emlrtDynamicBoundsCheckR2012b(c_d + 1, 1, varargin_1, &ab_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        loop_ub = N->size[1];
        if ((N->size[1] != varargin_2) &&
            ((N->size[1] != 1) && (varargin_2 != 1))) {
          emlrtDimSizeImpxCheckR2021b(N->size[1], varargin_2, &h_emlrtECI,
                                      (emlrtConstCTX)sp);
        }
        if (N->size[1] == varargin_2) {
          b_loop_ub = er->size[0] * er->size[1];
          er->size[0] = 1;
          er->size[1] = N->size[1];
          emxEnsureCapacity_real_T(sp, er, b_loop_ub, &gd_emlrtRTEI);
          er_data = er->data;
          for (t = 0; t < loop_ub; t++) {
            er_data[t] =
                N_data[t] - Dn->contents->data[c_d + Dn->contents->size[0] * t];
          }
          st.site = &ud_emlrtRSI;
          hash(&st, Dn, M, N, er, (real_T)b_d + 1.0, (real_T *)&col_data);
        } else {
          st.site = &ud_emlrtRSI;
          binary_expand_op(&st, (real_T *)&col_data, ud_emlrtRSI, Dn, M, N, c_d,
                           b_d);
        }
        if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
          emlrtIntegerCheckR2012b(col_data, &y_emlrtDCI, (emlrtConstCTX)sp);
        }
        if (((int32_T)col_data < 1) || ((int32_T)col_data > A->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, A->size[1],
                                        &jc_emlrtBCI, (emlrtConstCTX)sp);
        }
        if (((int32_T)row < 1) || ((int32_T)row > A->size[0])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)row, 1, A->size[0],
                                        &ob_emlrtBCI, (emlrtConstCTX)sp);
        }
        A_data[((int32_T)row + A->size[0] * ((int32_T)col_data - 1)) - 1] = 1.0;
        if (c_d + 1 > varargin_1) {
          emlrtDynamicBoundsCheckR2012b(c_d + 1, 1, varargin_1, &bb_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        b_loop_ub = er->size[0] * er->size[1];
        er->size[0] = 1;
        er->size[1] = i5 - i4;
        emxEnsureCapacity_real_T(sp, er, b_loop_ub, &id_emlrtRTEI);
        er_data = er->data;
        for (t = 0; t < g_loop_ub; t++) {
          er_data[t] =
              Dn->contents->data[c_d + Dn->contents->size[0] * (i4 + t)];
        }
        st.site = &td_emlrtRSI;
        if (sum(&st, er) > 0.0) {
          if (c_d + 1 > varargin_1) {
            emlrtDynamicBoundsCheckR2012b(c_d + 1, 1, varargin_1, &eb_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if ((N->size[1] != varargin_2) &&
              ((N->size[1] != 1) && (varargin_2 != 1))) {
            emlrtDimSizeImpxCheckR2021b(N->size[1], varargin_2, &i_emlrtECI,
                                        (emlrtConstCTX)sp);
          }
          if (N->size[1] == varargin_2) {
            b_loop_ub = er->size[0] * er->size[1];
            er->size[0] = 1;
            er->size[1] = N->size[1];
            emxEnsureCapacity_real_T(sp, er, b_loop_ub, &ld_emlrtRTEI);
            er_data = er->data;
            for (t = 0; t < loop_ub; t++) {
              er_data[t] = N_data[t] -
                           Dn->contents->data[c_d + Dn->contents->size[0] * t];
            }
            st.site = &sd_emlrtRSI;
            hash(&st, Dn, M, N, er, (real_T)b_d + 1.0, (real_T *)&col_data);
          } else {
            st.site = &sd_emlrtRSI;
            binary_expand_op(&st, (real_T *)&col_data, sd_emlrtRSI, Dn, M, N,
                             c_d, b_d);
          }
          if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
            emlrtIntegerCheckR2012b(col_data, &gb_emlrtDCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)col_data < 1) || ((int32_T)col_data > B->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, B->size[1],
                                          &tc_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)row < 1) || ((int32_T)row > B->size[0])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)row, 1, B->size[0],
                                          &bc_emlrtBCI, (emlrtConstCTX)sp);
          }
          B_data[((int32_T)row + B->size[0] * ((int32_T)col_data - 1)) - 1] =
              1.0;
        } else {
          if (!(R->contents >= 0.0)) {
            emlrtNonNegativeCheckR2012b(R->contents, &v_emlrtDCI,
                                        (emlrtConstCTX)sp);
          }
          if (R->contents != (int32_T)muDoubleScalarFloor(R->contents)) {
            emlrtIntegerCheckR2012b(R->contents, &w_emlrtDCI,
                                    (emlrtConstCTX)sp);
          }
          b_loop_ub = er->size[0] * er->size[1];
          er->size[0] = 1;
          er->size[1] = b_R;
          emxEnsureCapacity_real_T(sp, er, b_loop_ub, &kd_emlrtRTEI);
          er_data = er->data;
          for (t = 0; t < b_R; t++) {
            er_data[t] = 0.0;
          }
          if (((int32_T)r < 1) || ((int32_T)r > (int32_T)R->contents)) {
            emlrtDynamicBoundsCheckR2012b((int32_T)r, 1, (int32_T)R->contents,
                                          &uc_emlrtBCI, (emlrtConstCTX)sp);
          }
          er_data[(int32_T)r - 1] = 1.0;
          if (c_d + 1 > varargin_1) {
            emlrtDynamicBoundsCheckR2012b(c_d + 1, 1, varargin_1, &fb_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if ((N->size[1] != varargin_2) &&
              ((N->size[1] != 1) && (varargin_2 != 1))) {
            emlrtDimSizeImpxCheckR2021b(N->size[1], varargin_2, &j_emlrtECI,
                                        (emlrtConstCTX)sp);
          }
          if (N->size[1] == varargin_2) {
            b_loop_ub = b_n->size[0] * b_n->size[1];
            b_n->size[0] = 1;
            b_n->size[1] = N->size[1];
            emxEnsureCapacity_real_T(sp, b_n, b_loop_ub, &od_emlrtRTEI);
            n_data = b_n->data;
            for (t = 0; t < loop_ub; t++) {
              n_data[t] = N_data[t] -
                          Dn->contents->data[c_d + Dn->contents->size[0] * t];
            }
          } else {
            st.site = &rd_emlrtRSI;
            binary_expand_op_2(&st, b_n, N, Dn, c_d);
            n_data = b_n->data;
          }
          c_loop_ub = b_n->size[1];
          if ((b_n->size[1] != er->size[1]) &&
              ((b_n->size[1] != 1) && (er->size[1] != 1))) {
            emlrtDimSizeImpxCheckR2021b(b_n->size[1], er->size[1], &j_emlrtECI,
                                        (emlrtConstCTX)sp);
          }
          if (b_n->size[1] == er->size[1]) {
            b_loop_ub = c_n->size[0] * c_n->size[1];
            c_n->size[0] = 1;
            c_n->size[1] = b_n->size[1];
            emxEnsureCapacity_real_T(sp, c_n, b_loop_ub, &od_emlrtRTEI);
            b_n_data = c_n->data;
            b_loop_ub = (b_n->size[1] / 2) << 1;
            loop_ub = b_loop_ub - 2;
            for (t = 0; t <= loop_ub; t += 2) {
              __m128d b_r;
              __m128d r1;
              b_r = _mm_loadu_pd(&n_data[t]);
              r1 = _mm_loadu_pd(&er_data[t]);
              _mm_storeu_pd(&b_n_data[t], _mm_add_pd(b_r, r1));
            }
            for (t = b_loop_ub; t < c_loop_ub; t++) {
              b_n_data[t] = n_data[t] + er_data[t];
            }
            st.site = &rd_emlrtRSI;
            hash(&st, Dn, M, N, c_n, (real_T)b_d + 1.0, (real_T *)&col_data);
          } else {
            st.site = &rd_emlrtRSI;
            binary_expand_op_1(&st, (real_T *)&col_data, rd_emlrtRSI, Dn, M, N,
                               b_n, er, b_d);
          }
          if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
            emlrtIntegerCheckR2012b(col_data, &jb_emlrtDCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)col_data < 1) || ((int32_T)col_data > B->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, B->size[1],
                                          &cd_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)row < 1) || ((int32_T)row > B->size[0])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)row, 1, B->size[0],
                                          &cc_emlrtBCI, (emlrtConstCTX)sp);
          }
          B_data[((int32_T)row + B->size[0] * ((int32_T)col_data - 1)) - 1] =
              1.0;
        }
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b((emlrtConstCTX)sp);
        }
      }
    } else {
      if (varargin_2 < 1) {
        emlrtDynamicBoundsCheckR2012b(1, 1, varargin_2, &hb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      loop_ub = (int32_T)r;
      if (((int32_T)r < 1) || ((int32_T)r > varargin_2)) {
        emlrtDynamicBoundsCheckR2012b((int32_T)r, 1, varargin_2, &ib_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (c_d + 1 > varargin_1) {
        emlrtDynamicBoundsCheckR2012b(c_d + 1, 1, varargin_1, &gb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      b_loop_ub = er->size[0] * er->size[1];
      er->size[0] = 1;
      er->size[1] = (int32_T)r;
      emxEnsureCapacity_real_T(sp, er, b_loop_ub, &ed_emlrtRTEI);
      er_data = er->data;
      for (b_d = 0; b_d < loop_ub; b_d++) {
        er_data[b_d] = Dn->contents->data[c_d + Dn->contents->size[0] * b_d];
      }
      st.site = &qd_emlrtRSI;
      if (sum(&st, er) < M->contents) {
        c_loop_ub = (int32_T)d;
        emlrtForLoopVectorCheckR2021a(1.0, 1.0, d, mxDOUBLE_CLASS, (int32_T)d,
                                      &x_emlrtRTEI, (emlrtConstCTX)sp);
        for (k = 0; k < c_loop_ub; k++) {
          int32_T f_loop_ub;
          /*  add CE */
          row++;
          if (c_d + 1 > varargin_1) {
            emlrtDynamicBoundsCheckR2012b(c_d + 1, 1, varargin_1, &qb_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          f_loop_ub = N->size[1];
          if ((N->size[1] != varargin_2) &&
              ((N->size[1] != 1) && (varargin_2 != 1))) {
            emlrtDimSizeImpxCheckR2021b(N->size[1], varargin_2, &m_emlrtECI,
                                        (emlrtConstCTX)sp);
          }
          if (N->size[1] == varargin_2) {
            b_loop_ub = er->size[0] * er->size[1];
            er->size[0] = 1;
            er->size[1] = N->size[1];
            emxEnsureCapacity_real_T(sp, er, b_loop_ub, &hd_emlrtRTEI);
            er_data = er->data;
            for (t = 0; t < f_loop_ub; t++) {
              er_data[t] = N_data[t] -
                           Dn->contents->data[c_d + Dn->contents->size[0] * t];
            }
            st.site = &od_emlrtRSI;
            hash(&st, Dn, M, N, er, ((real_T)k + 1.0) + 1.0,
                 (real_T *)&col_data);
          } else {
            st.site = &od_emlrtRSI;
            binary_expand_op_5(&st, (real_T *)&col_data, od_emlrtRSI, Dn, M, N,
                               c_d, k);
          }
          if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
            emlrtIntegerCheckR2012b(col_data, &bb_emlrtDCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)col_data < 1) || ((int32_T)col_data > A->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, A->size[1],
                                          &lc_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)row < 1) || ((int32_T)row > A->size[0])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)row, 1, A->size[0],
                                          &pb_emlrtBCI, (emlrtConstCTX)sp);
          }
          A_data[((int32_T)row + A->size[0] * ((int32_T)col_data - 1)) - 1] =
              1.0;
          if (c_d + 1 > varargin_1) {
            emlrtDynamicBoundsCheckR2012b(c_d + 1, 1, varargin_1, &sb_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if ((N->size[1] != varargin_2) &&
              ((N->size[1] != 1) && (varargin_2 != 1))) {
            emlrtDimSizeImpxCheckR2021b(N->size[1], varargin_2, &n_emlrtECI,
                                        (emlrtConstCTX)sp);
          }
          if (N->size[1] == varargin_2) {
            b_loop_ub = er->size[0] * er->size[1];
            er->size[0] = 1;
            er->size[1] = N->size[1];
            emxEnsureCapacity_real_T(sp, er, b_loop_ub, &md_emlrtRTEI);
            er_data = er->data;
            for (t = 0; t < f_loop_ub; t++) {
              er_data[t] = N_data[t] -
                           Dn->contents->data[c_d + Dn->contents->size[0] * t];
            }
            st.site = &nd_emlrtRSI;
            b_hash(&st, Dn, M, N, er, (real_T *)&col_data);
          } else {
            st.site = &nd_emlrtRSI;
            binary_expand_op_6(&st, (real_T *)&col_data, nd_emlrtRSI, Dn, M, N,
                               c_d);
          }
          if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
            emlrtIntegerCheckR2012b(col_data, &fb_emlrtDCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)col_data < 1) || ((int32_T)col_data > A->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, A->size[1],
                                          &rc_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)row < 1) || ((int32_T)row > A->size[0])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)row, 1, A->size[0],
                                          &rb_emlrtBCI, (emlrtConstCTX)sp);
          }
          A_data[((int32_T)row + A->size[0] * ((int32_T)col_data - 1)) - 1] =
              -1.0;
          loop_ub = (int32_T)(r - 1.0);
          for (b_d = 0; b_d < loop_ub; b_d++) {
            if (c_d + 1 > varargin_1) {
              emlrtDynamicBoundsCheckR2012b(c_d + 1, 1, varargin_1,
                                            &ub_emlrtBCI, (emlrtConstCTX)sp);
            }
            if ((f_loop_ub != varargin_2) &&
                ((f_loop_ub != 1) && (varargin_2 != 1))) {
              emlrtDimSizeImpxCheckR2021b(f_loop_ub, varargin_2, &o_emlrtECI,
                                          (emlrtConstCTX)sp);
            }
            st.site = &md_emlrtRSI;
            if (N->size[1] == varargin_2) {
              b_loop_ub = er->size[0] * er->size[1];
              er->size[0] = 1;
              er->size[1] = f_loop_ub;
              emxEnsureCapacity_real_T(&st, er, b_loop_ub, &qd_emlrtRTEI);
              er_data = er->data;
              for (t = 0; t < f_loop_ub; t++) {
                er_data[t] =
                    N_data[t] -
                    Dn->contents->data[c_d + Dn->contents->size[0] * t];
              }
            } else {
              b_st.site = &md_emlrtRSI;
              binary_expand_op_2(&b_st, er, N, Dn, c_d);
              er_data = er->data;
            }
            /*  N=ONER(N,r) */
            /*  Decrement element in position of r of input vector */
            /*  */
            /*  Copyright (c) 2012-2026, Imperial College London */
            /*  All rights reserved. */
            if ((b_d + 1 < 1) || (b_d + 1 > er->size[1])) {
              emlrtDynamicBoundsCheckR2012b(b_d + 1, 1, er->size[1],
                                            &xc_emlrtBCI, &st);
            }
            er_data[b_d]--;
            st.site = &md_emlrtRSI;
            hash(&st, Dn, M, N, er, ((real_T)k + 1.0) + 1.0,
                 (real_T *)&col_data);
            if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
              emlrtIntegerCheckR2012b(col_data, &lb_emlrtDCI,
                                      (emlrtConstCTX)sp);
            }
            if (((int32_T)col_data < 1) || ((int32_T)col_data > A->size[1])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, A->size[1],
                                            &fd_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (((int32_T)row < 1) || ((int32_T)row > A->size[0])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)row, 1, A->size[0],
                                            &tb_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (((int32_T)((uint32_T)k + 1U) < 1) ||
                ((int32_T)((uint32_T)k + 1U) > L->size[0])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)k + 1U), 1,
                                            L->size[0], &gd_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if ((b_d + 1 < 1) || (b_d + 1 > L->size[1])) {
              emlrtDynamicBoundsCheckR2012b(b_d + 1, 1, L->size[1],
                                            &gd_emlrtBCI, (emlrtConstCTX)sp);
            }
            A_data[((int32_T)row + A->size[0] * ((int32_T)col_data - 1)) - 1] =
                -L_data[k + L->size[0] * b_d];
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b((emlrtConstCTX)sp);
            }
          }
          if (c_d + 1 > varargin_1) {
            emlrtDynamicBoundsCheckR2012b(c_d + 1, 1, varargin_1, &ec_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if ((N->size[1] != varargin_2) &&
              ((N->size[1] != 1) && (varargin_2 != 1))) {
            emlrtDimSizeImpxCheckR2021b(N->size[1], varargin_2, &p_emlrtECI,
                                        (emlrtConstCTX)sp);
          }
          if (N->size[1] == varargin_2) {
            b_loop_ub = er->size[0] * er->size[1];
            er->size[0] = 1;
            er->size[1] = N->size[1];
            emxEnsureCapacity_real_T(sp, er, b_loop_ub, &pd_emlrtRTEI);
            er_data = er->data;
            for (t = 0; t < f_loop_ub; t++) {
              er_data[t] = N_data[t] -
                           Dn->contents->data[c_d + Dn->contents->size[0] * t];
            }
            st.site = &gd_emlrtRSI;
            hash(&st, Dn, M, N, er, ((real_T)k + 1.0) + 1.0,
                 (real_T *)&col_data);
          } else {
            st.site = &gd_emlrtRSI;
            binary_expand_op_5(&st, (real_T *)&col_data, gd_emlrtRSI, Dn, M, N,
                               c_d, k);
          }
          if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
            emlrtIntegerCheckR2012b(col_data, &ib_emlrtDCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)col_data < 1) || ((int32_T)col_data > B->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, B->size[1],
                                          &ad_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)row < 1) || ((int32_T)row > B->size[0])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)row, 1, B->size[0],
                                          &dc_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)((uint32_T)k + 1U) < 1) ||
              ((int32_T)((uint32_T)k + 1U) > L->size[0])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)k + 1U), 1,
                                          L->size[0], &bd_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if (((int32_T)r < 1) || ((int32_T)r > L->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)r, 1, L->size[1],
                                          &bd_emlrtBCI, (emlrtConstCTX)sp);
          }
          B_data[((int32_T)row + B->size[0] * ((int32_T)col_data - 1)) - 1] =
              L_data[k + L->size[0] * ((int32_T)r - 1)];
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b((emlrtConstCTX)sp);
          }
        }
        c_loop_ub = (int32_T)(r - 1.0);
        if ((int32_T)(r - 1.0) - 1 >= 0) {
          d1 = d;
          i6 = (int32_T)d;
          i7 = varargin_2;
          h_loop_ub = N->size[1];
        }
        for (k = 0; k < c_loop_ub; k++) {
          boolean_T b;
          /*  add PC to A */
          row++;
          if (c_d + 1 > varargin_1) {
            emlrtDynamicBoundsCheckR2012b(c_d + 1, 1, varargin_1, &jb_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          if ((h_loop_ub != i7) && ((h_loop_ub != 1) && (i7 != 1))) {
            emlrtDimSizeImpxCheckR2021b(h_loop_ub, i7, &k_emlrtECI,
                                        (emlrtConstCTX)sp);
          }
          if (h_loop_ub == i7) {
            b_loop_ub = b_n->size[0] * b_n->size[1];
            b_n->size[0] = 1;
            b_n->size[1] = h_loop_ub;
            emxEnsureCapacity_real_T(sp, b_n, b_loop_ub, &jd_emlrtRTEI);
            n_data = b_n->data;
            for (t = 0; t < h_loop_ub; t++) {
              n_data[t] = N_data[t] -
                          Dn->contents->data[c_d + Dn->contents->size[0] * t];
            }
          } else {
            st.site = &vg_emlrtRSI;
            binary_expand_op_2(&st, b_n, N, Dn, c_d);
            n_data = b_n->data;
          }
          st.site = &ld_emlrtRSI;
          b_hash(&st, Dn, M, N, b_n, (real_T *)&col_data);
          if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
            emlrtIntegerCheckR2012b(col_data, &db_emlrtDCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)col_data < 1) || ((int32_T)col_data > A->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, A->size[1],
                                          &oc_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)row < 1) || ((int32_T)row > A->size[0])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)row, 1, A->size[0],
                                          &vb_emlrtBCI, (emlrtConstCTX)sp);
          }
          loop_ub = b_n->size[1];
          if ((k + 1 < 1) || (k + 1 > b_n->size[1])) {
            emlrtDynamicBoundsCheckR2012b(k + 1, 1, b_n->size[1], &pc_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          A_tmp = n_data[k];
          A_data[((int32_T)row + A->size[0] * ((int32_T)col_data - 1)) - 1] =
              A_tmp;
          st.site = &kd_emlrtRSI;
          b_loop_ub = er->size[0] * er->size[1];
          er->size[0] = 1;
          er->size[1] = b_n->size[1];
          emxEnsureCapacity_real_T(&st, er, b_loop_ub, &nd_emlrtRTEI);
          er_data = er->data;
          for (t = 0; t < loop_ub; t++) {
            er_data[t] = n_data[t];
          }
          /*  N=ONER(N,r) */
          /*  Decrement element in position of r of input vector */
          /*  */
          /*  Copyright (c) 2012-2026, Imperial College London */
          /*  All rights reserved. */
          b = ((k + 1 < 1) || (k + 1 > b_n->size[1]));
          if (b) {
            emlrtDynamicBoundsCheckR2012b(k + 1, 1, b_n->size[1], &xc_emlrtBCI,
                                          &st);
          }
          if ((k + 1 < 1) || (k + 1 > b_n->size[1])) {
            emlrtDynamicBoundsCheckR2012b(k + 1, 1, b_n->size[1], &yc_emlrtBCI,
                                          &st);
          }
          er_data[k] = A_tmp - 1.0;
          st.site = &kd_emlrtRSI;
          b_hash(&st, Dn, M, N, er, (real_T *)&col_data);
          if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
            emlrtIntegerCheckR2012b(col_data, &kb_emlrtDCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)col_data < 1) || ((int32_T)col_data > A->size[1])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, A->size[1],
                                          &dd_emlrtBCI, (emlrtConstCTX)sp);
          }
          if (((int32_T)row < 1) || ((int32_T)row > A->size[0])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)row, 1, A->size[0],
                                          &wb_emlrtBCI, (emlrtConstCTX)sp);
          }
          b_loop_ub = Z->size[0] * Z->size[1];
          if ((k + 1 < 1) || (k + 1 > b_loop_ub)) {
            emlrtDynamicBoundsCheckR2012b(k + 1, 1, b_loop_ub, &ed_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          A_data[((int32_T)row + A->size[0] * ((int32_T)col_data - 1)) - 1] =
              -Z_data[k];
          emlrtForLoopVectorCheckR2021a(1.0, 1.0, d1, mxDOUBLE_CLASS,
                                        (int32_T)d1, &y_emlrtRTEI,
                                        (emlrtConstCTX)sp);
          if (i6 - 1 >= 0) {
            i_loop_ub = b_n->size[1];
          }
          for (b_d = 0; b_d < i6; b_d++) {
            st.site = &jd_emlrtRSI;
            b_loop_ub = er->size[0] * er->size[1];
            er->size[0] = 1;
            er->size[1] = loop_ub;
            emxEnsureCapacity_real_T(&st, er, b_loop_ub, &nd_emlrtRTEI);
            er_data = er->data;
            for (t = 0; t < i_loop_ub; t++) {
              er_data[t] = n_data[t];
            }
            /*  N=ONER(N,r) */
            /*  Decrement element in position of r of input vector */
            /*  */
            /*  Copyright (c) 2012-2026, Imperial College London */
            /*  All rights reserved. */
            if ((k + 1 < 1) || (k + 1 > loop_ub)) {
              emlrtDynamicBoundsCheckR2012b(k + 1, 1, loop_ub, &yc_emlrtBCI,
                                            &st);
            }
            er_data[k] = A_tmp - 1.0;
            st.site = &jd_emlrtRSI;
            hash(&st, Dn, M, N, er, ((real_T)b_d + 1.0) + 1.0,
                 (real_T *)&col_data);
            if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
              emlrtIntegerCheckR2012b(col_data, &mb_emlrtDCI,
                                      (emlrtConstCTX)sp);
            }
            if (((int32_T)col_data < 1) || ((int32_T)col_data > A->size[1])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, A->size[1],
                                            &hd_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (((int32_T)row < 1) || ((int32_T)row > A->size[0])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)row, 1, A->size[0],
                                            &xb_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (((int32_T)((uint32_T)b_d + 1U) < 1) ||
                ((int32_T)((uint32_T)b_d + 1U) > L->size[0])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_d + 1U), 1,
                                            L->size[0], &id_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if ((k + 1 < 1) || (k + 1 > L->size[1])) {
              emlrtDynamicBoundsCheckR2012b(k + 1, 1, L->size[1], &id_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            A_data[((int32_T)row + A->size[0] * ((int32_T)col_data - 1)) - 1] =
                -L_data[b_d + L->size[0] * k];
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b((emlrtConstCTX)sp);
            }
          }
          if (((int32_T)row < 1) || ((int32_T)row > B->size[0])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)row, 1, B->size[0],
                                          &fc_emlrtBCI, (emlrtConstCTX)sp);
          }
          b_loop_ub = B->size[1];
          for (t = 0; t < b_loop_ub; t++) {
            B_data[((int32_T)row + B->size[0] * t) - 1] = 0.0;
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b((emlrtConstCTX)sp);
          }
        }
      }
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  emxFree_real_T(sp, &c_n);
  /* add PC of class R */
  if ((varargin_1 == 0) || (varargin_2 == 0)) {
    c_loop_ub = 0;
  } else {
    c_loop_ub = muIntScalarMax_sint32(varargin_1, varargin_2);
    col_data = R->contents - 1.0;
    if (r > col_data) {
      i2 = 0;
      i3 = 0;
    } else {
      if (((int32_T)r < 1) || ((int32_T)r > varargin_2)) {
        emlrtDynamicBoundsCheckR2012b((int32_T)r, 1, varargin_2, &lb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      i2 = (int32_T)r - 1;
      if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
        emlrtIntegerCheckR2012b(col_data, &x_emlrtDCI, (emlrtConstCTX)sp);
      }
      if (((int32_T)col_data < 1) || ((int32_T)col_data > varargin_2)) {
        emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, varargin_2,
                                      &mb_emlrtBCI, (emlrtConstCTX)sp);
      }
      i3 = (int32_T)col_data;
    }
    e_loop_ub = i3 - i2;
  }
  for (b_d = 0; b_d < c_loop_ub; b_d++) {
    if (b_d + 1 > varargin_1) {
      emlrtDynamicBoundsCheckR2012b(b_d + 1, 1, varargin_1, &kb_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    loop_ub = er->size[0] * er->size[1];
    er->size[0] = 1;
    er->size[1] = i3 - i2;
    emxEnsureCapacity_real_T(sp, er, loop_ub, &dd_emlrtRTEI);
    er_data = er->data;
    for (t = 0; t < e_loop_ub; t++) {
      er_data[t] = Dn->contents->data[b_d + Dn->contents->size[0] * (i2 + t)];
    }
    st.site = &pd_emlrtRSI;
    if (sum(&st, er) <= 0.0) {
      row++;
      if (b_d + 1 > varargin_1) {
        emlrtDynamicBoundsCheckR2012b(b_d + 1, 1, varargin_1, &nb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      b_loop_ub = N->size[1];
      if ((N->size[1] != varargin_2) &&
          ((N->size[1] != 1) && (varargin_2 != 1))) {
        emlrtDimSizeImpxCheckR2021b(N->size[1], varargin_2, &l_emlrtECI,
                                    (emlrtConstCTX)sp);
      }
      if (N->size[1] == varargin_2) {
        loop_ub = b_n->size[0] * b_n->size[1];
        b_n->size[0] = 1;
        b_n->size[1] = N->size[1];
        emxEnsureCapacity_real_T(sp, b_n, loop_ub, &fd_emlrtRTEI);
        n_data = b_n->data;
        for (t = 0; t < b_loop_ub; t++) {
          n_data[t] =
              N_data[t] - Dn->contents->data[b_d + Dn->contents->size[0] * t];
        }
      } else {
        st.site = &ug_emlrtRSI;
        binary_expand_op_2(&st, b_n, N, Dn, b_d);
        n_data = b_n->data;
      }
      st.site = &id_emlrtRSI;
      b_hash(&st, Dn, M, N, b_n, (real_T *)&col_data);
      if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
        emlrtIntegerCheckR2012b(col_data, &ab_emlrtDCI, (emlrtConstCTX)sp);
      }
      if (((int32_T)col_data < 1) || ((int32_T)col_data > A->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, A->size[1],
                                      &kc_emlrtBCI, (emlrtConstCTX)sp);
      }
      if (((int32_T)row < 1) || ((int32_T)row > A->size[0])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)row, 1, A->size[0], &yb_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (((int32_T)r < 1) || ((int32_T)r > b_n->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)r, 1, b_n->size[1], &mc_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      A_data[((int32_T)row + A->size[0] * ((int32_T)col_data - 1)) - 1] =
          n_data[(int32_T)r - 1];
      st.site = &hd_emlrtRSI;
      b_hash(&st, Dn, M, N, b_n, (real_T *)&col_data);
      if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
        emlrtIntegerCheckR2012b(col_data, &cb_emlrtDCI, (emlrtConstCTX)sp);
      }
      if (((int32_T)col_data < 1) || ((int32_T)col_data > DA->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, DA->size[1],
                                      &nc_emlrtBCI, (emlrtConstCTX)sp);
      }
      if (((int32_T)row < 1) || ((int32_T)row > DA->size[0])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)row, 1, DA->size[0],
                                      &ac_emlrtBCI, (emlrtConstCTX)sp);
      }
      DA_data[((int32_T)row + DA->size[0] * ((int32_T)col_data - 1)) - 1] = 1.0;
      st.site = &fd_emlrtRSI;
      b_hash(&st, Dn, M, N, b_n, (real_T *)&col_data);
      if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
        emlrtIntegerCheckR2012b(col_data, &eb_emlrtDCI, (emlrtConstCTX)sp);
      }
      if (((int32_T)col_data < 1) || ((int32_T)col_data > B->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, B->size[1],
                                      &qc_emlrtBCI, (emlrtConstCTX)sp);
      }
      if (((int32_T)row < 1) || ((int32_T)row > B->size[0])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)row, 1, B->size[0], &gc_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      loop_ub = Z->size[0] * Z->size[1];
      if (((int32_T)r < 1) || ((int32_T)r > loop_ub)) {
        emlrtDynamicBoundsCheckR2012b((int32_T)r, 1, loop_ub, &sc_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      B_data[((int32_T)row + B->size[0] * ((int32_T)col_data - 1)) - 1] =
          Z_data[(int32_T)r - 1];
      loop_ub = (int32_T)d;
      emlrtForLoopVectorCheckR2021a(1.0, 1.0, d, mxDOUBLE_CLASS, (int32_T)d,
                                    &ab_emlrtRTEI, (emlrtConstCTX)sp);
      for (t = 0; t < loop_ub; t++) {
        st.site = &ed_emlrtRSI;
        hash(&st, Dn, M, N, b_n, ((real_T)t + 1.0) + 1.0, (real_T *)&col_data);
        if (col_data != (int32_T)muDoubleScalarFloor(col_data)) {
          emlrtIntegerCheckR2012b(col_data, &hb_emlrtDCI, (emlrtConstCTX)sp);
        }
        if (((int32_T)col_data < 1) || ((int32_T)col_data > B->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)col_data, 1, B->size[1],
                                        &vc_emlrtBCI, (emlrtConstCTX)sp);
        }
        if (((int32_T)row < 1) || ((int32_T)row > B->size[0])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)row, 1, B->size[0],
                                        &hc_emlrtBCI, (emlrtConstCTX)sp);
        }
        if (((int32_T)((uint32_T)t + 1U) < 1) ||
            ((int32_T)((uint32_T)t + 1U) > L->size[0])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)t + 1U), 1,
                                        L->size[0], &wc_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        if (((int32_T)r < 1) || ((int32_T)r > L->size[1])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)r, 1, L->size[1], &wc_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        B_data[((int32_T)row + B->size[0] * ((int32_T)col_data - 1)) - 1] =
            L_data[t + L->size[0] * ((int32_T)r - 1)];
        if (*emlrtBreakCheckR2012bFlagVar != 0) {
          emlrtBreakCheckR2012b((emlrtConstCTX)sp);
        }
      }
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  emxFree_real_T(sp, &b_n);
  emxFree_real_T(sp, &er);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

static int32_T hash(const emlrtStack *sp, const b_captured_var *Dn,
                    const captured_var *M, const emxArray_real_T *N,
                    const emxArray_real_T *n, real_T i, real_T col_data[])
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  emxArray_boolean_T *b_Dn;
  emxArray_boolean_T *r;
  emxArray_boolean_T *x;
  emxArray_real_T *row;
  const real_T *N_data;
  const real_T *n_data;
  real_T *row_data;
  int32_T b_i;
  int32_T col_size;
  int32_T hash_numThreads;
  int32_T i1;
  int32_T i2;
  boolean_T *Dn_data;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  n_data = n->data;
  N_data = N->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  emxInit_real_T(sp, &row, 2, &qc_emlrtRTEI);
  emxInit_boolean_T(sp, &x, 1, &tc_emlrtRTEI);
  emxInit_boolean_T(sp, &r, 2, &uc_emlrtRTEI);
  emxInit_boolean_T(sp, &b_Dn, 2, &sc_emlrtRTEI);
  if (i == 1.0) {
    real_T c_Dn;
    int32_T ii_data;
    int32_T loop_ub;
    int32_T vectorUB;
    loop_ub = N->size[1];
    if ((N->size[1] != n->size[1]) &&
        ((N->size[1] != 1) && (n->size[1] != 1))) {
      emlrtDimSizeImpxCheckR2021b(N->size[1], n->size[1], &e_emlrtECI,
                                  (emlrtConstCTX)sp);
    }
    st.site = &ob_emlrtRSI;
    if (N->size[1] == n->size[1]) {
      col_size = row->size[0] * row->size[1];
      row->size[0] = 1;
      row->size[1] = N->size[1];
      emxEnsureCapacity_real_T(&st, row, col_size, &rc_emlrtRTEI);
      row_data = row->data;
      col_size = (N->size[1] / 2) << 1;
      vectorUB = col_size - 2;
      for (b_i = 0; b_i <= vectorUB; b_i += 2) {
        _mm_storeu_pd(&row_data[b_i], _mm_sub_pd(_mm_loadu_pd(&N_data[b_i]),
                                                 _mm_loadu_pd(&n_data[b_i])));
      }
      for (b_i = col_size; b_i < loop_ub; b_i++) {
        row_data[b_i] = N_data[b_i] - n_data[b_i];
      }
    } else {
      b_st.site = &ob_emlrtRSI;
      minus(&b_st, row, N, n);
      row_data = row->data;
    }
    /*  pos = matchrow(M, r) */
    /*  Return position of row r in matrix M if unique, -1 otherwise */
    /*  */
    /*  Copyright (c) 2012-2026, Imperial College London */
    /*  All rights reserved. */
    if (Dn->contents->size[1] != row->size[1]) {
      /* Incompatible matrix and row sizes */
      ii_data = -2;
    } else {
      ii_data = Dn->contents->size[0];
      if (ii_data < 1) {
        emlrtDynamicBoundsCheckR2012b(ii_data, 1, ii_data, &u_emlrtBCI, &st);
      }
      col_size = Dn->contents->size[1];
      if ((col_size != row->size[1]) &&
          ((col_size != 1) && (row->size[1] != 1))) {
        emlrtDimSizeImpxCheckR2021b(col_size, row->size[1], &g_emlrtECI, &st);
      }
      col_size = b_Dn->size[0] * b_Dn->size[1];
      b_Dn->size[0] = 1;
      if (row->size[1] == 1) {
        loop_ub = Dn->contents->size[1];
      } else {
        loop_ub = row->size[1];
      }
      b_Dn->size[1] = loop_ub;
      emxEnsureCapacity_boolean_T(&st, b_Dn, col_size, &sc_emlrtRTEI);
      Dn_data = b_Dn->data;
      col_size = (Dn->contents->size[1] != 1);
      vectorUB = (row->size[1] != 1);
      if (loop_ub < 1600) {
        for (i2 = 0; i2 < loop_ub; i2++) {
          Dn_data[i2] =
              (Dn->contents
                   ->data[(ii_data + Dn->contents->size[0] * (i2 * col_size)) -
                          1] == row_data[i2 * vectorUB]);
        }
      } else {
        emlrtEnterParallelRegion(&st, omp_in_parallel());
        emlrtPushJmpBuf(&st, &emlrtJBStack);
        hash_numThreads =
            emlrtAllocRegionTLSs(st.tls, omp_in_parallel(),
                                 omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(hash_numThreads)

        for (i2 = 0; i2 < loop_ub; i2++) {
          Dn_data[i2] =
              (Dn->contents
                   ->data[(ii_data + Dn->contents->size[0] * (i2 * col_size)) -
                          1] == row_data[i2 * vectorUB]);
        }
        emlrtPopJmpBuf(&st, &emlrtJBStack);
        emlrtExitParallelRegion(&st, omp_in_parallel());
      }
      b_st.site = &qb_emlrtRSI;
      if (!all(&b_st, b_Dn)) {
        b_st.site = &rb_emlrtRSI;
        c_st.site = &rb_emlrtRSI;
        bsxfun(&c_st, Dn->contents, row, r);
        c_st.site = &rb_emlrtRSI;
        b_all(&c_st, r, x);
        c_st.site = &wb_emlrtRSI;
        col_size = eml_find(&c_st, x, (int32_T *)&ii_data);
        if (col_size == 0) {
          ii_data = -1;
        }
      }
    }
    c_Dn = (real_T)Dn->contents->size[0] * M->contents;
    col_size = 1;
    col_data[0] = c_Dn + (real_T)ii_data;
  } else {
    int32_T ii_data;
    int32_T loop_ub;
    int32_T vectorUB;
    loop_ub = N->size[1];
    if ((N->size[1] != n->size[1]) &&
        ((N->size[1] != 1) && (n->size[1] != 1))) {
      emlrtDimSizeImpxCheckR2021b(N->size[1], n->size[1], &f_emlrtECI,
                                  (emlrtConstCTX)sp);
    }
    st.site = &pb_emlrtRSI;
    if (N->size[1] == n->size[1]) {
      col_size = row->size[0] * row->size[1];
      row->size[0] = 1;
      row->size[1] = N->size[1];
      emxEnsureCapacity_real_T(&st, row, col_size, &qc_emlrtRTEI);
      row_data = row->data;
      col_size = (N->size[1] / 2) << 1;
      vectorUB = col_size - 2;
      for (b_i = 0; b_i <= vectorUB; b_i += 2) {
        _mm_storeu_pd(&row_data[b_i], _mm_sub_pd(_mm_loadu_pd(&N_data[b_i]),
                                                 _mm_loadu_pd(&n_data[b_i])));
      }
      for (b_i = col_size; b_i < loop_ub; b_i++) {
        row_data[b_i] = N_data[b_i] - n_data[b_i];
      }
    } else {
      b_st.site = &pb_emlrtRSI;
      minus(&b_st, row, N, n);
      row_data = row->data;
    }
    /*  pos = matchrow(M, r) */
    /*  Return position of row r in matrix M if unique, -1 otherwise */
    /*  */
    /*  Copyright (c) 2012-2026, Imperial College London */
    /*  All rights reserved. */
    if (Dn->contents->size[1] != row->size[1]) {
      /* Incompatible matrix and row sizes */
      ii_data = -2;
    } else {
      ii_data = Dn->contents->size[0];
      if (ii_data < 1) {
        emlrtDynamicBoundsCheckR2012b(ii_data, 1, ii_data, &u_emlrtBCI, &st);
      }
      col_size = Dn->contents->size[1];
      if ((col_size != row->size[1]) &&
          ((col_size != 1) && (row->size[1] != 1))) {
        emlrtDimSizeImpxCheckR2021b(col_size, row->size[1], &g_emlrtECI, &st);
      }
      col_size = b_Dn->size[0] * b_Dn->size[1];
      b_Dn->size[0] = 1;
      if (row->size[1] == 1) {
        loop_ub = Dn->contents->size[1];
      } else {
        loop_ub = row->size[1];
      }
      b_Dn->size[1] = loop_ub;
      emxEnsureCapacity_boolean_T(&st, b_Dn, col_size, &sc_emlrtRTEI);
      Dn_data = b_Dn->data;
      col_size = (Dn->contents->size[1] != 1);
      vectorUB = (row->size[1] != 1);
      if (loop_ub < 1600) {
        for (i1 = 0; i1 < loop_ub; i1++) {
          Dn_data[i1] =
              (Dn->contents
                   ->data[(ii_data + Dn->contents->size[0] * (i1 * col_size)) -
                          1] == row_data[i1 * vectorUB]);
        }
      } else {
        emlrtEnterParallelRegion(&st, omp_in_parallel());
        emlrtPushJmpBuf(&st, &emlrtJBStack);
        hash_numThreads =
            emlrtAllocRegionTLSs(st.tls, omp_in_parallel(),
                                 omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(hash_numThreads)

        for (i1 = 0; i1 < loop_ub; i1++) {
          Dn_data[i1] =
              (Dn->contents
                   ->data[(ii_data + Dn->contents->size[0] * (i1 * col_size)) -
                          1] == row_data[i1 * vectorUB]);
        }
        emlrtPopJmpBuf(&st, &emlrtJBStack);
        emlrtExitParallelRegion(&st, omp_in_parallel());
      }
      b_st.site = &qb_emlrtRSI;
      if (!all(&b_st, b_Dn)) {
        b_st.site = &rb_emlrtRSI;
        c_st.site = &rb_emlrtRSI;
        bsxfun(&c_st, Dn->contents, row, r);
        c_st.site = &rb_emlrtRSI;
        b_all(&c_st, r, x);
        c_st.site = &wb_emlrtRSI;
        col_size = eml_find(&c_st, x, (int32_T *)&ii_data);
        if (col_size == 0) {
          ii_data = -1;
        }
      }
    }
    col_size = 1;
    col_data[0] = (((real_T)ii_data - 1.0) * M->contents + i) - 1.0;
  }
  emxFree_boolean_T(sp, &b_Dn);
  emxFree_boolean_T(sp, &r);
  emxFree_boolean_T(sp, &x);
  emxFree_real_T(sp, &row);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return col_size;
}

static void minus(const emlrtStack *sp, emxArray_real_T *in1,
                  const emxArray_real_T *in2, const emxArray_real_T *in3)
{
  jmp_buf *volatile emlrtJBStack;
  const real_T *in2_data;
  const real_T *in3_data;
  real_T *in1_data;
  int32_T i;
  int32_T loop_ub;
  int32_T minus_numThreads;
  int32_T stride_0_1;
  int32_T stride_1_1;
  in3_data = in3->data;
  in2_data = in2->data;
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[0] = 1;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &rc_emlrtRTEI);
  if (in3->size[1] == 1) {
    loop_ub = in2->size[1];
  } else {
    loop_ub = in3->size[1];
  }
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &rc_emlrtRTEI);
  in1_data = in1->data;
  stride_0_1 = (in2->size[1] != 1);
  stride_1_1 = (in3->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      in1_data[i] = in2_data[i * stride_0_1] - in3_data[i * stride_1_1];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    minus_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(minus_numThreads)

    for (i = 0; i < loop_ub; i++) {
      in1_data[i] = in2_data[i * stride_0_1] - in3_data[i * stride_1_1];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
}

static void plus(const emlrtStack *sp, emxArray_real_T *in1,
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
  emxInit_real_T(sp, &b_in1, 2, &ke_emlrtRTEI);
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
  emxEnsureCapacity_real_T(sp, b_in1, stride_0_0, &ke_emlrtRTEI);
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
          in1_data[i1 * stride_0_0 + in1->size[0] * aux_0_1] +
          in2_data[i1 * stride_1_0 + in2->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_1;
    aux_0_1 += stride_0_1;
  }
  stride_0_0 = in1->size[0] * in1->size[1];
  in1->size[0] = loop_ub;
  in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_0, &ke_emlrtRTEI);
  in1_data = in1->data;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      in1_data[i1 + in1->size[0] * i] = b_in1_data[i1 + b_in1->size[0] * i];
    }
  }
  emxFree_real_T(sp, &b_in1);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

emlrtCTX emlrtGetRootTLSGlobal(void)
{
  return emlrtRootTLSGlobal;
}

void emlrtLockerFunction(EmlrtLockeeFunction aLockee, emlrtConstCTX aTLS,
                         void *aData)
{
  omp_set_lock(&emlrtLockGlobal);
  emlrtCallLockeeFunction(aLockee, aTLS, aData);
  omp_unset_lock(&emlrtLockGlobal);
}

real_T pfqn_comom(const emlrtStack *sp, emxArray_real_T *L,
                  const emxArray_real_T *N, const emxArray_real_T *Z,
                  real_T b_atol)
{
  __m128d r2;
  __m128d r3;
  ptrdiff_t k_t;
  ptrdiff_t lda_t;
  ptrdiff_t ldb_t;
  ptrdiff_t ldc_t;
  ptrdiff_t m_t;
  ptrdiff_t n_t;
  b_captured_var Dn;
  captured_var M;
  captured_var R;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack g_st;
  emlrtStack h_st;
  emlrtStack i_st;
  emlrtStack st;
  emxArray_int32_T *r;
  emxArray_real_T *B;
  emxArray_real_T *DA;
  emxArray_real_T *Lmax;
  emxArray_real_T *b;
  emxArray_real_T *b_Lmax;
  emxArray_real_T *b_Z;
  emxArray_real_T *i2;
  emxArray_real_T *lh;
  emxArray_real_T *scale;
  emxArray_real_T *v;
  const real_T *N_data;
  const real_T *Z_data;
  real_T lG;
  real_T s;
  real_T *DA_data;
  real_T *L_data;
  real_T *Lmax_data;
  real_T *b_Lmax_data;
  real_T *b_data;
  real_T *lh_data;
  real_T *v_data;
  int32_T iv[2];
  int32_T iv1[2];
  int32_T b_i;
  int32_T b_loop_ub;
  int32_T c_loop_ub;
  int32_T d_loop_ub;
  int32_T e_loop_ub;
  int32_T i;
  int32_T j;
  int32_T loop_ub;
  int32_T m;
  int32_T n;
  int32_T nfb;
  int32_T *r1;
  char_T TRANSA1;
  char_T TRANSB1;
  boolean_T overflow;
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
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  st.site = &emlrtRSI;
  b_st.site = &s_emlrtRSI;
  st.site = &emlrtRSI;
  b_st.site = &s_emlrtRSI;
  st.site = &emlrtRSI;
  b_st.site = &s_emlrtRSI;
  st.site = &emlrtRSI;
  b_st.site = &s_emlrtRSI;
  /* { */
  /*  % @file pfqn_comom.m */
  /*  % @brief CoMoM algorithm for computing the normalizing constant. */
  /* } */
  /* { */
  /*  % @brief CoMoM algorithm for computing the normalizing constant. */
  /*  % @fn pfqn_comom(L, N, Z, atol) */
  /*  % @param L Service demand matrix. */
  /*  % @param N Population vector. */
  /*  % @param Z Think time vector. */
  /*  % @param atol Tolerance. */
  /*  % @return lG Logarithm of the normalizing constant. */
  /* } */
  M.contents = L->size[0];
  R.contents = L->size[1];
  /*  rescale demands */
  emxInit_real_T(sp, &Lmax, 2, &gb_emlrtRTEI);
  nfb = Lmax->size[0] * Lmax->size[1];
  Lmax->size[0] = L->size[0];
  Lmax->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, Lmax, nfb, &gb_emlrtRTEI);
  Lmax_data = Lmax->data;
  loop_ub = L->size[0] * L->size[1];
  for (i = 0; i < loop_ub; i++) {
    Lmax_data[i] = L_data[i];
  }
  /*  use L */
  m = 0;
  for (i = 0; i < loop_ub; i++) {
    if (L_data[i] < b_atol) {
      m++;
    }
  }
  emxInit_int32_T(sp, &r, 1, &ic_emlrtRTEI);
  nfb = r->size[0];
  r->size[0] = m;
  emxEnsureCapacity_int32_T(sp, r, nfb, &hb_emlrtRTEI);
  r1 = r->data;
  nfb = 0;
  for (i = 0; i < loop_ub; i++) {
    if (L_data[i] < b_atol) {
      r1[nfb] = i;
      nfb++;
    }
  }
  nfb = r->size[0];
  for (i = 0; i < nfb; i++) {
    if (r1[i] > Z->size[1] - 1) {
      emlrtDynamicBoundsCheckR2012b(r1[i], 0, Z->size[1] - 1, &m_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
  }
  for (i = 0; i < nfb; i++) {
    if (r1[i] > loop_ub - 1) {
      emlrtDynamicBoundsCheckR2012b(r1[i], 0, loop_ub - 1, &n_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    Lmax_data[r1[i]] = Z_data[r1[i]];
  }
  emxFree_int32_T(sp, &r);
  /*  unless zero */
  st.site = &b_emlrtRSI;
  b_st.site = &t_emlrtRSI;
  c_st.site = &u_emlrtRSI;
  d_st.site = &v_emlrtRSI;
  if (Lmax->size[0] < 1) {
    emlrtErrorWithMessageIdR2018a(&d_st, &f_emlrtRTEI,
                                  "Coder:toolbox:eml_min_or_max_varDimZero",
                                  "Coder:toolbox:eml_min_or_max_varDimZero", 0);
  }
  e_st.site = &w_emlrtRSI;
  f_st.site = &x_emlrtRSI;
  g_st.site = &y_emlrtRSI;
  m = Lmax->size[0];
  n = Lmax->size[1];
  emxInit_real_T(&g_st, &b_Lmax, 2, &gb_emlrtRTEI);
  nfb = b_Lmax->size[0] * b_Lmax->size[1];
  b_Lmax->size[0] = 1;
  b_Lmax->size[1] = Lmax->size[1];
  emxEnsureCapacity_real_T(&g_st, b_Lmax, nfb, &ib_emlrtRTEI);
  b_Lmax_data = b_Lmax->data;
  if (Lmax->size[1] >= 1) {
    h_st.site = &bb_emlrtRSI;
    if (Lmax->size[1] > 2147483646) {
      i_st.site = &cb_emlrtRSI;
      check_forloop_overflow_error(&i_st);
    }
    overflow = (Lmax->size[0] > 2147483646);
    for (i = 0; i < n; i++) {
      b_Lmax_data[i] = Lmax_data[Lmax->size[0] * i];
      h_st.site = &ab_emlrtRSI;
      if (overflow) {
        i_st.site = &cb_emlrtRSI;
        check_forloop_overflow_error(&i_st);
      }
      for (b_i = 2; b_i <= m; b_i++) {
        boolean_T p;
        lG = b_Lmax_data[i];
        s = Lmax_data[(b_i + Lmax->size[0] * i) - 1];
        if (muDoubleScalarIsNaN(s)) {
          p = false;
        } else if (muDoubleScalarIsNaN(lG)) {
          p = true;
        } else {
          p = (lG < s);
        }
        if (p) {
          b_Lmax_data[i] = s;
        }
      }
    }
  }
  st.site = &c_emlrtRSI;
  b_st.site = &c_emlrtRSI;
  repmat(&b_st, b_Lmax, L->size[0], Lmax);
  Lmax_data = Lmax->data;
  b_st.site = &gb_emlrtRSI;
  c_st.site = &hb_emlrtRSI;
  assertCompatibleDims(&c_st, L, Lmax);
  if ((L->size[0] == Lmax->size[0]) && (L->size[1] == Lmax->size[1])) {
    nfb = (loop_ub / 2) << 1;
    m = nfb - 2;
    for (i = 0; i <= m; i += 2) {
      r2 = _mm_loadu_pd(&L_data[i]);
      r3 = _mm_loadu_pd(&Lmax_data[i]);
      _mm_storeu_pd(&L_data[i], _mm_div_pd(r2, r3));
    }
    for (i = nfb; i < loop_ub; i++) {
      L_data[i] /= Lmax_data[i];
    }
  } else {
    c_st.site = &tg_emlrtRSI;
    b_rdivide(&c_st, L, Lmax);
  }
  st.site = &d_emlrtRSI;
  emxInit_real_T(&st, &b_Z, 2, &hc_emlrtRTEI);
  b_st.site = &d_emlrtRSI;
  repmat(&b_st, b_Lmax, M.contents, b_Z);
  L_data = b_Z->data;
  b_st.site = &gb_emlrtRSI;
  c_st.site = &hb_emlrtRSI;
  b_assertCompatibleDims(&c_st, Z, b_Z);
  if (Z->size[1] == b_Z->size[1]) {
    n = b_Z->size[0];
    nfb = Lmax->size[0] * Lmax->size[1];
    Lmax->size[0] = b_Z->size[0];
    loop_ub = Z->size[1];
    Lmax->size[1] = Z->size[1];
    emxEnsureCapacity_real_T(&b_st, Lmax, nfb, &jb_emlrtRTEI);
    Lmax_data = Lmax->data;
    for (i = 0; i < loop_ub; i++) {
      nfb = (b_Z->size[0] / 2) << 1;
      m = nfb - 2;
      for (b_i = 0; b_i <= m; b_i += 2) {
        r2 = _mm_loadu_pd(&L_data[b_i + b_Z->size[0] * i]);
        _mm_storeu_pd(&Lmax_data[b_i + Lmax->size[0] * i],
                      _mm_div_pd(_mm_set1_pd(Z_data[i]), r2));
      }
      for (b_i = nfb; b_i < n; b_i++) {
        Lmax_data[b_i + Lmax->size[0] * i] =
            Z_data[i] / L_data[b_i + b_Z->size[0] * i];
      }
    }
    nfb = b_Z->size[0] * b_Z->size[1];
    b_Z->size[0] = Lmax->size[0];
    b_Z->size[1] = Lmax->size[1];
    emxEnsureCapacity_real_T(&b_st, b_Z, nfb, &kb_emlrtRTEI);
    L_data = b_Z->data;
    nfb = Lmax->size[0] * Lmax->size[1];
    for (i = 0; i < nfb; i++) {
      L_data[i] = Lmax_data[i];
    }
  } else {
    c_st.site = &tg_emlrtRSI;
    rdivide(&c_st, b_Z, Z);
  }
  /*  sort from smallest to largest */
  /* [~,rsort] = sort(Z,'ascend'); */
  /* L=L(:,rsort); */
  /* Z=Z(:,rsort); */
  /*  prepare comom data structures */
  st.site = &e_emlrtRSI;
  multichoose(&st, R.contents, M.contents, Lmax);
  Lmax_data = Lmax->data;
  b_loop_ub = Lmax->size[0];
  emxInitStruct_captured_var(sp, &Dn, &lb_emlrtRTEI);
  nfb = Dn.contents->size[0] * Dn.contents->size[1];
  Dn.contents->size[0] = Lmax->size[0];
  c_loop_ub = Lmax->size[1];
  Dn.contents->size[1] = Lmax->size[1];
  emxEnsureCapacity_real_T(sp, Dn.contents, nfb, &lb_emlrtRTEI);
  d_loop_ub = Lmax->size[0] * Lmax->size[1];
  for (i = 0; i < d_loop_ub; i++) {
    Dn.contents->data[i] = Lmax_data[i];
  }
  e_loop_ub = (int32_T)R.contents;
  if ((R.contents < 1.0) || (e_loop_ub > Lmax->size[1])) {
    emlrtDynamicBoundsCheckR2012b(e_loop_ub, 1, Lmax->size[1], &l_emlrtBCI,
                                  (emlrtConstCTX)sp);
  }
  for (i = 0; i < b_loop_ub; i++) {
    Dn.contents->data[i + Dn.contents->size[0] * (e_loop_ub - 1)] = 0.0;
  }
  st.site = &f_emlrtRSI;
  nfb = Lmax->size[0] * Lmax->size[1];
  Lmax->size[0] = b_loop_ub;
  Lmax->size[1] = c_loop_ub;
  emxEnsureCapacity_real_T(&st, Lmax, nfb, &mb_emlrtRTEI);
  Lmax_data = Lmax->data;
  for (i = 0; i < d_loop_ub; i++) {
    Lmax_data[i] = Dn.contents->data[i];
  }
  /*  sorts a set of combinations with repetition according to the number of */
  /*  nonzeros */
  emxInit_real_T(&st, &v, 2, &sb_emlrtRTEI);
  emxInit_real_T(&st, &i2, 2, &qb_emlrtRTEI);
  for (b_i = 0; b_i <= b_loop_ub - 2; b_i++) {
    loop_ub = Lmax->size[0] - b_i;
    for (j = 0; j <= loop_ub - 2; j++) {
      uint32_T b_j;
      b_j = ((uint32_T)b_i + (uint32_T)j) + 2U;
      b_st.site = &mb_emlrtRSI;
      if (b_i + 1 > b_loop_ub) {
        emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, b_loop_ub, &i_emlrtBCI,
                                      &b_st);
      }
      nfb = v->size[0] * v->size[1];
      v->size[0] = 1;
      v->size[1] = c_loop_ub;
      emxEnsureCapacity_real_T(&b_st, v, nfb, &pb_emlrtRTEI);
      v_data = v->data;
      for (i = 0; i < c_loop_ub; i++) {
        v_data[i] = Lmax_data[b_i + Lmax->size[0] * i];
      }
      overflow = (((int32_T)b_j < 1) || ((int32_T)b_j > b_loop_ub));
      if (overflow) {
        emlrtDynamicBoundsCheckR2012b((int32_T)b_j, 1, b_loop_ub, &h_emlrtBCI,
                                      &b_st);
      }
      nfb = i2->size[0] * i2->size[1];
      i2->size[0] = 1;
      i2->size[1] = c_loop_ub;
      emxEnsureCapacity_real_T(&b_st, i2, nfb, &qb_emlrtRTEI);
      L_data = i2->data;
      for (i = 0; i < c_loop_ub; i++) {
        L_data[i] = Lmax_data[((int32_T)b_j + Lmax->size[0] * i) - 1];
      }
      /*  return 1 if i1<i2 */
      m = 0;
      n = 0;
      for (i = 0; i < c_loop_ub; i++) {
        if (v_data[i] != 0.0) {
          m++;
        }
        if (L_data[i] != 0.0) {
          n++;
        }
      }
      if (m > n) {
        nfb = 1;
        /*  i2 has more zeros and is thus greater */
      } else if (m < n) {
        nfb = 0;
        /*  i1 has more zeros and is thus greater */
      } else {
        /* nnz1==nnz2 */
        nfb = 0;
        int32_T exitg1;
        do {
          exitg1 = 0;
          if (nfb <= Lmax->size[1] - 1) {
            boolean_T guard1;
            if (nfb + 1 > c_loop_ub) {
              emlrtDynamicBoundsCheckR2012b(nfb + 1, 1, c_loop_ub, &d_emlrtBCI,
                                            &b_st);
            }
            guard1 = false;
            if (v_data[nfb] == 0.0) {
              if (nfb + 1 > c_loop_ub) {
                emlrtDynamicBoundsCheckR2012b(nfb + 1, 1, c_loop_ub,
                                              &c_emlrtBCI, &b_st);
              }
              if (L_data[nfb] > 0.0) {
                nfb = 1;
                /*  i2 has the left-most zero */
                exitg1 = 1;
              } else {
                guard1 = true;
              }
            } else {
              guard1 = true;
            }
            if (guard1) {
              if (nfb + 1 > c_loop_ub) {
                emlrtDynamicBoundsCheckR2012b(nfb + 1, 1, c_loop_ub,
                                              &b_emlrtBCI, &b_st);
              }
              if (v_data[nfb] > 0.0) {
                if (nfb + 1 > c_loop_ub) {
                  emlrtDynamicBoundsCheckR2012b(nfb + 1, 1, c_loop_ub,
                                                &emlrtBCI, &b_st);
                }
                if (L_data[nfb] == 0.0) {
                  nfb = 0;
                  exitg1 = 1;
                } else {
                  nfb++;
                }
              } else {
                nfb++;
              }
            }
          } else {
            nfb = 0;
            exitg1 = 1;
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&b_st);
          }
        } while (exitg1 == 0);
      }
      if (nfb == 1) {
        if (b_i + 1 > b_loop_ub) {
          emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, b_loop_ub, &g_emlrtBCI,
                                        &st);
        }
        nfb = v->size[0] * v->size[1];
        v->size[0] = 1;
        v->size[1] = c_loop_ub;
        emxEnsureCapacity_real_T(&st, v, nfb, &sb_emlrtRTEI);
        v_data = v->data;
        for (i = 0; i < c_loop_ub; i++) {
          v_data[i] = Lmax_data[b_i + Lmax->size[0] * i];
        }
        if (b_i + 1 > b_loop_ub) {
          emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, b_loop_ub, &e_emlrtBCI,
                                        &st);
        }
        if (overflow) {
          emlrtDynamicBoundsCheckR2012b((int32_T)b_j, 1, b_loop_ub, &f_emlrtBCI,
                                        &st);
        }
        iv[0] = 1;
        iv[1] = c_loop_ub;
        iv1[0] = 1;
        iv1[1] = c_loop_ub;
        emlrtSubAssignSizeCheckR2012b(&iv[0], 2, &iv1[0], 2, &b_emlrtECI, &st);
        nfb = i2->size[0] * i2->size[1];
        i2->size[0] = 1;
        i2->size[1] = c_loop_ub;
        emxEnsureCapacity_real_T(&st, i2, nfb, &tb_emlrtRTEI);
        L_data = i2->data;
        for (i = 0; i < c_loop_ub; i++) {
          L_data[i] = Lmax_data[((int32_T)b_j + Lmax->size[0] * i) - 1];
        }
        for (i = 0; i < c_loop_ub; i++) {
          Lmax_data[b_i + Lmax->size[0] * i] = L_data[i];
        }
        iv[0] = 1;
        iv[1] = c_loop_ub;
        emlrtSubAssignSizeCheckR2012b(&iv[0], 2, &v->size[0], 2, &emlrtECI,
                                      &st);
        for (i = 0; i < c_loop_ub; i++) {
          Lmax_data[((int32_T)b_j + Lmax->size[0] * i) - 1] = v_data[i];
        }
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b(&st);
      }
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b(&st);
    }
  }
  nfb = Dn.contents->size[0] * Dn.contents->size[1];
  Dn.contents->size[0] = b_loop_ub;
  Dn.contents->size[1] = c_loop_ub;
  emxEnsureCapacity_real_T(sp, Dn.contents, nfb, &nb_emlrtRTEI);
  for (i = 0; i < d_loop_ub; i++) {
    Dn.contents->data[i] = Lmax_data[i];
  }
  /*  initialize */
  nfb = v->size[0] * v->size[1];
  v->size[0] = 1;
  v->size[1] = e_loop_ub;
  emxEnsureCapacity_real_T(sp, v, nfb, &ob_emlrtRTEI);
  v_data = v->data;
  for (i = 0; i < e_loop_ub; i++) {
    v_data[i] = 0.0;
  }
  st.site = &g_emlrtRSI;
  emxInit_real_T(&st, &lh, 1, &fc_emlrtRTEI);
  lG = (real_T)Lmax->size[0] * (M.contents + 1.0);
  if (lG != (int32_T)lG) {
    emlrtIntegerCheckR2012b(lG, &g_emlrtDCI, &st);
  }
  m = (int32_T)lG;
  nfb = lh->size[0];
  lh->size[0] = (int32_T)lG;
  emxEnsureCapacity_real_T(&st, lh, nfb, &rb_emlrtRTEI);
  lh_data = lh->data;
  if (lG != (int32_T)lG) {
    emlrtIntegerCheckR2012b(lG, &g_emlrtDCI, &st);
  }
  for (i = 0; i < m; i++) {
    lh_data[i] = 0.0;
  }
  nfb = (int32_T)M.contents;
  emlrtForLoopVectorCheckR2021a(0.0, 1.0, M.contents, mxDOUBLE_CLASS, nfb + 1,
                                &e_emlrtRTEI, &st);
  for (i = 0; i <= nfb; i++) {
    b_st.site = &nb_emlrtRSI;
    hash(&b_st, &Dn, &M, N, N, (real_T)i + 1.0, (real_T *)&lG);
    if (lG != (int32_T)muDoubleScalarFloor(lG)) {
      emlrtIntegerCheckR2012b(lG, &h_emlrtDCI, &st);
    }
    if (((int32_T)lG < 1) || ((int32_T)lG > lh->size[0])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)lG, 1, lh->size[0], &o_emlrtBCI,
                                    &st);
    }
    lh_data[(int32_T)lG - 1] = 1.0;
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b(&st);
    }
  }
  st.site = &h_emlrtRSI;
  /*  lf=FACTLN(n) */
  /*  Compure the logarithm of n!        */
  /*  */
  /*  Copyright (c) 2012-2026, Imperial College London */
  /*  All rights reserved.   */
  b_st.site = &oc_emlrtRSI;
  c_st.site = &pc_emlrtRSI;
  lG = (M.contents - 1.0) + 1.0;
  d_st.site = &qc_emlrtRSI;
  scalar_gammaln(&d_st, &lG);
  st.site = &h_emlrtRSI;
  /*  lf=FACTLN(n) */
  /*  Compure the logarithm of n!        */
  /*  */
  /*  Copyright (c) 2012-2026, Imperial College London */
  /*  All rights reserved.   */
  b_st.site = &oc_emlrtRSI;
  nfb = i2->size[0] * i2->size[1];
  i2->size[0] = 1;
  i2->size[1] = e_loop_ub;
  emxEnsureCapacity_real_T(&b_st, i2, nfb, &ub_emlrtRTEI);
  L_data = i2->data;
  for (i = 0; i < e_loop_ub; i++) {
    L_data[i] = 1.0;
  }
  c_st.site = &pc_emlrtRSI;
  applyScalarFunctionInPlace(&c_st, i2);
  st.site = &h_emlrtRSI;
  b_log(&st, lh);
  lh_data = lh->data;
  st.site = &h_emlrtRSI;
  s = sum(&st, i2);
  nfb = lh->size[0];
  m = (lh->size[0] / 2) << 1;
  n = m - 2;
  for (i = 0; i <= n; i += 2) {
    r2 = _mm_loadu_pd(&lh_data[i]);
    _mm_storeu_pd(&lh_data[i],
                  _mm_sub_pd(_mm_add_pd(r2, _mm_set1_pd(lG)), _mm_set1_pd(s)));
  }
  for (i = m; i < nfb; i++) {
    lh_data[i] = (lh_data[i] + lG) - s;
  }
  st.site = &i_emlrtRSI;
  b_exp(&st, lh);
  lh_data = lh->data;
  st.site = &j_emlrtRSI;
  lG = sum(&st, N);
  if (!(lG >= 0.0)) {
    emlrtNonNegativeCheckR2012b(lG, &f_emlrtDCI, (emlrtConstCTX)sp);
  }
  if (lG != (int32_T)muDoubleScalarFloor(lG)) {
    emlrtIntegerCheckR2012b(lG, &e_emlrtDCI, (emlrtConstCTX)sp);
  }
  emxInit_real_T(sp, &scale, 2, &vb_emlrtRTEI);
  nfb = scale->size[0] * scale->size[1];
  scale->size[0] = 1;
  m = (int32_T)lG;
  scale->size[1] = (int32_T)lG;
  emxEnsureCapacity_real_T(sp, scale, nfb, &vb_emlrtRTEI);
  L_data = scale->data;
  for (i = 0; i < m; i++) {
    L_data[i] = 0.0;
  }
  st.site = &k_emlrtRSI;
  lG = nchoosek(&st, (M.contents + R.contents) - 1.0, M.contents) *
       (M.contents + 1.0);
  if (!(lG >= 0.0)) {
    emlrtNonNegativeCheckR2012b(lG, &d_emlrtDCI, (emlrtConstCTX)sp);
  }
  n = (int32_T)muDoubleScalarFloor(lG);
  if (lG != n) {
    emlrtIntegerCheckR2012b(lG, &c_emlrtDCI, (emlrtConstCTX)sp);
  }
  nfb = Lmax->size[0] * Lmax->size[1];
  Lmax->size[0] = (int32_T)lG;
  Lmax->size[1] = (int32_T)lG;
  emxEnsureCapacity_real_T(sp, Lmax, nfb, &wb_emlrtRTEI);
  Lmax_data = Lmax->data;
  loop_ub = (int32_T)lG * (int32_T)lG;
  for (i = 0; i < loop_ub; i++) {
    Lmax_data[i] = 0.0;
  }
  if (lG != n) {
    emlrtIntegerCheckR2012b(lG, &b_emlrtDCI, (emlrtConstCTX)sp);
  }
  emxInit_real_T(sp, &B, 2, &xb_emlrtRTEI);
  nfb = B->size[0] * B->size[1];
  m = (int32_T)lG;
  B->size[0] = m;
  B->size[1] = m;
  emxEnsureCapacity_real_T(sp, B, nfb, &xb_emlrtRTEI);
  b_Lmax_data = B->data;
  for (i = 0; i < loop_ub; i++) {
    b_Lmax_data[i] = 0.0;
  }
  if (m != n) {
    emlrtIntegerCheckR2012b(lG, &emlrtDCI, (emlrtConstCTX)sp);
  }
  emxInit_real_T(sp, &DA, 2, &yb_emlrtRTEI);
  nfb = DA->size[0] * DA->size[1];
  DA->size[0] = m;
  DA->size[1] = m;
  emxEnsureCapacity_real_T(sp, DA, nfb, &yb_emlrtRTEI);
  DA_data = DA->data;
  for (i = 0; i < loop_ub; i++) {
    DA_data[i] = 0.0;
  }
  /*  iterate */
  emxInit_real_T(sp, &b, 1, &gc_emlrtRTEI);
  for (b_i = 0; b_i < e_loop_ub; b_i++) {
    if (b_i + 1 > N->size[1]) {
      emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, N->size[1], &k_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    b_loop_ub = (int32_T)N_data[b_i];
    emlrtForLoopVectorCheckR2021a(1.0, 1.0, N_data[b_i], mxDOUBLE_CLASS,
                                  b_loop_ub, &g_emlrtRTEI, (emlrtConstCTX)sp);
    for (j = 0; j < b_loop_ub; j++) {
      if (b_i + 1 > v->size[1]) {
        emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, v->size[1], &p_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      v_data[b_i]++;
      if (j == 0) {
        st.site = &l_emlrtRSI;
        genmatrix(&st, &Dn, &M, &R, L, v, b_Z, (real_T)b_i + 1.0, Lmax, B, DA);
        DA_data = DA->data;
        b_Lmax_data = B->data;
        Lmax_data = Lmax->data;
      } else {
        if ((Lmax->size[0] != DA->size[0]) &&
            ((Lmax->size[0] != 1) && (DA->size[0] != 1))) {
          emlrtDimSizeImpxCheckR2021b(Lmax->size[0], DA->size[0], &d_emlrtECI,
                                      (emlrtConstCTX)sp);
        }
        if ((Lmax->size[1] != DA->size[1]) &&
            ((Lmax->size[1] != 1) && (DA->size[1] != 1))) {
          emlrtDimSizeImpxCheckR2021b(Lmax->size[1], DA->size[1], &c_emlrtECI,
                                      (emlrtConstCTX)sp);
        }
        if ((Lmax->size[0] == DA->size[0]) && (Lmax->size[1] == DA->size[1])) {
          nfb = Lmax->size[0] * Lmax->size[1];
          m = (nfb / 2) << 1;
          n = m - 2;
          for (i = 0; i <= n; i += 2) {
            r2 = _mm_loadu_pd(&Lmax_data[i]);
            r3 = _mm_loadu_pd(&DA_data[i]);
            _mm_storeu_pd(&Lmax_data[i], _mm_add_pd(r2, r3));
          }
          for (i = m; i < nfb; i++) {
            Lmax_data[i] += DA_data[i];
          }
        } else {
          st.site = &sg_emlrtRSI;
          plus(&st, Lmax, DA);
          Lmax_data = Lmax->data;
        }
      }
      st.site = &m_emlrtRSI;
      b_st.site = &fe_emlrtRSI;
      if (lh->size[0] != B->size[1]) {
        if (((B->size[0] == 1) && (B->size[1] == 1)) || (lh->size[0] == 1)) {
          emlrtErrorWithMessageIdR2018a(
              &b_st, &emlrtRTEI,
              "Coder:toolbox:mtimes_noDynamicScalarExpansion",
              "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
        } else {
          emlrtErrorWithMessageIdR2018a(&b_st, &b_emlrtRTEI, "MATLAB:innerdim",
                                        "MATLAB:innerdim", 0);
        }
      }
      b_st.site = &ee_emlrtRSI;
      m = B->size[0];
      if ((B->size[0] == 0) || (B->size[1] == 0) || (lh->size[0] == 0)) {
        nfb = b->size[0];
        b->size[0] = B->size[0];
        emxEnsureCapacity_real_T(&b_st, b, nfb, &ac_emlrtRTEI);
        b_data = b->data;
        for (i = 0; i < m; i++) {
          b_data[i] = 0.0;
        }
      } else {
        c_st.site = &ge_emlrtRSI;
        d_st.site = &he_emlrtRSI;
        TRANSB1 = 'N';
        TRANSA1 = 'N';
        lG = 1.0;
        s = 0.0;
        m_t = (ptrdiff_t)B->size[0];
        n_t = (ptrdiff_t)1;
        k_t = (ptrdiff_t)B->size[1];
        lda_t = (ptrdiff_t)B->size[0];
        ldb_t = (ptrdiff_t)lh->size[0];
        ldc_t = (ptrdiff_t)B->size[0];
        nfb = b->size[0];
        b->size[0] = B->size[0];
        emxEnsureCapacity_real_T(&d_st, b, nfb, &bc_emlrtRTEI);
        b_data = b->data;
        dgemm(&TRANSA1, &TRANSB1, &m_t, &n_t, &k_t, &lG, &b_Lmax_data[0],
              &lda_t, &lh_data[0], &ldb_t, &s, &b_data[0], &ldc_t);
      }
      if (b_i + 1 > v->size[1]) {
        emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, v->size[1], &j_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      lG = v_data[b_i];
      st.site = &m_emlrtRSI;
      s = (sum(&st, v) + M.contents) - 1.0;
      nfb = b->size[0];
      m = (b->size[0] / 2) << 1;
      n = m - 2;
      for (i = 0; i <= n; i += 2) {
        r2 = _mm_loadu_pd(&b_data[i]);
        _mm_storeu_pd(&b_data[i], _mm_div_pd(_mm_mul_pd(r2, _mm_set1_pd(lG)),
                                             _mm_set1_pd(s)));
      }
      for (i = m; i < nfb; i++) {
        b_data[i] = b_data[i] * lG / s;
      }
      st.site = &n_emlrtRSI;
      mldivide(&st, Lmax, b, lh);
      lh_data = lh->data;
      st.site = &o_emlrtRSI;
      lG = sum(&st, v);
      st.site = &p_emlrtRSI;
      loop_ub = lh->size[0];
      nfb = b->size[0];
      b->size[0] = lh->size[0];
      emxEnsureCapacity_real_T(&st, b, nfb, &cc_emlrtRTEI);
      b_data = b->data;
      for (i = 0; i < loop_ub; i++) {
        b_data[i] = lh_data[i];
      }
      b_st.site = &mf_emlrtRSI;
      sort(&b_st, b);
      st.site = &p_emlrtRSI;
      b_st.site = &ec_emlrtRSI;
      c_st.site = &fc_emlrtRSI;
      d_st.site = &gc_emlrtRSI;
      if (b->size[0] == 0) {
        s = 0.0;
      } else {
        e_st.site = &ng_emlrtRSI;
        f_st.site = &ic_emlrtRSI;
        if (b->size[0] < 4096) {
          g_st.site = &jc_emlrtRSI;
          s = sumColumnB(&g_st, b, b->size[0]);
        } else {
          nfb = (int32_T)((uint32_T)b->size[0] >> 12);
          m = nfb << 12;
          n = b->size[0] - m;
          s = sumColumnB4(b, 1);
          for (i = 2; i <= nfb; i++) {
            s += sumColumnB4(b, ((i - 1) << 12) + 1);
          }
          if (n > 0) {
            g_st.site = &kc_emlrtRSI;
            s += b_sumColumnB(&g_st, b, n, m + 1);
          }
        }
      }
      if (lG != (int32_T)muDoubleScalarFloor(lG)) {
        emlrtIntegerCheckR2012b(lG, &j_emlrtDCI, (emlrtConstCTX)sp);
      }
      if (((int32_T)lG < 1) || ((int32_T)lG > scale->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)lG, 1, scale->size[1],
                                      &r_emlrtBCI, (emlrtConstCTX)sp);
      }
      L_data[(int32_T)lG - 1] = muDoubleScalarAbs(s);
      st.site = &q_emlrtRSI;
      b_st.site = &og_emlrtRSI;
      nfb = b->size[0];
      b->size[0] = lh->size[0];
      emxEnsureCapacity_real_T(&b_st, b, nfb, &dc_emlrtRTEI);
      b_data = b->data;
      c_st.site = &pg_emlrtRSI;
      if (lh->size[0] > 2147483646) {
        d_st.site = &cb_emlrtRSI;
        check_forloop_overflow_error(&d_st);
      }
      for (i = 0; i < loop_ub; i++) {
        b_data[i] = muDoubleScalarAbs(lh_data[i]);
      }
      if (((int32_T)lG < 1) || ((int32_T)lG > scale->size[1])) {
        emlrtDynamicBoundsCheckR2012b((int32_T)lG, 1, scale->size[1],
                                      &s_emlrtBCI, (emlrtConstCTX)sp);
      }
      lG = L_data[(int32_T)lG - 1];
      nfb = lh->size[0];
      lh->size[0] = loop_ub;
      emxEnsureCapacity_real_T(sp, lh, nfb, &ec_emlrtRTEI);
      lh_data = lh->data;
      nfb = (b->size[0] / 2) << 1;
      m = nfb - 2;
      for (i = 0; i <= m; i += 2) {
        r2 = _mm_loadu_pd(&b_data[i]);
        _mm_storeu_pd(&lh_data[i], _mm_div_pd(r2, _mm_set1_pd(lG)));
      }
      for (i = nfb; i < loop_ub; i++) {
        lh_data[i] = b_data[i] / lG;
      }
      /*  rescale so that |h|=1 */
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  emxFree_real_T(sp, &v);
  emxFree_real_T(sp, &b_Z);
  emxFree_real_T(sp, &b);
  emxFree_real_T(sp, &DA);
  emxFree_real_T(sp, &B);
  emxFree_real_T(sp, &Lmax);
  emxFreeStruct_captured_var(sp, &Dn);
  /*  unscale and return the log of the normalizing constant */
  st.site = &r_emlrtRSI;
  c_log(&st, b_Lmax);
  st.site = &r_emlrtRSI;
  b_st.site = &fe_emlrtRSI;
  if (N->size[1] != b_Lmax->size[1]) {
    if ((N->size[1] == 1) || (b_Lmax->size[1] == 1)) {
      emlrtErrorWithMessageIdR2018a(
          &b_st, &emlrtRTEI, "Coder:toolbox:mtimes_noDynamicScalarExpansion",
          "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
    } else {
      emlrtErrorWithMessageIdR2018a(&b_st, &b_emlrtRTEI, "MATLAB:innerdim",
                                    "MATLAB:innerdim", 0);
    }
  }
  st.site = &r_emlrtRSI;
  b_st.site = &r_emlrtRSI;
  lG = (sum(&b_st, N) + M.contents) - 1.0;
  /*  lf=FACTLN(n) */
  /*  Compure the logarithm of n!        */
  /*  */
  /*  Copyright (c) 2012-2026, Imperial College London */
  /*  All rights reserved.   */
  b_st.site = &oc_emlrtRSI;
  c_st.site = &pc_emlrtRSI;
  s = lG + 1.0;
  d_st.site = &qc_emlrtRSI;
  scalar_gammaln(&d_st, &s);
  st.site = &r_emlrtRSI;
  /*  lf=FACTLN(n) */
  /*  Compure the logarithm of n!        */
  /*  */
  /*  Copyright (c) 2012-2026, Imperial College London */
  /*  All rights reserved.   */
  b_st.site = &oc_emlrtRSI;
  nfb = i2->size[0] * i2->size[1];
  i2->size[0] = 1;
  n = N->size[1];
  i2->size[1] = N->size[1];
  emxEnsureCapacity_real_T(&b_st, i2, nfb, &ub_emlrtRTEI);
  L_data = i2->data;
  nfb = (N->size[1] / 2) << 1;
  m = nfb - 2;
  for (i = 0; i <= m; i += 2) {
    _mm_storeu_pd(&L_data[i],
                  _mm_add_pd(_mm_loadu_pd(&N_data[i]), _mm_set1_pd(1.0)));
  }
  for (i = nfb; i < n; i++) {
    L_data[i] = N_data[i] + 1.0;
  }
  c_st.site = &pc_emlrtRSI;
  applyScalarFunctionInPlace(&c_st, i2);
  st.site = &r_emlrtRSI;
  lG = (real_T)lh->size[0] - (R.contents - 1.0);
  if (lG != (int32_T)muDoubleScalarFloor(lG)) {
    emlrtIntegerCheckR2012b(lG, &i_emlrtDCI, &st);
  }
  if (((int32_T)lG < 1) || ((int32_T)lG > lh->size[0])) {
    emlrtDynamicBoundsCheckR2012b((int32_T)lG, 1, lh->size[0], &q_emlrtBCI,
                                  &st);
  }
  lG = lh_data[(int32_T)lG - 1];
  emxFree_real_T(&st, &lh);
  if (lG < 0.0) {
    emlrtErrorWithMessageIdR2018a(
        &st, &d_emlrtRTEI, "Coder:toolbox:ElFunDomainError",
        "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
  }
  lG = muDoubleScalarLog(lG);
  st.site = &r_emlrtRSI;
  c_log(&st, scale);
  st.site = &r_emlrtRSI;
  lG = (((lG + s) - sum(&st, i2)) + mtimes(N, b_Lmax)) + sum(&st, scale);
  emxFree_real_T(sp, &i2);
  emxFree_real_T(sp, &b_Lmax);
  emxFree_real_T(sp, &scale);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return lG;
}

/* End of code generation (pfqn_comom.c) */
