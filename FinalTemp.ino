#include <FirebaseArduino.h>
#include <FirebaseHttpClient.h>
#include <ESP8266WiFi.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
#define rpin 0
#define FIREBASE_HOST "cdacproject-8bc9f.firebaseio.com"
#define FIREBASE_AUTH "3g5ADcUBvb82U7Rf8LtjhnUeetGjTFbT2dSXw91K"
#define WIFI_SSID "Sarthak"
#define WIFI_PASSWORD "9229569411"
DHT dht(DHTPIN, DHTTYPE);
int airsensor = A0;
int airdata;
int bedroomstate;
void setup() {
  pinMode(airsensor, INPUT);
  pinMode(rpin, OUTPUT);
  Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("DHT11 test");
  dht.begin();
}
void loop(){
  /* collecting all data */
  airdata = analogRead(airsensor); 
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.println(t);
  Serial.println(h);
  bedroomstate = Firebase.getInt("bedroomstate");
 digitalWrite(rpin, !bedroomstate); 
  /* setting temparature and humidity to firebase */
  Firebase.set("temp", t);
  Firebase.set("humidity", h);
  
  /* set air sensor data to firebase */
   if((airdata >= 0) && (airdata <= 50)){
      Serial.println("Good");
    Firebase.set("air_quality", "Good");
    }
    else if ((airdata >= 51) && (airdata <= 100)){
      Serial.println("moderate");
      Firebase.set("air_quality", "Moderate");
      }
    else if ((airdata >= 101) && (airdata <= 150)){
        Serial.println("unhealthy for sensitive people");
      Firebase.set("air_quality", "Unhealthy for sensitive people");
      }
    else if ((airdata >= 151) && (airdata <= 200)){
      Serial.println("unhealthy");
      Firebase.set("air_quality", "Unhealthy");
      }
    else if ((airdata >= 201) && (airdata <= 300)){
      Serial.println("very unhealthy");
      Firebase.set("air_quality", "Very Unhealthy");
      }
    else if (airdata >= 300){
        Serial.println("hazardous");
      Firebase.set("air_quality", "Hazardous");
      }
  delay(500);
  }
