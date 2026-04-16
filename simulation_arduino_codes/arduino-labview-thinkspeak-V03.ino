#include <WiFi.h>
#include <HTTPClient.h>

// 1. Replace with your Wi-Fi details
const char* ssid = "TEST";
const char* password = "123456789";

// 2. Replace with your ThingSpeak Write API Key
String apiKey = "1234567890";
const char* server = "http://api.thingspeak.com/update";

float latestVoltage = 0.0;
float totalCurrent = 0.0;
float totalPower = 0.0;

unsigned long lastUploadTime = 0;
const unsigned long uploadInterval = 20000; 

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT); 

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  // Read simulated data from LabVIEW
  if (Serial.available()) {
    String incomingData = Serial.readStringUntil('\n');
    incomingData.trim();
    
    int commaIndex = incomingData.indexOf(',');
    if (commaIndex > 0) {
      latestVoltage = incomingData.substring(0, commaIndex).toFloat();
      totalCurrent = incomingData.substring(commaIndex + 1).toFloat();
      
      // OPTIONAL: Enforce your 220-240V rule in software just in case the LabVIEW knob goes out of bounds
      if (latestVoltage < 220.0) latestVoltage = 220.0;
      if (latestVoltage > 240.0) latestVoltage = 240.0;

      // Calculate Total Power Consumption
      totalPower = latestVoltage * totalCurrent; 
      
      digitalWrite(2, HIGH);
      delay(20);
      digitalWrite(2, LOW);
    }
  }

  // Upload to ThingSpeak every 20 seconds
  if (millis() - lastUploadTime > uploadInterval) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      
      String url = String(server) + "?api_key=" + apiKey + 
                   "&field1=" + String(latestVoltage) + 
                   "&field2=" + String(totalCurrent) + 
                   "&field3=" + String(totalPower);
      
      http.begin(url);
      http.GET(); 
      http.end();
      
      lastUploadTime = millis();
    }
  }
}