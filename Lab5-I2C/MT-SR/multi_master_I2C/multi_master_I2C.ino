//MULTI_MASTER.INO
// NEEDS DEBUGGING
// Allows a node to work both as master and slave
#include <Wire.h>
#include <I2C.h>
#include <EEPROM.h>

uint8_t other_add = 0; //broadcast address
int own_address; //this dev address
char buff[8]; 
String c = "";
char buf[8];


void setup() {
  Serial.begin(115200);
  own_address = EEPROM.read(0); // Arduino's address is enconded in address 0 of EEPROM memory
  Serial.print("Connected, my adress is: ");
  Serial.println(own_address);

  //Send
  I2c.begin();
  I2c.timeOut(200);
  //I2c.scan();
  //receive
  Wire.begin(own_address); // join as a master
  TWAR = (own_address << 1)|1; // enable broadcasts to be received
  Wire.onReceive(receiveEvent); //event handler
}

void loop() {
  // Master operation: read from serial and send to other user
  if(Serial.available() > 0 ) {
    c = Serial.readString();
    c.toCharArray(buf,8);

    I2c.write(other_add, 0x00, buf);
  }
}

void receiveEvent(int howMany) {
  Serial.print("received: ");
  while(Wire.available() > 0) { //check data on BUS
    char c = Wire.read(); //receive byte at I2C BUS
    Serial.print(c); // echo on terminal
  }
}
