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

#ifndef MINDSPORE_LITE_NNACL_ERRORCODE_H_
#define MINDSPORE_LITE_NNACL_ERRORCODE_H_

typedef enum ErrorCodeCommonEnum {
  NNACL_OK = 0,
  NNACL_ERR = 1,
  NNACL_NULL_PTR,
  NNACL_PARAM_INVALID,
  OPLIB_COMMON_END = 9999
} ErrorCodeCommonEnum;

typedef enum ErrorCodeFp32OpEnum {
  NNACL_ERRCODE_OP_FP32_START = 10000,
  NNACL_ERRCODE_STRASSEN_RECURSION_MALLOC,
  NNACL_ERRCODE_REVERSE_MALLOC,
  NNACL_ERRCODE_SQRT_NEGATIVE,
  NNACL_ERRCODE_RSQRT_NEGATIVE_OR_ZERO,
  NNACL_ERRCODE_LOG_NEGATIVE_OR_ZERO,
  NNACL_ERRCODE_DIVISOR_ZERO,
  NNACL_ERRCODE_INDEX_OUT_OF_RANGE,
  NNACL_ERRCODE_OP_FP32_END = 19999
} ErrorCodeFp32OpEnum;

typedef enum ErrorCodeFp16OpEnum {
  NNACL_ERRCODE_OP_FP16_START = 20000,
  NNACL_ERRCODE_OP_FP16_END = 29999
} ErrorCodeFp16OpEnum;

typedef enum ErrorCodeUint8OpEnum {
  NNACL_ERRCODE_OP_UINT8_START = 30000,
  NNACL_ERRCODE_OP_UINT8_END = 39999
} ErrorCodeUint8OpEnum;

typedef enum ErrorCodeInt8OpEnum {
  NNACL_ERRCODE_OP_INT8_START = 40000,
  NNACL_ERRCODE_ADD_OVERFLOW,
  NNACL_ERRCODE_MUL_OVERFLOW,
  NNACL_ERRCODE_OP_INT8_END = 49999
} ErrorCodeInt8OpEnums;

#endif  // MINDSPORE_LITE_NNACL_ERRORCODE_H_
