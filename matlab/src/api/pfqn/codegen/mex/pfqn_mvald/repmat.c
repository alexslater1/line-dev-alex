/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * repmat.c
 *
 * Code generation for function 'repmat'
 *
 */

/* Include files */
#include "repmat.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_mvald_data.h"
#include "pfqn_mvald_emxutil.h"
#include "pfqn_mvald_types.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRSInfo xb_emlrtRSI = {
    34,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

static emlrtRSInfo yb_emlrtRSI = {
    80,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

static emlrtRSInfo ac_emlrtRSI = {
    83,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

static emlrtRSInfo bc_emlrtRSI = {
    85,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

static emlrtRTEInfo i_emlrtRTEI = {
    58,                   /* lineNo */
    23,                   /* colNo */
    "assertValidSizeArg", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "assertValidSizeArg.m" /* pName */
};

static emlrtRTEInfo wb_emlrtRTEI = {
    73,       /* lineNo */
    28,       /* colNo */
    "repmat", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pName */
};

/* Function Definitions */
void repmat(const emlrtStack *sp, const emxArray_real_T *a, real_T varargin_1,
            emxArray_real_T *b)
{
  emlrtStack b_st;
  emlrtStack st;
  const real_T *a_data;
  real_T *b_data;
  int32_T i;
  int32_T i1;
  int32_T ibmat;
  int32_T itilerow;
  int32_T jcol;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  a_data = a->data;
  st.site = &xb_emlrtRSI;
  if (varargin_1 != varargin_1) {
    emlrtErrorWithMessageIdR2018a(
        &st, &i_emlrtRTEI, "Coder:MATLAB:NonIntegerInput",
        "Coder:MATLAB:NonIntegerInput", 4, 12, MIN_int32_T, 12, MAX_int32_T);
  }
  i = (int32_T)varargin_1;
  ibmat = b->size[0] * b->size[1];
  b->size[0] = (int32_T)varargin_1;
  i1 = a->size[1];
  b->size[1] = a->size[1];
  emxEnsureCapacity_real_T(sp, b, ibmat, &wb_emlrtRTEI);
  b_data = b->data;
  st.site = &yb_emlrtRSI;
  if (a->size[1] > 2147483646) {
    b_st.site = &bb_emlrtRSI;
    check_forloop_overflow_error(&b_st);
  }
  for (jcol = 0; jcol < i1; jcol++) {
    ibmat = jcol * (int32_T)varargin_1;
    st.site = &ac_emlrtRSI;
    if ((int32_T)varargin_1 > 2147483646) {
      b_st.site = &bb_emlrtRSI;
      check_forloop_overflow_error(&b_st);
    }
    for (itilerow = 0; itilerow < i; itilerow++) {
      st.site = &bc_emlrtRSI;
      b_data[ibmat + itilerow] = a_data[jcol];
    }
  }
}

/* End of code generation (repmat.c) */
