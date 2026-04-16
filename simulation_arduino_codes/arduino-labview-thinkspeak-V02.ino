#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "TEST";
const char* password = "123456789";
String apiKey = "1234567890";
const char* server = "http://api.thingspeak.com/update";

void setup() {
  Serial.begin(115200);
  
  Serial.println();
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  
  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("Wi-Fi Connected successfully!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Attempting to send fake test data to ThingSpeak...");
    
    HTTPClient http;
    
    // We will send 230V, 5A, and 1150W as a test
    String url = String(server) + "?api_key=" + apiKey + "&field1=230&field2=5&field3=1150";
    
    http.begin(url);
    int httpCode = http.GET(); // Send the request
    http.end();
    
    if (httpCode == 200) {
      Serial.println("SUCCESS! Data sent to ThingSpeak. Check your dashboard.");
    } else {
      Serial.print("FAILED to send. HTTP Error Code: ");
      Serial.println(httpCode);
    }
  } else {
    Serial.println("Error: Wi-Fi disconnected.");
  }
  
  // Wait 20 seconds before trying again
  delay(20000); 
}