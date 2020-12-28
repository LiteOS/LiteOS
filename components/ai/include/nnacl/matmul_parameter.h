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

#ifndef MINDSPORE_LITE_NNACL_MATMUL_H_
#define MINDSPORE_LITE_NNACL_MATMUL_H_

#include "nnacl/op_base.h"

typedef void (*MATMUL_OPT_R4_FUNC)(const int8_t *a, const int8_t *b, int *dst, int row_4, int col_4, int deep_16,
                                   const int *input_sum, const int *bias);

typedef void (*MATMUL_OPT_R_FUNC)(const int8_t *a, const int8_t *b, int8_t *dst, size_t row, size_t col, size_t deep_4,
                                  size_t stride, const int32_t *input_sum, const int32_t *bias, int32_t *left_shift,
                                  int32_t *right_shift, int32_t *multiplier, int32_t output_zp, int32_t mini,
                                  int32_t maxi, bool per_channel);

typedef void (*MAT_TRANS_FUNC)(void *dst, void *a, int row, int col);

typedef enum OutType { OutType_C8 = 0, OutType_Nhwc = 1, OutType_TileC8 = 2 } OutType;

typedef struct MatMulParameter {
  OpParameter op_parameter_;
  int row_;
  int col_;
  int row_4_;
  int row_8_;
  int row_12_;
  int row_16_;
  int col_4_;
  int col_8_;
  int deep_;
  int deep_4_;
  int deep_16_;
  bool has_bias_;
  int batch;
  bool a_transpose_; /* false :  row-major  */
  bool b_transpose_; /* true  :  col-major  */
  bool a_const_;
  bool b_const_;
  ActType act_type_;
} MatMulParameter;

#endif  // MINDSPORE_LITE_NNACL_MATMUL_H_
