/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_sqni_initialize.c
 *
 * Code generation for function 'pfqn_sqni_initialize'
 *
 */

/* Include files */
#include "pfqn_sqni_initialize.h"
#include "_coder_pfqn_sqni_mex.h"
#include "pfqn_sqni_data.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void pfqn_sqni_once(void);

/* Function Definitions */
static void pfqn_sqni_once(void)
{
  mex_InitInfAndNan();
}

void pfqn_sqni_initialize(void)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtBreakCheckR2012bFlagVar = emlrtGetBreakCheckFlagAddressR2022b(&st);
  emlrtClearAllocCountR2012b(&st, false, 0U, NULL);
  emlrtEnterRtStackR2012b(&st);
  if (emlrtFirstTimeR2012b(emlrtRootTLSGlobal)) {
    pfqn_sqni_once();
  }
}

/* End of code generation (pfqn_sqni_initialize.c) */
