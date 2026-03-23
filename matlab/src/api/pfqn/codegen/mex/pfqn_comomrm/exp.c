/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * exp.c
 *
 * Code generation for function 'exp'
 *
 */

/* Include files */
#include "exp.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_comomrm_data.h"
#include "pfqn_comomrm_types.h"
#include "rt_nonfinite.h"
#include "mwmathutil.h"
#include "omp.h"

/* Variable Definitions */
static emlrtRSInfo we_emlrtRSI = {
    10,                                                           /* lineNo */
    "exp",                                                        /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/elfun/exp.m" /* pathName */
};

/* Function Definitions */
void b_exp(const emlrtStack *sp, emxArray_real_T *x)
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack st;
  real_T *x_data;
  int32_T b_exp_numThreads;
  int32_T k;
  int32_T nx;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  x_data = x->data;
  st.site = &we_emlrtRSI;
  nx = x->size[0];
  b_st.site = &vc_emlrtRSI;
  if (x->size[0] > 2147483646) {
    c_st.site = &tb_emlrtRSI;
    check_forloop_overflow_error(&c_st);
  }
  if (nx < 1600) {
    for (k = 0; k < nx; k++) {
      x_data[k] = muDoubleScalarExp(x_data[k]);
    }
  } else {
    emlrtEnterParallelRegion(&st, omp_in_parallel());
    emlrtPushJmpBuf(&st, &emlrtJBStack);
    b_exp_numThreads = emlrtAllocRegionTLSs(
        st.tls, omp_in_parallel(), omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(b_exp_numThreads)

    for (k = 0; k < nx; k++) {
      x_data[k] = muDoubleScalarExp(x_data[k]);
    }
    emlrtPopJmpBuf(&st, &emlrtJBStack);
    emlrtExitParallelRegion(&st, omp_in_parallel());
  }
}

/* End of code generation (exp.c) */
