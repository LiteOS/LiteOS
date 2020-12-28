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

#include <stdbool.h>
#include <stdint.h>
#include "lite_os_printf.h"

#define UP_DIV(x, y) (((x) + (y) - (1)) / (y))

static const int kPrintNums = 20;
static const int kLineSplitNum = 44;
static const int kLineNum = 45;

unsigned int GetTensorElementSize(const MsMicroTensor *tensor)
{
    unsigned int ans = 1;
    if (tensor->format == Format_NC4HW4) {
        for (unsigned int i = 0; i < tensor->ndim; ++i) {
            unsigned int dim = tensor->dims[i];
            if (i == 1) {
                dim = UP_DIV(dim, 4) * 4;
            }
            ans *= dim;
        }
    } else {
        for (unsigned int i = 0; i < tensor->ndim; ++i) {
            ans *= tensor->dims[i];
        }
    }
    return ans;
}

static const char *const TypeNames[] = {
    "DT_FLOAT", "DT_FLOAT16", "DT_INT8", "DT_INT32", "DT_UINT8", "DT_INT16",
    "", "", "DT_UINT32", "DT_INT64", "DT_UINT16", "", "", "", "", "", "DT_UNDEFINED", ""
};

const char *EnumNameFormat(enum Format e)
{
    switch (e) {
        case Format_NCHW:
            return "NCHW";
        case Format_NHWC:
            return "NHWC";
        case Format_HWKC:
            return "HWKC";
        case Format_HWCK:
            return "HWCK";
        case Format_KCHW:
            return "KCHW";
        case Format_CKHW:
            return "CKHW";
        case Format_KHWC:
            return "KHWC";
        case Format_CHWK:
            return "CHWK";
        case Format_NC4HW4:
            return "NC4HW4";
        case Format_NUM_OF_FORMAT:
            return "NUM_OF_FORMAT";
        default:
            return "";
    }
}

static void PrintTensor_U8(const void *data, unsigned int size)
{
    unsigned char *str = (unsigned char *)data;
    for (unsigned int i = 0; i < size - 1; ++i) {
        unsigned char c = str[i];
        printf(" %u,", c);
    }
    printf("\n\n");
    return;
}
static void PrintTensor_8(const void *data, unsigned int size)
{
    unsigned char *str = (unsigned char *)data;
    for (unsigned int i = 0; i < size - 1; ++i) {
        unsigned char c = str[i];
        printf(" %d,", c);
    }
    printf("\n\n");
    return;
}
static void PrintTensor_U16(const void *data, unsigned int size)
{
    unsigned short *str = (unsigned short *)data;
    for (unsigned int i = 0; i < size - 1; ++i) {
        unsigned short c = str[i];
        printf(" %u,", c);
    }
    printf("\n\n");
    return;
}
static void PrintTensor_16(const void *data, unsigned int size)
{
    short *str = (short *)data;
    for (unsigned int i = 0; i < size - 1; ++i) {
        short c = str[i];
        printf(" %d,", c);
    }
    printf("\n\n");
    return;
}
static void PrintTensor_U32(const void *data, unsigned int size)
{
    unsigned int *str = (unsigned int *)data;
    for (unsigned int i = 0; i < size - 1; ++i) {
        unsigned int c = str[i];
        printf(" %u,", c);
    }
    printf("\n\n");
    return;
}
static void PrintTensor_32(const void *data, unsigned int size)
{
    int *str = (int *)data;
    for (unsigned int i = 0; i < size - 1; ++i) {
        int c = str[i];
        printf(" %d,", c);
    }
    printf("\n\n");
    return;
}
static void PrintTensor_64(const void *data, unsigned int size)
{
    long *str = (long *)data;
    for (unsigned int i = 0; i < size - 1; ++i) {
        long c = str[i];
        printf(" %ld,", c);
    }
    printf("\n\n");
    return;
}

static void PrintTensor_fp(const void *data, unsigned int size)
{
    float *str = (float *)data;
    float c;
    for (unsigned int i = 0; i < size - 1; ++i) {
        c = str[i];
        printf("%6f,", c);
        if (i % kLineNum == kLineSplitNum) {
            printf(" \n");
        }
    }
    if (size > 0) {
        c = str[size - 1];
        printf(" %f", c);
    }
    printf("\n\n");
    return;
}

void PrintOutTensor(MsMicroTensor *tensor)
{
    if (tensor == NULL) {
        return;
    }
    unsigned int tensorSize = GetTensorElementSize(tensor);
    if (tensorSize > kPrintNums) {
        tensorSize = kPrintNums;
    }

    switch ((enum DataType)tensor->type) {
        case DataType_DT_FLOAT:
            PrintTensor_fp(tensor->data, tensorSize);
            break;
        case DataType_DT_FLOAT16:
            PrintTensor_fp(tensor->data, tensorSize);
            break;
        case DataType_DT_INT8:
            PrintTensor_8(tensor->data, tensorSize);
            break;
        case DataType_DT_INT32:
            PrintTensor_32(tensor->data, tensorSize);
            break;
        case DataType_DT_UINT8:
            PrintTensor_U8(tensor->data, tensorSize);
            break;
        case DataType_DT_INT16:
            PrintTensor_16(tensor->data, tensorSize);
            break;
        case DataType_DT_UINT32:
            PrintTensor_U32(tensor->data, tensorSize);
            break;
        case DataType_DT_INT64:
            PrintTensor_64(tensor->data, tensorSize);
            break;
        case DataType_DT_UINT16:
            PrintTensor_U16(tensor->data, tensorSize);
            break;
        default:
            printf("data type is not supported.\n\n\n");
    }
}
