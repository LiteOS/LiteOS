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

#ifndef MINDSPORE_LITE_NNACL_INT8_REDUCE_INT8_H_
#define MINDSPORE_LITE_NNACL_INT8_REDUCE_INT8_H_
#include "nnacl/quantization/quantize.h"
#ifdef __cplusplus
extern "C" {
#endif

int ReduceMeanInt8(const int outer_size, const int inner_size, const int axis_size, const int32_t *src_data,
                   int32_t *dst_data, const ReduceQuantArg *quant, const int tid, const int thread_num);
int ReduceMeanLastAxis(const int outer_size, const int inner_size, const int axis_size, const int32_t *src_data,
                       int8_t *dst_data, const ReduceQuantArg *quant, const int tid, const int thread_num);
int ReduceSumInt8(const int outer_size, const int inner_size, const int axis_size, const int32_t *src_data,
                  int32_t *dst_data, const ReduceQuantArg *quant, const int tid, const int thread_num);
int ReduceSumLastAxis(const int outer_size, const int inner_size, const int axis_size, const int32_t *src_data,
                      int8_t *dst_data, const ReduceQuantArg *quant, const int tid, const int thread_num);
int ReduceMaxInt8(const int outer_size, const int inner_size, const int axis_size, const int32_t *src_data,
                  int32_t *dst_data, const ReduceQuantArg *quant, const int tid, const int thread_num);
int ReduceMaxLastAxis(const int outer_size, const int inner_size, const int axis_size, const int32_t *src_data,
                      int8_t *dst_data, const ReduceQuantArg *quant, const int tid, const int thread_num);
int ReduceMinInt8(const int outer_size, const int inner_size, const int axis_size, const int32_t *src_data,
                  int32_t *dst_data, const ReduceQuantArg *quant, const int tid, const int thread_num);
int ReduceMinLastAxis(const int outer_size, const int inner_size, const int axis_size, const int32_t *src_data,
                      int8_t *dst_data, const ReduceQuantArg *quant, const int tid, const int thread_num);
int ReduceProdLastAxis(const int outer_size, const int inner_size, const int axis_size, const int32_t *src_data,
                       int8_t *dst_data, const ReduceQuantArg *quant, const int tid, const int thread_num);
int ReduceProdInt8(const int outer_size, const int inner_size, const int axis_size, const int32_t *src_data,
                   int32_t *dst_data, const ReduceQuantArg *quant, const int tid, const int thread_num);
int ReduceSumSquareLastAxis(const int outer_size, const int inner_size, const int axis_size, const int32_t *src_data,
                            int8_t *dst_data, const ReduceQuantArg *quant, const int tid, const int thread_num);
int ReduceSumSquareInt8(const int outer_size, const int inner_size, const int axis_size, const int32_t *src_data,
                        int32_t *dst_data, const ReduceQuantArg *quant, const int tid, const int thread_num);
bool isAddOverflow(int32_t x, int32_t y);
bool isMulOverflow(int32_t x, int32_t y);
#ifdef __cplusplus
}
#endif
#endif  // MINDSPORE_LITE_NNACL_INT8_REDUCE_INT8_H_
