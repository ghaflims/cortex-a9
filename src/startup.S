.global _Reset
.section .isr_vector
_Reset:
	B reset_handler
	B .
	B c_svc
	B .
	B .
	B .
	B c_irq
	B c_fiq
.text
reset_handler:
	ldr r0, =0x60000000
	mcr p15,0,r0,c12,c0,0
	ldr sp,=stackSVC

	cps  #0x1b
	ldr sp,=stackUND

	cps #0x17
	ldr sp,=stackABT

	cps #0x12
	ldr sp,=stackIRQ

	cps #0x11
	ldr sp,=stackFIQ

	cps #010
	ldr sp,=stackUSR

	BL _start
	B .

svc_handler:
	BL c_svc
//	movs pc,lr
	
