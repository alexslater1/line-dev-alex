/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_comom_mexutil.c
 *
 * Code generation for function 'pfqn_comom_mexutil'
 *
 */

/* Include files */
#include "pfqn_comom_mexutil.h"
#include "rt_nonfinite.h"

/* Function Definitions */
const mxArray *b_sprintf(const emlrtStack *sp, const mxArray *m,
                         const mxArray *m1, emlrtMCInfo *location)
{
  const mxArray *pArrays[2];
  const mxArray *m2;
  pArrays[0] = m;
  pArrays[1] = m1;
  return emlrtCallMATLABR2012b((emlrtConstCTX)sp, 1, &m2, 2, &pArrays[0],
                               "sprintf", true, location);
}

const mxArray *emlrt_marshallOut(const real_T u)
{
  const mxArray *m;
  const mxArray *y;
  y = NULL;
  m = emlrtCreateDoubleScalar(u);
  emlrtAssign(&y, m);
  return y;
}

/* End of code generation (pfqn_comom_mexutil.c) */
