#ifndef globals_h
#define globals_h


// System variables
const int Nodes = 2;
uint8_t own_addr = EEPROM.read(0);
float refValue = 0; // Desired illuminance in Lux
volatile bool flag = 0;
int occupancy = 1;
float noise = 0;
int energyCost = 1;

// Structs
struct solution {
  float d[Nodes];
  float cost;
};


// I/O variables
int input;
int LowValue = 20;  // Empty desk illuminance in Lux
int HighValue = 100; // Occupied desk illuminance

// Negotiation variables
bool consensus = false;
volatile int Negotiation = 0;
volatile bool ReceivedSolution = false;  // flag
volatile struct solution other_solution;

// Constants
const int LED1 = 5; // LED connected to analog pin5
const int LDRpin = A0; // LDR connected to A5 pin
const int Vcc = 5;
const double T = 0.005; // Sampling Period
float ki[Nodes] = {0};

float k11 = 70, k12 = 2.87, k21 = 2.80, k22 = 30; // Illuminance gains and cross-gains  

#endif
