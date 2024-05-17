SECTIONS
{
  . = 0x80000000;
  .text : { *(.text) }
  . = 0x80004000;
  .data : { *(.data) }
}
