#include <EEPROM.h>

/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int addr = 0;
int val = 4; // value to write on the EEPROM, should be the Arduino's own address
byte value; 

void setup() {
  Serial.begin(9600);
  //EEPROM.write(addr, val); // Write to the EEPROM
}

void loop() {
  value = EEPROM.read(addr);  // Read from the EEPROM
  Serial.print(addr);
  Serial.print("\t");
  Serial.print(value, DEC);
  Serial.println();
  delay(500);
}
