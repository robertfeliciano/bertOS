#include <utils.h>
#include <printf.h>
#include <pi/timer.h>

const unsigned int interval = 20000000;
unsigned int currVal = 0;

void timer_init(void){
    /* currVal = get32(TIMER_CLO);
    currVal += interval;
    put32(TIMER_C1, currVal); */
    put32(TIMER_CTRL, ((1 << 28) | interval));
}

void handle_timer_irq(void){
    /* currVal += interval;
    put32(TIMER_C1, currVal);
    put32(TIMER_CS, TIMER_CS_M1);
    printf("Timer interrupt received!\n\r"); */
    printf("Timer interrupt received! Generic timer.\n\r");
    generic_timer_reset();
}