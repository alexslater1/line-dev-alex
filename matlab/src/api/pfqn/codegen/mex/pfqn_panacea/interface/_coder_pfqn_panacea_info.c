/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_pfqn_panacea_info.c
 *
 * Code generation for function 'pfqn_panacea'
 *
 */

/* Include files */
#include "_coder_pfqn_panacea_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

/* Function Declarations */
static const mxArray *c_emlrtMexFcnResolvedFunctionsI(void);

/* Function Definitions */
static const mxArray *c_emlrtMexFcnResolvedFunctionsI(void)
{
  const mxArray *nameCaptureInfo;
  const char_T *data[6] = {
      "789ced96cd6ed34010c737a87c1c5ac8854a483dc0b512ddd0f2d11b524a430a24051a68"
      "2584e866bda9ddee7ab7f626b8f0103c021ce9adcfc39d43fb0e209a"
      "d84eec955609b8da08cb73998cfecefe26b3f15f034a1b8d1200e03a08a3321fe6b9a82e"
      "47f9124887aa97a23ca3d4715c8e943945ff1265cc5d490219162e62",
      "64f84d8b33c745ae6c1d09023ce273da23d640e93894b41c46b69245b35fb15a421a167d"
      "a9ff79cd26f860abcb8067fba30e69b218ce6357f37b67403a545d0d"
      "751eea73314ffc232f3e7f760c2fd645e7d0fd20908b304149fe6e46fe152d3f542cde6d"
      "5332e29d64e435b4bcb4fe6efd3db43923700f231f51029f785cb479",
      "0031b708a48e4bee5aa4b7b4e748c890a4a80d7d0f43241cd89f144c8e6b8981f1f3ba31"
      "61ff6a1e3d7f6d906bbfbf3ac0206ff1b39835c98b635abc4073dea4"
      "ffbf9b1a5e59d10f6c8fd37bd59d5ed0aebc59b94f1bfe8b6d3bd1c7cb319c717d004d6d"
      "eafcbcbec717e57b1d842575a7e7b3df32f21e6b7969fdefefa72b1d",
      "0ac3f19cdf88291f38fdc98cfaead98f3bb74cf2e2f85f7d755ec32b2bfaa3d6d3daf3cd"
      "95c30df9fa81f7a9beee6d37abcbd5c257f3e2ab57b5fd87ca008cd1"
      "d47cf57b465e5dcb4beb17713ff8fc6a4cf9c1ed5f66f7d60571fcd0242f8ebcfbeb4ea5"
      "bbba1c30f98a7e7cb6d9dc5fabbf15d8abe5c75ff3f6fe16fb6a18c5",
      "be9a8d57ecab6114fbea64e7ff01a1df4970",
      ""};
  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&data[0], 5704U, &nameCaptureInfo);
  return nameCaptureInfo;
}

mxArray *emlrtMexFcnProperties(void)
{
  mxArray *xEntryPoints;
  mxArray *xInputs;
  mxArray *xResult;
  const char_T *epFieldName[7] = {
      "QualifiedName",    "NumberOfInputs", "NumberOfOutputs", "ConstantInputs",
      "ResolvedFilePath", "TimeStamp",      "Visible"};
  const char_T *propFieldName[7] = {
      "Version",      "ResolvedFunctions", "Checksum", "EntryPoints",
      "CoverageInfo", "IsPolymorphic",     "AuxData"};
  uint8_T v[216] = {
      0U,   1U,   73U,  77U,  0U,   0U,   0U,   0U,   14U,  0U,   0U,   0U,
      200U, 0U,   0U,   0U,   6U,   0U,   0U,   0U,   8U,   0U,   0U,   0U,
      2U,   0U,   0U,   0U,   0U,   0U,   0U,   0U,   5U,   0U,   0U,   0U,
      8U,   0U,   0U,   0U,   1U,   0U,   0U,   0U,   1U,   0U,   0U,   0U,
      1U,   0U,   0U,   0U,   0U,   0U,   0U,   0U,   5U,   0U,   4U,   0U,
      17U,  0U,   0U,   0U,   1U,   0U,   0U,   0U,   17U,  0U,   0U,   0U,
      67U,  108U, 97U,  115U, 115U, 69U,  110U, 116U, 114U, 121U, 80U,  111U,
      105U, 110U, 116U, 115U, 0U,   0U,   0U,   0U,   0U,   0U,   0U,   0U,
      14U,  0U,   0U,   0U,   112U, 0U,   0U,   0U,   6U,   0U,   0U,   0U,
      8U,   0U,   0U,   0U,   2U,   0U,   0U,   0U,   0U,   0U,   0U,   0U,
      5U,   0U,   0U,   0U,   8U,   0U,   0U,   0U,   1U,   0U,   0U,   0U,
      0U,   0U,   0U,   0U,   1U,   0U,   0U,   0U,   0U,   0U,   0U,   0U,
      5U,   0U,   4U,   0U,   14U,  0U,   0U,   0U,   1U,   0U,   0U,   0U,
      56U,  0U,   0U,   0U,   81U,  117U, 97U,  108U, 105U, 102U, 105U, 101U,
      100U, 78U,  97U,  109U, 101U, 0U,   77U,  101U, 116U, 104U, 111U, 100U,
      115U, 0U,   0U,   0U,   0U,   0U,   0U,   0U,   80U,  114U, 111U, 112U,
      101U, 114U, 116U, 105U, 101U, 115U, 0U,   0U,   0U,   0U,   72U,  97U,
      110U, 100U, 108U, 101U, 0U,   0U,   0U,   0U,   0U,   0U,   0U,   0U};
  xEntryPoints =
      emlrtCreateStructMatrix(1, 1, 7, (const char_T **)&epFieldName[0]);
  xInputs = emlrtCreateLogicalMatrix(1, 3);
  emlrtSetField(xEntryPoints, 0, "QualifiedName",
                emlrtMxCreateString("pfqn_panacea"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(3.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(2.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(xEntryPoints, 0, "ResolvedFilePath",
                emlrtMxCreateString("/home/gcasale/Dropbox/code/line-dev.git/"
                                    "matlab/src/api/pfqn/pfqn_panacea.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(740039.37729166669));
  emlrtSetField(xEntryPoints, 0, "Visible", emlrtMxCreateLogicalScalar(true));
  xResult =
      emlrtCreateStructMatrix(1, 1, 7, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("25.1.0.2973910 (R2025a) Update 1"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)c_emlrtMexFcnResolvedFunctionsI());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("Tjdc7HaeXsfj9h3e50C78E"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  emlrtSetField(xResult, 0, "AuxData",
                emlrtMxCreateRowVectorUINT8((const uint8_T *)&v, 216U));
  return xResult;
}

/* End of code generation (_coder_pfqn_panacea_info.c) */
