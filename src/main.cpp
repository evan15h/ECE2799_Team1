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

//Declare Functions
bool checkAlarm (DateTime now, DateTime alarm);

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

  if (isButtonOneHeld()){
    setClock();
  }

  if (isButtonTwoHeld()){
    setAlarm();
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

void setClock(){    //(NO PARAMETERS BECAUSE USE GLOBAL VARIABLES)
  DateTime newNow = rtc.now();
  bool setting = true;
  int index = 0;  //0 = hour, 1 = minute, 2 = second

  while(setting){
    displayTime(newNow);
    if (isButtonOneHeld()){
      setting = false;
      display.clearDisplay();
      //ALSO NEED TO TURN IT OFF
    }
    else if (readButtonOne()){
      if(index < 2)
        index += 1;
      else
        index = 0;
    }
    if (readButtonTwo()){
      if (index = 0)
        newNow = newNow + TimeSpan(0,1,0,0);
      if (index = 1)
        newNow = newNow + TimeSpan(0,0,1,0);
      if (index = 2)
        newNow = newNow + TimeSpan(0,0,0,1);
    }
  }
  rtc.adjust(newNow);
}

void setAlarm(){
  DateTime newAlarm = alarm1;
  bool setting = true;
  int index = 0;  //0 = alarm, 1 = hour, 2 = minute
  int alarmNum = 1;

  while (setting){
    displayAlarm(newAlarm, alarmNum);
    if (isButtonOneHeld()){
      setting = false;
      display.clearDisplay();
      //ALSO NEED TO TURN IT OFF
    }
    else if (readButtonOne()){
      if(index < 2)
        index += 1;
      else
        index = 0;
    }
    if (readButtonTwo()){
      if (index = 0)
        alarmNum = 1; //Just working on one alarm for now.
      if (index = 1)
        newAlarm = newAlarm + TimeSpan(0,1,0,0);
      if (index = 2)
        newAlarm = newAlarm + TimeSpan(0,0,1,0);
    }
  }

  alarm1 = newAlarm;
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