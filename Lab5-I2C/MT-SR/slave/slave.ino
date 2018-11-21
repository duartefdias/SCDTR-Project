//SLAVE.INO
#include <Wire.h>
#include <EEPROM.h>
//const int own_address = 4; //this dev address
int own_address; //this dev address

void setup() {
  Serial.begin(9600);
  Wire.begin(own_address); // receive data
  Wire.onReceive(receiveEvent); //event handler

  own_address = EEPROM.read(0); // Arduino's address is enconded in address 0 of EEPROM memory
  Serial.print("Connected, my adress is: ");
  Serial.println(own_address);
}

void loop() {
  // EMPTY: all the work is done in receiveEvent
}

void receiveEvent(int howMany) {
  while(Wire.available() > 0) { //check data on BUS
    char c = Wire.read(); //receive byte at I2S BUS
    Serial.println(c); // echo on terminal
  }
}
