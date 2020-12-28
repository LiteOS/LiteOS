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

#ifndef MINDSPORE_LITE_NNACL_INT8_MATMUL_H_
#define MINDSPORE_LITE_NNACL_INT8_MATMUL_H_

#include <string.h>
#include "nnacl/op_base.h"
#include "nnacl/matmul_parameter.h"

#ifdef __cplusplus
extern "C" {
#endif
void MatMulInt8_16x4(const int8_t *a, const int8_t *b, int *dst, int row_4, int col_4, int deep_16,
                     const int *input_sum, const int *bias);
void MatMulInt8_16x4_r(const int8_t *a, const int8_t *b, int8_t *dst, size_t row, size_t col, size_t deep_16,
                       size_t stride, const int32_t *input_sum, const int32_t *bias, int32_t *left_shift,
                       int32_t *right_shift, int32_t *multiplier, int32_t output_zp, int32_t mini, int32_t maxi,
                       bool per_channel);
void RowMajor2Row8MajorInt8(int8_t *src_ptr, int8_t *dst_ptr, int row, int col);
void RowMajor2Row4x16MajorInt8(int8_t *src_ptr, int8_t *dst_ptr, int row, int col);
void RowMajor2Col8MajorInt8(int8_t *src_ptr, int8_t *dst_ptr, int row, int col);
void RowMajor2Row16x4MajorInt8(void *src_ptr, void *dst_ptr, int row, int col);

void MatMulInt8_8x8_r(const int8_t *a, const int8_t *b, int8_t *dst, size_t row, size_t col, size_t deep_4,
                      size_t stride, const int32_t *input_sum, const int32_t *bias, int32_t *left_shift,
                      int32_t *right_shift, int32_t *multiplier, int32_t output_zp, int32_t mini, int32_t maxi,
                      bool per_channel);
void RowMajor2Row8x4MajorInt8(const int8_t *src_ptr, int8_t *dst_ptr, int row, int col);
void RowMajor2Row4x8MajorInt8(const int8_t *src_ptr, int8_t *dst_ptr, int row, int col);

void RowMajor2Row4x16Major(int8_t *src, int row, int col, int8_t *dst, int col_16);
void RowMajor2Col16x4Major(int8_t *src, int row, int col, int8_t *dst, int row_16);
void CalcInputSums(int8_t *input, int row, int col, int weight_zp, int *dst, DataOrder order);
void CalcWeightBiasSums(int8_t *weight, int row, int col, int input_zp, int weight_zp, int *bias, int *dst,
                        DataOrder order);
void MatmulInt8(const int8_t *a, const int8_t *b, int8_t *dst, const int *a_sums, const int *bias, int act_min,
                int act_max, int out_zp, int multiplier, int left_shift, int right_shift, int row, int col, int deep16,
                int stride);

#ifdef ENABLE_ARM64
void MatmulInt8Neon64(const int8_t *a, const int8_t *b, int8_t *dst, int row4, int col4, int deep16, const int *a_sums,
                      const int *bias, int act_min, int act_max, int out_zp, int multiplier, int left_shift,
                      int right_shift, int row, int col, int stride);

void MatMulR4Int8Neon64(const int8_t *a, const int8_t *b, int32_t *dst, int row4, int col4, int deep16,
                        const int *input_sum, const int *bias);
#endif
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_SRC_BACKEND_ARM_NNACL_INT8_MATMUL_H_
