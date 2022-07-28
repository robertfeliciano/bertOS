#include <utils.h>
#include <printf.h>
#include <schedule.h>
#include <pi/timer.h>

const unsigned int interval = 20000000;
unsigned int currVal = 0;

void timer_init(void){
    put32(TIMER_CTRL, ((1 << 28) | interval));
}

void handle_timer_irq(void){
    printf("Timer interrupt received! Generic timer.\n\r");
    generic_timer_reset();
    timer_tick();
}