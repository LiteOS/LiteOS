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

#ifndef MINDSPORE_LITE_NNACL_FP32_ONE_HOT_H_
#define MINDSPORE_LITE_NNACL_FP32_ONE_HOT_H_

#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif
#include "nnacl/op_base.h"

typedef struct OneHotParameter {
  OpParameter op_parameter_;
  int axis_;
  int depth_;
  float on_value_;
  float off_value_;
  int outer_size_;
  int inner_size_;
} OneHotParameter;

#ifdef __cplusplus
extern "C" {
#endif
int OneHot(const int *indices, float *output, const OneHotParameter *one_hot_param, const int tid,
           const int thread_num);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_FP32_ONE_HOT_H_
