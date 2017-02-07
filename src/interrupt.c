#include "interrupt.h"
#include "pl050.h"
#include <stdio.h>
void __attribute__ ((interrupt("SWI"))) c_svc(void){
	printf("yaay SVC ticked\n");
}
void __attribute__ ((interrupt("FIQ"))) c_fiq(void){
	printf("yaay FIQ ticked\n");
}
void __attribute__ ((interrupt("IRQ"))) c_irq(void){
	GIC_ClearPendingIRQ(KMI0_INTR_IRQn);
	kb_handler();
	GIC_EndInterrupt(KMI0_INTR_IRQn);
}

void interrupt_init(void){
	GIC_Enable();
	GIC_EnableIRQ(KMI0_INTR_IRQn);
	asm volatile("cpsie i" : : : "memory", "cc");
}
