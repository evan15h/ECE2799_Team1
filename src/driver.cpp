#include <Wire.h>
#include "driver.h"

// Define the I2C pins for ESP32
#define I2C_SDA 21
#define I2C_SCL 22

Adafruit_DRV2605 drv;

void driverSetup() {
  Serial.begin(115200);

  // Start I2C communication
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialize the DRV2605 device on the I2C bus
  if (!drv.begin()) {
    Serial.println("Could not find DRV2605");
    while (1) delay(10); // Halt execution if DRV2605 is not found
  }

  // Select a haptic effect library.
  drv.selectLibrary(1); // 1 for ERM (Eccentric Rotating Mass) motors

  // Set mode to internal trigger
  drv.setMode(DRV2605_MODE_INTTRIG);
}

void playEffect(uint8_t effect) {
  Serial.println("Playing effect #" + String(effect));
  drv.setWaveform(0, effect); // Set the effect
  drv.setWaveform(1, 0);      // End of waveform sequence
  drv.go();                   // Trigger the effect
}

void activateVibrationMotor() {

  playEffect(1); // Play effect #1
  delay(100);   // Wait for a second
}

void stopVibrationMotor() {
  drv.stop();
}
