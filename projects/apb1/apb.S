.LC1:
        .string "time taken: %.2fs\n"
square:
        addi    sp,sp,-48
        sd      ra,40(sp)
        sd      s0,32(sp)
        addi    s0,sp,48
        li      t0,-8192
        add     sp,sp,t0
        mv      a4,a0
        li      a5,-8192
        addi    a5,a5,-16
        add     a5,a5,s0
        sw      a4,-20(a5)
        call    clock
        sd      a0,-32(s0)
        sw      zero,-20(s0)
        j       .L2
.L3:
        li      a5,-8192
        addi    a5,a5,-16
        add     a4,a5,s0
        lw      a5,-20(s0)
        slli    a5,a5,2
        add     a5,a4,a5
        lw      a4,-20(s0)
        sw      a4,-16(a5)
        lw      a5,-20(s0)
        addiw   a5,a5,1
        sw      a5,-20(s0)
.L2:
        lw      a5,-20(s0)
        sext.w  a4,a5
        li      a5,2047
        ble     a4,a5,.L3
        call    clock
        mv      a4,a0
        ld      a5,-32(s0)
        sub     a5,a4,a5
        fcvt.d.l        fa4,a5
        lui     a5,%hi(.LC0)
        fld     fa5,%lo(.LC0)(a5)
        fdiv.d  fa5,fa4,fa5
        fmv.x.d a1,fa5
        lui     a5,%hi(.LC1)
        addi    a0,a5,%lo(.LC1)
        call    printf
        li      a5,0
        mv      a0,a5
        li      t0,8192
        add     sp,sp,t0
        ld      ra,40(sp)
        ld      s0,32(sp)
        addi    sp,sp,48
        jr      ra
.LC0:
        .word   0
        .word   1093567616
