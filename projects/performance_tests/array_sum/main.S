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
	li t3, 0x80004000
	addi t1, zero, 0
	addi t2, zero, 30
	li t4, 10
	li t5, 0x80004000
	li t6, 0
	
	block_start:
	
	bge t1, t2, block_end
	sw t4, 0(t3)
	addi t3, t3, 0x8
	addi t1, t1, 1
	j block_start
	
	block_end:
	addi t1, zero, 0
	cycle_start:
	bge t1, t2, end
	lw t3, 0(t5)
	add t6, t6, t3
	addi t5, t5, 0x8
	addi t1, t1, 1
	j cycle_start
	
	end:
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
