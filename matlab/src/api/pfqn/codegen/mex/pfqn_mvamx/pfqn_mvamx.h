/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mvamx.h
 *
 * Code generation for function 'pfqn_mvamx'
 *
 */

#pragma once

/* Include files */
#include "pfqn_mvamx_types.h"
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

void pfqn_mvamx(const emlrtStack *sp, const emxArray_real_T *lambda,
                const emxArray_real_T *D, const emxArray_real_T *N,
                emxArray_real_T *Z, const emxArray_real_T *mi,
                emxArray_real_T *XN, emxArray_real_T *QN, emxArray_real_T *UN,
                emxArray_real_T *CN, real_T *lGN);

/* End of code generation (pfqn_mvamx.h) */
