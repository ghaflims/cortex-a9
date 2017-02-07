#include "sp804.h"

volatile timer804_t* const tregs = (timer804_t*)TIMER_BASE;

void timer_init(void){
	tregs->timers[1].Control = SP804_TIMER_ENABLE | SP804_TIMER_32BIT;
}