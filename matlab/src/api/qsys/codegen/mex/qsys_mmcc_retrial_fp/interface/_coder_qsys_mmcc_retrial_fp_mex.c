/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_qsys_mmcc_retrial_fp_mex.c
 *
 * Code generation for function '_coder_qsys_mmcc_retrial_fp_mex'
 *
 */

/* Include files */
#include "_coder_qsys_mmcc_retrial_fp_mex.h"
#include "_coder_qsys_mmcc_retrial_fp_api.h"
#include "qsys_mmcc_retrial_fp_data.h"
#include "qsys_mmcc_retrial_fp_initialize.h"
#include "qsys_mmcc_retrial_fp_terminate.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs,
                 const mxArray *prhs[])
{
  mexAtExit(&qsys_mmcc_retrial_fp_atexit);
  qsys_mmcc_retrial_fp_initialize();
  qsys_mmcc_retrial_fp_mexFunction(nlhs, plhs, nrhs, prhs);
  qsys_mmcc_retrial_fp_terminate();
}

emlrtCTX mexFunctionCreateRootTLS(void)
{
  emlrtCreateRootTLSR2022a(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1,
                           NULL, "UTF-8", true);
  return emlrtRootTLSGlobal;
}

void qsys_mmcc_retrial_fp_mexFunction(int32_T nlhs, mxArray *plhs[3],
                                      int32_T nrhs, const mxArray *prhs[5])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  const mxArray *outputs[3];
  int32_T i;
  st.tls = emlrtRootTLSGlobal;
  /* Check for proper number of arguments. */
  if (nrhs != 5) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 5, 4,
                        20, "qsys_mmcc_retrial_fp");
  }
  if (nlhs > 3) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 20,
                        "qsys_mmcc_retrial_fp");
  }
  /* Call the function. */
  qsys_mmcc_retrial_fp_api(prhs, nlhs, outputs);
  /* Copy over outputs to the caller. */
  if (nlhs < 1) {
    i = 1;
  } else {
    i = nlhs;
  }
  emlrtReturnArrays(i, &plhs[0], &outputs[0]);
}

/* End of code generation (_coder_qsys_mmcc_retrial_fp_mex.c) */
