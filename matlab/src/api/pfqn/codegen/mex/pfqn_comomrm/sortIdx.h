/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sortIdx.h
 *
 * Code generation for function 'sortIdx'
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
void merge_block(const emlrtStack *sp, emxArray_int32_T *idx,
                 emxArray_real_T *x, int32_T offset, int32_T n,
                 int32_T preSortLevel, emxArray_int32_T *iwork,
                 emxArray_real_T *xwork);

void sortIdx(const emlrtStack *sp, emxArray_real_T *x, emxArray_int32_T *idx);

/* End of code generation (sortIdx.h) */
