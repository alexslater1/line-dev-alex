/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_nc_sanitize.h
 *
 * Code generation for function 'pfqn_nc_sanitize'
 *
 */

#pragma once

/* Include files */
#include "pfqn_comomrm_types.h"
#include "rtwtypes.h"
#include "emlrt.h"
#include "mex.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function Declarations */
real_T pfqn_nc_sanitize(const emlrtStack *sp, emxArray_real_T *lambda,
                        emxArray_real_T *L, emxArray_real_T *N,
                        emxArray_real_T *Z, real_T b_atol);

/* End of code generation (pfqn_nc_sanitize.h) */
