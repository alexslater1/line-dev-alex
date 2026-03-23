/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mmint2_gausslegendre.h
 *
 * Code generation for function 'pfqn_mmint2_gausslegendre'
 *
 */

#pragma once

/* Include files */
#include "pfqn_mmint2_gausslegendre_types.h"
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

void pfqn_mmint2_gausslegendre(const emlrtStack *sp, const emxArray_real_T *L,
                               const emxArray_real_T *N,
                               const emxArray_real_T *Z, real_T m, real_T *G,
                               real_T *lG);

void pfqn_mmint2_gausslegendre_init(void);

/* End of code generation (pfqn_mmint2_gausslegendre.h) */
