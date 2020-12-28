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
#ifndef MINDSPORE_LITE_NNACL_PRIOR_BOX_H_
#define MINDSPORE_LITE_NNACL_PRIOR_BOX_H_

#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif
#include <memory.h>
#include "nnacl/op_base.h"
#define PRIOR_BOX_MAX_NUM 8
#define PRIOR_BOX_VAR_NUM 4
typedef struct PriorBoxParameter {
  OpParameter op_parameter_;
  int32_t min_sizes_size;
  int32_t min_sizes[PRIOR_BOX_MAX_NUM];
  int32_t max_sizes_size;
  int32_t max_sizes[PRIOR_BOX_MAX_NUM];
  int32_t aspect_ratios_size;
  float aspect_ratios[PRIOR_BOX_MAX_NUM];
  float variances[PRIOR_BOX_VAR_NUM];
  int32_t image_size_w;
  int32_t image_size_h;
  float step_w;
  float step_h;
  bool clip;
  bool flip;
  float offset;
} PriorBoxParameter;

#ifdef __cplusplus
extern "C" {
#endif
int PriorBox(const float *input_data, float *output_data, const size_t size, const int tid, const int thread_num);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_PRIOR_BOX_H_
