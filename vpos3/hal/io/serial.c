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
	// unsigned long rxstat;
	// /* Write getc func */
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
	float brd;
	/*  baud rate Here  */
	brd = (float) UART_CLK/(16 * UART_BAUDRATE);	
	idiv = (unsigned int) brd;
	fdiv = (unsigned int) ((brd-idiv)*64 + 0.5);

	/*  baud rate End  */
    UARTIBRD = idiv;
    UARTFBRD = fdiv;

	// set UART ctrl regs	
#ifdef FIFO_MODE
    UARTLCR_H = 0b0000000001110110;
	UARTIMSC =	0b0000000111101111;
	UARTIFLS = 	0b0000000000000100;
#else
	UARTLCR_H = 0b0000000001100110;
#endif

    UARTCR = 	0b0000001100000001;
	
	// clear buffer
	push_idx = 0;
	pop_idx = 0;
	for(int i=0; i<SERIAL_BUFF_SIZE; i++)
		serial_buff[i] = '\0';
}

void vh_serial_irq_enable(void)
{	
	/* enable GIC & interrupt */

	int n = INTERRUPT_ID_UART;

	// clear active & pending status
	GICD_ICACTIVER(n/32) = (1 << (n % 32));
	GICD_ICPENDR(n/32) = (1 << (n % 32));

	// enable interrupt
	GICD_ISENABLER(n/32) = (1 << (n % 32));

	// set interrupt target (to cpu 0)
	GICD_ITARGETSR(n/4) = (0b00000001) << ((n % 4) * 8);

	// set interrupt triggering type (edge-triggering)
	GICD_ICFGR(n/16) = 0b11 << ((n % 16) * 2);

	// clear uart interrupts (set all bits to 1 exclude bit 10, 9)
	UARTICR = 0b0000000111111111; 

	// enable UART Interrupts Mask RX
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

