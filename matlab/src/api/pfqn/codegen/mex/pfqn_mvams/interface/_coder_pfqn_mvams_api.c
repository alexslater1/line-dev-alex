/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_pfqn_mvams_api.c
 *
 * Code generation for function '_coder_pfqn_mvams_api'
 *
 */

/* Include files */
#include "_coder_pfqn_mvams_api.h"
#include "pfqn_mvams.h"
#include "pfqn_mvams_data.h"
#include "pfqn_mvams_emxutil.h"
#include "pfqn_mvams_types.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
static emlrtRTEInfo di_emlrtRTEI = {
    1,                       /* lineNo */
    1,                       /* colNo */
    "_coder_pfqn_mvams_api", /* fName */
    ""                       /* pName */
};

/* Function Declarations */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               emxArray_real_T *y);

static const mxArray *b_emlrt_marshallOut(const real_T u);

static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *nullptr,
                               const char_T *identifier, emxArray_real_T *y);

static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               emxArray_real_T *y);

static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId,
                               emxArray_real_T *ret);

static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *nullptr,
                             const char_T *identifier, emxArray_real_T *y);

static const mxArray *emlrt_marshallOut(emxArray_real_T *u);

static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId,
                               emxArray_real_T *ret);

/* Function Definitions */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               emxArray_real_T *y)
{
  e_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static const mxArray *b_emlrt_marshallOut(const real_T u)
{
  const mxArray *m;
  const mxArray *y;
  y = NULL;
  m = emlrtCreateDoubleScalar(u);
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
  f_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
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

static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
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

void pfqn_mvams_api(const mxArray *const prhs[6], int32_T nlhs,
                    const mxArray *plhs[5])
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
  emxArray_real_T *S;
  emxArray_real_T *UN;
  emxArray_real_T *XN;
  emxArray_real_T *Z;
  emxArray_real_T *lambda;
  emxArray_real_T *mi;
  const mxArray *prhs_copy_idx_3;
  real_T lG;
  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  prhs_copy_idx_3 = emlrtProtectR2012b(prhs[3], 3, false, -1);
  /* Marshall function inputs */
  emxInit_real_T(&st, &lambda, 2, &di_emlrtRTEI);
  lambda->canFreeData = false;
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "lambda", lambda);
  emxInit_real_T(&st, &L, 2, &di_emlrtRTEI);
  L->canFreeData = false;
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "L", L);
  emxInit_real_T(&st, &N, 2, &di_emlrtRTEI);
  N->canFreeData = false;
  emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "N", N);
  emxInit_real_T(&st, &Z, 2, &di_emlrtRTEI);
  Z->canFreeData = false;
  emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_3), "Z", Z);
  emxInit_real_T(&st, &mi, 2, &di_emlrtRTEI);
  mi->canFreeData = false;
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[4]), "mi", mi);
  emxInit_real_T(&st, &S, 2, &di_emlrtRTEI);
  S->canFreeData = false;
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[5]), "S", S);
  /* Invoke the target function */
  emxInit_real_T(&st, &XN, 2, &di_emlrtRTEI);
  emxInit_real_T(&st, &QN, 2, &di_emlrtRTEI);
  emxInit_real_T(&st, &UN, 2, &di_emlrtRTEI);
  emxInit_real_T(&st, &CN, 2, &di_emlrtRTEI);
  pfqn_mvams(&st, lambda, L, N, Z, mi, S, XN, QN, UN, CN, &lG);
  emxFree_real_T(&st, &S);
  emxFree_real_T(&st, &mi);
  emxFree_real_T(&st, &Z);
  emxFree_real_T(&st, &N);
  emxFree_real_T(&st, &L);
  emxFree_real_T(&st, &lambda);
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
    plhs[4] = b_emlrt_marshallOut(lG);
  }
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

/* End of code generation (_coder_pfqn_mvams_api.c) */
