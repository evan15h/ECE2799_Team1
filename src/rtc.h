// rtc.h
#ifndef RTC_H
#define RTC_H

#include <RTClib.h>

// Declare the display object and functions
extern RTC_DS1307 rtc;
extern DateTime lastDismissTime;

void rtcSetup();
//void printTime();
//void printWithLeadingZero(int num);

#endif