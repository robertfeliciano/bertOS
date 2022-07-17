#include <pi/irq.h>
#include <pi/timer.h>
#include <miniuart.h>
#include <printf.h>
#include <timer.h>
#include <irq.h>
#include <utils.h>

void kernel_main(){
    uart_init();
    init_printf(0, putc);
    //int el = get_el();
    //printf("Exception level: %d\r\n", el);
    irq_vector_init();
    generic_timer_init();
    enable_interrupt_controller();
    enable_irq();


    while(1){
        //uart_send(uart_recv());
        uart_recv();
        printf("IRQ_PENDING_1: %04x\n\r", get32(IRQ_PENDING_1));
    }
}
