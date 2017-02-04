#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "font5x7.h"
#include "ugui.h"


#define PL111_CR_EN		0x001
#define PL111_CR_PWR		0x800
#define PL111_IOBASE		0x10020000
#define PL111_PALBASE		(PL111_IOBASE + 0x200)

#define KMI_KB_BASE 0x10006000
//#define GIC_BASE 0x1E000000
//#define GIC_BASE 0x1E021000
#define GIC_INTERFACE 0x1e000100
#define GIC_DISTRIBUTOR 0x1e001000



#define ICDDCR 0
#define ICDICTR 4
#define ICDIIDR 8
#define ICDISR	0x80	
#define ICDISER 0x100	
#define ICDCER 0x180	
#define ICDISPR 0x200	
#define ICDICRP 0x280	
#define ICDABR 0x300	
#define ICDIPR 0x400	
#define ICDIPTR 0x800	
#define ICDICFR 0xC00
#define ICPPISR 0xD00

#define ICCICR 0
#define ICCPMR 4
#define ICCBPR 0x8
#define ICCIAR 0xC
#define ICCEOIR 0x10
#define ICCRPR 0x14
#define ICCIDR 0xFC

typedef unsigned int		uint32;
typedef unsigned char		uint8;
typedef unsigned short		uint16;
UG_GUI gui;

unsigned char static kbdus[128] = {

//	0 1 2 3 4 5 6 7 8 9 a b c d e f	
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 0x00 - 0x0f 
	0,0,0,0,0,'q','1',0,0,0,'z','s','a','w','2',0, // 0x10 - 0x1f 
	0,'c','x','d','e','4','3',0,0,' ','v','f','t','r','5',0, // 0x20 - 0x2f 
	0,'n','b','h','g','y','6',0,0,0,'m','j','u','7','8',0, // 0x30 - 0x3f 
	0,',','k','i','o','0','9',0,0,'.','/','l',';','p','-',0, // 0x40 - 0x4f 
	0,0,'\'',0,'[','=',0,0,0,0,'\n',']',0,0,0,0, // 0x50 - 0x5f 
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 0x60 - 0x6f 
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 // 0x70 - 0x7f
};	
 

/* Flags for the timer control registers  */
#define SP804_TIMER_ENABLE       (1 << 7)
#define SP804_TIMER_PERIODIC     (1 << 6)
#define SP804_TIMER_INT_ENABLE   (1 << 5)
#define SP804_TIMER_PRESCALE_1   (0 << 2)
#define SP804_TIMER_PRESCALE_16  (1 << 2)
#define SP804_TIMER_PRESCALE_256 (2 << 2)
#define SP804_TIMER_32BIT        (1 << 1)
#define SP804_TIMER_ONESHOT (1 << 0)
static volatile struct {
    struct {
        uint32_t Load;     /* +0x00 */
        uint32_t Value;    /* +0x04 */
        uint32_t Control;  /* +0x08 */
        uint32_t IntClr;   /* +0x0C */
        uint32_t RIS;      /* +0x10 */
        uint32_t MIS;      /* +0x14 */
        uint32_t BGLoad;   /* +0x18 */
        uint32_t Reserved; /* +0x1C */
    } timers[2];
} * const tregs = (void*)0x10011000;

typedef volatile struct {
 uint32_t DR;
 uint32_t RSR_ECR;
 uint8_t reserved1[0x10];
 const uint32_t FR;
 uint8_t reserved2[0x4];
 uint32_t LPR;
 uint32_t IBRD;
 uint32_t FBRD;
 uint32_t LCR_H;
 uint32_t CR;
 uint32_t IFLS;
 uint32_t IMSC;
 const uint32_t RIS;
 const uint32_t MIS;
 uint32_t ICR;
 uint32_t DMACR;
} pl011_t;

typedef struct {
	uint32_t cr;
	uint32_t status;
	uint32_t data;
	uint32_t ir;
	
} kmi_t;
enum {
 RXFE = 0x10,
 TXFF = 0x20,
};
 
pl011_t * const UART0 = (pl011_t *)0x10009000;

static void uart_putc(char c){
	while(UART0->FR & TXFF);
	UART0->DR = c;
}

int _write(int f, char *ptr, int len){
	int i;
	for(i=0; i < len; i++){
		uart_putc(*ptr++);
	}
	return len;
}
typedef struct _PL111MMIO 
{
	uint32		volatile tim0;		//0
	uint32		volatile tim1;		//4
	uint32		volatile tim2;		//8
	uint32		volatile tim3;		//c
	uint32		volatile upbase;	//10
	uint32		volatile f;		//14
	uint32		volatile control;		//18
	uint32		volatile g;	//1c
} PL111MMIO;
uint32		volatile *fb;

inline static void draw_pixel(uint32 x, uint32 y, uint32 c){
	fb[(y*800)+x] = c;
}

void pdraw(UG_S16 x , UG_S16 y ,UG_COLOR c){
	//UG_COLOR cc;
	//RGB565
	draw_pixel(x,y,c);
}

void draw_frect(uint32 x, uint32 y, uint32 l, uint32 w, uint16 c)
{
	int i, j;
	for(i=0; i<l; i++)
	{
		for(j=0; j<w; j++)
		{
			draw_pixel(x+i, y+j, c);
		}
	}
}
void draw_char(uint32 x, uint32 y, uint32 color,char c){
	int ii,jj;
	char line;
	for(ii=0;ii<6;ii++){
		if(ii==5)
			line = 0;
		else
			line = font[ii+(c*5)];
		for(jj=0;jj<8;jj++){
			if(line & 0x01){
				draw_pixel(x+ii,y+jj,color);
			}else{
				draw_pixel(x+ii,y+jj,color>>16);
			}
			line>>=1;
		}
	}
	
}
/*
void draw_string(uint32 x, uint32 y, uint32 color,char* s){
	if(x%6!=0 || y%8!=0 || strlen(s) > 168)
		return;
	int i=0;
	int j=0;
	int dy = 0;
	while(*(s+i)){
		if((i%200)==0 && i!=0){
			y+=8;
			x=0;
			j=0;
		}
		draw_char(x+(j*6),y,color,*(s+i));
		i++;
		j++;
	}
}
*/
volatile uint8_t tsc; 
void __attribute__ ((interrupt("SWI"))) c_svc(void){
	printf("yaay SVC ticked\n");
}
void __attribute__ ((interrupt("FIQ"))) c_fiq(void){
	printf("yaay FIQ ticked\n");
}
void __attribute__ ((interrupt("IRQ"))) c_irq(void){
	//printf("yaay IRQ ticked\n");
	(*(volatile uint32_t*)(GIC_DISTRIBUTOR+ICDISPR+4)) &= ~(1<<12);
	tsc = ((kmi_t*)(KMI_KB_BASE))->data;
	if(tsc & 0x80){

 	}else{
 		uart_putc(kbdus[tsc]);
 	}
}
void c_entry(void)
{
	// set the priority and enable distributer and cpu pending
	(*(volatile uint32_t*)(GIC_DISTRIBUTOR+ICDISER+4)) |=  (1<<12);
	(*(volatile uint32_t*)(GIC_DISTRIBUTOR+ICDIPTR+0x34)) = 0xffffffff; // target process 0 for id 
	(*(volatile uint32_t*)(GIC_DISTRIBUTOR+ICDICFR+0xc)) = 0x55555555;
	(*(volatile uint32_t*)(GIC_INTERFACE+ICCPMR)) = 0xffff;
	(*(volatile uint32_t*)(GIC_INTERFACE+ICCICR)) = 1;
	(*(volatile uint32_t*)(GIC_DISTRIBUTOR+ICDDCR)) = 1;
	asm volatile("cpsie i" : : : "memory", "cc");
	PL111MMIO	*plio;
	int		x;
	
 	char *memptr = malloc(8);
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
	//memset(fb,0xff,800*600*sizeof(uint16));
	for (x = 0; x < (800 * 600); ++x)
		fb[x] = 0;
 	draw_frect(100,200,40,40,0x001f);
 	//draw_string(60, 80, 0x0000001f,"hello from the other side..!!");
 	//uart_putc('M');
 	//uart_putc('u');
 	printf("ptr address: %p\n",memptr);
 	kmi_t volatile *kmi_kb;
 	kmi_kb = (kmi_t*)(KMI_KB_BASE);
 	kmi_kb->cr = 0x14;
 	uint8_t sc;
 	tregs->timers[1].Control = SP804_TIMER_ENABLE | SP804_TIMER_32BIT;
 	UG_Init (&gui ,pdraw, 800, 600);
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
	return;
}
