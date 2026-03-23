/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_xzabaup.h
 *
 * Code generation for function 'pfqn_xzabaup'
 *
 */

#pragma once

/* Include files */
#include "pfqn_xzabaup_types.h"
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

real_T pfqn_xzabaup(const emlrtStack *sp, const emxArray_real_T *L,
                    const emxArray_real_T *N, const emxArray_real_T *Z);

/* End of code generation (pfqn_xzabaup.h) */
