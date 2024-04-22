// driver.h
#ifndef DRIVER_H
#define DRIVER_H

#include <Adafruit_DRV2605.h>
#include "rtc.h"
#include "alarms.h"

// Declare the display object and functions
extern Adafruit_DRV2605 drv;

void driverSetup();
void driverLoop();
void playEffect(uint8_t effect);
void activateVibrationMotor();
void stopVibrationMotor();

#endif