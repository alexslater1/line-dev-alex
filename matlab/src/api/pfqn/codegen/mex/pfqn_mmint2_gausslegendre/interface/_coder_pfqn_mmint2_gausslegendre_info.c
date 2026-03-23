/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_pfqn_mmint2_gausslegendre_info.c
 *
 * Code generation for function 'pfqn_mmint2_gausslegendre'
 *
 */

/* Include files */
#include "_coder_pfqn_mmint2_gausslegendre_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

/* Function Declarations */
static const mxArray *c_emlrtMexFcnResolvedFunctionsI(void);

/* Function Definitions */
static const mxArray *c_emlrtMexFcnResolvedFunctionsI(void)
{
  const mxArray *nameCaptureInfo;
  const char_T *data[6] = {
      "789ced983d6f133118c71d545e8a54c842a58e9d981a9720289d4a439a26a086282f5469"
      "8552e7ce494eb5cfc99def4833f10d18111b6c488c2c8cfd0e6cec0c"
      "f029487a71923bc9ba922b0e8dfa2ccf3dfadbfed9cfe5feb20262b9bd1800e00ef0a2b0"
      "ece5a5611d1fe66bc01f413d36cc0b815ac4f5a1b214d0df0db3c64c",
      "8ebbdc2b4c44f168a6cea8612293974fda1858d866c4c5fa99d230082e1b1497268bfca0"
      "a2990969540ca4c1f3b316d68e4b0e0556cb1eef904c16a37e1c49ce"
      "bb00fc11d48311ec47709ce0bd9d9227d65f09e109bddde89835daef2c4fd69ac8b16d82"
      "9bd8d42dd1f7a8e717fc1bd2fd788ace9c3ac163de6944de8194e7d7",
      "0f775ec316a318363564238261da62ed3aeb428de91812c3c46b3a76134d83438a384175"
      "685b1a446d030e3a07a5ed4b506ffdb0fedd3de77982793cfed659fe"
      "72f3aba192b77dfbd3aa4a9e8859f1ba92f5cefb7bbc27e1c5037aae5c3ca9922aad30b7"
      "58e02cc7b50d7e30b18f4208276c1f4052ab5aff4832ff5ff9e8bcf3",
      "be4dc913eb574278423fac44f2499f37aee988a3447f1850f73dbf57ec8ff7bf7f5854c9"
      "1331effed8dacdeadbafb26f923df7b9bbc99dfd4cc66d81f9f1c753"
      "c9fc79bbf75cd4bdb181344eccd9dd533f46e46d49797efdefdf97c30d02bdf6f45f8a2a"
      "5ff8f5932af5d9df3f565754f2445c569f5d96f0e2017da3bc9b79f1",
      "f26127c78b8fac5e76c7dacfa792a92b9ffd5f7db61df13c8b21e7113a614ddba1b82b78"
      "b3f2ddcf11792929cfaf4fe9bba336a9fe1f40b5ff766a4faffc175c"
      "bcff3ed87b9c2c6c9263b45e5cafa65379bbde2b3d495f7efffd037cd845a4",
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
                emlrtMxCreateString("pfqn_mmint2_gausslegendre"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(4.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(2.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 0, "ResolvedFilePath",
      emlrtMxCreateString("/home/gcasale/Dropbox/code/line-dev.git/matlab/src/"
                          "api/pfqn/pfqn_mmint2_gausslegendre.m"));
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
                emlrtMxCreateString("L7Gj4Zkl3aJOuNQ4UaKoTG"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  emlrtSetField(xResult, 0, "AuxData",
                emlrtMxCreateRowVectorUINT8((const uint8_T *)&v, 216U));
  return xResult;
}

/* End of code generation (_coder_pfqn_mmint2_gausslegendre_info.c) */
