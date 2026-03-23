/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * qsys_mmcc_retrial_fp.c
 *
 * Code generation for function 'qsys_mmcc_retrial_fp'
 *
 */

/* Include files */
#include "qsys_mmcc_retrial_fp.h"
#include "qsys_mmcc_retrial_fp_data.h"
#include "rt_nonfinite.h"
#include "mwmathutil.h"

/* Variable Definitions */
static emlrtRSInfo emlrtRSI = {
    46,                     /* lineNo */
    "qsys_mmcc_retrial_fp", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/qsys/"
    "qsys_mmcc_retrial_fp.m" /* pathName */
};

static emlrtRSInfo b_emlrtRSI = {
    55,                     /* lineNo */
    "qsys_mmcc_retrial_fp", /* fcnName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/qsys/"
    "qsys_mmcc_retrial_fp.m" /* pathName */
};

static emlrtRTEInfo emlrtRTEI = {
    43,                     /* lineNo */
    12,                     /* colNo */
    "qsys_mmcc_retrial_fp", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/qsys/"
    "qsys_mmcc_retrial_fp.m" /* pName */
};

static emlrtRTEInfo b_emlrtRTEI = {
    62,        /* lineNo */
    9,         /* colNo */
    "ErlangB", /* fName */
    "/home/gcasale/Dropbox/code/line-dev.git/matlab/src/api/qsys/"
    "qsys_mmcc_retrial_fp.m" /* pName */
};

/* Function Definitions */
void qsys_mmcc_retrial_fp(const emlrtStack *sp, real_T lambda, real_T mu,
                          real_T c, real_T tol, real_T maxiter,
                          real_T *blocProb, real_T *r, real_T *niter)
{
  emlrtStack st;
  real_T b;
  real_T b_tmp;
  int32_T i;
  int32_T iter;
  boolean_T exitg1;
  st.prev = sp;
  st.tls = sp->tls;
  /*  [blocProb, r, niter] = QSYS_MMCC_RETRIAL_FP(LAMBDA, MU, C, TOL, MAXITER)
   */
  /*  */
  /*  Fixed-point approximation for M/M/c/c retrial queues. */
  /*  */
  /*  Customers arrive at rate LAMBDA to a system with C servers, each with */
  /*  service rate MU. Blocked customers join an orbit and retry. Under the */
  /*  assumption that the retrial rate is small relative to the service rate, */
  /*  the total arrival flow (fresh + retrial) is approximated by a Poisson */
  /*  process with rate LAMBDA + r, where r satisfies the fixed-point equation:
   */
  /*  */
  /*    r = (lambda + r) * B(lambda/mu + r/mu, c) */
  /*  */
  /*  and B(a, c) is the Erlang-B blocking probability for offered load a and */
  /*  c servers. */
  /*  */
  /*  INPUT: */
  /*    lambda  : arrival rate */
  /*    mu      : service rate per server */
  /*    c       : number of servers (= capacity, no waiting room) */
  /*    tol     : convergence tolerance (default: 1e-10) */
  /*    maxiter : maximum iterations (default: 10000) */
  /*  */
  /*  OUTPUT: */
  /*    blocProb : blocking probability (fraction of arrivals lost or retried)
   */
  /*    r        : additional arrival rate due to retrials */
  /*    niter    : number of iterations to converge */
  /*  */
  /*  REFERENCE: */
  /*    Cohen (1957), fixed-point approximation for M/M/c/c retrial queues. */
  /*    Phung-Duc, "Retrial Queueing Models: A Survey on Theory and */
  /*    Applications", 2019, Eq. (1). */
  *r = 0.0;
  *niter = 0.0;
  emlrtForLoopVectorCheckR2021a(1.0, 1.0, maxiter, mxDOUBLE_CLASS,
                                (int32_T)maxiter, &emlrtRTEI,
                                (emlrtConstCTX)sp);
  iter = 0;
  exitg1 = false;
  while ((!exitg1) && (iter <= (int32_T)maxiter - 1)) {
    real_T a;
    real_T a_tmp;
    int32_T b_i;
    *niter = (real_T)iter + 1.0;
    a_tmp = lambda + *r;
    a = a_tmp / mu;
    /*  offered load */
    st.site = &emlrtRSI;
    /*  Erlang-B formula using the recursive method (numerically stable). */
    /*  B(a, c) = blocking probability for offered load a and c servers. */
    b = 1.0;
    b_i = (int32_T)c;
    emlrtForLoopVectorCheckR2021a(1.0, 1.0, c, mxDOUBLE_CLASS, (int32_T)c,
                                  &b_emlrtRTEI, &st);
    for (i = 0; i < b_i; i++) {
      b_tmp = a * b;
      b = b_tmp / (((real_T)i + 1.0) + b_tmp);
      if (*emlrtBreakCheckR2012bFlagVar != 0) {
        emlrtBreakCheckR2012b(&st);
      }
    }
    b *= a_tmp;
    if (muDoubleScalarAbs(b - *r) < tol) {
      *r = b;
      exitg1 = true;
    } else {
      *r = b;
      iter++;
    }
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b((emlrtConstCTX)sp);
    }
  }
  st.site = &b_emlrtRSI;
  b = (lambda + *r) / mu;
  /*  Erlang-B formula using the recursive method (numerically stable). */
  /*  B(a, c) = blocking probability for offered load a and c servers. */
  *blocProb = 1.0;
  iter = (int32_T)c;
  emlrtForLoopVectorCheckR2021a(1.0, 1.0, c, mxDOUBLE_CLASS, (int32_T)c,
                                &b_emlrtRTEI, &st);
  for (i = 0; i < iter; i++) {
    b_tmp = b * *blocProb;
    *blocProb = b_tmp / (((real_T)i + 1.0) + b_tmp);
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b(&st);
    }
  }
}

/* End of code generation (qsys_mmcc_retrial_fp.c) */
