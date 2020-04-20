/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: M7 Hw Exc Implementation
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
 * --------------------------------------------------------------------------- */
 /* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

    .syntax unified
    .arch armv7-m
    .thumb
    .fpu vfpv4
    .section .text


    .global  OsExcNMI
    .global  OsExcHardFault
    .global  OsExcMemFault
    .global  OsExcBusFault
    .global  OsExcUsageFault
    .global  OsExcSvcCall

    .extern  OsExcHandleEntry
    .extern  g_excTbl
    .extern  g_taskScheduled

    .equ OS_EXC_CAUSE_NMI               , 16
    .equ OS_EXC_CAUSE_HARDFAULT         , 17
    .equ HF_DEBUGEVT                    , 20
    .equ HF_VECTBL                      , 21
    .equ OS_EXC_FLAG_FAULTADDR_VALID    , 0x10000
    .equ OS_EXC_FLAG_IN_HWI             , 0x20000
    .equ OS_EXC_FLAG_NO_FLOAT           , 0x10000000
    .equ OS_NVIC_FSR                    , 0xE000ED28 /* include BusFault/MemFault/UsageFault State Regeister */
    .equ OS_NVIC_HFSR                   , 0xE000ED2C /* HardFault State Regeister */
    .equ OS_NVIC_BFAR                   , 0xE000ED38
    .equ OS_NVIC_MMAR                   , 0xE000ED34
    .equ OS_NVIC_ACT_BASE               , 0xE000E300
    .equ OS_NVIC_SHCSRS                 , 0xE000ED24
    .equ OS_NVIC_SHCSR_MASK             , 0xC00
    .equ EXCEPT_FRAME_OFFSET_PC         , 0x06 * 4 /* see cortex-m7 reference manual: chapter 2.3.7 */

    .type OsExcNMI, %function
    .global OsExcNMI
OsExcNMI:
    .fnstart
    .cantunwind
    MOV  R0, #OS_EXC_CAUSE_NMI
    MOV  R1, #0
    B    OsExcDispatch
    .fnend

    .type OsExcHardFault, %function
    .global OsExcHardFault
OsExcHardFault:
    .fnstart
    .cantunwind
    MOV  R0, #OS_EXC_CAUSE_HARDFAULT
    LDR  R2, =OS_NVIC_HFSR
    LDR  R2, [R2]

    MOV  R1, #HF_DEBUGEVT
    ORR  R0, R0, R1, LSL #0x8
    TST  R2, #0x80000000
    BNE  OsExcDispatch /* DEBUGEVT */

    AND  R0, #0x000000FF
    MOV  R1, #HF_VECTBL
    ORR  R0, R0, R1, LSL #0x8
    TST  R2, #0x00000002
    BNE  OsExcDispatch /* VECTBL */

    /* if not DEBUGEVT and VECTBL then is FORCED */
    AND  R0, #0x000000FF

    MRS  R2, MSP
    LDR  R1, [R2, EXCEPT_FRAME_OFFSET_PC]

    LDR  R2, =OS_NVIC_FSR
    LDR  R2, [R2]

    TST  R2, #0x8000 /* BFARVALID */
    BNE  HFBusFault /* BusFault */

    TST  R2, #0x80 /* MMARVALID */
    BNE  HFMemFault /* MemFault */

    MOV  R12,#0
    B    OsHFExcCommonBMU
    .fnend

    .type HFBusFault, %function
HFBusFault:
    .fnstart
    .cantunwind
    LDR  R1, =OS_NVIC_BFAR
    LDR  R1, [R1]
    MOV  R12, #OS_EXC_FLAG_FAULTADDR_VALID
    B    OsHFExcCommonBMU
    .fnend

    .type HFMemFault, %function
HFMemFault:
    .fnstart
    .cantunwind
    LDR  R1, =OS_NVIC_MMAR
    LDR  R1, [R1]
    MOV  R12, #OS_EXC_FLAG_FAULTADDR_VALID
    .fnend

    .type OsHFExcCommonBMU, %function
    .global OsHFExcCommonBMU
OsHFExcCommonBMU:
    .fnstart
    .cantunwind
    CLZ  R2, R2
    LDR  R3, =g_excTbl
    ADD  R3, R3, R2
    LDRB R2, [R3]
    ORR  R0, R0, R2, LSL #0x8
    ORR  R0, R12
    B    OsExcDispatch
    .fnend

    .type OsExcSvcCall, %function
    .global OsExcSvcCall
OsExcSvcCall:
    .fnstart
    .cantunwind
    TST   LR, #0x4
    ITE   EQ
    MRSEQ R0, MSP
    MRSNE R0, PSP
    LDR   R1, [R0,#24]
    LDRB  R0, [R1,#-2]
    MOV   R1, #0
    B     OsExcDispatch
    .fnend

    .type OsExcBusFault, %function
    .global OsExcBusFault
OsExcBusFault:
    .fnstart
    .cantunwind
    LDR  R0, =OS_NVIC_FSR
    LDR  R0, [R0]

    TST  R0, #0x8000 /* BFARVALID */
    BEQ  ExcBusNoAddr
    LDR  R1, =OS_NVIC_BFAR
    LDR  R1, [R1]
    MOV  R12, #OS_EXC_FLAG_FAULTADDR_VALID
    AND  R0, #0x1F00

    B    OsExcCommonBMU
    .fnend

    .type ExcBusNoAddr, %function
ExcBusNoAddr:
    .fnstart
    .cantunwind
    MOV  R12,#0
    B    OsExcCommonBMU
    .fnend

    .type OsExcMemFault, %function
    .global OsExcMemFault
OsExcMemFault:
    .fnstart
    .cantunwind
    LDR  R0, =OS_NVIC_FSR
    LDR  R0, [R0]

    TST  R0, #0x80 /* MMARVALID */
    BEQ  ExcMemNoAddr
    LDR  R1, =OS_NVIC_MMAR
    LDR  R1, [R1]
    MOV  R12, #OS_EXC_FLAG_FAULTADDR_VALID
    AND  R0, #0x1B

    B    OsExcCommonBMU
    .fnend

    .type ExcMemNoAddr, %function
ExcMemNoAddr:
    .fnstart
    .cantunwind
    MOV  R12,#0
    B    OsExcCommonBMU
    .fnend

    .type OsExcUsageFault, %function
    .global OsExcUsageFault
OsExcUsageFault:
    LDR  R0, =OS_NVIC_FSR
    LDR  R0, [R0]

    LDR  R1, =#0x030F
    LSL  R1, #16
    AND  R0, R1
    MOV  R12, #0

OsExcCommonBMU:
    CLZ  R0, R0
    LDR  R3, =g_excTbl
    ADD  R3, R3, R0
    LDRB R0, [R3]
    ORR  R0, R0, R12

/* R0 -- EXCCAUSE(bit 16 is 1 if EXCADDR valid),  R1 -- EXCADDR */
OsExcDispatch:
    LDR   R2, =OS_NVIC_ACT_BASE
    MOV   R12, #8 /* R12 is hwi check loop counter */

HwiActiveCheck:
    LDR   R3, [R2] /* R3 store active hwi register when exc */
    CMP   R3, #0
    BEQ   HwiActiveCheckNext

    /* exc occured in IRQ */
    ORR   R0, #OS_EXC_FLAG_IN_HWI
    RBIT  R2, R3
    CLZ   R2, R2
    AND   R12, #1
    ADD   R2, R2, R12, LSL #5 /* calculate R2 (hwi number) as pid, thrid parameter */

ExcInMSP:
    CMP   LR, #0xFFFFFFED
    BNE   NoFloatInMsp
    ADD   R3, R13, #104
    PUSH  {R3}
    MRS   R12, PRIMASK /* store message-->exc: disable int */
    PUSH  {R4-R12} /* store message-->exc: {R4-R12} */
    VPUSH {D8-D15}
    B     HandleEntry

NoFloatInMsp:
    ADD   R3, R13, #32
    PUSH  {R3} /* save IRQ SP, store message-->exc: MSP(R13) */

    MRS   R12, PRIMASK /* store message-->exc: disable int? */
    PUSH  {R4-R12} /* store message-->exc: {R4-R12} */
    ORR   R0, R0, #OS_EXC_FLAG_NO_FLOAT
    B     HandleEntry

HwiActiveCheckNext:
    ADD   R2, #4 /* next NVIC ACT ADDR */
    SUBS  R12, #1
    BNE   HwiActiveCheck

    /* NMI interrupt excption */
    LDR   R2, =OS_NVIC_SHCSRS
    LDRH  R2,[R2]
    LDR   R3,=OS_NVIC_SHCSR_MASK
    AND   R2, R2,R3
    CMP   R2,#0
    BNE   ExcInMSP

    /* exc occured in Task or Init or exc reserved for register info from task stack */
    LDR   R2, =g_taskScheduled
    LDR   R2, [R2]
    TST   R2, #1 /*os scheduled */
    BEQ   ExcInMSP /* if exc occured in Init then branch */


    CMP   LR, #0xFFFFFFED /*auto push floating registers */
    BNE   NoFloatInPsp

    /* exc occured in Task */
    MOV   R2,  R13
    SUB   R13, #96 /* add 8 Bytes reg(for STMFD) */

    MRS   R3,  PSP
    ADD   R12, R3, #104
    PUSH  {R12} /* save task SP */

    MRS   R12, PRIMASK
    PUSH  {R4-R12}
    VPUSH {D8-D15}

    /* copy auto saved task register */
    LDMFD   R3!, {R4-R11} /* R4-R11 store PSP reg(auto push when exc in task) */
    VLDMIA  R3!, {D8-D15}
    VSTMDB  R2!, {D8-D15}
    STMFD   R2!, {R4-R11}
    B       HandleEntry

NoFloatInPsp:
    MOV   R2,  R13 /*no auto push floating registers */
    SUB   R13, #32 /* add 8 Bytes reg(for STMFD) */

    MRS   R3,  PSP
    ADD   R12, R3, #32
    PUSH  {R12} /* save task SP */

    MRS   R12, PRIMASK
    PUSH  {R4-R12}

    LDMFD R3, {R4-R11} /* R4-R11 store PSP reg(auto push when exc in task) */
    STMFD R2!, {R4-R11}
    ORR   R0, R0, #OS_EXC_FLAG_NO_FLOAT

HandleEntry:
    MOV   R3, R13 /* R13:the 4th param */
    CPSID I
    CPSID F
    B     OsExcHandleEntry

    NOP
