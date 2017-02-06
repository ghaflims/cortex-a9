#include "pl011.h"

pl011_t * const UART0 = (pl011_t *)0x10009000;

inline void uart_putc(char c){
	while(UART0->FR & TXFF);
	UART0->DR = c;
}