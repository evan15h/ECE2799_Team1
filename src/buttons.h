// buttons.h
#ifndef BUTTONS_H
#define BUTTONS_H

#include <Wire.h>
#include <Arduino.h>

// Pin definitions
#define BUTTON_PIN_1 0
#define BUTTON_PIN_2 4

#define DEBOUNCE_DELAY 75
#define LONG_PRESS_TIME 1000

// Define states for the UI
enum State {
  DISPLAY_TIME,
  SET_HOUR,
  SET_MINUTE,
  CONFIRM_REPEAT,
  CONFIRM_ALARM,
  REMOVE_ALARM
};

extern State currentState;
extern int setHour;
extern int setMinute;

// Function declarations
void buttonsSetup();
void buttonsLoop();
bool readButtonOne();
bool readButtonTwo();
bool isButtonOneHeld();
bool isButtonTwoHeld();

#endif // BUTTONS_H
