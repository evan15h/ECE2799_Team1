#include <WiFi.h>
#include "display.h"
#include "rfid.h"
#include "rtc.h"
#include "esp_sleep.h"
#include "driver.h"
#include "buttons.h"
#include "alarms.h"

#define WAKEUP_PIN GPIO_NUM_0

bool userIsActive();
void goToSleep();

void setup() {
    Serial.begin(115200);

    while (!Serial) {
        delay(10);
    }

    lastUserActivityTime = millis();

    // Disable WiFi and Bluetooth to save power
    WiFi.mode(WIFI_OFF);
    btStop();

    pinMode(WAKEUP_PIN, INPUT_PULLUP);  // Set the wakeup pin
    // Configure the wakeup source as a button press (falling edge)
    esp_sleep_enable_ext0_wakeup(WAKEUP_PIN, 0);  // 0 = LOW, wake up on falling edge

    // Needed setups to check if there is an alarm
    rtcSetup();
    displaySetup();
    EEPROMSetup();
    loadAlarmsFromMemory();

    DateTime now = rtc.now();
    bool alarmTriggered = false;

    for (int i = 0; i < alarmCount; i++) {
        if (alarms[i].enabled && alarms[i].hour == now.hour() && alarms[i].minute == now.minute()) {
            alarmTriggered = true;
            break;
        }
    }

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0 || alarmTriggered) {
        Serial.println("Woken up by external pin or alarm time matched.");
        buttonsSetup();
        rfidSetup();
        driverSetup();
    } else {
        Serial.println("No alarm matched, going back to deep sleep.");
        turnOffDisplay();
        esp_sleep_enable_timer_wakeup(60 * 1000000ULL); // 60 seconds in microseconds
        esp_deep_sleep_start(); // Enter deep sleep mode
    }

    //clearEEPROM();
}

void loop() {
    if (userIsActive()){
        buttonsLoop(); // Check button inputs and change states accordingly
        checkAndTriggerAlarms(); // Check for alarms and trigger
        checkRFID(); // Checks for known RFID tag if alarm is active
        dailyAlarmReset(); // For disabled alarms when the clock passes midnight
    }
    else{
        turnOffDisplay();
        goToSleep();
    }
}

bool userIsActive() {
    return (millis() - lastUserActivityTime <= userInactivityLimit);
}

void goToSleep() {
    Serial.println("User inactive, going to deep sleep.");
    esp_sleep_enable_timer_wakeup(60 * 1000000ULL); // Configure to wake up after 60 seconds
    esp_deep_sleep_start();
}