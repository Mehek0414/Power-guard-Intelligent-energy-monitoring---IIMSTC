void setup() {
  // Start the serial connection at 115200 baud rate
  Serial.begin(115200);
  
  // Set up the built-in LED (usually GPIO 2) for visual feedback
  pinMode(2, OUTPUT);
  Serial.println("ESP32 Ready. Waiting for LabVIEW data...");
}

void loop() {
  // Check if LabVIEW is sending data over the USB cable
  if (Serial.available()) {
    // Read the incoming string until the "Newline" character
    String incomingData = Serial.readStringUntil('\n');
    incomingData.trim(); // Remove extra spaces
    
    // We expect LabVIEW to send data like this: "230.5,2.1" (Voltage,Current)
    int commaIndex = incomingData.indexOf(',');
    
    if (commaIndex > 0) {
      // Split the string into Voltage and Current
      String voltString = incomingData.substring(0, commaIndex);
      String currString = incomingData.substring(commaIndex + 1);
      
      // Convert to decimals (floats)
      float voltage = voltString.toFloat();
      float current = currString.toFloat();
      
      // Calculate simulated Power
      float power = voltage * current;
      
      // Blink the LED so you know data arrived!
      digitalWrite(2, HIGH);
      delay(50);
      digitalWrite(2, LOW);
      
      // Print it back so we can verify it works
      Serial.print("ESP32 Processed -> Voltage: ");
      Serial.print(voltage);
      Serial.print("V, Current: ");
      Serial.print(current);
      Serial.print("A, Power: ");
      Serial.print(power);
      Serial.println("W");
      
      // NEXT PHASE: This is where we will add the code to send this data to ThingSpeak/Firebase!
    }
  }
}