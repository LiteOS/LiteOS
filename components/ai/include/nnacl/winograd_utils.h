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

#ifndef MINDSPORE_LITE_NNACL_WINOGRAD_UTILS_H_
#define MINDSPORE_LITE_NNACL_WINOGRAD_UTILS_H_

#ifdef ENABLE_ARM
#include <arm_neon.h>
#endif
#include "nnacl/matrix_table.h"
#include "nnacl/conv_parameter.h"
#include "nnacl/op_base.h"

typedef void (*InputTransformUnitFunc)(const float *src_data, float *dst_data, int src_step, int dst_step);
typedef void (*OutputTransformUnitFunc)(const float *src_data, float *dst_data, const float *bias_data, int src_step,
                                        int dst_step);

#ifdef __cplusplus
extern "C" {
#endif
void InputTransform4x4Unit(const float *src_data, float *dst_data, int src_step, int dst_step);

void InputTransform8x8Unit(const float *src_data, float *dst_data, int src_step, int dst_step);

void OutputTransform4x2Unit(const float *src_data, float *dst_data, const float *bias_data, int src_step, int dst_step);

void OutputTransform4x3Unit(const float *src_data, float *dst_data, const float *bias_data, int src_step, int dst_step);

void OutputTransform8x2Unit(const float *src_data, float *dst_data, const float *bias_data, int src_step, int dst_step);

void OutputTransform8x3Unit(const float *src_data, float *dst_data, const float *bias_data, int src_step, int dst_step);

void OutputTransform8x4Unit(const float *src_data, float *dst_data, const float *bias_data, int src_step, int dst_step);

void OutputTransform8x5Unit(const float *src_data, float *dst_data, const float *bias_data, int src_step, int dst_step);

void OutputTransform8x6Unit(const float *src_data, float *dst_data, const float *bias_data, int src_step, int dst_step);

void OutputTransform8x7Unit(const float *src_data, float *dst_data, const float *bias_data, int src_step, int dst_step);

int SelectOutputUnit(ConvParameter *conv_param);

InputTransformUnitFunc GetInputTransFunc(int input_unit);

OutputTransformUnitFunc GetOutputTransFunc(int input_unit, int output_unit);

void CheckIfUseWinograd(bool *use_winograd, int *output_unit, ConvParameter *conv_param,
                        InputTransformUnitFunc input_trans_func, OutputTransformUnitFunc output_trans_func);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_WINOGRAD_UTILS_H_
