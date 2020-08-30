// Library For Temperature and Humidity Sensor
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <Wire.h>
String apiKey = "5F2HRL1BOCJA9L83"; // Enter your Write API key from
ThingSpeak
const char *ssid = "Gamechanger"; // replace with your wifi ssid and wpa2 key
const char *pass = "computer";
const char* server = "api.thingspeak.com";
// Variable declarations for sensors pins
const int soil_sensor =A0;
WiFiClient client;
void setup() {
 // put your setup code here, to run once:
 Serial.begin(115200);
 pinMode(soil_sensor, INPUT);
 }
}
void loop() {
 // put your main code here, to run repeatedly:
 int moisture_percentage;
 int sensor_analog =0;
 sensor_analog = analogRead(soil_sensor);
 moisture_percentage = ( 100-(sensor_analog/1023.00) * 100 );

 upload_data(moisture_percentage);
 delay(1000);
}
void upload_data(int moisture_percentage){
 if (client.connect(server,80)) // "184.106.153.149" or api.thingspeak.com
 {

 String postStr = apiKey;
 postStr +="&field3=";
 postStr += String(moisture_percentage);
 postStr += "\r\n\r\n";
 client.print("POST /update HTTP/1.1\n");
 client.print("Host: api.thingspeak.com\n");
 client.print("Connection: close\n");
 client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
 client.print("Content-Type: application/x-www-form-urlencoded\n");
 client.print("Content-Length: ");
 client.print(postStr.length());
 client.print("\n\n");
 client.print(postStr);
 }
 client.stop();
 }
