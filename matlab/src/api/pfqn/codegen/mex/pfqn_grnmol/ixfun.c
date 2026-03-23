/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * ixfun.c
 *
 * Code generation for function 'ixfun'
 *
 */

/* Include files */
#include "ixfun.h"
#include "pfqn_grnmol_emxutil.h"
#include "pfqn_grnmol_types.h"
#include "rt_nonfinite.h"
#include "mwmathutil.h"
#include "omp.h"

/* Variable Definitions */
static emlrtRTEInfo i_emlrtRTEI = {
    233,                            /* lineNo */
    23,                             /* colNo */
    "expand_fltpower_domain_error", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/ixfun.m" /* pName
                                                                         */
};

static emlrtRTEInfo j_emlrtRTEI = {
    233,            /* lineNo */
    23,             /* colNo */
    "expand_power", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/ixfun.m" /* pName
                                                                         */
};

static emlrtRTEInfo nb_emlrtRTEI = {
    242,     /* lineNo */
    20,      /* colNo */
    "ixfun", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/ixfun.m" /* pName
                                                                         */
};

/* Function Definitions */
void expand_fltpower_domain_error(const emlrtStack *sp,
                                  const emxArray_real_T *a,
                                  const emxArray_real_T *b,
                                  emxArray_boolean_T *c)
{
  jmp_buf *volatile emlrtJBStack;
  const real_T *a_data;
  const real_T *b_data;
  real_T c_b;
  int32_T expand_fltpower_domain_error_numThreads;
  int32_T k;
  int32_T sak;
  int32_T sbk;
  boolean_T *c_data;
  b_data = b->data;
  a_data = a->data;
  sak = a->size[1];
  sbk = b->size[1];
  if (b->size[1] == 1) {
    sbk = a->size[1];
  } else if (a->size[1] == 1) {
    sbk = b->size[1];
  } else {
    sbk = muIntScalarMin_sint32(sak, sbk);
    if (a->size[1] != b->size[1]) {
      emlrtErrorWithMessageIdR2018a(sp, &i_emlrtRTEI,
                                    "MATLAB:sizeDimensionsMustMatch",
                                    "MATLAB:sizeDimensionsMustMatch", 0);
    }
  }
  sak = c->size[0] * c->size[1];
  c->size[0] = 1;
  c->size[1] = sbk;
  emxEnsureCapacity_boolean_T(sp, c, sak, &nb_emlrtRTEI);
  c_data = c->data;
  if (sbk != 0) {
    boolean_T b1;
    boolean_T b_b;
    b_b = (a->size[1] != 1);
    b1 = (b->size[1] != 1);
    if (sbk < 1600) {
      for (k = 0; k < sbk; k++) {
        c_b = b_data[b1 * k];
        if ((a_data[b_b * k] < 0.0) && (!muDoubleScalarIsNaN(c_b)) &&
            (muDoubleScalarFloor(c_b) != c_b)) {
          c_data[k] = true;
        } else {
          c_data[k] = false;
        }
      }
    } else {
      emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
      emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
      expand_fltpower_domain_error_numThreads =
          emlrtAllocRegionTLSs(sp->tls, omp_in_parallel(),
                               omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(                                          \
        expand_fltpower_domain_error_numThreads) private(c_b)

      for (k = 0; k < sbk; k++) {
        c_b = b_data[b1 * k];
        if ((a_data[b_b * k] < 0.0) && (!muDoubleScalarIsNaN(c_b)) &&
            (muDoubleScalarFloor(c_b) != c_b)) {
          c_data[k] = true;
        } else {
          c_data[k] = false;
        }
      }
      emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
      emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
    }
  }
}

void expand_power(const emlrtStack *sp, const emxArray_real_T *a,
                  const emxArray_real_T *b, emxArray_real_T *c)
{
  jmp_buf *volatile emlrtJBStack;
  const real_T *a_data;
  const real_T *b_data;
  real_T *c_data;
  int32_T expand_power_numThreads;
  int32_T k;
  int32_T sak;
  int32_T sbk;
  b_data = b->data;
  a_data = a->data;
  sak = a->size[1];
  sbk = b->size[1];
  if (b->size[1] == 1) {
    sbk = a->size[1];
  } else if (a->size[1] == 1) {
    sbk = b->size[1];
  } else {
    sbk = muIntScalarMin_sint32(sak, sbk);
    if (a->size[1] != b->size[1]) {
      emlrtErrorWithMessageIdR2018a(sp, &j_emlrtRTEI,
                                    "MATLAB:sizeDimensionsMustMatch",
                                    "MATLAB:sizeDimensionsMustMatch", 0);
    }
  }
  sak = c->size[0] * c->size[1];
  c->size[0] = 1;
  c->size[1] = sbk;
  emxEnsureCapacity_real_T(sp, c, sak, &nb_emlrtRTEI);
  c_data = c->data;
  if (sbk != 0) {
    boolean_T b1;
    boolean_T b_b;
    b_b = (a->size[1] != 1);
    b1 = (b->size[1] != 1);
    if (sbk < 1600) {
      for (k = 0; k < sbk; k++) {
        c_data[k] = muDoubleScalarPower(a_data[b_b * k], b_data[b1 * k]);
      }
    } else {
      emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
      emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
      expand_power_numThreads =
          emlrtAllocRegionTLSs(sp->tls, omp_in_parallel(),
                               omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(expand_power_numThreads)

      for (k = 0; k < sbk; k++) {
        c_data[k] = muDoubleScalarPower(a_data[b_b * k], b_data[b1 * k]);
      }
      emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
      emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
    }
  }
}

/* End of code generation (ixfun.c) */
