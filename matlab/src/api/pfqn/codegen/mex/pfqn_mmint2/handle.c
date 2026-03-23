/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * handle.c
 *
 * Code generation for function 'handle'
 *
 */

/* Include files */
#include "handle.h"
#include "pfqn_mmint2_internal_types.h"
#include "pfqn_mmint2_mexutil.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRSInfo nd_emlrtRSI = {
    24,                                            /* lineNo */
    "matlabCodegenHandle/matlabCodegenDestructor", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "matlabCodegenHandle.m" /* pathName */
};

static emlrtMCInfo g_emlrtMCI =
    {
        49,                    /* lineNo */
        17,                    /* colNo */
        "WarningState/delete", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "WarningState.m" /* pName */
};

static emlrtRSInfo pd_emlrtRSI =
    {
        49,                    /* lineNo */
        "WarningState/delete", /* fcnName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
        "WarningState.m" /* pathName */
};

/* Function Definitions */
void handle_matlabCodegenDestructor(const emlrtStack *sp,
                                    coder_internal_WarningState *obj)
{
  static const int32_T iv[2] = {1, 50};
  static const char_T u[50] = {
      'c', 'o', 'd', 'e', 'r', '.', 'i', 'n', 't', 'e', 'r', 'n', 'a',
      'l', '.', 'E', 'x', 't', 'r', 'i', 'n', 's', 'i', 'c', 'W', 'a',
      'r', 'n', 'i', 'n', 'g', 'M', 'a', 'n', 'a', 'g', 'e', 'r', '.',
      'd', 'e', 'l', 'e', 't', 'e', 'S', 't', 'a', 't', 'e'};
  emlrtStack b_st;
  emlrtStack st;
  const mxArray *b_y;
  const mxArray *m;
  const mxArray *y;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    st.site = &nd_emlrtRSI;
    if (obj->idx > 0) {
      y = NULL;
      m = emlrtCreateCharArray(2, &iv[0]);
      emlrtInitCharArrayR2013a(&st, 50, m, &u[0]);
      emlrtAssign(&y, m);
      b_y = NULL;
      m = emlrtCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
      *(int32_T *)emlrtMxGetData(m) = obj->idx;
      emlrtAssign(&b_y, m);
      b_st.site = &pd_emlrtRSI;
      b_feval(&b_st, y, b_y, &g_emlrtMCI);
    }
  }
}

/* End of code generation (handle.c) */
