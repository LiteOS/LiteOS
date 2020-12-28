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
#ifndef MINDSPORE_LITE_NNACL_CAST_H_
#define MINDSPORE_LITE_NNACL_CAST_H_

#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif
#include "nnacl/op_base.h"

// For cast.
typedef struct CastParameter {
  OpParameter op_parameter_;
  int src_type_;
  int dst_type_;
} CastParameter;

#ifdef __cplusplus
extern "C" {
#endif
void Uint8ToFloat32(const uint8_t *input, float *output, int number);
void Uint8ToInt8(const uint8_t *input, int8_t *output, int number);
void Int8ToUint8(const int8_t *input, uint8_t *output, int number);
void Int32ToFloat32(const int32_t *input, float *output, int number);
void Fp16ToFloat32(const uint16_t *input, float *output, int number);
void Float32ToFp16(const float *input, uint16_t *output, int number);
void Float32ToInt32(const float *input, int32_t *output, int number);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_CAST_H_
