/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_qsys_mmcc_retrial_fp_api.c
 *
 * Code generation for function '_coder_qsys_mmcc_retrial_fp_api'
 *
 */

/* Include files */
#include "_coder_qsys_mmcc_retrial_fp_api.h"
#include "qsys_mmcc_retrial_fp.h"
#include "qsys_mmcc_retrial_fp_data.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static real_T b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId);

static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                 const emlrtMsgIdentifier *msgId);

static real_T emlrt_marshallIn(const emlrtStack *sp, const mxArray *nullptr,
                               const char_T *identifier);

static const mxArray *emlrt_marshallOut(const real_T u);

/* Function Definitions */
static real_T b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = c_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                 const emlrtMsgIdentifier *msgId)
{
  static const int32_T dims = 0;
  real_T ret;
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 0U,
                          (const void *)&dims);
  ret = *(real_T *)emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

static real_T emlrt_marshallIn(const emlrtStack *sp, const mxArray *nullptr,
                               const char_T *identifier)
{
  emlrtMsgIdentifier thisId;
  real_T y;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(nullptr), &thisId);
  emlrtDestroyArray(&nullptr);
  return y;
}

static const mxArray *emlrt_marshallOut(const real_T u)
{
  const mxArray *m;
  const mxArray *y;
  y = NULL;
  m = emlrtCreateDoubleScalar(u);
  emlrtAssign(&y, m);
  return y;
}

void qsys_mmcc_retrial_fp_api(const mxArray *const prhs[5], int32_T nlhs,
                              const mxArray *plhs[3])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  real_T blocProb;
  real_T c;
  real_T lambda;
  real_T maxiter;
  real_T mu;
  real_T niter;
  real_T r;
  real_T tol;
  st.tls = emlrtRootTLSGlobal;
  /* Marshall function inputs */
  lambda = emlrt_marshallIn(&st, emlrtAliasP(prhs[0]), "lambda");
  mu = emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "mu");
  c = emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "c");
  tol = emlrt_marshallIn(&st, emlrtAliasP(prhs[3]), "tol");
  maxiter = emlrt_marshallIn(&st, emlrtAliasP(prhs[4]), "maxiter");
  /* Invoke the target function */
  qsys_mmcc_retrial_fp(&st, lambda, mu, c, tol, maxiter, &blocProb, &r, &niter);
  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(blocProb);
  if (nlhs > 1) {
    plhs[1] = emlrt_marshallOut(r);
  }
  if (nlhs > 2) {
    plhs[2] = emlrt_marshallOut(niter);
  }
}

/* End of code generation (_coder_qsys_mmcc_retrial_fp_api.c) */
