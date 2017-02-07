#ifndef __SP804_H__
#define __SP804_H__
#include <stdint.h>
/* Flags for the timer control registers  */
#define SP804_TIMER_ENABLE       (1 << 7)
#define SP804_TIMER_PERIODIC     (1 << 6)
#define SP804_TIMER_INT_ENABLE   (1 << 5)
#define SP804_TIMER_PRESCALE_1   (0 << 2)
#define SP804_TIMER_PRESCALE_16  (1 << 2)
#define SP804_TIMER_PRESCALE_256 (2 << 2)
#define SP804_TIMER_32BIT        (1 << 1)
#define SP804_TIMER_ONESHOT (1 << 0)

#define TIMER_BASE 0x10011000
typedef volatile struct {
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
} timer804_t;

void timer_init(void);
#endif