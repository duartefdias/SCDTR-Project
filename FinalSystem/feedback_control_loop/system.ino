
void setConstants(int addr) {
  if (addr == 1) {
    // Constants
    LowValue = 20;  // Empty desk illuminance in Lux
    HighValue = 100; // Occupied desk illuminance
    
    // LDR parameters
    C = 25.8439;
    m = -0.4934;
    
    /// System parameters: Gain and Time constant function of illuminance x [ARDUINO 1]
    mG = 0.1001;
    bG = 44.9773;
    mtau = -0.00001449;
    btau = 0.0119;
    
    // PI Controller parameters
    Kp = 3;
    Ki = 12;
  }
  else if (addr==2) {
    // Ref Values
    LowValue = 20;  // Empty desk illuminance in Lux
    HighValue = 80; // Occupied desk illuminance
    
    // LDR parameters
    C = 185.922;
    m = -0.4906;
    
    /// System parameters: Gain and Time constant function of illuminance x [ARDUINO 2]
    // G0(x) = mG*x + bG
    // Tau(x) = mtau*x + btau
    mG = 0.1227;
    bG = 13.3269;
    mtau = -0.00009;
    btau = 0.0237;
    
    // PI Controller parameters
    Kp = 3;
    Ki = 12;
  }
}

// Returns system gain for desired illuminance x
float G0(float x) {
  return mG*x + bG;
}

// Returns the time constant for desired illuminance x
double tau(float x) {
  return mtau*x + btau;
}

// Reads illuminance value from sensor, returns value in Lux
float readLDR() {
  // Read LDR voltage
  int LDR = analogRead(A0);
  float Vi = mapfloat(LDR, 0, 1023, 0, 5);

  // Convert voltage into Lux
  float base = (Vcc / Vi - 1);
  float exponent = 1 / m;
  float LuxValue = C * pow(base, exponent);
  return LuxValue;
}



// Converts value in Lux to dimming level [0-5 Volt]
float lux2volt(float lux) {
  float base = lux/C;
  float exponent = m;
  return Vcc/(pow(base, exponent) + 1);
}