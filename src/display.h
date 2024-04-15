// display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

// Declare the display object and functions
extern Adafruit_SSD1306 display;
extern RTC_DS1307 rtc;

void displaySetup();
void displayTime();
int centerText(String text, int textSize);
int calculateYPosition(int newSize, int previousY);
void refreshDisplayHour(int hour);

#endif
