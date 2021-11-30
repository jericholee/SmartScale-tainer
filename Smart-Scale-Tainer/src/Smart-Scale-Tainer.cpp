/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/jeric/Desktop/IoT/SmartScale-tainer/Smart-Scale-Tainer/src/Smart-Scale-Tainer.ino"
/*
 * Project Smart-Scale-Tainer
 * Description: Capstone Project Smart Scale for inventory needs
 * Author: Jericho Ortiz
 * Date: 11/29/2021
 */

//******************LIBRARIES************************************************
#include "credentials.h"
#include "HX711.h"

void setup();
void loop();
#line 12 "c:/Users/jeric/Desktop/IoT/SmartScale-tainer/Smart-Scale-Tainer/src/Smart-Scale-Tainer.ino"
HX711 scale(D3,D2);

//******************DECLARATIONS************************************************
const int CAL_FACTOR = 1727;
const int SAMPLES = 30;
float weight, rawData, calibration;
int offset;


SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
  Serial.begin(9600);
  scale.set_scale();
  delay(5000);
  scale.tare();  //Reset the scale to 0
  scale.set_scale(CAL_FACTOR);
}

void loop() {
  weight = scale.get_units(SAMPLES);
  // delay(1000);
  rawData = scale.get_value(SAMPLES);
  offset = scale.get_offset();
  calibration = scale.get_scale();
  Serial.println(scale.get_units(SAMPLES));
  // delay(500);
  }