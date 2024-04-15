#include "display.h"
#include "buttons.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // No reset pin
#define SCREEN_ADDRESS 0x3C // 0x3C for 7-bit address, 0x78 for 8-bit address
// Define the I2C pins for ESP32
#define I2C_SDA 21
#define I2C_SCL 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS1307 rtc;

void displaySetup() {
  Serial.begin(115200);
  
  // Initialize I2C for ESP32
  Wire.begin(I2C_SDA, I2C_SCL);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();

    // Start the RTC module
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Check if the RTC is running
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // Add an estimated delay time, e.g., 35 seconds
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second() + 30));
  }

}

void displayTime() {
  DateTime now = rtc.now();
  
  display.clearDisplay();
  
  int hour = now.hour();
  String ampm = "AM";
  
  // Convert to 12-hour format
  if (hour == 0) {
    hour = 12;  // Midnight case
  } else if (hour == 12) {
    ampm = "PM";  // Noon case
  } else if (hour > 12) {
    hour -= 12;
    ampm = "PM";
  }

  // Display time in larger font
  String timeStr = String(hour) + ":" + (now.minute() < 10 ? "0" : "") + String(now.minute()) + ampm;
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  int timeX = centerText(timeStr, 3);
  display.setCursor(timeX, 20);  // Adjust vertical position here
  display.println(timeStr);

  // Display date in smaller font
  String dateStr = (now.month() < 10 ? "0" : "") + String(now.month()) + "/" + String(now.day());
  display.setTextSize(1);
  int dateX = centerText(dateStr, 1);
  int dateY = calculateYPosition(1, 20);  // Calculate new Y based on previous text
  display.setCursor(dateX, dateY);
  display.println(dateStr);

  display.display();
  delay(1000);
}

int centerText(String text, int textSize) {
  int16_t x1, y1;
  uint16_t w, h;
  display.setTextSize(textSize);
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  return (SCREEN_WIDTH - w) / 2;
}

int calculateYPosition(int newSize, int previousY) {
  int16_t x1, y1;
  uint16_t w, h;
  display.setTextSize(newSize);
  display.getTextBounds("Ay", 0, 0, &x1, &y1, &w, &h);  // Use a tall character to measure max height
  return previousY + h + 18;  // Add some padding
}

void refreshDisplayHour(int hour) {
  display.clearDisplay();
  display.setTextSize(3); // Larger text for better visibility
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20); // Adjust positioning as needed

  String hourStr = String(hour) + ":" + String(rtc.now().minute());
  display.println(hourStr);
  display.display(); // Actually send the buffer to the display
}
