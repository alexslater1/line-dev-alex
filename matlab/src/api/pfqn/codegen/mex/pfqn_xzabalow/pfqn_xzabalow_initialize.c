/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_xzabalow_initialize.c
 *
 * Code generation for function 'pfqn_xzabalow_initialize'
 *
 */

/* Include files */
#include "pfqn_xzabalow_initialize.h"
#include "_coder_pfqn_xzabalow_mex.h"
#include "pfqn_xzabalow_data.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void pfqn_xzabalow_once(void);

/* Function Definitions */
static void pfqn_xzabalow_once(void)
{
  mex_InitInfAndNan();
}

void pfqn_xzabalow_initialize(void)
{
  static const volatile char_T *emlrtBreakCheckR2012bFlagVar = NULL;
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
    pfqn_xzabalow_once();
  }
}

/* End of code generation (pfqn_xzabalow_initialize.c) */
