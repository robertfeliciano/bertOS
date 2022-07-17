#include <miniuart.h>
#include <printf.h>
#include <timer.h>
#include <irq.h>

void kernel_main(){
    uart_init();
    init_printf(0, putc);
    //int el = get_el();
    //printf("Exception level: %d\r\n", el);
    irq_vector_init();
    timer_init();
    enable_interrupt_controller();
    enable_irq();


    while(1){
        uart_send(uart_recv());
    }
}
