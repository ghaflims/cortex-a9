#ifndef __PL111_H__
#define __PL111_H__
#include "ugui.h"


#define PL111_CR_EN		0x001
#define PL111_CR_PWR		0x800
#define PL111_IOBASE		0x10020000
#define PL111_PALBASE		(PL111_IOBASE + 0x200)

typedef unsigned int		uint32;
typedef unsigned char		uint8;
typedef unsigned short		uint16;


typedef struct _PL111MMIO{
	uint32		volatile tim0;		//0
	uint32		volatile tim1;		//4
	uint32		volatile tim2;		//8
	uint32		volatile tim3;		//c
	uint32		volatile upbase;	//10
	uint32		volatile f;		//14
	uint32		volatile control;		//18
	uint32		volatile g;	//1c
} PL111MMIO;


void clcd_init(void);

#endif
