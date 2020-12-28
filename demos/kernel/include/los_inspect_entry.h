/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS Kernel Demo Inspect Entry HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

#ifndef _LOS_INSPECT_ENTRY_H
#define _LOS_INSPECT_ENTRY_H

#include "los_typedef.h"

/* LiteOS Inspect status. */
typedef enum {
    LOS_INSPECT_STU_START         =  0xFF,
    LOS_INSPECT_STU_ERROR         =  0x01,
    LOS_INSPECT_STU_SUCCESS       =  0x00,
} enInspectStu;

/* LiteOS Inspect result. */
typedef enum {
    LOS_INSPECT_TASK = 0,
    LOS_INSPECT_DMEM,
    LOS_INSPECT_SMEM,
    LOS_INSPECT_INTERRUPT,
    LOS_INSPECT_MSG,
    LOS_INSPECT_EVENT,
    LOS_INSPECT_MUTEX,
    LOS_INSPECT_SEM,
    LOS_INSPECT_SYSTIC,
    LOS_INSPECT_TIMER,
    LOS_INSPECT_LIST,
    LOS_INSPECT_BUFF
} enInspectId;

typedef struct os_Inspect_def {
    enInspectId InspectId;
    enInspectStu Status;
    UINT32 (*Inspectfunc)(VOID);
    CHAR name[20];
} osInspect_Def;

extern UINT32 KernelDemoInspectEntry(VOID);
extern UINT32 LOS_InspectByID(enInspectId InspectId);
extern UINT32 LOS_InspectStatusSetById(enInspectId InspectId, enInspectStu InspectStu);

#endif /* _LOS_INSPECT_ENTRY_H */
