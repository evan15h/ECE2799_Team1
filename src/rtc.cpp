#include <Wire.h>
#include "rtc.h"

//RTC_PCF8523 rtc; // Create an instance of the RTC_PCF8523 class

#define DS1307_ADDRESS 0x68 ///< I2C address for DS1307
#define DS1307_CONTROL 0x07 ///< Control register
#define DS1307_NVRAM 0x08

RTC_DS1307 rtc;
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
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second() + 30));
  }

}

// void printTime() {
//   if (!rtc.isrunning()) {
//     Serial.println("RTC is NOT running!");
//     // Set the date and time to compile time
//     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//   }

//   DateTime now = rtc.now();
//   Serial.print(now.year(), DEC);
//   Serial.print('/');
//   printWithLeadingZero(now.month());
//   Serial.print('/');
//   printWithLeadingZero(now.day());
//   Serial.print(" ");
//   printWithLeadingZero(now.hour());
//   Serial.print(':');
//   printWithLeadingZero(now.minute());
//   Serial.print(':');
//   printWithLeadingZero(now.second());
//   Serial.println();
// }

// // Helper function to print a number with leading zero if it is less than 10
// void printWithLeadingZero(int num) {
//   if (num < 10) {
//     Serial.print('0');
//   }
//   Serial.print(num, DEC);
// }