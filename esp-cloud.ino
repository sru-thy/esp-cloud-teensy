#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#define SERVER_IP "192.168.1.43:5000"

#ifndef STASSID
#define STASSID "Lakshmi-2.4G"
#define STAPSK "12345678"
#endif

uint8_t inbuffer[45000];
uint32_t incount = 0;
unsigned long receivedtime = 0;

bool datareceiving = false;
bool sendtowifi = false;

void setup() {


  Serial.begin(115200);
  //Serial1.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (Serial.available()) {
    inbuffer[incount] = Serial.read();
    incount++;
    receivedtime = millis();
    datareceiving = true;
  } else {
    if (datareceiving) {
      if (millis() > receivedtime + 1000) {
        datareceiving = false;
        sendtowifi = true;
      }
    }
  }
  if (sendtowifi) {
    sendtowifi = false;

    if ((WiFi.status() == WL_CONNECTED)) {

      WiFiClient client;
      HTTPClient http;

      

      http.begin(client, "http://" SERVER_IP "/predict");

      http.addHeader("Content-Type", "application/json");



      int httpCode = http.POST(inbuffer, incount);
      incount = 0;
      
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        
        Serial.println(payload);
        
      } else {
        
        Serial.println("Error");
      }

      http.end();
    }
  }
  //delay(30000);
}
