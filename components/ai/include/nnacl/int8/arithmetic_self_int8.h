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

#ifndef MINDSPORE_LITE_NNACL_INT8_ARITHMETIC_SELF_INT8_H_
#define MINDSPORE_LITE_NNACL_INT8_ARITHMETIC_SELF_INT8_H_

#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif
#include "nnacl/op_base.h"
#include "nnacl/errorcode.h"
#include "nnacl/quantization/quantize.h"

#ifdef __cplusplus
extern "C" {
#endif

int Int8ElementRound(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int Int8ElementFloor(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int Int8ElementCeil(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int Int8ElementAbs(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int Int8ElementSin(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int Int8ElementCos(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int Int8ElementLog(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int Int8ElementSqrt(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int Int8ElementRsqrt(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int Int8ElementSquare(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int Int8ElementLogicalNot(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_INT8_ARITHMETIC_SELF_INT8_H_
