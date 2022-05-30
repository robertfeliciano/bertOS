// AArch64 boot for Raspberry Pi 4

.section ".text.boot"

.globl _start

    //boot code is loaded at 0x80000 in 64-bit mode rather than 0x8000 for 32-bit mode
    .org 0x80000

//entry point for the kernel
//registers look like:
//x0 -> 32 bit pointer to primary core 
//x1 -> 0
//x2 -> 0
//x3 -> 0
//x4 -> 32 bit kernel entry point (_start location)
_start:
    //set stack before code section
    ldr     x5, =_start
    mov     sp, x5

    //clear the bss section
    ldr     x5, =__bss_start
    ldr     w6, =__bss_size
3:  cbz     w6, 4f
    str     xzr, [x5], #8
    sub     w6, w6, #1
    cbnz    w6, 3b

    //go to C code (code should never return, but we will implement a failsafe just in case)
4:  bl      kernel_main
    //if C code returns, just branch
    b       1b

    