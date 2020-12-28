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
#ifndef MINDSPORE_LITE_NNACL_INT8_RESIZE_H_
#define MINDSPORE_LITE_NNACL_INT8_RESIZE_H_

#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif
#include <memory.h>
#include "nnacl/op_base.h"
#include "nnacl/quantization/quantize.h"
#include "nnacl/resize_parameter.h"

#ifdef __cplusplus
extern "C" {
#endif
int ResizeBilinearInt8(const int8_t *input_data, int8_t *output_data, const int *input_shape, const int *output_shape,
                       const bool align_corners, QuantArg *quant_in, QuantArg *quant_out, const QuantMulArg *mul_arg,
                       int tid, int thread_num);

int ResizeBilinearInt8WithFloatWeight(const int8_t *input_data, int8_t *output_data, const int *input_shape,
                                      const int *output_shape, const bool align_corners, QuantArg *quant_in,
                                      QuantArg *quant_out, const QuantMulArg *mul_arg, int tid, int thread_num);

void ComputeScale(const int32_t in_value, const int32_t out_value, const bool align_corners, int32_t *scale);

void ComputeInterpolationArgs(const int32_t pos, const int32_t scale, const int32_t size, int32_t *scaled_pos,
                              int32_t *low, int32_t *scaled_low_weight, int32_t *high, int32_t *scaled_high_weight);

int ComputeScaleFloat(const int32_t in_value, const int32_t out_value, const bool align_corners, float *scale);

void ComputeInterpolationArgsFloatWeight(const int32_t pos, const float scale, const int32_t size, float *actual_pos,
                                         int32_t *low, float *low_weight, int32_t *high, float *high_weight);

int ResizeNearestNeighborInt8Simple(const int8_t *input_data, int8_t *output_data, const int *input_shape,
                                    const int *output_shape, const bool align_corners, int tid, int thread_num);

int ResizeNearestNeighborInt8(const int8_t *input_data, int8_t *output_data, const int *input_shape,
                              const int *output_shape, const bool align_corners, const QuantMulArg *multiplier,
                              QuantArg *quant_in, QuantArg *quant_out, int tid, int thread_num);

void ComputeNearestNeighborInt(const int32_t pos, const int in_size, const int32_t new_size, const bool align_corners,
                               int32_t *nearest);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_INT8_RESIZE_H_
