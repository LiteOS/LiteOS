/*
 * Copyright (c) 2017-2018 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== sys/_internal.h ========
 *  Vendor specific internal details
 */

#ifndef ti_posix_gcc_sys__internal__include
#define ti_posix_gcc_sys__internal__include

/* compiler vendor check */
#ifndef __GNUC__
#error Incompatible compiler: use this include path (.../ti/posix/gcc) only with a GNU compiler. You appear to be using a different compiler.
#endif

#include <stdint.h>     /* C99 standard integer types */

#ifdef __cplusplus
extern "C" {
#endif

/*  These opaque object types replicate the kernel internal object
 *  structures. They are used in a union when defining an opaque
 *  object in order to guarantee the opaque object is correctly sized
 *  and aligned with respect to the kernel objects.
 */
enum Opaque_Mode {
    Opaque_Mode_1,
    Opaque_Mode_2,
    Opaque_Mode_3,
    Opaque_Mode_4
};

struct Opaque_Struct__;

struct Opaque_QueueElem {
    struct Opaque_QueueElem *volatile next;
    struct Opaque_QueueElem *volatile prev;
};

struct Opaque_QueueStruct {
    struct Opaque_QueueElem __f0;
    struct Opaque_Struct__ *__f1;
};

struct sysbios_Semaphore {
    struct Opaque_Struct__ *__f0;
    unsigned int __f1;
    enum Opaque_Mode __f2;
    volatile uint_least16_t __f3;
    struct Opaque_QueueStruct __f4;
    struct Opaque_Struct__ *__f5;
};

struct freertos_Semaphore {
    void *__f0;
};

struct sysbios_Barrier {
    struct sysbios_Semaphore sem;
    int count;
    int pendCount;
};

struct freertos_Barrier {
    int count;
    int pendCount;
    struct Opaque_Struct__ *waitList;
    struct Opaque_Struct__ *last;
};

struct sysbios_Mutex {
    struct Opaque_Struct__ *owner;
    int lockCnt;
    int type;
    struct sysbios_Semaphore sem;
    struct Opaque_Struct__ *mpo;
};

struct freertos_Mutex {
    int protocol;
    void *owner;
    int type;
    void *sem; /* struct freertos_Semaphore */
};

struct sysbios_RWLock {
    struct sysbios_Semaphore sem;
    struct sysbios_Semaphore readSem;
    int activeReaderCnt;
    int blockedReaderCnt;
    void *owner;
};

struct freertos_RWLock {
    struct freertos_Semaphore sem;
    struct freertos_Semaphore readSem;
    int activeReaderCnt;
    int blockedReaderCnt;
    void *owner;
};

struct sysbios_Cond {
    struct Opaque_QueueStruct waitList;
    uint32_t clockId;
};

struct freertos_Cond {
    struct Opaque_QueueElem waitList;
    uint32_t clockId;
};


#ifdef __cplusplus
}
#endif

#endif /* ti_posix_gcc_sys__internal__include */
