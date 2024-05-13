#include "vh_cpu_hal.h"
#include "vh_io_hal.h"
#include "timer.h"
#include "printk.h"
#include "debug.h"
#include "dd.h"
#include "scheduler.h"
#include "thread.h"
#include "queue.h"
#include "semaphore.h"
#include "recoplay.h"
#include "vh_variant_hal.h"


extern int vk_sched_lock;
extern vk_thread_t *vk_current_thread;

// set timer control register
void write_cntkctl(unsigned int temp)
{
	__asm__ __volatile__("mcr p15, 0, %0, c14, c1, 0\n"
							:: "r" (temp));
}

// get timer frequency
unsigned int read_cntfrq()
{
	unsigned long temp;	
	__asm__ __volatile__("mrc p15, 0, %0, c14, c0, 0\n"
							: "=r" (temp));
	return temp;
}

// set physical timer control register
void write_cntp_ctl(unsigned int temp)
{
	__asm__ __volatile__("mcr p15, 0, %0, c14, c2, 1\n"
							:: "r" (temp));
}

// read physical count register
unsigned long long read_cntpct()
{
	unsigned long long lval;	
	unsigned long long hval;	
	__asm__ __volatile__("mrrc p15, 0, %0, %1, c14\n"
							: "=r" (lval), "=r" (hval));

	return lval | (hval << 32);
}

// set physical timer compare value register
void write_cntp_cval(unsigned long long temp)
{
	unsigned int lval = temp;
	unsigned int hval = temp >> 32;	
	__asm__ __volatile__("mcrr p15, 2, %0, %1, c14\n"
							:: "r" (lval), "r" (hval));
}

// set physical timer value register
void write_cntp_tval(unsigned long temp)
{
	__asm__ __volatile__("mcr p15, 0, %0, c14, c2, 0\n"
							:: "r" (temp));
}

// read physical timer value register
unsigned int read_cntp_tval()
{
	unsigned int temp;
	__asm__ __volatile__("mrc p15, 0, %0, c14, c2, 0\n"
							: "=r" (temp));
	return temp;
}

unsigned int get_1sec()
{
	return read_cntfrq();
}

void vh_timer_init(void)
{
	vk_timer_flag = 0;
}

void vh_timer_irq_enable()
{

}

void vh_timer_interrupt_handler(void)
{
	vk_timer_irq_disable();
	vh_save_thread_ctx(vk_timer_save_stk);
		
	// timer interrupt clear & enable

	vk_sched_save_tcb_ptr = (unsigned int)vk_timer_save_stk;
	vk_timer_flag = 1;

	++(vk_current_thread->cpu_tick);
	if(vk_sched_lock==0) {
		vk_swi_scheduler();
	}
}

void vk_timer_irq_enable()
{

}

void vk_timer_irq_disable(void)
{		

}