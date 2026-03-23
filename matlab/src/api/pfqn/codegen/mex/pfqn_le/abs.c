/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * abs.c
 *
 * Code generation for function 'abs'
 *
 */

/* Include files */
#include "abs.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_le_data.h"
#include "pfqn_le_emxutil.h"
#include "pfqn_le_types.h"
#include "rt_nonfinite.h"
#include "mwmathutil.h"
#include "omp.h"

/* Variable Definitions */
static emlrtRSInfo jb_emlrtRSI = {
    19,                                                           /* lineNo */
    "abs",                                                        /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/abs.m" /* pathName */
};

static emlrtRSInfo kb_emlrtRSI = {
    82,                    /* lineNo */
    "applyScalarFunction", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunction.m" /* pathName */
};

static emlrtRTEInfo vb_emlrtRTEI = {
    30,                    /* lineNo */
    21,                    /* colNo */
    "applyScalarFunction", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyScalarFunction.m" /* pName */
};

/* Function Definitions */
void b_abs(const emlrtStack *sp, const emxArray_real_T *x, emxArray_real_T *y)
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  const real_T *x_data;
  real_T *y_data;
  int32_T b_abs_numThreads;
  int32_T i;
  int32_T k;
  int32_T nx;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  x_data = x->data;
  st.site = &jb_emlrtRSI;
  nx = x->size[0];
  i = y->size[0];
  y->size[0] = x->size[0];
  emxEnsureCapacity_real_T(&st, y, i, &vb_emlrtRTEI);
  y_data = y->data;
  b_st.site = &kb_emlrtRSI;
  if (x->size[0] > 2147483646) {
    c_st.site = &q_emlrtRSI;
    check_forloop_overflow_error(&c_st);
  }
  if (x->size[0] < 1600) {
    for (k = 0; k < nx; k++) {
      y_data[k] = muDoubleScalarAbs(x_data[k]);
    }
  } else {
    emlrtEnterParallelRegion(&st, omp_in_parallel());
    emlrtPushJmpBuf(&st, &emlrtJBStack);
    b_abs_numThreads = emlrtAllocRegionTLSs(
        st.tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(b_abs_numThreads)

    for (k = 0; k < nx; k++) {
      y_data[k] = muDoubleScalarAbs(x_data[k]);
    }
    emlrtPopJmpBuf(&st, &emlrtJBStack);
    emlrtExitParallelRegion(&st, omp_in_parallel());
  }
}

/* End of code generation (abs.c) */
