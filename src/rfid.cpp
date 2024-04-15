#include "rfid.h"

// Define the pins used for the SPI connection of the RFID reader
#define RST_PIN 2  // Reset pin for the RFID reader
#define SS_PIN  5 // Slave select pin for the SPI connection
#define SCK_PIN 18
#define MISO_PIN 19
#define MOSI_PIN 23

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create an instance of the MFRC522 class

void rfidSetup() {
  Serial.begin(115200); // Initialize serial communications with the PC
  while (!Serial);      // Do nothing if no serial port is opened

  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522 card

  Serial.println(F("Scan PICC to see UID..."));
}

void rfidLoop() {
    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    // Dump debug info about the card; PICC_HaltA() is automatically called
    Serial.print(F("Card UID:"));
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    // Example UIDs of known cards
    const byte tag1[4] = {0x73, 0xB2, 0x65, 0xFA}; // Replace these with your card's UID
    const byte knownCard1[4] = {0xD3, 0x8C, 0x65, 0x1A}; // Replace these with your card's UID

    // Check if the detected card matches one of the known UIDs
    if (memcmp(mfrc522.uid.uidByte, tag1, 4) == 0) {
        Serial.println("Hello, Tag 1!");
    } else if (memcmp(mfrc522.uid.uidByte, knownCard1, 4) == 0) {
        Serial.println("Hello, Card 1!");
    } else {
        Serial.println("Unknown card detected.");
    }

    delay(1000); // Wait a bit before trying again
}
