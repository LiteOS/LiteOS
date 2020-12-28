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
#ifndef MINDSPORE_LITE_NNACL_FP32_RESIZE_H_
#define MINDSPORE_LITE_NNACL_FP32_RESIZE_H_

#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif
#include <memory.h>
#include "nnacl/op_base.h"

#ifdef __cplusplus
extern "C" {
#endif

int PrepareResizeBilinear(const int *input_shape, const int *output_shape, bool align_corners, int *y_bottoms,
                          int *y_tops, int *x_lefts, int *x_rights, float *y_bottom_weights, float *x_left_weights);

int ResizeBilinear(const float *input_data, float *output_data, const int *input_shape, const int *output_shape,
                   int *y_bottoms, int *y_tops, int *x_lefts, int *x_rights, float *y_bottom_weights,
                   float *x_left_weights, int n_h_begin, int n_h_end);

int ResizeBilinear2(const float *input_data, float *output_data, const int *input_shape, const int *output_shape,
                    int *y_bottoms, int *y_tops, int *x_lefts, int *x_rights, float *y_bottom_weights,
                    float *x_left_weights, float *line0, float *line1, int n_h_begin, int n_h_end);

int ResizeNearestNeighbor(const float *input_data, float *output_data, const int *input_shape, const int *output_shape,
                          int tid, int thread_num);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_FP32_RESIZE_H_
