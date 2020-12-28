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

#ifndef MINDSPORE_LITE_NNACL_LOCAL_RESPONSE_NORM_H_
#define MINDSPORE_LITE_NNACL_LOCAL_RESPONSE_NORM_H_

#include "nnacl/op_base.h"

typedef struct LocalResponseNormParameter {
  OpParameter op_parameter_;
  int depth_radius_;
  float bias_;
  float alpha_;
  float beta_;
} LocalResponseNormParameter;

#ifdef __cplusplus
extern "C" {
#endif
int LocalResponseNorm(float *input_ptr, int out_size, int channel, float *output_ptr,
                      LocalResponseNormParameter *param);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_LOCAL_RESPONSE_NORM_H_
