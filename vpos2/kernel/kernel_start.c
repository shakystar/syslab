#include "machine_init.h"
#include "pthread.h"
#include "scheduler.h"
#include "queue.h"
#include "vpos_shell.h"
#include "version.h"
#include "printk.h"
#include "recoplay.h"
#include "timer.h"
#include "pmu.h"
#include "debug.h"
#include "serial.h"

void __aeabi_unwind_cpp_pr0(void){}

void TIMER_test(void){
	write_cntp_tval((read_cntp_tval())/16);

	for (int i = 0; i < 10; i++)
		printk("time_buffer[%d] = %u\n", i, read_cntp_tval());
}

void set_interrupt(void)
{
	// disable  interrupts
	GICD_CTRL = 0x0;
	GICC_CTRL = 0x0;

	// interrupt setting
	vh_serial_irq_enable();	
	vh_timer_irq_enable();

	// set priority mask to the lowest level (to accept interrupts of any priority level)
	GICC_PMR = 0xff;

	// enable interrupts
	GICD_CTRL = 0x1;
	GICC_CTRL = 0x1;
}

void VPOS_kernel_main( void )
{	
	pthread_t p_thread, p_thread_0, p_thread_1, p_thread_2;
	
	/* static and global variable initialization */
	vk_scheduler_unlock();
	init_thread_id();
	init_thread_pointer();
	vh_user_mode = USER_MODE;
	vk_init_kdata_struct();
	
	vk_machine_init();
	set_interrupt(); 

	printk("%s\n%s\n%s\n", top_line, version, bottom_line);
	
	//Timer Test...
	TIMER_test();

	/* initialization for thread */
	race_var = 0;
	pthread_create(&p_thread, NULL, VPOS_SHELL, (void *)NULL);
	pthread_create(&p_thread_0, NULL, race_ex_1, (void *)NULL);
	pthread_create(&p_thread_1, NULL, race_ex_0, (void *)NULL);
	pthread_create(&p_thread_2, NULL, race_ex_2, (void *)NULL);
	VPOS_start();

	/* cannot reach here */
	printk("OS ERROR: VPOS_kernel_main( void )\n");
	while(1){}
}

