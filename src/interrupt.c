#include "interrupt.h"
#include "pl050.h"
#include <stdio.h>

func_t isr_table[MAXIRQNUM];
void install_isr(IRQn_Type irq_num, func_t handler){
	isr_table[irq_num] = handler;
}
void __attribute__ ((interrupt("SWI"))) c_svc(void){
	printf("yaay SVC ticked\n");
}
void __attribute__ ((interrupt("FIQ"))) c_fiq(void){
	printf("yaay FIQ ticked\n");
}
void __attribute__ ((interrupt("IRQ"))) c_irq(void){
	asm volatile("cpsid i" : : : "memory", "cc");
	int irq_num = GIC_AcknowledgePending();
	GIC_ClearPendingIRQ(irq_num);
	if(isr_table[irq_num] != NULL){
		isr_table[irq_num]();
	}else{
		printf("no handler found for %d\n",irq_num);
	}
	
	GIC_EndInterrupt(irq_num);
	asm volatile("cpsie i" : : : "memory", "cc");
}

void enable_irq(IRQn_Type irq_num){
	GIC_EnableIRQ(irq_num);
}
void interrupt_init(void){
	GIC_Enable();
	asm volatile("cpsie i" : : : "memory", "cc");
}
