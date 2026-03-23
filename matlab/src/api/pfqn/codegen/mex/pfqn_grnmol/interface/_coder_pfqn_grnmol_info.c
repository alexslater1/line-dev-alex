/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_pfqn_grnmol_info.c
 *
 * Code generation for function 'pfqn_grnmol'
 *
 */

/* Include files */
#include "_coder_pfqn_grnmol_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

/* Function Declarations */
static const mxArray *c_emlrtMexFcnResolvedFunctionsI(void);

/* Function Definitions */
static const mxArray *c_emlrtMexFcnResolvedFunctionsI(void)
{
  const mxArray *nameCaptureInfo;
  const char_T *data[7] = {
      "789ced99cd6ed34010c71d94544808c80190ca8907806e2a9272436a130a56a04d894925"
      "1010c75ec76e76bdc61f4d72e115780472ec8103521f0921c1892be2"
      "841cdb716c697152876d6b792e9bd178f73799cdfeb59e7005fe4581e3b81b9c673fee79"
      "e375df2ffbe3152e6af178c11fd7627e6025ae189917c43ff9a34474",
      "1b8e6ccfd1450c67336582355dd46d616c40ce841641c7509e46140d4141c3b03defecb9"
      "1ede9d0bcd1c37e47eaeab501ab41dcc99aa156688e69d593dba94ef"
      "5b4ca847dce2f5883f17f08c33f282f5af25f082b8a17cd0dff74d1d1314e17753f269fb"
      "5ff62332717a0886bcaf2979cfa9bc68fccd93b740251882be245a22",
      "82a06112a347464022320448d3e103191e6ff4351b60d146620f58a6044443036ea5c05c"
      "b936b0bb5e52bd6e2e987f7c0c9fbf3a1d3fde9a682c7952a7799f25"
      "2fb0f3e28d28eb2dfafbbb43e19563f16a6bc01fbedc47037b283586e3d7a303c526f530"
      "8f560227290f8ee2b35a3fabe778d1fc4bd4fcbd886598445e216f59",
      "9d9da4e43da6f2a2f1e5f7c7b13504a6e5717784950e3c2a9d32d5d5bbbf8e3659f202cb"
      "baaeeeb478b5d239da3ee821a123d4f8577b8add7c961d5d9d50e65f"
      "d473cbeafe8a1d646b924a88e5bd299c97ae9ea4e435a8bc68fc8cfb335726b6f7d62f7f"
      "3e33d5d793d3b5db2c7981655d5f37ab9abcb5ef40d9ec48a836e65b",
      "75fdb0b6931d7dbdace737d7d9e578b9ceae8697ebac67b9ce2eb7fe8432ffa2de63bb29"
      "f34dea0718793fe05ffb61b0ee07fcfc8e99eae8ef6fefd659f202cb"
      "ba8eee56b63aed71b3397ca836856abbd2de56c6f069767434efb3d2f2ff3f7dd662cc0f"
      "795e448268fa07da25d1d5bccfba625e6059d7d5acf659ff0249efd9",
      "d3",
      ""};
  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&data[0], 8448U, &nameCaptureInfo);
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
  xInputs = emlrtCreateLogicalMatrix(1, 2);
  emlrtSetField(xEntryPoints, 0, "QualifiedName",
                emlrtMxCreateString("pfqn_grnmol"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(2.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(xEntryPoints, 0, "ResolvedFilePath",
                emlrtMxCreateString("/home/gcasale/Dropbox/code/line-dev.git/"
                                    "matlab/src/api/pfqn/pfqn_grnmol.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(740039.443125));
  emlrtSetField(xEntryPoints, 0, "Visible", emlrtMxCreateLogicalScalar(true));
  xResult =
      emlrtCreateStructMatrix(1, 1, 7, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("25.1.0.2973910 (R2025a) Update 1"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)c_emlrtMexFcnResolvedFunctionsI());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("bZsGsndgHJdtfNRP6jo6FD"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  emlrtSetField(xResult, 0, "AuxData",
                emlrtMxCreateRowVectorUINT8((const uint8_T *)&v, 216U));
  return xResult;
}

/* End of code generation (_coder_pfqn_grnmol_info.c) */
