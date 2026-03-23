/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_pfqn_sqni_api.c
 *
 * Code generation for function '_coder_pfqn_sqni_api'
 *
 */

/* Include files */
#include "_coder_pfqn_sqni_api.h"
#include "pfqn_sqni.h"
#include "pfqn_sqni_data.h"
#include "pfqn_sqni_emxutil.h"
#include "pfqn_sqni_types.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRTEInfo p_emlrtRTEI = {
    1,                      /* lineNo */
    1,                      /* colNo */
    "_coder_pfqn_sqni_api", /* fName */
    ""                      /* pName */
};

/* Function Declarations */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               emxArray_real_T *y);

static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId,
                               emxArray_real_T *ret);

static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *nullptr,
                             const char_T *identifier, emxArray_real_T *y);

static const mxArray *emlrt_marshallOut(emxArray_real_T *u);

/* Function Definitions */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               emxArray_real_T *y)
{
  c_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId,
                               emxArray_real_T *ret)
{
  static const int32_T dims[2] = {1, -1};
  int32_T iv[2];
  int32_T i;
  boolean_T bv[2] = {false, true};
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 2U,
                            (const void *)&dims[0], &bv[0], &iv[0]);
  ret->allocatedSize = iv[0] * iv[1];
  i = ret->size[0] * ret->size[1];
  ret->size[0] = iv[0];
  ret->size[1] = iv[1];
  emxEnsureCapacity_real_T(sp, ret, i, (emlrtRTEInfo *)NULL);
  ret->data = (real_T *)emlrtMxGetData(src);
  ret->canFreeData = false;
  emlrtDestroyArray(&src);
}

static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *nullptr,
                             const char_T *identifier, emxArray_real_T *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  b_emlrt_marshallIn(sp, emlrtAlias(nullptr), &thisId, y);
  emlrtDestroyArray(&nullptr);
}

static const mxArray *emlrt_marshallOut(emxArray_real_T *u)
{
  static const int32_T iv[2] = {0, 0};
  const mxArray *m;
  const mxArray *y;
  real_T *u_data;
  u_data = u->data;
  y = NULL;
  m = emlrtCreateNumericArray(2, (const void *)&iv[0], mxDOUBLE_CLASS, mxREAL);
  emlrtMxSetData((mxArray *)m, &u_data[0]);
  emlrtSetDimensions((mxArray *)m, &u->size[0], 2);
  u->canFreeData = false;
  emlrtAssign(&y, m);
  return y;
}

void pfqn_sqni_api(const mxArray *const prhs[3], int32_T nlhs,
                   const mxArray *plhs[3])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  emxArray_real_T *L;
  emxArray_real_T *N;
  emxArray_real_T *Q;
  emxArray_real_T *U;
  emxArray_real_T *X;
  emxArray_real_T *Z;
  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  /* Marshall function inputs */
  emxInit_real_T(&st, &N, &p_emlrtRTEI);
  N->canFreeData = false;
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "N", N);
  emxInit_real_T(&st, &L, &p_emlrtRTEI);
  L->canFreeData = false;
  emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "L", L);
  emxInit_real_T(&st, &Z, &p_emlrtRTEI);
  Z->canFreeData = false;
  emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "Z", Z);
  /* Invoke the target function */
  emxInit_real_T(&st, &Q, &p_emlrtRTEI);
  emxInit_real_T(&st, &U, &p_emlrtRTEI);
  emxInit_real_T(&st, &X, &p_emlrtRTEI);
  pfqn_sqni(&st, N, L, Z, Q, U, X);
  emxFree_real_T(&st, &Z);
  emxFree_real_T(&st, &L);
  emxFree_real_T(&st, &N);
  /* Marshall function outputs */
  Q->canFreeData = false;
  plhs[0] = emlrt_marshallOut(Q);
  emxFree_real_T(&st, &Q);
  if (nlhs > 1) {
    U->canFreeData = false;
    plhs[1] = emlrt_marshallOut(U);
  }
  emxFree_real_T(&st, &U);
  if (nlhs > 2) {
    X->canFreeData = false;
    plhs[2] = emlrt_marshallOut(X);
  }
  emxFree_real_T(&st, &X);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

/* End of code generation (_coder_pfqn_sqni_api.c) */
