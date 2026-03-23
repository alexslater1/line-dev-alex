/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * power.c
 *
 * Code generation for function 'power'
 *
 */

/* Include files */
#include "power.h"
#include "allOrAny.h"
#include "eml_int_forloop_overflow_check.h"
#include "ixfun.h"
#include "pfqn_mmint2_data.h"
#include "pfqn_mmint2_emxutil.h"
#include "pfqn_mmint2_types.h"
#include "rt_nonfinite.h"
#include "vAllOrAny.h"
#include "mwmathutil.h"
#include "omp.h"

/* Function Definitions */
void power(const emlrtStack *sp, const emxArray_real_T *a,
           const emxArray_real_T *b, emxArray_real_T *y)
{
  jmp_buf *volatile emlrtJBStack;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  emlrtStack f_st;
  emlrtStack g_st;
  emlrtStack st;
  emxArray_boolean_T b_c;
  emxArray_boolean_T *c;
  const real_T *a_data;
  const real_T *b_data;
  real_T b_varargin_1;
  real_T b_varargin_2;
  real_T c_varargin_1;
  real_T c_varargin_2;
  real_T d_varargin_1;
  real_T d_varargin_2;
  real_T *y_data;
  int32_T c_c;
  int32_T i;
  int32_T i1;
  int32_T k;
  int32_T loop_ub;
  int32_T nx;
  int32_T power_numThreads;
  boolean_T p;
  boolean_T varargout_1;
  boolean_T *c_data;
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
  f_st.prev = &e_st;
  f_st.tls = e_st.tls;
  g_st.prev = &f_st;
  g_st.tls = f_st.tls;
  b_data = b->data;
  a_data = a->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  st.site = &v_emlrtRSI;
  b_st.site = &w_emlrtRSI;
  if (a->size[1] == 1) {
    if (a_data[0] < 0.0) {
      c_st.site = &y_emlrtRSI;
      p = vAllOrAny(&c_st, b);
    } else {
      p = false;
    }
  } else if (b->size[1] == 1) {
    if ((!muDoubleScalarIsNaN(b_data[0])) &&
        (muDoubleScalarFloor(b_data[0]) != b_data[0])) {
      c_st.site = &ab_emlrtRSI;
      d_st.site = &cb_emlrtRSI;
      nx = a->size[1];
      p = false;
      e_st.site = &db_emlrtRSI;
      if (a->size[1] > 2147483646) {
        f_st.site = &j_emlrtRSI;
        check_forloop_overflow_error(&f_st);
      }
      for (k = 0; k < nx; k++) {
        if (p || (a_data[k] < 0.0)) {
          p = true;
        }
      }
    } else {
      p = false;
    }
  } else {
    uint32_T varargin_1[2];
    uint32_T varargin_2[2];
    boolean_T exitg1;
    varargin_1[0] = 1U;
    varargin_2[0] = 1U;
    varargin_1[1] = (uint32_T)a->size[1];
    varargin_2[1] = (uint32_T)b->size[1];
    p = true;
    nx = 0;
    exitg1 = false;
    while ((!exitg1) && (nx < 2)) {
      if ((int32_T)varargin_1[nx] != (int32_T)varargin_2[nx]) {
        p = false;
        exitg1 = true;
      } else {
        nx++;
      }
    }
    if (p) {
      p = false;
      nx = a->size[1];
      for (k = 0; k < nx; k++) {
        if (p) {
          p = true;
        } else if (a_data[k] < 0.0) {
          b_varargin_1 = b_data[k];
          if ((!muDoubleScalarIsNaN(b_varargin_1)) &&
              (muDoubleScalarFloor(b_varargin_1) != b_varargin_1)) {
            p = true;
          }
        }
      }
    } else {
      emxInit_boolean_T(&b_st, &c, &db_emlrtRTEI);
      c_st.site = &bb_emlrtRSI;
      d_st.site = &eb_emlrtRSI;
      if (a->size[1] == b->size[1]) {
        nx = c->size[0] * c->size[1];
        c->size[0] = 1;
        loop_ub = a->size[1];
        c->size[1] = a->size[1];
        emxEnsureCapacity_boolean_T(&d_st, c, nx, &cb_emlrtRTEI);
        c_data = c->data;
        nx = a->size[1];
        if (a->size[1] < 1600) {
          for (i1 = 0; i1 < loop_ub; i1++) {
            b_varargin_1 = a_data[i1];
            b_varargin_2 = b_data[i1];
            if ((b_varargin_1 < 0.0) && (!muDoubleScalarIsNaN(b_varargin_2)) &&
                (muDoubleScalarFloor(b_varargin_2) != b_varargin_2)) {
              p = true;
            } else {
              p = false;
            }
            c_data[i1] = p;
          }
        } else {
          emlrtEnterParallelRegion(&d_st, omp_in_parallel());
          emlrtPushJmpBuf(&d_st, &emlrtJBStack);
          power_numThreads =
              emlrtAllocRegionTLSs(d_st.tls, omp_in_parallel(),
                                   omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(power_numThreads) private(                \
        d_varargin_1, d_varargin_2, varargout_1)

          for (i1 = 0; i1 < nx; i1++) {
            d_varargin_1 = a_data[i1];
            d_varargin_2 = b_data[i1];
            if ((d_varargin_1 < 0.0) && (!muDoubleScalarIsNaN(d_varargin_2)) &&
                (muDoubleScalarFloor(d_varargin_2) != d_varargin_2)) {
              varargout_1 = true;
            } else {
              varargout_1 = false;
            }
            c_data[i1] = varargout_1;
          }
          emlrtPopJmpBuf(&d_st, &emlrtJBStack);
          emlrtExitParallelRegion(&d_st, omp_in_parallel());
        }
      } else {
        e_st.site = &fb_emlrtRSI;
        expand_fltpower_domain_error(&e_st, a, b, c);
      }
      c_st.site = &bb_emlrtRSI;
      d_st.site = &gb_emlrtRSI;
      e_st.site = &hb_emlrtRSI;
      f_st.site = &ib_emlrtRSI;
      b_c = *c;
      c_c = c->size[1];
      b_c.size = &c_c;
      b_c.numDimensions = 1;
      g_st.site = &r_emlrtRSI;
      p = allOrAny_anonFcn1(&g_st, &b_c);
      emxFree_boolean_T(&f_st, &c);
    }
  }
  if (p) {
    emlrtErrorWithMessageIdR2018a(&st, &e_emlrtRTEI,
                                  "Coder:toolbox:power_domainError",
                                  "Coder:toolbox:power_domainError", 0);
  }
  b_st.site = &x_emlrtRSI;
  c_st.site = &eb_emlrtRSI;
  if (a->size[1] == b->size[1]) {
    nx = y->size[0] * y->size[1];
    y->size[0] = 1;
    loop_ub = a->size[1];
    y->size[1] = a->size[1];
    emxEnsureCapacity_real_T(&c_st, y, nx, &bb_emlrtRTEI);
    y_data = y->data;
    nx = a->size[1];
    if (a->size[1] < 1600) {
      for (i = 0; i < loop_ub; i++) {
        b_varargin_1 = a_data[i];
        b_varargin_2 = b_data[i];
        y_data[i] = muDoubleScalarPower(b_varargin_1, b_varargin_2);
      }
    } else {
      emlrtEnterParallelRegion(&c_st, omp_in_parallel());
      emlrtPushJmpBuf(&c_st, &emlrtJBStack);
      power_numThreads =
          emlrtAllocRegionTLSs(c_st.tls, omp_in_parallel(),
                               omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(power_numThreads) private(                \
        c_varargin_1, c_varargin_2)

      for (i = 0; i < nx; i++) {
        c_varargin_1 = a_data[i];
        c_varargin_2 = b_data[i];
        y_data[i] = muDoubleScalarPower(c_varargin_1, c_varargin_2);
      }
      emlrtPopJmpBuf(&c_st, &emlrtJBStack);
      emlrtExitParallelRegion(&c_st, omp_in_parallel());
    }
  } else {
    d_st.site = &fb_emlrtRSI;
    expand_power(&d_st, a, b, y);
  }
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (power.c) */
