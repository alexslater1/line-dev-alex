/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * warning.c
 *
 * Code generation for function 'warning'
 *
 */

/* Include files */
#include "warning.h"
#include "pfqn_mmint2_mexutil.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtMCInfo c_emlrtMCI = {
    14,        /* lineNo */
    25,        /* colNo */
    "warning", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/shared/coder/coder/lib/+coder/+internal/"
    "warning.m" /* pName */
};

static emlrtMCInfo d_emlrtMCI = {
    14,        /* lineNo */
    9,         /* colNo */
    "warning", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/shared/coder/coder/lib/+coder/+internal/"
    "warning.m" /* pName */
};

static emlrtRSInfo qd_emlrtRSI = {
    14,        /* lineNo */
    "warning", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/shared/coder/coder/lib/+coder/+internal/"
    "warning.m" /* pathName */
};

/* Function Declarations */
static const mxArray *c_feval(const emlrtStack *sp, const mxArray *m,
                              const mxArray *m1, emlrtMCInfo *location);

static const mxArray *d_feval(const emlrtStack *sp, const mxArray *m,
                              const mxArray *m1, const mxArray *m2,
                              emlrtMCInfo *location);

/* Function Definitions */
static const mxArray *c_feval(const emlrtStack *sp, const mxArray *m,
                              const mxArray *m1, emlrtMCInfo *location)
{
  const mxArray *pArrays[2];
  const mxArray *m2;
  pArrays[0] = m;
  pArrays[1] = m1;
  return emlrtCallMATLABR2012b((emlrtConstCTX)sp, 1, &m2, 2, &pArrays[0],
                               "feval", true, location);
}

static const mxArray *d_feval(const emlrtStack *sp, const mxArray *m,
                              const mxArray *m1, const mxArray *m2,
                              emlrtMCInfo *location)
{
  const mxArray *pArrays[3];
  const mxArray *m3;
  pArrays[0] = m;
  pArrays[1] = m1;
  pArrays[2] = m2;
  return emlrtCallMATLABR2012b((emlrtConstCTX)sp, 1, &m3, 3, &pArrays[0],
                               "feval", true, location);
}

void b_warning(const emlrtStack *sp, const char_T varargin_1[14])
{
  static const int32_T iv[2] = {1, 7};
  static const int32_T iv1[2] = {1, 7};
  static const int32_T iv2[2] = {1, 27};
  static const int32_T iv3[2] = {1, 14};
  static const char_T msgID[27] = {'M', 'A', 'T', 'L', 'A', 'B', ':', 'i', 'n',
                                   't', 'e', 'g', 'r', 'a', 'l', ':', 'M', 'i',
                                   'n', 'S', 't', 'e', 'p', 'S', 'i', 'z', 'e'};
  static const char_T b_u[7] = {'m', 'e', 's', 's', 'a', 'g', 'e'};
  static const char_T u[7] = {'w', 'a', 'r', 'n', 'i', 'n', 'g'};
  emlrtStack st;
  const mxArray *b_y;
  const mxArray *c_y;
  const mxArray *d_y;
  const mxArray *m;
  const mxArray *y;
  st.prev = sp;
  st.tls = sp->tls;
  y = NULL;
  m = emlrtCreateCharArray(2, &iv[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 7, m, &u[0]);
  emlrtAssign(&y, m);
  b_y = NULL;
  m = emlrtCreateCharArray(2, &iv1[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 7, m, &b_u[0]);
  emlrtAssign(&b_y, m);
  c_y = NULL;
  m = emlrtCreateCharArray(2, &iv2[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 27, m, &msgID[0]);
  emlrtAssign(&c_y, m);
  d_y = NULL;
  m = emlrtCreateCharArray(2, &iv3[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 14, m, &varargin_1[0]);
  emlrtAssign(&d_y, m);
  st.site = &qd_emlrtRSI;
  b_feval(&st, y, d_feval(&st, b_y, c_y, d_y, &c_emlrtMCI), &d_emlrtMCI);
}

void c_warning(const emlrtStack *sp, const char_T varargin_1[9])
{
  static const int32_T iv[2] = {1, 7};
  static const int32_T iv1[2] = {1, 7};
  static const int32_T iv2[2] = {1, 39};
  static const int32_T iv3[2] = {1, 9};
  static const char_T msgID[39] = {
      'M', 'A', 'T', 'L', 'A', 'B', ':', 'i', 'n', 't', 'e', 'g', 'r',
      'a', 'l', ':', 'M', 'a', 'x', 'I', 'n', 't', 'e', 'r', 'v', 'a',
      'l', 'C', 'o', 'u', 'n', 't', 'R', 'e', 'a', 'c', 'h', 'e', 'd'};
  static const char_T b_u[7] = {'m', 'e', 's', 's', 'a', 'g', 'e'};
  static const char_T u[7] = {'w', 'a', 'r', 'n', 'i', 'n', 'g'};
  emlrtStack st;
  const mxArray *b_y;
  const mxArray *c_y;
  const mxArray *d_y;
  const mxArray *m;
  const mxArray *y;
  st.prev = sp;
  st.tls = sp->tls;
  y = NULL;
  m = emlrtCreateCharArray(2, &iv[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 7, m, &u[0]);
  emlrtAssign(&y, m);
  b_y = NULL;
  m = emlrtCreateCharArray(2, &iv1[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 7, m, &b_u[0]);
  emlrtAssign(&b_y, m);
  c_y = NULL;
  m = emlrtCreateCharArray(2, &iv2[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 39, m, &msgID[0]);
  emlrtAssign(&c_y, m);
  d_y = NULL;
  m = emlrtCreateCharArray(2, &iv3[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 9, m, &varargin_1[0]);
  emlrtAssign(&d_y, m);
  st.site = &qd_emlrtRSI;
  b_feval(&st, y, d_feval(&st, b_y, c_y, d_y, &c_emlrtMCI), &d_emlrtMCI);
}

void warning(const emlrtStack *sp)
{
  static const int32_T iv[2] = {1, 7};
  static const int32_T iv1[2] = {1, 7};
  static const int32_T iv2[2] = {1, 30};
  static const char_T msgID[30] = {'M', 'A', 'T', 'L', 'A', 'B', ':', 'i',
                                   'n', 't', 'e', 'g', 'r', 'a', 'l', ':',
                                   'N', 'o', 'n', 'F', 'i', 'n', 'i', 't',
                                   'e', 'V', 'a', 'l', 'u', 'e'};
  static const char_T b_u[7] = {'m', 'e', 's', 's', 'a', 'g', 'e'};
  static const char_T u[7] = {'w', 'a', 'r', 'n', 'i', 'n', 'g'};
  emlrtStack st;
  const mxArray *b_y;
  const mxArray *c_y;
  const mxArray *m;
  const mxArray *y;
  st.prev = sp;
  st.tls = sp->tls;
  y = NULL;
  m = emlrtCreateCharArray(2, &iv[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 7, m, &u[0]);
  emlrtAssign(&y, m);
  b_y = NULL;
  m = emlrtCreateCharArray(2, &iv1[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 7, m, &b_u[0]);
  emlrtAssign(&b_y, m);
  c_y = NULL;
  m = emlrtCreateCharArray(2, &iv2[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 30, m, &msgID[0]);
  emlrtAssign(&c_y, m);
  st.site = &qd_emlrtRSI;
  b_feval(&st, y, c_feval(&st, b_y, c_y, &c_emlrtMCI), &d_emlrtMCI);
}

/* End of code generation (warning.c) */
