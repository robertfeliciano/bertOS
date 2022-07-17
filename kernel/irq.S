.global irq_vector_init
irq_vector_init:
    adr     x0, vector_table
    msr     vbar_el1, x0
    ret

.global enable_irq
enable_irq:
    msr     daifclr, #2
    ret

.global disbale_irq
disable_irq:
    msr     daifset, #2
    ret