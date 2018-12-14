#ifndef globals_h
#define globals_h

struct solution {
  float d[2];
  float cost;
};

uint8_t own_addr = EEPROM.read(0);

float rho = 0.07;

bool consensus = false;
volatile int Negotiation = 0;
volatile bool ReceivedSolution = false;  // flag
int occupancy = 1;

volatile struct solution other_solution;

int L1 = 100, L2 = 80;  // Initial illuminance requirements
int c1 = 1, c2 = 2;
float o1 = 0.12, o2 = 0.08; // Ambient noise
float k11 = 70, k12 = 2.87, k21 = 2.80, k22 = 30; // Illuminance gains and cross-gains
  
Node my_node(-1,-1,-1,-1,-1,-1);

#endif
