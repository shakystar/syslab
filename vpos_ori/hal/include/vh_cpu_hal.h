
#ifndef _vh_CPU_HAL_H_
#define _vh_CPU_HAL_H_

/*****************************************************************
   CPU
 *****************************************************************/

#define VPOS_BASE_ADDR 0x40000000

/* stack location */
#define vh_userstack  (VPOS_BASE_ADDR + 0xa00000)
#define vh_swistack   (VPOS_BASE_ADDR + 0xb00000)
#define vh_irqstack   (VPOS_BASE_ADDR + 0xc00000)
#define vh_abortstack (VPOS_BASE_ADDR + 0xd00000)
#define vh_undefstack (VPOS_BASE_ADDR + 0xe00000)
#define vh_fiqstack   (VPOS_BASE_ADDR + 0xf00000)

/* cpsr */
#define vh_USERMODE   0x10
#define vh_FIQMODE    0x11
#define vh_IRQMODE    0x12
#define vh_SWIMODE    0x13
#define vh_ABORTMODE  0x17
#define vh_UNDEFMODE  0x1b
#define vh_MODEMASK   0x1f

#define vh_NOINT      0xc0 


#endif  //_vh_CPU_HAL_H_
