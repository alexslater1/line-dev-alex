/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_comom.h
 *
 * Code generation for function 'pfqn_comom'
 *
 */

#pragma once

/* Include files */
#include "pfqn_comom_types.h"
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

real_T pfqn_comom(const emlrtStack *sp, emxArray_real_T *L,
                  const emxArray_real_T *N, const emxArray_real_T *Z,
                  real_T b_atol);

/* End of code generation (pfqn_comom.h) */
