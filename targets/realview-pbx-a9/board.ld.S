#include "include/board.h"

#define TEXT_BASE  (SYS_MEM_BASE + TEXT_OFFSET)

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

MEMORY
{
    ram : ORIGIN = SYS_MEM_BASE, LENGTH = 0x10000000
}
SECTIONS
{
    /DISCARD/ : { *(.comment .note) }

    .ram_vectors TEXT_BASE : {
        __ram_vectors_vma = .;
        KEEP (*(.vectors))
    } > ram
    __ram_vectors_lma = LOADADDR(.ram_vectors);
}
