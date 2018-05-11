;----------------------------------------------------------------------------
 ; Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 ; All rights reserved.
 ; Redistribution and use in source and binary forms, with or without modification,
 ; are permitted provided that the following conditions are met:
 ; 1. Redistributions of source code must retain the above copyright notice, this list of
 ; conditions and the following disclaimer.
 ; 2. Redistributions in binary form must reproduce the above copyright notice, this list
 ; of conditions and the following disclaimer in the documentation and/or other materials
 ; provided with the distribution.
 ; 3. Neither the name of the copyright holder nor the names of its contributors may be used
 ; to endorse or promote products derived from this software without specific prior written
 ; permission.
 ; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ; "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 ; THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 ; PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 ; CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 ; EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 ; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 ; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 ; WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 ; OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ; ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ;---------------------------------------------------------------------------*/
;----------------------------------------------------------------------------
 ; Notice of Export Control Law
 ; ===============================================
 ; Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 ; include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 ; Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 ; applicable export control laws and regulations.
 ;---------------------------------------------------------------------------*/

    PRESERVE8
    SECTION    .text:CODE(2)
    THUMB

    EXPORT  osExcNMI
    EXPORT  osExcHardFault
    EXPORT  osExcMemFault
    EXPORT  osExcBusFault
    EXPORT  osExcUsageFault
    EXPORT  osExcSvcCall

    IMPORT osExcHandleEntry
    IMPORT g_vuwLosFlag
    IMPORT g_uwCurNestCount
    IMPORT g_uwExcTbl
    IMPORT g_bTaskScheduled

OS_FLG_BGD_ACTIVE   EQU   0x0002

OS_EXC_CAUSE_NMI            EQU   16
OS_EXC_CAUSE_HARDFAULT      EQU   17

HF_DEBUGEVT     EQU   20
HF_VECTBL       EQU   21

FLAG_ADDR_VALID             EQU   0x10000
FLAG_HWI_ACTIVE             EQU   0x20000
FLAG_NO_FLOAT               EQU   0x10000000

OS_NVIC_FSR                 EQU   0xE000ED28      ;include BusFault/MemFault/UsageFault State Regeister
OS_NVIC_HFSR                EQU   0xE000ED2C      ;HardFault State Regeister
OS_NVIC_BFAR                EQU   0xE000ED38
OS_NVIC_MMAR                EQU   0xE000ED34
OS_NVIC_ACT_BASE            EQU   0xE000E300
OS_NVIC_SHCSRS              EQU   0xE000ED24
OS_NVIC_SHCSR_MASK          EQU   0xC00

osExcNMI
    MOV  R0, #OS_EXC_CAUSE_NMI
    MOV  R1, #0
    B  osExcDispatch

osExcHardFault
    MOV  R0, #OS_EXC_CAUSE_HARDFAULT
    LDR  R2, =OS_NVIC_HFSR
    LDR  R2, [R2]

    MOV  R1, #HF_DEBUGEVT
    ORR  R0, R0, R1, LSL #0x8
    TST  R2, #0x80000000
    BNE  osExcDispatch     ; DEBUGEVT

    AND  R0, R0 , #0x000000FF
    MOV  R1, #HF_VECTBL
    ORR  R0, R0, R1, LSL #0x8
    TST  R2, #0x00000002
    BNE  osExcDispatch     ; VECTBL

    ;if not DEBUGEVT and VECTBL then is FORCED
    AND  R0, R0, #0x000000FF

    LDR  R2, =OS_NVIC_FSR
    LDR  R2, [R2]

    TST  R2, #0x8000     ; BFARVALID
    BNE  _HFBusFault     ; BusFault

    TST  R2, #0x80       ; MMARVALID
    BNE  _HFMemFault     ; MemFault

    MOV  R12,#0
    B    osHFExcCommonBMU

_HFBusFault
    LDR  R1, =OS_NVIC_BFAR
    LDR  R1, [R1]
    MOV  R12, #FLAG_ADDR_VALID
    B    osHFExcCommonBMU

_HFMemFault
    LDR  R1, =OS_NVIC_MMAR
    LDR  R1, [R1]
    MOV  R12, #FLAG_ADDR_VALID

osHFExcCommonBMU
    CLZ  R2, R2
    LDR  R3, =g_uwExcTbl
    ADD  R3, R3, R2
    LDRB R2, [R3]
    ORR  R0, R0, R2, LSL #0x8
    ORR  R0, R0 ,R12
    B    osExcDispatch

osExcSvcCall
    TST   LR, #0x4
    ITE   EQ
    MRSEQ R0, MSP
    MRSNE R0, PSP
    LDR   R1, [R0,#24]
    LDRB  R0, [R1,#-2]
    MOV   R1, #0
    B     osExcDispatch

osExcBusFault
    LDR  R0, =OS_NVIC_FSR
    LDR  R0, [R0]

    TST  R0, #0x8000 ; BFARVALID
    BEQ  _ExcBusNoADDR
    LDR  R1, =OS_NVIC_BFAR
    LDR  R1, [R1]
    MOV  R12, #FLAG_ADDR_VALID
    AND  R0, R0, #0x1F00

    B    osExcCommonBMU

_ExcBusNoADDR
    MOV  R12,#0
    B    osExcCommonBMU

osExcMemFault
    LDR  R0, =OS_NVIC_FSR
    LDR  R0, [R0]

    TST  R0, #0x80 ; MMARVALID
    BEQ  _ExcMemNoADDR
    LDR  R1, =OS_NVIC_MMAR
    LDR  R1, [R1]
    MOV  R12, #FLAG_ADDR_VALID
    AND  R0, R0, #0x1B

    B    osExcCommonBMU

_ExcMemNoADDR
    MOV  R12,#0
    B    osExcCommonBMU

osExcUsageFault
    LDR  R0, =OS_NVIC_FSR
    LDR  R0, [R0]

    MOV  R1, #0x030F
    LSL  R1, R1, #16
    AND  R0, R0, R1
    MOV  R12, #0

osExcCommonBMU
    CLZ  R0, R0
    LDR  R3, =g_uwExcTbl
    ADD  R3, R3, R0
    LDRB R0, [R3]
    ORR  R0, R0, R12

; R0 -- EXCCAUSE(bit 16 is 1 if EXCADDR valid),  R1 -- EXCADDR
osExcDispatch
    LDR   R2, =OS_NVIC_ACT_BASE
    MOV   R12, #8                       ; R12 is hwi check loop counter

_hwiActiveCheck
    LDR   R3, [R2]                      ; R3 store active hwi register when exc
    CMP   R3, #0
    BEQ   _hwiActiveCheckNext

    ; exc occured in IRQ
    ORR   R0, R0, #FLAG_HWI_ACTIVE
    RBIT  R2, R3
    CLZ   R2, R2
    AND   R12, R12, #1
    ADD   R2, R2, R12, LSL #5               ; calculate R2 (hwi number) as uwPid

_ExcInMSP
    CMP   LR, #0XFFFFFFED
    BNE   _NoFloatInMsp
    ADD   R3, R13, #104
    PUSH  {R3}
    MRS   R12, PRIMASK                  ; store message-->exc: disable int?
    PUSH {R4-R12}                       ; store message-->exc: {R4-R12}
    VPUSH {D8-D15}
    B     _handleEntry

_NoFloatInMsp
    ADD   R3, R13, #32
    PUSH  {R3} ; save IRQ SP            ; store message-->exc: MSP(R13)

    MRS   R12, PRIMASK                  ; store message-->exc: disable int?
    PUSH {R4-R12}                       ; store message-->exc: {R4-R12}
    ORR   R0, R0, #FLAG_NO_FLOAT
    B     _handleEntry

_hwiActiveCheckNext
    ADD   R2, R2, #4                        ; next NVIC ACT ADDR
    SUBS  R12, R12, #1
    BNE   _hwiActiveCheck

    ;/*NMI interrupt excption*/
    LDR   R2, =OS_NVIC_SHCSRS
    LDRH  R2,[R2]
    LDR   R3,=OS_NVIC_SHCSR_MASK
    AND   R2, R2,R3
    CMP   R2,#0
    BNE   _ExcInMSP
    ; exc occured in Task or Init or exc
    ; reserved for register info from task stack

    LDR  R2, =g_bTaskScheduled
    LDR  R2, [R2]
    TST  R2, #1         ; OS_FLG_BGD_ACTIVE
    BEQ  _ExcInMSP                      ; if exc occured in Init then branch


    CMP   LR, #0xFFFFFFED               ;auto push floating registers
    BNE   _NoFloatInPsp

    ; exc occured in Task
    MOV   R2,  R13
    SUB   R13, #96                      ; add 8 Bytes reg(for STMFD)

    MRS   R3,  PSP
    ADD   R12, R3, #104
    PUSH  {R12}    ; save task SP

    MRS   R12, PRIMASK
    PUSH {R4-R12}
    VPUSH {D8-D15}

    ; copy auto saved task register

    LDMFD R3!, {R4-R11}                  ; R4-R11 store PSP reg(auto push when exc in task)
    VLDMIA  R3!, {D8-D15}
    VSTMDB  R2!, {D8-D15}
    STMFD R2!, {R4-R11}
    B     _handleEntry

_NoFloatInPsp
    MOV   R2,  R13                           ;no auto push floating registers
    SUB   R13, #32                      ; add 8 Bytes reg(for STMFD)

    MRS   R3,  PSP
    ADD   R12, R3, #32
    PUSH  {R12}    ; save task SP

    MRS   R12, PRIMASK
    PUSH {R4-R12}

    LDMFD R3, {R4-R11}                  ; R4-R11 store PSP reg(auto push when exc in task)
    STMFD R2!, {R4-R11}
    ORR   R0, R0, #FLAG_NO_FLOAT

_handleEntry
    MOV R3, R13                         ; R13:the 4th param
    CPSID I
    CPSID F
    B  osExcHandleEntry

    NOP
    END
