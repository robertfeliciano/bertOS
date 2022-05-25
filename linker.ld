ENTRY(_start)

SECTIONS
{
    . = 0x80000;    /* AArch64 has LOADER_ADDR = 0x80000 rather than 0x8000 */
    __start = .;
    /* initialize text section in memory */
    __text_start = .;
    .text :
    {
        KEEP(*(.text.boot))
        *(.text)
    }
    . = ALIGN(4096);
    __text_end = .;

    /* initialize read-only data section */
    __rodata_start = .;
    .rodata :
    {
        *(.rodata)
    }
    . = ALIGN(4096);
    __rodata_end = .;

    /* initialize read-write data section */
    __data_start = .;
    .data :
    {
        *(.data)
    }
    . = ALIGN(4096);
    __data_end = .;

    /* initialize bss data section */
    __bss_start = .;
    .bss :
    {
        bss = .;
        *(.bss)
    }
    . = ALIGN(4096);
    __bss_end = .;
    __bss_size = __bss_end - __bss_start;
    __end = .;
}