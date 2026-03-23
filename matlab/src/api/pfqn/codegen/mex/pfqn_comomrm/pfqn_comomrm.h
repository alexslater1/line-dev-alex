/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_comomrm.h
 *
 * Code generation for function 'pfqn_comomrm'
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
emlrtCTX emlrtGetRootTLSGlobal(void);

void emlrtLockerFunction(EmlrtLockeeFunction aLockee, emlrtConstCTX aTLS,
                         void *aData);

void pfqn_comomrm(const emlrtStack *sp, const emxArray_real_T *L,
                  const emxArray_real_T *N, const emxArray_real_T *Z, real_T m,
                  real_T b_atol, real_T *lG, emxArray_real_T *lGbasis);

/* End of code generation (pfqn_comomrm.h) */
