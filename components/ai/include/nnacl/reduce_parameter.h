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

#ifndef MINDSPORE_LITE_NNACL_REDUCE_PARAMETER_H_
#define MINDSPORE_LITE_NNACL_REDUCE_PARAMETER_H_
#include "nnacl/op_base.h"
#define REDUCE_MAX_AXES_NUM 8

struct ReduceParameter {
  OpParameter op_parameter_;
  bool keep_dims_;
  bool reduce_to_end_;
  int axes_[REDUCE_MAX_AXES_NUM];
  int num_axes_;
  int mode_;
};

#endif  // MINDSPORE_LITE_NNACL_REDUCE_PARAMETER_H_
