#include "../include/vh_cpu_hal.h"

	.extern VPOS_kernel_main
	.extern vk_undef_handler
	.extern vk_swi_classifier
	.extern vk_pabort_handler
	.extern vk_dabort_handler
	.extern vk_fiq_handler
	.extern vk_not_used_handler
	.extern vk_irq_test

	.extern vk_sched_save_tcb_ptr

	.global vh_VPOS_STARTUP

	.global vk_save_swi_mode_stack_ptr		
	.global vk_save_swi_current_tcb_bottom
	.global vk_save_irq_mode_stack_ptr	
	.global vk_save_irq_current_tcb_bottom
	.global vk_save_pabort_current_tcb_bottom

	.global vh_restore_thread_ctx
	.global vh_save_thread_ctx
	.global vh_save_ctx_bottom


	.text
vh_VPOS_STARTUP:
	/* Camouflaged code for imitating linux
	   Linux has a header that includes 8 nop operation, branch code, magic number, binaray file start offset, and file size(end offset */
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	b 	vh_VPOS_reset
	magicn:	.long 0x016f2818	// Linux magic number
	startn:	.long 0x00000000	// start address(offset) is 0
	endn:	.long 0x0000d8fc	// end address(offset) is file size(byte)
	/* Camouflaged code end */
	
	nop
	nop
	nop
	nop

	.align 6
	.global vh_vector_start
vh_vector_start:
	b       vk_not_used_handler		// not used
	b       vk_undef				// undefined instruction
	b       vh_software_interrupt   // supervisor call
	b       vh_pabort				// prefetch abort
	b       vk_dabort				// data abort
	b       vk_not_used_handler		// not used
	b       vh_irq                  // irq
	b       vk_fiq_handler			// fiq


vh_VPOS_reset:
	// variable initialization
	mov		r0, #0x00
	str		r0, vk_save_swi_mode_stack_ptr
	str		r0, vk_save_swi_current_tcb_bottom
	str		r0, vk_save_irq_mode_stack_ptr
	str		r0, vk_save_irq_current_tcb_bottom

	// Mask interrupt and fast interrupt
	mrs		r0, cpsr		
	orr		r0, r0, #vh_NOINT
	msr		cpsr, r0

	// Invalidate all instruction caches to point of unification. Also flushes branch target cache.
	mov		r0, #0x00
	mcr 	p15, 0, r0, c7, c5, 0   

	// Control Register Setting
	mrc		p15, 0, r0, c1, c0, 0 		// Read control register 
	bic		r0, r0, #0x01				// MMU disabled
	bic		r0, r0, #0x04				// D-Cache disabled
	bic		r0, r0, #0x1000				// I-Cache disabled
	bic		r0, r0, #0x2000				// Normal exception vectors, base address 0x0000000
	orr		r0, r0, #0x02				// Alignment fault checking enabled
	orr		r0, r0, #0x800				// Branch prediction enabled 
	mcr 	p15, 0, r0, c1, c0, 0		// write control register

	// change vector table base address
	ldr		r0, =vh_vector_start
	mcr		p15, 0, r0, c12, c0, 0

	// stack pointer setting
	mrs 	r0, cpsr	
	bic 	r0, r0, #vh_MODEMASK | vh_NOINT

	orr	 	r1, r0, #vh_UNDEFMODE | vh_NOINT
	msr 	cpsr_cxsf, r1                
	ldr 	sp, =vh_undefstack

	orr 	r1, r0, #vh_ABORTMODE | vh_NOINT
	msr 	cpsr_cxsf, r1               
	ldr 	sp, =vh_abortstack

	orr 	r1,r0, #vh_IRQMODE | vh_NOINT
	msr 	cpsr_cxsf, r1              
	ldr 	sp, =vh_irqstack

	orr 	r1, r0, #vh_FIQMODE | vh_NOINT
	msr 	cpsr_cxsf, r1             
	ldr 	sp, =vh_fiqstack

	orr 	r1, r0, #vh_SWIMODE | vh_NOINT
	msr 	cpsr_cxsf, r1            
  	ldr 	sp, =vh_swistack	

	b 		VPOS_kernel_main

	.global	vt_read_sp
vt_read_sp:	
	mov		r0, sp
	mov		pc, lr

	.global vt_read
vt_read:
	mrs 	r0, cpsr
	mov		pc, lr

vk_undef:
	str		lr, vk_error_addr
	b 		vk_undef_handler       

vk_dabort:
	str		lr, vk_error_addr
	b 		vk_dabort_handler

vh_software_interrupt:
	str		sp, vk_save_swi_mode_stack_ptr
	stmfd 	sp, {r14}^
	sub 	sp, sp, #4
	stmfd	sp, {r13}^
	sub 	sp, sp, #4
	stmfd	sp!, {r0-r12}
	mrs		r0, spsr_all
	stmfd	sp!, {r0, lr}
	
	mrs		r0, cpsr
	orr		r0, r0, #0x80
	msr		cpsr, r0
	
	str		sp, vk_save_swi_current_tcb_bottom
	ldr		r0, [sp, #4*2]

	bl		vk_swi_classifier

vh_leaving_swi:
	ldmfd	sp!, {r0, lr}
	msr		spsr_cxsf, r0
	ldmfd	sp!, {r0-r12}
	ldmfd	sp, {r13}^
	add 	sp, sp, #4
	ldmfd	sp, {r14}^
	add 	sp, sp, #4

	movs	pc, lr


vh_irq:
	sub		lr, lr, #4
	str		sp, vk_save_irq_mode_stack_ptr
	stmfd	sp, {r14}^
	sub		sp, sp, #4
	stmfd	sp, {r13}^
	sub		sp, sp, #4
	stmfd	sp!, {r0-r12}
	mrs		r0, spsr_all
	stmfd	sp!, {r0, lr}

	str		sp, vk_save_irq_current_tcb_bottom

	bl		vk_irq_handler

vh_leaving_irq:
	ldmfd	sp!, {r0, lr}
	msr		spsr_cxsf, r0
	ldmfd	sp!, {r0-r12}
	ldmfd	sp, {r13}^
	add		sp, sp, #4
	ldmfd	sp, {r14}^
	add		sp, sp, #4

	movs	pc, lr

vh_pabort:	
    ldr 	sp, =vh_abortstack

	sub		lr, lr, #4
	str		sp, vk_save_pabort_stack_ptr
	stmfd	sp!, {r0-r14}^
	mrs		r0, spsr_all
	stmfd	sp!, {r0, lr}
	str		sp, vk_save_pabort_current_tcb_bottom
	
	bl		vk_pabort_handler

vh_leaving_pabort:
	ldmfd	sp!, {r0, lr}
	msr		spsr_all, r0
	ldmfd 	sp!, {r0-r12}^
	ldmfd	sp, {r13}^
	add		sp, #4
	ldmfd	sp, {lr}^
	add		sp, #4		
	
	movs	pc, lr

vh_restore_thread_ctx:
	mov		sp, r0   				// sp <- tcb_bottom 

	/*	restore thread psr(r0) and thread address for restart(lr) */
	ldmfd	sp!, {r0, lr}
	msr		spsr_all, r0				// set spsr (current thread psr)

	/* 	restore current thread context.
		we cannot multiple load r13 in Cortex-A9.*/
	ldmfd	sp!, {r0-r12}	
	ldmfd 	sp, {r13}^
	add		sp, #4	
	ldmfd	sp, {lr}^
	add		sp, #4		
	
	ldr		sp, vk_save_swi_mode_stack_ptr
	movs	pc, lr


	.global vr_save_thread_ctx
vr_save_thread_ctx:
	ldr		r2, vk_save_pabort_current_tcb_bottom
	b		vh_save_ctx_bottom

	.global vh_change_ctx
vh_change_ctx:
	mov		r2, r0
	ldr		r0, vk_save_swi_current_tcb_bottom
	b		vh_save_ctx_bottom

// this function copy data from r2 to r0
vh_save_thread_ctx:
	/*	Check mode */
	mrs		r1, cpsr_all
	and 	r1, r1, #vh_MODEMASK
	cmps	r1, #vh_IRQMODE

	ldreq	r2, vk_save_irq_current_tcb_bottom
	//ldrne	r2, vk_save_swi_current_tcb_bottom
	ldrne	r1, =vk_sched_save_tcb_ptr
	ldrne	r2, [r1]
	
	b		vh_save_ctx_bottom

vh_save_ctx_bottom:
	.rept	17 					// copy context from swi stack to USER stack
	ldr		r1, [r2], #4
	str		r1, [r0], #4
	.endr
	mov		pc, lr					// return to caller


vk_save_swi_mode_stack_ptr:	
	.long 0
vk_save_swi_current_tcb_bottom:
	.long 0
vk_save_irq_mode_stack_ptr:
	.long 0
vk_save_irq_current_tcb_bottom:
	.long 0
vk_save_pabort_stack_ptr:	
	.long 0
vk_save_pabort_current_tcb_bottom:
	.long 0

	.global vk_error_addr
vk_error_addr:
	.long 0
