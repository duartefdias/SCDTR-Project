#ifndef constants_h
#define constants_h

// Constants
const int LED1 = 5; // LED connected to analog pin5
const int LDRpin = A5; // LDR connected to A5 pin
const int Vcc = 5;
const int LowValue = 40;  // Empty desk illuminance in Lux
const int HighValue = 80; // Occupied desk illuminance

// LDR parameters
const float C = 185.922;
const float m = -0.4906;

/// System parameters: Gain and Time constant function of illuminance x [ARDUINO 2]
// G0(x) = mG1*x + bG1
// Tau(x) = mtau1*x + btau1
const float mG1 = 0.1227;
const float bG1 = 13.3269;
const double mtau1 = -0.00009;
const double btau1 = 0.0237;


// PI Controller parameters
const float Kp = 3;
const float Ki = 12;


// Sampling Period
const double T = 0.005;

#endif