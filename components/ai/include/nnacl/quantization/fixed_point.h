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

#ifndef MINDSPORE_LITE_NNACL_QUANTIZATION_FIXED_POINT_H_
#define MINDSPORE_LITE_NNACL_QUANTIZATION_FIXED_POINT_H_

#include <stdint.h>
#include <limits.h>
#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// returns the high-32 bits of a * b with rounding
// assume that a and b is divided by 2^31, who fall into [-1, 1]
// so the mantissa of a * b is (a / 2^31) * (b / 2^31) * 2^31= (a * b) / 2^31
// actually we compute 2 * a * b / 2^32
// and take 32 bits of mantissa for rounding
int SaturatingRoundingDoublingHighMul(int a, int b);

int16_t SaturatingRoundingDoublingHighMulInt16(int16_t a, int16_t b);

// division by a 2^exponent with rounding
// or arithmetic right shift with rouding
int RoundingDivideByPOT(int x, int exponent);

int MultiplyByQuantizedMultiplier(int32_t value, int32_t multiplier, int32_t left_shift, int32_t right_shift);

int32_t Rescale(int x, int kIntegerBitsSrc, int kIntegerBitsDst);

int CountLeadingSignBits(int32_t x);

int32_t ComputerReciprocal(int32_t x, int x_digits, int *recip_shift);

int exp_on_negative_values(int a, const int tIntegerBits);

#ifdef __cplusplus
}
#endif

#ifdef ENABLE_NEON
int32x4_t RoundingDivideByPOTInt32x4(int32x4_t x, int exponent);

int32x4_t SaturatingRoundingDoublingHighMulInt32x4(int32x4_t a, int32x4_t b);
#endif

#endif  // MINDSPORE_LITE_NNACL_QUANTIZATION_FIXED_POINT_H_
