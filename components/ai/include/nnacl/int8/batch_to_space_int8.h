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
#ifndef MINDSPORE_LITE_NNACL_INT8_BATCH_TO_SPACE_INT8_H_
#define MINDSPORE_LITE_NNACL_INT8_BATCH_TO_SPACE_INT8_H_
#include "nnacl/op_base.h"
#include "nnacl/quantization/quantize.h"

#ifdef __cplusplus
extern "C" {
#endif
void BatchToSpaceNoCropForNHWCInt8(const int8_t *input, int8_t *output, const int *in_shape, int out_n,
                                   const int *block, QuantArg *in_quant_arg, QuantArg *out_quant_arg);
void BatchToSpaceForNHWCInt8(const int8_t *input, int8_t *output, const int *in_shape, int out_n, const int *block,
                             const int *crops, QuantArg *in_quant_arg, QuantArg *out_quant_arg);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_INT8_BATCH_TO_SPACE_INT8_H_
