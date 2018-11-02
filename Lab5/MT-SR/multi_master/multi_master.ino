//MULTI_MASTER.INO
// Allows a node to work both as master and slave
#include <Wire.h>
const int other_add = 0; //broadcast address
const int own_address = 6; //this dev address


void setup() {
  Serial.begin(9600);
  Wire.begin(own_address); // join as a master
  TWAR = (own_address << 1)|1; // enable broadcasts to be received
  Wire.onReceive(receiveEvent); //event handler

}

void loop() {
  char c;

  // Master operation: read from serial and send to other user
  if(Serial.available() > 0 ) {
    c = Serial.read();
    //Serial.write(c);
    Wire.beginTransmission(other_add);//get BUS
    Wire.write(c); //send byte to address on BUS
    Wire.endTransmission(); //release BUS 
  }
}

void receiveEvent(int howMany) {
  while(Wire.available() > 0) { //check data on BUS
    char c = Wire.read(); //receive byte at I2S BUS
    Serial.write(c); // echo on terminal
  }
}
