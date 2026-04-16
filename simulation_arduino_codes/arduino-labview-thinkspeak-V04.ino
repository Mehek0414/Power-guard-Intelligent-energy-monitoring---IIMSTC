#include <WiFi.h>
#include <HTTPClient.h>

// 1. Replace with your Wi-Fi details
const char* ssid = "TEST";
const char* password = "123456789";

// 2. Replace with your ThingSpeak Write API Key
String apiKey = "123456789";
const char* server = "http://api.thingspeak.com/update";

float latestVoltage = 0.0;
float totalCurrent = 0.0;
float totalPower = 0.0;

// New variables for Energy and Cost
float totalEnergy_kWh = 0.0;
float costPerUnit = 8.0; // Change this to your local electricity rate (e.g., ₹8 per kWh)
float totalBill = 0.0;

unsigned long lastUploadTime = 0;
const unsigned long uploadInterval = 20000; // 20 seconds

// Timer for calculating energy consumption accurately
unsigned long lastCalcTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT); 

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  lastCalcTime = millis();
}

void loop() {
  // 1. Read simulated data from LabVIEW
  if (Serial.available()) {
    String incomingData = Serial.readStringUntil('\n');
    incomingData.trim();
    
    int commaIndex = incomingData.indexOf(',');
    if (commaIndex > 0) {
      latestVoltage = incomingData.substring(0, commaIndex).toFloat();
      totalCurrent = incomingData.substring(commaIndex + 1).toFloat();
      
      // Calculate instantaneous power
      totalPower = latestVoltage * totalCurrent; 
      
      digitalWrite(2, HIGH);
      delay(20);
      digitalWrite(2, LOW);
    }
  }

  // 2. Continually calculate accumulated Energy (kWh)
  unsigned long currentTime = millis();
  unsigned long timeElapsed = currentTime - lastCalcTime; // Time since last calculation in milliseconds
  
  if (timeElapsed > 1000) { // Update energy accumulator every 1 second
    // Convert power (Watts) to Kilowatts (kW)
    float power_kW = totalPower / 1000.0;
    
    // Convert time elapsed (milliseconds) to hours
    float hoursElapsed = timeElapsed / 3600000.0;
    
    // Add to total energy: Energy = Power(kW) * Time(hours)
    // Note: Because a showcase is short, we multiply by a "Speed Up Factor" (e.g., 3600) 
    // so you can actually watch the bill increase during a 5-minute presentation!
    float speedUpFactor = 3600.0; // Makes 1 real second act like 1 hour of usage
    totalEnergy_kWh += (power_kW * hoursElapsed * speedUpFactor);
    
    // Calculate total bill
    totalBill = totalEnergy_kWh * costPerUnit;
    
    lastCalcTime = currentTime;
  }

  // 3. Upload to ThingSpeak every 20 seconds
  if (currentTime - lastUploadTime > uploadInterval) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      
      // Send 5 fields now: Voltage, Current, Power, Energy (kWh), and Cost
      String url = String(server) + "?api_key=" + apiKey + 
                   "&field1=" + String(latestVoltage) + 
                   "&field2=" + String(totalCurrent) + 
                   "&field3=" + String(totalPower) +
                   "&field4=" + String(totalEnergy_kWh, 4) + // Send with 4 decimal places
                   "&field5=" + String(totalBill, 2);        // Send with 2 decimal places
      
      http.begin(url);
      http.GET(); 
      http.end();
      
      lastUploadTime = currentTime;
    }
  }
}