/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_pfqn_mvald_api.c
 *
 * Code generation for function '_coder_pfqn_mvald_api'
 *
 */

/* Include files */
#include "_coder_pfqn_mvald_api.h"
#include "pfqn_mvald.h"
#include "pfqn_mvald_data.h"
#include "pfqn_mvald_emxutil.h"
#include "pfqn_mvald_types.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRTEInfo xb_emlrtRTEI = {
    1,                       /* lineNo */
    1,                       /* colNo */
    "_coder_pfqn_mvald_api", /* fName */
    ""                       /* pName */
};

/* Function Declarations */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               emxArray_real_T *y);

static const mxArray *b_emlrt_marshallOut(const boolean_T u);

static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *nullptr,
                               const char_T *identifier, emxArray_real_T *y);

static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               emxArray_real_T *y);

static real_T e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *nullptr,
                                 const char_T *identifier);

static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *nullptr,
                             const char_T *identifier, emxArray_real_T *y);

static const mxArray *emlrt_marshallOut(emxArray_real_T *u);

static real_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId);

static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId,
                               emxArray_real_T *ret);

static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId,
                               emxArray_real_T *ret);

static real_T i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                 const emlrtMsgIdentifier *msgId);

/* Function Definitions */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               emxArray_real_T *y)
{
  g_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static const mxArray *b_emlrt_marshallOut(const boolean_T u)
{
  const mxArray *m;
  const mxArray *y;
  y = NULL;
  m = emlrtCreateLogicalScalar(u);
  emlrtAssign(&y, m);
  return y;
}

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

static real_T e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *nullptr,
                                 const char_T *identifier)
{
  emlrtMsgIdentifier thisId;
  real_T y;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = f_emlrt_marshallIn(sp, emlrtAlias(nullptr), &thisId);
  emlrtDestroyArray(&nullptr);
  return y;
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

static real_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = i_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
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

static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
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

static real_T i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
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

void pfqn_mvald_api(const mxArray *const prhs[5], int32_T nlhs,
                    const mxArray *plhs[7])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  emxArray_real_T *CN;
  emxArray_real_T *L;
  emxArray_real_T *N;
  emxArray_real_T *QN;
  emxArray_real_T *UN;
  emxArray_real_T *XN;
  emxArray_real_T *Z;
  emxArray_real_T *lGN;
  emxArray_real_T *mu;
  emxArray_real_T *pi;
  real_T stabilize;
  boolean_T isNumStable;
  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  /* Marshall function inputs */
  emxInit_real_T(&st, &L, 2, &xb_emlrtRTEI);
  L->canFreeData = false;
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "L", L);
  emxInit_real_T(&st, &N, 2, &xb_emlrtRTEI);
  N->canFreeData = false;
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "N", N);
  emxInit_real_T(&st, &Z, 2, &xb_emlrtRTEI);
  Z->canFreeData = false;
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "Z", Z);
  emxInit_real_T(&st, &mu, 2, &xb_emlrtRTEI);
  mu->canFreeData = false;
  emlrt_marshallIn(&st, emlrtAlias(prhs[3]), "mu", mu);
  stabilize = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[4]), "stabilize");
  /* Invoke the target function */
  emxInit_real_T(&st, &XN, 2, &xb_emlrtRTEI);
  emxInit_real_T(&st, &QN, 2, &xb_emlrtRTEI);
  emxInit_real_T(&st, &UN, 2, &xb_emlrtRTEI);
  emxInit_real_T(&st, &CN, 2, &xb_emlrtRTEI);
  emxInit_real_T(&st, &lGN, 2, &xb_emlrtRTEI);
  emxInit_real_T(&st, &pi, 2, &xb_emlrtRTEI);
  pfqn_mvald(&st, L, N, Z, mu, stabilize, XN, QN, UN, CN, lGN, &isNumStable,
             pi);
  emxFree_real_T(&st, &mu);
  emxFree_real_T(&st, &Z);
  emxFree_real_T(&st, &N);
  emxFree_real_T(&st, &L);
  /* Marshall function outputs */
  XN->canFreeData = false;
  plhs[0] = emlrt_marshallOut(XN);
  emxFree_real_T(&st, &XN);
  if (nlhs > 1) {
    QN->canFreeData = false;
    plhs[1] = emlrt_marshallOut(QN);
  }
  emxFree_real_T(&st, &QN);
  if (nlhs > 2) {
    UN->canFreeData = false;
    plhs[2] = emlrt_marshallOut(UN);
  }
  emxFree_real_T(&st, &UN);
  if (nlhs > 3) {
    CN->canFreeData = false;
    plhs[3] = emlrt_marshallOut(CN);
  }
  emxFree_real_T(&st, &CN);
  if (nlhs > 4) {
    lGN->canFreeData = false;
    plhs[4] = emlrt_marshallOut(lGN);
  }
  emxFree_real_T(&st, &lGN);
  if (nlhs > 5) {
    plhs[5] = b_emlrt_marshallOut(isNumStable);
  }
  if (nlhs > 6) {
    pi->canFreeData = false;
    plhs[6] = emlrt_marshallOut(pi);
  }
  emxFree_real_T(&st, &pi);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

/* End of code generation (_coder_pfqn_mvald_api.c) */
