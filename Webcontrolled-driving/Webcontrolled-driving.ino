#include "LittleFS.h"
#include <WiFi.h>

const char* ssid = "Auto";
const char* password = "12345678";
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  
  // Mount file system
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  

  // WiFi Server
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());

  server.begin();
}

void loop() {

  WiFiClient client = server.available();
  if(client) {

    if(client.connected()) {

      File file = LittleFS.open("/controller.html", "r");

      if(!file){
        Serial.println("Failed to open file for reading");
      }
      
      while(file.available()){
        client.println(file.read());
        Serial.println(file.read());
      }
      client.println();
      file.close();

    }

  }

}
