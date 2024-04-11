#include <WiFi.h>
#include <rtc.cpp>

void setup() {
  Serial.begin(115200);

  // Disable WiFi
  WiFi.mode(WIFI_OFF);

  // Disable Bluetooth
  btStop();

  Serial.println("WiFi and Bluetooth are disabled");
}

void loop() {
  // Your code here

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
          pillbox = true
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

bool checkAlarm (DateTime now, DateTime alarm){
  if(now.hour == alarm.hour && now.minut == alarm.minute){
    return true;
  }
  else{
    return false;
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