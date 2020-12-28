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

#ifndef MSMICRO_TENSOR_H
#define MSMICRO_TENSOR_H
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

enum DataType {
  DataType_DT_FLOAT = 0,
  DataType_DT_FLOAT16 = 1,
  DataType_DT_INT8 = 2,
  DataType_DT_INT32 = 3,
  DataType_DT_UINT8 = 4,
  DataType_DT_INT16 = 5,
  DataType_DT_UINT32 = 8,
  DataType_DT_INT64 = 9,
  DataType_DT_UINT16 = 10,
  DataType_DT_UNDEFINED = 16,
  DataType_MIN = DataType_DT_FLOAT,
  DataType_MAX = DataType_DT_UNDEFINED
};

enum Format {
  Format_NCHW = 0,
  Format_NHWC = 1,
  Format_HWKC = 2,
  Format_HWCK = 3,
  Format_KCHW = 4,
  Format_CKHW = 5,
  Format_KHWC = 6,
  Format_CHWK = 7,
  Format_NC4HW4 = 100,
  Format_NUM_OF_FORMAT = 101,
  Format_MIN = Format_NCHW,
  Format_MAX = Format_NUM_OF_FORMAT
};

typedef struct MsMicroTensor_ {
  enum DataType type;
  enum Format format;
  unsigned int ndim;
  unsigned int dims[4];
  void *data;
} MsMicroTensor;

typedef struct MsMicroTensorArray_ {
  int size;
  MsMicroTensor *data;
} MsMicroTensorArray;

#ifdef __cplusplus
}
#endif  // __cplusplus
#endif  // MSMICRO_TENSOR_H
