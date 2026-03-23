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
#include "pfqn_mvams_emxutil.h"
#include "pfqn_mvams_types.h"
#include "rt_nonfinite.h"
#include "mwmathutil.h"
#include "omp.h"

/* Variable Definitions */
static emlrtRTEInfo c_emlrtRTEI = {
    233,          /* lineNo */
    23,           /* colNo */
    "expand_min", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/ixfun.m" /* pName
                                                                         */
};

static emlrtRTEInfo yc_emlrtRTEI = {
    242,     /* lineNo */
    20,      /* colNo */
    "ixfun", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/ixfun.m" /* pName
                                                                         */
};

/* Function Definitions */
void expand_min(const emlrtStack *sp, const emxArray_real_T *a,
                const emxArray_real_T *b, emxArray_real_T *c)
{
  jmp_buf *volatile emlrtJBStack;
  const real_T *a_data;
  const real_T *b_data;
  real_T *c_data;
  int32_T expand_min_numThreads;
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
      emlrtErrorWithMessageIdR2018a(sp, &c_emlrtRTEI,
                                    "MATLAB:sizeDimensionsMustMatch",
                                    "MATLAB:sizeDimensionsMustMatch", 0);
    }
  }
  sak = c->size[0] * c->size[1];
  c->size[0] = 1;
  c->size[1] = sbk;
  emxEnsureCapacity_real_T(sp, c, sak, &yc_emlrtRTEI);
  c_data = c->data;
  if (sbk != 0) {
    boolean_T b1;
    boolean_T b_b;
    b_b = (a->size[1] != 1);
    b1 = (b->size[1] != 1);
    if (sbk < 1600) {
      for (k = 0; k < sbk; k++) {
        c_data[k] = muDoubleScalarMin(a_data[b_b * k], b_data[b1 * k]);
      }
    } else {
      emlrtEnterParallelRegion((emlrtCTX)sp, omp_in_parallel());
      emlrtPushJmpBuf((emlrtCTX)sp, &emlrtJBStack);
      expand_min_numThreads =
          emlrtAllocRegionTLSs(sp->tls, omp_in_parallel(),
                               omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(expand_min_numThreads)

      for (k = 0; k < sbk; k++) {
        c_data[k] = muDoubleScalarMin(a_data[b_b * k], b_data[b1 * k]);
      }
      emlrtPopJmpBuf((emlrtCTX)sp, &emlrtJBStack);
      emlrtExitParallelRegion((emlrtCTX)sp, omp_in_parallel());
    }
  }
}

/* End of code generation (ixfun.c) */
