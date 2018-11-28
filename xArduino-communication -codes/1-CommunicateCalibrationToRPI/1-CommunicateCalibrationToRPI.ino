//MULTI_MASTER.INO
// NEEDS DEBUGGING
// Allows a node to work both as master and slave
#include <Wire.h>
#include <EEPROM.h>

const int other_add = 2; //broadcast address
uint8_t own_address; //this dev address
char buff[8]; 
String c = "";

//uint8_t = [0, 255]
uint8_t raspberryPiAddress = 0;
uint8_t otherArduinoAddress = 1; // 1 or 2
uint8_t messageType = 0; // 1 to 4
uint16_t luxValue = 0; // 1 to 510
uint8_t pwmValue = 0; // 1 to 255

void setup() {
  Serial.begin(9600);
  own_address = EEPROM.read(0); // Arduino's address is enconded in address 0 of EEPROM memory
  Serial.print("Connected, my adress is: ");
  Serial.println(own_address);

  // Define address of other arduino
  if(own_address == 1){
    otherArduinoAddress = 2;
  }
  else if(own_address == 2){
    otherArduinoAddress = 1;
  }
    
  Wire.begin(own_address); // join as a master
  TWAR = (own_address << 1)|1; // enable broadcasts to be received
  Wire.onReceive(receiveEvent); //event handler
}

void loop() {
  sendInitialLuxValue(125);
  sendPeriodicalLuxValue(340);
  sendInitialPwmValue(25);
  sendPeriodicalPwmValue(255);
  delay(1500);
}

void receiveEvent(int howMany) {
  while(Wire.available() > 0) { //check data on BUS
    //Wire.readBytes(buff, 8);
    char c = Wire.read(); //receive byte at I2C BUS
    //Serial.println(buff); // echo on terminal
  }
}

void sendInitialLuxValue(uint16_t luxValue){
  Wire.beginTransmission(raspberryPiAddress);//get BUS
  Wire.write(own_address);
  messageType = 0;
  Wire.write(messageType);
  // Sum the 2 bytes together to get lux value
  // ToDo: Find a better way for doing this
  Wire.write(luxValue/2);
  Wire.endTransmission(); //release BUS
}

void sendPeriodicalLuxValue(uint16_t luxValue){
  Wire.beginTransmission(raspberryPiAddress);//get BUS
  Wire.write(own_address);
  messageType = 1;
  Wire.write(messageType);
  // Sum the 2 bytes together to get lux value
  // ToDo: Find a better way for doing this
  Wire.write(luxValue/2);
  Wire.endTransmission(); //release BUS
}

void sendInitialPwmValue(uint16_t pwmValue){
  Wire.beginTransmission(raspberryPiAddress);//get BUS
  Wire.write(own_address);
  messageType = 2;
  Wire.write(messageType);
  Wire.write(pwmValue); //send byte to address on BUS
  Serial.print("Sent pwm value to RPI: ");
  Serial.println(pwmValue);
  Wire.endTransmission(); //release BUS
}

void sendPeriodicalPwmValue(uint16_t pwmValue){
  Wire.beginTransmission(raspberryPiAddress);//get BUS
  Wire.write(own_address);
  messageType = 3;
  Wire.write(messageType);
  Wire.write(pwmValue); //send byte to address on BUS
  Serial.print("Sent pwm value to RPI: ");
  Serial.println(pwmValue);
  Wire.endTransmission(); //release BUS
}
