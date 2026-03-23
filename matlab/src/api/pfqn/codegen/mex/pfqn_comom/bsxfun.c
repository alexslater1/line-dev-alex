/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * bsxfun.c
 *
 * Code generation for function 'bsxfun'
 *
 */

/* Include files */
#include "bsxfun.h"
#include "pfqn_comom_emxutil.h"
#include "pfqn_comom_types.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRTEInfo n_emlrtRTEI = {
    53,       /* lineNo */
    15,       /* colNo */
    "bsxfun", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/bsxfun.m" /* pName */
};

static emlrtRTEInfo vc_emlrtRTEI = {
    60,       /* lineNo */
    20,       /* colNo */
    "bsxfun", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elmat/bsxfun.m" /* pName */
};

/* Function Definitions */
void bsxfun(const emlrtStack *sp, const emxArray_real_T *a,
            const emxArray_real_T *b, emxArray_boolean_T *c)
{
  const real_T *a_data;
  const real_T *b_data;
  int32_T acoef;
  int32_T b_k;
  int32_T csz_idx_1;
  int32_T i;
  int32_T k;
  boolean_T *c_data;
  b_data = b->data;
  a_data = a->data;
  if (b->size[1] == 1) {
    csz_idx_1 = a->size[1];
  } else if (a->size[1] == 1) {
    csz_idx_1 = b->size[1];
  } else if (a->size[1] == b->size[1]) {
    csz_idx_1 = a->size[1];
  } else {
    emlrtErrorWithMessageIdR2018a(sp, &n_emlrtRTEI,
                                  "MATLAB:bsxfun:arrayDimensionsMustMatch",
                                  "MATLAB:bsxfun:arrayDimensionsMustMatch", 0);
  }
  i = a->size[0];
  acoef = c->size[0] * c->size[1];
  c->size[0] = a->size[0];
  c->size[1] = csz_idx_1;
  emxEnsureCapacity_boolean_T(sp, c, acoef, &vc_emlrtRTEI);
  c_data = c->data;
  if ((a->size[0] != 0) && (csz_idx_1 != 0)) {
    int32_T b_acoef;
    int32_T bcoef;
    acoef = (a->size[1] != 1);
    bcoef = (b->size[1] != 1);
    b_acoef = (a->size[0] != 1);
    for (k = 0; k < csz_idx_1; k++) {
      int32_T varargin_2;
      int32_T varargin_3;
      varargin_2 = acoef * k;
      varargin_3 = bcoef * k;
      for (b_k = 0; b_k < i; b_k++) {
        c_data[b_k + c->size[0] * k] =
            (a_data[b_acoef * b_k + a->size[0] * varargin_2] ==
             b_data[varargin_3]);
      }
    }
  }
}

/* End of code generation (bsxfun.c) */
