#include <Wire.h>
#include <EEPROM.h>
 
uint8_t own_addr;
uint8_t rcAdress, rcMessageType, rcPwmValue;
uint16_t rcLuxValue = 0;

own_addr = EEPROM.read(0);
 
void setup() {
    Wire.begin(own_addr);
    TWAR = (own_addr << 1) | 1;  // enable broadcasts to be received
    Wire.onReceive(receiveEvent);   // set up receive handler
}
 
void loop() {
  sendPeriodicalLuxValue(340);
  sendPeriodicalPwmValue(255);
  delay(1500);
}

void receiveEvent(int howMany) {
  // we are expecting 3 or 4 bytes, so check we got them
  if (howMany == 3 || howMany == 4) {
     rcAdress = Wire.read();
     rcMessageType = Wire.read();
     if(rcMessageType == 0) {
        rcPwmValue = Wire.read();
     }
     else {
        rcLuxValue = Wire.read();
        rcLuxValue <<= 8;
        rcLuxValue |= Wire.read();       
     }

  //use values to do something
  
  }
// throw away any garbage
while (Wire.available() > 0) 
  Wire.read();    
}


void sendPeriodicalLuxValue(uint16_t luxValue){
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_address);
  uint8_t messageType = 0;
  Wire.write(messageType);
  uint8_t luxValueH = luxValue >> 8;
  uint8_t luxValueL = luxValue & 0xFF;
  Wire.write(luxValueH);
  Wire.write(luxValueL);
  Wire.endTransmission(); //release BUS
}

void sendPeriodicalPwmValue(uint8_t pwmValue){
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_address);
  uint8_t messageType = 1;
  Wire.write(messageType);
  Wire.write(pwmValue);
  Wire.endTransmission(); //release BUS
}
