#include "alarms.h"

bool alarmIsActive = false;  // Define and initialize the global flag
Alarm alarms[MAX_ALARMS];
int alarmCount = 0;  // Current number of alarms stored
int selectedAlarmIndex = 0;  // Index of the currently selected alarm
bool midnightPassed = false;  // Global variable to keep track of execution past midnight



void EEPROMSetup() {
    EEPROM.begin(EEPROM_SIZE);
    uint16_t signature = 0;
    EEPROM.get(EEPROM_SIGNATURE_ADDR, signature);
    Serial.print("Read Signature: "); Serial.println(signature, HEX);
    if (signature != EEPROM_SIGNATURE) {
        // EEPROM not initialized
        Serial.println("No valid signature found. Initializing EEPROM...");
        signature = EEPROM_SIGNATURE;
        EEPROM.put(EEPROM_SIGNATURE_ADDR, signature);
        alarmCount = 0;  // Reset alarm count
        EEPROM.put(EEPROM_ALARM_COUNT_ADDR, alarmCount);  // Start storing alarm count after 2 bytes used by signature
        EEPROM.commit();
        Serial.println("EEPROM initialized with new signature and alarm count.");
    } 
    else {
        // Load alarms
        Serial.println("Signature found. Loading alarms...");
        EEPROM.get(EEPROM_ALARM_COUNT_ADDR, alarmCount);
        int addr = EEPROM_ALARM_COUNT_ADDR + sizeof(alarmCount); // Starting address for alarms

        for (int i = 0; i < alarmCount; i++) {
            EEPROM.get(addr, alarms[i]);
            addr += sizeof(Alarm);
            Serial.print("Alarm "); Serial.print(i + 1); Serial.print(": ");
            Serial.print(alarms[i].hour); Serial.print(":"); Serial.println(alarms[i].minute);
        }
    }
}

void storeAlarmsToMemory() {
    int addr = 0;
    EEPROM.write(addr, alarmCount);
    addr += sizeof(alarmCount);
    for (int i = 0; i < alarmCount; i++) {
        EEPROM.put(addr, alarms[i]);
        addr += sizeof(alarms[i]);
    }
    EEPROM.commit();  // Make sure to commit changes to EEPROM
}

void loadAlarmsFromMemory() {
    int addr = 0; // Start reading from the beginning
    EEPROM.get(addr, alarmCount); // Load the alarm count
    addr += sizeof(alarmCount);  // Move the address pointer

    for (int i = 0; i < alarmCount; i++) {
        EEPROM.get(addr, alarms[i]);
        addr += sizeof(Alarm);
    }
}

void storeAlarm(int hour, int minute, bool repeatsDaily) {
  if (alarmCount < MAX_ALARMS) {
    alarms[alarmCount++] = {hour, minute, true, repeatsDaily};  // Store new alarm and increment count
    //storeAlarmsToMemory(); // Update EEPROM after adding alarm
  } 
  else {
    Serial.println("Alarm list full!");
  }
}

// void displayAlarms() {
//     Serial.print("Loading alarms, count: ");
//     Serial.println(alarmCount);
//     for (int i = 0; i < min(alarmCount, MAX_ALARMS); i++) {  // Ensure we do not exceed bounds
//         if (alarms[i].enabled) {
//             Serial.print("Alarm ");
//             Serial.print(i + 1);
//             Serial.print(": ");
//             Serial.print(alarms[i].hour);
//             Serial.print(":");
//             Serial.println(alarms[i].minute);
//         }
//     }
// }

void checkAndTriggerAlarms() {
    DateTime now = rtc.now(); // Assuming you have a function to get the current time
    bool foundActiveAlarm = false;  // To check if at least one alarm is active

    for (int i = 0; i < alarmCount; i++) {
        if (alarms[i].enabled && alarms[i].hour == now.hour() && alarms[i].minute == now.minute()) {
            if (!lastDismissTime.isValid() || (lastDismissTime.minute() != now.minute()) || (lastDismissTime.hour() != now.hour())) {
                activateVibrationMotor();
                activateRFIDReader();
                foundActiveAlarm = true;
                break; // Break after the first match to avoid multiple triggers per minute
            }
        }
    }
    if (!foundActiveAlarm && alarmIsActive) {
        powerDownRFID();  // No active alarms and one was previously active, power down
        alarmIsActive = foundActiveAlarm;
    } 
    else if (foundActiveAlarm) {
        alarmIsActive = foundActiveAlarm;
    }
}

void dismissAlarm() {
    DateTime now = rtc.now();
    stopVibrationMotor();
    powerDownRFID();
    lastDismissTime = rtc.now(); // Store the dismissal time
    for (int i = 0; i < alarmCount; i++) {
        // Check if the alarm matches the current time and is enabled
        if (alarms[i].enabled && alarms[i].hour && alarms[i].minute == now.hour() && alarms[i].minute == now.minute()) {
            if (!alarms[i].repeatsDaily) {
                alarms[i].enabled = false; // Disable non-repeating alarms that go off
            }
            // Optionally reset the alarm here if it should repeat daily
            break; // Stop after handling the relevant alarm
        }
    }
    alarmIsActive = false; // Reset the alarm active flag
    Serial.println("Alarm dismissed");
    // Code to stop the vibration motor
    // Optionally reset the alarm or mark it as not enabled if it should not repeat
}

// int countSetAlarms() {
//     int count = 0;
//     for (int i = 0; i < alarmCount; i++) {
//         if (alarms[i].enabled) {
//             count++;
//         }
//     }
//     return count;
// }

void dailyAlarmReset() {
    DateTime now = rtc.now();
    if (now.hour() == 0 && now.minute() == 0 && !midnightPassed) {
        for (int i = 0; i < alarmCount; i++) {
            if (alarms[i].repeatsDaily && !alarms[i].enabled) {
                alarms[i].enabled = true;  // Re-enable alarms for the new day
            }
        }
        midnightPassed = true;  // Prevent multiple executions
    } 
    else if (now.minute() != 0) {
        midnightPassed = false;  // Reset flag after midnight has passed
    }
}

Alarm& currentlySelectedAlarm() {
    return alarms[selectedAlarmIndex];  // Return a reference to the currently selected alarm
}

void clearEEPROM() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  // Clear the EEPROM by setting each byte to zero
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, 0);
  }

  EEPROM.commit();  // Ensure changes are written to EEPROM
  Serial.println("EEPROM has been cleared.");

  EEPROM.end();  // Close EEPROM to release resources
}