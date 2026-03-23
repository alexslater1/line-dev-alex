/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mvaldmx_ec.h
 *
 * Code generation for function 'pfqn_mvaldmx_ec'
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
void pfqn_mvaldmx_ec(const emlrtStack *sp, const emxArray_real_T *lambda,
                     const emxArray_real_T *D, emxArray_real_T *mu,
                     emxArray_real_T *EC, emxArray_real_T *E,
                     emxArray_real_T *Eprime);

/* End of code generation (pfqn_mvaldmx_ec.h) */
