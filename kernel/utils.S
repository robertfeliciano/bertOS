.global get_el
get_el:
    mrs     x0, CurrentEL
    lsr     x0, x0, #2
    ret

.global put32
put32:
    str     w1, [x0]
    ret

.global get32
get32:
    ldr     w0, [x0]
    ret

.global delay
delay:
    subs    x0, x0, #1
    bne     delay
    ret

.global gen_timer_init
gen_timer_init:
    mov     x0, #1
    msr     CNTP_CTL_EL0, x0
    ret

.global gen_timer_reset
gen_timer_reset:
    mov     x0, #1
    lsl     x0, x0, #24
    msr     CNTP_TVAL_EL0, x0
    ret