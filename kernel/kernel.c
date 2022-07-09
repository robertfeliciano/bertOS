#include <miniuart.h>

void kernel_main(){
	uart_init();
	uart_send_string("hello, kernel!\r\n");

	while(1) uart_send(uart_recv());
}
