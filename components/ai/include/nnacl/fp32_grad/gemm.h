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

#ifndef MINDSPORE_LITE_NNACL_FP32_GRAD_GEMM_H_
#define MINDSPORE_LITE_NNACL_FP32_GRAD_GEMM_H_

#ifdef __cplusplus
extern "C" {
#endif
void gemm(int transpose_a, int transpose_b, int M, int N, int K, float alpha, float *mat_a, int lda, float *mat_b,
          int ldb, float beta, float *mat_c, int ldc);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_FP32_GRAD_GEMM_H_
