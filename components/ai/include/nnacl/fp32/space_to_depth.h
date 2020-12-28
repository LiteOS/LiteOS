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
#ifndef MINDSPORE_LITE_SRC_BACKEND_ARM_NNACL_FP32_SPACE_TO_DEPTH_H_
#define MINDSPORE_LITE_SRC_BACKEND_ARM_NNACL_FP32_SPACE_TO_DEPTH_H_
#include "nnacl/op_base.h"

typedef struct SpaceToDepthParameter {
  OpParameter op_parameter_;
  int32_t block_size_;
} SpaceToDepthParameter;
#ifdef __cplusplus
extern "C" {
#endif
int SpaceToDepthForNHWC(const float *input, float *output, int *in_shape, int *out_shape, int shape_size,
                        int block_size, int h_start, int h_end);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_SRC_BACKEND_ARM_NNACL_FP32_SPACE_TO_DEPTH_H_
