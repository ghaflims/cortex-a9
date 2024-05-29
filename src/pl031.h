#ifndef __PL031_H__
#define __PL031_H__
#include <stdint.h>
#define RTC_BASE 0x10017000
uint32_t read_rtc();

typedef volatile struct {
	const uint32_t RTCDR;
	uint32_t RTCMR;
	uint32_t RTCLR;
	uint32_t RTCCR;
	uint32_t RTCIMSC;
	const uint32_t RTCRIS;
	const uint32_t RTCMIS;
	uint32_t RTCICR;
} pl031_t;

#endif // __PL031_H__