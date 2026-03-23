/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_ctmc_stochcomp_api.c
 *
 * Code generation for function '_coder_ctmc_stochcomp_api'
 *
 */

/* Include files */
#include "_coder_ctmc_stochcomp_api.h"
#include "ctmc_stochcomp.h"
#include "ctmc_stochcomp_data.h"
#include "ctmc_stochcomp_emxutil.h"
#include "ctmc_stochcomp_types.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRTEInfo bb_emlrtRTEI = {
    1,                           /* lineNo */
    1,                           /* colNo */
    "_coder_ctmc_stochcomp_api", /* fName */
    ""                           /* pName */
};

/* Function Declarations */
static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *nullptr,
                               const char_T *identifier, emxArray_real_T *y);

static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               emxArray_real_T *y);

static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *nullptr,
                               const char_T *identifier, emxArray_real_T *y);

static const mxArray *emlrt_marshallOut(emxArray_real_T *u);

static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               emxArray_real_T *y);

static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId,
                               emxArray_real_T *ret);

static void i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId,
                               emxArray_real_T *ret);

/* Function Definitions */
static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *nullptr,
                               const char_T *identifier, emxArray_real_T *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  d_emlrt_marshallIn(sp, emlrtAlias(nullptr), &thisId, y);
  emlrtDestroyArray(&nullptr);
}

static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               emxArray_real_T *y)
{
  h_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *nullptr,
                               const char_T *identifier, emxArray_real_T *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  f_emlrt_marshallIn(sp, emlrtAlias(nullptr), &thisId, y);
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

static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               emxArray_real_T *y)
{
  i_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId,
                               emxArray_real_T *ret)
{
  static const int32_T dims[2] = {-1, -1};
  int32_T iv[2];
  int32_T i;
  boolean_T bv[2] = {true, true};
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

static void i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
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

void ctmc_stochcomp_api(const mxArray *const prhs[2], int32_T nlhs,
                        const mxArray *plhs[6])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  emxArray_real_T *Q;
  emxArray_real_T *Q11;
  emxArray_real_T *Q12;
  emxArray_real_T *Q21;
  emxArray_real_T *Q22;
  emxArray_real_T *S;
  emxArray_real_T *T;
  emxArray_real_T *b_I;
  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  /* Marshall function inputs */
  emxInit_real_T(&st, &Q, 2, &bb_emlrtRTEI);
  Q->canFreeData = false;
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "Q", Q);
  emxInit_real_T(&st, &b_I, 2, &bb_emlrtRTEI);
  b_I->canFreeData = false;
  e_emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "I", b_I);
  /* Invoke the target function */
  emxInit_real_T(&st, &S, 2, &bb_emlrtRTEI);
  emxInit_real_T(&st, &Q11, 2, &bb_emlrtRTEI);
  emxInit_real_T(&st, &Q12, 2, &bb_emlrtRTEI);
  emxInit_real_T(&st, &Q21, 2, &bb_emlrtRTEI);
  emxInit_real_T(&st, &Q22, 2, &bb_emlrtRTEI);
  emxInit_real_T(&st, &T, 2, &bb_emlrtRTEI);
  ctmc_stochcomp(&st, Q, b_I, S, Q11, Q12, Q21, Q22, T);
  emxFree_real_T(&st, &b_I);
  emxFree_real_T(&st, &Q);
  /* Marshall function outputs */
  S->canFreeData = false;
  plhs[0] = emlrt_marshallOut(S);
  emxFree_real_T(&st, &S);
  if (nlhs > 1) {
    Q11->canFreeData = false;
    plhs[1] = emlrt_marshallOut(Q11);
  }
  emxFree_real_T(&st, &Q11);
  if (nlhs > 2) {
    Q12->canFreeData = false;
    plhs[2] = emlrt_marshallOut(Q12);
  }
  emxFree_real_T(&st, &Q12);
  if (nlhs > 3) {
    Q21->canFreeData = false;
    plhs[3] = emlrt_marshallOut(Q21);
  }
  emxFree_real_T(&st, &Q21);
  if (nlhs > 4) {
    Q22->canFreeData = false;
    plhs[4] = emlrt_marshallOut(Q22);
  }
  emxFree_real_T(&st, &Q22);
  if (nlhs > 5) {
    T->canFreeData = false;
    plhs[5] = emlrt_marshallOut(T);
  }
  emxFree_real_T(&st, &T);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

/* End of code generation (_coder_ctmc_stochcomp_api.c) */
