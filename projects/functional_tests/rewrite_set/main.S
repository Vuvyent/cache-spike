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
	li t1, 0x7
	li t2, 0x4
	li t3, 0x80004000
	sw t1, 64(t3)
	sw t2, 320(t3)
	li t4, 0x9
	sw t4, 576(t3)
	lw t5, 64(t3)


# a0 exit code
tohost_exit:
        slli a0, a0, 1
        ori a0, a0, 1

        la t0, tohost
        sd a0, 0(t0)

        1: j 1b # wait for termination
