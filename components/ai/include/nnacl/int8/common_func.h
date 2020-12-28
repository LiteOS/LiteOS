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

#ifndef MINDSPORE_LITE_NNACL_INT8_COMMON_FUNC_H_
#define MINDSPORE_LITE_NNACL_INT8_COMMON_FUNC_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nnacl/op_base.h"
#include "nnacl/conv_parameter.h"

#ifdef __cplusplus
extern "C" {
#endif

void PostFuncInt8C8(const int32_t *in, const int32_t *bias, int8_t *out, size_t oc, size_t plane, int32_t multiplier,
                    int32_t left_shift, int32_t right_shift, int32_t zp, int32_t mini, int32_t maxi);
void PostFuncInt8C4(const int32_t *in, const int32_t *bias, int8_t *out, size_t oc, size_t plane, size_t stride,
                    int32_t multiplier, int32_t left_shift, int32_t right_shift, int32_t zp, int32_t mini,
                    int32_t maxi);
#ifdef ENABLE_ARM64
void PostFuncInt8C4Neon64(const int32_t *in, const int32_t *bias, int8_t *out, size_t oc4div, size_t oc4res,
                          size_t plane, size_t stride, int32_t multiplier, int32_t left_shift, int32_t right_shift,
                          int32_t zp, int32_t mini, int32_t maxi);
void IndirectGemmInt16to32_8x4(int32_t *dst, const int16_t *src, const int16_t *weight, size_t ksize, size_t ic8,
                               size_t oc4, size_t offset);
void IndirectGemmInt8_4x4(int8_t *output, const int8_t *input, const int8_t *weight, const int32_t *bias, size_t ksize,
                          size_t ic4, size_t oc, size_t offset, const int32_t *input_sum, size_t act_min,
                          size_t act_max, size_t out_zp, int32_t *out_multiplier, int32_t *shift_before,
                          int32_t *shift_after, size_t asymmetric, size_t per_channel);
void DeconvDwInt8Center(int32_t *dst, const int16_t *src, const int16_t *weight, size_t height, size_t width,
                        size_t kernel_h, size_t kernel_w, size_t out_h_step, size_t block_channel, size_t in_sh_step,
                        size_t in_sw_step, size_t in_kh_step, size_t in_kw_step);
void ConvDwInt8Center(int8_t *dst, const int16_t *src, const int16_t *weight, const int32_t *bias, size_t height,
                      size_t width, size_t kernel_h, size_t kernel_w, size_t out_h_step, size_t block_channel,
                      size_t in_sh_step, size_t in_sw_step, size_t in_kh_step, size_t in_kw_step, int out_multiplier,
                      int left_shift, int right_shift, int32_t out_zp, int32_t acc_min, int32_t acc_max);
void ConvDwInt8Row(int32_t *output_ptr, const int8_t *input_ptr, const int16_t *weight_ptr, int num_pixels,
                   int output_channel, int input_step, int8_t input_zp);
void ConvDwInt8PostAlign4(int8_t *dst, int32_t *buffer, int num_pixels, int32_t output_zp, int32_t out_multiplier,
                          int32_t left_shift, int32_t right_shift, int32_t acc_min, int32_t acc_max);
#endif

#ifdef __cplusplus
}
#endif

#endif /* MINDSPORE_LITE_NNACL_FP32_COMMON_FUNC_H_ */
