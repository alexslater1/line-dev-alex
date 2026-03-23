/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * nchoosek.c
 *
 * Code generation for function 'nchoosek'
 *
 */

/* Include files */
#include "nchoosek.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_comom_data.h"
#include "pfqn_comom_mexutil.h"
#include "rt_nonfinite.h"
#include "warning.h"
#include "mwmathutil.h"

/* Variable Definitions */
static emlrtRSInfo yc_emlrtRSI = {
    56,         /* lineNo */
    "nchoosek", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/nchoosek.m" /* pathName
                                                                          */
};

static emlrtRSInfo ad_emlrtRSI = {
    141,   /* lineNo */
    "nCk", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/nchoosek.m" /* pathName
                                                                          */
};

static emlrtRSInfo bd_emlrtRSI = {
    142,   /* lineNo */
    "nCk", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/nchoosek.m" /* pathName
                                                                          */
};

static emlrtRSInfo cd_emlrtRSI = {
    129,   /* lineNo */
    "nCk", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/nchoosek.m" /* pathName
                                                                          */
};

static emlrtRSInfo dd_emlrtRSI = {
    153,        /* lineNo */
    "nCkInt64", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/nchoosek.m" /* pathName
                                                                          */
};

static emlrtRTEInfo r_emlrtRTEI = {
    17,         /* lineNo */
    23,         /* colNo */
    "nchoosek", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/nchoosek.m" /* pName
                                                                          */
};

static emlrtRTEInfo s_emlrtRTEI = {
    38,         /* lineNo */
    26,         /* colNo */
    "nchoosek", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/nchoosek.m" /* pName
                                                                          */
};

static emlrtRTEInfo t_emlrtRTEI = {
    45,         /* lineNo */
    4,          /* colNo */
    "nchoosek", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/nchoosek.m" /* pName
                                                                          */
};

static emlrtRTEInfo u_emlrtRTEI = {
    121,   /* lineNo */
    17,    /* colNo */
    "nCk", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/nchoosek.m" /* pName
                                                                          */
};

static emlrtRTEInfo le_emlrtRTEI = {
    155,        /* lineNo */
    13,         /* colNo */
    "nCkInt64", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/specfun/nchoosek.m" /* pName
                                                                          */
};

/* Function Declarations */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               char_T y[23]);

static void emlrt_marshallIn(const emlrtStack *sp,
                             const mxArray *a__output_of_sprintf_,
                             const char_T *identifier, char_T y[23]);

static void k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, char_T ret[23]);

/* Function Definitions */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId, char_T y[23])
{
  k_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static void emlrt_marshallIn(const emlrtStack *sp,
                             const mxArray *a__output_of_sprintf_,
                             const char_T *identifier, char_T y[23])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  b_emlrt_marshallIn(sp, emlrtAlias(a__output_of_sprintf_), &thisId, y);
  emlrtDestroyArray(&a__output_of_sprintf_);
}

static void k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, char_T ret[23])
{
  static const int32_T dims[2] = {1, 23};
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "char", false, 2U,
                          (const void *)&dims[0]);
  emlrtImportCharArrayR2015b((emlrtConstCTX)sp, src, &ret[0], 23);
  emlrtDestroyArray(&src);
}

real_T nchoosek(const emlrtStack *sp, real_T x, real_T k)
{
  static const int32_T iv[2] = {1, 7};
  static const int32_T iv1[2] = {1, 7};
  static const char_T rfmt[7] = {'%', '2', '3', '.', '1', '5', 'e'};
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack st;
  const mxArray *b_y;
  const mxArray *c_y;
  const mxArray *m;
  real_T r;
  real_T y;
  uint64_T u;
  int32_T j;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  if ((!(k >= 0.0)) || (!(k == muDoubleScalarFloor(k)))) {
    emlrtErrorWithMessageIdR2018a(sp, &r_emlrtRTEI,
                                  "MATLAB:nchoosek:InvalidArg2",
                                  "MATLAB:nchoosek:InvalidArg2", 0);
  }
  if (!(k <= x)) {
    emlrtErrorWithMessageIdR2018a(sp, &s_emlrtRTEI,
                                  "MATLAB:nchoosek:KOutOfRange",
                                  "MATLAB:nchoosek:KOutOfRange", 0);
  }
  r = k;
  if (k > x - k) {
    r = x - k;
  }
  if (x > 9.007199254740992E+15) {
    emlrtErrorWithMessageIdR2018a(sp, &t_emlrtRTEI,
                                  "MATLAB:nchoosek:NOutOfRange",
                                  "MATLAB:nchoosek:NOutOfRange", 0);
  }
  if (r == 0.0) {
    y = 1.0;
  } else if (r == 1.0) {
    y = x;
  } else {
    st.site = &yc_emlrtRSI;
    if (muDoubleScalarIsInf(r) || muDoubleScalarIsNaN(r)) {
      y = rtNaN;
    } else if (r > 1000.0) {
      y = rtInf;
    } else {
      real_T maxRelErr;
      int32_T i;
      maxRelErr = 0.0;
      y = x;
      i = (int32_T)(r - 1.0);
      emlrtForLoopVectorCheckR2021a(2.0, 1.0, r, mxDOUBLE_CLASS,
                                    (int32_T)(r - 1.0), &u_emlrtRTEI, &st);
      for (j = 0; j < i; j++) {
        y *= ((x - ((real_T)j + 2.0)) + 1.0) / ((real_T)j + 2.0);
        if (!(y < 1.125899906842624E+15)) {
          maxRelErr += 4.4408920985006262E-16;
        }
        y = muDoubleScalarRound(y);
      }
      if ((maxRelErr != 0.0) && (y <= 3.6893488147419103E+19)) {
        uint64_T b_i;
        uint64_T b_k;
        uint64_T n;
        uint64_T yint;
        boolean_T exitg1;
        b_st.site = &cd_emlrtRSI;
        n = (uint64_T)muDoubleScalarRound(x);
        r = muDoubleScalarRound(r);
        if (r < 1.8446744073709552E+19) {
          if (r >= 0.0) {
            b_k = (uint64_T)r;
          } else {
            b_k = 0UL;
          }
        } else {
          b_k = 0UL;
        }
        yint = 1UL;
        c_st.site = &dd_emlrtRSI;
        if (b_k > 18446744073709551614UL) {
          d_st.site = &cb_emlrtRSI;
          b_check_forloop_overflow_error(&d_st);
        }
        b_i = 1UL;
        exitg1 = false;
        while ((!exitg1) && (b_i <= b_k)) {
          uint64_T q_tmp;
          q_tmp = yint / b_i;
          if (n == 0UL) {
            emlrtDivisionByZeroErrorR2012b(&le_emlrtRTEI, &b_st);
          } else {
            u = MAX_uint64_T / n;
          }
          if (q_tmp >= u) {
            yint = MAX_uint64_T;
            exitg1 = true;
          } else {
            yint = q_tmp * n + (yint - q_tmp * b_i) * n / b_i;
            n--;
            b_i++;
          }
        }
        y = (real_T)yint;
        maxRelErr = 0.0;
      }
      if (y > 9.007199254740992E+15) {
        maxRelErr = muDoubleScalarMax(maxRelErr, 2.2204460492503131E-16);
      }
      if ((maxRelErr != 0.0) &&
          ((!muDoubleScalarIsInf(y)) && (!muDoubleScalarIsNaN(y))) &&
          (!emlrtSetWarningFlag(&st))) {
        char_T b_str[23];
        char_T str[23];
        b_st.site = &bd_emlrtRSI;
        b_y = NULL;
        m = emlrtCreateCharArray(2, &iv[0]);
        emlrtInitCharArrayR2013a(&b_st, 7, m, &rfmt[0]);
        emlrtAssign(&b_y, m);
        c_st.site = &qg_emlrtRSI;
        emlrt_marshallIn(
            &c_st,
            b_sprintf(&c_st, b_y, emlrt_marshallOut(maxRelErr), &emlrtMCI),
            "<output of sprintf>", str);
        b_st.site = &bd_emlrtRSI;
        c_y = NULL;
        m = emlrtCreateCharArray(2, &iv1[0]);
        emlrtInitCharArrayR2013a(&b_st, 7, m, &rfmt[0]);
        emlrtAssign(&c_y, m);
        c_st.site = &qg_emlrtRSI;
        emlrt_marshallIn(
            &c_st,
            b_sprintf(&c_st, c_y,
                      emlrt_marshallOut(muDoubleScalarCeil(maxRelErr * y)),
                      &emlrtMCI),
            "<output of sprintf>", b_str);
        b_st.site = &ad_emlrtRSI;
        warning(&b_st, str, b_str);
      }
    }
  }
  return y;
}

/* End of code generation (nchoosek.c) */
