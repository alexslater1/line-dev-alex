/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_comom_initialize.c
 *
 * Code generation for function 'pfqn_comom_initialize'
 *
 */

/* Include files */
#include "pfqn_comom_initialize.h"
#include "_coder_pfqn_comom_mex.h"
#include "pfqn_comom_data.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void pfqn_comom_once(void);

/* Function Definitions */
static void pfqn_comom_once(void)
{
  mex_InitInfAndNan();
}

void pfqn_comom_initialize(void)
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
    pfqn_comom_once();
  }
}

/* End of code generation (pfqn_comom_initialize.c) */
