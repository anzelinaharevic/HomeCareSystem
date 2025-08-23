#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

PulseOximeter pox;
uint32_t tsLastReport = 0;

int totalScans = 0;
float bpmSum = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  Serial.print("Initializing sensor...");
  if (!pox.begin()) {
    Serial.println("FAILED");
    while (true); // stop here if sensor init failed
  }
  Serial.println("SUCCESS");

  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}

void loop() {
  pox.update();

  if (millis() - tsLastReport > 500) { // every 1 second
    float bpm = pox.getHeartRate();
    float spo2 = pox.getSpO2();

    printScanData(totalScans + 1, bpm, spo2);
    processScan(bpm);

    tsLastReport = millis();
  }
}

// Prints scan number and sensor data
void printScanData(int scanNumber, float bpm, float spo2) {
  if(scanNumber > 9){
    Serial.print("Scan #");
  Serial.print(scanNumber+10);
  Serial.print(" - BPM: ");
  Serial.print(bpm);
  Serial.print("  SpO2: ");
  Serial.println(spo2);
  }
}

// Processes the BPM reading for accumulation and checks emergency when appropriate
void processScan(float bpm) {
  totalScans++;

  if (totalScans > 10 && totalScans <= 110) {
    if (bpm > 0) {  // valid reading check
      bpmSum += bpm;
    }
  }

  if (totalScans == 110) {
    checkEmergency();
    resetCounters();
  }
}

// Calculates average BPM and prints emergency if outside safe range
void checkEmergency() {
  float averageBPM = bpmSum / 100.0; // 100 - 10 = 90 accumulated scans
  Serial.print("Average BPM (scans 1-100): ");
  Serial.println(averageBPM);

  if (averageBPM > 190 || averageBPM < 36) {
    Serial.println("EMERGENCY");
  } else {
    Serial.println("Heart rate normal.");
  }
}

// Resets counters for next measurement cycle
void resetCounters() {
  totalScans = 0;
  bpmSum = 0;
}
