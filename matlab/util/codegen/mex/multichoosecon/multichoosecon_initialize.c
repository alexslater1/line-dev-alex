/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * multichoosecon_initialize.c
 *
 * Code generation for function 'multichoosecon_initialize'
 *
 */

/* Include files */
#include "multichoosecon_initialize.h"
#include "_coder_multichoosecon_mex.h"
#include "multichoosecon_data.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void multichoosecon_once(void);

/* Function Definitions */
static void multichoosecon_once(void)
{
  mex_InitInfAndNan();
}

void multichoosecon_initialize(void)
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
    multichoosecon_once();
  }
}

/* End of code generation (multichoosecon_initialize.c) */
