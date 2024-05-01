#include <Wire.h>
#include "rtc.h"

//RTC_PCF8523 rtc; // Create an instance of the RTC_PCF8523 class

#define DS1307_ADDRESS 0x68 ///< I2C address for DS1307
#define DS1307_CONTROL 0x07 ///< Control register
#define DS1307_NVRAM 0x08

RTC_PCF8523 rtc;
DateTime lastDismissTime;

void rtcSetup() {
  Serial.begin(115200);
  // Wait for serial port to open on native USB devices
  while (!Serial) {
    delay(1);
  }

  // Start the RTC module
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Check if the RTC is running
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // Add an estimated delay time, e.g., 30 seconds
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second() + 28));
  }

}