.section .tohost, "aw", @progbits

.align 6
.globl tohost
tohost: .dword 0

.align 6
.globl fromhost
fromhost: .dword 0

.section .text

.globl _start


_start:

	addi t1, zero, 0
	addi t2, zero, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
	addi t2, t2, 0x7ff
        li t3, 0x80004000
	
	block_start:
	
	bge t1, t2, block_end
	lw t4, 0(t3)
	addi t3, t3, 0x8
	sw t1, 0(t3)
	addi t1, t1, 1
	j block_start
	
	
	block_end:
	nop;
	li a0, 0
        j tohost_exit # just terminate with exit code 0
        
	
# a0 exit code
tohost_exit:
        slli a0, a0, 1
        ori a0, a0, 1

        la t0, tohost
        sd a0, 0(t0)

        1: j 1b # wait for termination
