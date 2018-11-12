//SLAVE.INO
#include <Wire.h>
const int own_address = 4; //this dev addres
int count = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(own_address); // receive data
  Wire.onRequest(requestEvent); //event handler
}

void loop() {
  delay(100);
}

void requestEvent() {
  Wire.write("hello ");
}
