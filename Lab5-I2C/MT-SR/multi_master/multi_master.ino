//MULTI_MASTER.INO
// NEEDS DEBUGGING
// Allows a node to work both as master and slave
#include <Wire.h>
#include <EEPROM.h>

const int other_add = 2; //broadcast address
int own_address; //this dev address
char buff[8]; 
String c = "";
char buf[8];


void setup() {
  Serial.begin(9600);
  own_address = EEPROM.read(0); // Arduino's address is enconded in address 0 of EEPROM memory
  Serial.print("Connected, my adress is: ");
  Serial.println(own_address);
    
  Wire.begin(own_address); // join as a master
  TWAR = (own_address << 1)|1; // enable broadcasts to be received
  Wire.onReceive(receiveEvent); //event handler
}

void loop() {
  // Master operation: read from serial and send to other user
  if(Serial.available() > 0 ) {
    c = Serial.readString();
    c.toCharArray(buf,4);
    
    Wire.beginTransmission(other_add);//get BUS
    Wire.write(buf); //send byte to address on BUS
    Serial.println(buf);
    Wire.endTransmission(); //release BUS
    Wire.beginTransmission(0);//get BUS
    Wire.write(buf); //send byte to address on BUS
    Serial.println(buf);
    Wire.endTransmission(); //release BUS 
  }
}

void receiveEvent(int howMany) {
  while(Wire.available() > 0) { //check data on BUS
    Wire.readBytes(buff, 8);
    //char c = Wire.read(); //receive byte at I2C BUS
    Serial.println(buff); // echo on terminal
  }
}
