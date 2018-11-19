#include <avr/io.h>
#include <avr/interrupt.h>
#include <EEPROM.h>
#include "constants.h"

// Global variables
int address = EEPROM.read(0);  // Read from the EEPROM:  Uno or Due
float refValue = 0; // Desired illuminance in Lux
int input = 0;

// Setup Interrupt
volatile bool flag;
ISR(TIMER1_COMPA_vect) {
  flag = 1; //notify main loop
}

// Equivalent of map function for floats
float mapfloat(double val, double in_min, double in_max, double out_min, double out_max) {
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void setup() {
  // Setup pinmodes
  setConstants(address);
  pinMode(LED1, OUTPUT);
  pinMode(LDRpin, INPUT);
  Serial.begin(115200);

  // Reference value
  calibrateSystem();
}
  
void loop() {
  if (flag)
  {
    // Read current output - y
    float measuredY = readLDR();
    
    // Print measurement
    Serial.print("Measured illuminance: ");
    Serial.println(measuredY);
    flag = 0;
  }

  delay(1000);
}
