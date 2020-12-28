/**
 * Copyright 2019 Huawei Technologies Co., Ltd
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

#ifndef MINDSPORE_LITE_NNACL_FP32_BATCH_NORM_H_
#define MINDSPORE_LITE_NNACL_FP32_BATCH_NORM_H_

typedef struct bnParameter {
  int batch;
  int channels;
  int spatial;
  float eps;
} bnParameter;

#ifdef __cplusplus
extern "C" {
#endif
void scaleBias(const float *scales, int batch, int n, int size, float *output);
void normalize(const float *x, const float *mean, const float *variance, float eps, int batch, int filters, int spatial,
               float *out);
void backwardScale(const float *x_norm, const float *delta, int batch, int n, int size, float *scale_updates);
void meanVar(const float *in, int batch, int size, int ch, float *mean, float *var);
void meanDelta(float *yt, int size, int ch, float eps, float *variance, float *mean_delta);
void varianceDelta(const float *x, const float *delta, const float *mean, const float *variance, int batch, int ch,
                   int spatial, float eps, float *variance_delta);
void meanAdd(const float *x, const float *mean, const float *variance_delta, int batch, int filters, int spatial,
             float *mean_add, float *mean_delta);
void NormalizeDelta(const float *x, const float *mean, const float *variance, const float *mean_delta,
                    const float *variance_delta, int batch, int filters, int spatial, float eps, float *delta);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_FP32_BATCH_NORM_H_
