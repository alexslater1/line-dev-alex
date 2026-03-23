/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * ctmc_stochcomp.h
 *
 * Code generation for function 'ctmc_stochcomp'
 *
 */

#pragma once

/* Include files */
#include "ctmc_stochcomp_types.h"
#include "rtwtypes.h"
#include "emlrt.h"
#include "mex.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function Declarations */
void ctmc_stochcomp(const emlrtStack *sp, const emxArray_real_T *Q,
                    const emxArray_real_T *b_I, emxArray_real_T *S,
                    emxArray_real_T *Q11, emxArray_real_T *Q12,
                    emxArray_real_T *Q21, emxArray_real_T *Q22,
                    emxArray_real_T *T);

/* End of code generation (ctmc_stochcomp.h) */
