#include "sp804.h"
#include "interrupt.h"
#include <stdio.h>
volatile timer804_t* const tregs = (timer804_t*)TIMER_BASE;
volatile uint32_t counter;
void timer_handler(void){
	printf("counter is: %lu\n",counter++);
	tregs->timers[0].IntClr = 0;
}
void timer_init(void){
	counter = 0;
	tregs->timers[0].Control = SP804_TIMER_PERIODIC | SP804_TIMER_32BIT  | SP804_TIMER_PRESCALE_256 | SP804_TIMER_INT_ENABLE;
	tregs->timers[0].Load = 0;
	tregs->timers[0].Value = 0;
	tregs->timers[0].RIS = 0;
	tregs->timers[0].MIS = 0;
	tregs->timers[0].BGLoad = 0x1000;
	tregs->timers[0].Control |= SP804_TIMER_ENABLE;
	install_isr(TIM01INT_IRQn,timer_handler);
	enable_irq(TIM01INT_IRQn);
}