#include <gpio.h>
#include <stddef.h>

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

//i've already learned so much just writing up to this section! for example:
//I learned that I hate UART. Then I came to love UART
void uart_init(int raspi){

    mmio_init(raspi);

    //disable all aspects of UART hardware. CR is the UART's control register
    mmio_write(UART0_CR, 0x00000000);

    //amrks that pins should be disabled
    mmio_write(GPPUD, 0x00000000);
    delay(150);

    //specifies to disable pins 14 and 15
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);

    //write 0 to GPPUDCLK0 to make the above changes take effect
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

    //set baud rate of connection
    mmio_write(UART0_IBRD, 1);
    mmio_write(UART0_FBRD, 40);

    //writes bits 4, 5, and 6 to line control register.
    //setting bit 4 means UART will hold data in 8 item deep FIFO rather than 1 item deep register
    //setting bits 5 and 6 means that data sent or received will have 8-bit long words
    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

	//disable all interrupts from the UART
	mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
	                       (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
 
	//writing bit 0 enables the UART hardware
    //writing bit 8 enables the ability to receive data
    //writing bit 9 enables the ability to transmit data
	mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));

}

void uart_putc(unsigned char c){
    //wait for UART to become ready to transmit data
    //FR is the flag register and tells us whether the read FIFO has any data for us to read
    //DR is the data register and is where data is read from and also written to
    while(mmio_read(UART0_FR) & (1 << 5));
    mmio_write(UART0_DR, c);
}

unsigned char uart_getc(){
    //the flag register can also tell us whether the write FIFO can accept any data
    while(mmio_read(UART0_FR) & (1 << 4));
    return mmio_read(UART0_DR);
}

void uart_puts(const char* str){
    for (size_t i = 0; str[i] != '\0'; i++){
        uart_putc((unsigned char) str[i]);
    }
}

#ifdef __cplusplus
extern "C"
#endif

void kernel_main(uint64_t dtb_ptr32, uint64_t x1, uint64_t x2, uint64_t x3)
{
    uint32_t reg;
    int raspi;
    asm volatile ("mrs %x0, midr_el1" : "=r" (reg));

    switch((reg >> 4) & 0xFFF){
        case 0xB76: raspi = 1; break;
        case 0xC07: raspi = 2; break;
        case 0xD03: raspi = 3; break;
        case 0xD08: raspi = 4; break;
        default: raspi = 1; break;
    }

    uart_init(raspi);
    uart_puts("Hello, kernel!\r\n");
    while(1) {
        uart_putc(uart_getc());
    }
}

