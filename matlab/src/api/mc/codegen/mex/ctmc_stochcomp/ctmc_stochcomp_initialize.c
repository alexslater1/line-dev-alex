/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * ctmc_stochcomp_initialize.c
 *
 * Code generation for function 'ctmc_stochcomp_initialize'
 *
 */

/* Include files */
#include "ctmc_stochcomp_initialize.h"
#include "_coder_ctmc_stochcomp_mex.h"
#include "ctmc_stochcomp_data.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void ctmc_stochcomp_once(void);

/* Function Definitions */
static void ctmc_stochcomp_once(void)
{
  mex_InitInfAndNan();
}

void ctmc_stochcomp_initialize(void)
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
    ctmc_stochcomp_once();
  }
}

/* End of code generation (ctmc_stochcomp_initialize.c) */
