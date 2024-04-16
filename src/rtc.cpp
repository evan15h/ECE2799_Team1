#include <Wire.h>
#include <RTClib.h>

//RTC_PCF8523 rtc; // Create an instance of the RTC_PCF8523 class

#define DS1307_ADDRESS 0x68 ///< I2C address for DS1307
#define DS1307_CONTROL 0x07 ///< Control register
#define DS1307_NVRAM 0x08

RTC_DS1307 rtc;

void rtcSetup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, set the date & time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set January 1, 2020 at 00:00 you would call:
    // rtc.adjust(DateTime(2020, 1, 1, 0, 0, 0));
  }
}

void rtcLoop() {
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000); // Wait for a second before updating the time
}

void displayTime(DateTime now){
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}

void displayAlarm(DateTime alarm, int alarmNum){
  Serial.print(alarmNum, DEC);
  Serial.print(' ');
  Serial.print(alarm.hour(), DEC);
  Serial.print(':');
  Serial.print(alarm.minute(), DEC);
  Serial.print(':');
}