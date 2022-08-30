#include <schedule.h>

.global cpu_switch_to
cpu_switch_to:
    mov	x10, #THREAD_CPU_CONTEXT
    add	x8, x0, x10             // x0 is pointer to current task_struct
                                // x8 now points to the cpu_context we are switching from
    mov	x9, sp
    stp	x19, x20, [x8], #16		// store callee-saved registers
    stp	x21, x22, [x8], #16
    stp	x23, x24, [x8], #16
    stp	x25, x26, [x8], #16
    stp	x27, x28, [x8], #16
    stp	x29, x9, [x8], #16
    str	x30, [x8]
    add	x8, x1, x10             // x1 is pointer to next task_struct we're switching to
                                // x8 now points to the cpu_context we are switching into
    ldp	x19, x20, [x8], #16		// restore callee-saved registers
    ldp	x21, x22, [x8], #16
    ldp	x23, x24, [x8], #16
    ldp	x25, x26, [x8], #16
    ldp	x27, x28, [x8], #16
    ldp	x29, x9, [x8], #16
    ldr	x30, [x8]
    mov	sp, x9
    ret