/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sort.c
 *
 * Code generation for function 'sort'
 *
 */

/* Include files */
#include "sort.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_comomrm_data.h"
#include "pfqn_comomrm_emxutil.h"
#include "pfqn_comomrm_types.h"
#include "rt_nonfinite.h"
#include "sortIdx.h"
#include "mwmathutil.h"

/* Variable Definitions */
static emlrtRSInfo nd_emlrtRSI = {
    72,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pathName
                                                                        */
};

static emlrtRSInfo hf_emlrtRSI = {
    76,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pathName
                                                                        */
};

static emlrtRSInfo if_emlrtRSI = {
    79,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pathName
                                                                        */
};

static emlrtRSInfo jf_emlrtRSI = {
    81,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pathName
                                                                        */
};

static emlrtRSInfo kf_emlrtRSI = {
    84,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pathName
                                                                        */
};

static emlrtRSInfo lf_emlrtRSI = {
    87,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pathName
                                                                        */
};

static emlrtRSInfo mf_emlrtRSI = {
    90,     /* lineNo */
    "sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pathName
                                                                        */
};

static emlrtRTEInfo oe_emlrtRTEI = {
    56,        /* lineNo */
    5,         /* colNo */
    "sortIdx", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pName
                                                                           */
};

static emlrtRTEInfo se_emlrtRTEI = {
    56,     /* lineNo */
    24,     /* colNo */
    "sort", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pName
                                                                        */
};

static emlrtRTEInfo te_emlrtRTEI = {
    56,     /* lineNo */
    1,      /* colNo */
    "sort", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pName
                                                                        */
};

static emlrtRTEInfo ue_emlrtRTEI = {
    1,      /* lineNo */
    20,     /* colNo */
    "sort", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sort.m" /* pName
                                                                        */
};

/* Function Definitions */
void b_sort(const emlrtStack *sp, emxArray_real_T *x)
{
  emlrtStack b_st;
  emlrtStack st;
  emxArray_int32_T *tf_emlrtRSI;
  emxArray_real_T *vwork;
  real_T *vwork_data;
  real_T *x_data;
  int32_T dim;
  int32_T i;
  int32_T i1;
  int32_T j;
  int32_T k;
  int32_T vstride;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  x_data = x->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  dim = 2;
  if (x->size[0] != 1) {
    dim = 1;
  }
  if (dim <= 1) {
    i = x->size[0];
  } else {
    i = 1;
  }
  emxInit_real_T(sp, &vwork, 1, &te_emlrtRTEI);
  i1 = vwork->size[0];
  vwork->size[0] = i;
  emxEnsureCapacity_real_T(sp, vwork, i1, &se_emlrtRTEI);
  vwork_data = vwork->data;
  st.site = &hf_emlrtRSI;
  vstride = 1;
  i1 = dim - 2;
  for (k = 0; k <= i1; k++) {
    vstride *= x->size[0];
  }
  st.site = &if_emlrtRSI;
  st.site = &jf_emlrtRSI;
  if (vstride > 2147483646) {
    b_st.site = &tb_emlrtRSI;
    check_forloop_overflow_error(&b_st);
  }
  emxInit_int32_T(sp, &tf_emlrtRSI, 1, &ue_emlrtRTEI);
  for (j = 0; j < vstride; j++) {
    st.site = &kf_emlrtRSI;
    if (i > 2147483646) {
      b_st.site = &tb_emlrtRSI;
      check_forloop_overflow_error(&b_st);
    }
    for (k = 0; k < i; k++) {
      vwork_data[k] = x_data[j + k * vstride];
    }
    st.site = &lf_emlrtRSI;
    sortIdx(&st, vwork, tf_emlrtRSI);
    vwork_data = vwork->data;
    st.site = &mf_emlrtRSI;
    for (k = 0; k < i; k++) {
      x_data[j + k * vstride] = vwork_data[k];
    }
  }
  emxFree_int32_T(sp, &tf_emlrtRSI);
  emxFree_real_T(sp, &vwork);
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

void sort(const emlrtStack *sp, emxArray_real_T *x, emxArray_int32_T *idx)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack st;
  emxArray_int32_T *iwork;
  emxArray_real_T *xwork;
  real_T *x_data;
  real_T *xwork_data;
  int32_T b;
  int32_T b_b;
  int32_T b_k;
  int32_T i1;
  int32_T k;
  int32_T nBlocks;
  int32_T *idx_data;
  int32_T *iwork_data;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  e_st.prev = &d_st;
  e_st.tls = d_st.tls;
  x_data = x->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  st.site = &nd_emlrtRSI;
  i1 = idx->size[0] * idx->size[1];
  idx->size[0] = 1;
  nBlocks = x->size[1];
  idx->size[1] = nBlocks;
  emxEnsureCapacity_int32_T(&st, idx, i1, &oe_emlrtRTEI);
  idx_data = idx->data;
  for (k = 0; k < nBlocks; k++) {
    idx_data[k] = 0;
  }
  if (x->size[1] != 0) {
    real_T x4[4];
    int32_T idx4[4];
    int32_T bLen;
    int32_T bLen2;
    int32_T i2;
    int32_T i4;
    int32_T ib;
    int32_T nNaNs;
    int32_T quartetOffset;
    int32_T wOffset;
    b_st.site = &od_emlrtRSI;
    emxInit_int32_T(&b_st, &iwork, 1, &pe_emlrtRTEI);
    i1 = iwork->size[0];
    iwork->size[0] = nBlocks;
    emxEnsureCapacity_int32_T(&b_st, iwork, i1, &pe_emlrtRTEI);
    iwork_data = iwork->data;
    for (k = 0; k < nBlocks; k++) {
      iwork_data[k] = 0;
    }
    emxInit_real_T(&b_st, &xwork, 1, &re_emlrtRTEI);
    i1 = xwork->size[0];
    xwork->size[0] = nBlocks;
    emxEnsureCapacity_real_T(&b_st, xwork, i1, &qe_emlrtRTEI);
    xwork_data = xwork->data;
    c_st.site = &pd_emlrtRSI;
    x4[0] = 0.0;
    idx4[0] = 0;
    x4[1] = 0.0;
    idx4[1] = 0;
    x4[2] = 0.0;
    idx4[2] = 0;
    x4[3] = 0.0;
    idx4[3] = 0;
    nNaNs = 0;
    ib = 0;
    d_st.site = &ud_emlrtRSI;
    if (x->size[1] > 2147483646) {
      e_st.site = &tb_emlrtRSI;
      check_forloop_overflow_error(&e_st);
    }
    for (k = 0; k < nBlocks; k++) {
      if (muDoubleScalarIsNaN(x_data[k])) {
        i1 = (nBlocks - nNaNs) - 1;
        idx_data[i1] = k + 1;
        xwork_data[i1] = x_data[k];
        nNaNs++;
      } else {
        ib++;
        idx4[ib - 1] = k + 1;
        x4[ib - 1] = x_data[k];
        if (ib == 4) {
          real_T d;
          real_T d1;
          quartetOffset = k - nNaNs;
          if (x4[0] <= x4[1]) {
            i1 = 1;
            i2 = 2;
          } else {
            i1 = 2;
            i2 = 1;
          }
          if (x4[2] <= x4[3]) {
            ib = 3;
            i4 = 4;
          } else {
            ib = 4;
            i4 = 3;
          }
          d = x4[i1 - 1];
          d1 = x4[ib - 1];
          if (d <= d1) {
            if (x4[i2 - 1] <= d1) {
              bLen = i1;
              bLen2 = i2;
              i1 = ib;
              i2 = i4;
            } else if (x4[i2 - 1] <= x4[i4 - 1]) {
              bLen = i1;
              bLen2 = ib;
              i1 = i2;
              i2 = i4;
            } else {
              bLen = i1;
              bLen2 = ib;
              i1 = i4;
            }
          } else if (d <= x4[i4 - 1]) {
            if (x4[i2 - 1] <= x4[i4 - 1]) {
              bLen = ib;
              bLen2 = i1;
              i1 = i2;
              i2 = i4;
            } else {
              bLen = ib;
              bLen2 = i1;
              i1 = i4;
            }
          } else {
            bLen = ib;
            bLen2 = i4;
          }
          idx_data[quartetOffset - 3] = idx4[bLen - 1];
          idx_data[quartetOffset - 2] = idx4[bLen2 - 1];
          idx_data[quartetOffset - 1] = idx4[i1 - 1];
          idx_data[quartetOffset] = idx4[i2 - 1];
          x_data[quartetOffset - 3] = x4[bLen - 1];
          x_data[quartetOffset - 2] = x4[bLen2 - 1];
          x_data[quartetOffset - 1] = x4[i1 - 1];
          x_data[quartetOffset] = x4[i2 - 1];
          ib = 0;
        }
      }
    }
    wOffset = x->size[1] - nNaNs;
    if (ib > 0) {
      int8_T perm[4];
      perm[1] = 0;
      perm[2] = 0;
      perm[3] = 0;
      if (ib == 1) {
        perm[0] = 1;
      } else if (ib == 2) {
        if (x4[0] <= x4[1]) {
          perm[0] = 1;
          perm[1] = 2;
        } else {
          perm[0] = 2;
          perm[1] = 1;
        }
      } else if (x4[0] <= x4[1]) {
        if (x4[1] <= x4[2]) {
          perm[0] = 1;
          perm[1] = 2;
          perm[2] = 3;
        } else if (x4[0] <= x4[2]) {
          perm[0] = 1;
          perm[1] = 3;
          perm[2] = 2;
        } else {
          perm[0] = 3;
          perm[1] = 1;
          perm[2] = 2;
        }
      } else if (x4[0] <= x4[2]) {
        perm[0] = 2;
        perm[1] = 1;
        perm[2] = 3;
      } else if (x4[1] <= x4[2]) {
        perm[0] = 2;
        perm[1] = 3;
        perm[2] = 1;
      } else {
        perm[0] = 3;
        perm[1] = 2;
        perm[2] = 1;
      }
      d_st.site = &vd_emlrtRSI;
      if (ib > 2147483646) {
        e_st.site = &tb_emlrtRSI;
        check_forloop_overflow_error(&e_st);
      }
      i1 = (uint8_T)ib;
      for (k = 0; k < i1; k++) {
        quartetOffset = (wOffset - ib) + k;
        i2 = perm[k];
        idx_data[quartetOffset] = idx4[i2 - 1];
        x_data[quartetOffset] = x4[i2 - 1];
      }
    }
    i1 = nNaNs >> 1;
    d_st.site = &wd_emlrtRSI;
    for (k = 0; k < i1; k++) {
      quartetOffset = wOffset + k;
      i2 = idx_data[quartetOffset];
      ib = (nBlocks - k) - 1;
      idx_data[quartetOffset] = idx_data[ib];
      idx_data[ib] = i2;
      x_data[quartetOffset] = xwork_data[ib];
      x_data[ib] = xwork_data[quartetOffset];
    }
    if (((uint32_T)nNaNs & 1U) != 0U) {
      i1 += wOffset;
      x_data[i1] = xwork_data[i1];
    }
    i1 = 2;
    if (wOffset > 1) {
      if (x->size[1] >= 256) {
        nBlocks = wOffset >> 8;
        if (nBlocks > 0) {
          c_st.site = &qd_emlrtRSI;
          for (b = 0; b < nBlocks; b++) {
            real_T b_xwork[256];
            int32_T b_iwork[256];
            c_st.site = &rd_emlrtRSI;
            i4 = (b << 8) - 1;
            for (b_b = 0; b_b < 6; b_b++) {
              bLen = 1 << (b_b + 2);
              bLen2 = bLen << 1;
              nNaNs = 256 >> (b_b + 3);
              d_st.site = &xd_emlrtRSI;
              for (b_k = 0; b_k < nNaNs; b_k++) {
                i1 = (i4 + b_k * bLen2) + 1;
                d_st.site = &yd_emlrtRSI;
                for (k = 0; k < bLen2; k++) {
                  ib = i1 + k;
                  b_iwork[k] = idx_data[ib];
                  b_xwork[k] = x_data[ib];
                }
                i2 = 0;
                quartetOffset = bLen;
                ib = i1 - 1;
                int32_T exitg1;
                do {
                  exitg1 = 0;
                  ib++;
                  if (b_xwork[i2] <= b_xwork[quartetOffset]) {
                    idx_data[ib] = b_iwork[i2];
                    x_data[ib] = b_xwork[i2];
                    if (i2 + 1 < bLen) {
                      i2++;
                    } else {
                      exitg1 = 1;
                    }
                  } else {
                    idx_data[ib] = b_iwork[quartetOffset];
                    x_data[ib] = b_xwork[quartetOffset];
                    if (quartetOffset + 1 < bLen2) {
                      quartetOffset++;
                    } else {
                      ib -= i2;
                      d_st.site = &ae_emlrtRSI;
                      for (k = i2 + 1; k <= bLen; k++) {
                        quartetOffset = ib + k;
                        idx_data[quartetOffset] = b_iwork[k - 1];
                        x_data[quartetOffset] = b_xwork[k - 1];
                      }
                      exitg1 = 1;
                    }
                  }
                } while (exitg1 == 0);
              }
            }
          }
          i1 = nBlocks << 8;
          quartetOffset = wOffset - i1;
          if (quartetOffset > 0) {
            c_st.site = &sd_emlrtRSI;
            merge_block(&c_st, idx, x, i1, quartetOffset, 2, iwork, xwork);
          }
          i1 = 8;
        }
      }
      c_st.site = &td_emlrtRSI;
      merge_block(&c_st, idx, x, 0, wOffset, i1, iwork, xwork);
    }
    emxFree_real_T(&b_st, &xwork);
    emxFree_int32_T(&b_st, &iwork);
  }
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (sort.c) */
