#include <Wire.h>
#include <Adafruit_DRV2605.h>

// Define the I2C pins for ESP32
#define I2C_SDA 21
#define I2C_SCL 22

Adafruit_DRV2605 drv;

void driverSetup() {
  Serial.begin(115200);
  Serial.println("Initializing the DRV2605 haptic controller");

  drv.begin();

  // Explicitly define I2C pins, if necessary
  Wire.begin(I2C_SDA, I2C_SCL);

  drv.begin();

  // Set the haptic driver to use the I2C address (if necessary)
  // drv.begin(&Wire); // Uncomment if you need to explicitly set the I2C address

  // Select a haptic effect library.
  drv.selectLibrary(1); // 1 for ERM motors

  // Set mode to internal trigger
  drv.setMode(DRV2605_MODE_INTTRIG);
}

void driverLoop() {
  Serial.println("Playing effect #1");
  drv.setWaveform(0, 1); // effect 1
  drv.setWaveform(1, 0); // end of waveform sequence
  drv.go();

  delay(1000);

  Serial.println("Playing effect #2");
  drv.setWaveform(0, 2); // effect 2
  drv.setWaveform(1, 0); // end of waveform sequence
  drv.go();

  delay(1000);

  // Add more effects as desired
}
