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
#ifndef MINDSPORE_LITE_NNACL_FP32_DECONV_H_
#define MINDSPORE_LITE_NNACL_FP32_DECONV_H_

#include <string.h>
#include "nnacl/pack.h"
#include "nnacl/op_base.h"
#include "nnacl/conv_parameter.h"
#include "nnacl/errorcode.h"
#include "nnacl/fp32/common_func.h"

#ifdef __cplusplus
extern "C" {
#endif
void PackDeConvWeightFp32(const float *weight, float *dst, int input_channel, int output_channel, int plane);
int DeConvPostFp32C12x8(const float *src, float *tmp_out, const float *bias, float *dst, int output_channel,
                        ConvParameter *conv_param);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_FP32_DECONV_H_
