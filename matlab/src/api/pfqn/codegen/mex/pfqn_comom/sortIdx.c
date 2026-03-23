/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sortIdx.c
 *
 * Code generation for function 'sortIdx'
 *
 */

/* Include files */
#include "sortIdx.h"
#include "eml_int_forloop_overflow_check.h"
#include "pfqn_comom_data.h"
#include "pfqn_comom_emxutil.h"
#include "pfqn_comom_types.h"
#include "rt_nonfinite.h"
#include "mwmathutil.h"

/* Variable Definitions */
static emlrtRSInfo uf_emlrtRSI = {
    105,       /* lineNo */
    "sortIdx", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo vf_emlrtRSI = {
    301,                /* lineNo */
    "block_merge_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo wf_emlrtRSI = {
    309,                /* lineNo */
    "block_merge_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo xf_emlrtRSI = {
    310,                /* lineNo */
    "block_merge_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo yf_emlrtRSI = {
    318,                /* lineNo */
    "block_merge_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo ag_emlrtRSI = {
    326,                /* lineNo */
    "block_merge_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo bg_emlrtRSI = {
    381,                      /* lineNo */
    "initialize_vector_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo cg_emlrtRSI = {
    409,                      /* lineNo */
    "initialize_vector_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo dg_emlrtRSI = {
    416,                      /* lineNo */
    "initialize_vector_sort", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo eg_emlrtRSI = {
    576,                /* lineNo */
    "merge_pow2_block", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo fg_emlrtRSI = {
    578,                /* lineNo */
    "merge_pow2_block", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo gg_emlrtRSI = {
    606,                /* lineNo */
    "merge_pow2_block", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo hg_emlrtRSI = {
    488,           /* lineNo */
    "merge_block", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo jg_emlrtRSI = {
    496,           /* lineNo */
    "merge_block", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo kg_emlrtRSI = {
    503,           /* lineNo */
    "merge_block", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo lg_emlrtRSI = {
    550,     /* lineNo */
    "merge", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRSInfo mg_emlrtRSI = {
    519,     /* lineNo */
    "merge", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pathName
                                                                           */
};

static emlrtRTEInfo ge_emlrtRTEI = {
    61,        /* lineNo */
    5,         /* colNo */
    "sortIdx", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pName
                                                                           */
};

static emlrtRTEInfo he_emlrtRTEI = {
    296,       /* lineNo */
    1,         /* colNo */
    "sortIdx", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pName
                                                                           */
};

static emlrtRTEInfo ie_emlrtRTEI = {
    298,       /* lineNo */
    24,        /* colNo */
    "sortIdx", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pName
                                                                           */
};

static emlrtRTEInfo je_emlrtRTEI = {
    298,       /* lineNo */
    1,         /* colNo */
    "sortIdx", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/sortIdx.m" /* pName
                                                                           */
};

/* Function Declarations */
static void merge(const emlrtStack *sp, emxArray_int32_T *idx,
                  emxArray_real_T *x, int32_T offset, int32_T np, int32_T nq,
                  emxArray_int32_T *iwork, emxArray_real_T *xwork);

static void merge_block(const emlrtStack *sp, emxArray_int32_T *idx,
                        emxArray_real_T *x, int32_T offset, int32_T n,
                        int32_T preSortLevel, emxArray_int32_T *iwork,
                        emxArray_real_T *xwork);

/* Function Definitions */
static void merge(const emlrtStack *sp, emxArray_int32_T *idx,
                  emxArray_real_T *x, int32_T offset, int32_T np, int32_T nq,
                  emxArray_int32_T *iwork, emxArray_real_T *xwork)
{
  emlrtStack b_st;
  emlrtStack st;
  real_T *x_data;
  real_T *xwork_data;
  int32_T j;
  int32_T *idx_data;
  int32_T *iwork_data;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  xwork_data = xwork->data;
  iwork_data = iwork->data;
  x_data = x->data;
  idx_data = idx->data;
  if (nq != 0) {
    int32_T iout;
    int32_T n;
    int32_T p;
    int32_T q;
    n = np + nq;
    st.site = &mg_emlrtRSI;
    if (n > 2147483646) {
      b_st.site = &cb_emlrtRSI;
      check_forloop_overflow_error(&b_st);
    }
    for (j = 0; j < n; j++) {
      q = offset + j;
      iwork_data[j] = idx_data[q];
      xwork_data[j] = x_data[q];
    }
    p = 0;
    q = np;
    iout = offset - 1;
    int32_T exitg1;
    do {
      exitg1 = 0;
      iout++;
      if (xwork_data[p] <= xwork_data[q]) {
        idx_data[iout] = iwork_data[p];
        x_data[iout] = xwork_data[p];
        if (p + 1 < np) {
          p++;
        } else {
          exitg1 = 1;
        }
      } else {
        idx_data[iout] = iwork_data[q];
        x_data[iout] = xwork_data[q];
        if (q + 1 < n) {
          q++;
        } else {
          q = iout - p;
          st.site = &lg_emlrtRSI;
          if ((p + 1 <= np) && (np > 2147483646)) {
            b_st.site = &cb_emlrtRSI;
            check_forloop_overflow_error(&b_st);
          }
          for (j = p + 1; j <= np; j++) {
            iout = q + j;
            idx_data[iout] = iwork_data[j - 1];
            x_data[iout] = xwork_data[j - 1];
          }
          exitg1 = 1;
        }
      }
    } while (exitg1 == 0);
  }
}

static void merge_block(const emlrtStack *sp, emxArray_int32_T *idx,
                        emxArray_real_T *x, int32_T offset, int32_T n,
                        int32_T preSortLevel, emxArray_int32_T *iwork,
                        emxArray_real_T *xwork)
{
  emlrtStack st;
  int32_T bLen;
  int32_T k;
  int32_T nPairs;
  st.prev = sp;
  st.tls = sp->tls;
  nPairs = n >> preSortLevel;
  bLen = 1 << preSortLevel;
  while (nPairs > 1) {
    int32_T tailOffset;
    if (((uint32_T)nPairs & 1U) != 0U) {
      int32_T nTail;
      nPairs--;
      tailOffset = bLen * nPairs;
      nTail = n - tailOffset;
      if (nTail > bLen) {
        st.site = &hg_emlrtRSI;
        merge(&st, idx, x, offset + tailOffset, bLen, nTail - bLen, iwork,
              xwork);
      }
    }
    tailOffset = bLen << 1;
    nPairs >>= 1;
    for (k = 0; k < nPairs; k++) {
      st.site = &jg_emlrtRSI;
      merge(&st, idx, x, offset + k * tailOffset, bLen, bLen, iwork, xwork);
    }
    bLen = tailOffset;
  }
  if (n > bLen) {
    st.site = &kg_emlrtRSI;
    merge(&st, idx, x, offset, bLen, n - bLen, iwork, xwork);
  }
}

void sortIdx(const emlrtStack *sp, emxArray_real_T *x, emxArray_int32_T *idx)
{
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
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
  int32_T quartetOffset;
  int32_T *idx_data;
  int32_T *iwork_data;
  uint32_T unnamed_idx_0;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  x_data = x->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  unnamed_idx_0 = (uint32_T)x->size[0];
  i1 = idx->size[0];
  idx->size[0] = (int32_T)unnamed_idx_0;
  emxEnsureCapacity_int32_T(sp, idx, i1, &ge_emlrtRTEI);
  idx_data = idx->data;
  quartetOffset = (int32_T)unnamed_idx_0;
  for (k = 0; k < quartetOffset; k++) {
    idx_data[k] = 0;
  }
  if (x->size[0] != 0) {
    real_T x4[4];
    int32_T idx4[4];
    int32_T bLen;
    int32_T bLen2;
    int32_T i2;
    int32_T i4;
    int32_T ib;
    int32_T n;
    int32_T nBlocks;
    int32_T wOffset;
    st.site = &uf_emlrtRSI;
    emxInit_int32_T(&st, &iwork, 1, &he_emlrtRTEI);
    i1 = iwork->size[0];
    iwork->size[0] = (int32_T)unnamed_idx_0;
    emxEnsureCapacity_int32_T(&st, iwork, i1, &he_emlrtRTEI);
    iwork_data = iwork->data;
    for (k = 0; k < quartetOffset; k++) {
      iwork_data[k] = 0;
    }
    emxInit_real_T(&st, &xwork, 1, &je_emlrtRTEI);
    i1 = xwork->size[0];
    xwork->size[0] = (int32_T)unnamed_idx_0;
    emxEnsureCapacity_real_T(&st, xwork, i1, &ie_emlrtRTEI);
    xwork_data = xwork->data;
    b_st.site = &vf_emlrtRSI;
    n = x->size[0];
    x4[0] = 0.0;
    idx4[0] = 0;
    x4[1] = 0.0;
    idx4[1] = 0;
    x4[2] = 0.0;
    idx4[2] = 0;
    x4[3] = 0.0;
    idx4[3] = 0;
    nBlocks = 0;
    ib = 0;
    c_st.site = &bg_emlrtRSI;
    if (x->size[0] > 2147483646) {
      d_st.site = &cb_emlrtRSI;
      check_forloop_overflow_error(&d_st);
    }
    for (k = 0; k < n; k++) {
      if (muDoubleScalarIsNaN(x_data[k])) {
        i1 = (n - nBlocks) - 1;
        idx_data[i1] = k + 1;
        xwork_data[i1] = x_data[k];
        nBlocks++;
      } else {
        ib++;
        idx4[ib - 1] = k + 1;
        x4[ib - 1] = x_data[k];
        if (ib == 4) {
          real_T d;
          real_T d1;
          quartetOffset = k - nBlocks;
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
    wOffset = x->size[0] - nBlocks;
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
      c_st.site = &cg_emlrtRSI;
      if (ib > 2147483646) {
        d_st.site = &cb_emlrtRSI;
        check_forloop_overflow_error(&d_st);
      }
      i1 = (uint8_T)ib;
      for (k = 0; k < i1; k++) {
        quartetOffset = (wOffset - ib) + k;
        i2 = perm[k];
        idx_data[quartetOffset] = idx4[i2 - 1];
        x_data[quartetOffset] = x4[i2 - 1];
      }
    }
    i1 = nBlocks >> 1;
    c_st.site = &dg_emlrtRSI;
    for (k = 0; k < i1; k++) {
      quartetOffset = wOffset + k;
      i2 = idx_data[quartetOffset];
      ib = (n - k) - 1;
      idx_data[quartetOffset] = idx_data[ib];
      idx_data[ib] = i2;
      x_data[quartetOffset] = xwork_data[ib];
      x_data[ib] = xwork_data[quartetOffset];
    }
    if (((uint32_T)nBlocks & 1U) != 0U) {
      i1 += wOffset;
      x_data[i1] = xwork_data[i1];
    }
    i1 = 2;
    if (wOffset > 1) {
      if (x->size[0] >= 256) {
        nBlocks = wOffset >> 8;
        if (nBlocks > 0) {
          b_st.site = &wf_emlrtRSI;
          for (b = 0; b < nBlocks; b++) {
            real_T b_xwork[256];
            int32_T b_iwork[256];
            b_st.site = &xf_emlrtRSI;
            i4 = (b << 8) - 1;
            for (b_b = 0; b_b < 6; b_b++) {
              bLen = 1 << (b_b + 2);
              bLen2 = bLen << 1;
              n = 256 >> (b_b + 3);
              c_st.site = &eg_emlrtRSI;
              for (b_k = 0; b_k < n; b_k++) {
                i1 = (i4 + b_k * bLen2) + 1;
                c_st.site = &fg_emlrtRSI;
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
                      c_st.site = &gg_emlrtRSI;
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
            b_st.site = &yf_emlrtRSI;
            merge_block(&b_st, idx, x, i1, quartetOffset, 2, iwork, xwork);
          }
          i1 = 8;
        }
      }
      b_st.site = &ag_emlrtRSI;
      merge_block(&b_st, idx, x, 0, wOffset, i1, iwork, xwork);
    }
    emxFree_real_T(&st, &xwork);
    emxFree_int32_T(&st, &iwork);
  }
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (sortIdx.c) */
