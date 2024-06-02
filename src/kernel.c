#include <stdint.h>
#include <stdio.h>
#include "pl011.h"
#include "pl050.h"
#include "pl111.h"
#include "pl181.h"
#include "sp804.h"
#include "pl041.h"
#include "interrupt.h"
#include "ff.h"


int main(void){
 	interrupt_init();
	//sd_init();
	FATFS fs0;
	FIL fil;
	FRESULT fr;
	char line[100];
	char out[100];
	unsigned int wc;
	for(int i=0;i<100;i++)
		out[i]='A';
	for(int i=64;i<100;i++)
		out[i]='M';
	//mount sd card as FAT FS 
	f_mount(&fs0,"",0);
	fr = f_open(&fil,"test.txt",FA_READ);
	f_read(&fil,line,100,&wc);
	printf("%s",line);
	f_close(&fil);
	fr = f_open(&fil,"test2.txt",FA_WRITE|FA_CREATE_ALWAYS);
	f_write(&fil,out,100,&wc);
	f_close(&fil);
	printf("# of bytes written: %d\n",wc);
	clcd_init();
 	kb_init();
	aaci_init();
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
	aaci_test_sound();
	aaci_test_wavfile();
	//printf("Done reading SD CARD\n");
	//UG_PutString (50,50 ,"SD READ DONE");
	//unmount FAT FS
	f_unmount("");
 	for(;;);
	return 0;
}
