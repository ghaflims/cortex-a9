#include "pl111.h"

UG_GUI gui;
uint32	volatile *fb;

inline static void draw_pixel(uint32 x, uint32 y, uint32 c){
	fb[(y*800)+x] = c;
}

void pdraw(UG_S16 x , UG_S16 y ,UG_COLOR c){
	//UG_COLOR cc;
	//RGB565
	draw_pixel(x,y,c);
}

void clcd_init(void){
	PL111MMIO	*plio;
	plio = (PL111MMIO*)PL111_IOBASE;
 
	/* 640x480 pixels */
	//plio->tim0 = 0x3f1f3f9c;
	plio->tim0 = 0x3f1f3cc4;
	//plio->tim1 = 0x080b61df;
	plio->tim1 = 0x080b6257;
	plio->upbase = 0x60110000;
	/* 16-bit color */
	plio->control = 0x192b;
	fb = (uint32*)0x60110000;
	int x;
	for (x = 0; x < (800 * 600); ++x)
		fb[x] = 0;

	UG_Init (&gui ,pdraw, 800, 600);
}
