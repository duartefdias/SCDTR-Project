#include <avr/io.h>
#include <avr/interrupt.h>
#include <EEPROM.h>
#include <Wire.h>
#include "Node.h"
#include "constants.h"
#include "globals.h"

int debug = 1;

void setup() {
  // Setup pinmodes
  setConstants(own_addr);
  pinMode(LED1, OUTPUT);
  pinMode(LDRpin, INPUT);
  
  Serial.begin(115200);

  // Setup I2C communication
  I2CSetup();  
  // Compute illuminance influence of different LED's
  calibrateSystem();  
  // Setup Timer interrupt (100 Hz)
  timerSetup();  

  // Reference value
  Serial.print("Insert desired Lux value [0, 300]: ");
  while (refValue == 0) {
    if (Serial.available()) {
      refValue = Serial.parseInt();
      HighValue = refValue;
      Serial.flush();
    }
  }
  Serial.println(refValue);
  Negotiation = 1;
  sendNegotiationState(Negotiation);
  Serial.flush();

  // Setup consensus protocol and setup my_node
  consensusSetup();

  // TODO: some initial setup to do consensus and send initial values to server
}

void loop() {
  struct solution sol;
  //if (Negotiation) sendNegotiationState(Negotiation);
  if (flag){
    // Read current output - y
    float measuredY = readLDR();

    // Apply control law
    float u = Controller(refValue, measuredY);        
    u = mapfloat(u, 0, 5, 0, 255);
    analogWrite(LED1, u);
    
    // Print and send Lux measurement
    //Serial.print("Measured illuminance: ");
    //Serial.println(measuredY);
    sendLuxReading(measuredY);
    sendPwm(u);
    flag = 0;
  }
  if (Negotiation == 1){
    sol = consensus_algorithm();
    Serial.print("solution = "); Serial.print(sol.d[0]); Serial.print(" "); Serial.println(sol.d[1]); Serial.println();
    sendPWMRef(my_node.d[my_node.index]);
  }
  // Reset system (New node connected)
  if (Negotiation == 2){
    Serial.println("  SYSTEM RECALIBRATION");
    calibrateSystem();    
  }

  // Serial interface:
  // command: action
  //      -1: change desk occupancy to empty
  //       1: change desk occupancy to occupied
  //  'c' $p: change energy cost to $p (int) 
  //  'r' $p: change lux High reference to $p (int)
  if (Serial.available()){
    parseInput();
  }  
}
