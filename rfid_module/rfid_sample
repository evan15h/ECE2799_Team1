#include <SPI.h>
#include <MFRC522.h>

// Define the pins used for the SPI connection of the RFID reader
#define RST_PIN 2  // Reset pin for the RFID reader
#define SS_PIN  15 // Slave select pin for the SPI connection

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create an instance of the MFRC522 class

void setup() {
  Serial.begin(115200); // Initialize serial communications with the PC
  while (!Serial);      // Do nothing if no serial port is opened

  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522 card

  Serial.println(F("Scan PICC to see UID..."));
}

void loop() {
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
    const byte knownCard1[4] = {0xAA, 0xBB, 0xCC, 0xDD}; // Replace these with your card's UID
    const byte knownCard2[4] = {0x11, 0x22, 0x33, 0x44}; // Replace these with your card's UID

    // Check if the detected card matches one of the known UIDs
    if (memcmp(mfrc522.uid.uidByte, knownCard1, 4) == 0) {
        Serial.println("Hello, Card 1!");
    } else if (memcmp(mfrc522.uid.uidByte, knownCard2, 4) == 0) {
        Serial.println("Hello, Card 2!");
    } else {
        Serial.println("Unknown card detected.");
    }

    delay(1000); // Wait a bit before trying again
}
