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
#ifndef MINDSPORE_LITE_NNACL_INT8_ARG_MIN_MAX_INT8_H_
#define MINDSPORE_LITE_NNACL_INT8_ARG_MIN_MAX_INT8_H_

#include "nnacl/arg_min_max_parameter.h"
#include "nnacl/quantization/quantize.h"

#ifdef __cplusplus
extern "C" {
#endif

void Int8ArgMinMaxQuant(const int8_t *input, int8_t *output, const int *in_shape, ArgMinMaxParameter *param,
                        QuantArg *in_quant, QuantArg *out_quant);
void Int8ArgMinMaxDim0(const int8_t *input, int8_t *output, const int *in_shape, ArgMinMaxParameter *param,
                       QuantArg *in_quant, QuantArg *out_quant);
void Int8ArgMinMaxDim1(const int8_t *input, int8_t *output, const int *in_shape, ArgMinMaxParameter *param,
                       QuantArg *in_quant, QuantArg *out_quant);
void Int8ArgMinMaxDim2(const int8_t *input, int8_t *output, const int *in_shape, ArgMinMaxParameter *param,
                       QuantArg *in_quant, QuantArg *out_quant);
void Int8ArgMinMaxDim3(const int8_t *input, int8_t *output, const int *in_shape, ArgMinMaxParameter *param,
                       QuantArg *in_quant, QuantArg *out_quant);

#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_INT8_ARG_MIN_MAX_INT8_H_
