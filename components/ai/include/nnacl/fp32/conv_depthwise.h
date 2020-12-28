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

#ifndef MINDSPORE_LITE_NNACL_FP32_CONV_DEPTHWISE_H_
#define MINDSPORE_LITE_NNACL_FP32_CONV_DEPTHWISE_H_

#include "nnacl/conv_parameter.h"

#ifndef ENABLE_ARM64
void DepthwiseCenter(float *dst, const float *src, const float *weight, const float *bias, int height, int width,
                     int kernel_h, int kernel_w, int out_h_step, int block_channel, int in_sh_step, int in_sw_step,
                     int in_kh_step, int in_kw_step, bool is_relu, bool is_relu6);
#endif

#ifdef __cplusplus
extern "C" {
#endif

void ConvDw(float *output_data, const float *input_data, const float *weight_data, const float *bias_data,
            const ConvParameter *conv_param, int task_id);

void InitSlidingParam(SlidingWindowParam *sliding, const ConvParameter *conv_param, int block);

void InitSlidingParamConv(SlidingWindowParam *sliding, const ConvParameter *conv_param, int block);

void AppendSlidingParamConv(SlidingWindowParam *sliding, const ConvParameter *conv_param, int block);

void InitSlidingParamConvDw(SlidingWindowParam *sliding, const ConvParameter *conv_param, int block);

void AppendSlidingParamConvDw(SlidingWindowParam *sliding, const ConvParameter *conv_param, int block);

void DepthwiseBorder(float *dst, const float *src, const float *weight, const float *bias, int top, int bottom,
                     int left, int right, const ConvParameter *conv_param, const SlidingWindowParam *sliding);

void ConvDwC4Fp32(float *output_data, const float *input_data, const float *weight_data, const float *bias_data,
                  const ConvParameter *conv_param, const SlidingWindowParam *sliding, int task_id);

void DeconvDwC4Fp32(float *output_data, const float *input_data, const float *weight_data, const float *bias_data,
                    const ConvParameter *conv_param, const SlidingWindowParam *sliding, int task_id);

#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_FP32_CONV_DEPTHWISE_H_
