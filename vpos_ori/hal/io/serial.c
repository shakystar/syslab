#include "serial.h"
#include "vh_cpu_hal.h"
#include "vh_variant_hal.h"
#include "vh_io_hal.h"
#include "vh_variant_hal.h"
#include "dd.h"
#include "printk.h"

// #define FIFO_MODE

void vh_serial_interrupt_handler(void);

char getc(void)
{
	char c;
	unsigned long rxstat; // unused...

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
	fdiv = (unsigned int)((((UART_CLK/(16*UART_BAUDRATE))-idiv)*64)+0.5f);

	/*  baud rate End  */
    UARTIBRD = idiv;
    UARTFBRD = fdiv;

	// set UART ctrl regs
#ifdef FIFO_MODE
	UARTLCR_H = 0b0000000001110110;
	UARTIMSC =	0b0000000111101111;
	UARTIFLS = 	0b0000000000000100;
	UARTCR 	  = 0b0000001100000001;
#else
	UARTLCR_H = 0b0000000001100110;
	// UARTIMSC =	0b0000000111101111;
	// UARTIFLS = 	0b0000000000000100;
	UARTCR 	  = 0b0000001100000001;
#endif
	
	// clear buffer
	push_idx = 0;
	pop_idx = 0;
	for(int i=0; i<SERIAL_BUFF_SIZE; i++)
		serial_buff[i] = '\0';
}

void vh_serial_irq_enable(void)
{	
	/* enable GIC & interrupt */

	// not all of which are the same number.
	int n = INTERRUPT_ID_UART; // 33

	// clear active & pending status
	GICD_ICACTIVER((unsigned int)(n/32)) = 1 << (n % 32); // 1 = 1 << 1
	GICD_ICPENDR((unsigned int)(n/32)) = 1 << (n % 32); // 1 = 1 << 1

	// enable interrupt
	GICD_ISENABLER((unsigned int)(n/32)) = 1 << (n % 32); // 1 = 1 << 1

	// set interrupt target (to cpu 0)
	GICD_ITARGETSR((unsigned int)(n/4)) = 1 << ((n % 4) * 8); // 8 = 1 << 8

	// set interrupt triggering type (edge-triggering)
	GICD_ICFGR((unsigned int)(n/16)) = 3 << ((n % 16) * 2); // 2 = 3 << 2

	// clear uart interrupts
	UARTICR = 0b0000000111111111; 

	// enable UART Interrupt Mask RX
	UARTIMSC = 0b0000000000010000;
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

