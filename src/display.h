// display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include "buttons.h"
#include "rtc.h"
#include "alarms.h"

// Declare the display object and functions
extern Adafruit_SSD1306 display;
extern RTC_DS1307 rtc;

void displaySetup();
void displayTime();
int centerText(String text, int textSize);
std::pair<int, int> centerArrow(String text, int textSize);
int calculateYPosition(int newSize, int previousY);
uint16_t strWidth(const String& str, int textSize);
void displaySetHour();
void displaySetMinute();
void displayConfirmRepeat();
void displayRemoveAlarm();
void cycleThroughAlarms();
void displayRemoveSelectedAlarm();
void displayNoAlarms();

#endif
