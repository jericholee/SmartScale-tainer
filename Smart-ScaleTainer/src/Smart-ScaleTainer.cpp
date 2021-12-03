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
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"


//******************DECLARATIONS************************************************
void setup();
void loop();
void startOLED();
void MQTT_connect();
#line 20 "c:/Users/jeric/Desktop/IoT/SmartScale-tainer/Smart-ScaleTainer/src/Smart-ScaleTainer.ino"
const int CAL_FACTOR = 1719;
const int CAL_FACTOR2 = 1748;
const int SAMPLES = 10;
const int OLED_RESET = D4;

unsigned long last, lastTime;
float weight, weight2, rawData, calibration;
int offset;

HX711 scale(A4,A3);
HX711 scale2(A2,A1);
Adafruit_SSD1306 display(OLED_RESET);
TCPClient TheClient;
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER ,AIO_SERVERPORT, AIO_USERNAME,AIO_KEY);

Adafruit_MQTT_Publish WeightObj = Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/WeightOneRead");
Adafruit_MQTT_Publish Weight2Obj = Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/WeightTwoRead");

// SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
  Serial.begin(9600);
    waitFor(Serial.isConnected, 15000);
  WiFi.connect();
  while(WiFi.connecting()) {
  Serial.printf("."); 
  scale.set_scale();
  scale2.set_scale();
  delay(5000);
  scale.tare();  //Reset the scale to 0
  scale2.tare();
  scale.set_scale(CAL_FACTOR);
  scale2.set_scale(CAL_FACTOR2);
  startOLED();
 }
}

void loop() {
  if ((millis()-last)>120000) {
      Serial.printf("Pinging MQTT \n");
      if(! mqtt.ping()) {
        Serial.printf("Disconnecting \n");
        mqtt.disconnect();
      }
      last = millis();
  }

  if((millis()-lastTime > 6000)) {
    if(mqtt.Update()) {
      Weight2Obj.publish(weight2);
      Serial.printf("Publishing %0.2f \n",weight2); 
      } 
    lastTime = millis();

    if((millis()-lastTime > 6000)) {
    if(mqtt.Update()) {
      WeightObj.publish(weight);
      Serial.printf("Publishing %0.2f \n",weight); 
      } 
    lastTime = millis();
  }

  weight = scale.get_units(SAMPLES);
  weight2 = scale2.get_units(SAMPLES);
  Serial.println(weight);
  Serial.println(weight2);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(weight);
  display.println(weight2);
  display.display();
    }
  }


  void startOLED() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  display.drawPixel(10, 10, WHITE);
  display.display();
  display.clearDisplay();
  }

  // Function to connect and reconnect as necessary to the MQTT server.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.printf("%s\n",(char *)mqtt.connectErrorString(ret));
       Serial.printf("Retrying MQTT connection in 5 seconds..\n");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  Serial.printf("MQTT Connected!\n");
}