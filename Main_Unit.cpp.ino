#define BLYNK_PRINT Serial   
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#define BLYNK_PRINT Serial
#include "DHT.h"
String apiKey = "5F2HRL1BOCJA9L83"; // Enter your Write API key from ThingSpeak
unsigned long counterChannelNumber = 1234; // channel number for reading the data
const char *ssid = "Gamechanger"; // replace with your wifi ssid and wpa2 key
const char *pass = "computer";
const char* server = "api.thingspeak.com";
int total_numberOfSubunits = 7;
char auth[] ="your auth token"; 
char ssid[] = "your wifi name"; 
char pass[] = "your wifi password";//App Connectivity 

#define DHTTYPE DHT11 // DHT 11
// Channel ID
const char * myCounterReadAPIKey = "8A2XBN1BOCJA9L76"; // Read API Key
const int FieldNumber1 = 1;
const int FieldNumber2 = 2;
// Variable declarations for sensors pins
int photon_analog = D0;
#define dht_dpin D4 // Sensor connected to pin 8 arduino
WiFiClient client;
#define DHTTYPE DHT11 // DHT 11
DHT dht(dht_dpin, DHTTYPE);
// Variable declaration for actuators
int buzzer = D8;
int motor = D6;


void setup() {
WiFi.mode(WIFI_STA);
ThingSpeak.begin(client);
dht.begin();
timer.setInterval(1000L, sendSensor);
Serial.begin(115200);
Blynk.begin(auth, ssid, pass);
pinMode(buzzer, OUTPUT);
pinMode(motor,OUTPUT);

}


void loop() {
// **********************SENSOR CODE*******************************
// Code for temperature - humidity sensor On Arduino
Blynk.run(); 
timer.run(); 
sendTemps();
int humidity = dht.readHumidity();
int temperature1 = dht.readTemperature();
delay(100);
int temperature = -1*temperature1;
// Code for soil moisture sensor
delay(100);
int moisture_percentage = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1,
myCounterReadAPIKey);
delay(100);
print_data();
delay(800);
sendSensor(humidity, moisture,soil_moisture);
//************************SENSOR CODE END ********************************
//************************ALGORITHM FOR ANALYSIS**************************** ***
// Analysis Algoirthm and Code for actuators
delay(4000);
if(moisture_percentage<=20){
if(temperature<25){
digitalWrite(buzzer,HIGH);
delay(1000);
digitalWrite(buzzer,LOW);
delay(1000);
while(moisture_percentage<30){
digitalWrite(motor,HIGH);
delay(100);
int temperature = -1*temperature1;
sensor_analog = analogRead(soil_sensor);
moisture_percentage = ( 100-(sensor_analog/1023.00) * 100 );
print_data(temperature,humidity,moisture_percentage);
// Printing data to cloud
send_data(temperature,humidity,moisture_percentage);
// end
delay(3000);
digitalWrite(motor,LOW); }
Serial.println("MOTOR IS TURNED OFF..!!!");
digitalWrite(buzzer,HIGH);
delay(1000);
digitalWrite(buzzer,LOW);
}
else if(temperature>=25 && temperature <35){
digitalWrite(buzzer,HIGH);
delay(1000);
digitalWrite(buzzer,LOW);
delay(1000);
while(moisture_percentage<35){
digitalWrite(motor,HIGH);
delay(100);
int temperature = -1*temperature1;
sensor_analog = analogRead(soil_sensor);
moisture_percentage = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1,
myCounterReadAPIKey);
statusCode = ThingSpeak.getLastReadStatus();
print_data(temperature,humidity,moisture_percentage);
// Printing data to cloud
send_data(temperature,humidity,moisture_percentage);
// end
delay(3000);
digitalWrite(motor,LOW);
}
Serial.println("MOTOR IS TURNED OFF..!!!");
digitalWrite(buzzer,HIGH);
delay(1000);
digitalWrite(buzzer,LOW);
}
else if(temperature>=35 && temperature <50){
digitalWrite(buzzer,HIGH);
delay(1000);
digitalWrite(buzzer,LOW);
delay(1000);
while(moisture_percentage<50){
digitalWrite(motor,HIGH);
delay(100);
int temperature = -1*temperature1;
moisture_percentage = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1,
myCounterReadAPIKey);
statusCode = ThingSpeak.getLastReadStatus();
print_data(temperature,humidity,moisture_percentage);
// Printing data to cloud
send_data(temperature,humidity,moisture_percentage);
//end
delay(3000);digitalWrite(motor,LOW);
}
Serial.println("MOTOR IS TURNED OFF..!!!");
digitalWrite(buzzer,HIGH);
delay(1000);
digitalWrite(buzzer,LOW);
}
}
else if(moisture_percentage>20 && moisture_percentage<=55) {
if(temperature<25){
digitalWrite(buzzer,HIGH);
delay(1000);
digitalWrite(buzzer,LOW);
delay(1000);
while(moisture_percentage<30){
digitalWrite(motor,HIGH);
delay(100);
int temperature = -1*temperature1;
moisture_percentage = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1,
myCounterReadAPIKey);
statusCode = ThingSpeak.getLastReadStatus();
print_data(temperature,humidity,moisture_percentage);
// Printing data to cloud
send_data(temperature,humidity,moisture_percentage);
delay(3000); digitalWrite(motor,LOW);
}
Serial.println("MOTOR IS TURNED OFF..!!!");
digitalWrite(buzzer,HIGH);
delay(1000);
digitalWrite(buzzer,LOW);
}
else if(temperature>=25 && temperature <35){
digitalWrite(buzzer,HIGH);
delay(1000);
digitalWrite(buzzer,LOW);
delay(1000);
while(moisture_percentage<35){
digitalWrite(motor,HIGH);
delay(100);
int temperature = -1*temperature1;
moisture_percentage = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1,
myCounterReadAPIKey);
statusCode = ThingSpeak.getLastReadStatus();
print_data(temperature,humidity,moisture_percentage);
// Printing data to cloud
send_data(temperature,humidity,moisture_percentage);
delay(3000);
digitalWrite(motor,LOW);
} Serial.println("MOTOR IS TURNED OFF..!!!");
digitalWrite(buzzer,HIGH);
delay(1000);
digitalWrite(buzzer,LOW);
}
else if(temperature>=35 && temperature <50){
digitalWrite(buzzer,HIGH);
delay(1000);
digitalWrite(buzzer,LOW);
delay(1000);
while(moisture_percentage<50){
digitalWrite(motor,HIGH);
delay(100);
int temperature = -1*temperature1;
moisture_percentage = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1,
myCounterReadAPIKey);
statusCode = ThingSpeak.getLastReadStatus();
print_data(temperature,humidity,moisture_percentage);
// Printing data to cloud
send_data(temperature,humidity,moisture_percentage);
// end
delay(3000);
digitalWrite(motor,LOW);
} Serial.println("MOTOR IS TURNED OFF..!!!");
digitalWrite(buzzer,HIGH);
delay(1000);
digitalWrite(buzzer,LOW);
}
}
//*************************END*********************************
send_data(temperature,humidity,moisture_percentage);
//***********************END***********************************
}
void sendSensor(int humidity,int moisture,int soil_moisture)
{
  if (isnan(humidity) || isnan(moisture)|| isnan(soil_moisture)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } 
  Blynk.virtualWrite(V5, humidity);  //V5 is for Humidity
  Blynk.virtualWrite(V6, moisture);  //V6 is for  Moisture
  Blynk.virtualWrite(V7, temperature); //v7 is for temperature
}
int sensor=0;
void sendTemps()
{
sensor=analogRead(A0);
sensors.requestTemperatures();
float temp = sensors.getTempCByIndex(0); 
Serial.println(temp);
Serial.println(sensor);
Blynk.virtualWrite(V1, temp);
Blynk.virtualWrite(V2,sensor);
delay(1000);
}
void send_data(int temperature,int humidity, int moistur_percentage){
if (client.connect(server,80)) // "184.106.153.149" or api.thingspeak.com
{
String postStr = apiKey;
postStr +="&field1=";
postStr += String(temperature);
postStr +="&field2=";
postStr += String(humidity);
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
void print_data(int temp,int hum , int moist){
Serial.println("MOTOR IS TURNED ON..!!!\t");
Serial.print("CURRENT MOISTURE PERCENTAGE :");
Serial.print(moist);
Serial.print("\tHUMIDITY : ");
Serial.print(hum);
Serial.print("\tTEMPERATURE :");
Serial.print(temp);
Serial.println();
}
