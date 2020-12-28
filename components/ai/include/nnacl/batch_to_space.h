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
#ifndef MINDSPORE_LITE_NNACL_BATCH_TO_SPACE_H_
#define MINDSPORE_LITE_NNACL_BATCH_TO_SPACE_H_
#include "nnacl/op_base.h"

#define BATCH_TO_SPACE_BLOCK_SHAPE_SIZE 2
#define BATCH_TO_SPACE_CROPS_SIZE 4

typedef struct BatchToSpaceParameter {
  OpParameter op_parameter_;
  int32_t block_shape_[BATCH_TO_SPACE_BLOCK_SHAPE_SIZE];
  int32_t crops_[BATCH_TO_SPACE_CROPS_SIZE];
} BatchToSpaceParameter;

#ifdef __cplusplus
extern "C" {
#endif
void BatchToSpaceNoCropForNHWC(const void *input, void *output, const int *in_shape, int out_n, const int *block,
                               int data_size);
void BatchToSpaceForNHWC(const void *input, void *output, const int *in_shape, int out_n, const int *block,
                         const int *crops, int data_size);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_FP32_BATCH_TO_SPACE_H_
