#include "alarms.h"

bool alarmIsActive = false;  // Define and initialize the global flag
Alarm alarms[MAX_ALARMS];
int alarmCount = 0;  // Current number of alarms stored
int selectedAlarmIndex = 0;  // Index of the currently selected alarm
bool midnightPassed = false;  // Global variable to keep track of execution past midnight



void EEPROMSetup() {
    EEPROM.begin(EEPROM_SIZE);
    uint8_t initFlag;
    EEPROM.get(EEPROM_INIT_CHECK, initFlag); // Read initilization flag

    Serial.print("Read Signature: "); 
    Serial.println(initFlag, HEX);
    if (initFlag != EEPROM_SIGNATURE) {  // Check for a specific value to see if EEPROM is initialized
        // EEPROM is not initialized, so perform initial setup
        Serial.println("Initializing EEPROM...");
        initFlag = 0xA5;  // Set the initialization flag
        EEPROM.put(EEPROM_INIT_CHECK, initFlag);
        
        alarmCount = 0;
        EEPROM.put(ALARM_COUNT_ADDRESS, alarmCount);

        // Optionally initialize each alarm slot to a known state (not strictly necessary)
        Alarm emptyAlarm = {};  // Default state for alarms
        for (int i = 0; i < MAX_ALARMS; i++) {
            EEPROM.put(ALARMS_START_ADDRESS + i * sizeof(Alarm), emptyAlarm);
        }

        EEPROM.commit();  // Ensure all changes are written to EEPROM
    } else {
        Serial.println("EEPROM already initialized.");
    }

    EEPROM.end();  
}

void storeAlarmsToMemory() {
    Serial.println("Storing alarms to memory...");
    EEPROM.begin(EEPROM_SIZE);
    // Store number of alarms
    EEPROM.put(ALARM_COUNT_ADDRESS, alarmCount);
    int address = ALARMS_START_ADDRESS;
    // Store each alarm
    for (int i = 0; i < alarmCount; i++) {
        EEPROM.put(address, alarms[i]);
        address += sizeof(Alarm);
    }
    EEPROM.commit();  // Ensure data is written to EEPROM
    EEPROM.end();
    Serial.println("Alarms stored.");
}

void loadAlarmsFromMemory() {
    EEPROM.begin(EEPROM_SIZE);
    // Read the number of alarms
    EEPROM.get(ALARM_COUNT_ADDRESS, alarmCount);
    int address = ALARMS_START_ADDRESS;
    // Read each alarm
    for (int i = 0; i < alarmCount; i++) {
        EEPROM.get(address, alarms[i]);
        address += sizeof(Alarm);
    }
    EEPROM.end();
    Serial.println("Alarms loaded");
}

void storeAlarm(uint8_t hour, uint8_t minute, bool repeatsDaily) {
  if (alarmCount < MAX_ALARMS) {
    alarms[alarmCount] = {hour, minute, true, repeatsDaily};  // Store new alarm and increment count
    alarmCount++;
    storeAlarmsToMemory(); // Update EEPROM after adding alarm
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
                lastUserActivityTime = millis(); // Update activity time
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
    lastDismissTime = now; // Store the dismissal time
    bool needsSaving = false; // Flag to check if EEPROM needs updating

    for (int i = 0; i < alarmCount; i++) {
        // Check if the alarm matches the current time and is enabled
        if (alarms[i].enabled && alarms[i].hour == now.hour() && alarms[i].minute == now.minute()){
            if (!alarms[i].repeatsDaily) {
                // Shift all subsequent alarms forward to fill the gap
                for (int j = i; j < alarmCount - 1; j++){
                    alarms[j] = alarms[j + 1];
                }
                alarmCount--; // Decrement the count of active alarms
                i--; // Adjust index to continue checking correctly
                needsSaving = true; // Mark for saving since alarms array was modified
            }
            else{
                alarms[i].enabled = false; // Temporarily disable daily alarms that go off
                needsSaving = true;
            }
            break; // Stop after handling the relevant alarm
        }
    }
    if (needsSaving){
        storeAlarmsToMemory(); // Save changes to EEPROM if needed
        Serial.println("Alarms stored after dismissal.");
    }

    alarmIsActive = false; // Reset the alarm active flag
}

void removeSelectedAlarm() {
    // Logic to remove the selected alarm from your alarms array or list
    for (int i = selectedAlarmIndex; i < alarmCount - 1; i++){
        alarms[i] = alarms[i + 1];  // Shift alarms down in the array
    }
    alarms[alarmCount - 1] = Alarm();
    alarmCount--;  // Decrease the count of alarms
    storeAlarmsToMemory();
    Serial.println("Alarms stored after removal.");
}

void dailyAlarmReset() {
    DateTime now = rtc.now();
    if (now.hour() == 0 && now.minute() == 0 && !midnightPassed){
        reenableAlarms();
    } 
    else if (now.hour() != 0){ // Once past any minute of the 00 hour
        midnightPassed = false; // Reset flag after midnight has truly passed
    }
}

void reenableAlarms() {
    for (int i = 0; i < alarmCount; i++){
        if (alarms[i].repeatsDaily && !alarms[i].enabled){
            alarms[i].enabled = true; // Re-enable alarms for the new day
        }
    }
    midnightPassed = true; // Prevent multiple executions
}

Alarm& currentlySelectedAlarm() {
    return alarms[selectedAlarmIndex];  // Return a reference to the currently selected alarm
}

void clearEEPROM() {
    EEPROM.begin(EEPROM_SIZE); // EEPROM_SIZE should be defined according to your device's EEPROM capacity
    for (int i = 0; i < EEPROM_SIZE; i++){
        EEPROM.write(i, 0xFF); // Write 0xFF to all EEPROM cells
    }
    EEPROM.commit(); // Ensure all changes are saved
    EEPROM.end(); // Clean up EEPROM object
    Serial.println("EEPROM Cleared");
}