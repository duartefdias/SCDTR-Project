const int raspberryPiAddress = 0;

void sendInitialLuxValue(uint16_t luxValue){
  Wire.beginTransmission(raspberryPiAddress);//get BUS
  Wire.write(address);
  messageType = 0;
  Wire.write(messageType);
  // Sum the 2 bytes together to get lux value
  // ToDo: Find a better way for doing this
  Wire.write(luxValue/2);
  Wire.endTransmission(); //release BUS
}

void sendPeriodicalLuxValue(uint16_t luxValue){
  Wire.beginTransmission(raspberryPiAddress);//get BUS
  Wire.write(address);
  messageType = 1;
  Wire.write(messageType);
  // Sum the 2 bytes together to get lux value
  // ToDo: Find a better way for doing this
  Wire.write(luxValue/2);
  Wire.endTransmission(); //release BUS
}

void sendInitialPwmValue(uint16_t pwmValue){
  Wire.beginTransmission(raspberryPiAddress);//get BUS
  Wire.write(address);
  messageType = 2;
  Wire.write(messageType);
  Wire.write(pwmValue); //send byte to address on BUS
  Serial.print("Sent pwm value to RPI: ");
  Serial.println(pwmValue);
  Wire.endTransmission(); //release BUS
}

void sendPeriodicalPwmValue(uint16_t pwmValue){
  Wire.beginTransmission(raspberryPiAddress);//get BUS
  Wire.write(address);
  messageType = 3;
  Wire.write(messageType);
  Wire.write(pwmValue); //send byte to address on BUS
  Serial.print("Sent pwm value to RPI: ");
  Serial.println(pwmValue);
  Wire.endTransmission(); //release BUS
}
