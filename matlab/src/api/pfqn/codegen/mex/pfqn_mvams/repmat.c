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
#include "pfqn_mvams_data.h"
#include "pfqn_mvams_emxutil.h"
#include "pfqn_mvams_types.h"
#include "rt_nonfinite.h"
#include "mwmathutil.h"

/* Variable Definitions */
static emlrtRSInfo ib_emlrtRSI = {
    78,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

static emlrtRTEInfo d_emlrtRTEI = {
    58,                   /* lineNo */
    23,                   /* colNo */
    "assertValidSizeArg", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "assertValidSizeArg.m" /* pName */
};

/* Function Definitions */
void b_repmat(const emlrtStack *sp, const emxArray_real_T *a, real_T varargin_1,
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
  st.site = &hb_emlrtRSI;
  if ((varargin_1 != varargin_1) || muDoubleScalarIsInf(varargin_1) ||
      (varargin_1 > 2.147483647E+9)) {
    emlrtErrorWithMessageIdR2018a(
        &st, &d_emlrtRTEI, "Coder:MATLAB:NonIntegerInput",
        "Coder:MATLAB:NonIntegerInput", 4, 12, MIN_int32_T, 12, MAX_int32_T);
  }
  i = (int32_T)varargin_1;
  ibmat = b->size[0] * b->size[1];
  b->size[0] = (int32_T)varargin_1;
  i1 = a->size[1];
  b->size[1] = a->size[1];
  emxEnsureCapacity_real_T(sp, b, ibmat, &ed_emlrtRTEI);
  b_data = b->data;
  st.site = &nf_emlrtRSI;
  if (a->size[1] > 2147483646) {
    b_st.site = &gb_emlrtRSI;
    check_forloop_overflow_error(&b_st);
  }
  for (jcol = 0; jcol < i1; jcol++) {
    ibmat = jcol * (int32_T)varargin_1;
    st.site = &of_emlrtRSI;
    if ((int32_T)varargin_1 > 2147483646) {
      b_st.site = &gb_emlrtRSI;
      check_forloop_overflow_error(&b_st);
    }
    for (itilerow = 0; itilerow < i; itilerow++) {
      b_data[ibmat + itilerow] = a_data[jcol];
    }
  }
}

void repmat(const emlrtStack *sp, const emxArray_real_T *a, real_T varargin_2,
            emxArray_real_T *b)
{
  emlrtStack b_st;
  emlrtStack st;
  const real_T *a_data;
  real_T *b_data;
  int32_T i;
  int32_T i1;
  int32_T ibtile;
  int32_T jtilecol;
  int32_T k;
  boolean_T overflow;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  a_data = a->data;
  st.site = &hb_emlrtRSI;
  if ((varargin_2 != varargin_2) || muDoubleScalarIsInf(varargin_2) ||
      (varargin_2 > 2.147483647E+9)) {
    emlrtErrorWithMessageIdR2018a(
        &st, &d_emlrtRTEI, "Coder:MATLAB:NonIntegerInput",
        "Coder:MATLAB:NonIntegerInput", 4, 12, MIN_int32_T, 12, MAX_int32_T);
  }
  i = a->size[0];
  ibtile = b->size[0] * b->size[1];
  b->size[0] = a->size[0];
  i1 = (int32_T)varargin_2;
  b->size[1] = (int32_T)varargin_2;
  emxEnsureCapacity_real_T(sp, b, ibtile, &ed_emlrtRTEI);
  b_data = b->data;
  st.site = &ib_emlrtRSI;
  if ((int32_T)varargin_2 > 2147483646) {
    b_st.site = &gb_emlrtRSI;
    check_forloop_overflow_error(&b_st);
  }
  overflow = (a->size[0] > 2147483646);
  for (jtilecol = 0; jtilecol < i1; jtilecol++) {
    ibtile = jtilecol * i;
    st.site = &jb_emlrtRSI;
    if (overflow) {
      b_st.site = &gb_emlrtRSI;
      check_forloop_overflow_error(&b_st);
    }
    for (k = 0; k < i; k++) {
      b_data[ibtile + k] = a_data[k];
    }
  }
}

/* End of code generation (repmat.c) */
