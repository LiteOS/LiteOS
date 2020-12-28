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
#ifndef MINDSPORE_LITE_NNACL_FILL_H_
#define MINDSPORE_LITE_NNACL_FILL_H_

#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif
#include "nnacl/op_base.h"
#include "nnacl/errorcode.h"

#define FILL_DIMS_MAX_SIZE 4

typedef struct FillParameter {
  OpParameter op_parameter_;
  int dims_[FILL_DIMS_MAX_SIZE];
  int num_dims_;
} FillParameter;

#ifdef __cplusplus
extern "C" {
#endif
int Fill(float *output, int size, float data);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_FILL_H_
