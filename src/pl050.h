#ifndef __PL050_H__
#define __PL050_H__

#define KMI_KB_BASE 0x10006000


typedef struct {
	uint32_t cr;
	uint32_t status;
	uint32_t data;
	uint32_t clk;
	uint32_t ir;
	
} kmi_t;

void kb_handler(void);
void kb_init(void);

#endif
