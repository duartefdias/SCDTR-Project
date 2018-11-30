#include <Wire.h>
#include <EEPROM.h>
#define MAXLUX 500
 
uint8_t own_addr = EEPROM.read(0);
uint8_t rcAddress, rcMessageType, rcPwmValue, rcPwmNegotiation, rcOccupancy;
uint16_t rcLuxValue = 0, rcLuxLowerBound = 0, rcLuxBackground = 0, rcLuxRef = 0;
float rcPwm = 0, rcPwmN = 0, rcLux=0;

float mapfloat(double val, double in_min, double in_max, double out_min, double out_max) {
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  Wire.begin(own_addr);
  TWAR = (own_addr << 1) | 1;  // enable broadcasts to be received
  Wire.onReceive(receiveEvent);   // set up receive handler
}
 
void loop() {
  sendLuxReading(100.2);
  delay(1000);
  sendPwm(4.12);
  delay(1000);
  sendNegotiation(4.20);
  delay(1000);
  sendOccupancy(0);
  delay(1000);
  sendLuxLowerBound(100);
  delay(1000);
  sendLuxBackground(0.12);
  delay(1000);
  sendLuxRef(100);
  delay(1500);
}

void receiveEvent(int howMany) {  
  // we are expecting 3 or 4 bytes, so check we got them
  if (howMany == 3 || howMany == 4) {
     rcAddress = Wire.read();
     rcMessageType = Wire.read();
    
     //pwm
     if(rcMessageType == 0) {
        rcPwmValue = Wire.read();
        rcPwm = mapfloat(rcPwmValue, 0, 255, 0, 5);
     }
     //lux
     else if(rcMessageType == 1) {
        rcLuxValue = Wire.read();
        rcLuxValue <<= 8;
        rcLuxValue |= Wire.read();
        rcLux = mapfloat(rcLuxValue, 0, 65536, 0, MAXLUX);
     }
     //pwm negotiation
     else if(rcMessageType == 2) {
        rcPwmNegotiation= Wire.read();
        rcPwm = mapfloat(rcPwmNegotiation, 0, 255, 0, 5);
     }
     //occupancy
     else if(rcMessageType == 3) {
        rcOccupancy = Wire.read();
     }
     //lux lower bound
     else if(rcMessageType == 4) {
        rcLuxLowerBound = Wire.read();
        rcLuxLowerBound <<= 8;
        rcLuxLowerBound |= Wire.read();
        rcLux = mapfloat(rcLuxLowerBound, 0, 65536, 0, MAXLUX);
     }
     //background lux
     else if(rcMessageType == 5) {
        rcLuxBackground = Wire.read();
        rcLuxBackground <<= 8;
        rcLuxBackground |= Wire.read();
        rcLux = mapfloat(rcLuxBackground, 0, 65536, 0, MAXLUX);
     }
     //reference lux
     else if(rcMessageType == 6) {
        rcLuxRef = Wire.read();
        rcLuxRef <<= 8;
        rcLuxRef |= Wire.read();
        rcLux = mapfloat(rcLuxRef, 0, 65536, 0, MAXLUX);
     }  
    //use values to do something  
  }
  // throw away any garbage
  while (Wire.available() > 0) 
    Wire.read();    
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

void sendNegotiation(float pwmValue){
  uint8_t pwmNegotiation = mapfloat(pwmValue, 0, 5, 0, 255);
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_addr);
  Wire.write(2);  //message type
  Wire.write(pwmNegotiation);
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

void sendLuxRef(float luxReff){
  uint16_t luxRef = mapfloat(luxReff, 0, MAXLUX, 0, 65536);
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_addr);
  Wire.write(6);  //message type
  uint8_t luxH = luxRef >> 8;
  uint8_t luxL = luxRef & 0xFF;
  Wire.write(luxH);
  Wire.write(luxL);
  Wire.endTransmission(); //release BUS
}
