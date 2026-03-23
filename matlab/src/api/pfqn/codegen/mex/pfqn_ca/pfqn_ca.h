/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_ca.h
 *
 * Code generation for function 'pfqn_ca'
 *
 */

#pragma once

/* Include files */
#include "pfqn_ca_types.h"
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

void pfqn_ca(const emlrtStack *sp, const emxArray_real_T *L,
             const emxArray_real_T *N, emxArray_real_T *Z, real_T *Gn,
             real_T *lGn);

/* End of code generation (pfqn_ca.h) */
