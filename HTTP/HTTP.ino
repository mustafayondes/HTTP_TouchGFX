
#include <WiFi.h>
#include <HTTPClient.h>
#include <HardwareSerial.h>
#include <ArduinoJson.h>


 HardwareSerial UART(1);

#define RX 11
#define TX 10

const char* ssid = "Mustafa";
const char* password = "1234567mm";

//Your Domain name with URL path or IP address with path
String serverName = "https://api.restful-api.dev/objects";
//"https://api.publicapis.org/random";


// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200); 

  UART.begin(115200,SERIAL_8N1,RX,TX);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

  
}

void loop() {
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName ;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
       
         DynamicJsonDocument doc(4096); // Bellek boyutu ayarlanabilir
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    // JSON verisini parçalama
    JsonArray arr = doc.as<JsonArray>();
    for (JsonObject obj : arr) {
      const char* id = obj["id"];
      const char* name = obj["name"];
      
      Serial.println("--------------------");
      Serial.print("ID: ");
      Serial.println(id);
      Serial.print("Name: ");
      Serial.println(name);


      UART.println("--------------------");
      UART.print("ID: ");
      UART.println(id);
      UART.print("Name: ");
      UART.println(name);

      // Data alanını parçalama (eğer varsa)
      if (obj.containsKey("data")) {
        JsonObject data = obj["data"];
        Serial.println("Data:");
        for (JsonPair kvp : data) {
          Serial.print(kvp.key().c_str());
           UART.println(kvp.key().c_str());

          Serial.print(": ");
          if (kvp.value().is<const char*>()) {
            Serial.println(kvp.value().as<const char*>());
            UART.println(kvp.value().as<const char*>());

          } else {
            Serial.println(kvp.value().as<float>(), 2); // Float için 2 ondalık basamak
             UART.println(kvp.value().as<float>(), 2);

          }
        }
      }
    }
       // Serial.println(payload);
       // UART.println(payload);

      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}