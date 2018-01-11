    
  .syntax unified
  .cpu cortex-m4
  .thumb

.global  g_pfnVectors
.global  Default_Handler

/* start address for the initialization values of the .ram2 section. 
defined in linker script */
.word  _siram2
/* start address for the .ram2 section. defined in linker script */  
.word  _sram2
/* end address for the .ram2 section. defined in linker script */
.word  _eram2
/* start address for the initialization values of the .data section. 
defined in linker script */
.word  _sidata
/* start address for the .data section. defined in linker script */  
.word  _sdata
/* end address for the .data section. defined in linker script */
.word  _edata
/* start address for the .bss section. defined in linker script */
.word  _sbss
/* end address for the .bss section. defined in linker script */
.word  _ebss
/* stack used for SystemInit_ExtMemCtl; always internal RAM used */

/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called. 
 * @param  None
 * @retval : None
*/

    .section  .text.Reset_Handler
  .weak  Reset_Handler
  .type  Reset_Handler, %function
Reset_Handler: 
  ldr   sp, =_estack       /* set stack pointer 是将_estack这个变量的地址，放到sp中*/
 
/* Copy the data segment initializers from flash to SRAM */  
  movs  r1, #0				/*将0放入到r1寄存器中*/
  b  LoopCopyDataInit		/*强制跳转到LoopCopyDataInit标签处*/	

CopyDataInit:				/*实现flash中的_sidata++4、8、12....的值依次拷贝到RAM中的_sdata++4、8、12....的地方，主要就是data段数据拷贝*/
  ldr  r3, =_sidata			/*将_sidata的地址放入到r3中*/
  ldr  r3, [r3, r1]			/*将存储器地址为R3+R1的字数据读入寄存器R3*/
  str  r3, [r0, r1]			/*将R3中的值存到以R0+R1的值为地址的内存处*/
  adds  r1, r1, #4			/*将r1加4的地址放入到r1中*/
    
LoopCopyDataInit:			/*主要比较_sdata++4、8、12....和_edata的值是否相等，不相等就跳转*/
  ldr  r0, =_sdata			/*将_sdata的地址放入到r0中*/
  ldr  r3, =_edata			/*将_edata的地址放入到r3中*/
  adds  r2, r0, r1			/*将r0与r1的地址相加放入到r2中*/
  cmp  r2, r3				/*比较r2和r3是否相等*/
  bcc  CopyDataInit			/*如果不相等就跳转到CopyDataInit标签处*/
  
/* Copy the data segment initializers from flash to SRAM2 */  
  movs  r1, #0				/*将0放入到r1寄存器中*/
  b  LoopCopyRam2Init		/*强制跳转到LoopCopyRam2Init标签处*/	

CopyRam2Init:				/*实现flash中的_siram2++4、8、12....的值依次拷贝到RAM中的_sram2++4、8、12....的地方，主要就是data段数据拷贝*/
  ldr  r3, =_siram2			/*将_siram2的地址放入到r3中*/
  ldr  r3, [r3, r1]			/*将存储器地址为R3+R1的字数据读入寄存器R3*/
  str  r3, [r0, r1]			/*将R3中的值存到以R0+R1的值为地址的内存处*/
  adds  r1, r1, #4			/*将r1加4的地址放入到r1中*/
    
LoopCopyRam2Init:			/*主要比较_sram2++4、8、12....和_eram2的值是否相等，不相等就跳转*/
  ldr  r0, =_sram2			/*将_sram2的地址放入到r0中*/
  ldr  r3, =_eram2			/*将_eram2的地址放入到r3中*/
  adds  r2, r0, r1			/*将r0与r1的地址相加放入到r2中*/
  cmp  r2, r3				/*比较r2和r3是否相等*/
  bcc  CopyRam2Init			/*如果不相等就跳转到CopyDataInit标签处*/
  
  ldr  r2, =_sbss			/*将_sbss的地址放入到r2中*/
  b  LoopFillZerobss		/*强制跳转到LoopFillZerobss标签处*/	
/* Zero fill the bss segment. */  
FillZerobss:
  movs  r3, #0				/*将0放入到r3寄存器中*/
  str  r3, [r2], #4			/*将R3中的值0赋给R2，然后将R2的地址变成R2+4的位置*/
    
LoopFillZerobss:			/*主要比较_sbss++4、8、12....和_ebss的值是否相等，不相等就跳转*/
  ldr  r3, = _ebss			/*将_ebss的地址放入到r3中*/	
  cmp  r2, r3				/*比较r2和r3是否相等*/
  bcc  FillZerobss			/*如果不相等就跳转到FillZerobss标签处*/
  
/* Call the clock system initialization function.*/
  bl  SystemInit 			/*跳转到SystemInit处执行，并将下一条指令拷贝到LR链接寄存器中*/

/* Call static constructors */
    bl __libc_init_array
/* Call the application entry point.*/
  bl  main					/*跳转到main处执行，并将下一条指令拷贝到LR链接寄存器中*/
  bx  lr    				/*跳转到lr中存放的地址处,完成子程序返回*/
.size  Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an 
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None     
 * @retval None       
*/
    .section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b  Infinite_Loop
  .size  Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M3. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
* 
*******************************************************************************/
   .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors
   
g_pfnVectors:
  .word  _estack
  .word  Reset_Handler

  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  DebugMon_Handler
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler
  
  
/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler. 
* As they are weak aliases, any function with the same name will override 
* this definition.
* 
*******************************************************************************/
   .weak      NMI_Handler
   .thumb_set NMI_Handler,Default_Handler
  
   .weak      HardFault_Handler
   .thumb_set HardFault_Handler,Default_Handler
  
   .weak      MemManage_Handler
   .thumb_set MemManage_Handler,Default_Handler
  
   .weak      BusFault_Handler
   .thumb_set BusFault_Handler,Default_Handler

   .weak      UsageFault_Handler
   .thumb_set UsageFault_Handler,Default_Handler

   .weak      SVC_Handler
   .thumb_set SVC_Handler,Default_Handler

   .weak      DebugMon_Handler
   .thumb_set DebugMon_Handler,Default_Handler

   .weak      PendSV_Handler
   .thumb_set PendSV_Handler,Default_Handler

   .weak      SysTick_Handler
   .thumb_set SysTick_Handler,Default_Handler              
  

 
   
   

