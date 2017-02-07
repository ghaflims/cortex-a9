#include <errno.h>
#include <stddef.h> /* where ptrdiff_t is defined */
#include <stdlib.h>
#include <sys/stat.h>
#include "pl011.h"
/* Start of the heap.  */
extern const char _HEAP_START __attribute__((section(".heap")));
/* End of the heap (maximum value of heap_ptr).  */
extern const char _HEAP_MAX __attribute__((section(".heap")));

/* Extend heap space by size bytes.
   Return start of new space allocated, or -1 for errors 
   Error cases:
    1. Allocation is not within heap range */

void _exit(int r){
  while(1);
}

int _write(int f, char *ptr, int len){
  int i;
  for(i=0; i < len; i++){
    uart_putc(*ptr++);
  }
  return len;
}

extern int main(void);

void _start(void){
	main();
	exit(0);
}

int _fini(int i){
  return -1;
}
void * _sbrk (ptrdiff_t size)
{
  /*
  * The following two memory locations should be defined in the linker script file
  */

  static const char * heap_ptr;  /* pointer to head of heap */
  const char * old_heap_ptr;
  static unsigned char init_sbrk = 0;

  /* heap_ptr is initialized to HEAP_START */
  if (init_sbrk == 0) 
  {
    heap_ptr = &_HEAP_START;
    init_sbrk = 1;
  }

  old_heap_ptr = heap_ptr;

  if ((heap_ptr + size) > &_HEAP_MAX)
  { 
    /* top of heap is bigger than _HEAP_MAX */
    errno = ENOMEM;
    return (void *) -1;
  }

  /* success: update heap_ptr and return previous value */
  heap_ptr += size;
  return (void *)old_heap_ptr;
}

int _close(int f){
  return -1;
}

int _read(int f, char* ptr, int len){
  return -1;
}
int _lseek(int f, int ptr, int dir){
  return -1;
}
int _fstat(int f, struct stat* st){
  return -1;
}
int _isatty(int fd){
  return -1;
}
