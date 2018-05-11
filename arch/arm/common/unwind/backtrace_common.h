/*
 * arch/arm/include/asm/unwind.h
 *
 * Copyright (C) 2008 ARM Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef __ASM_UNWIND_H
#define __ASM_UNWIND_H


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifndef __ASSEMBLY__

#define  OS_BACKTRACE_DEPTH  10

/* Unwind reason code according the the ARM EABI documents */
enum unwind_reason_code {
    URC_OK = 0,            /* operation completed successfully */
    URC_CONTINUE_UNWIND = 8,
    URC_FAILURE = 9            /* unspecified failure of some kind */
};

struct unwind_idx {
    unsigned long addr_offset;
    unsigned long insn;
};

struct list_head {
    struct list_head *pstNext, *pstPrev;
};

struct stackframe {
    /*
     * FP member should hold R7 when CONFIG_THUMB2_KERNEL is enabled
     * and R11 otherwise.
     */
    unsigned long fp;
    unsigned long sp;
    unsigned long lr;
    unsigned long pc;
};

typedef struct backtrace
{
    void *function;
    void *address;
    const char *name;
} backtrace_t;

extern void unwind_backtrace(backtrace_t *buffer, int size, struct stackframe frame);

#endif    /* !__ASSEMBLY__ */

#ifdef CONFIG_ARM_UNWIND
#define UNWIND(code...)        code
#else
#define UNWIND(code...)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif    /* __ASM_UNWIND_H */
