SECTIONS
{
  . = 0x80000000;
  .text : { *(.text) }
  . = 0x80001024;
  .data : { *(.data) }
}
