#ifndef constants_h
#define constants_h

// Constants
const int LED1 = 5; // LED connected to analog pin5
const int LDRpin = A0; // LDR connected to A5 pin
const int Vcc = 5;
int LowValue = 20;  // Empty desk illuminance in Lux
int HighValue = 100; // Occupied desk illuminance

// LDR parameters
float C = 25.8439;
float m = -0.4934;

/// System parameters: Gain and Time constant function of illuminance x [ARDUINO 1]
// G0(x) = mG1*x + bG1
// Tau(x) = mtau1*x + btau1
float mG = 0.1001;
float bG = 44.9773;
double mtau = -0.00001449;
double btau = 0.0119;


// PI Controller parameters
float Kp = 3;
float Ki = 12;


// Sampling Period
const double T = 0.005;

#endif
