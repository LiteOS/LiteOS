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

#ifndef MINDSPORE_LITE_NNACL_COMMON_FUNC_H_
#define MINDSPORE_LITE_NNACL_COMMON_FUNC_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nnacl/op_base.h"
#include "nnacl/conv_parameter.h"

#ifdef __cplusplus
extern "C" {
#endif

int8_t MinInt8(int8_t a, int8_t b);
int8_t MaxInt8(int8_t a, int8_t b);
void ReluFp32(float *data, float *dst, int ele_num);
void Relu6Fp32(float *data, float *dst, int ele_num);
void SimplePostFuncInt8(const int *in, int8_t *out, int oc, int plane, int plane8, int32_t multiplier,
                        int32_t left_shift, int32_t right_shift, int32_t zp);
void IndirectGemmFp32_8x8(float *output, const float *input, const float *weight, const float *bias, size_t step,
                          size_t ic4, size_t output_channel, size_t offset, size_t mode, size_t writeC4, size_t relu,
                          size_t relu6);
void IndirectGemmFp32_8x4(float *output, const float *input, const float *weight, const float *bias, size_t step,
                          size_t ic4, size_t output_channel, size_t offset, size_t mode, size_t writeC4, size_t relu,
                          size_t relu6);
void IndirectGemmFp32_Comm(float *output, const float *input, const float *weight, size_t ic4, size_t hw, size_t oc,
                           size_t offset);
void IndirectGemmFp32(float *output, const float *input, const float *weight, const float *bias, size_t step, int ic4,
                      int output_channel, size_t offset, size_t relu, size_t relu6);
int offset(const int *shape, const int dim0, const int dim1, const int dim2, const int dim3);
int offsetComm(const int *shape, const int dim0, const int dim1, const int dim2);
int offset4d(const int *shape, const int *dims);

#ifdef ENABLE_ARM64
void BiasAdd(const float *bias, float *data, size_t oc4, size_t plan_size);
void BiasAddRelu6(const float *bias, float *data, size_t oc4, size_t plan_size);
void BiasAddRelu(const float *bias, float *data, size_t oc4, size_t plan_size);
void Relu6(float *data, size_t element4);
void Relu(float *data, size_t element4);
#endif

#ifdef __cplusplus
}
#endif

#endif /* MINDSPORE_LITE_NNACL_COMMON_FUNC_H_ */
