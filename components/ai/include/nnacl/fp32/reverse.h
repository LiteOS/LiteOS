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

#ifndef MINDSPORE_LITE_NNACL_REVERSE_H_
#define MINDSPORE_LITE_NNACL_REVERSE_H_

#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif
#include "nnacl/op_base.h"
#define REVERSE_SHAPE_MAX_SIZE 4

// For reverse.
typedef struct ReverseParameter {
  OpParameter op_parameter_;
  int axis_[REVERSE_SHAPE_MAX_SIZE];
  int num_axis_;
} ReverseParameter;

#ifdef __cplusplus
extern "C" {
#endif
int Reverse(const float *input, float *output, size_t elem_size, int *index);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_REVERSE_H_
