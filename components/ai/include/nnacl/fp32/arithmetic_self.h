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

#ifndef MINDSPORE_LITE_NNACL_ARITHMETIC_SELF_H_
#define MINDSPORE_LITE_NNACL_ARITHMETIC_SELF_H_

#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif
#include "nnacl/op_base.h"
#include "nnacl/errorcode.h"

#ifdef __cplusplus
extern "C" {
#endif
int ElementAbs(float *input, float *output, int element_size);

int ElementCos(float *input, float *output, int element_size);

int ElementExp(float *input, float *output, int element_size);

int ElementLog(float *input, float *output, int element_size);

int ElementSquare(float *input, float *output, int element_size);

int ElementSqrt(float *input, float *output, int element_size);

int ElementRsqrt(float *input, float *output, int element_size);

int ElementSin(float *input, float *output, int element_size);

int ElementLogicalNot(float *input, float *output, int element_size);

int ElementRound(float *input, float *output, int element_size);

int ElementFloor(float *input, float *output, int element_size);

int ElementCeil(float *input, float *output, int number);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_ARITHMETIC_SELF_H_
