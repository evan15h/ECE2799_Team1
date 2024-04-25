// alarms.h
#ifndef ALARMS_H
#define ALARMS_H

#include <EEPROM.h>
#include "display.h"
#include "driver.h"
#include "rfid.h"

#define MAX_ALARMS 10  // Maximum number of alarms
#define EEPROM_SIZE 512  // Define the required EEPROM size
#define EEPROM_INIT_CHECK 0  // Address to store a special initialization flag
#define ALARM_COUNT_ADDRESS sizeof(uint8_t)  // Address to store the number of alarms
#define ALARMS_START_ADDRESS (ALARM_COUNT_ADDRESS + sizeof(int))
#define EEPROM_SIGNATURE 0xA5
#define EEPROM_SIGNATURE_ADDR 0
#define EEPROM_ALARM_COUNT_ADDR sizeof(uint16_t)  // After the signature

extern bool alarmIsActive;  // Declare the global flag
extern int selectedAlarmIndex;
extern int alarmCount;


struct Alarm {
  uint8_t hour;
  uint8_t minute;
  bool enabled;
  bool repeatsDaily;  // True if the alarm should repeat daily
};

extern Alarm alarms[MAX_ALARMS];

void EEPROMSetup();
void storeAlarmsToMemory();
void loadAlarmsFromMemory();
void storeAlarm(uint8_t hour, uint8_t minute, bool repeatsDaily);
void displayAlarms();
void clearEEPROM();
void checkAndTriggerAlarms();
void dismissAlarm();
void removeSelectedAlarm();
void dailyAlarmReset();
void reenableAlarms();
Alarm& currentlySelectedAlarm();

#endif // ALARMS_H