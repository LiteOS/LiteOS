/*
 * memory area definitions
 */

INCLUDE "stm32f429/liteos_mem.ld"

ENTRY (__no_entry)

/*
 * Memory Layout (NOT CORRECT)
 *
 * +-------------------+	0x0000 0000
 * +  vector           +
 * +--------------------	0x0000 1000 (PROC_TABLE_BASE)
 * +  proc_table       |
 * +--------------------	0x0000 2000 (KERNEL_BASE)
 * |  text / data      | 30k
 * ~~~~~~~~~~~~~~~~~~~~~
 *
 * +-------------------+        0x1000 0000
 * + Bitmap /          +
 * + kernel AHB        +
 * +-------------------+
 *
 * +-------------------+	0x2000 0000
 * +  stack            + 1k
 * +-------------------+	0x2000 0400 (+KERNEL_STACK)
 * +  data / bss       +
 * +-------------------+
 * +..userspace........+
 * +-------------------+
 *
 * +-------------------+	0x4000 0000
 * +  APB mem          +
 * +-------------------+        0x4002 0000
 * +  AHB mem          +
 * +-------------------+
 */


SECTIONS {
	.text :
	{
		KEEP(*(.vector))
		*(.text*)
		*(.rodata*)
	} >RAM
}
