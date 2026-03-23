/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * eye.c
 *
 * Code generation for function 'eye'
 *
 */

/* Include files */
#include "eye.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_comomrm_data.h"
#include "pfqn_comomrm_emxutil.h"
#include "pfqn_comomrm_types.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRSInfo xe_emlrtRSI = {
    98,                                                           /* lineNo */
    "eye",                                                        /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/eye.m" /* pathName */
};

static emlrtRSInfo ye_emlrtRSI = {
    50,                                                           /* lineNo */
    "eye",                                                        /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/eye.m" /* pathName */
};

static emlrtRSInfo af_emlrtRSI = {
    21,                           /* lineNo */
    "checkAndSaturateExpandSize", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "checkAndSaturateExpandSize.m" /* pathName */
};

static emlrtRTEInfo lc_emlrtRTEI = {
    96,                                                           /* lineNo */
    5,                                                            /* colNo */
    "eye",                                                        /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/eye.m" /* pName */
};

/* Function Definitions */
void eye(const emlrtStack *sp, real_T varargin_1, emxArray_real_T *b_I)
{
  emlrtStack b_st;
  emlrtStack st;
  real_T *I_data;
  int32_T k;
  int32_T loop_ub;
  int32_T m;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  st.site = &ye_emlrtRSI;
  b_st.site = &af_emlrtRSI;
  if (varargin_1 != varargin_1) {
    emlrtErrorWithMessageIdR2018a(
        &b_st, &j_emlrtRTEI, "Coder:MATLAB:NonIntegerInput",
        "Coder:MATLAB:NonIntegerInput", 4, 12, MIN_int32_T, 12, MAX_int32_T);
  }
  m = (int32_T)varargin_1;
  loop_ub = b_I->size[0] * b_I->size[1];
  b_I->size[0] = (int32_T)varargin_1;
  b_I->size[1] = (int32_T)varargin_1;
  emxEnsureCapacity_real_T(sp, b_I, loop_ub, &lc_emlrtRTEI);
  I_data = b_I->data;
  loop_ub = (int32_T)varargin_1 * (int32_T)varargin_1;
  for (k = 0; k < loop_ub; k++) {
    I_data[k] = 0.0;
  }
  st.site = &xe_emlrtRSI;
  if ((int32_T)varargin_1 > 2147483646) {
    b_st.site = &tb_emlrtRSI;
    check_forloop_overflow_error(&b_st);
  }
  for (k = 0; k < m; k++) {
    I_data[k + b_I->size[0] * k] = 1.0;
  }
}

/* End of code generation (eye.c) */
