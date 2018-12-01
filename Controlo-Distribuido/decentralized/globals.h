#ifndef globals_h
#define globals_h

struct solution {
  float d[2];
  float cost;
};

uint8_t own_addr = EEPROM.read(0);

float rho = 0.07;

bool consensus = false;
int Negotiation = 0;
bool ReceivedSolution = false;  // flag
int occupancy = 0;

struct solution other_solution;

int L1 = 200, L2 = 100;  // Illuminance requirements
int c1 = 1, c2 = 2;
float o1 = 0.12, o2 = 0.08; // Ambient noise
float k11 = 70, k12 = 2.87, k21 = 2.80, k22 = 30; // Illuminance gains and cross-gains
float K[][2] = {
  {k11,k12},
  {k21,k22}
};
  
Node my_node(-1,-1,-1,-1,-1,-1);

#endif
