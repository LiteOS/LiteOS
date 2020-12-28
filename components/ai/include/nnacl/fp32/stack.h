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
#ifndef MINDSPORE_LITE_NNACL_FP32_STACK_H_
#define MINDSPORE_LITE_NNACL_FP32_STACK_H_

#include "nnacl/op_base.h"

typedef struct StackParameter {
  OpParameter op_parameter_;
  int32_t axis_;
} StackParameter;

#ifdef __cplusplus
extern "C" {
#endif
void DoStack(const float *const *inputs, size_t input_num, int *in_shape, size_t shape_size, int axis, float *output);
void DoStackInt32(const int32_t *const *inputs, size_t input_num, int *in_shape, size_t shape_size, int axis,
                  int32_t *output);
void DoStackOneInput(const int8_t *input, int8_t *output, size_t data_size);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_FP32_STACK_H_
