/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mvaldmx.h
 *
 * Code generation for function 'pfqn_mvaldmx'
 *
 */

#pragma once

/* Include files */
#include "pfqn_mvams_types.h"
#include "rtwtypes.h"
#include "emlrt.h"
#include "mex.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function Declarations */
real_T pfqn_mvaldmx(const emlrtStack *sp, const emxArray_real_T *lambda,
                    const emxArray_real_T *D, const emxArray_real_T *N,
                    const emxArray_real_T *Z, const emxArray_real_T *mu,
                    emxArray_real_T *XN, emxArray_real_T *QN,
                    emxArray_real_T *UN, emxArray_real_T *CN);

/* End of code generation (pfqn_mvaldmx.h) */
