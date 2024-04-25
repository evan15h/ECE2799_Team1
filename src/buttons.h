// buttons.h
#ifndef BUTTONS_H
#define BUTTONS_H

#include <Wire.h>
#include <Arduino.h>

// Pin definitions
#define BUTTON_PIN_1 0
#define BUTTON_PIN_2 4

#define DEBOUNCE_DELAY 75
#define LONG_PRESS_TIME 850

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
extern uint8_t setHour;
extern uint8_t setMinute;
extern unsigned long lastUserActivityTime;
extern const unsigned long userInactivityLimit; // 15 seconds of inactivity allowed

// Function declarations
void buttonsSetup();
void buttonsLoop();
bool readButtonOne();
bool readButtonTwo();
bool isButtonOneHeld();
bool isButtonTwoHeld();

#endif // BUTTONS_H
