/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mmint2_mexutil.h
 *
 * Code generation for function 'pfqn_mmint2_mexutil'
 *
 */

#pragma once

/* Include files */
#include "rtwtypes.h"
#include "emlrt.h"
#include "mex.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function Declarations */
void b_feval(const emlrtStack *sp, const mxArray *m, const mxArray *m1,
             emlrtMCInfo *location);

const mxArray *emlrt_marshallOut(const real_T u);

/* End of code generation (pfqn_mmint2_mexutil.h) */
