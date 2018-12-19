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
  byte buf[3];
  buf[0] = own_addr;
  buf[1] = 0;
  buf[2] = pwmValue;
  Wire.beginTransmission(0);//get BUS
  Wire.write(buf, sizeof(buf));
  Wire.endTransmission(); //release BUS
}

void sendLuxReading(float luxReading){
  uint16_t luxValue = mapfloat(luxReading, 0, MAXLUX, 0, 65536);
  byte buf[4];
  buf[0] = own_addr;
  buf[1] = 1;  //message type
  buf[2] = luxValue >> 8;
  buf[3] = luxValue & 0xFF;
  Wire.beginTransmission(0);//get BUS
  Wire.write(buf, sizeof(buf));
  Wire.endTransmission(); //release BUS
}

void sendNegotiation(float pwmValues[], int N){
  uint8_t pwmNegotiation;
  byte buf[3+N];
  buf[0] = own_addr;
  buf[1] = 2;  //message type
  buf[2] = N;
  for (int j=0; j<N; j++){
    pwmNegotiation = mapfloat(pwmValues[j], 0, 5, 0, 255);
    buf[j+3] = pwmNegotiation;
  }
  Wire.beginTransmission(0);//get BUS
  Wire.write(buf, sizeof(buf));
  Wire.endTransmission(); //release BUS
}

void sendOccupancy(uint8_t occupancy){
  byte buf[3];
  buf[0] = own_addr;
  buf[1] = 3;  //message type
  buf[2] = occupancy;
  Wire.beginTransmission(0);//get BUS
  Wire.write(buf, sizeof(buf));
  Wire.endTransmission(); //release BUS
}

void sendLuxLowerBound(float lowerBound){
  uint16_t luxLowerBound = mapfloat(lowerBound, 0, MAXLUX, 0, 65536);
  byte buf[4];
  buf[0] = own_addr;
  buf[1] = 4;  //message type
  buf[2] = luxLowerBound >> 8;
  buf[3] = luxLowerBound & 0xFF;
  Wire.beginTransmission(0);//get BUS
  Wire.write(buf, sizeof(buf));
  Wire.endTransmission(); //release BUS
}

void sendLuxBackground(float background){
  uint16_t luxBackground = mapfloat(background, 0, MAXLUX, 0, 65536);
  byte buf[4];
  buf[0] = own_addr;
  buf[1] = 5;  //message type
  buf[2] = luxBackground >> 8;
  buf[3] = luxBackground & 0xFF;
  Wire.beginTransmission(0);//get BUS
  Wire.write(buf, sizeof(buf));
  Wire.endTransmission(); //release BUS
}

void sendPWMRef(float pwmReff){
  uint8_t pwmRef = mapfloat(pwmReff, 0, 5, 0, 255);
  byte buf[3];
  buf[0] = own_addr;
  buf[1] = 6;
  buf[2] = pwmRef;
  Wire.beginTransmission(0);//get BUS
  Wire.write(buf, sizeof(buf));
  Wire.endTransmission(); //release BUS
}

void sendNegotiationState(uint8_t state){
  byte buf[3];
  Serial.println();
  buf[0] = own_addr;
  buf[1] = 7;
  buf[2] = state;
  Wire.beginTransmission(0);//get BUS
  Wire.write(buf, sizeof(buf));
  Wire.endTransmission(); //release BUS
}
