

#include <csi_config.h>

        .import irq_dispatch
        .import g_int_cnt
        
        .section .bss
        
        .global irq_stack
        .space  0x200
irq_stack:

        .text

        .global irq_handler
        .type   irq_handler, %function
irq_handler:

        psrset  ee
        
        subi    sp, 4 * 8
        stm     r0-r7, (sp)
        
        mov     r4, r15
        
        lrw     r0, g_int_cnt
        ld.w    r1, (r0)
        addi    r1, 1
        st.w    r1, (r0)
        cmpnei  r1, 1
        bt      0f
        
        mov     r5, sp
        
        lrw     sp, irq_stack
        
0:      /* already on interrupt stack, means interrupt nesting */

        mfcr    r6, epsr
        mfcr    r7, epc
                
        mfcr    r0, psr

        //psrset  ie
        
        lsri    r0, 16
        sextb   r0
        subi    r0, 32
        
        bsr     irq_dispatch
        
        //psrclr  ie
        
        lrw     r0, g_int_cnt
        ld.w    r1, (r0)
        subi    r1, 1
        st.w    r1, (r0)
        cmpnei  r1, 0
        bt      _reti
        
        mov     sp, r5
        
        lrw     r0, g_usLosTaskLock
        ld.h    r0, (r0)
        cmpnei  r0, 0
        bt      _reti
        
        lrw     r0, g_stLosTask
        ld.w    r1, (r0, 4)
        ld.w    r2, (r0)
        cmpne   r1, r2
        bf      _reti
        
        subi    sp, 4 * 9
        st.w    r4, (sp, 4 * 8)
        stm     r6-r13, (sp)
        
_switch_new:

        lrw     r4, g_stLosTask
        ld.w    r5, (r4)
        st.w    sp, (r5)
        
        bsr     osTaskSwitchCheck

        ld.h    r3, (r5, 4)
        bclri   r3, 4
        st.h    r3, (r5, 4)

_load_new:
        
        ld.w    r1, (r4, 4)
        ld.h    r0, (r1, 4)
        bseti   r0, 4
        st.h    r0, (r1, 4)
        st.w    r1, (r4)
        
        ld.w    sp, (r1)

        ldm     r6-r13, (sp)        /* psr->r6, pc->r7, restore r8-r13 */
        ld.w    r4, (sp, 4 * 8)     /* get lr */
        addi    sp, 4 * 9  

        /* fall thru as the logic is the same */

_reti:

        /* interrupt now disabled */
        
        mtcr    r6, epsr
        mtcr    r7, epc
        
        mov     r15, r4
        
        ldm     r0-r7, (sp)
        addi    sp, 4 * 8
        
        rte

        .size   irq_handler, . - irq_handler

        .global LOS_StartToRun
LOS_StartToRun:

        lrw     r4, g_stLosTask

        br      _load_new
        .size   LOS_StartToRun, . - LOS_StartToRun
       
        .global osSchedule  
osSchedule:   
        push    r4-r7

        mfcr    r6, psr
        mov     r7, r15

        subi    sp, 4 * 13          /* r0-r3, r15(lr) needless to save */
        stm     r6-r13, (sp)
        
        psrclr  ie
        
        br      _switch_new
        
        .size   osSchedule, . - osSchedule    
        
        .global LOS_IntLock
LOS_IntLock:
        mfcr    r0, psr
        psrclr  ie
        rts  
        
        .global LOS_IntUnLock
LOS_IntUnLock:
        mfcr    r0, psr
        psrset  ie
        rts  
        
        .global LOS_IntRestore
LOS_IntRestore:
        btsti   r0, 6
        bf      0f
        psrset  ie
        rts
0:
        psrclr  ie
        rts
        
        .end