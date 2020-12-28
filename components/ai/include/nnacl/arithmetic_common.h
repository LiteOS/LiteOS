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
#ifndef MINDSPORE_LITE_NNACL_ARITHMETIC_COMMON_H_
#define MINDSPORE_LITE_NNACL_ARITHMETIC_COMMON_H_

#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif
#include <string.h>
#include "nnacl/op_base.h"
#include "nnacl/arithmetic_common.h"

typedef struct ArithmeticParameter {
  OpParameter op_parameter_;
  bool broadcasting_;
  size_t ndim_;
  int activation_type_;
  int in_shape0_[5];
  int in_elements_num0_;
  int in_shape1_[5];
  int in_elements_num1_;

  int out_shape_[5];
  int out_elements_num_;

  int in_strides0_[5];
  int in_strides1_[5];
  int out_strides_[5];

  int multiples0_[5];
  int multiples1_[5];
} ArithmeticParameter;

#ifdef __cplusplus
extern "C" {
#endif
void TileOneDimension(float *inData, float *outData, int dim, size_t ndim, int *inShape, int *inStrides,
                      int *outStrides, int *multiple);
void ComputeStrides(int *shape, int *strides, int ndim);

void CalcMultiplesAndStrides(ArithmeticParameter *param);

void TileDimensions(float *data0, float *data1, float *tile_data0, float *tile_data1, ArithmeticParameter *param);
void TileDimensionsUint8(uint8_t *data0, uint8_t *data1, uint8_t *tile_data0, uint8_t *tile_data1,
                         ArithmeticParameter *param);
void TileDimensionsInt8(int8_t *data0, int8_t *data1, int8_t *tile_data0, int8_t *tile_data1,
                        ArithmeticParameter *param);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_ARITHMETIC_COMMON_H_
