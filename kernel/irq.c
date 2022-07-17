#include <utils.h>
#include <printf.h>
#include <timer.h>
#include <entry.h>
#include <pi/irq.h>

const char* entry_error_messages[] = {
    "SYNC_INVALID_EL1t",
    "IRQ_INVALID_EL1t",		
    "FIQ_INVALID_EL1t",		
    "ERROR_INVALID_EL1T",		

    "SYNC_INVALID_EL1h",		
    "IRQ_INVALID_EL1h",		
    "FIQ_INVALID_EL1h",		
    "ERROR_INVALID_EL1h",		

    "SYNC_INVALID_EL0_64",		
    "IRQ_INVALID_EL0_64",		
    "FIQ_INVALID_EL0_64",		
    "ERROR_INVALID_EL0_64",	

    "SYNC_INVALID_EL0_32",		
    "IRQ_INVALID_EL0_32",		
    "FIQ_INVALID_EL0_32",		
    "ERROR_INVALID_EL0_32"	
};

void enable_interrupt_controller(){
    put32(CORE0_INT_CTR, SYSTEM_TIMER_IRQ_1);
}

void show_invalid_entry_message(int type, unsigned long esr, unsigned long address){
    printf("%s, ESR: %x, address: %x\r\n", entry_error_messages[type], esr, address);
}

void handle_irq(){
    unsigned int irq = get32(CORE0_INT_SOURCE);
    switch (irq){
        case (CNTPNSIRQ_INT):
            handle_timer_irq();
            break;
        default:
            printf("Unknown pending irq: 0x%x\r\n", irq);
    }
}