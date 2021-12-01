/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/jeric/Desktop/IoT/SmartScale-tainer/Smart-ScaleTainer/src/Smart-ScaleTainer.ino"
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
#include "math.h"
#include "Keypad_Particle.h"




//******************DECLARATIONS************************************************
void setup();
void loop();
void startOLED();
#line 20 "c:/Users/jeric/Desktop/IoT/SmartScale-tainer/Smart-ScaleTainer/src/Smart-ScaleTainer.ino"
const int CAL_FACTOR = 1727;
const int SAMPLES = 10;
const int OLED_RESET = D4;

float weight, rawData, calibration;
int offset;

// set up keypad buttons
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};
byte rowPins[ROWS] = { A3, A2, A1, D4 };
byte colPins[COLS] = { D5, D6, D7, D8 };

// create Keypad object variable called "keypad"
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

HX711 scale(D3,D2);
Adafruit_SSD1306 display(OLED_RESET);
Servo myServo;

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
  Serial.begin(9600);
  scale.set_scale();
  delay(5000);
  scale.tare();  //Reset the scale to 0
  scale.set_scale(CAL_FACTOR);
  startOLED();
  myServo.attach(5);
}

void loop() {
  weight = scale.get_units(SAMPLES);
  Serial.println(weight);
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(weight);
  display.display();
  char key = keypad.getKey();
    if (key){
      Serial.println(key);
    }
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