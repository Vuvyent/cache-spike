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
	li t1, 1
	li t2, 2
	li t3, 3
	li t4, 4
	li t5, 0x80004000
	li t6, 0x80004000
	sw t1, 0(t5)
	addi t5, t5, 0x8
	sw t2, 0(t5)
	addi t5, t5, 0x8
	sw t3, 0(t5)
	addi t5, t5, 0x8
	sw t4, 0(t5)
	addi t5, t5, 0x8
	li t1, 5
	li t2, 6
	li t3, 7
	li t4, 8
	sw t1, 0(t5)
	addi t5, t5, 0x8
	sw t2, 0(t5)
	addi t5, t5, 0x8
	sw t3, 0(t5)
	addi t5, t5, 0x8
	sw t4, 0(t5)
	addi t5, t5, 0x8
	li t1, 9
	li t2, 10
	li t3, 11
	li t4, 12
	sw t1, 0(t5)
	addi t5, t5, 0x8
	sw t2, 0(t5)
	addi t5, t5, 0x8
	sw t3, 0(t5)
	addi t5, t5, 0x8
	sw t4, 0(t5)
	addi t5, t5, 0x8
	li t1, 13
	li t2, 14
	li t3, 15
	li t4, 16
	sw t1, 0(t5)
	addi t5, t5, 0x8
	sw t2, 0(t5)
	addi t5, t5, 0x8
	sw t3, 0(t5)
	addi t5, t5, 0x8
	sw t4, 0(t5)
	addi t5, t5, 0x8
	
	addi t1, zero, 1
	addi t2, zero, 4
	
	first_cycle:
	
	bge t1, t2, block_end
	lw a1, 0(t6)
	addi t6, t6, 0x8
	lw a2, 0(t6)
	addi t6, t6, 0x8
	lw a3, 0(t6)
	addi t6, t6, 0x8
	lw a4, 0(t6)
	addi t6, t6, 0x8
	li a5, 0
	add a5, a5, a1
	add a5, a5, a2
	add a5, a5, a3
	add a5, a5, a4
	addi t1, t1, 1
	j first_cycle
	
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
