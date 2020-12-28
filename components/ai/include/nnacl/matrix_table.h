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

#ifndef MINDSPORE_LITE_NNACL_MATRIX_TABLE_H_
#define MINDSPORE_LITE_NNACL_MATRIX_TABLE_H_

#ifdef __cplusplus
extern "C" {
#endif
void MatrixG4x2(float *matrix_data);

void MatrixGT2x4(float *matrix_data);

void MatrixG8x2(float *matrix_data);

void MatrixGT2x8(float *matrix_data);

void MatrixG8x3(float *matrix_data);

void MatrixGT3x8(float *matrix_data);

void MatrixG8x4(float *matrix_data);

void MatrixGT4x8(float *matrix_data);

void MatrixG8x5(float *matrix_data);

void MatrixGT5x8(float *matrix_data);

void MatrixG8x6(float *matrix_data);

void MatrixGT6x8(float *matrix_data);

void MatrixG8x7(float *matrix_data);

void MatrixGT7x8(float *matrix_data);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_MATRIX_TABLE_H_
