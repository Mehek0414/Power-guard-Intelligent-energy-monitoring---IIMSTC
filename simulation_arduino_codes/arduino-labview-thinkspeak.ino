#include <WiFi.h>
#include <HTTPClient.h>

// 1. Replace with your Wi-Fi details
const char* ssid = "TEST";
const char* password = "123456789";

// 2. Replace with your ThingSpeak Write API Key
String apiKey = "1234567890";
const char* server = "http://api.thingspeak.com/update";

// Variables to store the latest values from LabVIEW
float latestVoltage = 0.0;
float latestCurrent = 0.0;
float latestPower = 0.0;

// Timer for ThingSpeak (Upload every 20 seconds)
unsigned long lastUploadTime = 0;
const unsigned long uploadInterval = 20000; 

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT); // Built-in LED

  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
}

void loop() {

  // 1. Read data from LabVIEW via Serial
  if (Serial.available()) {
    String incomingData = Serial.readStringUntil('\n');
    incomingData.trim();
    
    int commaIndex = incomingData.indexOf(',');

    if (commaIndex > 0) {
      // Parse Voltage & Current
      latestVoltage = incomingData.substring(0, commaIndex).toFloat();
      latestCurrent = incomingData.substring(commaIndex + 1).toFloat();
      latestPower = latestVoltage * latestCurrent;

      // Print values (for debugging & viva)
      Serial.print("Voltage: ");
      Serial.println(latestVoltage);

      Serial.print("Current: ");
      Serial.println(latestCurrent);

      Serial.print("Power: ");
      Serial.println(latestPower);

      // LED blink (data received)
      digitalWrite(2, HIGH);
      delay(20);
      digitalWrite(2, LOW);
    }
  }

  // 2. Upload to ThingSpeak every 20 seconds
  if (millis() - lastUploadTime > uploadInterval) {

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      String url = String(server) + "?api_key=" + apiKey + 
                   "&field1=" + String(latestVoltage) + 
                   "&field2=" + String(latestCurrent) + 
                   "&field3=" + String(latestPower);

      http.begin(url);
      int httpCode = http.GET();

      // Check response
      if (httpCode > 0) {
        Serial.println("Data sent to ThingSpeak successfully!");
      } else {
        Serial.println("Error sending data to ThingSpeak");
      }

      http.end();
    } else {
      Serial.println("WiFi Disconnected!");
    }

    lastUploadTime = millis();
  }
}