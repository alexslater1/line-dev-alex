/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * integralCalc.h
 *
 * Code generation for function 'integralCalc'
 *
 */

#pragma once

/* Include files */
#include "pfqn_mmint2_types.h"
#include "rtwtypes.h"
#include "emlrt.h"
#include "mex.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function Declarations */
real_T scalarValuedIntegral(const emlrtStack *sp,
                            const emxArray_real_T *fun_workspace_Z,
                            const emxArray_real_T *fun_workspace_nnzClasses,
                            const emxArray_real_T *fun_workspace_L,
                            const emxArray_real_T *fun_workspace_N,
                            const emxArray_real_T *interval, int32_T nt,
                            real_T pathlen, real_T *errbnd, boolean_T *success);

/* End of code generation (integralCalc.h) */
