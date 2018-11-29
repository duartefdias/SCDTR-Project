#include <Wire.h>
#include <EEPROM.h>
 
uint8_t own_addr = EEPROM.read(0);;
uint8_t rcAdress, rcMessageType, rcPwmValue, rcPwmNegotiation;
uint16_t rcLuxValue = 0;

 
void setup() {
    Serial.begin(9600);
    Wire.begin(own_addr);
    TWAR = (own_addr << 1) | 1;  // enable broadcasts to be received
    Wire.onReceive(receiveEvent);   // set up receive handler
}
 
void loop() {
  sendPeriodicalLuxValue(100);
  sendPeriodicalPwmValue(255);
  sendPeriodicalNegotioation(200);
  delay(1500);
}

void receiveEvent(int howMany) {
  // we are expecting 3 or 4 bytes, so check we got them
  if (howMany == 3 || howMany == 4) {
     rcAdress = Wire.read();
     rcMessageType = Wire.read();
     if(rcMessageType == 0) {
        rcPwmValue = Wire.read();
        rcPwmValue = map(rcPwmValue, 0, 255, 0, 5);
        Serial.print("Received PWM value from ");
        Serial.println(rcAdress);
        Serial.print("PWM value: ");
        Serial.println(rcPwmValue);
        Serial.println();
     }
     else if(rcMessageType == 1) {
        rcLuxValue = Wire.read();
        rcLuxValue <<= 8;
        rcLuxValue |= Wire.read();
        Serial.print("Received Lux value from ");
        Serial.println(rcAdress);
        Serial.print("LUX value: ");
        Serial.println(rcLuxValue);
        Serial.println();       
     }
     else if(rcMessageType == 2) {
        rcPwmNegotiation= Wire.read();
        Serial.print("Received PWM negotiation from ");
        Serial.println(rcAdress);
        Serial.print("PWM negotiation: ");
        Serial.println(rcPwmNegotiation);
        Serial.println();       
     }

  //use values to do something
  
  }
// throw away any garbage
while (Wire.available() > 0) 
  Wire.read();    
}


void sendPeriodicalPwmValue(uint8_t pwmValue){
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_addr);
  uint8_t messageType = 0;
  Wire.write(messageType);
  Wire.write(pwmValue);
  Wire.endTransmission(); //release BUS
}

void sendPeriodicalLuxValue(uint16_t luxValue){
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_addr);
  uint8_t messageType = 1;
  Wire.write(messageType);
  uint8_t luxValueH = luxValue >> 8;
  uint8_t luxValueL = luxValue & 0xFF;
  Wire.write(luxValueH);
  Wire.write(luxValueL);
  Wire.endTransmission(); //release BUS
}

void sendPeriodicalNegotioation(uint8_t pwmNegotiation){
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_addr);
  uint8_t messageType = 2;
  Wire.write(messageType);
  Wire.write(pwmNegotiation);
  Wire.endTransmission(); //release BUS
}
