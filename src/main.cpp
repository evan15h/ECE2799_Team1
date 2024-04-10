#include <WiFi.h>

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

