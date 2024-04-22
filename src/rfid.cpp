#include "rfid.h"

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create an instance of the MFRC522 class

// Known tags and cards
const byte tag1[4] = {0x73, 0xB2, 0x65, 0xFA}; 
const byte knownCard1[4] = {0xD3, 0x8C, 0x65, 0x1A};
//const byte knownCard1[4] = {0xD3, 0x8C, 0x65, 0x1A}; //Enter new ones in this format

void rfidSetup() {
  Serial.begin(115200); // Initialize serial communications with the PC
  while (!Serial);      // Do nothing if no serial port is opened
  
  pinMode(RST_PIN, OUTPUT); // Set the reset pin as an output
  digitalWrite(RST_PIN, HIGH); // Start with the RFID module active

  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522 card

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

void activateRFIDReader() {
    if (RST_PIN == LOW){
        powerUpRFID;
    }

    Serial.println("RFID reader activated");
}

void checkRFID() {
    if (alarmIsActive) {
        // RFID checking logic here
        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
            // Check if the detected card matches one of the known UIDs
            if (memcmp(mfrc522.uid.uidByte, tag1, 4) == 0) {
                Serial.println("Hello, Tag 1!");
                dismissAlarm();
            } 
            else if (memcmp(mfrc522.uid.uidByte, knownCard1, 4) == 0) {
                Serial.println("Hello, Card 1!");
                dismissAlarm();
            } 
            else {
                Serial.println("Unknown card detected.");
            }
        }
    }
}

void powerDownRFID() {
    digitalWrite(RST_PIN, LOW); // Set reset pin LOW to power down the RFID reader
    Serial.println("RFID reader powered down.");
}

void powerUpRFID() {
    digitalWrite(RST_PIN, HIGH); // Set reset pin HIGH to power up the RFID reader
    Serial.println("RFID reader powered up.");
}