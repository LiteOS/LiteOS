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

#ifndef MINDSPORE_LITE_NNACL_FP32_LSTM_H_
#define MINDSPORE_LITE_NNACL_FP32_LSTM_H_

#include "nnacl/op_base.h"

typedef struct LstmParameter {
  OpParameter op_parameter_;
  int input_size_;
  int hidden_size_;  // output_size
  int seq_len_;
  int batch_;
  int input_step_;
  int output_step_;
  bool bidirectional_;
} LstmParameter;

#ifdef __cplusplus
extern "C" {
#endif
void Lstm(float *output, const float *input, const float *weight_i, const float *weight_h, const float *bias,
          float *hidden_state, float *cell_state, float *gate_buffer, LstmParameter *lstm_parm);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_FP32_LSTM_H_
