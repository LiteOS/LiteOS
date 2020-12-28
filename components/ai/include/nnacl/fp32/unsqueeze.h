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

#ifndef MINDSPORE_LITE_NNACL_UNSQUEEZE_H_
#define MINDSPORE_LITE_NNACL_UNSQUEEZE_H_

#include "nnacl/op_base.h"

#define UNSQUEEZE_DIMS_MAX_SIZE 4

typedef struct UnsqueezeParameter {
  OpParameter op_parameter_;
  int dims_[UNSQUEEZE_DIMS_MAX_SIZE];
  int num_dim_;
} UnsqueezeParameter;

#ifdef __cplusplus
extern "C" {
#endif
int Unsqueeze(const int8_t *input_ptr, int8_t *output_ptr, size_t data_size);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_UNSQUEEZE_H_
