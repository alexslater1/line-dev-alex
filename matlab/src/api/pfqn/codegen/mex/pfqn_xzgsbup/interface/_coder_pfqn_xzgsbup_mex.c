/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_pfqn_xzgsbup_mex.c
 *
 * Code generation for function '_coder_pfqn_xzgsbup_mex'
 *
 */

/* Include files */
#include "_coder_pfqn_xzgsbup_mex.h"
#include "_coder_pfqn_xzgsbup_api.h"
#include "pfqn_xzgsbup_data.h"
#include "pfqn_xzgsbup_initialize.h"
#include "pfqn_xzgsbup_terminate.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs,
                 const mxArray *prhs[])
{
  mexAtExit(&pfqn_xzgsbup_atexit);
  pfqn_xzgsbup_initialize();
  pfqn_xzgsbup_mexFunction(nlhs, plhs, nrhs, prhs);
  pfqn_xzgsbup_terminate();
}

emlrtCTX mexFunctionCreateRootTLS(void)
{
  emlrtCreateRootTLSR2022a(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1,
                           NULL, "UTF-8", true);
  return emlrtRootTLSGlobal;
}

void pfqn_xzgsbup_mexFunction(int32_T nlhs, mxArray *plhs[1], int32_T nrhs,
                              const mxArray *prhs[3])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  const mxArray *outputs;
  st.tls = emlrtRootTLSGlobal;
  /* Check for proper number of arguments. */
  if (nrhs != 3) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 3, 4,
                        12, "pfqn_xzgsbup");
  }
  if (nlhs > 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 12,
                        "pfqn_xzgsbup");
  }
  /* Call the function. */
  pfqn_xzgsbup_api(prhs, &outputs);
  /* Copy over outputs to the caller. */
  emlrtReturnArrays(1, &plhs[0], &outputs);
}

/* End of code generation (_coder_pfqn_xzgsbup_mex.c) */
