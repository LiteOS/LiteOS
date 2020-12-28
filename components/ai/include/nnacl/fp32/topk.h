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

#ifndef MINDSPORE_LITE_NNACL_TOPK_H_
#define MINDSPORE_LITE_NNACL_TOPK_H_

#include "nnacl/op_base.h"

typedef struct TopkNode {
  float element;
  int32_t index;
} TopkNode;

typedef struct TopkParameter {
  OpParameter op_parameter_;
  int last_dim_size_;
  int loop_num_;
  int k_;
  bool sorted_;
  void *topk_node_list_;
} TopkParameter;

#ifdef __cplusplus
extern "C" {
#endif
void Topk(float *input_data, float *output_data, int32_t *output_index, TopkParameter *parameter);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_TOPK_H_
