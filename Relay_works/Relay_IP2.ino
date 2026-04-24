#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>

#define PZEM_RX_PIN D5
#define PZEM_TX_PIN D6
#define RELAY_PIN   D1

SoftwareSerial pzemSWSerial(PZEM_RX_PIN, PZEM_TX_PIN);
PZEM004Tv30 pzem(pzemSWSerial);

#define RELAY_ACTIVE_LOW 1

#if RELAY_ACTIVE_LOW
  #define RELAY_ON  LOW
  #define RELAY_OFF HIGH
#else
  #define RELAY_ON  HIGH
  #define RELAY_OFF LOW
#endif

bool relayState = false;

// Hysteresis thresholds
const float RELAY_ON_BELOW  = 0.12;
const float RELAY_OFF_ABOVE = 0.15;

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
  }
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);

  Serial.begin(115200);
  delay(1000);

  pzemSWSerial.begin(9600);
  delay(1000);

  Serial.println("\n=== PZEM + Relay Threshold Control ===");
  Serial.println("Commands: ON, OFF");
}

void loop() {
  handleSerialCommand();

  float voltage   = pzem.voltage();
  float current   = pzem.current();
  float power     = pzem.power();
  float energy    = pzem.energy();
  float frequency = pzem.frequency();
  float pf        = pzem.pf();

  Serial.println("\n--- Reading Cycle ---");
  Serial.print("Voltage: ");   Serial.print(voltage);   Serial.println(" V");
  Serial.print("Current: ");   Serial.print(current);   Serial.println(" A");
  Serial.print("Power: ");     Serial.print(power);     Serial.println(" W");
  Serial.print("Energy: ");    Serial.print(energy, 3); Serial.println(" kWh");
  Serial.print("Frequency: "); Serial.print(frequency); Serial.println(" Hz");
  Serial.print("PF: ");        Serial.println(pf);
  Serial.print("Relay: ");     Serial.println(relayState ? "ON" : "OFF");

  if (!isnan(current)) {
    if (current < RELAY_ON_BELOW && !relayState) {
      Serial.println("Current low -> Relay ON");
      setRelay(true);
    }
    else if (current > RELAY_OFF_ABOVE && relayState) {
      Serial.println("Current high -> Relay OFF");
      setRelay(false);
    }
  } else {
    Serial.println("Current read failed");
  }

  delay(3000);
}