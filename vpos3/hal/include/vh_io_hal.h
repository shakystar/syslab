#ifndef _vh_IO_HAL_H_
#define _vh_IO_HAL_H_

/******************************************************************
  	GIC address
 ******************************************************************/

#define GICD_BASE_ADDR  0x08000000
#define GICC_BASE_ADDR  0x08010000

#define GICD_CTRL  		(*(volatile unsigned int*)(GICD_BASE_ADDR + 0x0))
#define GICD_ISENABLER(x) 	(*(volatile unsigned int*)(GICD_BASE_ADDR + 0x100 + x*4))
#define GICD_ICENABLER(x) 	(*(volatile unsigned int*)(GICD_BASE_ADDR + 0x180 + x*4))
#define GICD_ISPENDR(x) 	(*(volatile unsigned int*)(GICD_BASE_ADDR + 0x200 + x*4))
#define GICD_ICPENDR(x) 	(*(volatile unsigned int*)(GICD_BASE_ADDR + 0x280 + x*4))
#define GICD_ICACTIVER(x) 	(*(volatile unsigned int*)(GICD_BASE_ADDR + 0x380 + x*4))
#define GICD_ITARGETSR(x) 	(*(volatile unsigned int*)(GICD_BASE_ADDR + 0x800 + x*4))
#define GICD_ICFGR(x) 	(*(volatile unsigned int*)(GICD_BASE_ADDR + 0xC00 + x*4))

#define GICC_CTRL  		(*(volatile unsigned int*)(GICC_BASE_ADDR + 0x0))
#define GICC_PMR  		(*(volatile unsigned int*)(GICC_BASE_ADDR + 0x004))
#define GICC_IAR  		(*(volatile unsigned int*)(GICC_BASE_ADDR + 0x00C))
#define GICC_EOIR  		(*(volatile unsigned int*)(GICC_BASE_ADDR + 0x010))

#define INTERRUPT_ID_SGI_BEGIN 0
#define INTERRUPT_ID_PPI_BEGIN 16
#define INTERRUPT_ID_SPI_BEGIN 32
#define INTERRUPT_ID_UART (INTERRUPT_ID_SPI_BEGIN + 1)
#define INTERRUPT_ID_TIMER 30 	// physical timer 
#define INTERRUPT_ID_END 1019

/******************************************************************
  	UART address
 ******************************************************************/

#define UART_BASE_ADDR  0x09000000
#define UART_CLK        24000000

#define UARTDR     		(*(volatile unsigned int*)(UART_BASE_ADDR + 0x0))
#define UARTRSR    		(*(volatile unsigned int*)(UART_BASE_ADDR + 0x004))
#define UARTFR   		(*(volatile unsigned int*)(UART_BASE_ADDR + 0x018))
#define UARTIBRD   		(*(volatile unsigned int*)(UART_BASE_ADDR + 0x024))
#define UARTFBRD   		(*(volatile unsigned int*)(UART_BASE_ADDR + 0x028))
#define UARTLCR_H  		(*(volatile unsigned int*)(UART_BASE_ADDR + 0x02C))
#define UARTCR  	   	(*(volatile unsigned int*)(UART_BASE_ADDR + 0x030))
#define UARTIFLS		(*(volatile unsigned int*)(UART_BASE_ADDR + 0x034))
#define UARTIMSC  	   	(*(volatile unsigned int*)(UART_BASE_ADDR + 0x038))
#define UARTMIS  	   	(*(volatile unsigned int*)(UART_BASE_ADDR + 0x040))
#define UARTICR	    	(*(volatile unsigned int*)(UART_BASE_ADDR + 0x044))

/* Register Bit Setting */
#define UARTFR_RXFE		(1 << 4)
#define UARTFR_TXFF     (1 << 5)
#define UARTFR_RXFF		(1 << 6)
#define UARTFR_TXFE		(1 << 7)

/* your choice */
#define UART_BAUDRATE   115200
#define UART_ERR_MASK	0XF

/******************************************************************
  	Timer
 ******************************************************************/
#define CNTKCTL_PL0P	( 1 << 0 | 1 << 9)	


#endif  //_vh_IO_HAL_H_
