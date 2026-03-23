/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * qsys_mmcc_retrial_fp_initialize.c
 *
 * Code generation for function 'qsys_mmcc_retrial_fp_initialize'
 *
 */

/* Include files */
#include "qsys_mmcc_retrial_fp_initialize.h"
#include "_coder_qsys_mmcc_retrial_fp_mex.h"
#include "qsys_mmcc_retrial_fp_data.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void qsys_mmcc_retrial_fp_once(void);

/* Function Definitions */
static void qsys_mmcc_retrial_fp_once(void)
{
  mex_InitInfAndNan();
}

void qsys_mmcc_retrial_fp_initialize(void)
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
    qsys_mmcc_retrial_fp_once();
  }
}

/* End of code generation (qsys_mmcc_retrial_fp_initialize.c) */
