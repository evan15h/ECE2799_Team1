#include "buttons.h"
#include "display.h"
#include "alarms.h"

State currentState = DISPLAY_TIME; // Initial state

uint8_t setHour = 12;  // Initial alarm hour
uint8_t setMinute = 0; // Initial alarm minute
unsigned long lastUserActivityTime = 0;
const unsigned long userInactivityLimit = 10000; // 15 seconds of inactivity allowed

unsigned long lastButtonPress = 0;


void buttonsSetup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLDOWN); 

}

void buttonsLoop() {
  switch (currentState) {
    case DISPLAY_TIME:
      displayTime();
      // if (readButtonOne()) {
      //   lastUserActivityTime = millis(); // Update activity time
      //   Serial.println("User active!");
      // }
      // if (readButtonTwo()) {
      //   lastUserActivityTime = millis(); // Update activity time
      //   Serial.println("User active!");
      // }
      if (isButtonOneHeld()) {
        currentState = SET_HOUR;
        setHour = rtc.now().hour(); // Start with the current hour
        display.clearDisplay();
        delay(750);
      } 
      else if (isButtonTwoHeld()) {  // Long press on button two to remove alarm
        if (alarmCount == 0) {
          displayNoAlarms();
        }
        else{
          display.clearDisplay();
          currentState = REMOVE_ALARM;
          delay(500);
        }
      }
      break;
    case SET_HOUR:
      displaySetHour();
      if (isButtonOneHeld()) { // Hold button one to exit
        //Exit
        currentState = DISPLAY_TIME;
      }
      if (readButtonOne()) {
        setHour = (setHour + 1) % 24; // Increment hour
        //refreshDisplayHour(setHour);  // Update display function to show changes
      }
      if (readButtonTwo()) {
        currentState = SET_MINUTE;
        setMinute = rtc.now().minute();
      }
      break;
    case SET_MINUTE:
      displaySetMinute();
      if (isButtonOneHeld()) { // Hold button one to exit
        //Exit
        currentState = DISPLAY_TIME;
      }
      if (readButtonOne()) {
        setMinute = (setMinute + 1) % 60; // Increment minute
        //refreshDisplayMinute(setHour, setMinute); // Update display function to show changes
      }
      if (readButtonTwo()) {
        display.clearDisplay();
        displayConfirmRepeat();
        currentState = CONFIRM_REPEAT;
      }
      break;
    case CONFIRM_REPEAT:
      if (readButtonOne()) {  // Button one for YES
        storeAlarm(setHour, setMinute, true);
        currentState = DISPLAY_TIME;
      }
      if (readButtonTwo()) {  // Button two for NO
        storeAlarm(setHour, setMinute, false);
        currentState = DISPLAY_TIME;
      }
      break;
    case REMOVE_ALARM:
      displayRemoveAlarm(); // Display instructions or info about removing alarms
      if (isButtonTwoHeld()) { // Hold button two to exit
        //Exit
        currentState = DISPLAY_TIME;
      }
      if (readButtonTwo()) { // Short press to cycle through alarms
        cycleThroughAlarms(); // Function to highlight each alarm
      } 
      if (readButtonOne()) { // Press button one to confirm removal
        removeSelectedAlarm(); // Function to remove the currently selected alarm
        displayRemoveSelectedAlarm();
        currentState = DISPLAY_TIME;
      }
      break;
  }
}

bool readButtonOne() {
  static unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
  static int buttonState;             // the current reading from the input pin
  static int lastButtonState = HIGH;   // the previous reading from the input pin
  int reading = digitalRead(BUTTON_PIN_1);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      lastButtonState = reading;
      if (buttonState == LOW) {
        lastUserActivityTime = millis(); // Update activity time
        Serial.println("User active!");
        return true;
      }
    }
  }
  lastButtonState = reading;
  return false;
}

bool readButtonTwo() {
  static unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
  static int buttonState;             // the current reading from the input pin
  static int lastButtonState = LOW;   // the previous reading from the input pin
  int reading = digitalRead(BUTTON_PIN_2);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      lastButtonState = reading;
      if (buttonState == HIGH) {
        lastUserActivityTime = millis(); // Update activity time
        Serial.println("User active!");
        return true;
      }
    }
  }
  lastButtonState = reading;
  return false;
}

bool isButtonOneHeld() {
  static unsigned long buttonPressTime = 0;  // When the button was first pressed
  static bool isButtonPressed = false;       // Current press state

  int currentButtonState = digitalRead(BUTTON_PIN_1);

  // Check if the button is currently pressed
  if (currentButtonState == LOW) {
    if (!isButtonPressed) {
      // Button was just pressed down
      isButtonPressed = true;
      buttonPressTime = millis();  // Reset the timer at the start of a press
    } 
    else {
      // Button is held down
      if ((millis() - buttonPressTime) >= LONG_PRESS_TIME) {
        // Only report true once, and only after the button is held down long enough
        if (isButtonPressed) {
          isButtonPressed = false;  // Prevent repeated triggers
          lastUserActivityTime = millis(); // Update activity time
          Serial.println("User active!");
          return true;
        }
      }
    }
  } 
  else {
    // Button is not pressed
    isButtonPressed = false;
  }
  return false;
}

bool isButtonTwoHeld() {
  static unsigned long buttonPressTime = 0;  // When the button was first pressed
  static bool isButtonPressed = false;       // Current press state

  int currentButtonState = digitalRead(BUTTON_PIN_2);

  // Check if the button is currently pressed
  if (currentButtonState == HIGH) {
    if (!isButtonPressed) {
      // Button was just pressed down
      isButtonPressed = true;
      buttonPressTime = millis();  // Reset the timer at the start of a press
    } 
    else {
      // Button is held down
      if ((millis() - buttonPressTime) >= LONG_PRESS_TIME) {
        // Only report true once, and only after the button is held down long enough
        if (isButtonPressed) {
          isButtonPressed = false;  // Prevent repeated triggers
          lastUserActivityTime = millis(); // Update activity time
          Serial.println("User active!");
          return true;
        }
      }
    }
  } 
  else {
    // Button is not pressed
    isButtonPressed = false;
  }
  return false;
}