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

  Serial.begin(250000);
  calibrateSystem();

  // Setup Timer interrupt (200 Hz)
  cli(); // stop interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  // set compare match register for 200 Hz increments
  OCR1A = 9999; // = 16000000 / (8 * 200) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12, CS11 and CS10 bits for 8 prescaler
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei(); // allow interrupts

  // Reference value
  Serial.print("Insert desired Lux value [0, 300]: ");
  while (refValue == 0) {
    if (Serial.available() > 0) {
      refValue = Serial.parseInt();
    }
  }
}

void loop() {
  if (flag)
  {
    // Read current output - y
    float measuredY = readLDR();

    // Apply control law
    float u = Controller(refValue, measuredY);        
    u = mapfloat(u, 0, 5, 0, 255);
    analogWrite(LED1, u);
    
    // Print measurement
    Serial.print("Measured illuminance: ");
    Serial.println(measuredY);
    flag = 0;
  }

  if (Serial.available()){
    input = Serial.parseInt();
    if (input == 0) {
      Serial.println("Empty");
      refValue = LowValue;    // Empty desk
    }
    else if (input == 1) {
      Serial.println("Occupied");
      refValue = HighValue;   // Occupied desk
    }
  }  
}
