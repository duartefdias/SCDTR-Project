#ifndef constants_h
#define constants_h

// Physical System Parameters
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

#endif
