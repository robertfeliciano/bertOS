.global irq_vector_init
irq_vector_init:
    //load adr of vector table in vbar_el1
    adr     x0, vector_table
    msr     vbar_el1, x0
    ret

.global enable_irq
enable_irq:
    msr     daifclr, #2
    ret

.global disable_irq
disable_irq:
    msr     daifset, #2
    ret