// rfid.h
#ifndef RFID_H
#define RFID_H

// Define the pins used for the SPI connection of the RFID reader
#define RST_PIN 17  // Reset pin for the RFID reader
#define SS_PIN  5 // Slave select pin for the SPI connection
#define SCK_PIN 18
#define MISO_PIN 19
#define MOSI_PIN 23

#include <SPI.h>
#include <MFRC522.h>
#include "rtc.h"
#include "alarms.h"

// Declare the display object and functions
extern MFRC522 mfrc522;

void rfidSetup();
void rfidLoop();
void activateRFIDReader();
void checkRFID();
void powerDownRFID();
void powerUpRFID();

#endif