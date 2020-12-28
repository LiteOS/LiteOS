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

#ifndef MINDSPORE_LITE_NNACL_OP_BASE_H_
#define MINDSPORE_LITE_NNACL_OP_BASE_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define C4NUM 4
#define C8NUM 8
#define C12NUM 12
#define C16NUM 16
#define TILE_NUM 8

#define MSMIN(x, y) ((x) < (y) ? (x) : (y))
#define MSMAX(x, y) ((x) > (y) ? (x) : (y))

#define UP_DIV(x, y) (((x) + (y) - (1)) / (y))
#define UP_ROUND(x, y) (((x) + (y) - (1)) / (y) * (y))
#define UP_ROUND_DIV(x, y) (x % y == 0 ? (x / y) : (x / y) + 1)
#define DOWN_DIV(x, y) (((x) - (y) + (1)) / (y))

#define MSVALID(left, x, right) (MSMIN((MSMAX(left, x)), right))

#define DIMENSION_4D 4

#define kInputIndex 0
#define kWeightIndex 1
#define kBiasIndex 2
#define kOutputIndex 0
#define kNHWC_N 0
#define kNHWC_H 1
#define kNHWC_W 2
#define kNHWC_C 3
#define kInputSize1 2
#define kInputSize2 3

typedef enum LiteDataType {
  kDataTypeFloat,
  kDataTypeInt8,
} LiteDataType;

typedef enum DataOrder {
  RowMajor,
  ColMajor,
} DataOrder;

typedef struct OpParameter {
  char name_[100];
  int type_;
  int thread_num_;
} OpParameter;

typedef enum ActType { ActType_No, ActType_Relu, ActType_Relu6, ActType_Prelu } ActType;

#endif  // MINDSPORE_LITE_NNACL_OP_BASE_H_
