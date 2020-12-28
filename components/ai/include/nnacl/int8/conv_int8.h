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
#ifndef MINDSPORE_LITE_NNACL_INT8_CONV_INT8_H_
#define MINDSPORE_LITE_NNACL_INT8_CONV_INT8_H_

#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif
#include "nnacl/pack.h"
#include "nnacl/op_base.h"
#include "nnacl/common_func.h"
#include "nnacl/conv_parameter.h"
#include "nnacl/winograd_utils.h"
#include "nnacl/quantization/quantize.h"
#include "nnacl/matmul_parameter.h"
#include "nnacl/int8/matmul_int8.h"

typedef void (*GEMM_FUNC)(int8_t *dst, const int8_t *src, const int8_t *weight, const int32_t *bias, size_t ksize,
                          size_t ic4, size_t output_channel, size_t offset, const int32_t *input_sum, size_t act_min,
                          size_t act_max, size_t out_zp, int32_t *out_multiplier, int32_t *shift_before,
                          int32_t *shift_after, size_t asymmetric, size_t per_channel);

#ifdef __cplusplus
extern "C" {
#endif
void IndirectGemmInt8(int8_t *dst, int32_t *tmp_dst, const int8_t *src, const int8_t *weight, const int32_t *bias,
                      int ic4, size_t kernel_plane, size_t output_channel, const int32_t *input_sum,
                      ConvParameter *conv_param);

void IndirectGemmInt8Opt(int8_t *dst, int32_t *tmp_dst, const int8_t *src, const int8_t *weight, const int32_t *bias,
                         int ic4, size_t kernel_plane, size_t output_channel, const int32_t *input_sum,
                         ConvParameter *conv_param, GEMM_FUNC gemm_func);

// int8 conv common
void ConvInt8(int8_t *input_data, int8_t *packed_input, int8_t *packed_weight, const int32_t *bias_data,
              int32_t *tmp_dst, int8_t *tmp_out, int8_t *output_data, int32_t *input_sum, int task_id,
              ConvParameter *conv_param);

void ConvInt8Opt(int8_t *input_data, int8_t *packed_input, int8_t *packed_weight, const int32_t *bias_data,
                 int32_t *tmp_dst, int8_t *tmp_out, int8_t *output_data, int32_t *input_sum, int task_id,
                 ConvParameter *conv_param, GEMM_FUNC gemm_func);

// int8 convolution 1x1
void Conv1x1PreOpt(const int8_t *src_input, int8_t *packed_input, int32_t *input_sum, size_t input_channel,
                   size_t output_channel, size_t plane_size, ConvParameter *conv_param);
void Conv1x1Int8(const int8_t *packed_input, const int8_t *packed_weight, int8_t *dst, const int32_t *input_sum,
                 const int32_t *bias, int row, int col, int deep16, ConvParameter *conv_param);
void Conv1x1Int8Opt(const int8_t *packed_input, const int8_t *packed_weight, int8_t *dst, const int32_t *input_sum,
                    const int32_t *bias, int row, int col, int deep4, ConvParameter *conv_param,
                    MATMUL_OPT_R_FUNC matmul_func);

// int8 convolution 3x3
void Conv3x3Int8(int16_t *input_data, int16_t *transed_weight, const int32_t *bias_data, int8_t *output_data,
                 int16_t *tile_buffer, int16_t *block_unit_buffer, int32_t *tmp_dst_buffer, int8_t *tmp_out,
                 int task_id, ConvParameter *conv_param);

#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_INT8_CONV_INT8_H_
