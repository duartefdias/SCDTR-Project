//MASTER.INO
#include <Wire.h>
const int other_add = 4; //other dev address

void setup() {
  Serial.begin(9600);
  Wire.begin(); // join as a master
}

void loop() {
  Wire.requestFrom(other_add, 6);

  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }

  delay(500);  
}
