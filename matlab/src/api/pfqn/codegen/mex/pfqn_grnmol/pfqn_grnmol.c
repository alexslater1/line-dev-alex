/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_grnmol.c
 *
 * Code generation for function 'pfqn_grnmol'
 *
 */

/* Include files */
#include "pfqn_grnmol.h"
#include "eml_int_forloop_overflow_check.h"
#include "multichoose.h"
#include "pfqn_grnmol_data.h"
#include "pfqn_grnmol_emxutil.h"
#include "pfqn_grnmol_types.h"
#include "power.h"
#include "prod.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "blas.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>
#include <stddef.h>

/* Variable Definitions */
static emlrtRSInfo emlrtRSI =
    {
        20,            /* lineNo */
        "pfqn_grnmol", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m" /* pathName */
};

static emlrtRSInfo b_emlrtRSI =
    {
        26,            /* lineNo */
        "pfqn_grnmol", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m" /* pathName */
};

static emlrtRSInfo c_emlrtRSI =
    {
        27,            /* lineNo */
        "pfqn_grnmol", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m" /* pathName */
};

static emlrtRSInfo d_emlrtRSI =
    {
        29,            /* lineNo */
        "pfqn_grnmol", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m" /* pathName */
};

static emlrtRSInfo e_emlrtRSI =
    {
        30,            /* lineNo */
        "pfqn_grnmol", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m" /* pathName */
};

static emlrtRSInfo f_emlrtRSI =
    {
        34,            /* lineNo */
        "pfqn_grnmol", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m" /* pathName */
};

static emlrtRSInfo t_emlrtRSI = {
    44,       /* lineNo */
    "mpower", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/matfun/mpower.m" /* pathName
                                                                       */
};

static emlrtRSInfo v_emlrtRSI = {
    10,          /* lineNo */
    "factorial", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/factorial.m" /* pathName
                                                                           */
};

static emlrtRSInfo w_emlrtRSI = {
    17,                           /* lineNo */
    "applyScalarFunctionInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunctionInPlace.m" /* pathName */
};

static emlrtRSInfo x_emlrtRSI = {
    24,                                                           /* lineNo */
    "sprod",                                                      /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/sprod.m" /* pathName */
};

static emlrtRSInfo db_emlrtRSI =
    {
        94,                  /* lineNo */
        "eml_mtimes_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pathName */
};

static emlrtRSInfo eb_emlrtRSI =
    {
        69,                  /* lineNo */
        "eml_mtimes_helper", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pathName */
};

static emlrtRSInfo fb_emlrtRSI =
    {
        142,      /* lineNo */
        "mtimes", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+blas/"
        "mtimes.m" /* pathName */
};

static emlrtRSInfo gb_emlrtRSI =
    {
        177,           /* lineNo */
        "mtimes_blas", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+blas/"
        "mtimes.m" /* pathName */
};

static emlrtRSInfo ac_emlrtRSI = {
    35,                                                           /* lineNo */
    "sprod",                                                      /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/sprod.m" /* pathName */
};

static emlrtRSInfo bc_emlrtRSI = {
    39,                                                           /* lineNo */
    "sprod",                                                      /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/sprod.m" /* pathName */
};

static emlrtRSInfo cc_emlrtRSI = {
    21,                                                           /* lineNo */
    "pprod",                                                      /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/pprod.m" /* pathName */
};

static emlrtRSInfo ec_emlrtRSI =
    {
        18,            /* lineNo */
        "ifWhileCond", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "ifWhileCond.m" /* pathName */
};

static emlrtRSInfo gc_emlrtRSI = {
    44,                           /* lineNo */
    "applyScalarFunctionInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunctionInPlace.m" /* pathName */
};

static emlrtRSInfo hc_emlrtRSI = {
    45,                           /* lineNo */
    "applyScalarFunctionInPlace", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunctionInPlace.m" /* pathName */
};

static emlrtRTEInfo emlrtRTEI = {
    21,                 /* lineNo */
    5,                  /* colNo */
    "scalar_factorial", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/factorial.m" /* pName
                                                                           */
};

static emlrtRTEInfo b_emlrtRTEI =
    {
        133,                   /* lineNo */
        23,                    /* colNo */
        "dynamic_size_checks", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

static emlrtRTEInfo c_emlrtRTEI =
    {
        138,                   /* lineNo */
        23,                    /* colNo */
        "dynamic_size_checks", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

static emlrtECInfo emlrtECI = {
    -1,                                                           /* nDims */
    48,                                                           /* lineNo */
    9,                                                            /* colNo */
    "sprod",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/sprod.m" /* pName */
};

static emlrtBCInfo emlrtBCI = {
    -1,                                                            /* iFirst */
    -1,                                                            /* iLast */
    48,                                                            /* lineNo */
    23,                                                            /* colNo */
    "D{r}",                                                        /* aName */
    "sprod",                                                       /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/sprod.m", /* pName */
    0 /* checkKind */
};

static emlrtECInfo b_emlrtECI = {
    -1,                                                           /* nDims */
    31,                                                           /* lineNo */
    9,                                                            /* colNo */
    "sprod",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/sprod.m" /* pName */
};

static emlrtBCInfo b_emlrtBCI = {
    -1,                                                            /* iFirst */
    -1,                                                            /* iLast */
    31,                                                            /* lineNo */
    23,                                                            /* colNo */
    "D{r}",                                                        /* aName */
    "sprod",                                                       /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/sprod.m", /* pName */
    0 /* checkKind */
};

static emlrtBCInfo c_emlrtBCI =
    {
        -1,            /* iFirst */
        -1,            /* iLast */
        28,            /* lineNo */
        16,            /* colNo */
        "bvec",        /* aName */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m", /* pName */
        0                /* checkKind */
};

static emlrtBCInfo d_emlrtBCI =
    {
        -1,            /* iFirst */
        -1,            /* iLast */
        26,            /* lineNo */
        30,            /* colNo */
        "c",           /* aName */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m", /* pName */
        0                /* checkKind */
};

static emlrtRTEInfo e_emlrtRTEI =
    {
        24,            /* lineNo */
        7,             /* colNo */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m" /* pName */
};

static emlrtDCInfo emlrtDCI =
    {
        22,            /* lineNo */
        11,            /* colNo */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m", /* pName */
        1                /* checkKind */
};

static emlrtDCInfo b_emlrtDCI =
    {
        21,            /* lineNo */
        1,             /* colNo */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m", /* pName */
        1                /* checkKind */
};

static emlrtDCInfo c_emlrtDCI =
    {
        21,            /* lineNo */
        1,             /* colNo */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m", /* pName */
        4                /* checkKind */
};

static emlrtBCInfo e_emlrtBCI =
    {
        -1,            /* iFirst */
        -1,            /* iLast */
        25,            /* lineNo */
        7,             /* colNo */
        "c",           /* aName */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m", /* pName */
        0                /* checkKind */
};

static emlrtBCInfo f_emlrtBCI =
    {
        -1,            /* iFirst */
        -1,            /* iLast */
        26,            /* lineNo */
        7,             /* colNo */
        "w",           /* aName */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m", /* pName */
        0                /* checkKind */
};

static emlrtBCInfo g_emlrtBCI =
    {
        -1,            /* iFirst */
        -1,            /* iLast */
        32,            /* lineNo */
        15,            /* colNo */
        "w",           /* aName */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m", /* pName */
        0                /* checkKind */
};

static emlrtBCInfo h_emlrtBCI =
    {
        -1,            /* iFirst */
        -1,            /* iLast */
        32,            /* lineNo */
        22,            /* colNo */
        "H",           /* aName */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m", /* pName */
        0                /* checkKind */
};

static emlrtBCInfo i_emlrtBCI =
    {
        -1,            /* iFirst */
        -1,            /* iLast */
        29,            /* lineNo */
        47,            /* colNo */
        "c",           /* aName */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m", /* pName */
        0                /* checkKind */
};

static emlrtBCInfo j_emlrtBCI =
    {
        -1,            /* iFirst */
        -1,            /* iLast */
        29,            /* lineNo */
        20,            /* colNo */
        "H",           /* aName */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m", /* pName */
        0                /* checkKind */
};

static emlrtRTEInfo l_emlrtRTEI =
    {
        21,            /* lineNo */
        1,             /* colNo */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m" /* pName */
};

static emlrtRTEInfo m_emlrtRTEI =
    {
        22,            /* lineNo */
        1,             /* colNo */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m" /* pName */
};

static emlrtRTEInfo n_emlrtRTEI =
    {
        23,            /* lineNo */
        1,             /* colNo */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m" /* pName */
};

static emlrtRTEInfo o_emlrtRTEI = {
    1,           /* lineNo */
    24,          /* colNo */
    "factorial", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/factorial.m" /* pName
                                                                           */
};

static emlrtRTEInfo p_emlrtRTEI = {
    29,                                                           /* lineNo */
    5,                                                            /* colNo */
    "sprod",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/sprod.m" /* pName */
};

static emlrtRTEInfo q_emlrtRTEI = {
    31,                                                           /* lineNo */
    18,                                                           /* colNo */
    "sprod",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/sprod.m" /* pName */
};

static emlrtRTEInfo r_emlrtRTEI =
    {
        27,            /* lineNo */
        33,            /* colNo */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m" /* pName */
};

static emlrtRTEInfo s_emlrtRTEI =
    {
        29,            /* lineNo */
        34,            /* colNo */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m" /* pName */
};

static emlrtRTEInfo t_emlrtRTEI =
    {
        94,                  /* lineNo */
        5,                   /* colNo */
        "eml_mtimes_helper", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/"
        "eml_mtimes_helper.m" /* pName */
};

static emlrtRTEInfo u_emlrtRTEI =
    {
        218,      /* lineNo */
        20,       /* colNo */
        "mtimes", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/+blas/"
        "mtimes.m" /* pName */
};

static emlrtRTEInfo v_emlrtRTEI = {
    38,                                                           /* lineNo */
    5,                                                            /* colNo */
    "sprod",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/sprod.m" /* pName */
};

static emlrtRTEInfo w_emlrtRTEI = {
    40,                                                           /* lineNo */
    9,                                                            /* colNo */
    "sprod",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/sprod.m" /* pName */
};

static emlrtRTEInfo x_emlrtRTEI = {
    48,                                                           /* lineNo */
    18,                                                           /* colNo */
    "sprod",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/sprod.m" /* pName */
};

static emlrtRTEInfo y_emlrtRTEI =
    {
        30,            /* lineNo */
        33,            /* colNo */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m" /* pName */
};

static emlrtRTEInfo ab_emlrtRTEI =
    {
        27,            /* lineNo */
        8,             /* colNo */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m" /* pName */
};

static emlrtRTEInfo bb_emlrtRTEI =
    {
        29,            /* lineNo */
        33,            /* colNo */
        "pfqn_grnmol", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_grnmol.m" /* pName */
};

static emlrtRTEInfo cb_emlrtRTEI = {
    17,                                                           /* lineNo */
    1,                                                            /* colNo */
    "sprod",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/sprod.m" /* pName */
};

static emlrtRTEInfo db_emlrtRTEI = {
    31,                                                           /* lineNo */
    9,                                                            /* colNo */
    "sprod",                                                      /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/util/sprod.m" /* pName */
};

/* Function Definitions */
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

real_T pfqn_grnmol(const emlrtStack *sp, const emxArray_real_T *L,
                   const emxArray_real_T *N)
{
  static const real_T dv[170] = {1.0,
                                 2.0,
                                 6.0,
                                 24.0,
                                 120.0,
                                 720.0,
                                 5040.0,
                                 40320.0,
                                 362880.0,
                                 3.6288E+6,
                                 3.99168E+7,
                                 4.790016E+8,
                                 6.2270208E+9,
                                 8.71782912E+10,
                                 1.307674368E+12,
                                 2.0922789888E+13,
                                 3.55687428096E+14,
                                 6.402373705728E+15,
                                 1.21645100408832E+17,
                                 2.43290200817664E+18,
                                 5.109094217170944E+19,
                                 1.1240007277776077E+21,
                                 2.5852016738884978E+22,
                                 6.2044840173323941E+23,
                                 1.5511210043330986E+25,
                                 4.0329146112660565E+26,
                                 1.0888869450418352E+28,
                                 3.0488834461171384E+29,
                                 8.8417619937397008E+30,
                                 2.6525285981219103E+32,
                                 8.2228386541779224E+33,
                                 2.6313083693369352E+35,
                                 8.6833176188118859E+36,
                                 2.9523279903960412E+38,
                                 1.0333147966386144E+40,
                                 3.7199332678990118E+41,
                                 1.3763753091226343E+43,
                                 5.23022617466601E+44,
                                 2.0397882081197442E+46,
                                 8.1591528324789768E+47,
                                 3.3452526613163803E+49,
                                 1.4050061177528798E+51,
                                 6.0415263063373834E+52,
                                 2.6582715747884485E+54,
                                 1.1962222086548019E+56,
                                 5.5026221598120885E+57,
                                 2.5862324151116818E+59,
                                 1.2413915592536073E+61,
                                 6.0828186403426752E+62,
                                 3.0414093201713376E+64,
                                 1.5511187532873822E+66,
                                 8.0658175170943877E+67,
                                 4.2748832840600255E+69,
                                 2.3084369733924138E+71,
                                 1.2696403353658276E+73,
                                 7.1099858780486348E+74,
                                 4.0526919504877221E+76,
                                 2.3505613312828789E+78,
                                 1.3868311854568986E+80,
                                 8.3209871127413916E+81,
                                 5.0758021387722484E+83,
                                 3.1469973260387939E+85,
                                 1.98260831540444E+87,
                                 1.2688693218588417E+89,
                                 8.2476505920824715E+90,
                                 5.4434493907744307E+92,
                                 3.6471110918188683E+94,
                                 2.4800355424368305E+96,
                                 1.711224524281413E+98,
                                 1.197857166996989E+100,
                                 8.5047858856786218E+101,
                                 6.1234458376886077E+103,
                                 4.4701154615126834E+105,
                                 3.3078854415193856E+107,
                                 2.4809140811395391E+109,
                                 1.8854947016660498E+111,
                                 1.4518309202828584E+113,
                                 1.1324281178206295E+115,
                                 8.9461821307829729E+116,
                                 7.1569457046263779E+118,
                                 5.7971260207473655E+120,
                                 4.75364333701284E+122,
                                 3.9455239697206569E+124,
                                 3.314240134565352E+126,
                                 2.8171041143805494E+128,
                                 2.4227095383672724E+130,
                                 2.1077572983795269E+132,
                                 1.8548264225739836E+134,
                                 1.6507955160908452E+136,
                                 1.4857159644817607E+138,
                                 1.3520015276784023E+140,
                                 1.24384140546413E+142,
                                 1.1567725070816409E+144,
                                 1.0873661566567424E+146,
                                 1.0329978488239052E+148,
                                 9.916779348709491E+149,
                                 9.6192759682482062E+151,
                                 9.426890448883242E+153,
                                 9.33262154439441E+155,
                                 9.33262154439441E+157,
                                 9.4259477598383536E+159,
                                 9.6144667150351211E+161,
                                 9.9029007164861754E+163,
                                 1.0299016745145622E+166,
                                 1.0813967582402903E+168,
                                 1.1462805637347078E+170,
                                 1.2265202031961373E+172,
                                 1.3246418194518284E+174,
                                 1.4438595832024928E+176,
                                 1.5882455415227421E+178,
                                 1.7629525510902437E+180,
                                 1.9745068572210728E+182,
                                 2.2311927486598123E+184,
                                 2.5435597334721862E+186,
                                 2.9250936934930141E+188,
                                 3.3931086844518965E+190,
                                 3.969937160808719E+192,
                                 4.6845258497542883E+194,
                                 5.5745857612076033E+196,
                                 6.6895029134491239E+198,
                                 8.09429852527344E+200,
                                 9.8750442008335976E+202,
                                 1.2146304367025325E+205,
                                 1.5061417415111404E+207,
                                 1.8826771768889254E+209,
                                 2.3721732428800459E+211,
                                 3.0126600184576582E+213,
                                 3.8562048236258025E+215,
                                 4.9745042224772855E+217,
                                 6.4668554892204716E+219,
                                 8.4715806908788174E+221,
                                 1.1182486511960039E+224,
                                 1.4872707060906852E+226,
                                 1.9929427461615181E+228,
                                 2.6904727073180495E+230,
                                 3.6590428819525472E+232,
                                 5.01288874827499E+234,
                                 6.9177864726194859E+236,
                                 9.6157231969410859E+238,
                                 1.346201247571752E+241,
                                 1.89814375907617E+243,
                                 2.6953641378881614E+245,
                                 3.8543707171800706E+247,
                                 5.5502938327393013E+249,
                                 8.0479260574719866E+251,
                                 1.17499720439091E+254,
                                 1.7272458904546376E+256,
                                 2.5563239178728637E+258,
                                 3.8089226376305671E+260,
                                 5.7133839564458505E+262,
                                 8.6272097742332346E+264,
                                 1.3113358856834518E+267,
                                 2.0063439050956811E+269,
                                 3.0897696138473489E+271,
                                 4.7891429014633912E+273,
                                 7.47106292628289E+275,
                                 1.1729568794264138E+278,
                                 1.8532718694937338E+280,
                                 2.9467022724950369E+282,
                                 4.714723635992059E+284,
                                 7.5907050539472148E+286,
                                 1.2296942187394488E+289,
                                 2.0044015765453015E+291,
                                 3.2872185855342945E+293,
                                 5.423910666131586E+295,
                                 9.0036917057784329E+297,
                                 1.5036165148649983E+300,
                                 2.5260757449731969E+302,
                                 4.2690680090047027E+304,
                                 7.257415615307994E+306};
  __m128d r1;
  jmp_buf emlrtJBEnviron;
  ptrdiff_t k_t;
  ptrdiff_t lda_t;
  ptrdiff_t ldb_t;
  ptrdiff_t ldc_t;
  ptrdiff_t m_t;
  ptrdiff_t n_t;
  jmp_buf *volatile emlrtJBStack;
  cell_wrap_0 D;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack st;
  emlrtStack *b_sp;
  emxArray_real_T *H;
  emxArray_real_T *bvec;
  emxArray_real_T *c;
  emxArray_real_T *n;
  emxArray_real_T *r;
  emxArray_real_T *w;
  emxArray_real_T *y;
  const real_T *L_data;
  const real_T *N_data;
  real_T G;
  real_T S;
  real_T alpha1;
  real_T b_n;
  real_T beta1;
  real_T c_n;
  real_T d;
  real_T d_n;
  real_T s_data;
  real_T *H_data;
  real_T *bvec_data;
  real_T *c_data;
  real_T *n_data;
  real_T *w_data;
  real_T *y_data;
  int32_T M;
  int32_T S_data;
  int32_T S_tmp;
  int32_T b_i;
  int32_T exitg1;
  int32_T i;
  int32_T i1;
  int32_T k;
  int32_T loop_ub;
  int32_T nx;
  int32_T pfqn_grnmol_numThreads;
  int32_T scalarLB;
  int32_T size_tmp_idx_1;
  int32_T vectorUB;
  char_T TRANSA1;
  char_T TRANSB1;
  boolean_T b_y;
  boolean_T emlrtHadParallelError = false;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  N_data = N->data;
  L_data = L->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /* { */
  /*  % @file pfqn_grnmol.m */
  /*  % @brief Normalizing constant using Grundmann-Moeller quadrature. */
  /* } */
  /* { */
  /*  % @brief Normalizing constant using Grundmann-Moeller quadrature. */
  /*  % @fn pfqn_grnmol(L, N) */
  /*  % @param L Service demand matrix. */
  /*  % @param N Population vector. */
  /*  % @return G Normalizing constant. */
  /* } */
  M = L->size[0];
  G = 0.0;
  st.site = &emlrtRSI;
  alpha1 = sum(&st, N) - 1.0;
  alpha1 = muDoubleScalarCeil(alpha1);
  S = alpha1 / 2.0;
  emxInit_real_T(sp, &H, 1, &l_emlrtRTEI);
  if (!(S + 1.0 >= 0.0)) {
    emlrtNonNegativeCheckR2012b(S + 1.0, &c_emlrtDCI, (emlrtConstCTX)sp);
  }
  alpha1 = (int32_T)muDoubleScalarFloor(S + 1.0);
  if (S + 1.0 != alpha1) {
    emlrtIntegerCheckR2012b(S + 1.0, &b_emlrtDCI, (emlrtConstCTX)sp);
  }
  nx = H->size[0];
  loop_ub = (int32_T)(S + 1.0);
  H->size[0] = (int32_T)(S + 1.0);
  emxEnsureCapacity_real_T(sp, H, nx, &l_emlrtRTEI);
  H_data = H->data;
  if (S + 1.0 != alpha1) {
    emlrtIntegerCheckR2012b(S + 1.0, &b_emlrtDCI, (emlrtConstCTX)sp);
  }
  for (i = 0; i < loop_ub; i++) {
    H_data[i] = 0.0;
  }
  if (S + 1.0 != alpha1) {
    emlrtIntegerCheckR2012b(S + 1.0, &emlrtDCI, (emlrtConstCTX)sp);
  }
  emxInit_real_T(sp, &c, 2, &m_emlrtRTEI);
  nx = c->size[0] * c->size[1];
  c->size[0] = 1;
  c->size[1] = (int32_T)(S + 1.0);
  emxEnsureCapacity_real_T(sp, c, nx, &m_emlrtRTEI);
  c_data = c->data;
  for (i = 0; i < loop_ub; i++) {
    c_data[i] = 0.0;
  }
  emxInit_real_T(sp, &w, 2, &n_emlrtRTEI);
  nx = w->size[0] * w->size[1];
  w->size[0] = 1;
  w->size[1] = (int32_T)(S + 1.0);
  emxEnsureCapacity_real_T(sp, w, nx, &n_emlrtRTEI);
  w_data = w->data;
  for (i = 0; i < loop_ub; i++) {
    w_data[i] = 0.0;
  }
  emlrtForLoopVectorCheckR2021a(0.0, 1.0, S, mxDOUBLE_CLASS, (int32_T)(S + 1.0),
                                &e_emlrtRTEI, (emlrtConstCTX)sp);
  emxInit_real_T(sp, &bvec, 2, &ab_emlrtRTEI);
  emxInit_real_T(sp, &y, 2, &bb_emlrtRTEI);
  emxInit_real_T(sp, &n, 1, &p_emlrtRTEI);
  emxInitMatrix_cell_wrap_0(sp, &D, &cb_emlrtRTEI);
  emxInit_real_T(sp, &r, 1, &db_emlrtRTEI);
  for (b_i = 0; b_i < loop_ub; b_i++) {
    if (((int32_T)((uint32_T)b_i + 1U) < 1) ||
        ((int32_T)((uint32_T)b_i + 1U) > c->size[1])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_i + 1U), 1,
                                    c->size[1], &e_emlrtBCI, (emlrtConstCTX)sp);
    }
    alpha1 = S - (real_T)b_i;
    c_data[b_i] = 2.0 * alpha1 + (real_T)M;
    st.site = &b_emlrtRSI;
    b_st.site = &t_emlrtRSI;
    c_st.site = &u_emlrtRSI;
    st.site = &b_emlrtRSI;
    b_st.site = &t_emlrtRSI;
    c_st.site = &u_emlrtRSI;
    st.site = &b_emlrtRSI;
    i1 = c->size[1];
    b_y = ((b_i + 1 < 1) || (b_i + 1 > c->size[1]));
    if (b_y) {
      emlrtDynamicBoundsCheckR2012b(b_i + 1, 1, c->size[1], &d_emlrtBCI, &st);
    }
    beta1 = 2.0 * S;
    b_st.site = &t_emlrtRSI;
    c_st.site = &u_emlrtRSI;
    if ((c_data[b_i] < 0.0) && (!muDoubleScalarIsNaN(beta1 + 1.0)) &&
        (muDoubleScalarFloor(beta1 + 1.0) != beta1 + 1.0)) {
      emlrtErrorWithMessageIdR2018a(&c_st, &d_emlrtRTEI,
                                    "Coder:toolbox:power_domainError",
                                    "Coder:toolbox:power_domainError", 0);
    }
    st.site = &b_emlrtRSI;
    b_st.site = &v_emlrtRSI;
    c_st.site = &w_emlrtRSI;
    if (b_i > 170) {
      s_data = rtInf;
    } else if (b_i < 1) {
      s_data = 1.0;
    } else {
      s_data = dv[b_i - 1];
    }
    st.site = &b_emlrtRSI;
    b_st.site = &v_emlrtRSI;
    c_st.site = &w_emlrtRSI;
    c_n = (real_T)b_i + c_data[b_i];
    if ((c_n < 0.0) || (muDoubleScalarFloor(c_n) != c_n) ||
        muDoubleScalarIsInf(c_n)) {
      emlrtErrorWithMessageIdR2018a(&c_st, &emlrtRTEI,
                                    "MATLAB:factorial:NNegativeInt",
                                    "MATLAB:factorial:NNegativeInt", 0);
    } else if (c_n > 170.0) {
      d_n = rtInf;
    } else if (c_n < 1.0) {
      d_n = 1.0;
    } else {
      d_n = dv[(int32_T)c_n - 1];
    }
    if (((int32_T)((uint32_T)b_i + 1U) < 1) ||
        ((int32_T)((uint32_T)b_i + 1U) > w->size[1])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_i + 1U), 1,
                                    w->size[1], &f_emlrtBCI, (emlrtConstCTX)sp);
    }
    w_data[b_i] = muDoubleScalarPower(2.0, -beta1) *
                  muDoubleScalarPower(-1.0, b_i) *
                  muDoubleScalarPower(c_data[b_i], beta1 + 1.0) / s_data / d_n;
    st.site = &c_emlrtRSI;
    /*  [s,n,S,D]=SPROD(M,N) % init */
    /*  [s,n]=SPROD(s,S,D) % next state */
    /*  Sequence of non-negative matrices with constant row sums */
    /*   */
    /*  Example:  */
    /*  [s,n,S,D]=sprod(2,5); % m=2 elements summing to N=5 */
    /*  */
    /*  while s>=0 */
    /*  ...  */
    /*  [s,n]=sprod(s,S,D) % generate next state */
    /*  end */
    /*  */
    /*  Copyright (c) 2012-2026, Imperial College London */
    /*  All rights reserved. */
    /*  this is init */
    b_st.site = &x_emlrtRSI;
    multichoose(&b_st, M, alpha1, D.f1);
    size_tmp_idx_1 = D.f1->size[1];
    S_tmp = D.f1->size[0];
    S_data = D.f1->size[0];
    S_data--;
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
    s_data = 0.0;
    nx = n->size[0];
    n->size[0] = M;
    emxEnsureCapacity_real_T(&st, n, nx, &p_emlrtRTEI);
    n_data = n->data;
    for (i = 0; i < M; i++) {
      n_data[i] = 0.0;
    }
    if (D.f1->size[0] < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, D.f1->size[0], &b_emlrtBCI, &st);
    }
    nx = r->size[0];
    r->size[0] = D.f1->size[1];
    emxEnsureCapacity_real_T(&st, r, nx, &q_emlrtRTEI);
    y_data = r->data;
    for (i = 0; i < size_tmp_idx_1; i++) {
      y_data[i] = D.f1->data[D.f1->size[0] * i];
    }
    emlrtSubAssignSizeCheckR2012b(&M, 1, &r->size[0], 1, &b_emlrtECI, &st);
    for (i = 0; i < M; i++) {
      n_data[i] = y_data[i];
    }
    nx = bvec->size[0] * bvec->size[1];
    bvec->size[0] = 1;
    bvec->size[1] = M;
    emxEnsureCapacity_real_T(sp, bvec, nx, &r_emlrtRTEI);
    bvec_data = bvec->data;
    for (i = 0; i < M; i++) {
      bvec_data[i] = n_data[i];
    }
    do {
      exitg1 = 0;
      if (bvec->size[1] < 1) {
        emlrtDynamicBoundsCheckR2012b(1, 1, bvec->size[1], &c_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (bvec_data[0] >= 0.0) {
        st.site = &d_emlrtRSI;
        nx = bvec->size[0] * bvec->size[1];
        bvec->size[0] = 1;
        emxEnsureCapacity_real_T(&st, bvec, nx, &s_emlrtRTEI);
        bvec_data = bvec->data;
        if (((int32_T)((uint32_T)b_i + 1U) < 1) ||
            ((int32_T)((uint32_T)b_i + 1U) > i1)) {
          emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_i + 1U), 1, i1,
                                        &i_emlrtBCI, &st);
        }
        alpha1 = c_data[b_i];
        nx = bvec->size[1] - 1;
        scalarLB = (bvec->size[1] / 2) << 1;
        vectorUB = scalarLB - 2;
        for (i = 0; i <= vectorUB; i += 2) {
          r1 = _mm_loadu_pd(&bvec_data[i]);
          _mm_storeu_pd(&bvec_data[i],
                        _mm_div_pd(_mm_add_pd(_mm_mul_pd(_mm_set1_pd(2.0), r1),
                                              _mm_set1_pd(1.0)),
                                   _mm_set1_pd(alpha1)));
        }
        for (i = scalarLB; i <= nx; i++) {
          bvec_data[i] = (2.0 * bvec_data[i] + 1.0) / alpha1;
        }
        b_st.site = &eb_emlrtRSI;
        if (L->size[0] != bvec->size[1]) {
          if ((bvec->size[1] == 1) ||
              ((L->size[0] == 1) && (L->size[1] == 1))) {
            emlrtErrorWithMessageIdR2018a(
                &b_st, &b_emlrtRTEI,
                "Coder:toolbox:mtimes_noDynamicScalarExpansion",
                "Coder:toolbox:mtimes_noDynamicScalarExpansion", 0);
          } else {
            emlrtErrorWithMessageIdR2018a(
                &b_st, &c_emlrtRTEI, "MATLAB:innerdim", "MATLAB:innerdim", 0);
          }
        }
        b_st.site = &db_emlrtRSI;
        if ((bvec->size[1] == 0) || (L->size[0] == 0) || (L->size[1] == 0)) {
          nx = y->size[0] * y->size[1];
          y->size[0] = 1;
          scalarLB = L->size[1];
          y->size[1] = L->size[1];
          emxEnsureCapacity_real_T(&b_st, y, nx, &t_emlrtRTEI);
          y_data = y->data;
          for (i = 0; i < scalarLB; i++) {
            y_data[i] = 0.0;
          }
        } else {
          c_st.site = &fb_emlrtRSI;
          d_st.site = &gb_emlrtRSI;
          TRANSB1 = 'N';
          TRANSA1 = 'N';
          alpha1 = 1.0;
          beta1 = 0.0;
          m_t = (ptrdiff_t)1;
          n_t = (ptrdiff_t)L->size[1];
          k_t = (ptrdiff_t)bvec->size[1];
          lda_t = (ptrdiff_t)1;
          ldb_t = (ptrdiff_t)L->size[0];
          ldc_t = (ptrdiff_t)1;
          nx = y->size[0] * y->size[1];
          y->size[0] = 1;
          y->size[1] = L->size[1];
          emxEnsureCapacity_real_T(&d_st, y, nx, &u_emlrtRTEI);
          y_data = y->data;
          dgemm(&TRANSA1, &TRANSB1, &m_t, &n_t, &k_t, &alpha1, &bvec_data[0],
                &lda_t, (real_T *)&L_data[0], &ldb_t, &beta1, &y_data[0],
                &ldc_t);
        }
        st.site = &d_emlrtRSI;
        power(&st, y, N, bvec);
        if (((int32_T)((uint32_T)b_i + 1U) < 1) ||
            ((int32_T)((uint32_T)b_i + 1U) > H->size[0])) {
          emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_i + 1U), 1,
                                        H->size[0], &j_emlrtBCI,
                                        (emlrtConstCTX)sp);
        }
        st.site = &d_emlrtRSI;
        H_data[b_i] += prod(&st, bvec);
        st.site = &e_emlrtRSI;
        /*  [s,n,S,D]=SPROD(M,N) % init */
        /*  [s,n]=SPROD(s,S,D) % next state */
        /*  Sequence of non-negative matrices with constant row sums */
        /*   */
        /*  Example:  */
        /*  [s,n,S,D]=sprod(2,5); % m=2 elements summing to N=5 */
        /*  */
        /*  while s>=0 */
        /*  ...  */
        /*  [s,n]=sprod(s,S,D) % generate next state */
        /*  end */
        /*  */
        /*  Copyright (c) 2012-2026, Imperial College London */
        /*  All rights reserved. */
        b_st.site = &ac_emlrtRSI;
        /*  n=PPROD(N) % init */
        /*  n=PPROD(n,N) % next state */
        /*  Return a sequence of non-negative vectors less than a given vector
         */
        /*  */
        /*  n=pprod(N); */
        /*  while n>=0 */
        /*    ... */
        /*    n=pprod(n,N); */
        /*  end */
        /*  */
        /*  Copyright (c) 2012-2026, Imperial College London */
        /*  All rights reserved. */
        c_st.site = &cc_emlrtRSI;
        if (s_data == S_data) {
          s_data = -1.0;
        } else if (s_data == S_data) {
          s_data = 0.0;
          /* n=-1*ones(1,R);         */
        } else {
          s_data++;
        }
        nx = n->size[0];
        n->size[0] = size_tmp_idx_1;
        emxEnsureCapacity_real_T(&st, n, nx, &v_emlrtRTEI);
        n_data = n->data;
        for (i = 0; i < size_tmp_idx_1; i++) {
          n_data[i] = 0.0;
        }
        b_st.site = &bc_emlrtRSI;
        c_st.site = &ec_emlrtRSI;
        b_y = (s_data == -1.0);
        if (b_y) {
          nx = n->size[0];
          n->size[0] = size_tmp_idx_1;
          emxEnsureCapacity_real_T(&st, n, nx, &w_emlrtRTEI);
          n_data = n->data;
          for (i = 0; i < size_tmp_idx_1; i++) {
            n_data[i] = -1.0;
          }
        } else {
          if (((int32_T)(s_data + 1.0) < 1) ||
              ((int32_T)(s_data + 1.0) > S_tmp)) {
            emlrtDynamicBoundsCheckR2012b((int32_T)(s_data + 1.0), 1, S_tmp,
                                          &emlrtBCI, &st);
          }
          nx = r->size[0];
          r->size[0] = size_tmp_idx_1;
          emxEnsureCapacity_real_T(&st, r, nx, &x_emlrtRTEI);
          y_data = r->data;
          for (i = 0; i < size_tmp_idx_1; i++) {
            y_data[i] =
                D.f1->data[((int32_T)(s_data + 1.0) + D.f1->size[0] * i) - 1];
          }
          emlrtSubAssignSizeCheckR2012b(&size_tmp_idx_1, 1, &r->size[0], 1,
                                        &emlrtECI, &st);
          for (i = 0; i < size_tmp_idx_1; i++) {
            n_data[i] = y_data[i];
          }
        }
        nx = bvec->size[0] * bvec->size[1];
        bvec->size[0] = 1;
        bvec->size[1] = size_tmp_idx_1;
        emxEnsureCapacity_real_T(sp, bvec, nx, &y_emlrtRTEI);
        bvec_data = bvec->data;
        for (i = 0; i < size_tmp_idx_1; i++) {
          bvec_data[i] = n_data[i];
        }
      } else {
        exitg1 = 1;
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    } while (exitg1 == 0);
    if (((int32_T)((uint32_T)b_i + 1U) < 1) ||
        ((int32_T)((uint32_T)b_i + 1U) > w->size[1])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_i + 1U), 1,
                                    w->size[1], &g_emlrtBCI, (emlrtConstCTX)sp);
    }
    if (((int32_T)((uint32_T)b_i + 1U) < 1) ||
        ((int32_T)((uint32_T)b_i + 1U) > H->size[0])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)b_i + 1U), 1,
                                    H->size[0], &h_emlrtBCI, (emlrtConstCTX)sp);
    }
    G += w_data[b_i] * H_data[b_i];
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  emxFree_real_T(sp, &r);
  emxFreeMatrix_cell_wrap_0(sp, &D);
  emxFree_real_T(sp, &n);
  emxFree_real_T(sp, &y);
  emxFree_real_T(sp, &w);
  emxFree_real_T(sp, &c);
  emxFree_real_T(sp, &H);
  st.site = &f_emlrtRSI;
  b_st.site = &f_emlrtRSI;
  alpha1 = (sum(&b_st, N) + (real_T)L->size[0]) - 1.0;
  b_st.site = &v_emlrtRSI;
  c_st.site = &w_emlrtRSI;
  if ((alpha1 < 0.0) || (muDoubleScalarFloor(alpha1) != alpha1) ||
      muDoubleScalarIsInf(alpha1)) {
    emlrtErrorWithMessageIdR2018a(&c_st, &emlrtRTEI,
                                  "MATLAB:factorial:NNegativeInt",
                                  "MATLAB:factorial:NNegativeInt", 0);
  } else if (alpha1 > 170.0) {
    alpha1 = rtInf;
  } else if (alpha1 < 1.0) {
    alpha1 = 1.0;
  } else {
    alpha1 = dv[(int32_T)alpha1 - 1];
  }
  st.site = &f_emlrtRSI;
  nx = bvec->size[0] * bvec->size[1];
  bvec->size[0] = 1;
  scalarLB = N->size[1];
  bvec->size[1] = N->size[1];
  emxEnsureCapacity_real_T(&st, bvec, nx, &o_emlrtRTEI);
  bvec_data = bvec->data;
  for (i = 0; i < scalarLB; i++) {
    bvec_data[i] = N_data[i];
  }
  b_st.site = &v_emlrtRSI;
  nx = N->size[1];
  c_st.site = &gc_emlrtRSI;
  if (N->size[1] > 2147483646) {
    d_st.site = &s_emlrtRSI;
    check_forloop_overflow_error(&d_st);
  }
  if (N->size[1] < 1600) {
    for (k = 0; k < scalarLB; k++) {
      c_st.site = &hc_emlrtRSI;
      d = bvec_data[k];
      if ((d < 0.0) || (muDoubleScalarFloor(d) != d) ||
          muDoubleScalarIsInf(d)) {
        emlrtErrorWithMessageIdR2018a(&c_st, &emlrtRTEI,
                                      "MATLAB:factorial:NNegativeInt",
                                      "MATLAB:factorial:NNegativeInt", 0);
      } else if (d > 170.0) {
        b_n = rtInf;
      } else if (d < 1.0) {
        b_n = 1.0;
      } else {
        b_n = dv[(int32_T)d - 1];
      }
      bvec_data[k] = b_n;
    }
  } else {
    emlrtEnterParallelRegion(&b_st, omp_in_parallel());
    emlrtPushJmpBuf(&b_st, &emlrtJBStack);
    pfqn_grnmol_numThreads =
        emlrtAllocRegionTLSs(b_st.tls, omp_in_parallel(), omp_get_max_threads(),
                             omp_get_num_procs());
#pragma omp parallel num_threads(pfqn_grnmol_numThreads) private(              \
        b_n, d, emlrtJBEnviron, f_st)                                          \
    firstprivate(e_st, b_st, emlrtHadParallelError)
    {
      if (setjmp(emlrtJBEnviron) == 0) {
        e_st.prev = &b_st;
        e_st.tls = emlrtAllocTLS(&b_st, omp_get_thread_num());
        e_st.site = NULL;
        emlrtSetJmpBuf(&e_st, &emlrtJBEnviron);
        f_st.prev = &e_st;
        f_st.tls = e_st.tls;
      } else {
        emlrtHadParallelError = true;
      }
#pragma omp for nowait
      for (k = 0; k < nx; k++) {
        if (emlrtHadParallelError) {
          continue;
        }
        if (setjmp(emlrtJBEnviron) == 0) {
          f_st.site = &hc_emlrtRSI;
          d = bvec_data[k];
          if ((d < 0.0) || (muDoubleScalarFloor(d) != d) ||
              muDoubleScalarIsInf(d)) {
            emlrtErrorWithMessageIdR2018a(&f_st, &emlrtRTEI,
                                          "MATLAB:factorial:NNegativeInt",
                                          "MATLAB:factorial:NNegativeInt", 0);
          } else if (d > 170.0) {
            b_n = rtInf;
          } else if (d < 1.0) {
            b_n = 1.0;
          } else {
            b_n = dv[(int32_T)d - 1];
          }
          bvec_data[k] = b_n;
        } else {
          emlrtHadParallelError = true;
        }
      }
    }
    emlrtPopJmpBuf(&b_st, &emlrtJBStack);
    emlrtExitParallelRegion(&b_st, omp_in_parallel());
  }
  st.site = &f_emlrtRSI;
  G = G * alpha1 / prod(&st, bvec);
  emxFree_real_T(sp, &bvec);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return G;
}

/* End of code generation (pfqn_grnmol.c) */
