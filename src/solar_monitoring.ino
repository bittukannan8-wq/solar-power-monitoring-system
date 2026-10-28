#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

const int buzzerPin = 8;

// Example alarm limits
const float LOW_VOLTAGE = 4.5;
const float LOW_CURRENT = 0.05;  // Amperes

Adafruit_INA219 ina219;
Adafruit_SSD1306 display(
  SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET
);

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  if (!ina219.begin()) {
    Serial.println("INA219 sensor not detected!");
    while (1) delay(10);
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not detected!");
    while (1) delay(10);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("SOLAR MONITOR");
  display.display();
}

void loop() {
  float voltage = ina219.getBusVoltage_V();
  float current = ina219.getCurrent_mA() / 1000.0;
  float power = voltage * current;

  // Alarm responds to low voltage OR low current
  bool alarm = (voltage < LOW_VOLTAGE) ||
               (current < LOW_CURRENT);

  digitalWrite(buzzerPin, alarm ? HIGH : LOW);

  // Display readings
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  display.println("SOLAR MONITOR");
  display.println("----------------");

  display.print("Voltage: ");
  display.print(voltage, 2);
  display.println(" V");

  display.print("Current: ");
  display.print(current, 3);
  display.println(" A");

  display.print("Power: ");
  display.print(power, 2);
  display.println(" W");

  if (alarm) {
    display.println("WARNING: LOW OUTPUT");
  } else {
    display.println("Status: NORMAL");
  }

  display.display();

  Serial.print("Voltage: ");
  Serial.print(voltage, 2);
  Serial.print(" V, Current: ");
  Serial.print(current, 3);
  Serial.print(" A, Power: ");
  Serial.print(power, 2);
  Serial.print(" W, Alarm: ");
  Serial.println(alarm ? "ON" : "OFF");

  delay(100); // Check readings every 100 ms
}
