#include <stddef.h>
#include <stdint.h>

static uint32_t MMIO_BASE;

//MMIO area base address depends on board type
//yes this is going on my raspberry pi4 but hey maybe one day i get a pi 3 and want to use it
//the other models don't support 64 bit but OSDev included them so I will too.
static inline void mmio_init(int raspi){
    switch(raspi){
        case 2:
        case 3:  MMIO_BASE = 0x3F000000; break; //pi 2 and 3
        case 4:  MMIO_BASE = 0xFE000000; break; //pi 4
        default: MMIO_BASE = 0x20000000; break; //models before 2
    }
}

//output for Memory-Mapped IO
static inline void mmio_write(uint32_t reg, uint32_t data){
    *(volatile uint32_t*)(MMIO_BASE + reg) = data;
}

//input for Memory-Mapped IO
static inline uint32_t mmio_read(uint32_t reg){
    return *(volatile uint32_t*)(MMIO_BASE + reg);
}

//basically just a loop that the compiler's optimization won't get rid of
static inline void delay(int32_t count){
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                :"=r"(count)
                :[count]"0"(count)
                :"cc");
}

enum{
    //the offsets for reach register.
    GPIO_BASE = 0x200000,
 
    //controls actuation of pull up/down to ALL GPIO pins.
    GPPUD = (GPIO_BASE + 0x94),
 
    //controls actuation of pull up/down for specific GPIO pin.
    GPPUDCLK0 = (GPIO_BASE + 0x98),
 
    //the base address for UART.
    UART0_BASE = (GPIO_BASE + 0x1000), // for raspi4 0xFE201000, raspi2 & 3 0x3F201000, and 0x20201000 for raspi1
 
    //the offsets for reach register for the UART.
    UART0_DR     = (UART0_BASE + 0x00),
    UART0_RSRECR = (UART0_BASE + 0x04),
    UART0_FR     = (UART0_BASE + 0x18),
    UART0_ILPR   = (UART0_BASE + 0x20),
    UART0_IBRD   = (UART0_BASE + 0x24),
    UART0_FBRD   = (UART0_BASE + 0x28),
    UART0_LCRH   = (UART0_BASE + 0x2C),
    UART0_CR     = (UART0_BASE + 0x30),
    UART0_IFLS   = (UART0_BASE + 0x34),
    UART0_IMSC   = (UART0_BASE + 0x38),
    UART0_RIS    = (UART0_BASE + 0x3C),
    UART0_MIS    = (UART0_BASE + 0x40),
    UART0_ICR    = (UART0_BASE + 0x44),
    UART0_DMACR  = (UART0_BASE + 0x48),
    UART0_ITCR   = (UART0_BASE + 0x80),
    UART0_ITIP   = (UART0_BASE + 0x84),
    UART0_ITOP   = (UART0_BASE + 0x88),
    UART0_TDR    = (UART0_BASE + 0x8C),
 
    //the offsets for Mailbox registers
    MBOX_BASE    = 0xB880,
    MBOX_READ    = (MBOX_BASE + 0x00),
    MBOX_STATUS  = (MBOX_BASE + 0x18),
    MBOX_WRITE   = (MBOX_BASE + 0x20)
};

//Mailbox message with clock rate of PL011 to #MHz tag
volatile unsigned int __attribute__((aligned(16))) mbox[9] = {
    9*4, 0, 0x38002, 12, 8, 3000000, 0, 0
};

//i've already learned so much just writing up to this section! for example:
//I learned that I hate UART.
void uart_init(int raspi){

    mmio_init(raspi);

    //disable UART0
    mmio_write(UART0_CR, 0x00000000);
    //setup the GPIO pin 14 && 15

    //disable pull up/down for all GPIO pins & delay for 150 cycles
    mmio_write(GPPUD, 0x00000000);
    delay(150);

    //disable pull up/down for pin 14, 15 & delay for 150 cycles
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);

    //write 0 to GPPUDCLK0 to make it take effect (activate it)
    mmio_write(GPPUDCLK0, 0x00000000);

    //clear pending interrupts
    mmio_write(UART0_ICR, 0x7FF);

    //for pi 3 and 4 the UART_CLOCK is system-clock dependent. 
    //set it to 3MHz.
    if (raspi >= 3){
        unsigned int r = (((unsigned int)(&mbox) & ~0xF) | 8);
        //wait until we can talk to the VC
        while(mmio_read(MBOX_STATUS) & 0x80000000);
        //send message to property channel and wait for response
        mmio_write(MBOX_WRITE, r);
        while((mmio_read(MBOX_STATUS) & 0x40000000) || mmio_read(MBOX_READ) != r);
    }
    
}