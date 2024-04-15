// rfid.h
#ifndef RFID_H
#define RFID_H

#include <SPI.h>
#include <MFRC522.h>

// Declare the display object and functions
extern MFRC522 mfrc522;

void rfidSetup();
void rfidLoop();

#endif
