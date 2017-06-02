MEMORY {
 ICCM : ORIGIN = 0x00000000, LENGTH = 0x10000
 DCCM : ORIGIN = 0x80000000, LENGTH = 0x10000
 EXT_RAM : ORIGIN = 0x10000000, LENGTH = 0x8000000
}
ENTRY(_start)

SECTIONS {

 GROUP : {
  .init_bootstrap:{
   *(.init_vector)
   *(.init_bootstrap)
  }
  .vector ALIGN(1024): {
   _f_vector = .;
   *(.vector)
   _e_vector = .;
  }
 } > EXT_RAM

 GROUP : {

  .text ALIGN(4): {
   _f_text = .;
   *(TYPE text)
   _e_text = .;
  }

  .rodata ALIGN(4): {
   _f_rodata = .;

   _fctors = .;
   *(.ctors*)
   _ectors = .;
   _fdtors = .;
   *(.dtors*)
   _edtors = .;
   _feh_frame = .;
   *(.eh_frame*)
   _eeh_frame = .;

   *(TYPE lit)

   _e_rodata = .;
  }

 } > EXT_RAM


 GROUP : {
  .data ALIGN(8): {
   _f_data = .;
   _f_sdata = .;
   *(.sdata)
   _e_sdata = .;
   *(TYPE data)
  }
  .tls ALIGN(8): {
   *(.tls*)
   _e_data = .;
  }
 } > EXT_RAM AT > EXT_RAM

 GROUP (NOLOAD) : {
  .bss ALIGN(8): {
   _f_bss = .;
   *(TYPE bss)
   _e_bss = .;
  }
  .stack ALIGN(4) SIZE(8192): {}
  .heap? ALIGN(4) SIZE(8192): {}
 } > EXT_RAM

 _f_stack = ADDR(.stack);
 _e_stack = ADDR(.stack) + SIZEOF(.stack);
 _f_heap = ADDR(.heap);
 _e_heap = ADDR(.heap) + SIZEOF(.heap);

 _load_addr_text = LOADADDR(.text);
 _load_addr_rodata = LOADADDR(.rodata);
 _load_addr_data = LOADADDR(.data);
}