#include <pi/irq.h>
#include <pi/timer.h>
#include <miniuart.h>
#include <printf.h>
#include <timer.h>
#include <irq.h>
#include <utils.h>
#include <schedule.h>
#include <fork.h>

void process(char* arr){
    while (1){
        for (int i = 0; i < 5; i++){
            printf("%c", arr[i]);
            delay(10000000);
        }
    }
}

void kernel_main(){
    uart_init();
    init_printf(0, putc);
    printf("\n\n=============== bertOS initializing ===============\r\n");
    printf("Exception level: %d\r\n", get_el());
    irq_vector_init();
    timer_init();
    enable_interrupt_controller();
    enable_irq();

    int res = copy_process((unsigned long)&process, (unsigned long)"12345", 3);
    if (res){
        printf("error while starting process 1");
        return;
    }
    res = copy_process((unsigned long)&process, (unsigned long)"abcde", 2);
    if (res){
        printf("error while starting process 2");
        return;
    }
    res = copy_process((unsigned long)&process, (unsigned long)"@#$&+", 1);
    if (res){
        printf("error while starting process 3");
        return;
    }
    while(1){
        //uart_send(uart_recv());
        /* uart_recv();
        printf("IRQ_PENDING_1: %04x\n\r", get32(IRQ_PENDING_1)); */
        printf("\n\rScheduling");
        schedule();
    }
}
