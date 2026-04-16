#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>

#define PZEM_RX_PIN D5   // GPIO14
#define PZEM_TX_PIN D6   // GPIO12

SoftwareSerial pzemSWSerial(PZEM_RX_PIN, PZEM_TX_PIN);
PZEM004Tv30 pzem(pzemSWSerial);

void setup() {
    Serial.begin(115200);
    delay(1000);
    pzemSWSerial.begin(9600);
    delay(1000);
    
    Serial.println("\n\n=== PZEM-004T Initialization ===");
    
    // Test communication
    uint8_t addr = pzem.readAddress();
    Serial.print("Sensor Address: 0x");
    Serial.println(addr, HEX);
    
    if (addr == 0xF8) {
        Serial.println("✓ Communication OK");
    } else if (addr == 0x00 || addr == 0xFF) {
        Serial.println("✗ Communication FAILED - Check RX/TX pins!");
    }
    
    delay(2000);
}

void loop() {
    Serial.println("\n--- Reading Cycle ---");
    
    // Read each value with delay
    Serial.print("Reading Voltage... ");
    float voltage = pzem.voltage();
    Serial.print(voltage); Serial.println(" V");
    
    delay(100);
    
    Serial.print("Reading Current... ");
    float current = pzem.current();
    Serial.print(current); Serial.println(" A");
    
    delay(100);
    
    Serial.print("Reading Power... ");
    float power = pzem.power();
    Serial.print(power); Serial.println(" W");
    
    delay(100);
    
    Serial.print("Reading Energy... ");
    float energy = pzem.energy();
    Serial.print(energy); Serial.println(" kWh");
    
    delay(100);
    
    Serial.print("Reading Frequency... ");
    float frequency = pzem.frequency();
    Serial.print(frequency); Serial.println(" Hz");
    
    delay(100);
    
    Serial.print("Reading Power Factor... ");
    float pf = pzem.pf();
    Serial.println(pf);
    
    // Summary
    Serial.println("\n=== Summary ===");
    Serial.print("Voltage: ");  Serial.print(voltage);   Serial.println(" V");
    Serial.print("Current: ");  Serial.print(current);   Serial.println(" A");
    Serial.print("Power: ");    Serial.print(power);     Serial.println(" W");
    Serial.print("Energy: ");   Serial.print(energy, 3); Serial.println(" kWh");
    Serial.print("Frequency: ");Serial.print(frequency); Serial.println(" Hz");
    Serial.print("PF: ");       Serial.println(pf);
    
    // Diagnostic checks
    Serial.println("\n=== Diagnostics ===");
    if (isnan(voltage)) {
        Serial.println("❌ Voltage is NaN");
    }
    if (isnan(current)) {
        Serial.println("❌ Current is NaN (Communication issue?)");
    } else if (current == 0.0) {
        Serial.println("⚠️  Current is 0.0A (Check CT coil!)");
    }
    
    delay(3000);
} 
