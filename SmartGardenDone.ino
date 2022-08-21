#include <ESP8266WiFi.h>
#include <Wire.h>
#include "SSD1306.h"

int D1, D2 ;

SSD1306  display(0x3c, D1, D2);

// It's the channel's thingspeak API key,
String apiKey = "7BE9DUC616QRJIOO";
const char* ssid = "DESKTOP-NUM3BKI 3540";  // Enter Wi-fi ID
const char* password = "<E3i6291";   // Enter Wi-fi password
const char* server = "api.thingspeak.com";
WiFiClient client;

void setup() {    
  Serial.begin(9600);
  pinMode(A0,INPUT);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  
  delay(10);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
}


void loop() {
  display.clear();
  int temp = analogRead(A0);
  temp = map(temp,550,0,0,100);

  if (client.connect(server,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(temp);
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

     
     if(temp<0){        
        Serial.print(" Sensor Not connected!\n");
     }
     else if(temp<10){
        Serial.print("Moisture Level: ");
        Serial.print(temp);
        Serial.print("%, Needs Water!!! \n");
     }
     else{
        Serial.print("Moisture Level: ");
        Serial.print(temp);
        Serial.print("%");
        Serial.print("\n");
        Serial.print("Sensor Value ");
        Serial.print(temp);
        Serial.println(" send to Thingspeak\n");
     }
     
     display.drawString(0, 20,"Moisture: ");
     display.drawString(0, 30,String(temp));
     display.drawString(0, 10, "Send to Thingspeak");  
     display.display();
  }

//  client.stop();

   Serial.println("Waiting...");    
   
  // thingspeak needs minimum 15 sec delay between updates
  // Here I set upload every 20 second

   delay(20000); //will wait for 20 second
    
}
