// alarms.h
#ifndef ALARMS_H
#define ALARMS_H

#include <EEPROM.h>
#include "display.h"
#include "driver.h"
#include "rfid.h"

#define MAX_ALARMS 5  // Maximum number of alarms
#define EEPROM_SIZE 512  // Define the required EEPROM size
#define EEPROM_SIGNATURE 0xA501
#define EEPROM_SIGNATURE_ADDR 0
#define EEPROM_ALARM_COUNT_ADDR sizeof(uint16_t)  // After the signature

extern bool alarmIsActive;  // Declare the global flag
extern int selectedAlarmIndex;
extern int alarmCount;


struct Alarm {
  int hour;
  int minute;
  bool enabled;
  bool repeatsDaily;  // True if the alarm should repeat daily
};

extern Alarm alarms[MAX_ALARMS];

void EEPROMSetup();
void storeAlarmsToMemory();
void loadAlarmsFromMemory();
void storeAlarm(int hour, int minute, bool repeatsDaily);
//void displayAlarms();
void clearEEPROM();
void checkAndTriggerAlarms();
void dismissAlarm();
//int countSetAlarms();
void dailyAlarmReset();
Alarm& currentlySelectedAlarm();

#endif // ALARMS_H