#include <WiFi.h>
#include <rtc.cpp>
#include <display.cpp>
#include <driver.cpp>
#include <rfid.cpp>


//Global alarm variable
DateTime alarm1;
bool alerting = false;
bool pillboxScanned = false;
const byte tag1[4] = {0x73, 0xB2, 0x65, 0xFA};

void setup() {
  Serial.begin(115200);

  // Disable WiFi
  WiFi.mode(WIFI_OFF);

  // Disable Bluetooth
  btStop();

  Serial.println("WiFi and Bluetooth are disabled");

  //Setup devices
  buttonsSetup();
  displaySetup();
  driverSetup();
  rfidSetup();
  rtcSetup();

}

void loop() {
  // Your code here
  if (alerting || checkAlarm(rtc.now(), alarm1)){
    alerting = true;
    if (memcmp(mfrc522.uid.uidByte, tag1, 4) == 0) {
      pillboxScanned = true;
    }
    if (!pillboxScanned){
      drv.go();
      display.clearDisplay();
      display.println("Alarm 1");
      display.display();
    }
    else{
      drv.stop();
      display.clearDisplay();
      //ALSO NEED TO TURN OFF DISPLAY FOR POWER SAVING
    }
  }
}

bool checkAlarm (DateTime now, DateTime alarm){
  if(now.hour() == alarm.hour() && now.minute() == alarm.minute()){
    return true;
  }
  else{
    return false;
  }
}

/*
PSUEDOCODE

void loop(){
  for list of alarms{     (CAN PROBABLY GET AWAY WITH ONE ALARM FOR NOW)
    if(alerting = true || checkAlarm(now,alarm)){
      alerting = true
      drv.go();    (I THINK THIS IS THE CODE FOR SETTING DRIVER ON)
      display alarm on screen
      if(pillboxScanned == false){
        if(RFID sample found)
          pillboxScanned = true
      }
      if(pillboxScanned == true){
        drv.stop()
        stop display
        alerting = false
      }
    }
  }

  if (button1.ispressed()){
    setClock()
  }
  if (button2.ispressed()){
    setAlarm()
  }
}

void setClock(){    (NO PARAMETERS BECAUSE USE GLOBAL VARIABLES)
  DateTime newNow = now

  loop{
    display newNow
    if (button1.ispressed())
      move to next thing in newNow
    if (button2.ispressed())
      iterate up
  }
  rtc.adjust(newNow)
}

void setAlarm(){
  DateTime newAlarm
  
  loop{
    if (button1.ispressed())
      move to next thing in list or in new Alarm
    if (button2.ispressed())
      iterate up
  }

  alarmList[alarmNum] = newAlarm
}


*/