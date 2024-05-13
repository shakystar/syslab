#include "serial.h"
#include "vh_cpu_hal.h"
#include "vh_variant_hal.h"
#include "vh_io_hal.h"
#include "vh_variant_hal.h"
#include "dd.h"
#include "printk.h"

void vh_serial_interrupt_handler(void);

char getc(void)
{
	char c;
	unsigned long rxstat;
	
	/* Write getc func */
	// while (UARTFR & UARTFR_RXFE);

	// c = UARTDR;

	/* End getc func */

	while (pop_idx == push_idx);
	
	c = serial_buff[pop_idx++];

	if (pop_idx == SERIAL_BUFF_SIZE)
		pop_idx = 0;

	return c;
}

void putc(char c)
{
	while (UARTFR & UARTFR_TXFF);
	
	UARTDR = c;
}

void vh_serial_init(void)
{
	// set baud rate
	unsigned int idiv, fdiv;
	/*  baud rate Here  */

	idiv = (unsigned int)(UART_CLK/(16*UART_BAUDRATE));
	fdiv = (unsigned int)(((UART_CLK/(16*UART_BAUDRATE))-idiv)*64+0.5);

	/*  baud rate End  */
    UARTIBRD = idiv;
    UARTFBRD = fdiv;

	// set UART ctrl regs
    UARTLCR_H = 0b0000000001110110;
    UARTCR    = 0b0000001100000001;
	UARTIMSC  = 0b0000000111101111;
	UARTIFLS  = 0b0000000000000100;
	
	// clear buffer
	push_idx = 0;
	pop_idx = 0;
	for(int i=0; i<SERIAL_BUFF_SIZE; i++)
		serial_buff[i] = '\0';
}

void vh_serial_irq_enable(void)
{	
	/* enable GIC & interrupt */
}

void vk_serial_push(void)
{
	char c = UARTDR;
	
	serial_buff[push_idx++] = c;

	if (push_idx == SERIAL_BUFF_SIZE){	// buffer is full
		push_idx = 0;
	}	
}

void vh_serial_interrupt_handler(void)
{
	vk_serial_push();	
}

