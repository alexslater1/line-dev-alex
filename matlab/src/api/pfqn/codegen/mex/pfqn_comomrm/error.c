/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * error.c
 *
 * Code generation for function 'error'
 *
 */

/* Include files */
#include "error.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtMCInfo emlrtMCI = {
    27,                                                            /* lineNo */
    5,                                                             /* colNo */
    "error",                                                       /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/lang/error.m" /* pName */
};

static emlrtRSInfo qf_emlrtRSI = {
    27,                                                            /* lineNo */
    "error",                                                       /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/lang/error.m" /* pathName
                                                                    */
};

/* Function Declarations */
static void c_error(const emlrtStack *sp, const mxArray *m, const mxArray *m1,
                    const mxArray *m2, emlrtMCInfo *location);

/* Function Definitions */
static void c_error(const emlrtStack *sp, const mxArray *m, const mxArray *m1,
                    const mxArray *m2, emlrtMCInfo *location)
{
  const mxArray *pArrays[3];
  pArrays[0] = m;
  pArrays[1] = m1;
  pArrays[2] = m2;
  emlrtCallMATLABR2012b((emlrtConstCTX)sp, 0, NULL, 3, &pArrays[0], "error",
                        true, location);
}

void b_error(const emlrtStack *sp)
{
  static const int32_T iv[2] = {1, 6};
  static const int32_T iv1[2] = {1, 12};
  static const int32_T iv2[2] = {1, 53};
  static const char_T varargin_3[53] = {
      'T', 'h', 'e', ' ', 's', 'o', 'l', 'v', 'e', 'r', ' ', 'a', 'c', 'c',
      'e', 'p', 't', 's', ' ', 'a', 't', ' ', 'm', 'o', 's', 't', ' ', 'a',
      ' ', 's', 'i', 'n', 'g', 'l', 'e', ' ', 'q', 'u', 'e', 'u', 'e', 'i',
      'n', 'g', ' ', 's', 't', 'a', 't', 'i', 'o', 'n', '.'};
  static const char_T varargin_2[12] = {'p', 'f', 'q', 'n', '_', 'c',
                                        'o', 'm', 'o', 'm', 'r', 'm'};
  static const char_T varargin_1[6] = {'%', 's', ':', ' ', '%', 's'};
  emlrtStack st;
  const mxArray *b_y;
  const mxArray *c_y;
  const mxArray *m;
  const mxArray *y;
  st.prev = sp;
  st.tls = sp->tls;
  y = NULL;
  m = emlrtCreateCharArray(2, &iv[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 6, m, &varargin_1[0]);
  emlrtAssign(&y, m);
  b_y = NULL;
  m = emlrtCreateCharArray(2, &iv1[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 12, m, &varargin_2[0]);
  emlrtAssign(&b_y, m);
  c_y = NULL;
  m = emlrtCreateCharArray(2, &iv2[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 53, m, &varargin_3[0]);
  emlrtAssign(&c_y, m);
  st.site = &qf_emlrtRSI;
  c_error(&st, y, b_y, c_y, &emlrtMCI);
}

/* End of code generation (error.c) */
