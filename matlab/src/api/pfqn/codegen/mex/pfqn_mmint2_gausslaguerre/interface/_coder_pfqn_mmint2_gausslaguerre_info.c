/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_pfqn_mmint2_gausslaguerre_info.c
 *
 * Code generation for function 'pfqn_mmint2_gausslaguerre'
 *
 */

/* Include files */
#include "_coder_pfqn_mmint2_gausslaguerre_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

/* Function Declarations */
static const mxArray *c_emlrtMexFcnResolvedFunctionsI(void);

/* Function Definitions */
static const mxArray *c_emlrtMexFcnResolvedFunctionsI(void)
{
  const mxArray *nameCaptureInfo;
  const char_T *data[6] = {
      "789ced984d6fd33018c75d345e8634d869d20e1cf60156afe5fd34d6f56501da8d35d360"
      "13eadcc44dc3eca44d9ca9ec84c407e0c8156e481cb970dce7e0ce01"
      "8e7c02da256e9b485646335c88f65c9e3cfadbfed94f9abfac828c52cd00006e003fb616"
      "fc3c17d4f341be04c211d533419e89d43c2e07ca5c447f1764cdb618",
      "ee31bfb010c5c399ba4d4d0b594c7dddc1c0c1ae4d8eb07eaab44c825593e2fa78511b54"
      "b43c260d8b8134785e6f63edb0ee51e0b4ddd10ec97831ecc781e0bc"
      "33201c513d1ad17e44c771de9b09797cfdc5181ed73badaed5a0fdceb27cc3409eeb1264"
      "78d87178df939e9ff3af08f7e32bbaed35091ef14e12f2f684bcb0be",
      "5f7a09db36c5d0d0908b088645c7ee34ed1ed46c1d43625a7859c74759c3649022465013"
      "ba8e0651c78483ce4161fbb2d45f3fae7f37cf789e681e8dbf769a3f"
      "5ffd62cae4ad5dffb82493c7635abc9e60bdb3fe1e1704bcf988bef76c8355ca6a9329bb"
      "65a43d56953b2857a98cf9720c276e1f4050cb5aff4030ff6ff968da",
      "795f27e4f1f57762785cdfdf49e493216f5cd61143d9fe3020ef7b7e2fd91f7fbd556fc9"
      "e4f148bb3fe69faa45b7fcbcb4a9d22a52d62ab68273af0ae9f1c713"
      "c1fcb4dd7bceebded8421a23d6f4eea91f12f25685bcb0fee7efcb6326817e7bfa2f4596"
      "2ffcf84ea5faeccf6f4b8b32793cd2eeb3f7d54af9c9e6edaec2b6ef",
      "3ac71b2567b756c85ff8ec3febb39d84e7998d390fd7896db81ec53dce9b96ef7e4ac82b"
      "0879617d42df1db649f6ff00b2fdb7db7874e1bfe0fcfd3757bd97df"
      "7a480ed1caf6ca8b62a1e6368feb0f8affbffffe062f1844f2",
      ""};
  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&data[0], 5736U, &nameCaptureInfo);
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
  xInputs = emlrtCreateLogicalMatrix(1, 4);
  emlrtSetField(xEntryPoints, 0, "QualifiedName",
                emlrtMxCreateString("pfqn_mmint2_gausslaguerre"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(4.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(2.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 0, "ResolvedFilePath",
      emlrtMxCreateString("/home/gcasale/Dropbox/code/line-dev.git/matlab/src/"
                          "api/pfqn/pfqn_mmint2_gausslaguerre.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(740053.8169791667));
  emlrtSetField(xEntryPoints, 0, "Visible", emlrtMxCreateLogicalScalar(true));
  xResult =
      emlrtCreateStructMatrix(1, 1, 7, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("25.1.0.2973910 (R2025a) Update 1"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)c_emlrtMexFcnResolvedFunctionsI());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("mGnz2F8WynOckvxtaimEtE"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  emlrtSetField(xResult, 0, "AuxData",
                emlrtMxCreateRowVectorUINT8((const uint8_T *)&v, 216U));
  return xResult;
}

/* End of code generation (_coder_pfqn_mmint2_gausslaguerre_info.c) */
