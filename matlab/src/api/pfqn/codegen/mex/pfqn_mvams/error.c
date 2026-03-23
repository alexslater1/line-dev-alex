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
#include "pfqn_mvams_data.h"
#include "pfqn_mvams_mexutil.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static const char_T cv[51] = {
    'A', 'r', 'r', 'i', 'v', 'a', 'l', ' ', 'r', 'a', 't', 'e', ' ',
    'c', 'a', 'n', 'n', 'o', 't', ' ', 'b', 'e', ' ', 's', 'p', 'e',
    'c', 'i', 'f', 'i', 'e', 'd', ' ', 'o', 'n', ' ', 'c', 'l', 'o',
    's', 'e', 'd', ' ', 'c', 'l', 'a', 's', 's', 'e', 's', '.'};

static const char_T cv1[12] = {'p', 'f', 'q', 'n', '_', 'm',
                               'v', 'a', 'l', 'd', 'm', 'x'};

/* Function Definitions */
void b_error(const emlrtStack *sp)
{
  static const int32_T iv[2] = {1, 6};
  static const int32_T iv1[2] = {1, 10};
  static const int32_T iv2[2] = {1, 51};
  static const char_T varargin_2[10] = {'p', 'f', 'q', 'n', '_',
                                        'm', 'v', 'a', 'm', 'x'};
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
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 10, m, &varargin_2[0]);
  emlrtAssign(&b_y, m);
  c_y = NULL;
  m = emlrtCreateCharArray(2, &iv2[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 51, m, &cv[0]);
  emlrtAssign(&c_y, m);
  st.site = &ng_emlrtRSI;
  f_error(&st, y, b_y, c_y, &emlrtMCI);
}

void c_error(const emlrtStack *sp)
{
  static const int32_T iv[2] = {1, 6};
  static const int32_T iv1[2] = {1, 12};
  static const int32_T iv2[2] = {1, 106};
  static const char_T varargin_3[106] = {
      'M', 'V', 'A', 'L', 'D', 'M', 'X', ' ', 'r', 'e', 'q', 'u', 'i', 'r',
      'e', 's', ' ', 't', 'o', ' ', 's', 'p', 'e', 'c', 'i', 'f', 'y', ' ',
      't', 'h', 'e', ' ', 'l', 'o', 'a', 'd', '-', 'd', 'e', 'p', 'e', 'n',
      'd', 'e', 'n', 't', ' ', 'r', 'a', 't', 'e', 's', ' ', 'w', 'i', 't',
      'h', ' ', 'o', 'n', 'e', ' ', 'j', 'o', 'b', ' ', 'm', 'o', 'r', 'e',
      ' ', 't', 'h', 'a', 'n', ' ', 't', 'h', 'e', ' ', 'm', 'a', 'x', 'i',
      'm', 'u', 'm', ' ', 'c', 'l', 'o', 's', 'e', 'd', ' ', 'p', 'o', 'p',
      'u', 'l', 'a', 't', 'i', 'o', 'n', '.'};
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
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 12, m, &cv1[0]);
  emlrtAssign(&b_y, m);
  c_y = NULL;
  m = emlrtCreateCharArray(2, &iv2[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 106, m, &varargin_3[0]);
  emlrtAssign(&c_y, m);
  st.site = &ng_emlrtRSI;
  f_error(&st, y, b_y, c_y, &emlrtMCI);
}

void d_error(const emlrtStack *sp)
{
  static const int32_T iv[2] = {1, 6};
  static const int32_T iv1[2] = {1, 12};
  static const int32_T iv2[2] = {1, 51};
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
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 12, m, &cv1[0]);
  emlrtAssign(&b_y, m);
  c_y = NULL;
  m = emlrtCreateCharArray(2, &iv2[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 51, m, &cv[0]);
  emlrtAssign(&c_y, m);
  st.site = &ng_emlrtRSI;
  f_error(&st, y, b_y, c_y, &emlrtMCI);
}

void e_error(const emlrtStack *sp)
{
  static const int32_T iv[2] = {1, 6};
  static const int32_T iv1[2] = {1, 10};
  static const int32_T iv2[2] = {1, 59};
  static const char_T varargin_3[59] = {
      'Q', 'u', 'e', 'u', 'e', ' ', 'r', 'e', 'p', 'l', 'i', 'c', 'a', 's', ' ',
      'n', 'o', 't', ' ', 'a', 'v', 'a', 'i', 'l', 'a', 'b', 'l', 'e', ' ', 'i',
      'n', ' ', 'e', 'x', 'a', 'c', 't', ' ', 'M', 'V', 'A', ' ', 'f', 'o', 'r',
      ' ', 'm', 'i', 'x', 'e', 'd', ' ', 'm', 'o', 'd', 'e', 'l', 's', '.'};
  static const char_T varargin_2[10] = {'p', 'f', 'q', 'n', '_',
                                        'm', 'v', 'a', 'm', 's'};
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
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 10, m, &varargin_2[0]);
  emlrtAssign(&b_y, m);
  c_y = NULL;
  m = emlrtCreateCharArray(2, &iv2[0]);
  emlrtInitCharArrayR2013a((emlrtConstCTX)sp, 59, m, &varargin_3[0]);
  emlrtAssign(&c_y, m);
  st.site = &ng_emlrtRSI;
  f_error(&st, y, b_y, c_y, &emlrtMCI);
}

/* End of code generation (error.c) */
