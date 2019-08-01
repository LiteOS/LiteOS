/** ----------------------------------------------------------------------------
 * Copyright (c) <2019>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
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
 *---------------------------------------------------------------------------*/
/** ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/****************************************************************************************
*                                  EXPORT FUNCTIONS
****************************************************************************************/

    .global  SVC_Handler

/****************************************************************************************
*                                  EXTERN PARAMETERS
****************************************************************************************/

    .extern svc_tables

/****************************************************************************************
*                                  CODE GENERATION DIRECTIVES
****************************************************************************************/

    .syntax unified
    .arch armv6-m
    .thumb
    .section .text

/****************************************************************************************
* Function:
*        VOID SVC_Handler(VOID);
* Description:
*        SVC Handler.
****************************************************************************************/
    .type SVC_Handler, %function
SVC_Handler:
    MRS   R3, CONTROL
    MOVS  R1, #0
    MSR   CONTROL, R1

    /* if LR bit 2 is 0, using MSP, 1 using PSP */

    MOVS  R1, #4
    MOV   R0, LR
    TST   R0, R1
    MOV   R0, SP
    BEQ   0f
    MRS   R0, PSP
0:
    LDR   R1, [R0, #4 * 6]      /* get original PC */
    ORRS  R1, R3                /* get CONTROL.nPRIV (usr-bit) */
    STR   R1, [R0, #4 * 4]      /* save original (PC | usr-bit) in R12 */

    ADR   R1, __svc_usr_stub
    STR   R1, [R0, #4 * 6]

    BX    LR

    NOP

__svc_usr_stub:
    PUSH  {R4, R5}
    MOV   R4, R12
    PUSH  {R4, LR}

    LDR   R4, =svc_tables

    LSRS  R5, r7, #22           /* get module id in r0 */
    LDR   R4, [r4, r5]

    CMP   R4, #0
    BEQ   2f

    LSLS  R5, r7, #16           /* clean high 16 bits */
    LSRS  R5, r5, #14           /* get svc id in r1 */

    LDR   R4, [r4, r5]

    CMP   R4, #0
    BEQ   2f

    BLX   R4
0:
    POP   {R2, R3}
    POP   {R4, R5}
    MOV   LR, R3

    MOVS  R3, #1
    TST   R2, R3
    BEQ   1f
    MOVS  R3, #3
    MSR   CONTROL, R3
    BX    R2
1:
    ORRS  R2, R3
    BX    R2

2:
    MOVS  R0, #0                /* return -1 on error */
    MVNS  R0, R0
    B     0b

