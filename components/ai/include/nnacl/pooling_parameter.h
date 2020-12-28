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
#ifndef MINDSPORE_LITE_NNACL_POOLING_PARAMETER_H_
#define MINDSPORE_LITE_NNACL_POOLING_PARAMETER_H_

#include "nnacl/op_base.h"
#include "nnacl/quantization/quantize.h"

typedef enum PoolMode { PoolMode_No, PoolMode_MaxPool, PoolMode_AvgPool } PoolMode;

typedef enum RoundMode { RoundMode_No, RoundMode_Ceil, RoundMode_Floor } RoundMode;

typedef struct PoolingParameter {
  OpParameter op_parameter_;
  PoolMode pool_mode_;
  RoundMode round_mode_;
  ActType act_type_;
  QuantArg **quant_args_;
  int window_w_;
  int window_h_;
  int input_w_;
  int input_h_;
  int input_batch_;
  int input_channel_;
  int output_w_;
  int output_h_;
  int output_batch_;
  int output_channel_;
  int pad_u_;
  int pad_d_;
  int pad_l_;
  int pad_r_;
  int stride_w_;
  int stride_h_;
  int thread_num_;
  bool global_;
  bool quantize_;
} PoolingParameter;

#endif  // MINDSPORE_LITE_NNACL_POOLING_PARAMETER_H_
