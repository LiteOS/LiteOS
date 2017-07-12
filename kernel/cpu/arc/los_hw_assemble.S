/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * 
 * Copyright (c) 2017, Synopsys, Inc.
 * Modified for port to ARC processor by Wayne Ren wei.ren@synopsys.com 
 *
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
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
/*
 * core-dependent part in assemble language (for arc)
 */
#include "inc/arc/arc.h"
#include "inc/arc/arc_asm_common.h"


#define OS_TASK_STATUS_RUNNING_BIT 7
/*
 *  task dispatcher
 *
 */
	.text
	.align 4
	.global osTaskSchedule
osTaskSchedule:
/*
 *  the pre-conditions of this routine are task context, CPU is
 *  locked, dispatch is enabled.
 */	
	sr 	blink, [AUX_ERRET]
	lr 	blink, [AUX_STATUS32]
	sr 	blink, [AUX_ERSTATUS]
	EXCEPTION_PROLOGUE
	SAVE_CALLEE_REGS
	b 	dispatcher

/*
 *  start dispatch
 */
	.global LOS_StartToRun
	.align 4
LOS_StartToRun:
/*
 *  this routine is called in the non-task conext during the startup of the kernel
 *  , and all the interrupts are locked.
 *
 *  when the dispatcher is called, the cpu is locked, no nest exception (CPU exception/interrupt).
 *  In target_initialize, all interrupt priority mask should be cleared, cpu should be
 *  locked, the interrupts outside the kernel such as fiq can be
 *  enabled.
 */
	clri
	//  g_stLosTask.pstRunTask = g_stLosTask.pstNewTask;
	ld 	r2, [g_stLosTask, 4]
	st 	r2, [g_stLosTask]
	//   g_stLosTask.pstRunTask->usTaskStatus |=  OS_TASK_STATUS_RUNNING;	
	ldh 	r1, [r2, 4]   
	bset 	r1, r1, OS_TASK_STATUS_RUNNING_BIT //
	sth 	r1, [r2, 4]
	//   g_bTaskScheduled = 1;
	mov 	r0, 1
	st 	r0, [g_bTaskScheduled]

	mov 	r0, 0
	st 	r0, [exc_nest_count]
	b 	dispatcher_0

dispatcher:
	ld	r0, [g_stLosTask]
	st 	sp, [r0]
	ldh	r1, [r0, 4]
	bclr	r1, r1, OS_TASK_STATUS_RUNNING_BIT
	sth 	r1, [r0, 4]

	ld 	r2, [g_stLosTask, 4]
	st 	r2, [g_stLosTask]

	ldh 	r1, [r2, 4]
	bset	r1, r1, OS_TASK_STATUS_RUNNING_BIT
	sth 	r1, [r2, 4]
// recover the new running task	
dispatcher_0:
	ld	sp, [r2]	/* recover task stack */
	RESTORE_CALLEE_REGS
	EXCEPTION_EPILOGUE
	rtie



/****** exceptions and interrupts handing ******/
/****** entry for exception handling ******/
	.global exc_entry_cpu
	.align 4
exc_entry_cpu:

	EXCEPTION_PROLOGUE

	mov	blink,	sp
	mov	r3, sp		/* as exception handler's para(p_excinfo) */

	ld	r0, [exc_nest_count]
	add	r1, r0, 1
	st	r1, [exc_nest_count]
	cmp	r0, 0
	bne	exc_handler_1
/* chang to interrupt stack if interrupt happened in task context */
	mov	sp, _e_stack
exc_handler_1:
	PUSH	blink

	lr	r0, [AUX_ECR]
	lsr	r0, r0, 16
	mov	r1, exc_int_handler_table
	ld.as	r2, [r1, r0]

	mov	r0, r3
	jl	[r2]		/* !!!!jump to exception handler where interrupts are not allowed! */

/* interrupts are not allowed */
ret_exc:
	POP	sp
	mov	r1, exc_nest_count
	ld	r0, [r1]
	sub	r0, r0, 1
	cmp	r0, 0
	bne.d	ret_exc_r_1
	st	r0, [r1]

	ld	r0, [context_switch_reqflg]
	cmp	r0, 0
	beq	ret_exc_r_1
	/* clear dispatch request */
	mov	r0, 0
	st	r0, [context_switch_reqflg]

	SAVE_CALLEE_REGS	/* save callee save registers */
	b	dispatcher	/* r0->pxCurrentTCB */

ret_exc_r_1:
	EXCEPTION_EPILOGUE
	rtie

/****** entry for normal interrupt exception handling ******/
	.global exc_entry_int	/* entry for interrupt handling */
	.align 4
exc_entry_int:
	clri	/* disable interrupt */

#if ARC_FEATURE_FIRQ == 1
#if ARC_FEATURE_RGF_NUM_BANKS > 1
	lr	r0, [AUX_IRQ_ACT]			/*  check whether it is P0 interrupt */
	btst	r0, 0
	jnz	exc_entry_firq
#else
	PUSH	r10
	lr	r10, [AUX_IRQ_ACT]
	btst	r10, 0
	POP	r10
	jnz	exc_entry_firq
#endif
#endif
	INTERRUPT_PROLOGUE

	mov	blink, sp

	ld	r3, [exc_nest_count]
	add	r2, r3, 1
	st	r2, [exc_nest_count]
	cmp	r3, 0
	bne	irq_handler_1
/* chang to interrupt stack if interrupt happened in task context */
	mov	sp, _e_stack
irq_handler_1:
	PUSH	blink

	lr	r0, [AUX_IRQ_CAUSE]
	sr	r0, [AUX_IRQ_SELECT]
	mov	r1, exc_int_handler_table
	ld.as	r2, [r1, r0]	/* r2 = exc_int_handler_table + irqno *4 */
/* handle software triggered interrupt */
	lr	r3, [AUX_IRQ_HINT]
	cmp	r3, r0
	bne.d irq_hint_handled
	xor	r3, r3, r3
	sr	r3, [AUX_IRQ_HINT]
irq_hint_handled:
	seti	/* enable higher priority interrupt */
	jl	[r2]		/* jump to interrupt handler */
/* no interrupts are allowed from here */
ret_int:
	clri	/* disable interrupt */
	POP	sp
	mov	r1, exc_nest_count
	ld	r0, [r1]
	sub	r0, r0, 1
	cmp	r0, 0
	bne.d	ret_int_r_1
	st	r0, [r1]

	ld	r0, [context_switch_reqflg]
	cmp	r0, 0
	beq	ret_int_r_1
	/* clear dispatch requst */
	mov	r0, 0
	st	r0, [context_switch_reqflg]

	SAVE_CALLEE_REGS	/* save callee save registers */

	ld	r0, [g_stLosTask]
	st 	sp, [r0]
	ldh	r1, [r0, 4]
	bclr	r1, r1, OS_TASK_STATUS_RUNNING_BIT
	sth 	r1, [r0, 4]

	ld 	r2, [g_stLosTask, 4]
	st 	r2, [g_stLosTask]

	ldh 	r1, [r2, 4]
	bset	r1, r1, OS_TASK_STATUS_RUNNING_BIT
	sth 	r1, [r2, 4]

	ld	sp, [r2]  // recover task stack 

	RESTORE_CALLEE_REGS
ret_int_r_1:
	INTERRUPT_EPILOGUE
	rtie

	.global osDisableIRQ
	.align 4
osDisableIRQ:
	clri
	j [blink]