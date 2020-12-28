ENTRY(reset_vector)
INCLUDE board.ld
SECTIONS
{
    .initcall.init : {

     _initcall0_start = ABSOLUTE(.);
     KEEP(*(SORT (.initcall0.init)))
     _initcall0_end = ABSOLUTE(.);

     _initcall1_start = ABSOLUTE(.);
     KEEP(*(SORT (.initcall1.init)))
     _initcall1_end = ABSOLUTE(.);

     _initcall2_start = ABSOLUTE(.);
     KEEP(*(SORT (.initcall2.init)))
     _initcall2_end = ABSOLUTE(.);

     _initcall3_start = ABSOLUTE(.);
     KEEP(*(SORT (.initcall3.init)))
     _initcall3_end = ABSOLUTE(.);

     _initcall4_start = ABSOLUTE(.);
     KEEP(*(SORT (.initcall4.init)))
     _initcall4_end = ABSOLUTE(.);

     _initcall5_start = ABSOLUTE(.);
     KEEP(*(SORT (.initcall5.init)))
     _initcall5_end = ABSOLUTE(.);

     _initcall6_start = ABSOLUTE(.);
     KEEP(*(SORT (.initcall6.init)))
     _initcall6_end = ABSOLUTE(.);

     _initcall7_start = ABSOLUTE(.);
     KEEP(*(SORT (.initcall7.init)))
     _initcall7_end = ABSOLUTE(.);

    } > ram

    .got ALIGN(0x4) : { *(.got.plt) *(.got) } > ram

    .gcc_except_table ALIGN (0x8) : { . = .; } > ram  .gcc_except_table : { KEEP(*(.gcc_except_table*)) }
    .exception_ranges ALIGN (0x8) : ONLY_IF_RW { *(.exception_ranges .exception_ranges*) } > ram

    .ARM.extab ALIGN(0x4) : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > ram

    /* .ARM.exidx is sorted, so has to go in its own output section.  */
    .ARM.exidx ALIGN(0x8) : { __exidx_start = .; *(.ARM.exidx* .gnu.linkonce.armexidx.*) ;__exidx_end = .;} > ram

    /* text/read-only data */
    .text ALIGN(0x1000) : {
        __text_start = .;
        *(.text* .sram.text.glue_7* .gnu.linkonce.t.*)
    } > ram

    .rel.text : { *(.rel.text) *(.rel.text.*) *(.rel.gnu.linkonce.t*) } > ram
    .rela.text : { *(.rela.text) *(.rela.text.*) *(.rela.gnu.linkonce.t*) } > ram
    .rel.data : { *(.rel.data) *(.rel.data.*) *(.rel.gnu.linkonce.d*) } > ram
    .rela.data : { *(.rela.data) *(.rela.data.*) *(.rela.gnu.linkonce.d*) } > ram
    .rel.rodata : { *(.rel.rodata) *(.rel.rodata.*) *(.rel.gnu.linkonce.r*) } > ram
    .rela.rodata : { *(.rela.rodata) *(.rela.rodata.*) *(.rela.gnu.linkonce.r*) } > ram
    .rel.got : { *(.rel.got) } > ram
    .rela.got : { *(.rela.got) } > ram
    .rel.ctors : { *(.rel.ctors) } > ram
    .rela.ctors : { *(.rela.ctors) } > ram
    .rel.dtors : { *(.rel.dtors) } > ram
    .rela.dtors : { *(.rela.dtors) } > ram
    .rel.init : { *(.rel.init) } > ram
    .rela.init : { *(.rela.init) } > ram
    .rel.fini : { *(.rel.fini) } > ram
    .rela.fini : { *(.rela.fini) } > ram
    .rel.bss : { *(.rel.bss) } > ram
    .rela.bss : { *(.rela.bss) } > ram
    .rel.plt : { *(.rel.plt) } > ram
    .rela.plt : { *(.rela.plt) } > ram
    .rel.dyn : { *(.rel.dyn) } > ram

    .dummy_post_text : {
        __text_end = .;
    } > ram

    .rodata ALIGN(0x1000) : {
        __rodata_start = .;
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    } > ram

    /*
     * extra linker scripts tend to insert sections just after .rodata,
     * so we want to make sure this symbol comes after anything inserted above,
     * but not aligned to the next section necessarily.
     */
    .dummy_post_rodata : {
        __rodata_end = .;
    } > ram

    .data ALIGN(0x1000) : {
        /* writable data  */
        __ram_data_start = .;
        *(.data .data.* .gnu.linkonce.d.*)
        . = ALIGN(0x4);
        KEEP(*( SORT (.liteos.table.*)));
    } > ram

    .ctors : ALIGN(0x4) {
        __ctor_list__ = .;
        KEEP (*(.ctors .init_array))
        __ctor_end__ = .;
    } > ram
    .dtors : ALIGN(0x4) {
        __dtor_list__ = .;
        KEEP (*(.dtors .fini_array))
        __dtor_end__ = .;
    } > ram
    /*
     * extra linker scripts tend to insert sections just after .data,
     * so we want to make sure this symbol comes after anything inserted above,
     * but not aligned to the next section necessarily.
     */
    .dummy_post_data : {
        __ram_data_end = .;
    } > ram

    /* unintialized data (in same segment as writable data) */
    .bss ALIGN(0x800) : {
        __int_stack_start = .;
        *(.int_stack);
        . = . + 0x24;
        KEEP(*(.bss.prebss.*))
        . = ALIGN(0x8);
        __bss_start = .;
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(0x8);
        __bss_end = .;
    } > ram

    . = ALIGN(0x1000);
    _end = .;

    /* Strip unnecessary stuff */
    /DISCARD/ 0 : { *(.comment .note) } > ram
}
