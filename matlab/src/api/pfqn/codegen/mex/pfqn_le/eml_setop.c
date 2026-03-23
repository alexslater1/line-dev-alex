/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * eml_setop.c
 *
 * Code generation for function 'eml_setop'
 *
 */

/* Include files */
#include "eml_setop.h"
#include "pfqn_le_emxutil.h"
#include "pfqn_le_types.h"
#include "rt_nonfinite.h"
#include "mwmathutil.h"

/* Variable Definitions */
static emlrtRTEInfo g_emlrtRTEI =
    {
        447,          /* lineNo */
        5,            /* colNo */
        "do_vectors", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/private/"
        "eml_setop.m" /* pName */
};

static emlrtRTEInfo h_emlrtRTEI =
    {
        409,          /* lineNo */
        5,            /* colNo */
        "do_vectors", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/private/"
        "eml_setop.m" /* pName */
};

static emlrtRTEInfo i_emlrtRTEI =
    {
        242,          /* lineNo */
        13,           /* colNo */
        "do_vectors", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/private/"
        "eml_setop.m" /* pName */
};

static emlrtRTEInfo wb_emlrtRTEI =
    {
        211,         /* lineNo */
        24,          /* colNo */
        "eml_setop", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/private/"
        "eml_setop.m" /* pName */
};

static emlrtRTEInfo xb_emlrtRTEI =
    {
        218,         /* lineNo */
        29,          /* colNo */
        "eml_setop", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/private/"
        "eml_setop.m" /* pName */
};

static emlrtRTEInfo yb_emlrtRTEI =
    {
        415,         /* lineNo */
        9,           /* colNo */
        "eml_setop", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/private/"
        "eml_setop.m" /* pName */
};

static emlrtRTEInfo ac_emlrtRTEI =
    {
        449,         /* lineNo */
        9,           /* colNo */
        "eml_setop", /* fName */
        "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/private/"
        "eml_setop.m" /* pName */
};

/* Function Definitions */
int32_T do_vectors(const emlrtStack *sp, const emxArray_real_T *a, real_T b,
                   emxArray_real_T *c, emxArray_int32_T *ia)
{
  const real_T *a_data;
  real_T ak;
  real_T *c_data;
  int32_T b_ialast;
  int32_T iafirst;
  int32_T ialast;
  int32_T ib_size;
  int32_T iblast;
  int32_T na;
  int32_T nc;
  int32_T nia;
  int32_T *ia_data;
  boolean_T y;
  a_data = a->data;
  na = a->size[1];
  iblast = c->size[0] * c->size[1];
  c->size[0] = 1;
  c->size[1] = a->size[1];
  emxEnsureCapacity_real_T(sp, c, iblast, &wb_emlrtRTEI);
  c_data = c->data;
  iblast = ia->size[0];
  ia->size[0] = a->size[1];
  emxEnsureCapacity_int32_T(sp, ia, iblast, &xb_emlrtRTEI);
  ia_data = ia->data;
  ib_size = 0;
  y = true;
  if ((a->size[1] != 0) && (a->size[1] != 1)) {
    boolean_T exitg1;
    iblast = 0;
    exitg1 = false;
    while ((!exitg1) && (iblast <= a->size[1] - 2)) {
      ak = a_data[iblast + 1];
      if ((a_data[iblast] <= ak) || muDoubleScalarIsNaN(ak)) {
        iblast++;
      } else {
        y = false;
        exitg1 = true;
      }
    }
  }
  if (!y) {
    emlrtErrorWithMessageIdR2018a(sp, &i_emlrtRTEI,
                                  "Coder:toolbox:eml_setop_unsortedA",
                                  "Coder:toolbox:eml_setop_unsortedA", 0);
  }
  nc = 0;
  nia = 0;
  iafirst = 0;
  ialast = 0;
  iblast = 1;
  while ((ialast + 1 <= na) && (iblast <= 1)) {
    b_ialast = ialast + 1;
    ak = a_data[ialast];
    while ((b_ialast < a->size[1]) && (a_data[b_ialast] == ak)) {
      b_ialast++;
    }
    ialast = b_ialast - 1;
    if (ak == b) {
      ialast = b_ialast;
      iafirst = b_ialast;
      iblast = 2;
    } else {
      if (muDoubleScalarIsNaN(b)) {
        y = !muDoubleScalarIsNaN(ak);
      } else if (muDoubleScalarIsNaN(ak)) {
        y = false;
      } else {
        y = (ak < b);
      }
      if (y) {
        nc++;
        nia = nc;
        c_data[nc - 1] = ak;
        ia_data[nc - 1] = iafirst + 1;
        ialast = b_ialast;
        iafirst = b_ialast;
      } else {
        iblast = 2;
      }
    }
  }
  while (ialast + 1 <= na) {
    iblast = ialast + 1;
    while ((iblast < a->size[1]) && (a_data[iblast] == a_data[ialast])) {
      iblast++;
    }
    b_ialast = nc;
    nc++;
    nia = b_ialast + 1;
    c_data[b_ialast] = a_data[ialast];
    ia_data[b_ialast] = iafirst + 1;
    ialast = iblast;
    iafirst = iblast;
  }
  if (a->size[1] > 0) {
    if (nia > a->size[1]) {
      emlrtErrorWithMessageIdR2018a(sp, &h_emlrtRTEI,
                                    "Coder:builtins:AssertionFailed",
                                    "Coder:builtins:AssertionFailed", 0);
    }
    if (nia < 1) {
      nia = 0;
    }
    iblast = ia->size[0];
    ia->size[0] = nia;
    emxEnsureCapacity_int32_T(sp, ia, iblast, &yb_emlrtRTEI);
    if (nc > a->size[1]) {
      emlrtErrorWithMessageIdR2018a(sp, &g_emlrtRTEI,
                                    "Coder:builtins:AssertionFailed",
                                    "Coder:builtins:AssertionFailed", 0);
    }
    iblast = c->size[0] * c->size[1];
    c->size[1] = nia;
    emxEnsureCapacity_real_T(sp, c, iblast, &ac_emlrtRTEI);
  }
  return ib_size;
}

/* End of code generation (eml_setop.c) */
