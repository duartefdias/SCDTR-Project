#ifndef constants_h
#define constants_h

// ARDUINO 1 Constants
const int LED1 = 5; // LED connected to analog pin5
const int LDRpin = A5; // LDR connected to A5 pin
const int Vcc = 5;
const int LowValue = 50;  // Empty desk illuminance in Lux
const int HighValue = 100; // Occupied desk illuminance

// LDR parameters
const float C = 25.8439;
const float m = -0.4934;

/// System parameters: Gain and Time constant function of illuminance x [ARDUINO 1]
// G0(x) = mG1*x + bG1
// Tau(x) = mtau1*x + btau1
const float mG1 = 0.1001;
const float bG1 = 44.9773;
const double mtau1 = -0.00001449;
const double btau1 = 0.0119;


// PI Controller parameters
const float Kp = 3;
const float Ki = 12;


// Sampling Period
const double T = 0.005;

#endif
