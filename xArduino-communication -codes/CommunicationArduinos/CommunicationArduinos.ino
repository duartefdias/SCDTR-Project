#include <Wire.h>
#include <EEPROM.h>
 
uint8_t own_addr = EEPROM.read(0);
uint8_t rcAddress, rcMessageType, rcPwmValue;
uint16_t rcLuxValue = 0;

 
void setup() {
    Serial.begin(9600);
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
     rcAddress = Wire.read();
     rcMessageType = Wire.read();
     // Received Lux reading
     if(rcMessageType == 1) {
        rcPwmValue = Wire.read();
        rcPwmValue = map(rcPwmValue, 0, 255, 0, 5);
        Serial.print("Received PWM value from ");
        Serial.println(rcAddress);
        Serial.print("PWM value: ");
        Serial.println(rcPwmValue);
        Serial.println();
     }
     else {
        rcLuxValue = Wire.read();
        rcLuxValue <<= 8;
        rcLuxValue |= Wire.read();
        Serial.print("Received Lux value from ");
        Serial.println(rcAddress);
        Serial.print("Lux value: ");
        Serial.println(rcLuxValue);
        Serial.println();       
     }

  //use values to do something
  
  }
// throw away any garbage
  while (Wire.available() > 0) 
    Wire.read();    
}


void sendPeriodicalLuxValue(uint16_t luxValue){
  Wire.beginTransmission(0);//get BUS
  Wire.write(own_addr);
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
  Wire.write(own_addr);
  uint8_t messageType = 1;
  Wire.write(messageType);
  Wire.write(pwmValue);
  Wire.endTransmission(); //release BUS
}
