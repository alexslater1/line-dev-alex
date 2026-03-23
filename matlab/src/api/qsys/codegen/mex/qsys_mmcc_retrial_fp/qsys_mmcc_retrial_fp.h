/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * qsys_mmcc_retrial_fp.h
 *
 * Code generation for function 'qsys_mmcc_retrial_fp'
 *
 */

#pragma once

/* Include files */
#include "rtwtypes.h"
#include "emlrt.h"
#include "mex.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function Declarations */
void qsys_mmcc_retrial_fp(const emlrtStack *sp, real_T lambda, real_T mu,
                          real_T c, real_T tol, real_T maxiter,
                          real_T *blocProb, real_T *r, real_T *niter);

/* End of code generation (qsys_mmcc_retrial_fp.h) */
