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
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"


//******************DECLARATIONS************************************************
void setup();
void loop();
void startOLED();
void MQTT_connect();
#line 21 "c:/Users/jeric/Desktop/IoT/SmartScale-tainer/Smart-ScaleTainer/src/Smart-ScaleTainer.ino"
const int CAL_FACTOR = 1719;
const int CAL_FACTOR2 = 1748;
const int SAMPLES = 10;
const int OLED_RESET = D4;

unsigned long last, lastTime;
float weight, weight2, rawData, calibration;
int offset;

//*********************KEYPAD SETUP**********************************
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

// ******************CREATED KEYPAD OBJECT VARIABLE "KEYPAD"******************
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

HX711 scale(D3,D2);
HX711 scale2(D10,D9);
Adafruit_SSD1306 display(OLED_RESET);
Servo myServo;
TCPClient TheClient;
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER ,AIO_SERVERPORT, AIO_USERNAME,AIO_KEY);

Adafruit_MQTT_Publish WeightObj = Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/WeightOneRead");
Adafruit_MQTT_Publish Weight2Obj = Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/WeightTwoRead");


void setup() {
  Serial.begin(9600);
    waitFor(Serial.isConnected, 15000);
  WiFi.connect();
  while(WiFi.connecting()) {
  Serial.printf("."); 
  delay(2000);
  Serial.printf("Hello World\n");
  scale.set_scale();
  scale2.set_scale();
  delay(5000);
  scale.tare();  //Reset the scale to 0
  scale2.tare();
  scale.set_scale(CAL_FACTOR);
  scale2.set_scale(CAL_FACTOR2);
  startOLED();
  myServo.attach(5);
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
      Weight2Obj.publish(weight);
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
  char key = keypad.getKey();
    if (key){
      Serial.println(key);
    }
  delay(500);
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