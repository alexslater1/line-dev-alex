/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pfqn_mvamx.c
 *
 * Code generation for function 'pfqn_mvamx'
 *
 */

/* Include files */
#include "pfqn_mvamx.h"
#include "pfqn_mva.h"
#include "pfqn_mvams_data.h"
#include "pfqn_mvams_emxutil.h"
#include "pfqn_mvams_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
real_T binary_expand_op(const emlrtStack *sp, const emlrtRSInfo in1,
                        const emxArray_real_T *in2, const emxArray_real_T *in3,
                        const emxArray_real_T *in4, const emxArray_real_T *in5,
                        const emxArray_real_T *in6, const emxArray_real_T *in7,
                        emxArray_real_T *in8, emxArray_real_T *in9,
                        emxArray_real_T *in10, emxArray_real_T *in11)
{
  emlrtStack st;
  emxArray_real_T *b_in2;
  emxArray_real_T *c_in4;
  emxArray_real_T *c_in6;
  const real_T *in2_data;
  const real_T *in3_data;
  const real_T *in4_data;
  const real_T *in5_data;
  const real_T *in6_data;
  real_T out1;
  real_T *b_in2_data;
  int32_T aux_0_1;
  int32_T aux_1_1;
  int32_T b_in4;
  int32_T b_in6;
  int32_T b_loop_ub;
  int32_T i;
  int32_T i1;
  int32_T loop_ub;
  int32_T stride_0_0;
  int32_T stride_0_1;
  int32_T stride_1_0;
  int32_T stride_1_1;
  st.prev = sp;
  st.tls = sp->tls;
  in6_data = in6->data;
  in5_data = in5->data;
  in4_data = in4->data;
  in3_data = in3->data;
  in2_data = in2->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  b_in4 = in4->size[1];
  b_in6 = in6->size[1];
  emxInit_real_T(sp, &b_in2, 2, &ec_emlrtRTEI);
  if (in3->size[0] == 1) {
    loop_ub = in2->size[0];
  } else {
    loop_ub = in3->size[0];
  }
  stride_0_0 = b_in2->size[0] * b_in2->size[1];
  b_in2->size[0] = loop_ub;
  if (in3->size[1] == 1) {
    b_loop_ub = in2->size[1];
  } else {
    b_loop_ub = in3->size[1];
  }
  b_in2->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(sp, b_in2, stride_0_0, &ec_emlrtRTEI);
  b_in2_data = b_in2->data;
  stride_0_0 = (in2->size[0] != 1);
  stride_0_1 = (in2->size[1] != 1);
  stride_1_0 = (in3->size[0] != 1);
  stride_1_1 = (in3->size[1] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_in2_data[i1 + b_in2->size[0] * i] =
          in2_data[i1 * stride_0_0 + in2->size[0] * aux_0_1] /
          in3_data[i1 * stride_1_0 + in3->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_1;
    aux_0_1 += stride_0_1;
  }
  emxInit_real_T(sp, &c_in4, 2, &hc_emlrtRTEI);
  stride_0_0 = c_in4->size[0] * c_in4->size[1];
  c_in4->size[0] = 1;
  stride_0_1 = in5->size[1];
  c_in4->size[1] = stride_0_1;
  emxEnsureCapacity_real_T(sp, c_in4, stride_0_0, &hc_emlrtRTEI);
  b_in2_data = c_in4->data;
  for (i = 0; i < stride_0_1; i++) {
    stride_0_0 = (int32_T)in5_data[i];
    if ((stride_0_0 < 1) || (stride_0_0 > b_in4)) {
      emlrtDynamicBoundsCheckR2012b(stride_0_0, 1, b_in4, &dc_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    b_in2_data[i] = in4_data[stride_0_0 - 1];
  }
  emxInit_real_T(sp, &c_in6, 2, &kc_emlrtRTEI);
  stride_0_0 = c_in6->size[0] * c_in6->size[1];
  c_in6->size[0] = 1;
  c_in6->size[1] = stride_0_1;
  emxEnsureCapacity_real_T(sp, c_in6, stride_0_0, &kc_emlrtRTEI);
  b_in2_data = c_in6->data;
  for (i = 0; i < stride_0_1; i++) {
    stride_0_0 = (int32_T)in5_data[i];
    if ((stride_0_0 < 1) || (stride_0_0 > b_in6)) {
      emlrtDynamicBoundsCheckR2012b(stride_0_0, 1, b_in6, &fc_emlrtBCI,
                                    (emlrtConstCTX)sp);
    }
    b_in2_data[i] = in6_data[stride_0_0 - 1];
  }
  st.site = (emlrtRSInfo *)&in1;
  out1 = pfqn_mva(&st, b_in2, c_in4, c_in6, in7, in8, in9, in10, in11);
  emxFree_real_T(sp, &c_in6);
  emxFree_real_T(sp, &c_in4);
  emxFree_real_T(sp, &b_in2);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
  return out1;
}

/* End of code generation (pfqn_mvamx.c) */
