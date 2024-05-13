
#ifndef _VPOS_SCHEDULER_H_
#define _VPOS_SCHEDULER_H_

#include "thread.h"

void vk_scheduler(void);
void vk_scheduler_lock(void);
void vk_scheduler_unlock(void);
void vk_thread_swittch_ctx(vk_thread_t *from_thread, vk_thread_t *to_thread);
//void VPOS_start(void);

extern unsigned int vk_sched_save_tcb_ptr;
extern unsigned char vk_restore_flag;
extern vk_thread_t* vk_sched_current_thread;

#endif	//_VPOS_SCHEDULER_H_
