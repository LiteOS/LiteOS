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
#ifndef MINDSPORE_LITE_NNACL_INT8_DECONV_H_
#define MINDSPORE_LITE_NNACL_INT8_DECONV_H_

#include <string.h>
#include "nnacl/pack.h"
#include "nnacl/op_base.h"
#include "nnacl/errorcode.h"
#include "nnacl/conv_parameter.h"
#include "nnacl/common_func.h"
#include "nnacl/int8/matmul_int8.h"

#ifdef __cplusplus
extern "C" {
#endif
void DeConvPackWeightSum(int8_t *weight, int32_t *weight_sum, int32_t input_zp, int32_t filter_zp, int deep16, int col4,
                         bool suppport_opt);
void DeConvPackInputSum(const int8_t *src, int32_t *dst, int32_t filter_zp, size_t row4, size_t col16,
                        bool suppport_opt);
void DeConvWeightTransInt8(int8_t *src, int8_t *dst, int input_channel, int output_channel, int plane,
                           bool support_optimize_);

int DeConvInt8(const int8_t *input, const int8_t *weight, int32_t *output, int32_t *weight_sum, int32_t *input_sum,
               size_t act_row, size_t act_col, size_t act_deep, ConvParameter *conv_param,
               MATMUL_OPT_R4_FUNC matmul_func);
int DeConvPostInt8(const int32_t *src, const int32_t *bias, int32_t *tmp, int8_t *out, int output_channel,
                   ConvParameter *conv_param, bool support_optimize);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_INT8_DECONV_H_
