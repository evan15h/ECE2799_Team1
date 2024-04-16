#include "buttons.h"
#include "display.h"

#define DEBOUNCE_DELAY 80

State currentState = DISPLAY_TIME; // Initial state

int setHour = 12;  // Initial alarm hour
int setMinute = 0; // Initial alarm minute

unsigned long lastButtonPress = 0;
const int longPressTime = 5000; // Time in milliseconds to count as long press

Alarm alarms[MAX_ALARMS];
int alarmCount = 0;  // Current number of alarms stored


void buttonsSetup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLDOWN); 

}

void buttonsLoop() {
  switch (currentState) {
    case DISPLAY_TIME:
      displayTime();
      if (isButtonOneHeld()) {
        currentState = SET_HOUR;
        setHour = rtc.now().hour(); // Start with the current hour
        display.clearDisplay();
      }
      break;
    case SET_HOUR:
      displaySetHour();
      if (readButtonOne()) {
        setHour = (setHour + 1) % 24; // Increment hour
        refreshDisplayHour(setHour);  // Update display function to show changes
      }
      if (readButtonTwo()) {
        currentState = SET_MINUTE;
        setMinute = 0;
      }
      break;
    case SET_MINUTE:
      displaySetMinute();
      if (readButtonOne()) {
        setMinute = (setMinute + 1) % 60; // Increment minute
        refreshDisplayHour(setMinute); // Update display function to show changes
      }
      if (readButtonTwo()) {
        currentState = CONFIRM_ALARM;
        storeAlarm(setHour, setMinute);
        currentState = DISPLAY_TIME; // Return to display time
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
        return true;
      }
    }
  }
  lastButtonState = reading;
  return false;
}

bool isButtonOneHeld() {
  bool isHeld = false;
  if (digitalRead(BUTTON_PIN_1) == LOW) {
    if (millis() - lastButtonPress >= longPressTime) {
      isHeld = true;
      lastButtonPress = millis(); // Reset timer to prevent continuous recognition
    }
  }
  return isHeld;
}

bool isButtonTwoHeld() {
  bool isHeld = false;
  if (digitalRead(BUTTON_PIN_2) == LOW) {
    if (millis() - lastButtonPress >= longPressTime) {
      isHeld = true;
      lastButtonPress = millis(); // Reset timer to prevent continuous recognition
    }
  }
  return isHeld;
}

void displaySetHour() {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  int hourX = centerText(String(setHour) + ":" + String(rtc.now().minute()), 3);
  display.setCursor(hourX, 20);
  display.println(String(setHour) + ":" + String(rtc.now().minute()));
  display.display();
}

void displaySetMinute() {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  int minuteX = centerText(String(setHour) + ":" + String(setMinute), 3);
  display.setCursor(minuteX, 20);
  display.println(String(setHour) + ":" + String(setMinute));
  display.display();
}

void storeAlarm(int hour, int minute) {
  if (alarmCount < MAX_ALARMS) {
    alarms[alarmCount++] = {hour, minute, true};  // Store new alarm and increment count
  } else {
    Serial.println("Alarm list full!");
  }
}

void displayAlarms() {
  for (int i = 0; i < alarmCount; i++) {
    if (alarms[i].enabled) {
      Serial.print("Alarm ");
      Serial.print(i+1);
      Serial.print(": ");
      Serial.print(alarms[i].hour);
      Serial.print(":");
      Serial.println(alarms[i].minute);
      // Add more detailed display handling as needed
    }
  }
}
