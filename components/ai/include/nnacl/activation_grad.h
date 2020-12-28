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
#ifndef MINDSPORE_LITE_NNACL_ACTIVATION_GRAD_H_
#define MINDSPORE_LITE_NNACL_ACTIVATION_GRAD_H_

#include <math.h>
#include "nnacl/op_base.h"
#include "nnacl/fp32/arithmetic.h"
#include "nnacl/errorcode.h"

typedef struct ActivationGradParameter {
  OpParameter op_parameter;
  int type_;
  float alpha_;
} ActivationGradParameter;
#ifdef __cplusplus
extern "C" {
#endif

int ReluGrad(float *src0, float *src1, int length, float *dst);
int Relu6Grad(float *src0, float *src1, int length, float *dst);
int LReluGrad(float *src0, float *src1, int length, float *dst, float alpha);
int SigmoidGrad(float *src0, float *src1, int length, float *dst);
int TanhGrad(float *src0, float *src1, int length, float *dst);
int HSwishGrad(float *src0, float *src1, int length, float *dst);
int HSigmoidGrad(float *src0, float *src1, int length, float *dst);

#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_ACTIVATION_GRAD_H_
