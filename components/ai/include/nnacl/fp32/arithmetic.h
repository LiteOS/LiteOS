/**
 * Copyright 2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MINDSPORE_LITE_NNACL_ARITHMETIC_H_
#define MINDSPORE_LITE_NNACL_ARITHMETIC_H_

#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif
#include "nnacl/op_base.h"
#include "nnacl/arithmetic_common.h"
#include "nnacl/errorcode.h"

#ifdef __cplusplus
extern "C" {
#endif
int ElementOptAdd(float *input0, float *input1, float *output, int element_size, ArithmeticParameter *param);
int ElementOptAddRelu(float *input0, float *input1, float *output, int element_size, ArithmeticParameter *param);
int ElementOptAddRelu6(float *input0, float *input1, float *output, int element_size, ArithmeticParameter *param);
int ElementOptSub(float *input0, float *input1, float *output, int element_size, ArithmeticParameter *param);
int ElementOptSubRelu(float *input0, float *input1, float *output, int element_size, ArithmeticParameter *param);
int ElementOptSubRelu6(float *input0, float *input1, float *output, int element_size, ArithmeticParameter *param);
int ElementOptMul(float *input0, float *input1, float *output, int element_size, ArithmeticParameter *param);
int ElementOptMulRelu(float *input0, float *input1, float *output, int element_size, ArithmeticParameter *param);
int ElementOptMulRelu6(float *input0, float *input1, float *output, int element_size, ArithmeticParameter *param);
int ElementOptDiv(float *input0, float *input1, float *output, int element_size, ArithmeticParameter *param);
int ElementOptDivRelu(float *input0, float *input1, float *output, int element_size, ArithmeticParameter *param);
int ElementOptDivRelu6(float *input0, float *input1, float *output, int element_size, ArithmeticParameter *param);
int ElementMul(float *input0, float *input1, float *output, int element_size);
int ElementMulRelu(float *input0, float *input1, float *output, int element_size);
int ElementMulRelu6(float *input0, float *input1, float *output, int element_size);
int BroadcastMul(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output, int element_size,
                 ArithmeticParameter *param);

int ElementAdd(float *input0, float *input1, float *output, int element_size);
int ElementAddRelu(float *input0, float *input1, float *output, int element_size);
int ElementAddRelu6(float *input0, float *input1, float *output, int element_size);
int BroadcastAdd(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output, int element_size,
                 ArithmeticParameter *param);
int BroadcastAddInt8(int8_t *input0, int8_t *input1, int8_t *tile_input0, int8_t *tile_input1, int8_t *output,
                     int element_size, ArithmeticParameter *param);

int ElementSub(float *input0, float *input1, float *output, int element_size);
int ElementSubRelu(float *input0, float *input1, float *output, int element_size);
int ElementSubRelu6(float *input0, float *input1, float *output, int element_size);
int BroadcastSub(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output, int element_size,
                 ArithmeticParameter *param);

int ElementDiv(float *input0, float *input1, float *output, int element_size);
int ElementDivRelu(float *input0, float *input1, float *output, int element_size);
int ElementDivRelu6(float *input0, float *input1, float *output, int element_size);
int BroadcastDiv(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output, int element_size,
                 ArithmeticParameter *param);

int ElementLogicalAnd(float *input0, float *input1, float *output, int element_size);
int BroadcastLogicalAnd(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output,
                        int element_size, ArithmeticParameter *param);

int ElementLogicalOr(float *input0, float *input1, float *output, int element_size);
int BroadcastLogicalOr(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output,
                       int element_size, ArithmeticParameter *param);

int ElementMaximum(float *input0, float *input1, float *output, int element_size);
int BroadcastMaximum(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output,
                     int element_size, ArithmeticParameter *param);

int ElementMinimum(float *input0, float *input1, float *output, int element_size);
int BroadcastMinimum(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output,
                     int element_size, ArithmeticParameter *param);

int ElementFloorDiv(float *input0, float *input1, float *output, int element_size);
int BroadcastFloorDiv(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output,
                      int element_size, ArithmeticParameter *param);

int ElementFloorMod(float *input0, float *input1, float *output, int element_size);
int BroadcastFloorMod(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output,
                      int element_size, ArithmeticParameter *param);

int ElementSquaredDifference(float *input0, float *input1, float *output, int element_size);
int BroadcastSquaredDifference(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output,
                               int element_size, ArithmeticParameter *param);

int ElementNotEqual(float *input0, float *input1, float *output, int element_size);

int BroadcastNotEqual(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output,
                      int element_size, ArithmeticParameter *param);

int ElementEqual(float *input0, float *input1, float *output, int element_size);

int BroadcastEqual(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output,
                   int element_size, ArithmeticParameter *param);

int ElementLess(float *input0, float *input1, float *output, int element_size);
int BroadcastLess(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output, int element_size,
                  ArithmeticParameter *param);

int ElementLessEqual(float *input0, float *input1, float *output, int element_size);
int BroadcastLessEqual(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output,
                       int element_size, ArithmeticParameter *param);

int ElementGreater(float *input0, float *input1, float *output, int element_size);
int BroadcastGreater(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output,
                     int element_size, ArithmeticParameter *param);

int ElementGreaterEqual(float *input0, float *input1, float *output, int element_size);
int BroadcastGreaterEqual(float *input0, float *input1, float *tile_input0, float *tile_input1, float *output,
                          int element_size, ArithmeticParameter *param);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_ARITHMETIC_H_
