#include <am.h>
#include <nemu.h>
#include <stdio.h>
void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
				uint32_t a = inl(RTC_ADDR);
				uint32_t b = inl(RTC_ADDR);
				printf("first is %u why why\n", a);
				printf("second is %u why why\n", b);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
