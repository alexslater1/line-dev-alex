/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mvald.c
 *
 * Code generation for function 'pfqn_mvald'
 *
 */

/* Include files */
#include "pfqn_mvald.h"
#include "any.h"
#include "div.h"
#include "eml_int_forloop_overflow_check.h"
#include "ifWhileCond.h"
#include "pfqn_mvams_data.h"
#include "pfqn_mvams_emxutil.h"
#include "pfqn_mvams_types.h"
#include "pprod.h"
#include "prod.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "mwmathutil.h"
#include "omp.h"
#include <emmintrin.h>

/* Variable Definitions */
static emlrtRSInfo pf_emlrtRSI =
    {
        120,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo qf_emlrtRSI =
    {
        117,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo rf_emlrtRSI =
    {
        116,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo sf_emlrtRSI =
    {
        114,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo tf_emlrtRSI =
    {
        111,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo uf_emlrtRSI =
    {
        106,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo vf_emlrtRSI =
    {
        105,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo wf_emlrtRSI =
    {
        104,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo xf_emlrtRSI =
    {
        100,          /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo yf_emlrtRSI =
    {
        95,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo ag_emlrtRSI =
    {
        86,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo bg_emlrtRSI =
    {
        78,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo cg_emlrtRSI =
    {
        66,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo dg_emlrtRSI =
    {
        73,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo eg_emlrtRSI =
    {
        71,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo fg_emlrtRSI =
    {
        63,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo gg_emlrtRSI =
    {
        62,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo hg_emlrtRSI =
    {
        56,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo ig_emlrtRSI =
    {
        53,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo jg_emlrtRSI =
    {
        51,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo kg_emlrtRSI =
    {
        50,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo lg_emlrtRSI =
    {
        39,           /* lineNo */
        "pfqn_mvald", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtRSInfo mg_emlrtRSI =
    {
        128,       /* lineNo */
        "hashpop", /* fcnName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pathName */
};

static emlrtDCInfo gb_emlrtDCI =
    {
        50,           /* lineNo */
        12,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        4               /* checkKind */
};

static emlrtDCInfo hb_emlrtDCI =
    {
        50,           /* lineNo */
        12,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtDCInfo ib_emlrtDCI =
    {
        51,           /* lineNo */
        11,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        4               /* checkKind */
};

static emlrtDCInfo jb_emlrtDCI =
    {
        51,           /* lineNo */
        11,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtDCInfo kb_emlrtDCI =
    {
        51,           /* lineNo */
        20,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        4               /* checkKind */
};

static emlrtDCInfo lb_emlrtDCI =
    {
        51,           /* lineNo */
        20,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtDCInfo mb_emlrtDCI =
    {
        54,           /* lineNo */
        16,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        4               /* checkKind */
};

static emlrtDCInfo nb_emlrtDCI =
    {
        54,           /* lineNo */
        16,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtRTEInfo bb_emlrtRTEI =
    {
        62,           /* lineNo */
        23,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtBCInfo jq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        66,           /* lineNo */
        52,           /* colNo */
        "WN",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtRTEInfo cb_emlrtRTEI =
    {
        71,           /* lineNo */
        11,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtBCInfo kq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        86,           /* lineNo */
        23,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo ob_emlrtDCI =
    {
        86,           /* lineNo */
        40,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo lq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        86,           /* lineNo */
        40,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo mq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        95,           /* lineNo */
        24,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo nq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        100,          /* lineNo */
        20,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo oq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        105,          /* lineNo */
        30,           /* colNo */
        "n",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo pq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        105,          /* lineNo */
        32,           /* colNo */
        "n",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo qq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        105,          /* lineNo */
        54,           /* colNo */
        "N",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo rq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        105,          /* lineNo */
        56,           /* colNo */
        "N",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo sq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        105,          /* lineNo */
        79,           /* colNo */
        "n",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo tq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        105,          /* lineNo */
        91,           /* colNo */
        "n",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo uq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        113,          /* lineNo */
        11,           /* colNo */
        "lGN",        /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo vq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        113,          /* lineNo */
        13,           /* colNo */
        "lGN",        /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo pb_emlrtDCI =
    {
        114,          /* lineNo */
        8,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo wq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        114,          /* lineNo */
        8,            /* colNo */
        "Xs",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo qb_emlrtDCI =
    {
        116,          /* lineNo */
        11,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo xq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        116,          /* lineNo */
        11,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo p_emlrtECI =
    {
        1,            /* nDims */
        117,          /* lineNo */
        6,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtECInfo q_emlrtECI =
    {
        2,            /* nDims */
        117,          /* lineNo */
        6,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtBCInfo yq_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        119,          /* lineNo */
        13,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtECInfo r_emlrtECI =
    {
        2,            /* nDims */
        120,          /* lineNo */
        6,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtBCInfo ar_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        59,           /* lineNo */
        14,           /* colNo */
        "n",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo br_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        61,           /* lineNo */
        20,           /* colNo */
        "WN",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo cr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        61,           /* lineNo */
        24,           /* colNo */
        "WN",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo dr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        66,           /* lineNo */
        34,           /* colNo */
        "n",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo er_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        66,           /* lineNo */
        40,           /* colNo */
        "Z",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo fr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        66,           /* lineNo */
        16,           /* colNo */
        "Xs",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo gr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        66,           /* lineNo */
        18,           /* colNo */
        "Xs",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo rb_emlrtDCI =
    {
        66,           /* lineNo */
        18,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo hr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        73,           /* lineNo */
        16,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ir_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        73,           /* lineNo */
        20,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo jr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        73,           /* lineNo */
        26,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo sb_emlrtDCI =
    {
        73,           /* lineNo */
        26,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo kr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        76,           /* lineNo */
        18,           /* colNo */
        "n",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo lr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        86,           /* lineNo */
        27,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo tb_emlrtDCI =
    {
        86,           /* lineNo */
        27,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo mr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        100,          /* lineNo */
        16,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo nr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        100,          /* lineNo */
        26,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo ub_emlrtDCI =
    {
        100,          /* lineNo */
        26,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo or_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        95,           /* lineNo */
        20,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo pr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        95,           /* lineNo */
        30,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo vb_emlrtDCI =
    {
        95,           /* lineNo */
        30,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo qr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        34,           /* colNo */
        "WN",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo rr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        38,           /* colNo */
        "WN",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo sr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        44,           /* colNo */
        "L",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo tr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        48,           /* colNo */
        "L",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ur_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        54,           /* colNo */
        "mu",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo vr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        58,           /* colNo */
        "mu",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo wr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        67,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo xr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        71,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo yr_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        79,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo wb_emlrtDCI =
    {
        63,           /* lineNo */
        79,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo as_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        24,           /* colNo */
        "WN",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo bs_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        63,           /* lineNo */
        28,           /* colNo */
        "WN",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo cs_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        53,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ds_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        57,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo es_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        63,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo xb_emlrtDCI =
    {
        78,           /* lineNo */
        63,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo fs_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        82,           /* colNo */
        "L",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo gs_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        86,           /* colNo */
        "L",          /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo hs_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        92,           /* colNo */
        "mu",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo is_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        96,           /* colNo */
        "mu",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo js_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        103,          /* colNo */
        "Xs",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ks_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        105,          /* colNo */
        "Xs",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo yb_emlrtDCI =
    {
        78,           /* lineNo */
        105,          /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo ls_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        122,          /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ms_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        126,          /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ns_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        134,          /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo ac_emlrtDCI =
    {
        78,           /* lineNo */
        134,          /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo os_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        24,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ps_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        28,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo qs_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        78,           /* lineNo */
        34,           /* colNo */
        "pi",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo bc_emlrtDCI =
    {
        78,           /* lineNo */
        34,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo rs_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        106,          /* lineNo */
        23,           /* colNo */
        "Xs",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ss_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        106,          /* lineNo */
        32,           /* colNo */
        "Xs",         /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtDCInfo cc_emlrtDCI =
    {
        106,          /* lineNo */
        32,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        1               /* checkKind */
};

static emlrtBCInfo ts_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        108,          /* lineNo */
        26,           /* colNo */
        "lGN",        /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo us_emlrtBCI =
    {
        -1,           /* iFirst */
        -1,           /* iLast */
        108,          /* lineNo */
        13,           /* colNo */
        "lGN",        /* aName */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo vs_emlrtBCI =
    {
        -1,        /* iFirst */
        -1,        /* iLast */
        128,       /* lineNo */
        16,        /* colNo */
        "N",       /* aName */
        "hashpop", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo ws_emlrtBCI =
    {
        -1,        /* iFirst */
        -1,        /* iLast */
        128,       /* lineNo */
        18,        /* colNo */
        "N",       /* aName */
        "hashpop", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtBCInfo xs_emlrtBCI =
    {
        -1,        /* iFirst */
        -1,        /* iLast */
        128,       /* lineNo */
        28,        /* colNo */
        "n",       /* aName */
        "hashpop", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m", /* pName */
        0               /* checkKind */
};

static emlrtRTEInfo wg_emlrtRTEI =
    {
        31,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo xg_emlrtRTEI =
    {
        32,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo yg_emlrtRTEI =
    {
        33,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo ah_emlrtRTEI =
    {
        34,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo bh_emlrtRTEI =
    {
        35,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo ch_emlrtRTEI =
    {
        36,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo dh_emlrtRTEI =
    {
        39,           /* lineNo */
        8,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo eh_emlrtRTEI =
    {
        50,           /* lineNo */
        17,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo fh_emlrtRTEI =
    {
        50,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo gh_emlrtRTEI =
    {
        51,           /* lineNo */
        25,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo hh_emlrtRTEI =
    {
        51,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo ih_emlrtRTEI =
    {
        52,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo jh_emlrtRTEI =
    {
        54,           /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo kh_emlrtRTEI =
    {
        56,           /* lineNo */
        7,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo lh_emlrtRTEI =
    {
        113,          /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo mh_emlrtRTEI =
    {
        114,          /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo nh_emlrtRTEI =
    {
        115,          /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo oh_emlrtRTEI =
    {
        66,           /* lineNo */
        47,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo ph_emlrtRTEI =
    {
        116,          /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo qh_emlrtRTEI =
    {
        105,          /* lineNo */
        28,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo rh_emlrtRTEI =
    {
        86,           /* lineNo */
        20,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo sh_emlrtRTEI =
    {
        105,          /* lineNo */
        52,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo th_emlrtRTEI =
    {
        105,          /* lineNo */
        76,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo uh_emlrtRTEI =
    {
        117,          /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo vh_emlrtRTEI =
    {
        119,          /* lineNo */
        6,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo wh_emlrtRTEI =
    {
        119,          /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo yh_emlrtRTEI =
    {
        120,          /* lineNo */
        1,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo ai_emlrtRTEI =
    {
        26,           /* lineNo */
        39,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo bi_emlrtRTEI =
    {
        63,           /* lineNo */
        87,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo ci_emlrtRTEI =
    {
        128,          /* lineNo */
        14,           /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRTEInfo hi_emlrtRTEI =
    {
        117,          /* lineNo */
        6,            /* colNo */
        "pfqn_mvald", /* fName */
        "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/pfqn/"
        "pfqn_mvald.m" /* pName */
};

static emlrtRSInfo og_emlrtRSI = {
    54,    /* lineNo */
    "div", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/div.m" /* pathName
                                                                       */
};

/* Function Declarations */
static void b_times(const emlrtStack *sp, emxArray_real_T *in1,
                    const emxArray_real_T *in2);

static void binary_expand_op_2(const emlrtStack *sp, emxArray_real_T *in1,
                               const emxArray_real_T *in2,
                               const emxArray_real_T *in3);

static real_T hashpop(const emlrtStack *sp, const emxArray_real_T *n,
                      const emxArray_real_T *N);

/* Function Definitions */
static void b_times(const emlrtStack *sp, emxArray_real_T *in1,
                    const emxArray_real_T *in2)
{
  emxArray_real_T *b_in2;
  const real_T *in2_data;
  real_T *b_in2_data;
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
  emxInit_real_T(sp, &b_in2, 2, &hi_emlrtRTEI);
  if (in1->size[0] == 1) {
    loop_ub = in2->size[0];
  } else {
    loop_ub = in1->size[0];
  }
  stride_0_0 = b_in2->size[0] * b_in2->size[1];
  b_in2->size[0] = loop_ub;
  if (in1->size[1] == 1) {
    b_loop_ub = in2->size[1];
  } else {
    b_loop_ub = in1->size[1];
  }
  b_in2->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(sp, b_in2, stride_0_0, &hi_emlrtRTEI);
  b_in2_data = b_in2->data;
  stride_0_0 = (in2->size[0] != 1);
  stride_0_1 = (in2->size[1] != 1);
  stride_1_0 = (in1->size[0] != 1);
  stride_1_1 = (in1->size[1] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_in2_data[i1 + b_in2->size[0] * i] =
          in2_data[i1 * stride_0_0 + in2->size[0] * aux_0_1] *
          in1_data[i1 * stride_1_0 + in1->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_1;
    aux_0_1 += stride_0_1;
  }
  stride_0_0 = in1->size[0] * in1->size[1];
  in1->size[0] = loop_ub;
  in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_0, &hi_emlrtRTEI);
  in1_data = in1->data;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      in1_data[i1 + in1->size[0] * i] = b_in2_data[i1 + b_in2->size[0] * i];
    }
  }
  emxFree_real_T(sp, &b_in2);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

static void binary_expand_op_2(const emlrtStack *sp, emxArray_real_T *in1,
                               const emxArray_real_T *in2,
                               const emxArray_real_T *in3)
{
  jmp_buf *volatile emlrtJBStack;
  const real_T *in2_data;
  const real_T *in3_data;
  real_T *in1_data;
  int32_T binary_expand_op_2_numThreads;
  int32_T i;
  int32_T loop_ub;
  int32_T stride_0_1;
  int32_T stride_1_1;
  in3_data = in3->data;
  in2_data = in2->data;
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[0] = 1;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &yh_emlrtRTEI);
  if (in3->size[1] == 1) {
    loop_ub = in2->size[1];
  } else {
    loop_ub = in3->size[1];
  }
  stride_0_1 = in1->size[0] * in1->size[1];
  in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(sp, in1, stride_0_1, &yh_emlrtRTEI);
  in1_data = in1->data;
  stride_0_1 = (in2->size[1] != 1);
  stride_1_1 = (in3->size[1] != 1);
  if (loop_ub < 1600) {
    for (i = 0; i < loop_ub; i++) {
      in1_data[in1->size[0] * i] =
          in2_data[i * stride_0_1] - in3_data[i * stride_1_1];
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    binary_expand_op_2_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(binary_expand_op_2_numThreads)

    for (i = 0; i < loop_ub; i++) {
      in1_data[in1->size[0] * i] =
          in2_data[i * stride_0_1] - in3_data[i * stride_1_1];
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
}

static real_T hashpop(const emlrtStack *sp, const emxArray_real_T *n,
                      const emxArray_real_T *N)
{
  emlrtStack st;
  emxArray_real_T *b_N;
  const real_T *N_data;
  const real_T *n_data;
  real_T i;
  real_T *b_N_data;
  int32_T b_i;
  int32_T i1;
  int32_T r;
  st.prev = sp;
  st.tls = sp->tls;
  N_data = N->data;
  n_data = n->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /*  I=HASHPOP(N,N) */
  i = 1.0;
  /*  index of the empty population */
  b_i = N->size[1];
  emxInit_real_T(sp, &b_N, 2, &ci_emlrtRTEI);
  for (r = 0; r < b_i; r++) {
    int32_T loop_ub;
    int32_T scalarLB;
    int32_T vectorUB;
    if (r < 1) {
      loop_ub = 0;
    } else {
      if (b_i < 1) {
        emlrtDynamicBoundsCheckR2012b(1, 1, b_i, &vs_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      if (r > b_i) {
        emlrtDynamicBoundsCheckR2012b(r, 1, b_i, &ws_emlrtBCI,
                                      (emlrtConstCTX)sp);
      }
      loop_ub = r;
    }
    scalarLB = b_N->size[0] * b_N->size[1];
    b_N->size[0] = 1;
    b_N->size[1] = loop_ub;
    emxEnsureCapacity_real_T(sp, b_N, scalarLB, &ci_emlrtRTEI);
    b_N_data = b_N->data;
    scalarLB = (loop_ub / 2) << 1;
    vectorUB = scalarLB - 2;
    for (i1 = 0; i1 <= vectorUB; i1 += 2) {
      _mm_storeu_pd(&b_N_data[i1],
                    _mm_add_pd(_mm_loadu_pd(&N_data[i1]), _mm_set1_pd(1.0)));
    }
    for (i1 = scalarLB; i1 < loop_ub; i1++) {
      b_N_data[i1] = N_data[i1] + 1.0;
    }
    if (r + 1 > n->size[1]) {
      emlrtDynamicBoundsCheckR2012b(r + 1, 1, n->size[1], &xs_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    st.site = &mg_emlrtRSI;
    i += prod(&st, b_N) * n_data[r];
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  emxFree_real_T(sp, &b_N);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return i;
}

void pfqn_mvald(const emlrtStack *sp, const emxArray_real_T *L,
                const emxArray_real_T *N, const emxArray_real_T *Z,
                const emxArray_real_T *mu, emxArray_real_T *XN,
                emxArray_real_T *QN, emxArray_real_T *UN, emxArray_real_T *CN,
                emxArray_real_T *lGN)
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  emxArray_boolean_T *b_N;
  emxArray_real_T *WN;
  emxArray_real_T *X;
  emxArray_real_T *Xs;
  emxArray_real_T *b_n;
  emxArray_real_T *c_N;
  emxArray_real_T *d_N;
  emxArray_real_T *n;
  emxArray_real_T *pi;
  real_T dv[2];
  const real_T *L_data;
  const real_T *N_data;
  const real_T *Z_data;
  const real_T *mu_data;
  real_T *WN_data;
  real_T *XN_data;
  real_T *X_data;
  real_T *Xs_data;
  real_T *c_N_data;
  real_T *lGN_data;
  real_T *n_data;
  real_T *pi_data;
  int32_T M;
  int32_T R;
  int32_T b_loop_ub;
  int32_T d_loop_ub;
  int32_T e_loop_ub;
  int32_T i;
  int32_T ibmat;
  int32_T ist;
  int32_T itilerow;
  int32_T k;
  int32_T loop_ub;
  int32_T pfqn_mvald_numThreads;
  int32_T s;
  boolean_T *b_N_data;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  mu_data = mu->data;
  Z_data = Z->data;
  N_data = N->data;
  L_data = L->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  /* { */
  /*  % @file pfqn_mvald.m */
  /*  % @brief Exact MVA for load-dependent closed queueing networks. */
  /* } */
  /* { */
  /*  % @brief Exact MVA for load-dependent closed queueing networks. */
  /*  % @fn pfqn_mvald(L, N, Z, mu, stabilize) */
  /*  % @param L Service demand matrix. */
  /*  % @param N Population vector. */
  /*  % @param Z Think time vector. */
  /*  % @param mu Load-dependent rate matrix (MxNt). */
  /*  % @param stabilize Force non-negative probabilities (default: true). */
  /*  % @return XN System throughput. */
  /*  % @return QN Mean queue lengths. */
  /*  % @return UN Utilization. */
  /*  % @return CN Cycle times. */
  /*  % @return lGN Logarithm of normalizing constant evolution. */
  /*  % @return isNumStable Numerical stability flag. */
  /*  % @return pi Marginal queue-length probabilities. */
  /* } */
  /*  [XN,QN,UN,CN,LGN]=PFQN_MVALD(L,N,Z,MU) */
  R = L->size[1];
  M = L->size[0];
  /*  get number of queues (M) and classes (R) */
  loop_ub = XN->size[0] * XN->size[1];
  XN->size[0] = L->size[0];
  XN->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, XN, loop_ub, &wg_emlrtRTEI);
  XN_data = XN->data;
  ibmat = L->size[0] * L->size[1];
  for (ist = 0; ist < ibmat; ist++) {
    XN_data[ist] = 0.0;
  }
  loop_ub = QN->size[0] * QN->size[1];
  QN->size[0] = L->size[0];
  QN->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, QN, loop_ub, &xg_emlrtRTEI);
  XN_data = QN->data;
  for (ist = 0; ist < ibmat; ist++) {
    XN_data[ist] = 0.0;
  }
  loop_ub = UN->size[0] * UN->size[1];
  UN->size[0] = L->size[0];
  UN->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, UN, loop_ub, &yg_emlrtRTEI);
  XN_data = UN->data;
  for (ist = 0; ist < ibmat; ist++) {
    XN_data[ist] = 0.0;
  }
  loop_ub = CN->size[0] * CN->size[1];
  CN->size[0] = L->size[0];
  CN->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, CN, loop_ub, &ah_emlrtRTEI);
  XN_data = CN->data;
  for (ist = 0; ist < ibmat; ist++) {
    XN_data[ist] = 0.0;
  }
  emxInit_real_T(sp, &WN, 2, &bh_emlrtRTEI);
  loop_ub = WN->size[0] * WN->size[1];
  WN->size[0] = L->size[0];
  WN->size[1] = L->size[1];
  emxEnsureCapacity_real_T(sp, WN, loop_ub, &bh_emlrtRTEI);
  WN_data = WN->data;
  for (ist = 0; ist < ibmat; ist++) {
    WN_data[ist] = 0.0;
  }
  loop_ub = lGN->size[0] * lGN->size[1];
  lGN->size[0] = 1;
  lGN->size[1] = 1;
  emxEnsureCapacity_real_T(sp, lGN, loop_ub, &ch_emlrtRTEI);
  lGN_data = lGN->data;
  lGN_data[0] = rtMinusInf;
  emxInit_boolean_T(sp, &b_N, &dh_emlrtRTEI);
  loop_ub = b_N->size[0] * b_N->size[1];
  b_N->size[0] = 1;
  b_loop_ub = N->size[1];
  b_N->size[1] = N->size[1];
  emxEnsureCapacity_boolean_T(sp, b_N, loop_ub, &dh_emlrtRTEI);
  b_N_data = b_N->data;
  ibmat = N->size[1];
  if (N->size[1] < 1600) {
    for (i = 0; i < b_loop_ub; i++) {
      b_N_data[i] = (N_data[i] < 0.0);
    }
  } else {
    emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
    emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    pfqn_mvald_numThreads = emlrtAllocRegionTLSs(
        sp->tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(pfqn_mvald_numThreads)

    for (i = 0; i < ibmat; i++) {
      b_N_data[i] = (N_data[i] < 0.0);
    }
    emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
    emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
  }
  st.site = &lg_emlrtRSI;
  if (!any(&st, b_N)) {
    __m128d r;
    real_T d;
    real_T numLGTerms;
    int32_T c_loop_ub;
    int32_T last_nnz;
    int32_T scalarLB;
    int32_T vectorUB;
    uint32_T lgIdx;
    /*  stabilize ensures that probabilities do not become negative */
    emxInit_real_T(sp, &c_N, 2, &eh_emlrtRTEI);
    loop_ub = c_N->size[0] * c_N->size[1];
    c_N->size[0] = 1;
    c_N->size[1] = b_loop_ub;
    emxEnsureCapacity_real_T(sp, c_N, loop_ub, &eh_emlrtRTEI);
    XN_data = c_N->data;
    scalarLB = (N->size[1] / 2) << 1;
    ibmat = scalarLB - 2;
    for (ist = 0; ist <= ibmat; ist += 2) {
      _mm_storeu_pd(&XN_data[ist],
                    _mm_add_pd(_mm_loadu_pd(&N_data[ist]), _mm_set1_pd(1.0)));
    }
    for (ist = scalarLB; ist < b_loop_ub; ist++) {
      XN_data[ist] = N_data[ist] + 1.0;
    }
    st.site = &kg_emlrtRSI;
    numLGTerms = prod(&st, c_N);
    if (!(numLGTerms >= 0.0)) {
      emlrtNonNegativeCheckR2012b(numLGTerms, &gb_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
      emlrtIntegerCheckR2012b(numLGTerms, &hb_emlrtDCI, (emlrtConstCTX)sp);
    }
    emxInit_real_T(sp, &Xs, 2, &fh_emlrtRTEI);
    loop_ub = Xs->size[0] * Xs->size[1];
    Xs->size[0] = R;
    Xs->size[1] = (int32_T)numLGTerms;
    emxEnsureCapacity_real_T(sp, Xs, loop_ub, &fh_emlrtRTEI);
    Xs_data = Xs->data;
    ibmat = L->size[1] * (int32_T)numLGTerms;
    for (ist = 0; ist < ibmat; ist++) {
      Xs_data[ist] = 0.0;
    }
    /*  throughput for a model with station i less */
    st.site = &jg_emlrtRSI;
    numLGTerms = b_sum(&st, N) + 1.0;
    if (!(numLGTerms >= 0.0)) {
      emlrtNonNegativeCheckR2012b(numLGTerms, &ib_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
      emlrtIntegerCheckR2012b(numLGTerms, &jb_emlrtDCI, (emlrtConstCTX)sp);
    }
    loop_ub = c_N->size[0] * c_N->size[1];
    c_N->size[0] = 1;
    c_N->size[1] = b_loop_ub;
    emxEnsureCapacity_real_T(sp, c_N, loop_ub, &gh_emlrtRTEI);
    XN_data = c_N->data;
    ibmat = scalarLB - 2;
    for (ist = 0; ist <= ibmat; ist += 2) {
      _mm_storeu_pd(&XN_data[ist],
                    _mm_add_pd(_mm_loadu_pd(&N_data[ist]), _mm_set1_pd(1.0)));
    }
    for (ist = scalarLB; ist < b_loop_ub; ist++) {
      XN_data[ist] = N_data[ist] + 1.0;
    }
    st.site = &jg_emlrtRSI;
    d = prod(&st, c_N);
    if (!(d >= 0.0)) {
      emlrtNonNegativeCheckR2012b(d, &kb_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (d != (int32_T)muDoubleScalarFloor(d)) {
      emlrtIntegerCheckR2012b(d, &lb_emlrtDCI, (emlrtConstCTX)sp);
    }
    emxInit_real_T(sp, &pi, 3, &ai_emlrtRTEI);
    loop_ub = pi->size[0] * pi->size[1] * pi->size[2];
    pi->size[0] = M;
    pi->size[1] = (int32_T)numLGTerms;
    pi->size[2] = (int32_T)d;
    emxEnsureCapacity_real_T(sp, pi, loop_ub, &hh_emlrtRTEI);
    pi_data = pi->data;
    ibmat = L->size[0] * (int32_T)numLGTerms * (int32_T)d;
    for (ist = 0; ist < ibmat; ist++) {
      pi_data[ist] = 1.0;
    }
    /*  marginal queue-length probabilities pi(k) */
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
    emxInit_real_T(sp, &n, 2, &ih_emlrtRTEI);
    loop_ub = n->size[0] * n->size[1];
    n->size[0] = 1;
    n->size[1] = N->size[1];
    emxEnsureCapacity_real_T(sp, n, loop_ub, &ih_emlrtRTEI);
    n_data = n->data;
    ibmat = N->size[1];
    for (ist = 0; ist < ibmat; ist++) {
      n_data[ist] = 0.0;
    }
    /*  initialize the current population */
    st.site = &ig_emlrtRSI;
    numLGTerms = b_sum(&st, N) + 1.0;
    if (!(numLGTerms >= 0.0)) {
      emlrtNonNegativeCheckR2012b(numLGTerms, &mb_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
      emlrtIntegerCheckR2012b(numLGTerms, &nb_emlrtDCI, (emlrtConstCTX)sp);
    }
    ibmat = lGN->size[0] * lGN->size[1];
    lGN->size[0] = 1;
    loop_ub = (int32_T)numLGTerms;
    lGN->size[1] = (int32_T)numLGTerms;
    emxEnsureCapacity_real_T(sp, lGN, ibmat, &jh_emlrtRTEI);
    lGN_data = lGN->data;
    for (ist = 0; ist < loop_ub; ist++) {
      lGN_data[ist] = 0.0;
    }
    lgIdx = 1U;
    emxInit_real_T(sp, &X, 1, &mh_emlrtRTEI);
    emxInit_real_T(sp, &d_N, 2, &bi_emlrtRTEI);
    c_N_data = d_N->data;
    emxInit_real_T(sp, &b_n, 2, &qh_emlrtRTEI);
    int32_T exitg1;
    do {
      exitg1 = 0;
      ibmat = b_N->size[0] * b_N->size[1];
      b_N->size[0] = 1;
      c_loop_ub = n->size[1];
      b_N->size[1] = n->size[1];
      emxEnsureCapacity_boolean_T(sp, b_N, ibmat, &kh_emlrtRTEI);
      b_N_data = b_N->data;
      for (ist = 0; ist < c_loop_ub; ist++) {
        b_N_data[ist] = (n_data[ist] != -1.0);
      }
      st.site = &hg_emlrtRSI;
      if (ifWhileCond(&st, b_N)) {
        boolean_T exitg2;
        ibmat = WN->size[0] * WN->size[1];
        loop_ub = (ibmat / 2) << 1;
        vectorUB = loop_ub - 2;
        for (itilerow = 0; itilerow <= vectorUB; itilerow += 2) {
          r = _mm_loadu_pd(&WN_data[itilerow]);
          _mm_storeu_pd(&WN_data[itilerow], _mm_mul_pd(_mm_set1_pd(0.0), r));
        }
        for (itilerow = loop_ub; itilerow < ibmat; itilerow++) {
          WN_data[itilerow] *= 0.0;
        }
        for (s = 0; s < R; s++) {
          if (s + 1 > c_loop_ub) {
            emlrtDynamicBoundsCheckR2012b(s + 1, 1, c_loop_ub, &ar_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          d = n_data[s];
          if (d > 0.0) {
            for (ist = 0; ist < M; ist++) {
              if (ist + 1 > WN->size[0]) {
                emlrtDynamicBoundsCheckR2012b(ist + 1, 1, WN->size[0],
                                              &br_emlrtBCI, (emlrtConstCTX)sp);
              }
              if (s + 1 > WN->size[1]) {
                emlrtDynamicBoundsCheckR2012b(s + 1, 1, WN->size[1],
                                              &cr_emlrtBCI, (emlrtConstCTX)sp);
              }
              WN_data[ist + WN->size[0] * s] = 0.0;
              st.site = &gg_emlrtRSI;
              numLGTerms = b_sum(&st, n);
              loop_ub = (int32_T)numLGTerms;
              emlrtForLoopVectorCheckR2021a(1.0, 1.0, numLGTerms,
                                            mxDOUBLE_CLASS, (int32_T)numLGTerms,
                                            &bb_emlrtRTEI, (emlrtConstCTX)sp);
              if ((int32_T)numLGTerms - 1 >= 0) {
                e_loop_ub = c_loop_ub;
              }
              for (k = 0; k < loop_ub; k++) {
                st.site = &fg_emlrtRSI;
                ibmat = d_N->size[0] * d_N->size[1];
                d_N->size[0] = 1;
                d_N->size[1] = c_loop_ub;
                emxEnsureCapacity_real_T(&st, d_N, ibmat, &xe_emlrtRTEI);
                c_N_data = d_N->data;
                for (itilerow = 0; itilerow < e_loop_ub; itilerow++) {
                  c_N_data[itilerow] = n_data[itilerow];
                }
                /*  N=ONER(N,r) */
                /*  Decrement element in position of r of input vector */
                /*  */
                /*  Copyright (c) 2012-2026, Imperial College London */
                /*  All rights reserved. */
                if (s + 1 > c_loop_ub) {
                  emlrtDynamicBoundsCheckR2012b(s + 1, 1, c_loop_ub,
                                                &ph_emlrtBCI, &st);
                }
                c_N_data[s] = d - 1.0;
                if (ist + 1 > WN->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(
                      ist + 1, 1, WN->size[0], &qr_emlrtBCI, (emlrtConstCTX)sp);
                }
                if (s + 1 > WN->size[1]) {
                  emlrtDynamicBoundsCheckR2012b(
                      s + 1, 1, WN->size[1], &rr_emlrtBCI, (emlrtConstCTX)sp);
                }
                if (ist + 1 > M) {
                  emlrtDynamicBoundsCheckR2012b(ist + 1, 1, M, &sr_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (s + 1 > R) {
                  emlrtDynamicBoundsCheckR2012b(s + 1, 1, R, &tr_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (ist + 1 > mu->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(
                      ist + 1, 1, mu->size[0], &ur_emlrtBCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)((uint32_T)k + 1U) < 1) ||
                    ((int32_T)((uint32_T)k + 1U) > mu->size[1])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)k + 1U), 1,
                                                mu->size[1], &vr_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (ist + 1 > pi->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(
                      ist + 1, 1, pi->size[0], &wr_emlrtBCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)((uint32_T)k + 1U) < 1) ||
                    ((int32_T)((uint32_T)k + 1U) > pi->size[1])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)k + 1U), 1,
                                                pi->size[1], &xr_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                st.site = &fg_emlrtRSI;
                numLGTerms = hashpop(&st, d_N, N);
                if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
                  emlrtIntegerCheckR2012b(numLGTerms, &wb_emlrtDCI,
                                          (emlrtConstCTX)sp);
                }
                if (((int32_T)numLGTerms < 1) ||
                    ((int32_T)numLGTerms > pi->size[2])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1,
                                                pi->size[2], &yr_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (ist + 1 > WN->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(
                      ist + 1, 1, WN->size[0], &as_emlrtBCI, (emlrtConstCTX)sp);
                }
                if (s + 1 > WN->size[1]) {
                  emlrtDynamicBoundsCheckR2012b(
                      s + 1, 1, WN->size[1], &bs_emlrtBCI, (emlrtConstCTX)sp);
                }
                WN_data[ist + WN->size[0] * s] +=
                    L_data[ist + L->size[0] * s] /
                    mu_data[ist + mu->size[0] * k] * ((real_T)k + 1.0) *
                    pi_data[(ist + pi->size[0] * k) +
                            pi->size[0] * pi->size[1] *
                                ((int32_T)numLGTerms - 1)];
                if (*emlrtBreakCheckR2012bFlagVar != 0) {
                  emlrtBreakCheckR2012b((emlrtConstCTX)sp);
                }
              }
              if (*emlrtBreakCheckR2012bFlagVar != 0) {
                emlrtBreakCheckR2012b((emlrtConstCTX)sp);
              }
            }
            if (s + 1 > WN->size[1]) {
              emlrtDynamicBoundsCheckR2012b(s + 1, 1, WN->size[1], &jq_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            loop_ub = WN->size[0];
            ibmat = X->size[0];
            X->size[0] = WN->size[0];
            emxEnsureCapacity_real_T(sp, X, ibmat, &oh_emlrtRTEI);
            X_data = X->data;
            for (ist = 0; ist < loop_ub; ist++) {
              X_data[ist] = WN_data[ist + WN->size[0] * s];
            }
            if (s + 1 > c_loop_ub) {
              emlrtDynamicBoundsCheckR2012b(s + 1, 1, c_loop_ub, &dr_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (s + 1 > Z->size[1]) {
              emlrtDynamicBoundsCheckR2012b(s + 1, 1, Z->size[1], &er_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (s + 1 > Xs->size[0]) {
              emlrtDynamicBoundsCheckR2012b(s + 1, 1, Xs->size[0], &fr_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            st.site = &cg_emlrtRSI;
            numLGTerms = hashpop(&st, n, N);
            if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
              emlrtIntegerCheckR2012b(numLGTerms, &rb_emlrtDCI,
                                      (emlrtConstCTX)sp);
            }
            if (((int32_T)numLGTerms < 1) ||
                ((int32_T)numLGTerms > Xs->size[1])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1, Xs->size[1],
                                            &gr_emlrtBCI, (emlrtConstCTX)sp);
            }
            st.site = &cg_emlrtRSI;
            Xs_data[s + Xs->size[0] * ((int32_T)numLGTerms - 1)] =
                d / (Z_data[s] + c_sum(&st, X));
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b((emlrtConstCTX)sp);
          }
        }
        /*  compute pi(k|n) */
        st.site = &eg_emlrtRSI;
        numLGTerms = b_sum(&st, n);
        loop_ub = (int32_T)numLGTerms;
        emlrtForLoopVectorCheckR2021a(1.0, 1.0, numLGTerms, mxDOUBLE_CLASS,
                                      (int32_T)numLGTerms, &cb_emlrtRTEI,
                                      (emlrtConstCTX)sp);
        for (s = 0; s < loop_ub; s++) {
          for (ist = 0; ist < M; ist++) {
            if (ist + 1 > pi->size[0]) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, pi->size[0],
                                            &hr_emlrtBCI, (emlrtConstCTX)sp);
            }
            if (((int32_T)((uint32_T)s + 2U) < 1) ||
                ((int32_T)((uint32_T)s + 2U) > pi->size[1])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)s + 2U), 1,
                                            pi->size[1], &ir_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            st.site = &dg_emlrtRSI;
            numLGTerms = hashpop(&st, n, N);
            if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
              emlrtIntegerCheckR2012b(numLGTerms, &sb_emlrtDCI,
                                      (emlrtConstCTX)sp);
            }
            if (((int32_T)numLGTerms < 1) ||
                ((int32_T)numLGTerms > pi->size[2])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1, pi->size[2],
                                            &jr_emlrtBCI, (emlrtConstCTX)sp);
            }
            pi_data[(ist + pi->size[0] * (s + 1)) +
                    pi->size[0] * pi->size[1] * ((int32_T)numLGTerms - 1)] =
                0.0;
            if (*emlrtBreakCheckR2012bFlagVar != 0) {
              emlrtBreakCheckR2012b((emlrtConstCTX)sp);
            }
          }
          for (ist = 0; ist < R; ist++) {
            if (ist + 1 > c_loop_ub) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, c_loop_ub, &kr_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            numLGTerms = n_data[ist];
            if (numLGTerms > 0.0) {
              if (M - 1 >= 0) {
                d_loop_ub = c_loop_ub;
              }
              for (k = 0; k < M; k++) {
                real_T d1;
                real_T d2;
                real_T d3;
                st.site = &bg_emlrtRSI;
                ibmat = d_N->size[0] * d_N->size[1];
                d_N->size[0] = 1;
                d_N->size[1] = c_loop_ub;
                emxEnsureCapacity_real_T(&st, d_N, ibmat, &xe_emlrtRTEI);
                c_N_data = d_N->data;
                for (itilerow = 0; itilerow < d_loop_ub; itilerow++) {
                  c_N_data[itilerow] = n_data[itilerow];
                }
                /*  N=ONER(N,r) */
                /*  Decrement element in position of r of input vector */
                /*  */
                /*  Copyright (c) 2012-2026, Imperial College London */
                /*  All rights reserved. */
                if (ist + 1 > c_loop_ub) {
                  emlrtDynamicBoundsCheckR2012b(ist + 1, 1, c_loop_ub,
                                                &ph_emlrtBCI, &st);
                }
                c_N_data[ist] = numLGTerms - 1.0;
                if (k + 1 > pi->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(
                      k + 1, 1, pi->size[0], &cs_emlrtBCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)((uint32_T)s + 2U) < 1) ||
                    ((int32_T)((uint32_T)s + 2U) > pi->size[1])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)s + 2U), 1,
                                                pi->size[1], &ds_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                st.site = &bg_emlrtRSI;
                d = hashpop(&st, n, N);
                if (d != (int32_T)muDoubleScalarFloor(d)) {
                  emlrtIntegerCheckR2012b(d, &xb_emlrtDCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)d < 1) || ((int32_T)d > pi->size[2])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)d, 1, pi->size[2],
                                                &es_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (k + 1 > M) {
                  emlrtDynamicBoundsCheckR2012b(k + 1, 1, M, &fs_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (ist + 1 > R) {
                  emlrtDynamicBoundsCheckR2012b(ist + 1, 1, R, &gs_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (k + 1 > mu->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(
                      k + 1, 1, mu->size[0], &hs_emlrtBCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)((uint32_T)s + 1U) < 1) ||
                    ((int32_T)((uint32_T)s + 1U) > mu->size[1])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)s + 1U), 1,
                                                mu->size[1], &is_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (ist + 1 > Xs->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(
                      ist + 1, 1, Xs->size[0], &js_emlrtBCI, (emlrtConstCTX)sp);
                }
                st.site = &bg_emlrtRSI;
                d1 = hashpop(&st, n, N);
                if (d1 != (int32_T)muDoubleScalarFloor(d1)) {
                  emlrtIntegerCheckR2012b(d1, &yb_emlrtDCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)d1 < 1) || ((int32_T)d1 > Xs->size[1])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)d1, 1, Xs->size[1],
                                                &ks_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (k + 1 > pi->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(
                      k + 1, 1, pi->size[0], &ls_emlrtBCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)((uint32_T)s + 1U) < 1) ||
                    ((int32_T)((uint32_T)s + 1U) > pi->size[1])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)s + 1U), 1,
                                                pi->size[1], &ms_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                st.site = &bg_emlrtRSI;
                d2 = hashpop(&st, d_N, N);
                if (d2 != (int32_T)muDoubleScalarFloor(d2)) {
                  emlrtIntegerCheckR2012b(d2, &ac_emlrtDCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)d2 < 1) || ((int32_T)d2 > pi->size[2])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)d2, 1, pi->size[2],
                                                &ns_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                if (k + 1 > pi->size[0]) {
                  emlrtDynamicBoundsCheckR2012b(
                      k + 1, 1, pi->size[0], &os_emlrtBCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)((uint32_T)s + 2U) < 1) ||
                    ((int32_T)((uint32_T)s + 2U) > pi->size[1])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)((uint32_T)s + 2U), 1,
                                                pi->size[1], &ps_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                st.site = &bg_emlrtRSI;
                d3 = hashpop(&st, n, N);
                if (d3 != (int32_T)muDoubleScalarFloor(d3)) {
                  emlrtIntegerCheckR2012b(d3, &bc_emlrtDCI, (emlrtConstCTX)sp);
                }
                if (((int32_T)d3 < 1) || ((int32_T)d3 > pi->size[2])) {
                  emlrtDynamicBoundsCheckR2012b((int32_T)d3, 1, pi->size[2],
                                                &qs_emlrtBCI,
                                                (emlrtConstCTX)sp);
                }
                pi_data[(k + pi->size[0] * (s + 1)) +
                        pi->size[0] * pi->size[1] * ((int32_T)d3 - 1)] =
                    pi_data[(k + pi->size[0] * (s + 1)) +
                            pi->size[0] * pi->size[1] * ((int32_T)d - 1)] +
                    L_data[k + L->size[0] * ist] /
                        mu_data[k + mu->size[0] * s] *
                        Xs_data[ist + Xs->size[0] * ((int32_T)d1 - 1)] *
                        pi_data[(k + pi->size[0] * s) +
                                pi->size[0] * pi->size[1] * ((int32_T)d2 - 1)];
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
        /*  compute pi(0|n) */
        for (ist = 0; ist < M; ist++) {
          st.site = &ag_emlrtRSI;
          numLGTerms = b_sum(&st, n);
          if (muDoubleScalarIsNaN(numLGTerms)) {
            vectorUB = d_N->size[0] * d_N->size[1];
            d_N->size[0] = 1;
            d_N->size[1] = 1;
            emxEnsureCapacity_real_T(sp, d_N, vectorUB, &gb_emlrtRTEI);
            c_N_data = d_N->data;
            c_N_data[0] = rtNaN;
          } else if (numLGTerms < 1.0) {
            d_N->size[0] = 1;
            d_N->size[1] = 0;
          } else {
            vectorUB = d_N->size[0] * d_N->size[1];
            d_N->size[0] = 1;
            d_N->size[1] = (int32_T)(numLGTerms - 1.0) + 1;
            emxEnsureCapacity_real_T(sp, d_N, vectorUB, &gb_emlrtRTEI);
            c_N_data = d_N->data;
            ibmat = (int32_T)(numLGTerms - 1.0);
            loop_ub = (((int32_T)(numLGTerms - 1.0) + 1) / 2) << 1;
            vectorUB = loop_ub - 2;
            for (itilerow = 0; itilerow <= vectorUB; itilerow += 2) {
              dv[0] = itilerow;
              dv[1] = itilerow + 1;
              r = _mm_loadu_pd(&dv[0]);
              _mm_storeu_pd(&c_N_data[itilerow],
                            _mm_add_pd(_mm_set1_pd(1.0), r));
            }
            for (itilerow = loop_ub; itilerow <= ibmat; itilerow++) {
              c_N_data[itilerow] = (real_T)itilerow + 1.0;
            }
          }
          if (ist + 1 > pi->size[0]) {
            emlrtDynamicBoundsCheckR2012b(ist + 1, 1, pi->size[0], &kq_emlrtBCI,
                                          (emlrtConstCTX)sp);
          }
          st.site = &ag_emlrtRSI;
          numLGTerms = hashpop(&st, n, N);
          if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
            emlrtIntegerCheckR2012b(numLGTerms, &ob_emlrtDCI,
                                    (emlrtConstCTX)sp);
          }
          if (((int32_T)numLGTerms < 1) ||
              ((int32_T)numLGTerms > pi->size[2])) {
            emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1, pi->size[2],
                                          &lq_emlrtBCI, (emlrtConstCTX)sp);
          }
          ibmat = c_N->size[0] * c_N->size[1];
          c_N->size[0] = 1;
          loop_ub = d_N->size[1];
          c_N->size[1] = d_N->size[1];
          emxEnsureCapacity_real_T(sp, c_N, ibmat, &rh_emlrtRTEI);
          XN_data = c_N->data;
          for (itilerow = 0; itilerow < loop_ub; itilerow++) {
            d = c_N_data[itilerow] + 1.0;
            if (d != (int32_T)muDoubleScalarFloor(d)) {
              emlrtIntegerCheckR2012b(d, &tb_emlrtDCI, (emlrtConstCTX)sp);
            }
            if (((int32_T)d < 1) || ((int32_T)d > pi->size[1])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)d, 1, pi->size[1],
                                            &lr_emlrtBCI, (emlrtConstCTX)sp);
            }
            XN_data[itilerow] =
                pi_data[(ist + pi->size[0] * ((int32_T)d - 1)) +
                        pi->size[0] * pi->size[1] * ((int32_T)numLGTerms - 1)];
          }
          st.site = &ag_emlrtRSI;
          numLGTerms = 1.0 - b_sum(&st, c_N);
          if (numLGTerms < 0.0) {
            if (pi->size[1] < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, pi->size[1], &mq_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (ist + 1 > pi->size[0]) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, pi->size[0],
                                            &or_emlrtBCI, (emlrtConstCTX)sp);
            }
            st.site = &yf_emlrtRSI;
            numLGTerms = hashpop(&st, n, N);
            if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
              emlrtIntegerCheckR2012b(numLGTerms, &vb_emlrtDCI,
                                      (emlrtConstCTX)sp);
            }
            if (((int32_T)numLGTerms < 1) ||
                ((int32_T)numLGTerms > pi->size[2])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1, pi->size[2],
                                            &pr_emlrtBCI, (emlrtConstCTX)sp);
            }
            pi_data[ist +
                    pi->size[0] * pi->size[1] * ((int32_T)numLGTerms - 1)] =
                2.2204460492503131E-16;
          } else {
            if (pi->size[1] < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, pi->size[1], &nq_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if (ist + 1 > pi->size[0]) {
              emlrtDynamicBoundsCheckR2012b(ist + 1, 1, pi->size[0],
                                            &mr_emlrtBCI, (emlrtConstCTX)sp);
            }
            st.site = &xf_emlrtRSI;
            d = hashpop(&st, n, N);
            if (d != (int32_T)muDoubleScalarFloor(d)) {
              emlrtIntegerCheckR2012b(d, &ub_emlrtDCI, (emlrtConstCTX)sp);
            }
            if (((int32_T)d < 1) || ((int32_T)d > pi->size[2])) {
              emlrtDynamicBoundsCheckR2012b((int32_T)d, 1, pi->size[2],
                                            &nr_emlrtBCI, (emlrtConstCTX)sp);
            }
            pi_data[ist + pi->size[0] * pi->size[1] * ((int32_T)d - 1)] =
                numLGTerms;
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b((emlrtConstCTX)sp);
          }
        }
        st.site = &wf_emlrtRSI;
        last_nnz = 0;
        emlrtForLoopVectorCheckR2021a(n->size[1], -1.0, 1.0, mxDOUBLE_CLASS,
                                      n->size[1], &h_emlrtRTEI, &st);
        ibmat = 0;
        exitg2 = false;
        while ((!exitg2) && (ibmat <= c_loop_ub - 1)) {
          loop_ub = c_loop_ub - ibmat;
          if ((loop_ub < 1) || (loop_ub > c_loop_ub)) {
            emlrtDynamicBoundsCheckR2012b(loop_ub, 1, c_loop_ub, &ff_emlrtBCI,
                                          &st);
          }
          if (n_data[loop_ub - 1] != 0.0) {
            last_nnz = loop_ub;
            exitg2 = true;
          } else {
            ibmat++;
          }
          if (*emlrtBreakCheckR2012bFlagVar != 0) {
            emlrtBreakCheckR2012b(&st);
          }
        }
        if (last_nnz > 0) {
          if (last_nnz - 1 < 1) {
            loop_ub = 0;
            c_loop_ub = 0;
          } else {
            if (n->size[1] < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, n->size[1], &oq_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if ((last_nnz - 1 < 1) || (last_nnz - 1 > n->size[1])) {
              emlrtDynamicBoundsCheckR2012b(last_nnz - 1, 1, n->size[1],
                                            &pq_emlrtBCI, (emlrtConstCTX)sp);
            }
            loop_ub = last_nnz - 1;
            if (b_loop_ub < 1) {
              emlrtDynamicBoundsCheckR2012b(1, 1, b_loop_ub, &qq_emlrtBCI,
                                            (emlrtConstCTX)sp);
            }
            if ((last_nnz - 1 < 1) || (last_nnz - 1 > b_loop_ub)) {
              emlrtDynamicBoundsCheckR2012b(last_nnz - 1, 1, b_loop_ub,
                                            &rq_emlrtBCI, (emlrtConstCTX)sp);
            }
            c_loop_ub = last_nnz - 1;
          }
          ibmat = b_n->size[0] * b_n->size[1];
          b_n->size[0] = 1;
          b_n->size[1] = loop_ub;
          emxEnsureCapacity_real_T(sp, b_n, ibmat, &qh_emlrtRTEI);
          XN_data = b_n->data;
          for (itilerow = 0; itilerow < loop_ub; itilerow++) {
            XN_data[itilerow] = n_data[itilerow];
          }
          vectorUB = c_N->size[0] * c_N->size[1];
          c_N->size[0] = 1;
          c_N->size[1] = c_loop_ub;
          emxEnsureCapacity_real_T(sp, c_N, vectorUB, &sh_emlrtRTEI);
          XN_data = c_N->data;
          for (ist = 0; ist < c_loop_ub; ist++) {
            XN_data[ist] = N_data[ist];
          }
          st.site = &vf_emlrtRSI;
          if (b_sum(&st, b_n) == b_sum(&st, c_N)) {
            if ((uint32_T)last_nnz + 1U > (uint32_T)R) {
              vectorUB = 0;
              ibmat = 0;
            } else {
              if (((int32_T)((uint32_T)last_nnz + 1U) < 1) ||
                  ((int32_T)((uint32_T)last_nnz + 1U) > n->size[1])) {
                emlrtDynamicBoundsCheckR2012b(
                    (int32_T)((uint32_T)last_nnz + 1U), 1, n->size[1],
                    &sq_emlrtBCI, (emlrtConstCTX)sp);
              }
              vectorUB = last_nnz;
              if ((R < 1) || (R > n->size[1])) {
                emlrtDynamicBoundsCheckR2012b(R, 1, n->size[1], &tq_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              ibmat = R;
            }
            loop_ub = b_n->size[0] * b_n->size[1];
            b_n->size[0] = 1;
            ibmat -= vectorUB;
            b_n->size[1] = ibmat;
            emxEnsureCapacity_real_T(sp, b_n, loop_ub, &th_emlrtRTEI);
            XN_data = b_n->data;
            for (itilerow = 0; itilerow < ibmat; itilerow++) {
              XN_data[itilerow] = n_data[vectorUB + itilerow];
            }
            st.site = &vf_emlrtRSI;
            if (b_sum(&st, b_n) == 0.0) {
              st.site = &uf_emlrtRSI;
              if (last_nnz > Xs->size[0]) {
                emlrtDynamicBoundsCheckR2012b(last_nnz, 1, Xs->size[0],
                                              &rs_emlrtBCI, &st);
              }
              b_st.site = &uf_emlrtRSI;
              numLGTerms = hashpop(&b_st, n, N);
              if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
                emlrtIntegerCheckR2012b(numLGTerms, &cc_emlrtDCI, &st);
              }
              if (((int32_T)numLGTerms < 1) ||
                  ((int32_T)numLGTerms > Xs->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1,
                                              Xs->size[1], &ss_emlrtBCI, &st);
              }
              numLGTerms =
                  Xs_data[(last_nnz + Xs->size[0] * ((int32_T)numLGTerms - 1)) -
                          1];
              if (numLGTerms < 0.0) {
                emlrtErrorWithMessageIdR2018a(
                    &st, &f_emlrtRTEI, "Coder:toolbox:ElFunDomainError",
                    "Coder:toolbox:ElFunDomainError", 3, 4, 3, "log");
              }
              numLGTerms = muDoubleScalarLog(numLGTerms);
              lgIdx++;
              if (((int32_T)lgIdx - 1 < 1) ||
                  ((int32_T)lgIdx - 1 > lGN->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)lgIdx - 1, 1,
                                              lGN->size[1], &ts_emlrtBCI,
                                              (emlrtConstCTX)sp);
              }
              if (((int32_T)lgIdx < 1) || ((int32_T)lgIdx > lGN->size[1])) {
                emlrtDynamicBoundsCheckR2012b((int32_T)lgIdx, 1, lGN->size[1],
                                              &us_emlrtBCI, (emlrtConstCTX)sp);
              }
              lGN_data[(int32_T)lgIdx - 1] =
                  lGN_data[(int32_T)lgIdx - 2] - numLGTerms;
            }
          }
        }
        st.site = &tf_emlrtRSI;
        pprod(&st, n, N);
        n_data = n->data;
        /*  get the next population */
      } else {
        exitg1 = 1;
      }
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b((emlrtConstCTX)sp);
      }
    } while (exitg1 == 0);
    emxFree_real_T(sp, &b_n);
    emxFree_real_T(sp, &c_N);
    emxFree_real_T(sp, &n);
    if (lGN->size[1] < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, lGN->size[1], &uq_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    if (((int32_T)lgIdx < 1) || ((int32_T)lgIdx > lGN->size[1])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)lgIdx, 1, lGN->size[1],
                                    &vq_emlrtBCI, (emlrtConstCTX)sp);
    }
    loop_ub = lGN->size[0] * lGN->size[1];
    lGN->size[1] = (int32_T)lgIdx;
    emxEnsureCapacity_real_T(sp, lGN, loop_ub, &lh_emlrtRTEI);
    st.site = &sf_emlrtRSI;
    numLGTerms = hashpop(&st, N, N);
    if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
      emlrtIntegerCheckR2012b(numLGTerms, &pb_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (((int32_T)numLGTerms < 1) || ((int32_T)numLGTerms > Xs->size[1])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1, Xs->size[1],
                                    &wq_emlrtBCI, (emlrtConstCTX)sp);
    }
    c_loop_ub = Xs->size[0];
    loop_ub = X->size[0];
    X->size[0] = Xs->size[0];
    emxEnsureCapacity_real_T(sp, X, loop_ub, &mh_emlrtRTEI);
    X_data = X->data;
    for (ist = 0; ist < c_loop_ub; ist++) {
      X_data[ist] = Xs_data[ist + Xs->size[0] * ((int32_T)numLGTerms - 1)];
    }
    loop_ub = XN->size[0] * XN->size[1];
    XN->size[0] = 1;
    XN->size[1] = Xs->size[0];
    emxEnsureCapacity_real_T(sp, XN, loop_ub, &nh_emlrtRTEI);
    XN_data = XN->data;
    for (ist = 0; ist < c_loop_ub; ist++) {
      XN_data[XN->size[0] * ist] = X_data[ist];
    }
    st.site = &rf_emlrtRSI;
    numLGTerms = hashpop(&st, N, N);
    if (numLGTerms != (int32_T)muDoubleScalarFloor(numLGTerms)) {
      emlrtIntegerCheckR2012b(numLGTerms, &qb_emlrtDCI, (emlrtConstCTX)sp);
    }
    if (((int32_T)numLGTerms < 1) || ((int32_T)numLGTerms > pi->size[2])) {
      emlrtDynamicBoundsCheckR2012b((int32_T)numLGTerms, 1, pi->size[2],
                                    &xq_emlrtBCI, (emlrtConstCTX)sp);
    }
    last_nnz = pi->size[0];
    loop_ub = Xs->size[0] * Xs->size[1];
    Xs->size[0] = pi->size[0];
    vectorUB = pi->size[1];
    Xs->size[1] = pi->size[1];
    emxEnsureCapacity_real_T(sp, Xs, loop_ub, &ph_emlrtRTEI);
    Xs_data = Xs->data;
    for (ist = 0; ist < vectorUB; ist++) {
      for (itilerow = 0; itilerow < last_nnz; itilerow++) {
        Xs_data[itilerow + Xs->size[0] * ist] =
            pi_data[(itilerow + pi->size[0] * ist) +
                    pi->size[0] * pi->size[1] * ((int32_T)numLGTerms - 1)];
      }
    }
    st.site = &qf_emlrtRSI;
    b_st.site = &hb_emlrtRSI;
    loop_ub = QN->size[0] * QN->size[1];
    QN->size[0] = M;
    QN->size[1] = c_loop_ub;
    emxEnsureCapacity_real_T(&st, QN, loop_ub, &ed_emlrtRTEI);
    XN_data = QN->data;
    b_st.site = &nf_emlrtRSI;
    if (X->size[0] > 2147483646) {
      c_st.site = &gb_emlrtRSI;
      check_forloop_overflow_error(&c_st);
    }
    for (ist = 0; ist < c_loop_ub; ist++) {
      ibmat = ist * M;
      b_st.site = &of_emlrtRSI;
      if (M > 2147483646) {
        c_st.site = &gb_emlrtRSI;
        check_forloop_overflow_error(&c_st);
      }
      for (itilerow = 0; itilerow < M; itilerow++) {
        b_st.site = &jb_emlrtRSI;
        XN_data[ibmat + itilerow] = X_data[ist];
      }
    }
    if ((WN->size[0] != M) && ((WN->size[0] != 1) && (M != 1))) {
      emlrtDimSizeImpxCheckR2021b(WN->size[0], M, &p_emlrtECI,
                                  (emlrtConstCTX)sp);
    }
    if ((WN->size[1] != c_loop_ub) &&
        ((WN->size[1] != 1) && (c_loop_ub != 1))) {
      emlrtDimSizeImpxCheckR2021b(WN->size[1], c_loop_ub, &q_emlrtECI,
                                  (emlrtConstCTX)sp);
    }
    if ((WN->size[0] == QN->size[0]) && (WN->size[1] == QN->size[1])) {
      ibmat = WN->size[0] * WN->size[1];
      loop_ub = QN->size[0] * QN->size[1];
      QN->size[0] = WN->size[0];
      QN->size[1] = WN->size[1];
      emxEnsureCapacity_real_T(sp, QN, loop_ub, &uh_emlrtRTEI);
      XN_data = QN->data;
      loop_ub = (ibmat / 2) << 1;
      vectorUB = loop_ub - 2;
      for (ist = 0; ist <= vectorUB; ist += 2) {
        __m128d r1;
        r = _mm_loadu_pd(&WN_data[ist]);
        r1 = _mm_loadu_pd(&XN_data[ist]);
        _mm_storeu_pd(&XN_data[ist], _mm_mul_pd(r, r1));
      }
      for (ist = loop_ub; ist < ibmat; ist++) {
        XN_data[ist] *= WN_data[ist];
      }
    } else {
      st.site = &qf_emlrtRSI;
      b_times(&st, QN, WN);
    }
    /* UN = repmat(XN,M,1) .* L; */
    if (pi->size[1] < 1) {
      emlrtDynamicBoundsCheckR2012b(1, 1, pi->size[1], &yq_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    emxInit_real_T(sp, &XN, 1, &vh_emlrtRTEI);
    loop_ub = XN->size[0];
    XN->size[0] = pi->size[0];
    emxEnsureCapacity_real_T(sp, XN, loop_ub, &vh_emlrtRTEI);
    lGN_data = XN->data;
    loop_ub = (Xs->size[0] / 2) << 1;
    vectorUB = loop_ub - 2;
    for (ist = 0; ist <= vectorUB; ist += 2) {
      r = _mm_loadu_pd(&Xs_data[ist]);
      _mm_storeu_pd(&lGN_data[ist], _mm_sub_pd(_mm_set1_pd(1.0), r));
    }
    for (ist = loop_ub; ist < last_nnz; ist++) {
      lGN_data[ist] = 1.0 - Xs_data[ist];
    }
    emxFree_real_T(sp, &Xs);
    loop_ub = UN->size[0] * UN->size[1];
    UN->size[0] = pi->size[0];
    emxFree_real_T(sp, &pi);
    UN->size[1] = 1;
    emxEnsureCapacity_real_T(sp, UN, loop_ub, &wh_emlrtRTEI);
    XN_data = UN->data;
    for (ist = 0; ist < last_nnz; ist++) {
      XN_data[ist] = lGN_data[ist];
    }
    emxFree_real_T(sp, &XN);
    st.site = &pf_emlrtRSI;
    b_st.site = &kb_emlrtRSI;
    c_st.site = &lb_emlrtRSI;
    if ((N->size[1] != 1) && (X->size[0] != 1) && (X->size[0] != N->size[1])) {
      emlrtErrorWithMessageIdR2018a(&c_st, &e_emlrtRTEI,
                                    "MATLAB:sizeDimensionsMustMatch",
                                    "MATLAB:sizeDimensionsMustMatch", 0);
    }
    if (X->size[0] == N->size[1]) {
      loop_ub = d_N->size[0] * d_N->size[1];
      d_N->size[0] = 1;
      d_N->size[1] = b_loop_ub;
      emxEnsureCapacity_real_T(&b_st, d_N, loop_ub, &xh_emlrtRTEI);
      c_N_data = d_N->data;
      loop_ub = scalarLB - 2;
      for (ist = 0; ist <= loop_ub; ist += 2) {
        r = _mm_loadu_pd(&X_data[ist]);
        _mm_storeu_pd(&c_N_data[ist],
                      _mm_div_pd(_mm_loadu_pd(&N_data[ist]), r));
      }
      for (ist = scalarLB; ist < b_loop_ub; ist++) {
        c_N_data[ist] = N_data[ist] / X_data[ist];
      }
    } else {
      c_st.site = &og_emlrtRSI;
      binary_expand_op_3(&c_st, d_N, N, X);
      c_N_data = d_N->data;
    }
    emxFree_real_T(&b_st, &X);
    ibmat = d_N->size[1];
    if ((d_N->size[1] != Z->size[1]) &&
        ((d_N->size[1] != 1) && (Z->size[1] != 1))) {
      emlrtDimSizeImpxCheckR2021b(d_N->size[1], Z->size[1], &r_emlrtECI,
                                  (emlrtConstCTX)sp);
    }
    if (d_N->size[1] == Z->size[1]) {
      loop_ub = CN->size[0] * CN->size[1];
      CN->size[0] = 1;
      CN->size[1] = d_N->size[1];
      emxEnsureCapacity_real_T(sp, CN, loop_ub, &yh_emlrtRTEI);
      XN_data = CN->data;
      loop_ub = (d_N->size[1] / 2) << 1;
      vectorUB = loop_ub - 2;
      for (ist = 0; ist <= vectorUB; ist += 2) {
        r = _mm_loadu_pd(&c_N_data[ist]);
        _mm_storeu_pd(&XN_data[ist], _mm_sub_pd(r, _mm_loadu_pd(&Z_data[ist])));
      }
      for (ist = loop_ub; ist < ibmat; ist++) {
        XN_data[ist] = c_N_data[ist] - Z_data[ist];
      }
    } else {
      st.site = &pf_emlrtRSI;
      binary_expand_op_2(&st, CN, d_N, Z);
    }
    emxFree_real_T(sp, &d_N);
    /*  cycle time exclusive of think time */
  }
  emxFree_boolean_T(sp, &b_N);
  emxFree_real_T(sp, &WN);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (pfqn_mvald.c) */
