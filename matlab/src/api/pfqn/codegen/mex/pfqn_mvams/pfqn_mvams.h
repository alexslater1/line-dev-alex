/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mvams.h
 *
 * Code generation for function 'pfqn_mvams'
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
emlrtCTX emlrtGetRootTLSGlobal(void);

void emlrtLockerFunction(EmlrtLockeeFunction aLockee, emlrtConstCTX aTLS,
                         void *aData);

void pfqn_mvams(const emlrtStack *sp, const emxArray_real_T *lambda,
                const emxArray_real_T *L, const emxArray_real_T *N,
                emxArray_real_T *Z, const emxArray_real_T *mi,
                const emxArray_real_T *S, emxArray_real_T *XN,
                emxArray_real_T *QN, emxArray_real_T *UN, emxArray_real_T *CN,
                real_T *lG);

/* End of code generation (pfqn_mvams.h) */
