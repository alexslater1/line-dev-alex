/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mvams_mexutil.c
 *
 * Code generation for function 'pfqn_mvams_mexutil'
 *
 */

/* Include files */
#include "pfqn_mvams_mexutil.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void f_error(const emlrtStack *sp, const mxArray *m, const mxArray *m1,
             const mxArray *m2, emlrtMCInfo *location)
{
  const mxArray *pArrays[3];
  pArrays[0] = m;
  pArrays[1] = m1;
  pArrays[2] = m2;
  emlrtCallMATLABR2012b((emlrtConstCTX)sp, 0, NULL, 3, &pArrays[0], "error",
                        true, location);
}

/* End of code generation (pfqn_mvams_mexutil.c) */
