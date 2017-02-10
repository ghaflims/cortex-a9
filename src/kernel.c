#include <stdint.h>
#include <stdio.h>
#include "pl011.h"
#include "pl050.h"
#include "pl111.h"
#include "sp804.h"
#include "interrupt.h"

int main(void){
 	interrupt_init();
	clcd_init();
 	kb_init();
 	timer_init();
 	UG_FillCircle(100, 100, 30, C_YELLOW);
 	UG_FillCircle(200, 100, 10, C_RED);
 	UG_FillCircle(250, 100, 10, C_BLUE);
 	UG_FillCircle(350, 100, 20, C_GREEN);
 	UG_FillCircle(300, 100, 10, 0xff0000);
 	UG_DrawFrame(400,400,440,440,C_WHITE);
 	UG_FontSelect(&FONT_16X26);
 	UG_SetBackcolor(C_BLACK);
	UG_SetForecolor(C_YELLOW);
 	UG_PutString (200,200 ,"hello form the other side\nthis is a new line hehehehehhehehehehehehe") ;
 	asm volatile("SVC 0x05");
 	for(;;);
	return 0;
}
