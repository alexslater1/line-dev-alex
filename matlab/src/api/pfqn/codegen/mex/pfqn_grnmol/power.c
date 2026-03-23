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
#include "eml_int_forloop_overflow_check.h"
#include "ixfun.h"
#include "pfqn_grnmol_data.h"
#include "pfqn_grnmol_emxutil.h"
#include "pfqn_grnmol_types.h"
#include "rt_nonfinite.h"
#include "mwmathutil.h"
#include "omp.h"

/* Variable Definitions */
static emlrtRSInfo hb_emlrtRSI = {
    81,                                                           /* lineNo */
    "fltpower",                                                   /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

static emlrtRSInfo ib_emlrtRSI = {
    94,                                                           /* lineNo */
    "fltpower",                                                   /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

static emlrtRSInfo jb_emlrtRSI = {
    106,                                                          /* lineNo */
    "fltpower_domain_error",                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

static emlrtRSInfo kb_emlrtRSI = {
    113,                                                          /* lineNo */
    "fltpower_domain_error",                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

static emlrtRSInfo lb_emlrtRSI = {
    123,                                                          /* lineNo */
    "fltpower_domain_error",                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pathName */
};

static emlrtRSInfo
    mb_emlrtRSI =
        {
            45,          /* lineNo */
            "vAllOrAny", /* fcnName */
            "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
            "vAllOrAny.m" /* pathName */
};

static emlrtRSInfo
    nb_emlrtRSI =
        {
            121,                  /* lineNo */
            "flatVectorAllOrAny", /* fcnName */
            "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
            "vAllOrAny.m" /* pathName */
};

static emlrtRSInfo ob_emlrtRSI = {
    65,      /* lineNo */
    "ixfun", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/ixfun.m" /* pathName
                                                                         */
};

static emlrtRSInfo pb_emlrtRSI = {
    110,                          /* lineNo */
    "binaryImplicitExpansionFun", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/ixfun.m" /* pathName
                                                                         */
};

static emlrtRSInfo qb_emlrtRSI = {
    16,                                                         /* lineNo */
    "any",                                                      /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/any.m" /* pathName */
};

static emlrtRSInfo rb_emlrtRSI = {
    57,         /* lineNo */
    "allOrAny", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/allOrAny.m" /* pathName
                                                                            */
};

static emlrtRSInfo sb_emlrtRSI = {
    106,                   /* lineNo */
    "applyToMultipleDims", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "applyToMultipleDims.m" /* pathName */
};

static emlrtRSInfo tb_emlrtRSI = {
    63,                               /* lineNo */
    "function_handle/parenReference", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/"
    "function_handle.m" /* pathName */
};

static emlrtRSInfo ub_emlrtRSI = {
    56,                                                             /* lineNo */
    "@(x)coder.internal.allOrAny(op,x,coder.internal.indexInt(1))", /* fcnName
                                                                     */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/allOrAny.m" /* pathName
                                                                            */
};

static emlrtRSInfo vb_emlrtRSI = {
    143,        /* lineNo */
    "allOrAny", /* fcnName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/allOrAny.m" /* pathName
                                                                            */
};

static emlrtRTEInfo kb_emlrtRTEI = {
    71,                                                           /* lineNo */
    5,                                                            /* colNo */
    "power",                                                      /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pName */
};

static emlrtRTEInfo lb_emlrtRTEI = {
    65,      /* lineNo */
    5,       /* colNo */
    "ixfun", /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/eml/+coder/+internal/ixfun.m" /* pName
                                                                         */
};

static emlrtRTEInfo mb_emlrtRTEI = {
    123,                                                          /* lineNo */
    13,                                                           /* colNo */
    "power",                                                      /* fName */
    "/usr/local/MATLAB/R2025a/toolbox/eml/lib/matlab/ops/power.m" /* pName */
};

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
  emlrtStack h_st;
  emlrtStack i_st;
  emlrtStack j_st;
  emlrtStack st;
  emxArray_boolean_T *c;
  const real_T *a_data;
  const real_T *b_data;
  real_T b_b;
  real_T b_varargin_1;
  real_T b_varargin_2;
  real_T c_varargin_1;
  real_T c_varargin_2;
  real_T d_varargin_2;
  real_T *y_data;
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
  h_st.prev = &g_st;
  h_st.tls = g_st.tls;
  i_st.prev = &h_st;
  i_st.tls = h_st.tls;
  j_st.prev = &i_st;
  j_st.tls = i_st.tls;
  b_data = b->data;
  a_data = a->data;
  emlrtHeapReferenceStackEnterFcnR2012b((emlrtConstCTX)sp);
  st.site = &u_emlrtRSI;
  b_st.site = &hb_emlrtRSI;
  if (a->size[1] == 1) {
    if (a_data[0] < 0.0) {
      c_st.site = &jb_emlrtRSI;
      d_st.site = &mb_emlrtRSI;
      nx = b->size[1];
      p = false;
      e_st.site = &nb_emlrtRSI;
      if (b->size[1] > 2147483646) {
        f_st.site = &s_emlrtRSI;
        check_forloop_overflow_error(&f_st);
      }
      for (k = 0; k < nx; k++) {
        if (p) {
          p = true;
        } else {
          b_b = b_data[k];
          if ((!muDoubleScalarIsNaN(b_b)) &&
              (muDoubleScalarFloor(b_b) != b_b)) {
            p = true;
          }
        }
      }
    } else {
      p = false;
    }
  } else if (b->size[1] == 1) {
    if ((!muDoubleScalarIsNaN(b_data[0])) &&
        (muDoubleScalarFloor(b_data[0]) != b_data[0])) {
      c_st.site = &kb_emlrtRSI;
      d_st.site = &mb_emlrtRSI;
      nx = a->size[1];
      p = false;
      e_st.site = &nb_emlrtRSI;
      if (a->size[1] > 2147483646) {
        f_st.site = &s_emlrtRSI;
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
          b_b = b_data[k];
          if ((!muDoubleScalarIsNaN(b_b)) &&
              (muDoubleScalarFloor(b_b) != b_b)) {
            p = true;
          }
        }
      }
    } else {
      emxInit_boolean_T(&b_st, &c, &mb_emlrtRTEI);
      c_st.site = &lb_emlrtRSI;
      d_st.site = &ob_emlrtRSI;
      if (a->size[1] == b->size[1]) {
        nx = c->size[0] * c->size[1];
        c->size[0] = 1;
        loop_ub = a->size[1];
        c->size[1] = a->size[1];
        emxEnsureCapacity_boolean_T(&d_st, c, nx, &lb_emlrtRTEI);
        c_data = c->data;
        nx = a->size[1];
        if (a->size[1] < 1600) {
          for (i1 = 0; i1 < loop_ub; i1++) {
            b_b = a_data[i1];
            b_varargin_2 = b_data[i1];
            if ((b_b < 0.0) && (!muDoubleScalarIsNaN(b_varargin_2)) &&
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
        c_varargin_1, d_varargin_2, varargout_1)

          for (i1 = 0; i1 < nx; i1++) {
            c_varargin_1 = a_data[i1];
            d_varargin_2 = b_data[i1];
            if ((c_varargin_1 < 0.0) && (!muDoubleScalarIsNaN(d_varargin_2)) &&
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
        e_st.site = &pb_emlrtRSI;
        expand_fltpower_domain_error(&e_st, a, b, c);
        c_data = c->data;
      }
      c_st.site = &lb_emlrtRSI;
      d_st.site = &qb_emlrtRSI;
      e_st.site = &rb_emlrtRSI;
      f_st.site = &sb_emlrtRSI;
      g_st.site = &tb_emlrtRSI;
      h_st.site = &ub_emlrtRSI;
      p = false;
      i_st.site = &vb_emlrtRSI;
      if (c->size[1] > 2147483646) {
        j_st.site = &s_emlrtRSI;
        check_forloop_overflow_error(&j_st);
      }
      nx = 1;
      exitg1 = false;
      while ((!exitg1) && (nx <= c->size[1])) {
        if (c_data[nx - 1]) {
          p = true;
          exitg1 = true;
        } else {
          nx++;
        }
      }
      emxFree_boolean_T(&h_st, &c);
    }
  }
  if (p) {
    emlrtErrorWithMessageIdR2018a(&st, &d_emlrtRTEI,
                                  "Coder:toolbox:power_domainError",
                                  "Coder:toolbox:power_domainError", 0);
  }
  b_st.site = &ib_emlrtRSI;
  c_st.site = &ob_emlrtRSI;
  if (a->size[1] == b->size[1]) {
    nx = y->size[0] * y->size[1];
    y->size[0] = 1;
    loop_ub = a->size[1];
    y->size[1] = a->size[1];
    emxEnsureCapacity_real_T(&c_st, y, nx, &kb_emlrtRTEI);
    y_data = y->data;
    nx = a->size[1];
    if (a->size[1] < 1600) {
      for (i = 0; i < loop_ub; i++) {
        b_b = a_data[i];
        b_varargin_2 = b_data[i];
        y_data[i] = muDoubleScalarPower(b_b, b_varargin_2);
      }
    } else {
      emlrtEnterParallelRegion(&c_st, omp_in_parallel());
      emlrtPushJmpBuf(&c_st, &emlrtJBStack);
      power_numThreads =
          emlrtAllocRegionTLSs(c_st.tls, omp_in_parallel(),
                               omp_get_max_threads(), omp_get_num_procs());
#pragma omp parallel for num_threads(power_numThreads) private(                \
        b_varargin_1, c_varargin_2)

      for (i = 0; i < nx; i++) {
        b_varargin_1 = a_data[i];
        c_varargin_2 = b_data[i];
        y_data[i] = muDoubleScalarPower(b_varargin_1, c_varargin_2);
      }
      emlrtPopJmpBuf(&c_st, &emlrtJBStack);
      emlrtExitParallelRegion(&c_st, omp_in_parallel());
    }
  } else {
    d_st.site = &pb_emlrtRSI;
    expand_power(&d_st, a, b, y);
  }
  emlrtHeapReferenceStackLeaveFcnR2012b((emlrtConstCTX)sp);
}

/* End of code generation (power.c) */
