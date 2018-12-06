#include <avr/io.h>
#include <avr/interrupt.h>
#include <EEPROM.h>
#include <Wire.h>
#include "Node.h"
#include "constants.h"
#include "globals.h"

// Global variables
float refValue = 0; // Desired illuminance in Lux
int input = 0;


// Setup Interrupt
volatile bool flag;
ISR(TIMER1_COMPA_vect) {
  flag = 1; //notify main loop
}

// Setup Timer Interrupt to 200 Hz sampling frequency 
void timerSetup(){
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
}

void setup() {
  // Setup pinmodes
  setConstants(own_addr);
  pinMode(LED1, OUTPUT);
  pinMode(LDRpin, INPUT);

  Serial.begin(250000);
  calibrateSystem();

  // Setup Timer interrupt (200 Hz)
  timerSetup();

  // Setup I2C communication
  I2CSetup();  

  // Reference value
  Serial.print("Insert desired Lux value [0, 300]: ");
  while (refValue == 0) {
    if (Serial.available() > 0) {
      refValue = Serial.parseInt();
      HighValue = refValue;
      if (own_addr==1) L1 = refValue;
      else if (own_addr==2) L2 = refValue;
      Serial.flush();
    }
  }

  // Setup consensus protocol - setup my_node
  consensusSetup();
}

void loop() {
  struct solution sol;
  //sendNegotiationState(Negotiation);
  if (flag){
    // Read current output - y
    float measuredY = readLDR();

    // Apply control law
    float u = Controller(refValue, measuredY);        
    u = mapfloat(u, 0, 5, 0, 255);
    analogWrite(LED1, u);
    
    // Print measurement
    //Serial.print("Measured illuminance: ");
    Serial.println(measuredY);
    sendLuxReading(measuredY);
    flag = 0;
  }
  if (Negotiation){
    sol = consensus_algorithm();
    Serial.print("solution = ");
    Serial.print(sol.d[0]);
    Serial.print(" ");
    Serial.println(sol.d[1]);
    Serial.println();
  }

  if (Serial.available()){
    input = Serial.parseInt();
    if (input == -1 and occupancy == 1) {
      Serial.println("Empty");
      occupancy = 0;
      refValue = LowValue;    // Empty desk
      my_node.L = refValue;
      my_node.updateGain(G0(refValue));
      
      Negotiation = 1;
      sendNegotiationState(Negotiation);
      Serial.flush();
    }
    else if (input == 1 and occupancy == 0) {
      Serial.println("Occupied");
      occupancy = 1;
      refValue = HighValue;   // Occupied desk
      my_node.L = refValue;
      my_node.updateGain(G0(refValue));
      
      Negotiation = 1;
      sendNegotiationState(Negotiation);
      Serial.flush();
    }
  }  
}
