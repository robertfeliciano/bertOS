#include <mm.h>
#include <aarch64/sysregs.h>

.section ".text.boot"

.global _start
_start:
    mrs		x0, mpidr_el1
    and		x0, x0, #0xFF
    //if we are not on the main core we just loop forever
    cbz		x0, main
    b		loop

loop:
    b		loop

main:
    //needed to determine if we are on physical pi or qemu
    mrs 	x0, CurrentEL
    cmp		x0, #CurrentEL_EL3
    beq		el3_entry
    b 		el2_entry

el3_entry:
    //this is what is run on a physical pi3
    ldr		x0, =SCTLR_VALUE_MMU_DISABLED
    msr		sctlr_el2, x0

    ldr		x0, =HCR_VALUE
    msr		hcr_el2, x0

    ldr 	x0, =SCR_VALUE
    msr 	scr_el3, x0

    ldr		x0, =SPSR_EL2
    msr		spsr_el3, x0

    adr		x0, el2_entry
    msr		elr_el3, x0

    eret

el2_entry:
    //qemu boots pi3 at el2 so this is needed for qemu
    ldr 	x0, =SCTLR_VALUE_MMU_DISABLED
    msr 	sctlr_el1, x0
    
    ldr     x0, =HCR_VALUE
    msr     hcr_el2, x0

    ldr 	x0, =SPSR_VALUE
    msr		spsr_el2, x0

    adr		x0, el1_entry
    msr 	elr_el2, x0

    eret

el1_entry:
    //we want our OS to run at el1
    adr		x0, bss_begin
    adr		x1, bss_end
    sub		x1, x1, x0
    bl		memzero

    mov		sp, #LOW_MEMORY
    bl		kernel_main
    b		loop
