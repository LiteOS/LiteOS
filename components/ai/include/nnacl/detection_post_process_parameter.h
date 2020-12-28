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

#ifndef MINDSPORE_LITE_NNACL_DETECTION_POST_PROCESS_PARAMETER_H_
#define MINDSPORE_LITE_NNACL_DETECTION_POST_PROCESS_PARAMETER_H_
#include "nnacl/op_base.h"

typedef struct DetectionPostProcessParameter {
  OpParameter op_parameter_;
  float h_scale_;
  float w_scale_;
  float x_scale_;
  float y_scale_;
  float nms_iou_threshold_;
  float nms_score_threshold_;
  int64_t max_detections_;
  int64_t detections_per_class_;
  int64_t max_classes_per_detection_;
  int64_t num_classes_;
  bool use_regular_nms_;
  bool out_quantized_;

  void *decoded_boxes_;
  void *nms_candidate_;
  void *selected_;
  void *score_with_class_;
  void *score_with_class_all_;
} DetectionPostProcessParameter;

#endif  // MINDSPORE_LITE_NNACL_DETECTION_POST_PROCESS_PARAMETER_H_
