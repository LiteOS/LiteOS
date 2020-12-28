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

#ifndef MINDSPORE_LITE_NNACL_TILE_H_
#define MINDSPORE_LITE_NNACL_TILE_H_

#include "nnacl/op_base.h"

typedef struct TileParameter {
  OpParameter op_parameter_;
  int in_dim_;
  int in_shape_[5];
  int out_shape_[5];
  int multiples_[5];
  int in_strides_[5];
  int out_strides_[5];
} TileParameter;

#ifdef __cplusplus
extern "C" {
#endif
void Tile(float *input_data, float *output_data, TileParameter *parameter);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_TILE_H_
