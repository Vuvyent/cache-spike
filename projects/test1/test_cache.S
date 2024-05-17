 .text

.globl _start

_start:

	addi t1, zero, 0
	addi t2, zero, 0x7ff
	li t3, 0
	sw t3, 0(t3)
	
	block_start:
	
	bge t1, t2, block_end
	lw t4, 0(t3)
	addi t3, t3, 0x8
	sw t3, 0(t3)
	addi t1, t1, 1
	j block_start
	
	
	block_end:
	nop;
