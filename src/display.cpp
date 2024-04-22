#include "display.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // No reset pin
#define SCREEN_ADDRESS 0x3C // 0x3C for 7-bit address, 0x78 for 8-bit address
// Define the I2C pins for ESP32
#define I2C_SDA 21
#define I2C_SCL 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Example bitmap of a bell icon (8x8 pixels)
static const unsigned char PROGMEM bell_bitmap[] =
{ 
    0b00111100,
    0b01111110,
    0b01111110,
    0b01111110,
    0b11111111,
    0b11111111,
    0b00111100,
    0b00011000  
};

static const unsigned char PROGMEM arrow_bitmap[] =
{ 
    0b00011000,
    0b00011000,
    0b00111100,
    0b00111100,
    0b01111110,
    0b01111110,
    0b11111111,
    0b11111111  
};

void displaySetup() {
  Serial.begin(115200);
  
  // Initialize I2C for ESP32
  Wire.begin(I2C_SDA, I2C_SCL);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

}

void displayTime() {
  DateTime now = rtc.now();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);


  // Display icons for all enabled alarms
  int iconPosition = 0; // Start position for icons
  for (int i = 0; i < alarmCount; i++) {
      if (alarms[i].enabled) {
          display.drawBitmap(0 + iconPosition * 12, 0, bell_bitmap, 8, 8, SSD1306_WHITE);
          iconPosition++;
      }
  }

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

std::pair<int, int> centerArrow(String text, int textSize) {
    int16_t x1, y1;
    uint16_t w, h;
    display.setTextSize(textSize);
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    int centeredX = (SCREEN_WIDTH - w) / 2;
    return {centeredX, w};  // Return both the centered position and width
}

int calculateYPosition(int newSize, int previousY) {
  int16_t x1, y1;
  uint16_t w, h;
  display.setTextSize(newSize);
  display.getTextBounds("Ay", 0, 0, &x1, &y1, &w, &h);  // Use a tall character to measure max height
  return previousY + h + 18;  // Add some padding
}

uint16_t strWidth(const String& str, int textSize) {
    int16_t x1, y1;
    uint16_t w, h;
    display.setTextSize(textSize);
    display.getTextBounds(str, 0, 0, &x1, &y1, &w, &h);
    return w;
}

void displaySetHour() {
  DateTime now = rtc.now();
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);

  int displayHour = setHour;
  String ampm = "AM";

  // Convert to 12-hour format for display
  if (displayHour == 0) {
      displayHour = 12;  // Midnight case
  } else if (displayHour == 12) {
      ampm = "PM";  // Noon case
  } else if (displayHour > 12) {
      displayHour -= 12;
      ampm = "PM";
  }

  String timeStr = String(displayHour) + ":" + (now.minute() < 10 ? "0" : "") + String(now.minute()) + ampm;
  int hourX = centerText(timeStr, 3);
  display.setCursor(hourX, 20);
  display.println(timeStr);

  // Position Text
  String confirm = "SET HOUR";
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  int qX = centerText(confirm, 1);
  display.setCursor(qX, 5);  // Adjust vertical position here
  display.println(confirm);

  // Position arrow
  display.drawBitmap(14, 50, arrow_bitmap, 8, 8, SSD1306_WHITE);
  
  display.display();
}

void displaySetMinute() {
  DateTime now = rtc.now();
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);

  int displayHour = setHour;
  String ampm = "AM";

  // Convert to 12-hour format for display
  if (displayHour == 0) {
      displayHour = 12;  // Midnight case
  } else if (displayHour == 12) {
      ampm = "PM";  // Noon case
  } else if (displayHour > 12) {
      displayHour -= 12;
      ampm = "PM";
  }

  String timeStr = String(displayHour) + ":" + (setMinute < 10 ? "0" : "") + String(setMinute) + ampm;
  int minuteX = centerText(timeStr, 3);
  display.setCursor(minuteX, 20);
  display.println(timeStr);

  // Position Text
  String confirm = "SET MINUTE";
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  int qX = centerText(confirm, 1);
  display.setCursor(qX, 5);  // Adjust vertical position here
  display.println(confirm);

  // Position arrow
  display.drawBitmap(60, 50, arrow_bitmap, 8, 8, SSD1306_WHITE);

  display.display();
}

void displayConfirmRepeat() {
    // Position Question
    String question = "Repeat?";
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    int qX = centerText(question, 2);
    display.setCursor(qX, 20);  // Adjust vertical position here
    display.println(question);

    // Position Answer
    String answer = "YES     NO";
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    int aX = centerText(answer, 1);
    int aY = calculateYPosition(1, 20);
    display.setCursor(aX, aY);
    display.println(answer);

    display.display();
}

void displayRemoveAlarm() {
    display.clearDisplay();
    Alarm currentAlarm = currentlySelectedAlarm();

    String text = String("REMOVE ALARM ") + String(selectedAlarmIndex+1) + ":";
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    int qX = centerText(text, 1);
    display.setCursor(qX, 5);  // Adjust vertical position here
    display.println(text);

    // Converting 24-hour time to 12-hour format for display
    int displayHour = currentAlarm.hour;
    String ampm = "AM";
    if (displayHour == 0) {
        displayHour = 12;  // Midnight case
    } 
    else if (displayHour == 12) {
        ampm = "PM";  // Noon case
    } 
    else if (displayHour > 12) {
        displayHour -= 12;
        ampm = "PM";
    }

    // Ensuring minutes are always two digits
    String minuteStr = currentAlarm.minute < 10 ? "0" + String(currentAlarm.minute) : String(currentAlarm.minute);
    String alarms = String(displayHour) + ":" + minuteStr + ampm;
    display.setTextSize(3);
    int aX = centerText(alarms, 3);
    display.setCursor(aX, 20);
    display.println(alarms);

    // Show whether the alarm repeats
    String repeatStatus = currentAlarm.repeatsDaily ? "REPEATS DAILY" : "DOES NOT REPEAT";
    display.setTextSize(1);
    int rX = centerText(repeatStatus, 1);
    int rY = calculateYPosition(1, 20); // Calculate new Y based on previous text
    display.setCursor(rX, rY);
    display.println(repeatStatus);

    display.display();
}

void cycleThroughAlarms() {
    // Logic to cycle through the list of alarms
    // This could be implementing a circular list of alarm indices
    selectedAlarmIndex = (selectedAlarmIndex + 1) % alarmCount;
    displayRemoveAlarm();
}

void displayRemoveSelectedAlarm() {
    alarms[alarmCount - 1] = Alarm();
    alarmCount--;  // Decrease the count of alarms
    display.clearDisplay();
    String alarm = String("ALARM ") + String(selectedAlarmIndex+1);
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    int qX = centerText(alarm, 2);
    display.setCursor(qX, 17);  // Adjust vertical position here
    display.println(alarm);

    String removed = "REMOVED";
    int aX = centerText(removed, 2);
    display.setCursor(aX, 37);
    display.println(removed);

    display.display();
    delay(2000);  // Show confirmation for a short time
}

void displayNoAlarms() {
  display.clearDisplay();
  String none = "NO ALARMS";
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  int qX = centerText(none, 2);
  display.setCursor(qX, 17);  // Adjust vertical position here
  display.println(none);

  String none1 = "TO REMOVE";
  int wX = centerText(none1, 2);
  display.setCursor(wX, 37);  // Adjust vertical position here
  display.println(none1);
  display.display();
  delay(2000); // Display message for 2 seconds
}