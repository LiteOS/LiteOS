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
#ifndef MINDSPORE_LITE_NNACL_FP32_ARG_MIN_MAX_H_
#define MINDSPORE_LITE_NNACL_FP32_ARG_MIN_MAX_H_

#include "nnacl/arg_min_max_parameter.h"

#ifdef __cplusplus
extern "C" {
#endif
void ArgMax(const float *input, float *output, ArgMinMaxParameter *param, int pre_axis_count, int axis_count,
            int after_axis_count);
void ArgMin(const float *input, float *output, ArgMinMaxParameter *param, int pre_axis_count, int axis_count,
            int after_axis_count);
void ArgMaxDim0(const float *input, float *output, const int *in_shape, ArgMinMaxParameter *param);
void ArgMinDim0(const float *input, float *output, const int *in_shape, ArgMinMaxParameter *param);
void ArgMaxDim1(const float *input, float *output, const int *in_shape, ArgMinMaxParameter *param);
void ArgMinDim1(const float *input, float *output, const int *in_shape, ArgMinMaxParameter *param);
void ArgMaxDim2(const float *input, float *output, const int *in_shape, ArgMinMaxParameter *param);
void ArgMinDim2(const float *input, float *output, const int *in_shape, ArgMinMaxParameter *param);
void ArgMaxDim3(const float *input, float *output, const int *in_shape, ArgMinMaxParameter *param);
void ArgMinDim3(const float *input, float *output, const int *in_shape, ArgMinMaxParameter *param);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_FP32_ARG_MIN_MAX_H_
