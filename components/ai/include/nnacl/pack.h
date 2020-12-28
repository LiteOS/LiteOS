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

#ifndef MINDSPORE_LITE_NNACL_PACK_H_
#define MINDSPORE_LITE_NNACL_PACK_H_

#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif
#include "nnacl/conv_parameter.h"
#include "nnacl/op_base.h"

#ifdef __cplusplus
extern "C" {
#endif
void Im2ColPackUnitFp32(const float *input_data, ConvParameter *conv_param, float *packed_input, int real_cal_num,
                        int block_index);

void Im2ColPackUnitInt8(const int8_t *input_data, int8_t *packed_input, int real_cal_num, int block_index,
                        int32_t *input_sum, ConvParameter *conv_param);

void Im2ColPackUnitInt8Opt(const int8_t *input_data, int8_t *packed_input, int real_cal_num, int block_index,
                           int32_t *input_sum, ConvParameter *conv_param);

void PackInputSum16x4PerLayer(const int8_t *src, int32_t *dst, int32_t filter_zp, size_t row4, size_t col16);

void Conv1x1InputPack(const void *src_ptr, void *dst_ptr, ConvParameter *conv_param, int data_size);

void Pack1x1WeightFp32(const float *weight_data, float *packed_weight, ConvParameter *conv_param);

void PackInputSum16x4Int8(const int8_t *input_value, int32_t *input_sum, size_t input_channel, size_t output_channel,
                          size_t plane_size, ConvParameter *conv_param);

void PackInputSum8x4Int8(const int8_t *input_value, int32_t *input_sum, size_t input_channel, size_t output_channel,
                         size_t plane_size, ConvParameter *conv_param);

void MatrixPack(const float *src, float *dst, int row, int ic4, int stride);

void PackInputToC8Int8(const int8_t *input_data, int16_t *packed_input, ConvParameter *conv_param);

void PackWeightFp32(float *weight_data, ConvParameter *conv_param, float *packed_weight, int oc_block,
                    int oc_block_num);

void PackWeightKHWToHWKFp32(const void *src, void *dst, int plane, int channel);

void PackWeightInt8(int8_t *weight_data, ConvParameter *conv_param, int8_t *packed_weight, int32_t *weight_sum);

void PackWeightInt8Opt(int8_t *weight_data, ConvParameter *conv_param, int8_t *packed_weight, int32_t *weight_sum);

void PackWeightToC8Int8(const int8_t *origin_weight_data, int16_t *packed_weight_data, ConvParameter *conv_param);

void PackNHWCToNC4HW4Fp32(const void *src, void *dst, int batch, int plane, int channel);

void PackNCHWToNC4HW4Fp32(const void *src, void *dst, int batch, int plane, int channel);

void PackNHWCToNHWC4Fp32(const void *src, void *dst, int batch, int plane, int channel);

void PackNHWCToNCHWFp32(const void *src, void *dst, int batch, int plane, int channel);

void PackNHWCToNCHWInt8(const void *src, void *dst, int batch, int plane, int channel);

void PackNCHWToNHWCFp32(const void *src, void *dst, int batch, int plane, int channel);

void PackNHWC4ToNHWCFp32(const void *src, void *dst, int batch, int plane, int channel);

void PackNCHWToNHWC4Fp32(const void *src, void *dst, int batch, int plane, int channel);

void PackNC4HW4ToNHWC4Fp32(const void *src, void *dst, int batch, int plane, int channel);

void PackNC4HW4ToNHWCFp32(const void *src, void *dst, int batch, int plane, int channel);

void PackNC4HW4ToNHWCReluFp32(const void *src, void *dst, int batch, int plane, int channel);

void PackNC4HW4ToNHWCRelu6Fp32(const void *src, void *dst, int batch, int plane, int channel);

void PackNC4HW4ToNHWCPreluFp32(const void *src, void *dst, const void *slope, int batch, int plane, int channel);

void PackNC4HW4ToNCHWFp32(const void *src, void *dst, int batch, int plane, int channel);

void PackNHWCToC8HWN8Fp32(const void *src, void *dst, int batch, int plane, int channel);

void PackNHWCToNHWC4Int8(const void *src, void *dst, int batch, int plane, int channel);

void PackNHWC4ToNHWCInt8(const void *src, void *dst, int batch, int plane, int channel);

void PackNCHWToNHWC4Int8(const void *src, void *dst, int batch, int plane, int channel);

void PackNC4HW4ToNHWC4Int8(const void *src, void *dst, int batch, int plane, int channel);

void PackNC4HW4ToNHWCInt8(const void *src, void *dst, int batch, int plane, int channel);

void PackNC4HW4ToNCHWInt8(const void *src, void *dst, int batch, int plane, int channel);

void PackNHWCToC8HWN8Int8(const void *src, void *dst, int batch, int plane, int channel);

void PackNHWCToNC8HW8Int8(const void *src, void *dst, int batch, int plane, int channel);

void PackNCHWToNHWCInt8(const void *src, void *dst, int batch, int plane, int channel);

void PackDepthwiseInt8Input(const int8_t *src, int16_t *dst, const ConvParameter *conv_param);

void PackDepthwiseInt8Weight(const int8_t *origin_weight, int16_t *packed_weight_, int plane, int channel,
                             ConvQuantArg *quant_qrg);
#ifdef __cplusplus
}
#endif

#endif  // MINDSPORE_LITE_NNACL_PACK_H_
