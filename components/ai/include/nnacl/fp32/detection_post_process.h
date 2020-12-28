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

#ifndef MINDSPORE_LITE_NNACL_FP32_DETECTION_POST_PROCESS_H_
#define MINDSPORE_LITE_NNACL_FP32_DETECTION_POST_PROCESS_H_

#include "nnacl/op_base.h"
#include "nnacl/detection_post_process_parameter.h"

typedef struct {
  float y;
  float x;
  float h;
  float w;
} BboxCenter;

typedef struct {
  float ymin;
  float xmin;
  float ymax;
  float xmax;
} BboxCorner;

typedef struct {
  float score;
  int index;
} ScoreWithIndex;

#ifdef __cplusplus
extern "C" {
#endif

void nms_multi_classes_regular();

void nms_multi_classes_fase();

int DetectionPostProcess(const int num_boxes, const int num_classes_with_bg, float *input_boxes, float *input_scores,
                         float *input_anchors, float *output_boxes, float *output_classes, float *output_scores,
                         float *output_num, DetectionPostProcessParameter *param);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_FP32_DETECTION_POST_PROCESS_H_
