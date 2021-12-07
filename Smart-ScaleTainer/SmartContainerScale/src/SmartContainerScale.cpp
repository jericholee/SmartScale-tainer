/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/jeric/Desktop/IoT/SmartScale-tainer/Smart-ScaleTainer/SmartContainerScale/src/SmartContainerScale.ino"
/*
 * Project SmartContainerScale
 * Description: Capstone project. Serves as an idead to make more secure inventory processes for any industry that uses material that is stored by weight. Whether its a lab that stores chemicals, the food idustry or the Cannabis industry alike. Security, efficiency, and transparency can all be afforded with this product.  
 * Author: Jericho Ortiz
 * Date: NOV-29-2021
 */

//******************LIBRARIES************************************************

#include "credentials.h"
#include "HX711.h"
#include "AdaFruit_SSD1306.h"
#include "wire.h"
#include "math.h"
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"


//******************DECLARATIONS************************************************
void setup();
void loop();
void startOLED();
void startOLED2();
void testdrawcircle(void);
void testdrawcircle2(void);
#line 21 "c:/Users/jeric/Desktop/IoT/SmartScale-tainer/Smart-ScaleTainer/SmartContainerScale/src/SmartContainerScale.ino"
const int CAL_FACTOR = 1719;
const int CAL_FACTOR2 = 1748;
const int SAMPLES = 10;
const int OLED_RESET = D4;
const int hexAddress = 0x76;
const int hallpin = D5;
const int button = A0;
int hallVal = 0;
int offset;
int buttonVal = 0;
int buttonState;

bool buttonClick();
bool containerStatus();

unsigned long last, lastTime;
float weight, weight2, rawData, calibration;


HX711 scale(A4,A3);
HX711 scale2(A2,A1);
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_SSD1306 display2(-1);
TCPClient TheClient;

//******************ADAFRUIT.IO SETUP*****************************************

Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER ,AIO_SERVERPORT, AIO_USERNAME,AIO_KEY);

//******************ADAFRUIT FEEDS********************************************

Adafruit_MQTT_Publish WeightObj = Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/ScaleOneRead");
Adafruit_MQTT_Publish Weight2Obj = Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/ScaleTwoRead");
Adafruit_MQTT_Publish containterStatObj = Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/containerStatus");


void setup() {
    Serial.begin(9600);
    waitFor(Serial.isConnected, 15000); //Wait for Serial Monitor

    WiFi.connect(); //Connect to internet, but not Particle Cloud
    while(WiFi.connecting()) {
        Serial.printf(".");
    }
    startOLED();
    startOLED2();
    scale.set_scale();
    scale2.set_scale();
    delay(5000);
    scale.tare();  //Reset the scale to 0
    scale2.tare();
    scale.set_scale(CAL_FACTOR);
    scale2.set_scale(CAL_FACTOR2);
    pinMode(hallpin,INPUT);
    pinMode(button, INPUT);
    attachInterrupt(button, buttonClick, FALLING);
    attachInterrupt(hallpin,containerStatus,RISING);


}
  

void loop() {
    if((millis()-lastTime > 15000)) {
    //Publishing to a MQTT Feed. 
    if(mqtt.Update()) {  //if mqtt object (Adafruit.io) is available to receive data.
       Serial.printf("Publishing %0.2f to ScaleOneReading\n",weight);
       WeightObj.publish(weight);
       Serial.printf("Publishing %0.2f to ScaleTwoReading\n",weight2);
       Weight2Obj.publish(weight2);
       Serial.printf("Publishing %i to contStatus\n",containerStatus());
       containterStatObj.publish(containerStatus());
    }
        lastTime = millis();
    }

    weight = scale.get_units(SAMPLES);
    weight2 = scale2.get_units(SAMPLES);
    Serial.println(weight);
    Serial.println(weight2);
    display.clearDisplay();
    display2.clearDisplay();
    display.setTextSize(2);
    display2.setTextSize(2);
    display.setTextColor(WHITE);
    display2.setTextColor(WHITE);
    display.setCursor(0,0);
    display2.setCursor(0,0);
    display.println(weight);
    display2.setCursor(0,32);
    display2.println(weight2);
    display.display();
    display2.display();
    containerStatus();

}

void startOLED() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();
  testdrawcircle();// draw mulitple circles
  display.display();
  display.clearDisplay();
  }


void startOLED2() {
  display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display2.display();
  delay(2000);
  display2.clearDisplay();
  testdrawcircle2();// draw mulitple circles
  display2.display();
  display2.clearDisplay();
  }


void testdrawcircle(void) {
  for (int16_t i=0; i<display.height(); i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
  }
}

void testdrawcircle2(void) {
  for (int16_t i=0; i<display2.height(); i+=2) {
    display2.drawCircle(display2.width()/2, display2.height()/2, i, WHITE);
    display2.display();
  }
}

bool containerStatus() {
  hallVal = digitalRead(hallpin);
  if(hallVal == HIGH) {
    Serial.printf("Container is open\n");
  }
    if(hallVal != HIGH) {
           Serial.printf("Container is closed\n");
    }
return hallVal;
}

bool buttonClick() {
  buttonVal = digitalRead(button);
  if(buttonVal) {
    Serial.printf("button is pressed \n");
  }
  else {
    Serial.printf("button is not pressed \n");
    delay(250);
  }
}


