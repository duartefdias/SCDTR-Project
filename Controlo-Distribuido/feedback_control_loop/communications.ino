#include <Wire.h>
#include <EEPROM.h>
#include "globals.h"
#define MAXLUX 500
#define MAXNODES 10
 
volatile uint8_t rcAddress, rcMessageType, rcN, rcPwmNegotiation[MAXNODES], rcNegotiation;
volatile float rcPwm[MAXNODES];

void I2CSetup() {
  Wire.begin(own_addr);
  TWAR = (own_addr << 1) | 1;  // enable broadcasts to be received
  Wire.onReceive(receiveEvent);   // set up receive handler
}

void receiveEvent(int howMany) {  
  // we are expecting 3 or 4 bytes, so check we got them
  if (howMany == 3 || howMany == 4 || howMany == 5 || howMany == 6) {
     rcAddress = Wire.read();
     rcMessageType = Wire.read();

    // Negotiation message received
     if(rcMessageType == 2) {
        rcN = Wire.read();
        for (int j=0; j<rcN; j++){
          rcPwmNegotiation[j] = Wire.read();
          rcPwm[j] = mapfloat(rcPwmNegotiation[j], 0, 255, 0, 5);
        }
        //Serial.print("Received "); Serial.print(rcN); Serial.println(" values from other");
        copy_array(rcPwm, other_solution.d, rcN);
        /*rcPwmNegotiation1 = Wire.read();
        rcPwmNegotiation2 = Wire.read();
        rcPwm1 = mapfloat(rcPwmNegotiation1, 0, 255, 0, 5);
        rcPwm2 = mapfloat(rcPwmNegotiation2, 0, 255, 0, 5);
        other_solution.d[0] = rcPwm1;
        other_solution.d[1] = rcPwm2;*/
        //Serial.print(" Received solution: d[0] = "); Serial.print(other_solution.d[0]); Serial.print(" d[1] = "); Serial.println(other_solution.d[1]);
        ReceivedSolution = true;
     }
     // Negotiation State
     else if(rcMessageType == 7) {
        rcNegotiation = Wire.read();
        if (rcNegotiation == 1) {           // received negotiation request  
          Negotiation = 1;                 
        } else if (rcNegotiation == 0) {    // no negotiation
          Negotiation = 0;
        } else if (rcNegotiation == 2) {    // received recalibration request
          Negotiation = 2;         
        }
     }
  }
  // throw away any garbage
  while (Wire.available()) {
    Wire.read();    
  }
}


void sendPwm(float pwm){
  uint8_t pwmValue = mapfloat(pwm, 0, 5, 0, 255);
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_addr);
  Wire.write(0);  //message type
  Wire.write(pwmValue);
  Wire.endTransmission(); //release BUS
}

void sendLuxReading(float luxReading){
  uint16_t luxValue = mapfloat(luxReading, 0, MAXLUX, 0, 65536);
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_addr);
  Wire.write(1);  //message type
  uint8_t luxH = luxValue >> 8;
  uint8_t luxL = luxValue & 0xFF;
  Wire.write(luxH);
  Wire.write(luxL);
  Wire.endTransmission(); //release BUS
}

void sendNegotiation(float pwmValues[], int N){
  uint8_t pwmNegotiation;
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_addr);
  Wire.write(2);  //message type
  Wire.write(N);
  for (int j=0; j<N; j++){
    pwmNegotiation = mapfloat(pwmValues[j], 0, 5, 0, 255);
    Wire.write(pwmNegotiation);
  }
  Wire.endTransmission(); //release BUS
}

void sendOccupancy(uint8_t occupancy){
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_addr);
  Wire.write(3);  //message type
  Wire.write(occupancy);
  Wire.endTransmission(); //release BUS
}

void sendLuxLowerBound(float lowerBound){
  uint16_t luxLowerBound = mapfloat(lowerBound, 0, MAXLUX, 0, 65536);
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_addr);
  Wire.write(4);  //message type
  uint8_t luxH = luxLowerBound >> 8;
  uint8_t luxL = luxLowerBound & 0xFF;
  Wire.write(luxH);
  Wire.write(luxL);
  Wire.endTransmission(); //release BUS
}

void sendLuxBackground(float background){
  uint16_t luxBackground = mapfloat(background, 0, MAXLUX, 0, 65536);
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_addr);
  Wire.write(5);  //message type
  uint8_t luxH = luxBackground >> 8;
  uint8_t luxL = luxBackground & 0xFF;
  Wire.write(luxH);
  Wire.write(luxL);
  Wire.endTransmission(); //release BUS
}

void sendPWMRef(float pwmReff){
  uint8_t pwmRef = mapfloat(pwmReff, 0, 5, 0, 255);
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_addr);
  Wire.write(6);  //message type
  Wire.write(pwmRef);
  Wire.endTransmission(); //release BUS
}

void sendNegotiationState(uint8_t state){
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_addr);
  Wire.write(7);  //message type
  Wire.write(state);
  Wire.endTransmission(); //release BUS
}