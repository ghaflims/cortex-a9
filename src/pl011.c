#include "pl011.h"

volatile pl011_t* const UART0 = (pl011_t*)UART0_BASE;

inline void uart_putc(char c){
	while(UART0->FR & TXFF);
	UART0->DR = c;
}