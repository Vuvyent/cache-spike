.text
.global _start

_start:
	li t1, 0x7
	sw t1, (t0)
	lw t3, (t0)
