// buttons.h
#ifndef BUTTONS_H
#define BUTTONS_H

#include <Wire.h>
#include <Arduino.h>

// Pin definitions
#define BUTTON_PIN_1 0
#define BUTTON_PIN_2 4
#define MAX_ALARMS 5  // Maximum number of alarms

// Define states for the UI
enum State {
  DISPLAY_TIME,
  SET_HOUR,
  SET_MINUTE,
  CONFIRM_ALARM
};

struct Alarm {
  int hour;
  int minute;
  bool enabled;
};

extern Alarm alarms[MAX_ALARMS];

// Function declarations
void buttonsSetup();
void buttonsLoop();
bool readButtonOne();
bool readButtonTwo();
bool isButtonOneHeld();
void displaySetHour();
void displaySetMinute();
void storeAlarm(int hour, int minute);
void displayAlarms();

#endif // BUTTONS_H
