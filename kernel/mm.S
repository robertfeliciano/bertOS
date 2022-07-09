.global memzero
memzero:
    str xzr, [x0], #8
    subs x1, x1, #8
    bgt memzero
    ret