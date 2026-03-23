/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_sqni.h
 *
 * Code generation for function 'pfqn_sqni'
 *
 */

#pragma once

/* Include files */
#include "pfqn_sqni_types.h"
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

void pfqn_sqni(const emlrtStack *sp, const emxArray_real_T *N,
               const emxArray_real_T *L, const emxArray_real_T *Z,
               emxArray_real_T *Q, emxArray_real_T *U, emxArray_real_T *X);

/* End of code generation (pfqn_sqni.h) */
