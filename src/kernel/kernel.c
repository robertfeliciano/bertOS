#include <kernel/board.h>

#ifdef __cplusplus
extern "C"
#endif

void kernel_main(uint64_t dtb_ptr32, uint64_t x1, uint64_t x2, uint64_t x3)
{


    uart_init();
    uart_writeText("Hello world!\n");
    while (1);

}

