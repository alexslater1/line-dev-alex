/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_pfqn_mvald_info.c
 *
 * Code generation for function 'pfqn_mvald'
 *
 */

/* Include files */
#include "_coder_pfqn_mvald_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

/* Function Declarations */
static const mxArray *c_emlrtMexFcnResolvedFunctionsI(void);

/* Function Definitions */
static const mxArray *c_emlrtMexFcnResolvedFunctionsI(void)
{
  const mxArray *nameCaptureInfo;
  const char_T *data[6] = {
      "789ced98cb6ed340148627558b10ea25ab4a3c049d48ad4a59800414a7d4bd2039b44a11"
      "34137b1c5b9df1388e93382c7901d64848c00e890d42e249d8b0472c"
      "7803c40a7c8b2fd2c8691c19b072362747bf33dfc939f62f39a0f2f0b0020058057eecad"
      "fb7925a8ab415e00c948eb95202fa6ea3096026525a5bf0cb2cc0c1b",
      "3bb65f1888e2f13715467503197663646260e11e2303ac788aaa13dcd02996e2c5915b51"
      "21268d0b57723fdfd7b07c21f529b0b45ed4218917e379b438bf7711"
      "2423ada7233d8ff47521cf9c92179e7f2d8317eaa6da35cee9001125c96fe5e45fe1f27d"
      "4561fd36c111ef634e9ec8e525f5270f9e428d510c3b32ea2182e1ae",
      "c5cc3673a0cc140c896ee01b0a1e6c74741b526413d4863d4b86c8d4a13b29188d6b837a"
      "e765cd6b6dc2fed339bafeaa979f2b9ff422790b5fbeae16c90be36f"
      "f11cce7993de7feb1c5e35a53787b663697bb4a31167e7a84b8885885a8ffa7894c1c9ea"
      "0370eaa2ce2feb733c69ff4bdcfe7dc5342da6cc9077599f7d9b9377",
      "87cb4bea97df4fdfd609f4c6e3aea4281ff8f19d16eaab3fbf3dbb5e242f8cb2fbaa50db"
      "3e9146a238dcd4c4c6965493eeaa233cf7d5d2f82aef7da21a28ccc0"
      "d62c7945fbea6d2e2fa94fe9abee78bc8d94d5573fbf3b9efb2a98bdafd68ef76f36e929"
      "dd1fc86dedb4abed5ae6f6e37b735ffd577c35efff04cb19fd87badb",
      "e1f91059866e74e2fc564e7e96afcb1a4af8fafb9cbc3a9797d4a7db97ce607c507f7655"
      "943fbcfef5a650bf7df1e1d55a91bc30caeeb7ddcd9383ade6ced950"
      "a8a3c6ad3341540febb2f0fffbed6f72074997",
      ""};
  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&data[0], 5728U, &nameCaptureInfo);
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
  xInputs = emlrtCreateLogicalMatrix(1, 5);
  emlrtSetField(xEntryPoints, 0, "QualifiedName",
                emlrtMxCreateString("pfqn_mvald"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(5.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(7.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(xEntryPoints, 0, "ResolvedFilePath",
                emlrtMxCreateString("/home/gcasale/Dropbox/code/line-dev.git/"
                                    "matlab/src/api/pfqn/pfqn_mvald.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(740053.33344907407));
  emlrtSetField(xEntryPoints, 0, "Visible", emlrtMxCreateLogicalScalar(true));
  xResult =
      emlrtCreateStructMatrix(1, 1, 7, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("25.1.0.2973910 (R2025a) Update 1"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)c_emlrtMexFcnResolvedFunctionsI());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("b3ja8fSeyCsZcPeUUSEfB"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  emlrtSetField(xResult, 0, "AuxData",
                emlrtMxCreateRowVectorUINT8((const uint8_T *)&v, 216U));
  return xResult;
}

/* End of code generation (_coder_pfqn_mvald_info.c) */
