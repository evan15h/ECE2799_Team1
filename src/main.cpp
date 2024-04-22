#include <WiFi.h>
#include "display.h"
#include "rfid.h"
//#include "rtc.h"
#include "esp_sleep.h"
#include "driver.h"
#include "buttons.h"
#include "alarms.h"

#define WAKEUP_PIN GPIO_NUM_0

void setup() {
    Serial.begin(115200);

    while (!Serial) {
        delay(10);
    }

    // Disable WiFi and Bluetooth to save power
    WiFi.mode(WIFI_OFF);
    btStop();

    pinMode(WAKEUP_PIN, INPUT_PULLUP);  // Set the wakeup pin

    // Configure the wakeup source as a button press (falling edge)
    esp_sleep_enable_ext0_wakeup(WAKEUP_PIN, 0);  // 0 = LOW, wake up on falling edge

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
        Serial.println("Woken up by external pin");
        // Code to turn on and update OLED display goes here
    }

    // Configure the ESP32 to wake up after 60 seconds
    //esp_sleep_enable_timer_wakeup(60 * 1000000ULL); // 60 seconds in microseconds

    //Serial.println("Going to deep sleep now");
    //esp_deep_sleep_start(); // Enter deep sleep mode

    rtcSetup();
    
    displaySetup();

    buttonsSetup();

    rfidSetup();

    driverSetup();

    //EEPROMSetup();
    //clearEEPROM();

    
}

void loop() {
    buttonsLoop(); // Check button inputs and change states accordingly
    checkAndTriggerAlarms();
    checkRFID();
    // Additional logic to decide when to go to sleep again, if needed
    dailyAlarmReset();
}