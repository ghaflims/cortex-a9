#include "pl031.h"

volatile pl031_t* const RTC = (pl031_t*)RTC_BASE;

uint32_t read_rtc(){
    return RTC->RTCDR;  
}