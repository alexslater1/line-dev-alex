/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_pfqn_comom_info.c
 *
 * Code generation for function 'pfqn_comom'
 *
 */

/* Include files */
#include "_coder_pfqn_comom_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

/* Function Declarations */
static const mxArray *c_emlrtMexFcnResolvedFunctionsI(void);

/* Function Definitions */
static const mxArray *c_emlrtMexFcnResolvedFunctionsI(void)
{
  const mxArray *nameCaptureInfo;
  const char_T *data[7] = {
      "789ced96cd6ed34010c73728541115900b481cfb00ed96429a1302da7c34e947421b1a5a"
      "841a67bda997ee7a9d8d13521e810b8f00dc7ae08054f132dc39c023"
      "70a289ed38b664b9892357b13c97c9e86fef7f3c13ff649028ed260000f78011e92523df"
      "b56a33df02ce70eb09332fb86a2b6e83a4e33e4bff6c66c4551df775",
      "a35025864777ca9c115552f5dab98681c01d4e7b581e2a2d42718d307c305eec0d2a5618"
      "9346c5401afcde54303a3be83220948edd211d2f46f368783c6fd267"
      "1eee70cfc37d9de5a74de9679d7fc7c7cfd2b5565b3d419c71e6f46f04f4f7da7fda5464"
      "de6d526cfbfd08e8b7ede9e7d4dfe6df4185330c4f91d491288639c1",
      "b526ef43c4650c2951f1b28c7b2ba744874cd2a9d4841d81a0a411389814b4c7b5620ccc"
      "6f5ef7afd9bf3bdbd7a786f953ea9284e907cb5bd930fdacb829bfbe"
      "c779d7fdff3df4f04bbbf4fdac5a3d6687c5fefb4cfeecb8898a42216b05bb8faa8f8f5f"
      "1fc0a30eebfc797f8f837277d1a77f4b675daa13a470dec10eff4640",
      "ff49b97b11d02fe7e9e7d427df575727148e8dc95854587cf8feef4ba8bcbdb85c7810a6"
      "9f1551e7ede3a7445eaf74b12c0e11cd9c97aa9b6a3db3111ddeceeb"
      "fb1b737632bf98b3b3f18b396b44ccd9c9ce9ff7efda46c0fe53aedaeedf50ae9a418ae0"
      "1fe695af2f3dfd9cfab47c35c7636e253c1efcf9cd42e56bfbe4c5a3",
      "30fdac883a5f735a369729efbe6a55f4d5373b47bd9d12edb563be4686af7ebc6b4948a7"
      "eacd7dbf7e0de8f7dcd3cfa94fc957633c573b892a57fffe5a8ab90a"
      "66cfd56cad58d8ae3c6997f4fd8cf8b89517f5bd8db598ab91e16ad255dbfd1b0a57b198"
      "a55fd85c7de6e9e7d4a7e4ea603cc38d4495ab3fbf5562ae82d97375",
      "b552ce1eb13a2bf75053a9b7959cd0d65f4780abff011135e60b",
      ""};
  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&data[0], 8488U, &nameCaptureInfo);
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
                emlrtMxCreateString("pfqn_comom"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(4.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(xEntryPoints, 0, "ResolvedFilePath",
                emlrtMxCreateString("/home/gcasale/Dropbox/code/line-dev.git/"
                                    "matlab/src/api/pfqn/pfqn_comom.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(740053.81956018519));
  emlrtSetField(xEntryPoints, 0, "Visible", emlrtMxCreateLogicalScalar(true));
  xResult =
      emlrtCreateStructMatrix(1, 1, 7, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("25.1.0.2973910 (R2025a) Update 1"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)c_emlrtMexFcnResolvedFunctionsI());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("J23PxPICZwVm2fFCC2gvXC"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  emlrtSetField(xResult, 0, "AuxData",
                emlrtMxCreateRowVectorUINT8((const uint8_T *)&v, 216U));
  return xResult;
}

/* End of code generation (_coder_pfqn_comom_info.c) */
