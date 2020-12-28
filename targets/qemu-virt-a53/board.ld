OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
MEMORY
{
    ram : ORIGIN = 0x40010000, LENGTH = 0x10000000
}
SECTIONS
{
    /DISCARD/ : { *(.comment .note) }
    .ram_vectors (0x40010000 + 0x00000000) : {
        __ram_vectors_vma = .;
        KEEP (*(.vectors))
    } > ram
    __ram_vectors_lma = LOADADDR(.ram_vectors);
}
