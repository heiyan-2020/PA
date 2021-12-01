#include <am.h>
#include <nemu.h>
#include <stdio.h>
void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
		uint64_t result = inl(RTC_ADDR + 4);
		result <<= 32;
		result |= inl(RTC_ADDR);
		uptime->us = result;
		uint64_t test = 1000000000000000L;
		printf("us = %ld\n", test);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
