
#ifndef VPOS_TIMER_H
#define VPOS_TIMER_H

#include "vh_io_hal.h"

void vh_timer_irq_enable();

void vh_timer_init(void);
void vh_timer_interrupt_handler(void);
void vk_timer_irq_enable(void);
void vk_timer_irq_disable(void);

extern unsigned int vk_timer_save_stk[17];
extern unsigned int vk_timer_flag;

#endif //VPOS_TIMER_H
