/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mvald.h
 *
 * Code generation for function 'pfqn_mvald'
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
void pfqn_mvald(const emlrtStack *sp, const emxArray_real_T *L,
                const emxArray_real_T *N, const emxArray_real_T *Z,
                const emxArray_real_T *mu, emxArray_real_T *XN,
                emxArray_real_T *QN, emxArray_real_T *UN, emxArray_real_T *CN,
                emxArray_real_T *lGN);

/* End of code generation (pfqn_mvald.h) */
