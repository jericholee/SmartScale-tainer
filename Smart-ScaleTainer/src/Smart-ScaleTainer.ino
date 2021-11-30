/*
 * Project Smart-Scale-Tainer
 * Description: Capstone Project Smart Scale for inventory needs
 * Author: Jericho Ortiz
 * Date: 11/29/2021
 */

//******************LIBRARIES************************************************
#include "credentials.h"
#include "HX711.h"
#include "wire.h"
#include "AdaFruit_SSD1306.h"



//******************DECLARATIONS************************************************
const int CAL_FACTOR = 1727;
const int SAMPLES = 30;
const int OLED_RESET = D4;
float weight, rawData, calibration;
int offset;

HX711 scale(D3,D2);
Adafruit_SSD1306 display(OLED_RESET);

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
  Serial.begin(9600);
  scale.set_scale();
  delay(5000);
  scale.tare();  //Reset the scale to 0
  scale.set_scale(CAL_FACTOR);
  startOLED();
}

void loop() {
  weight = scale.get_units(SAMPLES);
  // delay(1000);
  // rawData = scale.get_value(SAMPLES);
  // offset = scale.get_offset();
  // calibration = scale.get_scale();
  Serial.println(weight);
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(weight);
  display.display();
  // delay(500);
  }

  void startOLED() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  display.drawPixel(10, 10, WHITE);
  display.display();
  display.clearDisplay();
  }