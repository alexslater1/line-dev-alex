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
#include "pfqn_comom_data.h"
#include "pfqn_comom_emxutil.h"
#include "pfqn_comom_types.h"
#include "rt_nonfinite.h"
#include "mwmathutil.h"

/* Variable Definitions */
static emlrtRSInfo db_emlrtRSI = {
    34,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

static emlrtRSInfo eb_emlrtRSI = {
    80,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

static emlrtRSInfo fb_emlrtRSI = {
    83,       /* lineNo */
    "repmat", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m" /* pathName
                                                                      */
};

static emlrtRTEInfo h_emlrtRTEI = {
    64,                   /* lineNo */
    15,                   /* colNo */
    "assertValidSizeArg", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "assertValidSizeArg.m" /* pName */
};

static emlrtRTEInfo i_emlrtRTEI = {
    58,                   /* lineNo */
    23,                   /* colNo */
    "assertValidSizeArg", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "assertValidSizeArg.m" /* pName */
};

static emlrtDCInfo k_emlrtDCI = {
    37,       /* lineNo */
    14,       /* colNo */
    "repmat", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/repmat.m", /* pName
                                                                       */
    4 /* checkKind */
};

static emlrtRTEInfo jc_emlrtRTEI = {
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
  real_T d;
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
  st.site = &db_emlrtRSI;
  if ((varargin_1 != muDoubleScalarFloor(varargin_1)) ||
      muDoubleScalarIsInf(varargin_1) || (varargin_1 < -2.147483648E+9) ||
      (varargin_1 > 2.147483647E+9)) {
    emlrtErrorWithMessageIdR2018a(
        &st, &i_emlrtRTEI, "Coder:MATLAB:NonIntegerInput",
        "Coder:MATLAB:NonIntegerInput", 4, 12, MIN_int32_T, 12, MAX_int32_T);
  }
  if (varargin_1 <= 0.0) {
    d = 0.0;
  } else {
    d = varargin_1;
  }
  if (!(d <= 2.147483647E+9)) {
    emlrtErrorWithMessageIdR2018a(&st, &h_emlrtRTEI, "Coder:MATLAB:pmaxsize",
                                  "Coder:MATLAB:pmaxsize", 0);
  }
  if (!(varargin_1 >= 0.0)) {
    emlrtNonNegativeCheckR2012b(varargin_1, &k_emlrtDCI, (emlrtConstCTX)sp);
  }
  i = (int32_T)varargin_1;
  ibmat = b->size[0] * b->size[1];
  b->size[0] = (int32_T)varargin_1;
  i1 = a->size[1];
  b->size[1] = a->size[1];
  emxEnsureCapacity_real_T(sp, b, ibmat, &jc_emlrtRTEI);
  b_data = b->data;
  st.site = &eb_emlrtRSI;
  if (a->size[1] > 2147483646) {
    b_st.site = &cb_emlrtRSI;
    check_forloop_overflow_error(&b_st);
  }
  for (jcol = 0; jcol < i1; jcol++) {
    ibmat = jcol * (int32_T)varargin_1;
    st.site = &fb_emlrtRSI;
    if ((int32_T)varargin_1 > 2147483646) {
      b_st.site = &cb_emlrtRSI;
      check_forloop_overflow_error(&b_st);
    }
    for (itilerow = 0; itilerow < i; itilerow++) {
      b_data[ibmat + itilerow] = a_data[jcol];
    }
  }
}

/* End of code generation (repmat.c) */
