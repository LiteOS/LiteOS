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

#ifndef MINDSPORE_LITE_NNACL_INT8_CROP_INT8_H_
#define MINDSPORE_LITE_NNACL_INT8_CROP_INT8_H_
#include "nnacl/op_base.h"
#include "nnacl/crop_parameter.h"

#ifdef __cplusplus
extern "C" {
#endif
void Crop(const int8_t *input, int8_t *output, int task_id, CropParameter *para);
void Crop1D(const int8_t *input, int8_t *output, int task_id, CropParameter *para);
void Crop2D(const int8_t *input, int8_t *output, int task_id, CropParameter *para);
void Crop3D(const int8_t *input, int8_t *output, int task_id, CropParameter *para);
void Int8Crop4D(const int8_t *input, int8_t *output, int task_id, CropParameter *para);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_INT8_CROP_INT8_H_
