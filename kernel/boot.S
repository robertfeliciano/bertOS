// AArch64 boot for Raspberry Pi 4

.section ".text.boot"

.globl _start

    //boot code is loaded at 0x80000 in 64-bit mode rather than 0x8000 for 32-bit mode
    //.org 0x80000

//entry point for the kernel
//registers look like:
//x0 -> 32 bit pointer to primary core 
//x1 -> 0
//x2 -> 0
//x3 -> 0
//x4 -> 32 bit kernel entry point (_start location)
_start:
    //check processor ID is zero (executing on main core), else hang
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f
    //we're not on the main core, so hang in an infinite wait loop
1:  wfe
    b       1b

2:  //we're on the main core!

    //set stack before code section
    ldr     x1, =_start
    mov     sp, x1

    //clear the bss section
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
3:  cbz     w2, 4f
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b

    //go to C code (code should never return, but we will implement a failsafe just in case)
4:  bl      kernel_main
    //if C code returns, just branch
    b       1b