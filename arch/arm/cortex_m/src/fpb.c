/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description : LiteOS arm-m flash patch module implemention.
 * Author : Huawei LiteOS Team
 * Create : 2018-03-07
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ---------------------------------------------------------------------------- */
#include "arch/fpb.h"
#include "los_printf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define FPB_REG_MAX         254
#define LITERAL_REG1_INDEX  6
#define LITERAL_REG2_INDEX  7
#define S_POS               24
#define FPB_BASE            0xE0002000UL
#define REMAP_TABLE_ADDR    0x20000000UL
#define ROM_BASE            0x02200000UL
#define ROM_END             0x02214000UL
#define BL_DISTANCE_MAX     (1U << 24)
#define NOP_INSTR           0xbf00bf00
#define FPB                 ((FpbReg *) FPB_BASE)
#define REMAP_UNIT_SIZE     4
#define UPPER_MASK          0xffff0000
#define LOWER_MASK          0x0000ffff
#define SHIFT_BITS          16
#define LITERAL_ALIGN       3
#define INSTR_ALIGN         2

typedef struct {
    volatile UINT32 ctrl;
    volatile UINT32 remap;
    volatile UINT32 comp[FPB_REG_MAX];
} FpbReg;

STATIC VOID FpbControlEnable(VOID);

STATIC VOID FpbRemapRegConfig(UINT32 remapTableAddr);

STATIC UINT32 FpbComparatorRegConfig(UINT8 regIndex, UINT32 compAddr);

STATIC UINT32 FpbRedirectLiteral(UINT32 originalLiteralAddr, UINT32 targetLiteral);

STATIC UINT32 CalcBranchInstr(UINT32 instrAddr, UINT32 targetAddr);

STATIC UINT32 CalcBranchWLinkInstr(UINT32 instrAddr, UINT32 targetAddr);

STATIC UINT32 LittleEndian16Bit(UINT32 val);

STATIC UINT32 GetInstr(UINT32 instrAddr, UINT32 targetAddr, UINT32 *newInstr, UINT8 blInstr);

STATIC UINT32 GetInstrRegIndex(UINT32 oldAddr, UINT8 *regComp);

VOID FpbInit(VOID)
{
    UINT32 regCount;

    FpbControlEnable();
    FpbRemapRegConfig(REMAP_TABLE_ADDR);

    for (regCount = 0; regCount < FPB_REG_MAX; regCount++) {
        FPB->comp[regCount] = 0;
    }
}

UINT32 FpbAddPatch(UINT32 oldAddr, UINT32 patchValue, FpbCompTypeEnum fpbType)
{
    UINT32 newInstr;
    UINT32 ret;
    UINT8  regIndex;

    if (fpbType >= FPB_TYPE_MAX) {
        PRINT_ERR("type is wrong, set fpb patch err!\r\n");
        return FPB_TYPE_ERR;
    }

    if (fpbType == FPB_TYPE_LITERAL) {
        if (oldAddr & LITERAL_ALIGN) { // check aligned
            PRINT_ERR("addr is not aligned to 4!\r\n");
            return FPB_ADDR_NOT_ALIGN_ERR;
        }

        ret = FpbRedirectLiteral(oldAddr, patchValue);
        if (ret != FPB_SUCCESS) {
            PRINT_ERR("set literal patch err!\r\n");
            return ret;
        }
    } else {
        if (oldAddr & INSTR_ALIGN) { // check aligned
            PRINT_ERR("addr is not aligned to 4!\r\n");
            return FPB_ADDR_NOT_ALIGN_ERR;
        }

        ret = GetInstrRegIndex(oldAddr, &regIndex);
        if (ret != FPB_SUCCESS) {
            return ret;
        }

        ret = GetInstr(oldAddr, patchValue, &newInstr, FALSE);
        if (ret != FPB_SUCCESS) {
            return ret;
        }

        PRINT_DEBUG("new_instr:%x, ", new_instr);

        ret = FpbComparatorRegConfig(regIndex, oldAddr);
        if (ret != FPB_SUCCESS) {
            return ret;
        }

        PRINT_DEBUG("use COMP:%d\r\n", reg_index);
        *((UINT32 *)(UINTPTR)(REMAP_TABLE_ADDR + (regIndex * REMAP_UNIT_SIZE))) = newInstr;
    }

    return FPB_SUCCESS;
}

UINT32 FpbDeletePatch(UINT32 oldAddr, FpbCompTypeEnum fpbType)
{
    UINT8  regIndex;
    UINT32 regValue;
    UINT32 ret;

    if (fpbType >= FPB_TYPE_MAX) {
        PRINT_ERR("type is wrong, set pfb patch err!\r\n");
        return FPB_TYPE_ERR;
    }

    if (fpbType == FPB_TYPE_LITERAL) {
        regValue = FPB->comp[LITERAL_REG1_INDEX];
        if ((oldAddr | 0x01UL) == regValue) {
            FPB->comp[LITERAL_REG1_INDEX] = 0;
            return FPB_SUCCESS;
        } else {
            regValue = FPB->comp[LITERAL_REG2_INDEX];
            if ((oldAddr | 0x01UL) == regValue) {
                FPB->comp[LITERAL_REG2_INDEX] = 0;
                return FPB_SUCCESS;
            }
        }
    } else {
        ret = GetInstrRegIndex(oldAddr, &regIndex);
        if (ret == FPB_COMP_REPEAT_ERR) {
            PRINT_DEBUG("delete comp : %d patch\r\n", regIndex);
            FPB->comp[regIndex] = 0;
            return FPB_SUCCESS;
        }
    }

    PRINT_DEBUG("no patch need to delete\r\n");
    return FPB_NO_COMP_ERR;
}

STATIC UINT32 GetInstrRegIndex(UINT32 oldAddr, UINT8 *regComp)
{
    UINT32 regValue;
    UINT8 regCount;
    UINT8 flag      = 0;

    for (regCount = 0; regCount < FPB_REG_MAX; regCount++) {
        if (regCount == LITERAL_REG1_INDEX) {
            regCount++;
            continue;
        }

        regValue = FPB->comp[regCount];
        if (regValue & 0x01UL) {
            if (regValue == (oldAddr | 0x01UL)) {
                PRINT_WARN("the old_func_addr already has patched\r\n");
                *regComp = regCount;
                return FPB_COMP_REPEAT_ERR;
            }
        } else {
            if (flag == 0) {
                *regComp = regCount;
                flag = 1;
            }
        }
    }

    if ((regCount == FPB_REG_MAX) && (flag == 0)) {
        PRINT_WARN("there is no free fpb comp regiter\r\n");
        return FPB_NO_FREE_COMP_ERR;
    }

    return FPB_SUCCESS;
}

STATIC UINT32 FpbRedirectLiteral(UINT32 originalLiteralAddr, UINT32 targetLiteral)
{
    UINT8  regIndex;
    UINT32 ret;
    UINT32 literalAddr = originalLiteralAddr | 0x01UL;
    UINT32 literalReg1Value = FPB->comp[LITERAL_REG1_INDEX];
    UINT32 literalReg2Value = FPB->comp[LITERAL_REG2_INDEX];

    if ((originalLiteralAddr < ROM_BASE) || (originalLiteralAddr >= ROM_END)) {
        return FPB_TARGET_ADDR_ERR;
    }

    if ((literalReg1Value == literalAddr) || (literalReg2Value == literalAddr)) {
        return FPB_COMP_REPEAT_ERR;
    }

    if (((literalReg1Value & 0x1) != 0) && ((literalReg2Value & 0x1) != 0)) {
        return FPB_NO_FREE_COMP_ERR;
    }

    if ((literalReg1Value & 0x1) == 0) {
        regIndex = LITERAL_REG1_INDEX;
    } else {
        regIndex = LITERAL_REG2_INDEX;
    }

    ret = FpbComparatorRegConfig(regIndex, originalLiteralAddr);
    if (ret != FPB_SUCCESS) {
        return ret;
    }

    *((UINT32 *)(UINTPTR)(REMAP_TABLE_ADDR + (regIndex * REMAP_UNIT_SIZE))) = targetLiteral;
    return FPB_SUCCESS;
}

STATIC VOID FpbControlEnable(VOID)
{
    FPB->ctrl = 0x03UL; // set KEY bit & enable bit
}

STATIC VOID FpbRemapRegConfig(UINT32 remapTableAddr)
{
    FPB->remap = remapTableAddr;
}

STATIC UINT32 FpbComparatorRegConfig(UINT8 regIndex, UINT32 compAddr)
{
    if (regIndex >= FPB_REG_MAX) {
        PRINT_ERR("reg_index:%d is out of range, REG_MAX is %d\r\n", regIndex, FPB_REG_MAX);
        return FPB_NO_COMP_ERR;
    }

    FPB->comp[regIndex] = (compAddr | 0x01UL);
    return FPB_SUCCESS;
}

STATIC UINT32 CalcBranchInstr(UINT32 instrAddr, UINT32 targetAddr)
{
    INT32 distance = (INT32)(targetAddr - instrAddr);
    if (distance == 0) {
        PRINT_ERR("patch addr should not be same as buggy addr\r\n");
        return NOP_INSTR;
    }

    if (distance > BL_DISTANCE_MAX) {
        PRINT_ERR("the bl instr should whitin 16M\r\n");
        return NOP_INSTR;
    }

    if ((distance + BL_DISTANCE_MAX) < 0) {
        PRINT_ERR("the bl instr should whitin -16M\r\n");
        return NOP_INSTR;
    }

    UINT32 offset = (UINT32)(distance - REMAP_UNIT_SIZE);
    PRINT_DEBUG("instr_addr:%x, target_addr:%x\r\n", instrAddr, targetAddr);

    UINT16 offset10Upper = ((offset) >> 12) & 0x03FF; // get upper 10 bits [21:12]
    UINT16 offset11Lower = ((offset) >> 1)  & 0x07FF; // get lower 11 bits [11:1]

    UINT8 s  = (offset >> S_POS) & 0x1;       // get bit 24
    UINT8 i1 = (offset >> (S_POS - 1)) & 0x1; // get bit 23
    UINT8 i2 = (offset >> (S_POS - 2)) & 0x1; // get bit 22

    UINT8 j1 = 0x01 & ((~i1) ^ s);
    UINT8 j2 = 0x01 & ((~i2) ^ s);

    // upper instruction : [15:11]opcode1 0x1e [10]S [9:0] imm upper 10 bits
    UINT32 upperBlInstr =  ((0x1E << 11) | (s << 10) | offset10Upper);
    // lower instruction : [15:14]opcode2 0x2  [13]J1 [12]opcode3 [11]J2 [10:0] imm lower 11 bits
    UINT32 lowerBlInstr =  ((0x02 << 14) | (j1 << 13) | (0x01 << 12) | (j2 << 11) | offset11Lower);

    return ((upperBlInstr << SHIFT_BITS) | lowerBlInstr); // assembling 32bit instruction
}

STATIC UINT32 CalcBranchWLinkInstr(UINT32 instrAddr, UINT32 targetAddr)
{
    UINT32 branchInstr = CalcBranchInstr(instrAddr, targetAddr);
    if (branchInstr == NOP_INSTR) {
        return NOP_INSTR;
    }

    return (branchInstr | 0x00004000); // Set bit 14. This is the only difference between B and BL instructions
}

STATIC UINT32 LittleEndian16Bit(UINT32 val)
{
    return ((val & UPPER_MASK) >> SHIFT_BITS) | ((val & LOWER_MASK) << SHIFT_BITS); // little_endian swap
}

STATIC UINT32 GetInstr(UINT32 instrAddr, UINT32 targetAddr, UINT32 *newInstr, UINT8 blInstr)
{
    UINT32 tmpInstr;

    if (blInstr != 0) {
        tmpInstr = CalcBranchWLinkInstr(instrAddr, targetAddr);
    } else {
        tmpInstr = CalcBranchInstr(instrAddr, targetAddr);
    }

    if (tmpInstr == NOP_INSTR) {
        return FPB_TARGET_ADDR_ERR;
    }
    if (((instrAddr & (~0x1)) & INSTR_ALIGN) == 0) { // check aligned
        *newInstr = LittleEndian16Bit(tmpInstr);
    } else {
        PRINT_ERR("target addr is not 4 aligned,not support!\r\n");
        return FPB_ADDR_NOT_ALIGN_ERR;
    }

    return FPB_SUCCESS;
}

VOID FpbDisable(VOID)
{
    FPB->ctrl = 0x02UL; // set KEY bit enable write
    FPB->ctrl = 0x0UL;  // disable fpb
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
