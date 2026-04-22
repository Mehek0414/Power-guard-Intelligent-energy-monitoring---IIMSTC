#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>

#define PZEM_RX_PIN D5   // GPIO14
#define PZEM_TX_PIN D6   // GPIO12
#define RELAY_PIN   D1   // GPIO5

SoftwareSerial pzemSWSerial(PZEM_RX_PIN, PZEM_TX_PIN);
PZEM004Tv30 pzem(pzemSWSerial);

// Set this to 1 if your relay module is ACTIVE-LOW
#define RELAY_ACTIVE_LOW 1

#if RELAY_ACTIVE_LOW
  #define RELAY_ON  LOW
  #define RELAY_OFF HIGH
#else
  #define RELAY_ON  HIGH
  #define RELAY_OFF LOW
#endif

bool relayState = false;
bool autoCutoff = true;
const float CURRENT_LIMIT = 5.0;   // Change as needed

void setRelay(bool on) {
  relayState = on;
  digitalWrite(RELAY_PIN, on ? RELAY_ON : RELAY_OFF);

  Serial.print("Relay State: ");
  Serial.println(on ? "ON" : "OFF");
}

void handleSerialCommand() {
  if (!Serial.available()) return;

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  cmd.toUpperCase();

  if (cmd == "ON") {
    setRelay(true);
  } else if (cmd == "OFF") {
    setRelay(false);
  } else if (cmd == "AUTO1") {
    autoCutoff = true;
    Serial.println("Auto cutoff ENABLED");
  } else if (cmd == "AUTO0") {
    autoCutoff = false;
    Serial.println("Auto cutoff DISABLED");
  }
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);   // Keep relay OFF at startup

  Serial.begin(115200);
  delay(1000);

  pzemSWSerial.begin(9600);
  delay(1000);

  Serial.println("\n\n=== PZEM + Relay Initialization ===");
  Serial.println("Commands: ON, OFF, AUTO1, AUTO0");

  uint8_t addr = pzem.readAddress();
  Serial.print("Sensor Address: 0x");
  Serial.println(addr, HEX);

  if (addr == 0xF8) {
    Serial.println("PZEM Communication OK");
  } else if (addr == 0x00 || addr == 0xFF) {
    Serial.println("PZEM Communication FAILED - Check RX/TX pins!");
  }

  delay(1000);
}

void loop() {
  handleSerialCommand();

  Serial.println("\n--- Reading Cycle ---");

  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();

  Serial.print("Voltage: ");   Serial.print(voltage);   Serial.println(" V");
  Serial.print("Current: ");   Serial.print(current);   Serial.println(" A");
  Serial.print("Power: ");     Serial.print(power);     Serial.println(" W");
  Serial.print("Energy: ");    Serial.print(energy, 3); Serial.println(" kWh");
  Serial.print("Frequency: "); Serial.print(frequency); Serial.println(" Hz");
  Serial.print("PF: ");        Serial.println(pf);
  Serial.print("Relay: ");     Serial.println(relayState ? "ON" : "OFF");

  if (isnan(voltage)) {
    Serial.println("Voltage read failed");
  }

  if (isnan(current)) {
    Serial.println("Current read failed");
  } else if (current == 0.0) {
    Serial.println("Current is 0.0A");
  }

  // Auto protection example
  if (!isnan(current) && autoCutoff && relayState && current > CURRENT_LIMIT) {
    Serial.println("Over-current detected -> Turning relay OFF");
    setRelay(false);
  }

  delay(3000);
}